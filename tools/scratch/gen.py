import os
ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
import sys,re,subprocess
# usage: gen.py lo hi name1,name2,...   -> prints C for bitfield CC_ fns, stops at first non-pattern
lo,hi=int(sys.argv[1],16),int(sys.argv[2],16); names=sys.argv[3].split(',')
out=subprocess.run(['python3','/tmp/cdis.py',hex(lo),hex(hi)],capture_output=True,text=True).stdout
fld={0x144:'flags144',0x148:'flags148',0x14c:'flags',0x150:'flags150'}
res=[];conf=[]
for line,name in zip(out.strip().split('\n'),names):
    a=int(line.split(':')[0],16); s=line.split(': ',1)[1]
    m=re.search(r'xor \[e[a-d]x \+ (0x[0-9a-f]+)\], e[a-d]x; (pop esi; )?ret$',s)
    k=re.findall(r'and e[a-d]x, (0x[0-9a-f]+|\d+);',s)
    if not m or not k or 'call 0x6dc3a0' not in s: print('// STOP at',hex(a),name,file=sys.stderr); break
    off=int(m.group(1),16); mask=int(k[-1],0); bit=mask.bit_length()-1
    if mask!=1<<bit or off not in fld: print('// STOP2',hex(a),file=sys.stderr);break
    res.append(f'''// FUNCTION: LEGOBATMAN 0x00{a:06x}
void CC_{name}(NUFPAR *parser) {{
  ((CCBits *)&charconfig.runtime->{fld[off]})->b{bit} =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}}
''')
    conf.append(f'00{a:06x} CC_{name}  matched, Mac order')
open('/tmp/gen.c','w').write('\n'+'\n'.join(res))
open('/tmp/gen.conf','w').write('\n'.join(conf)+'\n')
print(len(res),'generated', file=sys.stderr)
