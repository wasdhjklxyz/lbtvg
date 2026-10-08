# lbtvg — project rules

Matching decompilation of LEGO Batman: The Videogame (PC, 2008). Read
`docs/recon.md` (binary recon, toolchain, scope) and `docs/plan.md` (approach)
before touching the binary.

## hard rules
- **Never write to `orig/`.** `orig/LEGOBatman.exe` (2025 re-link, target) and
  `orig/testapp.exe` (2008 SteamStub original) are read-only reference. Run
  `make verify` after anything that opened them.
- `game/` is a symlink into the Steam install. Never read the exes from there.
- Don't guess struct layouts or function roles. Show xrefs/decomp evidence.
- Byte-match is the test. "Looks right" is not a state.

## facts (don't re-derive)
- Toolchain: VS2005 (`cl` 14.00.50727, `link` 8.00.50727), static CRT `/MT`,
  no LTCG/PGO, x87 float, `/O2`-style, `/GS`, `/EHsc`. Assume SP1 until a CRT
  function proves otherwise.
- Source tree: `nu2api/` (NuCore engine), `gameapi/`, `batman/`. Mirror it.
- Game `.text` denominator ≈ 3.39 MB (`0x401000`–`0x73c8fb`); CRT/libs after.

## commands
- `nix develop` first. Everything below assumes the devshell.
- `make verify` — sha1 both exes.
- `make ghidra-import` — headless import+analyze into `ghidra/` (gitignored).
- `make ghidra` — open the project in the GUI (ReVa MCP listens on :8080 once
  enabled in the GUI; `.mcp.json` points at it).

## one-time ghidra GUI setup (after first `make ghidra`)
Extensions are baked in by the flake; plugins still need enabling once:
- Project window: File > Configure > (plug icon) "Configure All Plugins" >
  tick **ReVa Application Plugin**.
- CodeBrowser: File > Configure > Configure All Plugins > tick **ReVa Plugin**
  and (Experimental) **RelocationTableSynthesizedPlugin** (delinker). Then
  File > Save Tool.
- ReVa listens on `localhost:8080` by default. `/mcp` in claude should show
  it connected while the GUI is open.
- Missed-function recovery: Window > **Random Forest Function Finder**
  (MachineLearning ext) — train on the FID-named CRT functions, run over
  `.text`, then re-run `DumpStats.java` from the Script Manager.
