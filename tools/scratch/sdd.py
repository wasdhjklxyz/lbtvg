# sdd.py: every RTTI class's vtable slot 0 (its scalar/vector deleting dtor),
# the dtor it calls and the annotation state of both (F/S/Y=SYNTHETIC/-).
import re,glob,struct,pefile
pe=pefile.PE('orig/LEGOBatman.exe');img=pe.get_memory_mapped_image();B=0x400000
ann={}
for f in glob.glob('src/**/*.c*',recursive=True):
    t=open(f,errors='ignore').read()
    for m in re.finditer(r'//\s*(FUNCTION|STUB|SYNTHETIC):\s*LEGOBATMAN\s+0x([0-9a-fA-F]+)',t):
        ann[int(m.group(2),16)]=(m.group(1)[0].replace('S','S') if m.group(1)!='SYNTHETIC' else 'Y',f.split('src/')[-1])
def res(v):
    o=v-B
    if 0<=o<len(img)-5 and img[o]==0xe9: return v+5+struct.unpack_from('<i',img,o+1)[0]
    return v
seen=set()
for m in re.finditer(rb'\.\?AV([\w@]+?)@@\0',img):
    name=m.group(1).decode(); td=B+m.start()-8
    p=struct.pack('<I',td); j=img.find(p)
    while j!=-1:
        if struct.unpack_from('<I',img,j-12)[0]==0:
            q=struct.pack('<I',B+j-12); k=img.find(q)
            while k!=-1:
                s0=res(struct.unpack_from('<I',img,k+4)[0])
                if 0x401000<=s0<0x800000 and s0 not in seen:
                    seen.add(s0); o=s0-B; b=img[o:o+40]
                    kind='?'; dt=0
                    if b[:4]==bytes.fromhex('568bf1e8') and b[8:13]==bytes.fromhex('f644240801'):
                        kind='sdd'; dt=res(s0+8+struct.unpack_from('<i',b,4)[0])
                    elif b'\xf6\x44\x24\x04\x02' in b[:12] or b'\xf6\x44\x24\x08\x02' in b[:16]: kind='vdd'
                    elif b[:5]==bytes.fromhex('f644240401'): kind='sdd-inl'
                    a0=ann.get(s0,('-',''));ad=ann.get(dt,('-',''))
                    print(f'{s0:08x} {kind:7} {a0[0]} dtor {dt:08x} {ad[0]} {name:30} {ad[1] or a0[1]}')
                k=img.find(q,k+1)
        j=img.find(p,j+1)
