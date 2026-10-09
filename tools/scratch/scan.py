# scan.py LO HI [MAXSZ=120]: unannotated, unskipped functions in [LO,HI) up to
# MAXSZ bytes, including starts ghidra missed (found after int3 padding).
import sys,re,glob,pefile
lo,hi=int(sys.argv[1],16),int(sys.argv[2],16)
maxsz=int(sys.argv[3]) if len(sys.argv)>3 else 120
pe=pefile.PE('orig/LEGOBatman.exe');img=pe.get_memory_mapped_image()
done=set()
for f in glob.glob('src/**/*.c*',recursive=True):
    for m in re.finditer(r'//\s*(?:FUNCTION|STUB):\s*LEGOBATMAN\s+0x([0-9a-fA-F]+)',open(f,errors='ignore').read()):
        done.add(int(m.group(1),16))
skip=set()
for l in open('tools/symbols/skip.txt'):
    p=l.split()
    if p and re.match(r'^[0-9a-f]{8}$',p[0]): skip.add(int(p[0],16))
starts=set()
for l in open('tools/symbols/functions.tsv'):
    if l.startswith('#'): continue
    a=int(l.split('\t')[0],16)
    if lo<=a<hi: starts.add(a)
b=img[lo-0x400000:hi-0x400000]
i=0
while i<len(b):
    if b[i]==0xcc:
        j=i
        while j<len(b) and b[j]==0xcc: j+=1
        if j<len(b) and j-i>=1 and (lo+j)%16==0: starts.add(lo+j)
        i=j
    else: i+=1
s=sorted(starts)
for k,a in enumerate(s):
    end=s[k+1] if k+1<len(s) else hi
    # trim trailing int3
    e=end
    while e>a and img[e-1-0x400000]==0xcc: e-=1
    sz=e-a
    if a in done or a in skip or sz>maxsz: continue
    print(f'{a:08x} {sz:4d} {img[a-0x400000:a-0x400000+12].hex()}')
