#!/usr/bin/env python3
"""Write an objdiff progress report (report format v2, JSON) for decomp.dev.

    tools/report.py [site/data.json] > report.json

Built from site/data.json (committed by the pre-commit hook), so it runs in
CI with plain Python: no exe, no compiler. Same numbers as the badge: game
.text only, library code ('l') left out. Units are our source files; code
not written yet goes into one auto-generated "unassigned" unit. A function
is 100% when matched, 0% otherwise (stubs are not measured).
"""
import json, sys
from collections import defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
REPORT_VERSION = 2


def measures(items):
    total = sum(s for _, s, _ in items)
    matched = sum(s for _, s, ok in items if ok)
    nfun = len(items)
    nmatch = sum(1 for _, _, ok in items if ok)
    pct = 100.0 * matched / total if total else 100.0
    return {
        "fuzzy_match_percent": pct,
        "total_code": str(total),
        "matched_code": str(matched),
        "matched_code_percent": pct,
        "total_functions": nfun,
        "matched_functions": nmatch,
        "matched_functions_percent": 100.0 * nmatch / nfun if nfun else 100.0,
    }


def main(argv):
    data = json.load(open(argv[0] if argv else ROOT / "site/data.json"))
    units = defaultdict(list)
    for addr, size, state, name, path, _saga, _method in data["functions"]:
        if state == "l":
            continue
        unit = path if state in ("m", "s") and path else "unassigned"
        units[unit].append((addr, size, state, name))

    report_units, everything = [], []
    for unit in sorted(units, key=lambda u: (u == "unassigned", u)):
        rows = sorted(units[unit])
        items = [(a, s, st == "m") for a, s, st, _ in rows]
        everything += items
        functions = []
        for a, s, st, name in rows:
            short = (name or "").split("(")[0].split(" ")[-1].lstrip("*&") or f"fn_{a:08x}"
            item = {"name": short, "size": str(s),
                    "fuzzy_match_percent": 100.0 if st == "m" else 0.0,
                    "address": str(a),
                    "metadata": {"virtual_address": str(a)}}
            if name:
                item["metadata"]["demangled_name"] = name
            functions.append(item)
        meta = {"complete": False}
        if unit == "unassigned":
            meta["auto_generated"] = True
        else:
            meta["source_path"] = unit
        report_units.append({
            "name": unit.removeprefix("src/").rsplit(".", 1)[0] if unit != "unassigned" else unit,
            "measures": measures(items),
            "functions": functions,
            "metadata": meta,
        })

    total = measures(everything)
    total["total_units"] = len(report_units)
    if int(total["total_code"]) != data["total"]:
        sys.exit(f"report: total {total['total_code']} != site/data.json total {data['total']}")
    json.dump({"measures": total, "units": report_units, "version": REPORT_VERSION},
              sys.stdout, separators=(",", ":"))


if __name__ == "__main__":
    main(sys.argv[1:])
