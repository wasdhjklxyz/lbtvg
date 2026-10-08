#!/usr/bin/env python3
"""Pair PC functions with Mac symbol names.

The macOS 1.0.1 build (orig/donors/mac/LEGO Batman) is unstripped and from the
same source. Compilers differ (MSVC vs GCC) so bytes never match, but:

  strings  a function that references a string referenced by exactly one
           function on each side is the same function (the string is unique
           on both sides);
  order    functions keep their source/link order on both sides, so between
           two paired anchors with the same number of functions, pair 1:1;
  calls    paired functions call their callees in the same order; where the
           callee counts agree, pair callees 1:1.

Writes tools/symbols/pc-names.csv: pc_addr,mac_addr,mac_name,method,round.
Usage: tools/macnames.py [--pc orig/LEGOBatman.exe] [--mac "orig/donors/mac/LEGO Batman"]
"""
import bisect, csv, re, subprocess, sys
from pathlib import Path
import pefile, lief
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone.x86 import X86_OP_IMM, X86_OP_MEM

ROOT = Path(__file__).resolve().parent.parent
PC = ROOT / "orig/LEGOBatman.exe"
MAC = ROOT / "orig/donors/mac/LEGO Batman"
STATS = ROOT / "ghidra/LEGOBatman.exe.stats.tsv"
OUT = ROOT / "tools/symbols/pc-names.csv"
MIN_STR = 6   # shorter strings are too likely to be shared format fragments
PATH = re.compile(r"[\\/][^\\/]+\.(?:cpp|c|h|hpp)$", re.I)

md = Cs(CS_ARCH_X86, CS_MODE_32); md.detail = True

class Side:
    def __init__(self, name):
        self.name = name
        self.funcs = []        # [(addr, size, label)] sorted
        self.read = None       # bytes at va
        self.str_ranges = []   # [(lo, hi)]
        self.strings = {}      # va -> str (cached)
        self.refs = {}         # addr -> [string va]
        self.calls = {}         # addr -> [callee addr]
        self.pc_thunks = {}     # thunk addr -> capstone register id it loads (Mac PIC)
        self.pointer_targets = {}  # va of a pointer slot -> what it points at
    def idx(self):
        self.starts = [a for a, _, _ in self.funcs]
    def containing(self, va):
        i = bisect.bisect_right(self.starts, va) - 1
        if i >= 0 and va < self.funcs[i][0] + self.funcs[i][1]:
            return self.funcs[i][0]
        return None
    def string_at(self, va):
        if va in self.strings: return self.strings[va]
        b = self.read(va, 256)
        s = b.split(b"\0")[0]
        v = s.decode("latin-1") if len(s) >= MIN_STR and all(32 <= c < 127 or c in (9, 10, 13) for c in s) else None
        self.strings[va] = v
        return v
    def scan(self):
        for addr, size, _ in self.funcs:
            code = self.read(addr, size)
            if not code: continue
            ins = list(md.disasm(code, addr))
            refs, calls = [], []
            pic = {}   # register -> value it holds after a get_pc_thunk call (Mac PIC)
            for k, i in enumerate(ins):
                if i.mnemonic == "pop" and k > 0 and ins[k - 1].mnemonic == "call" \
                        and ins[k - 1].operands[0].type == X86_OP_IMM \
                        and ins[k - 1].operands[0].imm == i.address:
                    # old Apple GCC PIC: `call 1f; 1: pop reg`, reg = address of the pop
                    pic[i.operands[0].reg] = i.address
                    continue
                if i.mnemonic == "call" and i.operands and i.operands[0].type == X86_OP_IMM:
                    tgt = i.operands[0].imm
                    if tgt == i.address + i.size:
                        continue
                    if tgt in self.pc_thunks:
                        pic[self.pc_thunks[tgt]] = i.address + i.size
                    else:
                        calls.append(tgt)
                for op in i.operands:
                    v = None
                    if op.type == X86_OP_IMM:
                        v = op.imm
                    elif op.type == X86_OP_MEM and op.mem.index == 0:
                        if op.mem.base == 0:
                            v = op.mem.disp
                        elif op.mem.base in pic:
                            v = pic[op.mem.base] + op.mem.disp
                    if v is not None and v in self.pointer_targets:
                        v = self.pointer_targets[v]
                    if v is not None and any(lo <= v < hi for lo, hi in self.str_ranges):
                        s = self.string_at(v)
                        if not s: continue
                        if PATH.search(s):
                            # __FILE__ differs per build (c:\\tt\\... vs /Volumes/...); the
                            # basename plus the line number pushed next to it is the
                            # same call site on both sides.
                            base = re.split(r"[\\/]", s)[-1].lower()
                            lines = set()
                            for j in ins[max(0, k - 4):k + 5]:
                                for o in j.operands:
                                    if o.type == X86_OP_IMM and 1 <= o.imm <= 20000: lines.add(o.imm)
                            refs += [f"{base}:{ln}" for ln in lines] or [base]
                        else:
                            refs.append(s)
            self.refs[addr] = refs
            self.calls[addr] = calls

