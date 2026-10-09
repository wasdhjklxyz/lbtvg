import os
ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
import pefile,struct,re,glob,sys
pe=pefile.PE(ROOT+'/orig/LEGOBatman.exe');img=pe.get_memory_mapped_image()
done={}
for f in glob.glob(ROOT+'/src/**/*.c*',recursive=True):
    for m in re.finditer(r'//\s*(?:FUNCTION|STUB):\s*LEGOBATMAN\s+0x([0-9a-fA-F]+)\n(?:\s*//.*\n)*\s*[^\n(]*?(\w+)\(',open(f).read()): done[int(m.group(1),16)]=m.group(2)
macnames=set()
for l in open(ROOT+'/tools/symbols/mac-1.0.1.nm'):
    p=l.split()
    if len(p)==3:
        m=re.match(r'__Z\d+(\w+?)P7AISYS',p[2]) or re.match(r'_(\w+)$',p[2])
        if m: macnames.add(m.group(1))
pref=sys.argv[1]
for l in open('/tmp/kwmap.txt'):
    a,n,s,st,tab=l.split()
    if tab!=sys.argv[2]: continue
    va=int(a,16)
    if img[va-0x400000]==0xe9:
        va=va+5+struct.unpack('<i',img[va-0x400000+1:va-0x400000+5])[0]
    name=pref+s
    print(f'{va:08x} {name} {"MAC" if name in macnames else "-"} {done.get(va,"todo")}')
