# setup

How to get a working environment for this project on any OS. Nothing here
assumes NixOS; the `nix develop` path is just the one where every version
below is pinned for you.

## what you need

| tool | version used here | why |
|---|---|---|
| git, GNU make, curl | any | repo, `make` targets, `tools/vc8.sh` |
| `sha1sum`/`sha256sum` (coreutils) or `shasum` | any | `make verify`, `tools/vc8.sh` |
| Ghidra | **12.1.2** | disassembly / decompilation; the project database lives in `ghidra/` |
| JDK | 21 | Ghidra 12 requirement |
| ReVa (Ghidra extension) | 7.3.0 | MCP server so an AI client can read/rename inside the open project. Optional. |
| ghidra-delinker-extension | 0.8.0 | export ranges of the original binary as `.obj` so objdiff can compare original vs recompiled |
| MachineLearning (Ghidra bundled extension) | ships with 12.1.2 | random-forest function finder; relocs are stripped so plain analysis misses vtable-only code |
| wibo | **1.2.0** (static release binary) | runs the Windows `cl.exe` on Linux/macOS without wine |
| 7z (p7zip), msiextract (msitools), cabextract | any | `tools/vc8.sh` unpacks the compiler out of Microsoft's SDK ISO |
| llvm-objdump / llvm-ar (LLVM bintools) | 17+ | read COFF `.obj`/`.lib` (GNU binutils cannot) |
| binutils (`objdump`, `strings`) | any | quick looks at PE files |
| clang-format | any (style pinned in `.clang-format`) | `make fmt` |
| Python 3 | 3.14 (≥3.10 fine) | recon scripts |
| pefile / lief / capstone | 2024.8.26 / 0.17.6 / 5.0.7 | PE parsing, disassembly in scripts (`tools/requirements.txt`) |
| objdiff | 3.8.2 | per-object diffing |
| DepotDownloader | 3.4.0 (.NET 9) | pull the macOS depot of app 21000 (possible symbol source). Optional. |

The compiler itself (VC8 SP1) is not a package anywhere; `tools/vc8.sh`
fetches it from Microsoft and verifies it. See `docs/toolchain.md`.

## reference source

`ref/saga` is a git submodule of opensagadev/saga (same engine, GPL-3.0),
pinned to a commit. After cloning: `git submodule update --init`; `make hooks`
then keeps it in step on every pull/checkout. `tools/saga.py NAME|0xADDR`
prints saga's body for a function to port by hand (copy, never symlink: our
version has to diverge for VC8 and the 2008 layouts). To move the
pin: `git -C ref/saga pull` then commit `ref/saga`. Other reference clones
under `ref/` are local and gitignored.

## the game

You need your own copy. Steam app 21000. Copy, don't symlink:

```
orig/LEGOBatman.exe       2025-07-04 re-link, unpacked — the target
orig/testapp.exe          2008-09-05 original, SteamStub-packed — reference
orig/LEGOBatmanDemo.exe   Oct 2008 public demo — reference (optional)
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
make vc8           # 1.2 GB from microsoft.com -> toolchain/{dist,vc8,winsdk6} (gitignored)
make ghidra-import # headless import + analysis into ghidra/ (≈30–60 min)
make fid           # VC8 CRT -> ghidra/vc8.fidb -> CRT named in the game
make ghidra        # open the GUI
make match         # compile src/ and diff every annotated function against orig/
make ghidra-import EXE=orig/LEGOBatmanDemo.exe   # optional: the demo as a 2nd program
```

The devshell exports `ANALYZE_HEADLESS` and `GHIDRA_RUN` (nix wraps ghidra
in exec shims, so there is no stock install dir), and `LBTVG_TOOLCHAIN`,
`VC8`, `WINSDK6` pointing at the compiler. Everything fetched lives inside
the repo under gitignored dirs (`toolchain/`, `ghidra/`); set
`LBTVG_TOOLCHAIN` before `nix develop` to keep the compiler elsewhere. Extensions are already installed; you still do the
one-time GUI plugin enabling below.

## path B: manual

