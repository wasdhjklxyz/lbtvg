#!/usr/bin/env python3
"""Start work on a function: pick its file, put it there, try it.

    tools/new.py 0x005ae160          (or: make new FUNC=0x005ae160)

1. Name: from src/ if already written (then it just says where), else the
   Mac name (tools/symbols/pc-names.csv), else FUN_<addr>.
2. File, by evidence, strongest first:
   - written neighbours on both sides in the same file (functions of one
     source file are contiguous in the exe) -> that file
   - a written neighbour close by with no other source file's anchor in
     between -> that file
   - an existing file for the same subsystem (Grabber_* -> grabber_unk.cpp)
   - saga's file name, placed in our tree
   - otherwise <tree>/unk_<addr>.cpp, tree from the nearest docs/linkmap.md anchor
3. Body: saga's, if saga has the function: inserted in address order as a
   // FUNCTION:, compiled, and matched right away. If it does not compile
   yet it is parked in `#if 0` as a // STUB: with the compiler error, so
   the rest of the file keeps working. No saga body: a TODO comment with
   the Mac signature, for you to write.
4. Prints the original disassembly.
"""
import csv, re, subprocess, sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
import match  # noqa: E402  (VC8 paths and flags)
import saga   # noqa: E402

SRC = ROOT / "src"
NEAR = 0x4000                     # a neighbour this close is probably the same file
ANNOT = re.compile(r"//\s*(FUNCTION|STUB):\s*LEGOBATMAN\s+0x([0-9a-fA-F]+)")


def annotated():
    out = {}
    for f in sorted(list(SRC.rglob("*.c")) + list(SRC.rglob("*.cpp"))):
        for _, a in ANNOT.findall(f.read_text(errors="ignore")):
            out.setdefault(int(a, 16), f)
    return out


def anchors():
    out = []
    for m in re.finditer(r"^\| `([0-9a-f]{8})` \| `([^`]+)`", (ROOT / "docs/linkmap.md").read_text(), re.M):
        out.append((int(m.group(1), 16), m.group(2)))
    return sorted(out)


def foreign_anchor(lo, hi, file, anc):
    """Is there an anchor of a different source file strictly inside (lo, hi]?"""
    stem = file.stem.replace("_unk", "").lower()
    return any(lo < a <= hi and Path(p).stem.lower() != stem for a, p in anc)


def short_name(full):
    base = full.split("(")[0].strip()
    if " " in base and "operator" not in base:
        base = base.split(" ")[-1]
    return base.lstrip("_*&")


def group_of(name):
    if "::" in name:
        return name.split("::")[0]
    if "_" in name.strip("_"):
        return name.strip("_").split("_")[0]
    m = re.match(r"(Nu[A-Z][a-z0-9]+)", name)
    return m.group(1) if m else None


def tree_for(saga_path, addr, anc):
    if saga_path:
        parts = saga_path.split("/")
        if parts[0] in ("nu2api", "gameapi") and len(parts) > 2:
            return SRC / parts[0] / parts[1]
        if parts[0] == "nu2api":
            return SRC / "nu2api"
    prev = [p for a, p in anc if a <= addr]
    if prev:
        p = prev[-1].lstrip("./").split("/")
        if p[0] in ("nu2api", "gameapi"):
            return SRC / p[0]
    return SRC / "batman"


def choose_file(addr, name, saga_path, done, anc):
    before = [a for a in done if a < addr]
    after = [a for a in done if a > addr]
    pa, na = (max(before) if before else None), (min(after) if after else None)
    pf, nf = done.get(pa), done.get(na)
    if pf and pf == nf and not foreign_anchor(pa, na, pf, anc):
        return pf, f"between {pa:08x} and {na:08x}, both already in this file"
    if pf and addr - pa < NEAR and not foreign_anchor(pa, addr, pf, anc):
        return pf, f"nearest written neighbour {pa:08x} is {addr - pa:#x} bytes before, same file likely"
    if nf and na - addr < NEAR and not foreign_anchor(addr, na, nf, anc):
        return nf, f"nearest written neighbour {na:08x} is {na - addr:#x} bytes after, same file likely"
    g = group_of(name) if name else None
    if g:
        for f in sorted(SRC.rglob(f"{g.lower()}*_unk*.cpp")):
            return f, f"same subsystem as existing {f.name}"
    tree = tree_for(saga_path, addr, anc)
    if saga_path:
        return tree / (Path(saga_path).stem + "_unk.cpp"), f"saga keeps it in {saga_path}"
    if g:
        return tree / (g.lower() + "_unk.cpp"), f"new file for the {g} subsystem"
    return tree / f"unk_{addr:08x}.cpp", "no evidence; parked by address"


def rel_include(from_file, target):
    import os
    return os.path.relpath(SRC / target, from_file.parent).replace("\\", "/")


def insert(path, addr, block):
    """Insert block before the first annotated function with a higher address."""
    if not path.exists():
        path.parent.mkdir(parents=True, exist_ok=True)
        head = [f"// {path.relative_to(SRC).as_posix()}: placed by tools/new.py; file name unproven.",
                "", f'#include "{rel_include(path, "nu2api/nucore/common.h")}"', "#include <stddef.h>", ""]
        path.write_text("\n".join(head) + "\n" + block + "\n")
        return
    lines = path.read_text().splitlines()
    at = len(lines)
    for i, l in enumerate(lines):
        m = ANNOT.search(l)
        if m and int(m.group(2), 16) > addr:
            at = i
            while at > 0 and lines[at - 1].lstrip().startswith("//") and not ANNOT.search(lines[at - 1]):
                at -= 1
            break
    new = lines[:at] + ([""] if at and lines[at - 1].strip() else []) + block.splitlines() + [""] + lines[at:]
    path.write_text("\n".join(new).rstrip("\n") + "\n")


