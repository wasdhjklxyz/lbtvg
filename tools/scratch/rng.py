# rng.py LO HI: every function start in [LO,HI) (functions.tsv + int3 scan)
# with size, annotation state (F/S/-) and the pc-names.csv name and method.
import sys,re,glob,csv,pefile
lo,hi=int(sys.argv[1],16),int(sys.argv[2],16)
pe=pefile.PE('orig/LEGOBatman.exe');img=pe.get_memory_mapped_image()
ann={}
for f in glob.glob('src/**/*.c*',recursive=True):
    t=open(f,errors='ignore').read()
    for m in re.finditer(r'//\s*(FUNCTION|STUB|SYNTHETIC):\s*LEGOBATMAN\s+0x([0-9a-fA-F]+)[^\n]*\n(?:[^\n]*\n){0,2}?[^\n]*?([A-Za-z_][\w:~]*)\s*\(',t):
        ann[int(m.group(2),16)]=(m.group(1)[0],m.group(3),f.split('/')[-1])
nm={}
for r in csv.DictReader(open('tools/symbols/pc-names.csv')):
    nm[int(r['pc_addr'],16)]=(r['demangled'],r['method'],r['mac_addr'])
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
S=sorted(s)
for k,a in enumerate(S):
    e=S[k+1] if k+1<len(S) else hi
    while e>a and img[e-1-0x400000]==0xcc: e-=1
    st,name,f=ann.get(a,('-','',''))
    n=nm.get(a,('','',''))
    print(f'{a:08x} {e-a:5d} {st} {name[:34]:34} {f[:22]:22} {n[2]} {n[0][:60]} {n[1]}')
