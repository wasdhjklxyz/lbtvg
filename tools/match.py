#!/usr/bin/env python3
"""Compile annotated source and compare each function's bytes with the original.

    tools/match.py [-v] [ADDR|NAME ...] [src/path/file.cpp ...]
    (ADDR with or without 0x; NAME = function name. Colour unless piped or NO_COLOR.)

Scans src/**/*.c, *.cpp for reccmp-style annotations:

    // FUNCTION: LEGOBATMAN 0x0058b6d0
    int SetValue(int v) { ... }

Each TU is compiled once with the VC8 flag set (CFLAGS below) under wibo into
build/. The function is located in the .obj by demangled name, its bytes are
compared against orig/LEGOBatman.exe at the annotated address, with the 4 bytes
at every relocation masked (the .obj does not know final addresses). Exit code
is the number of mismatching functions.
"""
import os, re, subprocess, sys
from pathlib import Path
import lief, pefile
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone.x86 import X86_OP_IMM, X86_OP_MEM

ROOT = Path(__file__).resolve().parent.parent
EXE = ROOT / "orig/LEGOBatman.exe"
SRC = ROOT / "src"
BUILD = ROOT / "build"
VC8 = Path(os.environ.get("VC8", ROOT / "toolchain/vc8"))
WINSDK6 = Path(os.environ.get("WINSDK6", ROOT / "toolchain/winsdk6"))
DXSDK = Path(os.environ.get("DXSDK", ROOT / "toolchain/dxsdk"))
MODULE = "LEGOBATMAN"

# The starting flag set (docs/toolchain.md). /Gy only changes packaging: one
# COMDAT section per function, so each function's size is exact.
CFLAGS = ["/nologo", "/c", "/O2", "/Oy", "/GS", "/EHsc", "/MT", "/Gd", "/Gy", "/Z7"]

ANNOT = re.compile(r"//\s*FUNCTION:\s*" + MODULE + r"\s+0x([0-9a-fA-F]+)")
IDENT = re.compile(r"([A-Za-z_][\w:]*)\s*\(")

def die(msg):
    print("match: " + msg, file=sys.stderr); sys.exit(2)

def annotations(path):
    """[(addr, 'Class::name' or 'name')] in file order."""
    lines = path.read_text().splitlines()
    out = []
    for i, line in enumerate(lines):
        m = ANNOT.search(line)
        if not m:
            continue
        for nxt in lines[i + 1:]:
            s = nxt.strip()
            if not s or s.startswith("//"):
                continue
            mm = IDENT.search(s)
            if not mm:
                die(f"{path}:{i+1}: cannot find a function name after the annotation")
            out.append((int(m.group(1), 16), mm.group(1)))
            break
    return out

def compile_tu(src):
    BUILD.mkdir(exist_ok=True)
    obj = BUILD / (src.relative_to(SRC).as_posix().replace("/", "__") + ".obj")
    cmd = ["wibo", str(VC8 / "Bin/cl.exe")] + [f for f in CFLAGS if not (src.suffix == ".c" and f == "/EHsc")] + [
        f'/I"Z:{VC8 / "INCLUDE"}"', f'/I"Z:{DXSDK / "Include"}"', f'/I"Z:{WINSDK6 / "Include"}"',
        f'/Fo"Z:{obj}"', f'"Z:{src}"']
    # wibo wants a single guest command line; join and let it parse quotes.
    r = subprocess.run(" ".join(cmd), shell=True, capture_output=True, text=True)
    if r.returncode != 0 or not obj.exists():
        die(f"compile failed for {src}:\n{r.stdout}{r.stderr}")
    return obj

def demangle(names):
    r = subprocess.run(["llvm-undname"], input="\n".join(names) + "\n", capture_output=True, text=True)
    # llvm-undname prints "mangled\n\ndemangled\n\n" per name
    chunks = [c for c in r.stdout.split("\n\n") if c.strip()]
    out = {}
    for c in chunks:
        parts = c.strip().split("\n")
        if len(parts) >= 2:
            out[parts[0].strip()] = parts[-1].strip()
    return out

def functions_in_obj(obj):
    """{demangled: (bytes, {reloc offset: target symbol})} for every function symbol."""
    o = lief.COFF.parse(str(obj))
    secs = list(o.sections)
    syms = [s for s in o.symbols if s.type == 32 and s.section_idx > 0]
    rnames = {r.symbol.name for sec in secs for r in sec.relocations if getattr(r, "symbol", None)}
    dem = demangle([s.name for s in syms] + sorted(rnames))
    short = lambda n: re.sub(r".*?(\b[\w:~]+)\s*\(.*", r"\1", dem.get(n, n)) if n else ""
    out = {}
    for s in syms:
        sec = secs[s.section_idx - 1]
        data = bytes(sec.content)[s.value:]
        relocs = {}
        for r in sec.relocations:
            if r.address >= s.value:
                name = r.symbol.name if getattr(r, "symbol", None) else ""
                relocs[r.address - s.value] = short(name).lstrip("_")
        out[dem.get(s.name, s.name)] = (data, relocs)
    return out

