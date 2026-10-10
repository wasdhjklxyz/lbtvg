# variants_multi.py ADDR VARIANTS [FILE]: like variants.py, but each variant is
# a set of edits. VARIANTS holds variants separated by "\n#####\n"; a variant
# holds edits separated by "\n%%%%\n"; an edit is "old\n----\nnew" (old must
# occur exactly once in FILE). Runs match.py on ADDR for each variant, prints
# bytes/diff lines, and restores FILE afterwards.
import re
import subprocess
import sys

addr = sys.argv[1]
p = sys.argv[3] if len(sys.argv) > 3 else 'src/gameapi/gizmoblowups_unk.cpp'
orig = open(p).read()
vs = open(sys.argv[2]).read().split('\n#####\n')
try:
    for k, v in enumerate(vs):
        s = orig
        ok = True
        for pair in v.strip('\n').split('\n%%%%\n'):
            a, b = pair.split('\n----\n')
            if s.count(a) != 1:
                print(k, 'nomatch', repr(a[:60]))
                ok = False
                break
            s = s.replace(a, b)
        if not ok:
            continue
        open(p, 'w').write(s)
        r = subprocess.run(["python3", "tools/match.py", addr],
                           capture_output=True, text=True).stdout
        m = re.search(r"(\d+)/(\d+) bytes matched", r)
        nd = len([l for l in r.splitlines()
                  if re.search(r"\x1b\[31m\x1b\[1m[!<>]", l)])
        print(k, m.group(0) if m else "ERR", "difflines", nd, flush=True)
finally:
    open(p, 'w').write(orig)
