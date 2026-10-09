#!/usr/bin/env python3
"""Start work on a function: pick its file, put it there, try it.

    tools/new.py 0x005ae160          (or: make new FUNC=0x005ae160)
    tools/new.py random              (or: make new) a random todo: small, saga has it
    tools/new.py random any          a random todo of any kind

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


def saga_files_in(path):
    """Saga files the functions in one of our files were ported from."""
    return set(re.findall(r"//\s*from saga (\S+)", path.read_text(errors="ignore"))) if path.exists() else set()


def choose_file(addr, name, saga_path, done, anc):
    # a neighbour whose code came from a different saga file is a different TU
    if saga_path:
        done = {a: f for a, f in done.items()
                if not (saga_files_in(f) and saga_path not in saga_files_in(f))}
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
        [f'/I"Z:{match.VC8 / "INCLUDE"}"', f'/I"Z:{match.DXSDK / "Include"}"', f'/I"Z:{match.WINSDK6 / "Include"}"', f'/Fo"Z:{obj}"', f'"Z:{path}"']
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


SAGA_H = None


def saga_headers():
    global SAGA_H
    if SAGA_H is None:
        SAGA_H = [(f, f.read_text(errors="ignore")) for f in sorted((ROOT / "ref/saga/src").rglob("*.h"))]
    return SAGA_H


def saga_proto(ident):
    """saga's declaration of function ident, as a prototype line."""
    pat = re.compile(r"^[ \t]*(?!return\b|#|//)([\w][\w\s\*&:<>,]*?\b" + re.escape(ident) +
                     r"\s*\([^;{)]*\))\s*;", re.M)
    for f, text in saga_headers():
        m = pat.search(text)
        if m:
            return m.group(1).strip() + ";"
    for path, line, body in saga.find(ident):
        return body[:body.index("{")].strip() + ";"
    return None


def balanced_block(text, start):
    """From start through the matching close brace and its trailing `name;`."""
    i = text.index("{", start)
    depth = 0
    for j in range(i, len(text)):
        depth += text[j] == "{"
        depth -= text[j] == "}"
        if depth == 0:
            end = text.index(";", j)
            return text[start:end + 1]
    return None


def our_header_for(ident):
    """A header under src/ that already defines type ident."""
    pat = re.compile(r"(struct|union|enum)\s+" + re.escape(ident) + r"\s*\{|\}\s*" + re.escape(ident) +
                     r"\s*;|typedef\b[^;{]*\b" + re.escape(ident) + r"\s*;")
    for h in sorted(SRC.rglob("*.h")):
        if pat.search(h.read_text(errors="ignore")):
            return h
    return None


def saga_type(ident):
    """saga's definition of type ident (struct body and/or typedef)."""
    out = []
    for f, text in saga_headers():
        m = re.search(r"^[ \t]*(typedef\s+)?(struct|union|enum)\s+" + re.escape(ident) + r"\b[^;{]*\{", text, re.M)
        if m:
            out.append(balanced_block(text, m.start()))
            break
        m = re.search(r"^[ \t]*typedef\s+(struct|union|enum)\s+\w*\s*\{", text, re.M)
        while m:
            blk = balanced_block(text, m.start())
            if blk and re.search(r"\}\s*[^;]*\b" + re.escape(ident) + r"\b[^;]*;$", blk):
                out.append(blk)
                break
            m = re.compile(r"^[ \t]*typedef\s+(struct|union|enum)\s+\w*\s*\{", re.M).search(text, m.end())
        if out:
            break
        m = re.search(r"^[ \t]*typedef\s+(?:struct\s+|union\s+|enum\s+)?(\w+)[\s\*]*\b" + re.escape(ident) + r"\s*;", text, re.M)
        if m:
            inner = saga_type(m.group(1)) if m.group(1) != ident else None
            out += ([inner] if inner else []) + [m.group(0).strip()]
            break
    return "\n".join(x for x in out if x) or None


