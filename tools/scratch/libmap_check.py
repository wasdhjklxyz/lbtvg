# libmap_check.py: cross-check build/libmap.json (from libmap.py): every call/
# data relocation between mapped functions must hit the mapped address.
import sys, json, struct
sys.path.insert(0, 'tools')
import match, pefile
from pathlib import Path
pe = pefile.PE('orig/LEGOBatman.exe', fast_load=True); img = pe.get_memory_mapped_image()
m = json.load(open('build/libmap.json'))
bad = 0; checked = 0
for f, res in m.items():
    addr = {n.lstrip('_'): a for n, a, _, _ in res if a}
    # also global names from all files
    glob = {}
    for f2, r2 in m.items():
        for n, a, _, _ in r2:
            if a and not n.startswith('__'): glob.setdefault(n.lstrip('_'), a)
    funcs = match.functions_in_obj(match.compile_tu(Path(f).resolve()))
    for n, a, _, _ in res:
        if not a: continue
        data, relocs = funcs[n]
        for off, sym in relocs.items():
            t = addr.get(sym) or glob.get(sym)
            if not t: continue
            if off >= 1 and data[off-1] in (0xe8, 0xe9):
                real = a + off + 4 + struct.unpack_from('<i', img, a + off - 0x400000)[0]
            else:
                real = struct.unpack_from('<I', img, a + off - 0x400000)[0]
            checked += 1
            if real != t:
                # could be thunk/data; report
                bad += 1; print('MISMATCH', Path(f).stem, n, hex(a), 'off', off, sym, hex(t), 'exe', hex(real))
print('checked', checked, 'bad', bad)
