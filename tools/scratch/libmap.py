# libmap.py LO HI file.c...: compile third-party source files and find each
# function's exact address in [LO,HI) of the exe (relocation-masked byte
# search, 16-aligned). Writes build/libmap.json for libmap_check.py.
# Used to place libvorbis/libogg (docs/workflow.md).
import sys, os, json
sys.path.insert(0, 'tools')
import match
from pathlib import Path
import pefile
pe = pefile.PE('orig/LEGOBatman.exe', fast_load=True)
img = pe.get_memory_mapped_image()
LO, HI = int(sys.argv[1], 16), int(sys.argv[2], 16)
reg = img[LO-0x400000:HI-0x400000]
def hits(mb, relocs):
    out = []
    first = None
    for k in range(len(mb)):
        if k not in relocs and all(not (r <= k < r+4) for r in relocs): first = k; break
    for a in range(LO, HI - len(mb), 16):
        o = a - LO
        if reg[o+first] != mb[first]: continue
        seg = bytearray(reg[o:o+len(mb)])
        for r in relocs: seg[r:r+4] = b'\0\0\0\0'
        if bytes(seg) == mb: out.append(a)
    return out
out = {}
for f in sys.argv[3:]:
    obj = match.compile_tu(Path(f).resolve())
    funcs = match.functions_in_obj(obj)
    H = {}
    for name, (data, relocs) in funcs.items():
        mb = match.mask(data, relocs)
        H[name] = (hits(mb, relocs) if len(mb) > 0 else [], len(mb))
    uniq = [h[0][0] for n, h in H.items() if len(h[0]) == 1 and h[1] >= 40]
    lo, hi = (min(uniq), max(uniq)) if uniq else (0, 0)
    used = set(); res = []
    for name, (hs, n) in H.items():
        c = [a for a in hs if lo - 0x200 <= a <= hi + 0x2000 and a not in used]
        if c: used.add(c[0]); res.append((name, c[0], n, len(hs)))
        else: res.append((name, None, n, len(hs)))
    st = Path(f).stem
    print('## %s range %08x..%08x' % (st, lo, hi))
    for r in res: print('%-10s %-36s %s %d nhits=%d' % (st, r[0], '%08x' % r[1] if r[1] else '--------', r[2], r[3]), flush=True)
    out[f] = res
json.dump(out, open('build/libmap.json', 'w'))
