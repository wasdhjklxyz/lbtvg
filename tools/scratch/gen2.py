import os
ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
import sys,re,subprocess
lo,hi=int(sys.argv[1],16),int(sys.argv[2],16); names=sys.argv[3].split(',')
out=subprocess.run(['python3','/tmp/cdis.py',hex(lo),hex(hi)],capture_output=True,text=True).stdout
fld={0x144:'flags144',0x148:'flags148',0x14c:'flags',0x150:'flags150'}
res=[];conf=[]
for line,name in zip(out.strip().split('\n'),names):
    a=int(line.split(':')[0],16); s=line.split(': ',1)[1]
    code=None
    m=re.match(r'mov eax, \[(0xacb860|0xacb864)\]; or \[eax \+ (4|0x13c)\], (0x[0-9a-f]+|\d+); push esi; .*and \[eax \+ (4|0x13c)\], 0x[0-9a-f]+; pop esi; ret$',s)
    if m and 'call 0x6dc3a0' in s:
        f='CC_SetCDataFlagsj' if m.group(2)=='4' else 'CC_SetGCDataFlagsj'
        code=f'void CC_{name}(NUFPAR *parser) {{ {f}(parser, {int(m.group(3),0):#x}); }}\n'
    else:
        m=re.search(r'xor \[e[a-d]x \+ (0x[0-9a-f]+)\], e[a-d]x; (pop esi; )?ret$',s)
        k=re.findall(r'and e[a-d]x, (0x[0-9a-f]+|\d+);',s)
        if m and k and 'call 0x6dc3a0' in s and s.startswith('push esi'):
            off=int(m.group(1),16); mask=int(k[-1],0); bit=mask.bit_length()-1
            if mask==1<<bit and off in fld:
                code=f'''void CC_{name}(NUFPAR *parser) {{
  ((CCBits *)&charconfig.runtime->{fld[off]})->b{bit} =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}}
'''
    if code is None: print('// STOP at',hex(a),name,file=sys.stderr); break
    res.append(f'// FUNCTION: LEGOBATMAN 0x00{a:06x}\n'+code)
    conf.append(f'00{a:06x} CC_{name}  matched, Mac order')
open('/tmp/gen.c','w').write('\n'+'\n'.join(res))
open('/tmp/gen.conf','w').write('\n'.join(conf)+'\n')
print(len(res),'generated', file=sys.stderr)
