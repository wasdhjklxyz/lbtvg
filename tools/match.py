#!/usr/bin/env python3
"""Compile annotated source and compare each function's bytes with the original.

    tools/match.py [-v] [ADDR|NAME ...] [src/path/file.cpp ...]
    (ADDR with or without 0x; NAME = function name. Colour unless piped or NO_COLOR.)

Scans src/**/*.c, *.cpp for reccmp-style annotations:

    // FUNCTION: LEGOBATMAN 0x0058b6d0
    int SetValue(int v) { ... }

Each TU is compiled once with the VC8 flag set (cflags() below) under wibo into
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

# Per-TU exceptions, proven by matching: the libvorbis core library was built
# with /GS- (29 alloca/array functions match only that way); libvorbisfile
# (vorbisfile.c, its own library upstream) and libogg use the game's flags.
NO_GS = [("lib/libvorbis/lib", {"vorbisfile.c"})]

def cflags(src):
    """The flag set for one TU (C has no /EHsc; see NO_GS)."""
    src = Path(src).resolve()
    f = [x for x in CFLAGS if not (src.suffix == ".c" and x == "/EHsc")]
    for d, keep in NO_GS:
        if (SRC / d).resolve() in src.parents and src.name not in keep:
            f = ["/GS-" if x == "/GS" else x for x in f]
    return f

ANNOT = re.compile(r"//\s*FUNCTION:\s*" + MODULE + r"\s+0x([0-9a-fA-F]+)")
IDENT = re.compile(r"((?:[A-Za-z_~][\w~]*(?:<[^()]*>)?::)*"
                   r"(?:operator\s*(?:\(\)|[^\s(]+)|[A-Za-z_~][\w~]*)(?:<[^()]*>)?)\s*\(")
TYPEDEF = {"u8": "char", "i8": "char", "u16": "short", "i16": "short", "u32": "int",
           "i32": "int", "BOOL": "int", "u64": "__int64", "i64": "__int64",
           "f32": "float", "f64": "double"}

def untemplate(s):
    """Drop template arguments: our src and the demangler spell them differently."""
    prev = None
    while prev != s:
        prev, s = s, re.sub(r"<[^<>()]*>", "", s)
    return s
SIG = {}  # (path, addr) -> source parameter list, to tell overloads apart

def die(msg):
    print("match: " + msg, file=sys.stderr); sys.exit(2)

STUB_ANNOT = re.compile(r"//\s*STUB:\s*" + MODULE + r"\s+0x([0-9a-fA-F]+)")
# reccmp style, for compiler-generated functions with no source line: the
# name is on the annotation, as llvm-undname prints it, e.g.
#   // SYNTHETIC: LEGOBATMAN 0x005a3c30 FadeBase::`scalar deleting destructor'
SYNTH_ANNOT = re.compile(r"//\s*SYNTHETIC:\s*" + MODULE + r"\s+0x([0-9a-fA-F]+)\s+(\S.*?)\s*$")
UNDNAME = {"destructor'": "dtor'"}  # reccmp spelling -> llvm-undname spelling

def annotations(path, stubs=False):
    """[(addr, 'Class::name' or 'name')] in file order (STUBs instead, if asked)."""
    lines = path.read_text().splitlines()
    out = []
    for i, line in enumerate(lines):
        sm = None if stubs else SYNTH_ANNOT.search(line)
        if sm:
            name = sm.group(2)
            for a, b in UNDNAME.items():
                name = name.replace(a, b)
            out.append((int(sm.group(1), 16), name))
            continue
        m = (STUB_ANNOT if stubs else ANNOT).search(line)
        if not m:
            continue
        for j, nxt in enumerate(lines[i + 1:], i + 1):
            s = nxt.strip()
            if not s or s.startswith("//") or (stubs and (s.startswith("#") or "(" not in s)):
                continue
            # clang-format may put a long return type on its own line
            k = j
            while "(" not in s and k + 1 < len(lines) and k < j + 3:
                k += 1
                s += " " + lines[k].strip()
            mm = IDENT.search(s)
            if mm:
                rest = " ".join([s] + [l.strip() for l in lines[k + 1:k + 8]])
                SIG[(path, int(m.group(1), 16))] = params_of(rest[mm.end() - 1:])
            if not mm:
                if stubs:
                    break
                die(f"{path}:{i+1}: cannot find a function name after the annotation")
            out.append((int(m.group(1), 16), re.sub(r"\s+", "", untemplate(mm.group(1)))))
            break
    return out

def compile_tu(src):
    BUILD.mkdir(exist_ok=True)
    obj = BUILD / (src.relative_to(SRC).as_posix().replace("/", "__") + ".obj")
    cmd = ["wibo", str(VC8 / "Bin/cl.exe")] + cflags(src) + [
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

def params_of(text):
    """'(a, b) ...' -> canonical parameter types [(last type word, pointer count)]."""
    depth, out, cur = 0, [], ""
    for ch in text:
        if ch == "(":
            depth += 1
            if depth == 1:
                continue
        elif ch == ")":
            depth -= 1
            if depth == 0:
                break
        if ch == "," and depth == 1:
            out.append(cur); cur = ""
        else:
            cur += ch
    out.append(cur)
    sig = []
    for p in out:
        p = p.split("=")[0].strip()
        if not p or p == "void":
            continue
        words = [w for w in re.findall(r"[A-Za-z_]\w*", p) if w not in ("const", "struct", "union", "enum", "class")]
        if len(words) > 1 and not p.rstrip().endswith(("*", "&")):
            words = words[:-1]  # drop the parameter name
        w = words[-1] if words else ""
        sig.append((TYPEDEF.get(w, w), p.count("*") + p.count("&")))
    return sig

def find(funcs, name, sig=None):
    """Our function by name: a C++ symbol demangles to "... Name(...)"; a C
    (extern "C") symbol stays undecorated: _Name (cdecl), _Name@N (stdcall),
    @Name@N (fastcall)."""
    want = re.compile(r"(^|[\s:*&])" + re.escape(name) + r"\(")
    c_sym = re.compile(r"^[_@]" + re.escape(name.split("::")[-1]) + r"(@\d+)?$")
    plain = {k: untemplate(k) for k in funcs}
    hits = [k for k in funcs if want.search(plain[k]) or c_sym.match(k)]
    if len(hits) > 1 and sig is not None:  # overloads: compare parameter lists
        args = {k: params_of(plain[k][want.search(plain[k]).end() - 1:]) if want.search(plain[k]) else None
                for k in hits}
        same = [k for k in hits if args[k] is not None and len(args[k]) == len(sig)]
        exact = [k for k in same if [p for _, p in args[k]] == [p for _, p in sig]
                 and all(a == b or not a or not b for (a, _), (b, _) in zip(args[k], sig))]
        hits = exact if len(exact) == 1 else same if len(same) == 1 else hits
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
        sel = lambda a, n: a in only or n in only_names or n.split("::")[-1] in only_names
        ann = [(a, n, False) for a, n in annotations(src) if (not only and not only_names) or sel(a, n)]
        # a STUB is only test-matched when named explicitly; it never counts
        if only or only_names:
            ann += [(a, n, True) for a, n in annotations(src, stubs=True) if sel(a, n)]
        if not ann:
            continue
        funcs = functions_in_obj(compile_tu(src))
        for addr, name, is_stub in ann:
            seen += 1
            if is_stub:
                want = re.compile(r"(^|[\s:*&])" + re.escape(name) + r"\(|^[_@]" + re.escape(name.split("::")[-1]) + r"(@\d+)?$")
                if not [k for k in funcs if want.search(untemplate(k))]:
                    print(f"STUB  {addr:08x}  {name}: not compiled (still in #if 0?)   [{src.relative_to(ROOT)}]")
                    continue
            (code, relocs), sym = find(funcs, name, SIG.get((src, addr)))
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
            if is_stub:
                tag = (f"{C['g']}{C['b']}STUB-MATCH{C['x']} (change // STUB: to // FUNCTION:)" if ok
                       else f"{C['y']}{C['b']}STUB-DIFF{C['x']}")
                print(f"{tag} {addr:08x} {len(code):5d} B  {sym}   {C['d']}[{src.relative_to(ROOT)}]{C['x']}")
                if verbose or not ok:
                    show(insns(orig, addr, len(orig), names=names), insns(code, addr, len(code), relocs=relocs))
                continue
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