def load_pc():
    s = Side("pc")
    pe = pefile.PE(str(PC)); base = pe.OPTIONAL_HEADER.ImageBase
    secs = {x.Name.rstrip(b"\0").decode(): x for x in pe.sections}
    s.read = lambda va, n: pe.get_data(va - base, n)
    rd = secs[".rdata"]; s.str_ranges = [(base + rd.VirtualAddress, base + rd.VirtualAddress + rd.Misc_VirtualSize)]
    rows = [l.rstrip("\n").split("\t") for l in open(STATS) if not l.startswith("#")]
    s.funcs = sorted((int(a, 16), int(sz), n) for a, sz, _, n in rows)
    s.idx(); return s

def load_mac():
    s = Side("mac")
    m = lief.MachO.parse(str(MAC)).at(0)
    segs = {}
    for sec in m.sections:
        segs[(sec.segment_name, sec.name)] = sec
    def read(va, n):
        for sec in m.sections:
            if sec.virtual_address <= va < sec.virtual_address + sec.size:
                c = bytes(sec.content); o = va - sec.virtual_address
                return c[o:o + n]
        return b""
    s.read = read
    s.str_ranges = [(sec.virtual_address, sec.virtual_address + sec.size) for (seg, nm), sec in segs.items() if nm in ("__cstring", "__const", "__data")]
    text = segs[("__TEXT", "__text")]
    syms = []
    from capstone.x86 import X86_REG_EAX, X86_REG_EBX, X86_REG_ECX, X86_REG_EDX, X86_REG_ESI, X86_REG_EDI, X86_REG_EBP
    regs = {"ax": X86_REG_EAX, "bx": X86_REG_EBX, "cx": X86_REG_ECX, "dx": X86_REG_EDX, "si": X86_REG_ESI, "di": X86_REG_EDI, "bp": X86_REG_EBP}
    for line in open(ROOT / "tools/symbols/mac-1.0.1.nm"):
        p = line.split()
        if len(p) == 3 and "get_pc_thunk" in p[2]:
            s.pc_thunks[int(p[0], 16)] = regs[p[2].rsplit(".", 1)[-1]]
        if len(p) == 3 and p[1] in ("T", "t"):
            a = int(p[0], 16)
            if text.virtual_address <= a < text.virtual_address + text.size:
                syms.append((a, p[2]))
    syms.sort()
    end = text.virtual_address + text.size
    for i, (a, n) in enumerate(syms):
        nxt = syms[i + 1][0] if i + 1 < len(syms) else end
        s.funcs.append((a, nxt - a, n))
    s.idx(); return s

def demangle(names):
    r = subprocess.run(["llvm-cxxfilt"], input="\n".join(names) + "\n", capture_output=True, text=True)
    return dict(zip(names, r.stdout.splitlines()))

