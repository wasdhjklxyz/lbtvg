# kwsumm.py [N]: after kwall.py (writes /tmp/kwmap.txt), rank keyword tables by
# callbacks with no FUNCTION/STUB and not in skip.txt; includes ghidra-missed ones.
import pefile,struct,re,glob,collections,sys
ROOT='/home/uiop/Workspace/lbtvg'
pe=pefile.PE(ROOT+'/orig/LEGOBatman.exe');img=pe.get_memory_mapped_image()
done={}
for f in glob.glob(ROOT+'/src/**/*.c*',recursive=True):
    for m in re.finditer(r'//\s*(FUNCTION|STUB):\s*LEGOBATMAN\s+0x([0-9a-fA-F]+)',open(f).read()): done[int(m.group(2),16)]=m.group(1)
skip=set()
for l in open(ROOT+'/tools/symbols/skip.txt'):
    m=re.match(r'\s*(?:0x)?([0-9a-fA-F]{6,8})',l)
    if m: skip.add(int(m.group(1),16))
size={}
for l in open(ROOT+'/ghidra/LEGOBatman.exe.stats.tsv'):
    p=l.split('\t')
    if len(p)>1 and p[1].strip().isdigit() and not l.startswith('#'): size[int(p[0],16)]=int(p[1])
tabs=collections.defaultdict(list)
for l in open('/tmp/kwmap.txt'):
    a,n,s,st,tab=l.split()
    va=int(a,16)
    if img[va-0x400000]==0xe9: va=va+5+struct.unpack('<i',img[va-0x400000+1:va-0x400000+5])[0]
    tabs[tab].append((va,n,s))
rows=[]
for t,es in tabs.items():
    todo=[e for e in es if e[0] not in done and e[0] not in skip and size.get(e[0],0)<400]
    rows.append((len(todo),t,len(es),todo))
rows.sort(reverse=True)
for c,t,n,todo in rows[:int(sys.argv[1]) if len(sys.argv)>1 else 25]:
    print(t,n,c,' '.join(f'{x[1] if x[1]!="None" else x[2]}@{x[0]:x}/{size.get(x[0])}' for x in todo))
