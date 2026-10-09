#!/usr/bin/env python3
"""tools/disasm.py ADDR [ADDR ...]: disassemble functions from orig/LEGOBatman.exe.

Annotates .rdata operands with the string, or the f32/f64 value when it is
not a string, and call targets with their Mac name (tools/symbols/pc-names.csv)
or the annotated src/ name. Faster than ghidra for "what does this compile to".
"""
import csv, re, struct, sys
from pathlib import Path
import pefile
from capstone import Cs, CS_ARCH_X86, CS_MODE_32

ROOT = Path(__file__).resolve().parent.parent
pe = pefile.PE(str(ROOT / "orig/LEGOBatman.exe"))
base = pe.OPTIONAL_HEADER.ImageBase
secs = {s.Name.rstrip(b"\0").decode(): s for s in pe.sections}
rd = secs[".rdata"]
rlo, rhi = base + rd.VirtualAddress, base + rd.VirtualAddress + rd.Misc_VirtualSize

sizes, names = {}, {}
for l in open(ROOT / "ghidra/LEGOBatman.exe.stats.tsv"):
    if not l.startswith("#"):
        a, s, _, n = l.rstrip("\n").split("\t")
        sizes[int(a, 16)] = int(s)
        names[int(a, 16)] = n
for r in csv.DictReader(open(ROOT / "tools/symbols/pc-names.csv")):
    names[int(r["pc_addr"], 16)] = r["demangled"]
for f in ROOT.glob("src/**/*.c*"):
    t = f.read_text()
    for m in re.finditer(r"//\s*FUNCTION:\s*LEGOBATMAN\s+0x([0-9a-fA-F]+)\n(?:\s*//.*\n)*\s*([^\n{;]+)", t):
        names[int(m.group(1), 16)] = m.group(2).strip()

def string_at(va):
    d = pe.get_data(va - base, 200)
    s = d[:d.find(b"\0")]
    return s.decode() if len(s) >= 2 and all(32 <= c < 127 for c in s) else None

md = Cs(CS_ARCH_X86, CS_MODE_32)
for arg in sys.argv[1:]:
    addr = int(arg, 16)
    size = sizes.get(addr)
    if size is None:   # ghidra missed it: up to the next known start, minus padding
        nxt = min([a for a in list(sizes) + list(names) if a > addr] or [addr + 256])
        body = pe.get_data(addr - base, min(nxt - addr, 0x4000))
        size = len(body.rstrip(b"\xcc\x90")) or len(body)
    print(f"=== {addr:08x}  {size} B  {names.get(addr, '')}")
    for i in md.disasm(pe.get_data(addr - base, size), addr):
        extra = ""
        for m in re.finditer(r"0x([0-9a-f]{6,8})", i.op_str):
            v = int(m.group(1), 16)
            if i.mnemonic == "call" and v in names:
                extra += f"  ; {names[v]}"
            elif rlo <= v < rhi:
                s = string_at(v)
                if s:
                    extra += f'  ; "{s}"'
                else:
                    d = pe.get_data(v - base, 8)
                    extra += f"  ; f32={struct.unpack('<f', d[:4])[0]!r} f64={struct.unpack('<d', d)[0]!r}"
        print(f"{i.address:08x}  {i.bytes.hex():<20} {i.mnemonic} {i.op_str}{extra}")
