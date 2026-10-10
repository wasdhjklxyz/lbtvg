# tugaps.py: for each placeholder TU file (one with Unk_InlineUser_*), the
# range from its first annotated function to the next annotated function in
# another file, with the unannotated functions in it (int3-scan starts too).
# Sorted by unannotated bytes, smallest first.
import re,glob,bisect,pefile
pe=pefile.PE('orig/LEGOBatman.exe');img=pe.get_memory_mapped_image()
ann={}
for f in glob.glob('src/**/*.c*',recursive=True):
    for m in re.finditer(r'//\s*(?:FUNCTION|STUB):\s*LEGOBATMAN\s+0x([0-9a-fA-F]+)',open(f,errors='ignore').read()):
        ann[int(m.group(1),16)]=f
A=sorted(ann)
def starts(lo,hi):
    s=set()
    for l in open('tools/symbols/functions.tsv'):
        if l.startswith('#'): continue
        a=int(l.split('\t')[0],16)
        if lo<=a<hi: s.add(a)
    b=img[lo-0x400000:hi-0x400000];i=0
    while i<len(b):
        if b[i]==0xcc:
            j=i
            while j<len(b) and b[j]==0xcc: j+=1
            if j<len(b) and (lo+j)%16==0: s.add(lo+j)
            i=j
        else: i+=1
    return sorted(s)
out=[]
for f in glob.glob('src/**/*.c*',recursive=True):
    t=open(f,errors='ignore').read()
    if 'Unk_InlineUser_' not in t: continue
    mine=sorted(a for a in ann if ann[a]==f)
    # use the placeholder's own copies: the cluster around the file's
    # unk_<addr> name, or the highest cluster otherwise
    m=re.search(r'unk_([0-9a-f]{8})',f)
    st=int(m.group(1),16) if m else mine[-1]
    i=bisect.bisect_left(A,st)
    while i<len(A) and ann[A[i]]==f: i+=1
    end=A[i] if i<len(A) else st+0x1000
    S=starts(st,end); un=[]
    for k,a in enumerate(S):
        e=S[k+1] if k+1<len(S) else end
        while e>a and img[e-1-0x400000]==0xcc: e-=1
        if a not in ann: un.append((a,e-a))
    out.append((sum(z for _,z in un),f,st,end,un))
out.sort()
import sys
mx=int(sys.argv[1]) if len(sys.argv)>1 else 10**9
for tot,f,st,end,un in out:
    if tot>mx: continue
    print(f'{tot:6d} {len(un):3d} {st:08x}-{end:08x} {f} -> {ann.get(end,"")}  '+' '.join(f'{a:x}:{z}' for a,z in un[:8]))
