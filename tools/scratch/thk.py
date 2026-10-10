# thk.py ADDR...: resolve incremental-link jmp thunks to their target and name
# (our annotation, else pc-names.csv).
import sys,re,glob,csv,pefile,struct
pe=pefile.PE('orig/LEGOBatman.exe');img=pe.get_memory_mapped_image()
ann={}
for f in glob.glob('src/**/*.c*',recursive=True):
    t=open(f,errors='ignore').read()
    for m in re.finditer(r'//\s*(?:FUNCTION|STUB):\s*LEGOBATMAN\s+0x([0-9a-fA-F]+)\s*\n(?:[^\n]*\n){0,2}?[^\n]*?([A-Za-z_][\w:~]*)\s*\(',t):
        ann[int(m.group(1),16)]=m.group(2)
nm={int(r['pc_addr'],16):r['demangled'] for r in csv.DictReader(open('tools/symbols/pc-names.csv'))}
for q in sys.argv[1:]:
    a=int(q,16);o=a-0x400000
    t=a
    if img[o]==0xe9: t=a+5+struct.unpack('<i',img[o+1:o+5])[0]
    print(f'{a:08x} -> {t:08x} {ann.get(t,"")} | {nm.get(t,"")}')
