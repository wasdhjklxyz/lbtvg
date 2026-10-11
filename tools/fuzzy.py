#!/usr/bin/env python3
"""Fuzzy match scores for STUBs: how close a stub's compiled code is to the
original, as a percentage (for decomp.dev's "fuzzy match" bar).

    tools/fuzzy.py [--force]     print "addr pct name" for every stub

Each stub is compiled as in tools/match.py and both sides are disassembled
with the same masking as `make match`'s display; the score is difflib's
ratio over the instruction keys (mnemonic + masked bytes), i.e.
2*matching / (ours + original). A stub that doesn't compile (#if 0) is 0.

Results are cached per source file content in build/fuzzy-cache.json, so
the pre-commit hook only recompiles files whose text changed. A change in
a header alone doesn't invalidate the cache: `--force` recomputes all.
"""
import difflib, hashlib, json, sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import match  # noqa: E402

CACHE = match.ROOT / "build/fuzzy-cache.json"


def score(code, relocs, orig, addr, names):
    a = [i["key"] for i in match.insns(orig, addr, len(orig), names=names)]
    b = [i["key"] for i in match.insns(code, addr, len(code), relocs=relocs)]
    if not a or not b:
        return 0.0
    return 100.0 * difflib.SequenceMatcher(None, a, b, autojunk=False).ratio()


def stub_scores(orig_size, force=False):
    """{addr: pct} for every STUB in src/. orig_size(addr) -> original size."""
    try:
        cache = {} if force else json.loads(CACHE.read_text())
    except (OSError, ValueError):
        cache = {}
    import pefile
    pe = pefile.PE(str(match.EXE), fast_load=True)
    base = pe.OPTIONAL_HEADER.ImageBase
    names = match.load_names()
    out, fresh = {}, {}
    for src in sorted(list(match.SRC.rglob("*.c")) + list(match.SRC.rglob("*.cpp"))):
        stubs = match.annotations(src, stubs=True)
        if not stubs:
            continue
        rel = src.relative_to(match.ROOT).as_posix()
        digest = hashlib.sha1(src.read_bytes()).hexdigest()
        hit = cache.get(rel)
        if hit and hit["sha1"] == digest:
            scores = {int(a): p for a, p in hit["scores"].items()}
        else:
            scores = {}
            try:
                funcs = match.functions_in_obj(match.compile_tu(src))
            except SystemExit:  # doesn't compile: every stub in it scores 0
                funcs = {}
            for addr, name in stubs:
                try:
                    (code, relocs), _ = match.find(funcs, name, match.SIG.get((src, addr)))
                except LookupError:  # still in #if 0, or never emitted
                    scores[addr] = 0.0
                    continue
                size = orig_size(addr) or len(code)
                orig = pe.get_data(addr - base, size)
                scores[addr] = round(score(code, relocs, orig, addr, names), 1)
        fresh[rel] = {"sha1": digest, "scores": {str(a): p for a, p in scores.items()}}
        out.update(scores)
    CACHE.parent.mkdir(exist_ok=True)
    CACHE.write_text(json.dumps(fresh))
    return out


def main(argv):
    sizes = match.orig_sizes()
    scores = stub_scores(sizes.get, force="--force" in argv)
    names = match.load_names()
    for a in sorted(scores):
        print(f"{a:08x} {scores[a]:5.1f} {names.get(a, '')}")


if __name__ == "__main__":
    main(sys.argv[1:])
