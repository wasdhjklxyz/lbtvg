# hdrstatics_annotate.py [FILE...]: annotate the per-TU header-static copies in their TU file.
import re,os,sys,subprocess,collections
sys.argv=[sys.argv[0]]+sys.argv[1:]
exec(open('tools/scratch/hdrstatics_core.py').read())   # defines plan: {file: [(addr,family)]}
FN={'sin':('NuSinApprox','static f32 NuSinApprox(i32 angle);'),'cos':('NuCosApprox','static f32 NuCosApprox(i32 angle);'),
    'fabs':('NuFabs','static f32 NuFabs(f32 f);'),'fdiv':('NuFdiv','static f32 NuFdiv(f32 a, f32 b);'),
    'sign':('NuFsign','static f32 NuFsign(f32 f);'),'v4set':('NuVec4Set','static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);'),
    'v4copy':('NuVec4Copy','static void NuVec4Copy(f32 *dst, f32 *src);'),'vscale':('NuVecScaleInline','static void NuVecScaleInline(f32 *dst, f32 *src, f32 s);'),
    'mroty':('NuMtxRotateYInline','static void NuMtxRotateYInline(f32 *m, i32 a);'),'mcopy':('NuMtxCopyInline','static void NuMtxCopyInline(f32 *dst, f32 *src);'),
    'vmag':('NuVecMagInline','static f32 NuVecMagInline(f32 *v);')}
USE={'sin':'v[0] = NuSinApprox(i);','cos':'v[1] = NuCosApprox(i);','fabs':'v[2] = NuFabs(a);','fdiv':'v[3] = NuFdiv(a, v[4]);',
     'sign':'v[5] = NuFsign(a);','v4set':'NuVec4Set(v, a, a, a, a);','v4copy':'NuVec4Copy(v + 4, v);','vscale':'NuVecScaleInline(v + 8, v, a);',
     'mroty':'NuMtxRotateYInline(v + 16, i);','mcopy':'NuMtxCopyInline(v + 32, v + 16);',
     'vmag':'v[48] = NuVecMagInline(v);'}
def match(f):
    r=subprocess.run(['python3','tools/match.py',f],capture_output=True,text=True)
    out=re.sub(r'\x1b\[[0-9;]*m','',r.stdout+r.stderr)
    st={}
    for l in out.splitlines():
        m=re.match(r'(MATCH|DIFF|STUB-DIFF|STUB-MATCH)\s+([0-9a-f]{8})',l)
        if m: st[int(m.group(2),16)]=m.group(1)
    return st,('compile failed' in out or 'error' in out.lower() and not st)
def edit(path,orig,fams):
    s=orig
    rel=os.path.relpath('src/nu2api/numath',os.path.dirname(path)).replace(os.sep,'/')
    cut=len(s)
    for pat in (r'^#if 0',r'^// (FUNCTION|STUB|GLOBAL): '):
        m=re.search(pat,s,re.M)
        if m: cut=min(cut,m.start())
    incs=[m for m in re.finditer(r'^#include .*\n',s,re.M) if m.start()<cut]
    pos=incs[-1].end() if incs else 0
    add=''
    if any(n in ('sin','cos') for _,n in fams) and 'nutrig_unk.h' not in s: add+='#include "%s/nutrig_unk.h"\n'%rel
    if any(n not in ('sin','cos','mroty','mcopy') for _,n in fams) and 'nuinline_unk.h' not in s: add+='#include "%s/nuinline_unk.h"\n'%rel
    if any(n in ('mroty','mcopy') for _,n in fams) and 'numtx_inline_unk.h' not in s: add+='#include "%s/numtx_inline_unk.h"\n'%rel
    block='\n// Header statics: this TU\'s copies (bodies in nuinline_unk.h/nutrig_unk.h).\n'
    for a,n in fams: block+='// FUNCTION: LEGOBATMAN 0x%08x\n%s\n'%(a,FN[n][1])
    s=s[:pos]+add+block+s[pos:]
    stem=re.sub(r'\W','_',os.path.splitext(os.path.basename(path))[0])
    k=1
    while 'Unk_InlineUser_%s%s('%('%d_'%k if k>1 else '',stem) in s: k+=1
    stem=('%d_'%k if k>1 else '')+stem
    s=s.rstrip('\n')+'\n\n// Keeps the header-static copies above alive until their real callers are\n// matched.\nvoid Unk_InlineUser_%s(f32 *v, f32 a, i32 i) {\n'%stem+''.join('  %s\n'%USE[n] for _,n in fams)+'}\n'
    return s
for path,lst in plan.items():
    if sys.argv[1:] and path not in sys.argv[1:]: continue
    orig=open(path).read()
    base,_=match(path)
    if any(v=='DIFF' for v in base.values()): print('SKIP baseline DIFF',path); continue
    seen=set(); fams=[]
    code=re.sub(r'//.*','',orig)
    for a,n in lst:
        if n in seen or n not in FN: continue
        name=FN[n][0]
        if n=='sin' and 'NuSinApprox' in code:
            if not ('nutrig_unk.h' in code and not re.search(r'LEGOBATMAN 0x[0-9a-f]+\n(//.*\n)*static f32 NuSinApprox',orig)): continue
        if n=='cos' and 'NuCosApprox' in code: continue
        if n!='sin' and n!='cos' and re.search(r'\b%s\b'%name,code): continue
        if n=='cos' and 'NuSinApprox' in code and 'nutrig_unk.h' not in code: continue
        seen.add(n); fams.append((a,n))
    if not fams: print('NONE',path); continue
    while fams:
        open(path,'w').write(edit(path,orig,fams))
        st,err=match(path)
        bad=[(a,n) for a,n in fams if st.get(a)!='MATCH']
        broke=[a for a,v in base.items() if v=='MATCH' and st.get(a)!='MATCH']
        if not bad and not broke: break
        if broke or err or len(bad)==len(fams):
            # try dropping failures one family at a time
            if len(fams)==1 or broke: fams=[] if broke else []; 
            else: fams=[x for x in fams if x not in bad] if bad and len(bad)<len(fams) else fams[1:]
        else: fams=[x for x in fams if x not in bad]
    if not fams: open(path,'w').write(orig); print('REVERT',path)
    else: print('OK',path,' '.join('%x:%s'%x for x in fams))