def find(funcs, name):
    want = re.compile(r"(^|[\s:*&])" + re.escape(name) + r"\(")
    hits = [k for k in funcs if want.search(k)]
    if len(hits) != 1:
        die(f"{name}: {'no' if not hits else 'ambiguous'} symbol in .obj ({hits or list(funcs)})")
    return funcs[hits[0]], hits[0]

# --- comparison ------------------------------------------------------------------
def mask(b, relocs):
    b = bytearray(b)
    for off in relocs:
        b[off:off + 4] = b"\0\0\0\0"
    return bytes(b)

# For display, each side is masked on its own terms: our object at its
# relocations, the original at operands that look like addresses (memory
# displacements in the image, call/jump targets leaving the function, and
# immediates of mov/push/cmp in the image). The verdict does not use this.

MD = Cs(CS_ARCH_X86, CS_MODE_32)
MD.detail = True
IMG_LO, IMG_HI = 0x00400000, 0x02C00000

def operand_spans(i):
    """(offset, size, value) of imm/disp fields inside one instruction."""
    spans = []
    imm_off, disp_off = getattr(i, "imm_offset", 0), getattr(i, "disp_offset", 0)
    imm_sz, disp_sz = getattr(i, "imm_size", 0), getattr(i, "disp_size", 0)
    for op in i.operands:
        if op.type == X86_OP_IMM and imm_off and imm_sz == 4:
            spans.append((imm_off, 4, op.imm & 0xffffffff))
        elif op.type == X86_OP_MEM and disp_off and disp_sz == 4:
            spans.append((disp_off, 4, op.mem.disp & 0xffffffff))
    return spans

def insns(code, addr, size, relocs=None, names=None):
    """Instructions as dicts: text, masked bytes, display hex, a normalized key."""
    out = []
    for i in MD.disasm(code, addr):
        off = i.address - addr
        b = bytearray(i.bytes)
        masked = set()
        label = {}
        if relocs is not None:                      # ours: exact relocations
            for r, sym in relocs.items():
                if off <= r < off + len(b):
                    masked.update(range(r - off, min(r - off + 4, len(b))))
                    label[r - off] = sym
        else:                                       # original: address-like operands
            rel_branch = i.mnemonic == "call" or (i.mnemonic.startswith("j") and len(b) >= 5)
            for so, sz, val in operand_spans(i):
                if rel_branch and i.mnemonic == "call":
                    tgt = val
                    masked.update(range(so, so + sz)); label[so] = names.get(tgt, f"{tgt:#x}") if names else f"{tgt:#x}"
                elif rel_branch and not (addr <= val < addr + size):
                    masked.update(range(so, so + sz)); label[so] = f"{val:#x}"
                elif IMG_LO <= val < IMG_HI and not rel_branch and (
                        i.mnemonic in ("mov", "push", "cmp", "lea") or any(
                            op.type == X86_OP_MEM and op.mem.disp & 0xffffffff == val for op in i.operands)):
                    masked.update(range(so, so + sz)); label[so] = names.get(val, f"{val:#x}") if names else f"{val:#x}"
        for k in masked:
            b[k] = 0
        text = f"{i.mnemonic} {i.op_str}".strip()
        if i.mnemonic == "call" and label:
            text = f"call {next(iter(label.values())) or 'extern'}"
        key = (i.mnemonic, bytes(b))
        out.append({"addr": i.address, "raw": bytes(i.bytes), "masked": masked,
                    "text": text, "key": key, "norm": bytes(b)})
    return out

def color_on():
    if os.environ.get("NO_COLOR"):
        return False
    force = os.environ.get("FORCE_COLOR")
    if force is not None and force != "":
        return force != "0"
    return sys.stdout.isatty()

C = {}
def init_colors():
    on = color_on()
    for k, v in {"g": "32", "r": "31", "y": "33", "b": "1", "d": "2", "c": "36", "x": "0"}.items():
        C[k] = f"\033[{v}m" if on else ""

def hexcol(ins, other, side_diff):
    """Hex bytes; masked as ??, bytes that differ from the paired row in bold."""
    out = []
    for k, byte in enumerate(ins["raw"]):
        if k in ins["masked"]:
            out.append(f"{C['d']}??{C['x']}")
        elif side_diff and (other is None or k >= len(other["norm"]) or other["norm"][k] != ins["norm"][k]):
            out.append(f"{C['b']}{byte:02x}{C['x']}")
        else:
            out.append(f"{byte:02x}")
    return "".join(out), 2 * len(ins["raw"])

