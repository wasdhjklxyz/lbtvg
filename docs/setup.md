# setup

How to get a working environment for this project on any Linux. Nothing here
assumes NixOS; the `nix develop` path is just the one where every version
below is pinned for you. Everything fetched lands inside the repo in
gitignored directories (`toolchain/`, `ghidra/`, `play/`, `build/`).

## what you need

| tool | version used here | why |
|---|---|---|
| git, GNU make, curl, coreutils | any | repo, `make` targets, downloads, `sha1sum`/`sha256sum` |
| Python 3 + pefile / lief / capstone | 3.14 (≥3.10) / 2024.8.26 / 0.17.6 / 5.0.7 | every `tools/*.py` and the git hooks (`tools/requirements.txt`) |
| wibo | **1.2.0** (static release binary) | runs the Windows `cl.exe` on Linux without wine |
| p7zip, msitools, cabextract, unzip | any | unpack the compiler (`make vc8`) and the DirectX SDK (`make dxsdk`) |
| LLVM: clangd, clang-format, llvm-objdump, llvm-ar | 17+ (21 here) | LSP, `make fmt` / the hook, reading COFF `.obj`/`.lib` |
| Ghidra + JDK | **12.1.2** + 21 | disassembly / decompilation; project in `ghidra/` |
| Ghidra extensions | ReVa 7.3.0, delinker 0.8.0, MachineLearning (bundled) | MCP for AI clients, `.obj` export, function finder |
| binutils (`objdump`, `strings`) | any | quick looks at PE files |
| objdiff | 3.8.2 | per-object diffing (optional) |
| wine (WoW64 or multilib) + winetricks + a Vulkan driver | wine 11.0 | `make play` only |
| DepotDownloader | 3.4.0 (.NET 9) | optional, Steam depots |

The compiler (VC8 SP1) and the DirectX SDK (August 2007) are not packages
anywhere: `make vc8` and `make dxsdk` fetch them and check them against
Microsoft's hashes. See `docs/toolchain.md`.

## the game

You need your own copy (Steam app 21000). Copy the exes, don't symlink them:

```
orig/LEGOBatman.exe       2025-07-04 re-link, unpacked: the target
orig/testapp.exe          2008-09-05 original, SteamStub-packed: reference
orig/LEGOBatmanDemo.exe   Oct 2008 public demo: reference (optional)
```

`make verify` must pass. The sha1s in `orig/checksum.sha1` are the only
definition of "the binary"; if yours differ, stop and find out why.
`orig/*.exe` is gitignored and no tool ever writes to it.

For `make play`, `game` (gitignored) should point at the installed game
directory, the one with `GAME.DAT`:
`ln -s ~/.local/share/Steam/steamapps/common/'Lego Batman' game`, or pass
`GAME_DIR=...`.

## path A: nix (pinned, recommended)

```
git submodule update --init   # ref/saga
nix develop                   # every version in the table, pinned by flake.lock
make hooks                    # once per clone: commit checks + submodules follow pulls
make verify
make vc8                      # compiler: 1.2 GB from microsoft.com
make dxsdk                    # DirectX SDK Aug 2007: 469 MB, Microsoft's SHA-1 checked
make ghidra-import            # headless import + analysis into ghidra/ (30-60 min)
make fid                      # name the statically linked CRT in the game
make ghidra                   # open the GUI (see "one-time ghidra GUI setup")
make match                    # every annotated function vs orig/: should say 0 differ
```

The devshell also exports `ANALYZE_HEADLESS`/`GHIDRA_RUN` (nix wraps ghidra
in exec shims, so there is no stock install dir) and `LBTVG_TOOLCHAIN`,
`VC8`, `WINSDK6`, `DXSDK`. Wine lives in a separate shell
(`nix develop .#play`) that `make play` enters by itself, so nobody else
downloads it.

## path B: manual

