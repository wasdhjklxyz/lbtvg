import os
ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
import pefile,struct,re,glob
pe=pefile.PE(ROOT+'/orig/LEGOBatman.exe');img=pe.get_memory_mapped_image()
secs={s.Name.rstrip(b'\0').decode():s for s in pe.sections}
d=secs['.data']; lo=d.VirtualAddress; hi=lo+d.Misc_VirtualSize
def s_at(va):
    o=va-0x400000
    if o<0 or o>=len(img): return None
    e=img.find(b'\0',o,o+64)
    if e<=o: return None
    s=img[o:e]
    return s.decode() if all(32<c<127 for c in s) else None
mac={}
for l in open(ROOT+'/tools/symbols/mac-1.0.1.nm'):
    p=l.split()
    if len(p)==3 and p[2].endswith('P8nufpar_s'):
        m=re.match(r'__Z\d+(\w+)P8nufpar_s$',p[2])
        if m: mac.setdefault(m.group(1).lower(),[]).append(m.group(1))
done=set()
for f in glob.glob(ROOT+'/src/**/*.c*',recursive=True):
    for m in re.finditer(r'//\s*(?:FUNCTION|STUB):\s*LEGOBATMAN\s+0x([0-9a-fA-F]+)',open(f).read()): done.add(int(m.group(1),16))
out=[]
for off in range(lo,hi-8,4):
    a,b=struct.unpack('<II',img[off:off+8])
    if 0x401000<=b<0x73c8fb:
        s=s_at(a)
        if s and re.match(r'^[A-Za-z_][\w ]*$',s):
            cands=[n for k,v in mac.items() for n in v if k.endswith('_'+s.lower()) or k==s.lower()]
            out.append((off+0x400000,s,b,cands,b in done))
import collections
for o,s,b,c,dn in out:
    if c and not dn: print(hex(o),s,hex(b),c[:3])
# group runs: entries whose table offsets are spaced by a constant stride
print('=====')
runs=[];cur=[]
for e in out:
    if cur and e[0]-cur[-1][0] not in (8,12,16):
        runs.append(cur);cur=[]
    cur.append(e)
runs.append(cur)
res=[]
for r in runs:
    pre=collections.Counter(n[:len(n)-len(e[1])-1] for e in r for n in e[3] if n.lower().endswith('_'+e[1].lower()))
    if not pre: continue
    p=pre.most_common(1)[0][0]
    for o,s,b,c,dn in r:
        name=[n for n in c if n.lower()==(p+'_'+s).lower()]
        res.append((b,name[0] if name else None,s,dn,hex(r[0][0])))
with open('/tmp/kwmap.txt','w') as f:
    for b,n,s,dn,t in sorted(res,key=lambda x:x[0]):
        f.write(f'{b:08x} {n} {s} {"DONE" if dn else "todo"} {t}\n')
