#!/usr/bin/env python3
"""Progress: badge, map and todo list, all derived from src/ annotations.

    tools/progress.py            print the summary
    tools/progress.py --write    also rewrite README badge, docs/progress.svg,
                                 docs/todo.md (the pre-commit hook runs this)

Inputs (all tracked, so this works on a fresh clone):
  tools/symbols/functions.tsv   every function ghidra found in game code
  tools/symbols/pc-names.csv    PC <-> Mac 1.0.1 names
  docs/linkmap.md               leaked source-file anchors
  src/**                        // FUNCTION: / // STUB: annotations
  ref/saga/src (optional)       marks todo items that already have a saga body

Library functions (FID/EH-named by ghidra) are excluded from the denominator.
"""
import csv, re, sys
from collections import defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
TEXT_LO, TEXT_HI = 0x00401000, 0x0073C8FB  # game region of .text (docs/recon.md)
ANNOT = re.compile(r"//\s*(FUNCTION|STUB):\s*LEGOBATMAN\s+0x([0-9a-fA-F]+)")

MATCHED, STUB, NAMED, UNKNOWN = "matched", "stub", "named", "unknown"
COLOR = {MATCHED: "#2da44e", STUB: "#d4a72c", NAMED: "#5b8bd6", UNKNOWN: "#3a3f47"}
LABEL = {MATCHED: "matched", STUB: "stub (written, not byte-identical)",
         NAMED: "todo, real name known", UNKNOWN: "todo, unnamed"}


def load():
    funcs = {}  # addr -> (size, ghidra name)
    for line in open(ROOT / "tools/symbols/functions.tsv"):
        if line.startswith("#"):
            continue
        a, s, n = line.rstrip("\n").split("\t")
        funcs[int(a, 16)] = (int(s), n)
    lib = {a for a, (_, n) in funcs.items() if not n.startswith(("FUN_", "thunk_"))}
    names = {}
    for r in csv.DictReader(open(ROOT / "tools/symbols/pc-names.csv")):
        names[int(r["pc_addr"], 16)] = r["demangled"]
    state = {}
    for f in list(ROOT.glob("src/**/*.c")) + list(ROOT.glob("src/**/*.cpp")) + list(ROOT.glob("src/**/*.h")):
        for kind, a in ANNOT.findall(f.read_text(errors="ignore")):
            a = int(a, 16)
            if state.get(a) != MATCHED:
                state[a] = MATCHED if kind == "FUNCTION" else STUB
    for a in funcs:
        if a not in state:
            state[a] = NAMED if a in names else UNKNOWN
    return funcs, lib, names, state


def saga_bodies():
    root = ROOT / "ref/saga/src"
    defs = {}
    if not root.is_dir():
        return defs
    pat = re.compile(r"^[A-Za-z_][\w\s\*&:<>,]*?\b((?:\w+::)?~?\w+)\s*\([^;{]*\)\s*(?:const\s*)?\{", re.M)
    for f in list(root.rglob("*.c")) + list(root.rglob("*.cpp")):
        for m in pat.finditer(f.read_text(errors="ignore")):
            defs.setdefault(m.group(1), f.relative_to(root).as_posix())
    return defs


def anchors():
    out = []
    p = ROOT / "docs/linkmap.md"
    if p.exists():
        for m in re.finditer(r"^\| `([0-9a-f]{8})` \| `([^`]+)`", p.read_text(), re.M):
            out.append((int(m.group(1), 16), m.group(2).split("/")[-1]))
    return sorted(out)


def summary(funcs, lib, state):
    game = {a: s for a, (s, _) in funcs.items() if a not in lib}
    total = sum(game.values())
    by = defaultdict(lambda: [0, 0])
    for a, s in game.items():
        by[state[a]][0] += 1
        by[state[a]][1] += s
    pct = 100.0 * by[MATCHED][1] / total if total else 0.0
    return total, by, pct


# --- README badge -------------------------------------------------------------
def write_badge(pct):
    color = ("brightgreen" if pct >= 90 else "green" if pct >= 70 else
             "yellow" if pct >= 50 else "orange" if pct >= 30 else "red")
    badge = f"https://img.shields.io/badge/match%20progress-{pct:.2f}%25-{color}"
    readme = ROOT / "README.md"
    t = re.sub(r"https://img\.shields\.io/badge/match%20progress-[^)]*", badge, readme.read_text())
    readme.write_text(t)


