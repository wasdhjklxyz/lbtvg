#!/usr/bin/env python3
"""Add case-alias symlinks to the Windows headers so Linux tools can find them.

cl.exe under wibo does not care about case; clangd on Linux does (the SDK has
`Windows.h`, code says `<windows.h>`). For each include dir: link every
header's all-lowercase name, plus every spelling any header or src/ file
actually #includes. Idempotent. Run by tools/vc8.sh; usage:
    tools/casefold.py [DIR ...]      default: $LBTVG_TOOLCHAIN/{vc8/INCLUDE,winsdk6/Include}
"""
import os, re, sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
TC = Path(os.environ.get("LBTVG_TOOLCHAIN", ROOT / "toolchain"))
INC = re.compile(rb'^\s*#\s*include\s*[<"]([^>"]+)[>"]', re.M)


def main(argv):
    dirs = [Path(d) for d in argv] or [TC / "vc8/INCLUDE", TC / "winsdk6/Include"]
    dirs = [d for d in dirs if d.is_dir()]
    wanted = set()
    for d in dirs + [ROOT / "src"]:
        for f in d.rglob("*"):
            if f.is_file() and f.suffix.lower() in (".h", ".hpp", ".inl", ".c", ".cpp", "") and f.stat().st_size < 2_000_000:
                wanted.update(m.decode("latin-1").replace("\\", "/") for m in INC.findall(f.read_bytes()))
    made = 0
    for d in dirs:
        for sub in [p for p in d.rglob("*") if p.is_dir()] + [d]:
            names = {p.name for p in sub.iterdir()}
            lower = {}
            for n in names:
                lower.setdefault(n.lower(), n)
            want_here = {w.split("/")[-1] for w in wanted} | {n.lower() for n in names}
            for w in want_here:
                real = lower.get(w.lower())
                if real and w != real and w not in names:
                    os.symlink(real, sub / w)
                    names.add(w)
                    made += 1
    print(f"casefold: {made} case-alias links in {', '.join(str(d) for d in dirs)}")


if __name__ == "__main__":
    main(sys.argv[1:])