def main():
    pc, mac = load_pc(), load_mac()
    print(f"pc: {len(pc.funcs)} functions, mac: {len(mac.funcs)} functions", file=sys.stderr)
    pc.scan(); mac.scan()
    # string -> unique function on each side
    def uniq(side):
        owners = {}
        for a, strs in side.refs.items():
            for st in set(strs):
                owners.setdefault(st, set()).add(a)
        return {st: next(iter(o)) for st, o in owners.items() if len(o) == 1}
    upc, umac = uniq(pc), uniq(mac)
    pairs = {}   # pc addr -> (mac addr, method, round)
    used = set()
    votes = {}
    for st, pa in upc.items():
        if st in umac:
            votes.setdefault(pa, {}).setdefault(umac[st], 0)
            votes[pa][umac[st]] += 1
    for pa, cand in votes.items():
        ma, n = max(cand.items(), key=lambda kv: kv[1])
        if len(cand) == 1 or n >= 2:
            if ma not in used:
                pairs[pa] = (ma, "strings", 0); used.add(ma)
    # whole-function string multisets that are unique on both sides
    def sigs(side):
        o = {}
        for a, r in side.refs.items():
            if len(r) >= 2:
                o.setdefault(tuple(sorted(r)), []).append(a)
        return {k: v[0] for k, v in o.items() if len(v) == 1}
    spc, smac = sigs(pc), sigs(mac)
    for k, pa in spc.items():
        ma = smac.get(k)
        if ma is not None and pa not in pairs and ma not in used:
            pairs[pa] = (ma, "strings", 0); used.add(ma)
    print(f"strings: {len(pairs)} pairs", file=sys.stderr)
    mac_by_addr = {a: (sz, n) for a, sz, n in mac.funcs}
    pc_list = [a for a, _, _ in pc.funcs]; mac_list = [a for a, _, _ in mac.funcs]
    pc_size = {a: s for a, s, _ in pc.funcs}
    def plausible(a, b):
        sa, sb = pc_size.get(a, 0), mac_by_addr[b][0]
        return sa > 0 and sb > 0 and 0.4 <= sb / sa <= 2.5
    for rnd in range(1, 12):
        before = len(pairs)
        # order: between consecutive anchors with equal gap counts
        anchors = sorted(pairs.items())
        mono = [(pa, ma) for pa, (ma, _, _) in anchors]
        for (p1, m1), (p2, m2) in zip(mono, mono[1:]):
            if m2 <= m1: continue
            pi1, pi2 = bisect.bisect_right(pc_list, p1), bisect.bisect_left(pc_list, p2)
            mi1, mi2 = bisect.bisect_right(mac_list, m1), bisect.bisect_left(mac_list, m2)
            gp, gm = pc_list[pi1:pi2], mac_list[mi1:mi2]
            if gp and len(gp) == len(gm) and len(gp) <= 8:
                for pa, ma in zip(gp, gm):
                    if pa not in pairs and ma not in used and plausible(pa, ma):
                        pairs[pa] = (ma, "order", rnd); used.add(ma)
        # calls: equal callee counts; a callee pair needs two independent
        # callers agreeing (one caller with coincidentally equal counts mispairs)
        cvotes = {}
        for pa, (ma, _, _) in list(pairs.items()):
            cp = [c for c in pc.calls.get(pa, []) if c in pc.calls]
            cm = [c for c in mac.calls.get(ma, []) if c in mac.calls]
            if cp and len(cp) == len(cm):
                for a, b in set(zip(cp, cm)):
                    if a not in pairs and b not in used:
                        cvotes.setdefault(a, {}).setdefault(b, set()).add(pa)
        for a, cand in cvotes.items():
            b, callers = max(cand.items(), key=lambda kv: len(kv[1]))
            if len(callers) >= 2 and len(cand) == 1 and a not in pairs and b not in used:
                pairs[a] = (b, "calls", rnd); used.add(b)
        # callee gap-fill: within a paired function, already-paired callees are
        # fixed points; equal-length runs between two fixed points pair 1:1
        for pa, (ma, _, _) in list(pairs.items()):
            cp = [c for c in pc.calls.get(pa, []) if c in pc.calls]
            cm = [c for c in mac.calls.get(ma, []) if c in mac.calls]
            fixed = [(-1, -1)]
            j = 0
            for i, a in enumerate(cp):
                if a in pairs:
                    b = pairs[a][0]
                    try:
                        k = cm.index(b, j)
                    except ValueError:
                        continue
                    fixed.append((i, k)); j = k + 1
            fixed.append((len(cp), len(cm)))
            for (i1, k1), (i2, k2) in zip(fixed, fixed[1:]):
                gp, gm = cp[i1 + 1:i2], cm[k1 + 1:k2]
                if len(gp) == 1 and len(gm) == 1:
                    for a, b in zip(gp, gm):
                        if a not in pairs and b not in used and plausible(a, b):
                            pairs[a] = (b, "gapfill", rnd); used.add(b)
        print(f"round {rnd}: {len(pairs)} pairs (+{len(pairs) - before})", file=sys.stderr)
        if len(pairs) == before: break
    # precision check: among pairs whose callee lists have equal length, how
    # often do already-paired callees agree position by position?
    agree = total = 0
    for pa, (ma, _, _) in pairs.items():
        cp, cm = pc.calls.get(pa, []), mac.calls.get(ma, [])
        if len(cp) == len(cm):
            for a, b in zip(cp, cm):
                if a in pairs:
                    total += 1; agree += pairs[a][0] == b
    print(f"callee consistency: {agree}/{total}", file=sys.stderr)
    names = demangle([mac_by_addr[ma][1] for ma, _, _ in pairs.values()])
    with open(OUT, "w", newline="") as f:
        w = csv.writer(f); w.writerow(["pc_addr", "mac_addr", "mac_name", "demangled", "method", "round"])
        for pa, (ma, how, rnd) in sorted(pairs.items()):
            mn = mac_by_addr[ma][1]
            w.writerow([f"{pa:08x}", f"{ma:08x}", mn, names.get(mn, mn), how, rnd])
    print(f"wrote {OUT} ({len(pairs)} rows)", file=sys.stderr)

if __name__ == "__main__":
    main()
