import os
ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
import pefile,struct,re,glob,collections
pe=pefile.PE(ROOT+'/orig/LEGOBatman.exe');img=pe.get_memory_mapped_image()
secs={s.Name.rstrip(b'\0').decode():s for s in pe.sections}
def s_at(va):
    o=va-0x400000
    if o<0 or o>=len(img): return None
    e=img.find(b'\0',o,o+64)
    if e<=o: return None
    s=img[o:e]
    return s.decode() if all(32<c<127 for c in s) else None
mac=collections.defaultdict(list)
for l in open(ROOT+'/tools/symbols/mac-1.0.1.nm'):
    p=l.split()
    if len(p)==3 and p[1] in 'tT':
        m=re.match(r'__Z\d+(\w+?)(P|v|i|f|c)',p[2]) or re.match(r'_(\w+)$',p[2])
        if m: mac[m.group(1).lower()].append(m.group(1))
done={}
for f in glob.glob(ROOT+'/src/**/*.c*',recursive=True):
    for m in re.finditer(r'//\s*(?:FUNCTION|STUB):\s*LEGOBATMAN\s+0x([0-9a-fA-F]+)\n(?:\s*//.*\n)*\s*[^\n(]*?(\w+)\(',open(f).read()): done[int(m.group(1),16)]=m.group(2)
d=secs['.data']; lo=d.VirtualAddress+0x400000; hi=lo+d.Misc_VirtualSize
r=secs['.rdata']; rlo=r.VirtualAddress+0x400000; rhi=rlo+r.Misc_VirtualSize
res=[]
for base in (lo,rlo):
  top = hi if base==lo else rhi
  for off in range(base,top-8,4):
    a,=struct.unpack('<I',img[off-0x400000:off-0x400000+4])
    s=s_at(a)
    if not s or not re.match(r'^[A-Za-z_][\w]*$',s): continue
    for k in (4,8):
        b,=struct.unpack('<I',img[off-0x400000+k:off-0x400000+k+4])
        if 0x401000<=b<0x73c8fb:
            for pre in ('Condition_','Action_','Condition_','Message_','Locator_'):
                pass
            cands=[n for n in mac.get(('condition_'+s).lower(),[])+mac.get(('action_'+s).lower(),[])+mac.get(('condition_'+s+'init').lower(),[])]
            res.append((b,s,off,k,cands,done.get(b)))
for b,s,off,k,c,dn in sorted(set((x[0],x[1],x[2],x[3],tuple(x[4]),x[5]) for x in res)):
    if c and not dn: print(f'{b:08x} {s} tab={off:x}+{k} cands={list(c)}')
