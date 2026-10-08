#!/usr/bin/env python3
"""Project progress: matched bytes / game code bytes.

Denominator: functions ghidra found in the game region of .text (below the
first CRT function, see docs/recon.md), excluding FID/library-named ones.
Numerator: functions annotated `// FUNCTION:` under src/ (they must pass
`make match`; this script does not recompile). STUBs count as attempted.

    tools/progress.py            table
    tools/progress.py --readme   rewrite the progress line in README.md
"""
import re, sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
STATS = ROOT / "ghidra/LEGOBatman.exe.stats.tsv"
CRT_START = 0x0073C8FB  # first CRT function (docs/recon.md)
ANNOT = re.compile(r"//\s*(FUNCTION|STUB):\s*LEGOBATMAN\s+0x([0-9a-fA-F]+)")

def main(argv):
    funcs = {}
    for line in open(STATS):
        if line.startswith("#"):
            continue
        a, s, _, name = line.rstrip("\n").split("\t")
        a = int(a, 16)
        # game region, minus what FID/EH analysis named as library code
        if a < CRT_START and (name.startswith("FUN_") or name.startswith("thunk_")):
            funcs[a] = int(s)
    matched, stubbed = {}, {}
    for f in list(ROOT.glob("src/**/*.c")) + list(ROOT.glob("src/**/*.cpp")) + list(ROOT.glob("src/**/*.h")):
        for kind, a in ANNOT.findall(f.read_text()):
            (matched if kind == "FUNCTION" else stubbed)[int(a, 16)] = f
    total = sum(funcs.values())
    mb = sum(funcs.get(a, 0) for a in matched)
    sb = sum(funcs.get(a, 0) for a in stubbed)
    pct = 100.0 * mb / total if total else 0.0
    line = (f"Progress: {len(matched)} functions matched, {mb:,} / {total:,} bytes "
            f"of game code ({pct:.2f}%); {len(stubbed)} stubs.")
    if "--readme" in argv:
        readme = ROOT / "README.md"
        t = readme.read_text()
        t2, n = re.subn(r"^Progress: .*$", line, t, flags=re.M)
        if n == 0:
            t2 = t.replace("\nStatus:", "\n" + line + "\n\nStatus:", 1)
        readme.write_text(t2)
    print(line)
    print(f"  denominator: {len(funcs)} game-region functions without a library name")
    print(f"  stub bytes:  {sb:,}")

if __name__ == "__main__":
    main(sys.argv[1:])
