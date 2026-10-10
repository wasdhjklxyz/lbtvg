# newtu.py: create unk_<addr>.cpp TU files for clusters of still-unannotated header-static copies.
import re,os,sys,subprocess,bisect
exec(open('tools/scratch/hdrstatics_core.py').read())
FN={'sin':'static f32 NuSinApprox(i32 angle);','cos':'static f32 NuCosApprox(i32 angle);','fabs':'static f32 NuFabs(f32 f);','fdiv':'static f32 NuFdiv(f32 a, f32 b);',
    'sign':'static f32 NuFsign(f32 f);','v4set':'static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);','v4copy':'static void NuVec4Copy(f32 *dst, f32 *src);','vscale':'static void NuVecScaleInline(f32 *dst, f32 *src, f32 s);',
    'mroty':'static void NuMtxRotateYInline(f32 *m, i32 a);','mcopy':'static void NuMtxCopyInline(f32 *dst, f32 *src);'}
USE={'sin':'v[0] = NuSinApprox(i);','cos':'v[1] = NuCosApprox(i);','fabs':'v[2] = NuFabs(a);','fdiv':'v[3] = NuFdiv(a, v[4]);','sign':'v[5] = NuFsign(a);',
     'v4set':'NuVec4Set(v, a, a, a, a);','v4copy':'NuVec4Copy(v + 4, v);','vscale':'NuVecScaleInline(v + 8, v, a);',
     'mroty':'NuMtxRotateYInline(v + 16, i);','mcopy':'NuMtxCopyInline(v + 32, v + 16);'}
un=[(a,n) for a,n in copies if a not in ann and n in FN]
clusters=[];cur=[]
for a,n in un:
    if cur and a-cur[-1][0]>=0x300: clusters.append(cur);cur=[]
    cur.append((a,n))
if cur: clusters.append(cur)
made=0
for cl in clusters:
    seen=set();fams=[]
    for a,n in cl:
        if n not in seen: seen.add(n);fams.append((a,n))
    a0=cl[0][0]
    j=bisect.bisect_right(keys,a0)
    near=ann[keys[j]] if j<len(keys) else ann[keys[j-1]]
    if j>0 and (j>=len(keys) or a0-keys[j-1]<keys[j]-a0): near=ann[keys[j-1]]
    d=os.path.dirname(near); path='%s/unk_%08x.cpp'%(d,a0)
    if os.path.exists(path): print('EXISTS',path); continue
    rel=os.path.relpath('src/nu2api',d).replace(os.sep,'/')
    def text(fams):
        s='// %s: TU of unknown name, found by its header-static copies (the\n// functions after them are not matched yet).\n\n'%path[4:]
        s+='#include "%s/numath/nuinline_unk.h"\n#include "%s/numath/numtx_inline_unk.h"\n#include "%s/numath/nutrig_unk.h"\n\n'%(rel,rel,rel)
        for a,n in fams: s+='// FUNCTION: LEGOBATMAN 0x%08x\n%s\n'%(a,FN[n])
        s+='\n// Keeps the header-static copies above alive until their real callers are\n// matched.\nvoid Unk_InlineUser_%08x(f32 *v, f32 a, i32 i) {\n'%a0+''.join('  %s\n'%USE[n] for _,n in fams)+'}\n'
        return s
    for attempt in range(3):
        open(path,'w').write(text(fams))
        r=subprocess.run(['python3','tools/match.py',path],capture_output=True,text=True)
        out=re.sub(r'\x1b\[[0-9;]*m','',r.stdout+r.stderr)
        st={int(m.group(2),16):m.group(1) for m in re.finditer(r'^(MATCH|DIFF)\s+([0-9a-f]{8})',out,re.M)}
        bad=[x for x in fams if st.get(x[0])!='MATCH']
        if not bad: break
        fams=[x for x in fams if x not in bad]
        if not fams: break
    if fams and not bad: made+=1; print('NEW',path,' '.join('%x:%s'%x for x in fams))
    else: os.remove(path); print('FAIL',path)
print('made',made)