def missing_from(errs, path):
    """(kind, ident) pairs the compiler complained about."""
    want = []
    src_lines = path.read_text(errors="ignore").splitlines()
    for e in errs:
        m = re.search(r"C3861: '(\w+)': identifier not found", e)
        if m:
            want.append(("func", m.group(1))); continue
        m = re.search(r"C2065: '(\w+)' : undeclared identifier", e)
        if m:
            want.append(("var", m.group(1))); continue
        m = re.search(r"C20(?:61|79|27)[^']*'(?:struct )?(\w+)'", e)
        if m:
            want.append(("type", m.group(1))); continue
        m = re.search(r"\((\d+)\) : error C2146: syntax error : missing ';' before identifier '(\w+)'", e)
        if m and int(m.group(1)) <= len(src_lines):
            tm = re.search(r"(\w+)[\s\*&]+" + re.escape(m.group(2)) + r"\b", src_lines[int(m.group(1)) - 1])
            if tm:
                want.append(("type", tm.group(1)))
    return list(dict.fromkeys(want))


def resolve(kind, ident, saga_file, path):
    """Text to put before the function, and a label, or (None, None)."""
    if kind in ("type", "var"):
        h = our_header_for(ident)
        if h:
            return f'#include "{rel_include(path, h.relative_to(SRC).as_posix())}"', f"{ident} (our {h.relative_to(SRC)})"
        ty = saga_type(ident)
        if ty:
            return ty, f"{ident} (type, saga)"
    if kind == "var":
        d = saga_decl(ident, saga_file)
        if d:
            return d, f"{ident} (global, saga)"
    if kind in ("func", "var"):
        p = saga_proto(ident)
        if p:
            return p, f"{ident}() (saga)"
    return None, None


def pick_random(kind):
    """A random entry from docs/todo.md; by default small, not a stub, saga has it."""
    import random
    rows = []
    for l in (ROOT / "docs/todo.md").read_text().splitlines():
        m = re.match(r"- \[ \] `([0-9a-f]{8})` (\d+) B `([^`]*)`(.*)", l)
        if m:
            rows.append((int(m.group(1), 16), int(m.group(2)), m.group(3), m.group(4)))
    if kind != "any":
        easy = [r for r in rows if "**saga**" in r[3] and "**stub**" not in r[3] and r[1] <= 160]
        rows = easy or rows
    if not rows:
        sys.exit("todo list is empty")
    a, size, name, _ = random.choice(rows)
    print(f"random pick ({'any' if kind == 'any' else 'small, saga has it'}): {a:08x} {size} B {name}\n")
    return a


def main(argv):
    sys.stdout.reconfigure(line_buffering=True)
    if argv and argv[0] == "random":
        argv = [f"0x{pick_random(argv[1] if len(argv) > 1 else ''):08x}"]
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
        dep_list, fetched, tried = [], [], set()
        joined = lambda xs: "\n\n".join(xs)
        rank = lambda s: 0 if s.startswith("#include") else 1 if re.match(r"\s*(typedef|struct|union|enum)\b", s) else 2
        for _ in range(12):                      # fetch what the compiler says is missing, retry
            if ok:
                break
            add = []
            for kind, ident in missing_from(errs, path):
                if (kind, ident) in tried:
                    continue
                tried.add((kind, ident))
                text_, label = resolve(kind, ident, saga_path, path)
                if text_ and text_ not in dep_list and text_ not in add:
                    add.append(text_); fetched.append(label)
            if not add:
                break
            old_text = (joined(dep_list) + "\n\n" if dep_list else "") + block
            dep_list = sorted(dep_list + add, key=rank)   # includes, then types, then the rest
            text = path.read_text()
            path.write_text(text.replace(old_text, joined(dep_list) + "\n\n" + block))
            ok, errs = compiles(path)
        deps = joined(dep_list)
        if fetched:
            print("pulled:  " + ", ".join(fetched))
        if not ok:
            text = path.read_text()
            reason = (errs[0].split(" error ", 1)[-1] if errs else "compile failed")[:100]
            parked = (f"// STUB: LEGOBATMAN 0x{addr:08x}\n// does not compile yet: {reason}\n#if 0\n"
                      + (deps + "\n\n" if deps else "") + f"// from saga {saga_path}\n{body}\n#endif")
            path.write_text(text.replace((deps + "\n\n" if deps else "") + block, parked))
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
