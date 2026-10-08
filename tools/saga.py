#!/usr/bin/env python3
"""Print saga's body for a function, to port into src/ by hand.

    tools/saga.py NAME [NAME ...]     e.g. tools/saga.py NuVecCross GizForceSFX_Configure
    tools/saga.py 0xADDR              look the name up in tools/symbols/pc-names.csv

Reads ref/saga (git submodule, opensagadev/saga, GPL-3.0). Copy what you need
into the right src/ file and adapt it for VC8; say where it came from in the
commit message. Do not symlink saga files: our copy has to diverge.
"""
import csv, re, sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
SAGA = ROOT / "ref/saga/src"


def body_at(text, start):
    """From the opening brace at/after start, return the text through its match."""
    i = text.index("{", start)
    depth = 0
    for j in range(i, len(text)):
        if text[j] == "{":
            depth += 1
        elif text[j] == "}":
            depth -= 1
            if depth == 0:
                return text[start:j + 1]
    return text[start:]


def find(name):
    short = name.split("::")[-1]
    pat = re.compile(r"^[^\n;{}]*?\b" + re.escape(name) + r"\s*\([^;{]*\)\s*(?:const\s*)?\{", re.M)
    hits = []
    for f in sorted(list(SAGA.rglob("*.c")) + list(SAGA.rglob("*.cpp"))):
        t = f.read_text(errors="ignore")
        if short not in t:
            continue
        for m in pat.finditer(t):
            prefix = m.group(0)[: m.group(0).rfind(name)]
            if not prefix.strip() or re.search(r"[=(]|\breturn\b|\belse\b", prefix):
                continue  # a call or expression, not a definition
            line = t.count("\n", 0, m.start()) + 1
            hits.append((f.relative_to(ROOT), line, body_at(t, m.start())))
    return hits


def main(argv):
    if not SAGA.is_dir():
        sys.exit("ref/saga missing: git submodule update --init")
    names = {int(r["pc_addr"], 16): r["demangled"] for r in csv.DictReader(open(ROOT / "tools/symbols/pc-names.csv"))}
    for arg in argv:
        name = arg
        if arg.lower().startswith("0x"):
            full = names.get(int(arg, 16))
            if not full:
                print(f"// {arg}: no known name"); continue
            name = full.split("(")[0].split(" ")[-1].lstrip("_")
        hits = find(name)
        if not hits:
            print(f"// {name}: not in saga"); continue
        for path, line, body in hits:
            print(f"// {name}: {path}:{line}\n{body}\n")


if __name__ == "__main__":
    main(sys.argv[1:])
