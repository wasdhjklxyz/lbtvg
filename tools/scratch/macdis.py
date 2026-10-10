# macdis.py NAME|MACADDR: disassemble one Mac (i386 Mach-O) function, bounded
# by the next text symbol; symbol names resolved for call targets.
import sys,bisect,lief,capstone
b=lief.parse('orig/donors/mac/LEGO Batman')
nm=[]
for l in open('tools/symbols/mac-1.0.1.demangled.nm'):
    p=l.rstrip('\n').split(' ',2)
    if len(p)==3 and p[1] in 'Tt': nm.append((int(p[0],16),p[2]))
nm.sort(); A=[x for x,_ in nm]
q=sys.argv[1]
try: a=int(q,16)
except ValueError: a=[x for x,s in nm if s==q or s.startswith(q+'(') or s=='_'+q][0]
i=bisect.bisect_right(A,a); e=A[i] if i<len(A) else a+0x200
code=bytes(b.get_content_from_virtual_address(a,e-a))
md=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32)
names=dict(nm)
for ins in md.disasm(code,a):
    s=ins.op_str
    if ins.mnemonic=='call' and s.startswith('0x'):
        t=int(s,16); s+='  ; '+names.get(t,'')
    print(f'{ins.address:08x} {ins.mnemonic} {s}')
