# setup

How to get a working environment for this project on any OS. Nothing here
assumes NixOS; the `nix develop` path is just the one where every version
below is pinned for you.

## what you need

| tool | version used here | why |
|---|---|---|
| git, GNU make | any | repo, `make` targets |
| `sha1sum` (coreutils) or `shasum` | any | `make verify` |
| Ghidra | **12.1.2** | disassembly / decompilation; the project database lives in `ghidra/` |
| JDK | 21 | Ghidra 12 requirement |
| ReVa (Ghidra extension) | 7.3.0 | MCP server so an AI client (claude, etc.) can read/rename inside the open project. Optional. |
| ghidra-delinker-extension | 0.8.0 | export ranges of the original binary as `.obj` so objdiff can compare original vs recompiled per object |
| MachineLearning (Ghidra bundled extension) | ships with 12.1.2 | random-forest function finder; relocs are stripped so plain analysis misses vtable-only code |
| Python 3 | 3.14 (≥3.10 fine) | recon scripts |
| pefile / lief / capstone | 2024.8.26 / 0.17.6 / 5.0.7 | PE parsing, disassembly in scripts (`tools/requirements.txt`) |
| objdiff | 3.8.2 | per-object diffing once there's a compiler |
| DepotDownloader | 3.4.0 (.NET 9) | pull the macOS depot of app 21000 (possible symbol source). Optional. |
| binutils (`objdump`, `strings`) | any | quick looks; optional |

Not yet covered: the matching compiler (VS2005 SP1 `cl` 14.00.50727.762 under
wine) and the compare harness (reccmp). See `docs/recon.md` → "tomorrow".

## the game

You need your own copy. Steam app 21000. Copy, don't symlink:

```
orig/LEGOBatman.exe   2025-07-04 re-link, unpacked — the target
orig/testapp.exe      2008-09-05 original, SteamStub-packed — reference
```

Then `make verify`. The sha1s in `orig/checksum.sha1` are the only
definition of "the binary". If yours don't match, stop and figure out why
before anything else (different Steam build, different region, modified).

`orig/*.exe` is gitignored and must never be written to by any tool. Ghidra
import reads it; nothing writes it.

## path A: nix (pinned, recommended)

```
nix develop        # flake.nix + flake.lock pin every version in the table
make verify
make ghidra-import # headless import + analysis into ghidra/ (≈30–60 min)
make ghidra        # open the GUI
```

The devshell exports `ANALYZE_HEADLESS` and `GHIDRA_RUN` (nix wraps ghidra
in exec shims, so there is no stock install dir). Extensions are already
installed; you still do the one-time GUI plugin enabling below.

## path B: manual

1. **Ghidra 12.1.2** from https://github.com/NationalSecurityAgency/ghidra/releases
   plus a JDK 21. Unpack; export `GHIDRA_INSTALL_DIR=/path/to/ghidra_12.1.2_PUBLIC`.
   The Makefile uses `$GHIDRA_INSTALL_DIR/support/analyzeHeadless` and
   `$GHIDRA_INSTALL_DIR/ghidraRun`.
2. **Extensions** (File > Install Extensions in the project window, then
   restart Ghidra):
   - ReVa: https://github.com/cyberkaida/reverse-engineering-assistant/releases
     — take the zip built for 12.1.x.
   - delinker: https://github.com/boricj/ghidra-delinker-extension/releases
     — zip for 12.1.x.
   - MachineLearning: already in `$GHIDRA_INSTALL_DIR/Extensions/Ghidra/`,
     just install it from the same dialog.
3. **Python**: `python3 -m venv .venv && . .venv/bin/activate && pip install
   -r tools/requirements.txt`. (`.venv/` is gitignored.)
4. **objdiff**: https://github.com/encounter/objdiff/releases (CLI + GUI).
5. **DepotDownloader** (optional): https://github.com/SteamRE/DepotDownloader/releases,
   needs the .NET 9 runtime.
6. macOS: `make verify` wants `sha1sum`; use `make SHA1SUM="shasum -a 1" verify`.

## one-time ghidra GUI setup

Extensions installed ≠ plugins enabled. After the first `make ghidra`:

- Project window: File > Configure > plug icon ("Configure All Plugins") >
  tick **ReVa Application Plugin**.
- Open the program (CodeBrowser): File > Configure > Configure All Plugins >
  tick **ReVa Plugin** and, under Experimental,
  **RelocationTableSynthesizedPlugin** (delinker). File > Save Tool.
- ReVa listens on `localhost:8080`. `.mcp.json` in the repo points an MCP
  client at it; it only connects while the GUI is open.
- Missed-function recovery: Window > **Random Forest Function Finder**.
  Train on the FID-named CRT functions, run over `.text`, then re-run
  `tools/ghidra/DumpStats.java` from the Script Manager to recount.

## working rules

- Never write to `orig/`. Run `make verify` after anything that opened the exes.
- Don't guess struct layouts or function roles; show xrefs / decomp evidence.
- Byte-match is the test. "Looks right" is not a state.
- Mirror the leaked source tree (`nu2api/`, `gameapi/`, `batman/`) when
  source lands; see `docs/recon.md`.
- Toolchain facts are in `docs/recon.md`. Don't re-derive them, extend them.