# --- docs/progress.svg: game code in address order -----------------------------
def write_svg(funcs, lib, state, total, by, pct):
    ROW = 16384                  # bytes per row
    W, H, LEFT, TOP = 1024, 3, 110, 46
    rows = (TEXT_HI - TEXT_LO + ROW - 1) // ROW
    color = dict(COLOR, lib="#24292f", gap="#161b22")
    bpp = ROW / W                # bytes per pixel
    pix = [[{} for _ in range(W)] for _ in range(rows)]
    for a in sorted(funcs):
        s, _ = funcs[a]
        st = state[a] if state[a] in (MATCHED, STUB) else "lib" if a in lib else UNKNOWN
        lo, hi = a, a + s
        while lo < hi:
            off = lo - TEXT_LO
            r, px = int(off // ROW), int((off % ROW) // bpp)
            if r >= rows:
                break
            nxt = min(hi, TEXT_LO + r * ROW + (px + 1) * bpp)
            cell = pix[r][px]
            cell[st] = cell.get(st, 0) + (nxt - lo)
            lo = nxt
    rank = {MATCHED: 4, STUB: 3, NAMED: 2, UNKNOWN: 1, "lib": 0}
    out = []
    height = TOP + rows * (H + 1) + 10
    out.append(f'<svg xmlns="http://www.w3.org/2000/svg" width="{LEFT + W + 10}" height="{height}" '
               f'font-family="ui-monospace,monospace" font-size="10">')
    out.append('<rect width="100%" height="100%" fill="#0d1117"/>')
    out.append(f'<text x="10" y="16" fill="#e6edf3" font-size="12">LEGOBatman.exe game code: '
               f'{by[MATCHED][0]} functions matched, {pct:.2f}% of {total:,} bytes '
               f'(one row = {ROW // 1024} KB, address order)</text>')
    x = 10
    legend = [(MATCHED, f"matched ({by[MATCHED][0]})"), (STUB, f"stub ({by[STUB][0]})"),
              (UNKNOWN, f"todo ({by[NAMED][0] + by[UNKNOWN][0]}, {by[NAMED][0]} with a known name)")]
    for st, lab in legend:
        out.append(f'<rect x="{x}" y="26" width="10" height="10" fill="{COLOR[st]}"/>'
                   f'<text x="{x + 14}" y="35" fill="#9da7b3">{lab}</text>')
        x += 14 + 6 * len(lab) + 18
    paths = defaultdict(list)    # colour -> "Mx yh..." segments, far smaller than rects
    for r in range(rows):
        y = TOP + r * (H + 1)
        paths["gap"].append(f"M{LEFT} {y}h{W}v{H}h-{W}z")
        run_st, run_x = None, 0
        for px in range(W + 1):
            cell = pix[r][px] if px < W else {}
            # a pixel shows any matched/stub bytes it holds, else its majority
            st = max(cell, key=lambda k: (rank[k] >= 3 and cell[k] > 0, cell[k], rank[k])) if cell else "gap"
            if st != run_st:
                if run_st not in (None, "gap"):
                    w = px - run_x
                    paths[run_st].append(f"M{LEFT + run_x} {y}h{w}v{H}h-{w}z")
                run_st, run_x = st, px
    for st in ("gap", "lib", UNKNOWN, NAMED, STUB, MATCHED):
        if paths[st]:
            out.append(f'<path fill="{color[st]}" d="{"".join(paths[st])}"/>')
    last = -10
    for a, name in anchors():
        r = (a - TEXT_LO) // ROW
        if 0 <= r < rows and r - last >= 3:
            out.append(f'<text x="{LEFT - 4}" y="{TOP + r * (H + 1) + 4}" fill="#6e7681" '
                       f'font-size="8" text-anchor="end">{name}</text>')
            last = r
    out.append("</svg>")
    (ROOT / "docs/progress.svg").write_text("\n".join(out) + "\n")


# --- docs/todo.md: named functions not done yet --------------------------------
def group_of(name):
    base = name.split("(")[0].strip()
    base = base.split(" ")[-1] if " " in base and "operator" not in base else base
    if "::" in base:
        return base.split("::")[0]
    base = base.lstrip("_")
    if "_" in base:
        return base.split("_")[0]
    m = re.match(r"(Nu[A-Z][a-z0-9]+)", base)
    return m.group(1) if m else None


def write_todo(funcs, state, names, saga, by):
    groups = defaultdict(list)
    anc = anchors()
    for a, (s, _) in funcs.items():
        if state[a] in (NAMED, STUB) and a in names:
            n = names[a]
            short = n.split("(")[0].lstrip("_")
            g = group_of(n)
            if g is None:
                prev = [f for x, f in anc if x <= a]
                g = f"near {prev[-1]}" if prev else "start of .text"
            groups[g].append((s, a, n, state[a], saga.get(short)))
    lines = [
        "# todo",
        "",
        "Generated by `tools/progress.py` on every commit (pre-commit hook); do not",
        "edit. A function disappears from here once a `// FUNCTION:` annotation for",
        "it lands in `src/`. Only functions whose real name is known (paired from",
        "the Mac build) are listed; the other "
        f"{by[UNKNOWN][0]:,} are in `tools/symbols/functions.tsv`.",
        "",
        "Smallest first inside each group. **saga** = opensagadev/saga has a body",
        "for it under `ref/saga/src/` (a starting point, not a guaranteed match).",
        "**stub** = written but not byte-identical yet.",
        "",
    ]
    order = sorted(groups.items(), key=lambda kv: (-sum(1 for x in kv[1] if x[4]), kv[0].lower()))
    lines.append("| group | todo | with saga body |")
    lines.append("|---|---|---|")
    for g, items in order:
        slug = re.sub(r"[^a-z0-9 _-]", "", g.lower()).replace(" ", "-")
        lines.append(f"| [{g}](#{slug}) | {len(items)} | {sum(1 for x in items if x[4])} |")
    lines.append("")
    for g, items in order:
        lines.append(f"## {g}")
        lines.append("")
        for s, a, n, st, sg in sorted(items):
            tags = []
            if st == STUB:
                tags.append("**stub**")
            if sg:
                tags.append(f"**saga** `{sg}`")
            lines.append(f"- [ ] `{a:08x}` {s} B `{n}`" + ("  " + " · ".join(tags) if tags else ""))
        lines.append("")
    (ROOT / "docs/todo.md").write_text("\n".join(lines))


def main(argv):
    funcs, lib, names, state = load()
    total, by, pct = summary(funcs, lib, state)
    print(f"{by[MATCHED][0]} functions matched, {by[MATCHED][1]:,} / {total:,} bytes of game code "
          f"({pct:.2f}%); {by[STUB][0]} stubs; {by[NAMED][0]} named todo; {by[UNKNOWN][0]} unnamed todo.")
    if "--write" in argv or "--readme" in argv:
        write_badge(pct)
        write_svg(funcs, lib, state, total, by, pct)
        write_todo(funcs, state, names, saga_bodies(), by)


if __name__ == "__main__":
    main(sys.argv[1:])
