import os
ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
import sys,re,subprocess
# gen3.py GLOBALHEX STRUCT VAR PREFIX_STRIP < list of "addr name"
G=sys.argv[1]; S=sys.argv[2]; V=sys.argv[3]; strip=sys.argv[4]
items=[l.split() for l in sys.stdin if l.strip()]
fields={}; fns=[]; conf=[]; skipped=[]
for a,n in items:
    a=int(a,16)
    out=subprocess.run(['python3','/tmp/cdis.py',hex(a),hex(a+0x30)],capture_output=True,text=True).stdout.split('\n')[0]
    s=out.split(': ',1)[1] if ': ' in out else ''
    m=re.match(r'mov eax, \[esp \+ 4\]; push eax; call (0x6dd060|0x6daa40); mov ecx, \['+G+r'\]; (add esp, 4; )?(mov (b|word ptr |)\[ecx \+ (0x[0-9a-f]+)\], (al|ax|eax)|fstp \[ecx \+ (0x[0-9a-f]+)\]); (add esp, 4; )?ret$',s)
    if not m: skipped.append(n); continue
    if m.group(1)=='0x6daa40':
        if not m.group(7): skipped.append(n); continue
        off=int(m.group(7),16); ty='f32'; fn='NuFParGetFloat'
    else:
        if not m.group(5): skipped.append(n); continue
        off=int(m.group(5),16); ty={'al':'u8','ax':'i16','eax':'i32'}[m.group(6)]; fn='NuFParGetInt'
    fld=n[len(strip):]
    if off in fields and fields[off]!=(ty,fld): skipped.append(n+'(dup)'); continue
    fields[off]=(ty,fld)
    fns.append(f'// FUNCTION: LEGOBATMAN 0x{a:08x}\nvoid {n}(NUFPAR *parser) {{\n  {V}->{fld} = {fn}(parser);\n}}\n')
    conf.append(f'{a:08x} {n}  matched saga body, keyword table')
sz={'u8':1,'i16':2,'i32':4,'f32':4}
lines=[f'struct {S} {{']; pos=0
for off in sorted(fields):
    ty,f=fields[off]
    if off<pos: continue
    if off>pos: lines.append(f'  u8 pad{pos:x}[0x{off:x} - 0x{pos:x}];')
    lines.append(f'  {ty} {f}; // 0x{off:x}'); pos=off+sz[ty]
lines.append('};')
open('/tmp/gen3.struct','w').write('\n'.join(lines)+'\n')
open('/tmp/gen3.c','w').write('\n'.join(fns))
open('/tmp/gen3.conf','w').write('\n'.join(conf)+'\n')
print(len(fns),'fns; skipped:',' '.join(skipped),file=sys.stderr)
