import sys,subprocess,re
p=sys.argv[1]; addr=sys.argv[2]; start=sys.argv[3]; end=sys.argv[4]
variants=open(sys.argv[5]).read().split("\n====\n")
s=open(p).read()
i=s.index(start); j=s.index(end,i)
orig=s[i:j]
best=None
for k,v in enumerate(variants):
    open(p,'w').write(s[:i]+v.strip('\n')+"\n\n"+s[j:])
    r=subprocess.run(["python3","tools/match.py",addr],capture_output=True,text=True).stdout
    m=re.search(r"(\d+)/(\d+) bytes matched",r)
    nd=len([l for l in r.splitlines() if re.search(r"\x1b\[31m\x1b\[1m[!<>]",l)])
    print(k, m.group(0) if m else "ERR", "difflines",nd, flush=True)
    if m and m.group(1)==m.group(2):
        print("MATCH",k); sys.exit(0)
open(p,'w').write(s)
