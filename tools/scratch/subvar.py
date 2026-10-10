# subvar.py FILE ADDR VARFILE: VARFILE holds blocks "old\n----\nnew" split by
# "\n====\n"; each block is tried alone (text replaced once in FILE), the
# file is restored afterwards. Prints bytes matched and diff-line count.
import sys, subprocess, re
p, addr = sys.argv[1], sys.argv[2]
blocks = open(sys.argv[3]).read().split("\n====\n")
s = open(p).read()
try:
    for k, b in enumerate(blocks):
        old, new = b.split("\n----\n")
        if old not in s:
            print(k, "old text not found"); continue
        open(p, 'w').write(s.replace(old, new, 1))
        r = subprocess.run(["python3", "tools/match.py", addr],
                           capture_output=True, text=True, env={**__import__('os').environ, "NO_COLOR": "1"}).stdout
        m = re.search(r"(\d+)/(\d+) bytes matched", r)
        nd = len([l for l in r.splitlines() if re.search(r"\s[!<>]\s", l)])
        print(k, m.group(0) if m else "ERR", "difflines", nd, flush=True)
finally:
    open(p, 'w').write(s)