1. **Ghidra 12.1.2** from https://github.com/NationalSecurityAgency/ghidra/releases
   plus a JDK 21. Unpack; export `GHIDRA_INSTALL_DIR=/path/to/ghidra_12.1.2_PUBLIC`.
   The Makefile uses `$GHIDRA_INSTALL_DIR/support/analyzeHeadless` and
   `$GHIDRA_INSTALL_DIR/ghidraRun`.
2. **Extensions** (File > Install Extensions in the project window, then
   restart Ghidra):
   - ReVa: https://github.com/cyberkaida/reverse-engineering-assistant/releases
     — the zip built for 12.1.x.
   - delinker: https://github.com/boricj/ghidra-delinker-extension/releases
     — zip for 12.1.x.
   - MachineLearning: already in `$GHIDRA_INSTALL_DIR/Extensions/Ghidra/`,
     install it from the same dialog.
3. **wibo**: the `wibo-i686` static binary from
   https://github.com/decompals/wibo/releases/tag/1.2.0, on `PATH`. Needs a
   kernel that runs 32-bit ELF (any x86_64 Linux). On macOS use `wibo-macos`.
4. **p7zip, msitools, cabextract, curl** from your package manager, for
   `tools/vc8.sh`. Then `make vc8`.
5. **LLVM bintools** (`llvm-objdump`, `llvm-ar`) from your package manager.
6. **Python**: `python3 -m venv .venv && . .venv/bin/activate && pip install
   -r tools/requirements.txt`. (`.venv/` is gitignored.)
7. **objdiff**: https://github.com/encounter/objdiff/releases (CLI + GUI).
8. **DepotDownloader** (optional): https://github.com/SteamRE/DepotDownloader/releases,
   needs the .NET 9 runtime.
9. macOS: `make verify` wants `sha1sum`; use `make SHA1SUM="shasum -a 1" verify`.

## editor / LSP (clangd)

`compile_flags.txt` at the repo root makes clangd parse `src/` the way VC8
sees it: `clang-cl` mode, `_MSC_VER` 1400, VC8 and Windows SDK 6 headers from
`toolchain/` (relative paths, so it is committed and covers new files with no
`compile_commands.json` or `bear`). Two things it depends on:

- Use the devshell's clangd (`llvmPackages.clang-unwrapped`). The usual
  wrapped nix clangd injects host glibc/gcc headers, so `<string.h>` would
  quietly be Linux's, not VC8's.
- `make vc8` adds case-alias symlinks to the headers (`tools/casefold.py`):
  the SDK ships `Windows.h`, code says `<windows.h>`.

clang is stricter than 2005 MSVC, so an occasional red squiggle in code that
`make match` accepts is expected. If you moved the toolchain with
`LBTVG_TOOLCHAIN`, point the two `-imsvc` lines at it locally.

## one-time ghidra GUI setup

Extensions installed ≠ plugins enabled. After the first `make ghidra`:

- Project window: File > Configure > plug icon ("Configure All Plugins") >
  tick **ReVa Application Plugin**.
- Open the program (CodeBrowser): File > Configure > Configure All Plugins >
  tick **ReVa Plugin** and, under Experimental,
  **RelocationTableSynthesizedPlugin** (delinker). File > Save Tool.
- ReVa listens on `localhost:8080`. `.mcp.json` in the repo points an MCP
  client at it; it only connects while the GUI is open.
- Repo scripts in the GUI: Window > Script Manager > "Manage Script
  Directories" (list icon) > add `<repo>/tools/ghidra`. The Makefile only
  passes them to the headless analyzer; the GUI does not pick them up on its
  own.
- Missed-function recovery: Window > **Random Forest Function Finder**.
  Train on the FID-named CRT functions, run over `.text`, then
  `make fid-apply` to re-count.

## working rules

- Never write to `orig/`. Run `make verify` after anything that opened the exes.
- Never open the GUI while a `make ghidra-import` / `make fid*` is running;
  headless holds the project lock.
- Don't guess struct layouts or function roles; show xrefs / decomp evidence.
- Byte-match is the test. "Looks right" is not a state.
- Mirror the leaked source tree (`nu2api/`, `gameapi/`, `batman/`); see
  `docs/recon.md`.
- Toolchain facts are in `docs/recon.md`. Don't re-derive them, extend them.
