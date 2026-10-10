# hdrstatics_core.py: find the per-TU out-of-line copies of small header statics
# (families F below) and map each to the src file of its TU (the next annotated
# function before the next copy cluster). Exec'd by hdrstatics_annotate.py /
# hdrstatics_newtu.py; defines copies, ann, keys, plan.
# fam.py: header-static copies (by family) and the src file of their TU.
import re,pefile,subprocess,bisect,collections,sys
pe=pefile.PE('orig/LEGOBatman.exe',fast_load=True);base=pe.OPTIONAL_HEADER.ImageBase
t=[s for s in pe.sections if s.Name.startswith(b'.text')][0];d=t.get_data();va=base+t.VirtualAddress
out=subprocess.run(['grep','-rn','-E','// (FUNCTION|STUB): LEGOBATMAN 0x','src'],capture_output=True,text=True).stdout
ann={}
for l in out.splitlines():
    f,_,rest=l.split(':',2); a=int(re.search(r'0x([0-9a-f]{8})',rest).group(1),16); ann[a]=f
keys=sorted(ann)
lim=0x73c8fb-va
F={'sin':('8b44240425ffff00003d00c00000',147),'cos':('050040000050e8',15),'fabs':('8b44240425ffffff7f89442404d9442404c3',18),
   'fdiv':('d9eed9c0d9442404dde1dfe0',54),'sign':('d9eed9442404d8d1dfe0f6c4',40),'v4set':('d9442404d918d9442408d95804d944240cd95808d9442410d9580cc3',28),
   'v4copy':('d900d919d94004d95904d94008d95908d9400cd9590cc3',23),'vscale':('d900d9442404d9c0decad9c9d919',29),
   'vsub':('d9018b542404d820d91a',29),'vadd':('d9018b542404d800d91a',29),
   'mroty':('83ec188d860040000050e8',205),'mcopy':('d900d919d94004d95904d94008d95908d9400cd9590cd94010d95910',95)}
def flen(o):
    e=o+1
    while e<lim and not (e%16==0 and d[e-1]==0xcc): e+=1
    while d[e-1]==0xcc: e-=1
    return e-o
copies=[]
for o in range(0,lim,16):
    if o and d[o-1]!=0xcc: continue
    for n,(p,L) in F.items():
        if d[o:o+len(p)//2]==bytes.fromhex(p) and flen(o)==L: copies.append((va+o,n)); break
copies.sort()
res=collections.defaultdict(list)
for i,(a,n) in enumerate(copies):
    if a in ann: continue
    k=i+1
    while k<len(copies) and copies[k][0]-copies[k-1][0]<0x300: k+=1
    limit=copies[k][0] if k<len(copies) else 0x73c8fb
    j=bisect.bisect_right(keys,a)
    f=ann[keys[j]] if j<len(keys) and keys[j]<limit else None
    # prefer the file that already holds an annotated copy of this cluster
    k0=i
    while k0>0 and copies[k0][0]-copies[k0-1][0]<0x300: k0-=1
    same=[ann[c] for c,_ in copies[k0:k] if c in ann]
    if same: f=same[-1] if any(c<a for c,_ in copies[k0:k] if c in ann) else same[0]
    res[f].append((a,n))
plan={f:l for f,l in res.items() if f}