def compiles(path):
    obj = ROOT / "build/new-check.obj"
    obj.parent.mkdir(exist_ok=True)
    cmd = ["wibo", str(match.VC8 / "Bin/cl.exe")] + \
        [f for f in match.CFLAGS if not (path.suffix == ".c" and f == "/EHsc")] + \
        [f'/I"Z:{match.VC8 / "INCLUDE"}"', f'/I"Z:{match.WINSDK6 / "Include"}"', f'/Fo"Z:{obj}"', f'"Z:{path}"']
    r = subprocess.run(" ".join(cmd), shell=True, capture_output=True, text=True)
    errs = [l for l in (r.stdout + r.stderr).splitlines() if " error " in l]
    return r.returncode == 0, errs


def saga_decl(ident, saga_file):
    """A file-scope declaration of ident in saga (its own file first)."""
    files = [ROOT / "ref/saga/src" / saga_file] if saga_file else []
    files += sorted((ROOT / "ref/saga/src").rglob("*.h"))
    pat = re.compile(r"^(?!\s|#|//|return\b)[\w][\w\s\*:<>,]*\b" + re.escape(ident) +
                     r"\b\s*(\[[^\]]*\])*\s*(=[^;]*)?;", re.M)
    for f in files:
        if f.exists():
            m = pat.search(f.read_text(errors="ignore"))
            if m:
                return m.group(0).replace("extern ", "")
    return None


def main(argv):
    sys.stdout.reconfigure(line_buffering=True)
    if len(argv) != 1 or not argv[0].lower().startswith("0x"):
        sys.exit(__doc__)
    addr = int(argv[0], 16)
    matched = False
    done = annotated()
    if addr in done:
        print(f"{addr:08x} is already in {done[addr].relative_to(ROOT)}")
        print(f"  make match FUNC=0x{addr:08x}")
        return
    names = {int(r["pc_addr"], 16): r["demangled"] for r in csv.DictReader(open(ROOT / "tools/symbols/pc-names.csv"))}
    full = names.get(addr)
    name = short_name(full) if full else None
    hits = saga.find(name) if name else []
    hits.sort(key=lambda h: ("android" in str(h[0]), str(h[0])))
    saga_path = str(hits[0][0]).split("ref/saga/src/")[-1] if hits else None

    path, why = choose_file(addr, name or "", saga_path, done, anchors())
    rel = path.relative_to(ROOT)
    print(f"{addr:08x}  {full or 'FUN_%08x' % addr}")
    print(f"file:  {rel}  ({why})")

    if hits:
        body = hits[0][2].strip()
        block = f"// from saga {saga_path}\n// FUNCTION: LEGOBATMAN 0x{addr:08x}\n{body}"
        insert(path, addr, block)
        ok, errs = compiles(path)
        fetched = []
        for _ in range(6):                       # pull missing globals from saga, retry
            if ok:
                break
            missing = re.findall(r"error C2065: '(\w+)' : undeclared identifier", "\n".join(errs))
            decls = [(m, saga_decl(m, saga_path)) for m in dict.fromkeys(missing)]
            decls = [(m, d) for m, d in decls if d and m not in fetched]
            if not decls:
                break
            text = path.read_text()
            add = "\n".join(d for _, d in decls)
            path.write_text(text.replace(block, add + "\n\n" + block))
            block = add + "\n\n" + block
            fetched += [m for m, _ in decls]
            ok, errs = compiles(path)
        if fetched:
            print(f"globals: pulled from saga: {', '.join(fetched)}")
        if not ok:
            text = path.read_text()
            reason = (errs[0].split(" error ", 1)[-1] if errs else "compile failed")[:100]
            parked = (f"// STUB: LEGOBATMAN 0x{addr:08x}\n// does not compile yet: {reason}\n#if 0\n"
                      f"// from saga {saga_path}\n{body}\n#endif")
            path.write_text(text.replace(block, parked))
            print(f"saga:  {saga_path}  -> inserted, but it does not compile yet; parked in #if 0 as a STUB:")
            for e in errs[:6]:
                print("       " + e.split(str(SRC) + "/")[-1])
            print("       declare the missing types/functions, remove the #if 0, set it back to // FUNCTION:")
        else:
            print(f"saga:  {saga_path}  -> inserted and compiles; matching now")
            matched = subprocess.run([sys.executable, str(ROOT / "tools/match.py"), "-v", f"0x{addr:08x}"]).returncode == 0
    else:
        sig = full or f"FUN_{addr:08x}"
        insert(path, addr, f"// TODO LEGOBATMAN 0x{addr:08x}  mac: {sig}\n"
                           f"// write it here, then put // FUNCTION: LEGOBATMAN 0x{addr:08x} above it")
        print("saga:  no body; added a TODO with the Mac signature")
    print()
    subprocess.run([sys.executable, str(ROOT / "tools/disasm.py"), f"0x{addr:08x}"])
    if matched:
        print(f"\nmatched straight from saga. next:  make fmt && git add {rel} && git commit")
    else:
        print(f"\nnext:  edit {rel}, then  make match FUNC=0x{addr:08x}")


if __name__ == "__main__":
    main(sys.argv[1:])
