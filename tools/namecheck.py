#!/usr/bin/env python3
"""Warn when a function in src/ is named differently from confirmed.txt.

tools/symbols/confirmed.txt is the hand-checked truth for names; a
// FUNCTION: or // STUB: whose definition uses another name is probably stale
(written before the name was settled). Prints one line per mismatch; exit 0
either way (the pre-commit hook shows this as a warning, not a failure).
"""
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
DECL = re.compile(r"//\s*(?:FUNCTION|STUB):\s*LEGOBATMAN\s+0x([0-9a-fA-F]+)\n(?:\s*//.*\n)*\s*([^{;]*?)\s*(?:\{|;)", re.M)
# as in tools/match.py: operators, destructors, template members
IDENT = re.compile(r"((?:[A-Za-z_~][\w~]*(?:<[^()]*>)?::)*"
                   r"(?:operator\s*(?:\(\)|[^\s(]+)|[A-Za-z_~][\w~]*)(?:<[^()]*>)?)\s*\(")


def main():
    confirmed = {}
    for line in open(ROOT / "tools/symbols/confirmed.txt"):
        if line.strip() and not line.startswith("#"):
            a, name = line.split()[:2]
            confirmed[int(a, 16)] = name
    bad = 0
    for f in sorted(list(ROOT.glob("src/**/*.c")) + list(ROOT.glob("src/**/*.cpp")) + list(ROOT.glob("src/**/*.h"))):
        for m in DECL.finditer(f.read_text(errors="ignore")):
            a = int(m.group(1), 16)
            want = confirmed.get(a)
            if not want:
                continue
            got = IDENT.search(m.group(2))
            got = re.sub(r"\s+|<[^()]*>", "", got.group(1)) if got else m.group(2)
            if got.split("::")[-1] != want.split("::")[-1]:
                print(f"name: {a:08x} is {got} in {f.relative_to(ROOT)}, but confirmed.txt says {want}")
                bad += 1
    return bad


if __name__ == "__main__":
    main()