1. From your distro: git, make, curl, python3, p7zip, msitools, cabextract,
   unzip, clang/llvm (clangd, clang-format, llvm-objdump, llvm-ar), binutils.
2. **wibo**: `wibo-i686` from
   https://github.com/decompals/wibo/releases/tag/1.2.0 on `PATH` (any x86_64
   Linux runs it).
3. **Python**: `python3 -m venv .venv && . .venv/bin/activate && pip install -r tools/requirements.txt`.
   Keep the venv active when you commit: the hooks run Python, and without
   these modules they try `nix develop` instead.
4. **Ghidra 12.1.2** (https://github.com/NationalSecurityAgency/ghidra/releases)
   with JDK 21; `export GHIDRA_INSTALL_DIR=/path/to/ghidra_12.1.2_PUBLIC`. The
   Makefile uses `$GHIDRA_INSTALL_DIR/support/analyzeHeadless` and `ghidraRun`.
   Extensions via File > Install Extensions, then restart:
   [ReVa](https://github.com/cyberkaida/reverse-engineering-assistant/releases)
   and [delinker](https://github.com/boricj/ghidra-delinker-extension/releases)
   (zips for 12.1.x), MachineLearning (already in `Extensions/Ghidra/`).
5. Then the same commands as path A, minus `nix develop`:
   `git submodule update --init`, `make hooks`, `make verify`, `make vc8`,
   `make dxsdk`, `make ghidra-import`, `make fid`, `make match`.
6. For `make play`: wine with 32-bit support (a WoW64 build, or multilib),
   winetricks, cabextract and a working Vulkan driver.
7. Optional: [objdiff](https://github.com/encounter/objdiff/releases),
   [DepotDownloader](https://github.com/SteamRE/DepotDownloader/releases).

## editor / LSP (clangd)

`compile_flags.txt` at the repo root makes clangd parse `src/` the way VC8
sees it: `clang-cl` mode, `_MSC_VER` 1400, and the VC8, DirectX SDK and
Windows SDK headers from `toolchain/`. The paths are relative, so the file is
committed and covers new files without a `compile_commands.json` or `bear`.
It needs `make vc8` and `make dxsdk` first, and:

- **Nix:** use the devshell's clangd (`llvmPackages.clang-unwrapped`). The
  usual wrapped nix clangd injects host glibc/gcc headers, so `<string.h>`
  would quietly be Linux's, not VC8's. Start your editor from inside
  `nix develop`, or point it at that clangd.
- **Elsewhere:** a distro clangd works as is.
- `make vc8`/`make dxsdk` add case-alias symlinks to the headers
  (`tools/casefold.py`): the SDKs ship `Windows.h`, code says `<windows.h>`.

clang is stricter than 2005 MSVC, so an occasional red squiggle in code that
`make match` accepts is expected. If you moved the toolchain with
`LBTVG_TOOLCHAIN`, edit the three `-imsvc` lines locally.

## playing the game: `make play`

Runs the game under wine from `play/` (a test copy: data symlinked from
`game/`, your verified `orig/LEGOBatman.exe` copied, its own wine prefix), no
Steam needed. Nothing from the game is in this repo; it only works with your
own copy. `make play-setup` builds it; `make play` does so on first use.

```
make play                          # windowed 1280x720: maximize / fullscreen from the title bar
WINDOW=1600x900 make play          # another window size
FULLSCREEN=1 make play             # fullscreen, RES=1920x1080 by default
FULLSCREEN=1 RES=2560x1440 make play
```

Bring your own pieces instead of the defaults:

| setting | default | use it for |
|---|---|---|
| `WINE=/path/to/wine` | `wine` on `PATH`, else the nix `.#play` shell | a wine/Proton build you already have (`PATH=... make play` works too) |
| `WINETRICKS=...` | `winetricks` on `PATH` | only used once, to install DXVK |
| `DXVK=0` | DXVK installed into the prefix | you handle Direct3D yourself (or want wine's wined3d) |
| `D3DX9_DLL=/path/d3dx9_35.dll` | extracted from the game's `DirectX/*d3dx9_35_x86.cab` | your own copy of Microsoft's DLL |
| `PLAY_PREFIX=/path` | `play/prefix` | an existing wine prefix |
| `GAME_DIR=/path` | `./game` | where the installed game data is |

Why each default exists:

- **DXVK** (Direct3D 9 over Vulkan): wine's OpenGL-based Direct3D could not
  reach the NVIDIA driver from a nix wine on NixOS ("failed to create d3d
  device").
- **Microsoft's `d3dx9_35.dll`**: wine's builtin cannot compile the game's
  HLSL shaders (mostly black screen). Steam installs the same DLL through its
  DirectX installer.
- **Windowed by default** uses the game's own `-Windowed` switch and writes
  `WINDOW` into its `pcconfig.txt`. **Fullscreen** runs `-Fullscreen` inside a
  wine virtual desktop of `RES`: without it the game sees no display modes
  (resolution 0x0) and falls back to a blurry 640x480.
- The registry keys Steam's install script sets are added to the prefix.

The Steam install is never written; anything you put next to
`play/game/LEGOBatman.exe` (e.g. a test DLL) stays in `play/`. All settings:
`tools/play.sh help`.

## one-time ghidra GUI setup

Extensions installed ≠ plugins enabled. After the first `make ghidra`:

- Project window: File > Configure > plug icon ("Configure All Plugins") >
  tick **ReVa Application Plugin**.
- Open the program (CodeBrowser): File > Configure > Configure All Plugins >
  tick **ReVa Plugin** and, under Experimental,
  **RelocationTableSynthesizedPlugin** (delinker). File > Save Tool.
- ReVa listens on `localhost:8080`. `.mcp.json` points an MCP client at it;
  it only connects while the GUI is open.
- Repo scripts: Window > Script Manager > Manage Script Directories > add
  `<repo>/tools/ghidra`. Run `ApplyNames.java` once with
  `tools/symbols/pc-names.csv` to get ~800 real function names.
- Missed-function recovery: Window > **Random Forest Function Finder**, then
  `make fid-apply` to re-count.

## reference source

`ref/saga` is a git submodule of opensagadev/saga (same engine, GPL-3.0),
pinned to a commit; `make hooks` keeps it in step on every pull/checkout.
`tools/saga.py NAME|0xADDR` prints saga's body for a function, and `make new`
pastes it for you (see `docs/workflow.md`). Copy, never symlink: our version
has to diverge for VC8 and the 2008 layouts. To move the pin:
`git -C ref/saga pull`, then commit `ref/saga`. Other clones under `ref/` are
local and gitignored.

Third-party code statically linked into the game, for matching it from its
real source (the exe says `Xiph.Org libVorbis I 20070622`):

```
cd ref
curl -LO https://downloads.xiph.org/releases/vorbis/libvorbis-1.2.0.tar.gz  # sha256 6eb7040048e35448fe224fa3fd993eb4e49a905c57893886082f1674d43b0e73
curl -LO https://downloads.xiph.org/releases/ogg/libogg-1.1.3.tar.gz        # sha256 bae29e79fbc50bbedf1235852094b71c8c910a1ef0cd42fe4163b7b545630b65
tar xzf libvorbis-1.2.0.tar.gz && tar xzf libogg-1.1.3.tar.gz
```

libogg 1.1.3 is the release of that era; matching confirms or refutes it.

## working rules

- Never write to `orig/`. Run `make verify` after anything that opened the exes.
- Never open the GUI while `make ghidra-import` / `make fid*` runs: headless
  holds the project lock.
- Don't guess struct layouts or function roles; show xrefs / decomp evidence.
- Byte-match is the test. "Looks right" is not a state.
- Mirror the leaked source tree (`nu2api/`, `gameapi/`, `batman/`); see
  `docs/recon.md`. Toolchain facts are there too: extend them, don't re-derive.
