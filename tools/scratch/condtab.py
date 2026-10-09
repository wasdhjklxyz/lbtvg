# condtab.py: walk the AI condition keyword table ({name, condition, init}
# triples around 0x93b1b4) and print the condition/init functions that have
# no FUNCTION/STUB annotation yet, named Condition_<keyword>[Init].
import pefile,struct,re,glob
import os
R=os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
pe=pefile.PE(R+'/orig/LEGOBatman.exe');img=pe.get_memory_mapped_image();B=0x400000
def rd(va): return struct.unpack('<I',img[va-B:va-B+4])[0]
def res(va):
    if va and 0x401000<=va<0x404000 and img[va-B]==0xe9:
        return va+5+struct.unpack('<i',img[va-B+1:va-B+5])[0]
    return va
def cstr(va):
    s=b''
    while img[va-B] and len(s)<64: s+=bytes([img[va-B]]); va+=1
    return s.decode('latin1')
done={}
for f in glob.glob(R+'/src/**/*.c*',recursive=True):
    for m in re.finditer(r'//\s*(FUNCTION|STUB):\s*LEGOBATMAN\s+0x([0-9a-fA-F]+)',open(f).read()): done[int(m.group(2),16)]=m.group(1)
va=0x93b1b4
# find start: walk back while entries look valid
while True:
    p=rd(va-12)
    if 0x800000<p<0x900000 and 0x401000<=res(rd(va-8))<0x800000: va-=12
    else: break
seen=set()
while True:
    p=rd(va)
    if not (0x800000<p<0x900000): break
    name=cstr(p); c=res(rd(va+4)); i=res(rd(va+8))
    for kind,a in (('',c),('Init',i)):
        if a and a not in seen:
            seen.add(a)
            if a not in done: print(f'{a:08x} Condition_{name}{kind}')
    va+=12
