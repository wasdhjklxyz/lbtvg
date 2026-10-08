#!/usr/bin/env python3
"""Compile annotated source and compare each function's bytes with the original.

    tools/match.py [-v] [ADDR ...]

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

ROOT = Path(__file__).resolve().parent.parent
EXE = ROOT / "orig/LEGOBatman.exe"
SRC = ROOT / "src"
BUILD = ROOT / "build"
VC8 = Path(os.environ.get("VC8", ROOT / "toolchain/vc8"))
WINSDK6 = Path(os.environ.get("WINSDK6", ROOT / "toolchain/winsdk6"))
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
        f'/I"Z:{VC8 / "INCLUDE"}"', f'/I"Z:{WINSDK6 / "Include"}"',
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
    """{demangled: (bytes, {reloc offsets})} for every function symbol."""
    o = lief.COFF.parse(str(obj))
    secs = list(o.sections)
    syms = [s for s in o.symbols if s.type == 32 and s.section_idx > 0]
    dem = demangle([s.name for s in syms])
    out = {}
    for s in syms:
        sec = secs[s.section_idx - 1]
        data = bytes(sec.content)[s.value:]
        relocs = {r.address - s.value for r in sec.relocations if r.address >= s.value}
        out[dem.get(s.name, s.name)] = (data, relocs)
    return out

def find(funcs, name):
    want = re.compile(r"(^|[\s:*&])" + re.escape(name) + r"\(")
    hits = [k for k in funcs if want.search(k)]
    if len(hits) != 1:
        die(f"{name}: {'no' if not hits else 'ambiguous'} symbol in .obj ({hits or list(funcs)})")
    return funcs[hits[0]], hits[0]

def mask(b, relocs):
    b = bytearray(b)
    for off in relocs:
        b[off:off + 4] = b"\0\0\0\0"
    return bytes(b)

def disasm(code, addr, relocs):
    """[(text, masked bytes)] per instruction; relocated bytes print as ??."""
    md = Cs(CS_ARCH_X86, CS_MODE_32)
    for i in md.disasm(code, addr):
        off = i.address - addr
        masked = {k for r in relocs for k in range(r, r + 4)}
        hexs = "".join("??" if off + k in masked else f"{b:02x}" for k, b in enumerate(i.bytes))
        yield (f"{i.address:08x}  {hexs:<22} {i.mnemonic} {i.op_str}", mask(bytes(i.bytes), {r - off for r in relocs if 0 <= r - off < len(i.bytes)}))

def main(argv):
    verbose = "-v" in argv
    only = {int(a, 16) for a in argv if a.startswith("0x")}
    pe = pefile.PE(str(EXE))
    base = pe.OPTIONAL_HEADER.ImageBase
    total = matched = bad = 0
    for src in sorted(list(SRC.rglob("*.cpp")) + list(SRC.rglob("*.c"))):
        ann = [(a, n) for a, n in annotations(src) if not only or a in only]
        if not ann:
            continue
        funcs = functions_in_obj(compile_tu(src))
        for addr, name in ann:
            (code, relocs), sym = find(funcs, name)
            orig = pe.get_data(addr - base, len(code))
            ok = mask(code, relocs) == mask(orig, relocs)
            total += len(code); matched += len(code) if ok else 0; bad += 0 if ok else 1
            print(f"{'MATCH' if ok else 'DIFF '} {addr:08x} {len(code):5d} B  {sym}   [{src.relative_to(ROOT)}]")
            if verbose or not ok:
                o = list(disasm(orig, addr, relocs)); c = list(disasm(code, addr, relocs))
                w = max((len(x[0]) for x in o), default=40)
                print(f"  {'ORIGINAL':<{w}}   RECOMPILED")
                for i in range(max(len(o), len(c))):
                    l, r = (o[i] if i < len(o) else ("", None)), (c[i] if i < len(c) else ("", None))
                    same = l[1] is not None and l[1] == r[1]
                    print(f"  {l[0]:<{w}} {'=' if same else '!'} {r[0]}")
    print(f"\n{matched}/{total} bytes matched, {bad} function(s) differ")
    return bad

if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