def show(orig, ours):
    """Side-by-side, aligned like diff."""
    import difflib
    sm = difflib.SequenceMatcher(a=[x["key"] for x in orig], b=[x["key"] for x in ours], autojunk=False)
    rows = []
    for tag, a0, a1, b0, b1 in sm.get_opcodes():
        if tag == "equal":
            rows += [("=", orig[a0 + k], ours[b0 + k]) for k in range(a1 - a0)]
        else:
            n = max(a1 - a0, b1 - b0)
            for k in range(n):
                o = orig[a0 + k] if a0 + k < a1 else None
                c = ours[b0 + k] if b0 + k < b1 else None
                rows.append(("!" if o and c else "<" if o else ">", o, c))
    w = max((len(r[1]["text"]) for r in rows if r[1]), default=20)
    w = min(max(w, 24), 46)
    print(f"  {C['b']}{'ORIGINAL':<{8 + 2 + 22 + 1 + w}}   RECOMPILED{C['x']}")
    for mark, o, c in rows:
        diff = mark != "="
        col = C["g"] if not diff else C["r"]
        def cell(ins, other):
            if ins is None:
                return " " * (8 + 2 + 22 + 1 + w)
            hx, n = hexcol(ins, other, diff)
            text = ins["text"]
            text = text[: w - 1] + "…" if len(text) > w else text
            t = f"{C['b']}{text}{C['x']}" if diff else text
            return f"{C['d']}{ins['addr']:08x}{C['x']}  {hx}{' ' * max(0, 22 - n)} {t}{' ' * (w - len(text))}"
        print(f"  {cell(o, c)} {col}{C['b']}{mark}{C['x']} {cell(c, o)}")

def load_names():
    """Original addresses -> names, for call targets and globals in the listing."""
    names = {}
    p = ROOT / "tools/symbols/pc-names.csv"
    if p.exists():
        import csv
        for r in csv.DictReader(open(p)):
            names[int(r["pc_addr"], 16)] = r["demangled"].split("(")[0].lstrip("_")
    for src in list(SRC.rglob("*.c")) + list(SRC.rglob("*.cpp")):
        for a, n in annotations(src):
            names[a] = n
    return names

def orig_sizes():
    sizes = {}
    p = ROOT / "tools/symbols/functions.tsv"
    if p.exists():
        for line in open(p):
            if not line.startswith("#"):
                a, s, _ = line.rstrip("\n").split("\t")
                sizes[int(a, 16)] = int(s)
    return sizes

def parse_selectors(argv):
    """Addresses (0x-prefixed or not) and function names to restrict the run to."""
    addrs, names = set(), set()
    for a in argv:
        if a.startswith("-") or a.endswith((".c", ".cpp")):
            continue
        if re.fullmatch(r"(0x)?[0-9a-fA-F]{5,8}", a):
            addrs.add(int(a, 16))
        else:
            names.add(a)
    return addrs, names

def main(argv):
    init_colors()
    verbose = "-v" in argv
    only, only_names = parse_selectors(argv)
    files = {Path(a).resolve() for a in argv if a.endswith((".c", ".cpp"))}
    pe = pefile.PE(str(EXE))
    base = pe.OPTIONAL_HEADER.ImageBase
    names, sizes = load_names(), orig_sizes()
    total = matched = bad = seen = 0
    for src in sorted(list(SRC.rglob("*.cpp")) + list(SRC.rglob("*.c"))):
        if files and src.resolve() not in files:
            continue
        ann = [(a, n) for a, n in annotations(src)
               if (not only and not only_names) or a in only or n in only_names or n.split("::")[-1] in only_names]
        if not ann:
            continue
        funcs = functions_in_obj(compile_tu(src))
        for addr, name in ann:
            seen += 1
            (code, relocs), sym = find(funcs, name)
            osize = sizes.get(addr)
            orig = pe.get_data(addr - base, max(len(code), osize or 0))
            o = insns(orig, addr, len(orig), names=names)
            c = insns(code, addr, len(code), relocs=relocs)
            # Verdict: bytes, masked at our relocations on both sides. Sound: any
            # layout difference shows up as unequal bytes. The display below is
            # for reading only. Original longer than ours (beyond int3/nop
            # padding, which ghidra sometimes counts) means code is missing.
            tail = orig[len(code):osize] if osize and osize > len(code) else b""
            short = any(x not in (0xCC, 0x90) for x in tail)
            ok = mask(code, relocs) == mask(orig[:len(code)], relocs) and not short
            total += len(code); matched += len(code) if ok else 0; bad += 0 if ok else 1
            tag = f"{C['g']}{C['b']}MATCH{C['x']}" if ok else f"{C['r']}{C['b']}DIFF {C['x']}"
            print(f"{tag} {addr:08x} {len(code):5d} B  {sym}   {C['d']}[{src.relative_to(ROOT)}]{C['x']}")
            if short:
                print(f"  {C['y']}original is {osize} B, yours {len(code)} B: code missing at the end{C['x']}")
            if verbose or not ok:
                show(o, c)
    if seen == 0 and (only or only_names):
        die("no annotated function matches " + " ".join(argv))
    print(f"\n{matched}/{total} bytes matched, {bad} function(s) differ")
    return bad

if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
