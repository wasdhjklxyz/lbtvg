# recon

Static recon of the Steam build of LEGO Batman: The Videogame (PC), done
2026-10-08 before any decompilation work. Everything below was derived from the
files in the Steam install (now `game/` symlink; the two exes are copied to `orig/`) with `pefile`, `objdump`,
`strings`, and a Ghidra 12.1.2 headless pass. No game code has been reversed yet.

## tl;dr

- **Target binary: `orig/LEGOBatman.exe`** (sha1 `ed6c37c5…`, 7.6 MB). It is
  what Steam launches, it is *not* DRM-wrapped, and it still has its Rich header
  and CodeView debug entry.
- **Toolchain: Visual C++ 2005 (VC8), `cl` 14.00.50727 / `link` 8.00.50727,
  static CRT (`/MT`), no LTCG, no PGO, no `/arch:SSE`.** Almost certainly SP1
  (`.762`) — see "compiler" for how to confirm.
- `testapp.exe` is the **original 2008-09-05 build wrapped in SteamStub v2**
  (`.bind` section, `.text` entropy 8.00). Same source; only the build path
  differs. Keep it as a second reference once unpacked with Steamless.
- `LEGOBatman.exe` was **re-linked on 2025-07-04** by TT from a tree called
  `c:\TT\legobatman1_clean\Batman_Resub2\`. Readable strings are identical to
  the 2008 build modulo path prefix; `.text` is 256 bytes larger. Treat it as
  the same source, same compiler, tiny delta.
- Engine is TT's in-house **NuCore / "nu2api"** + a **`gameapi`** layer.
  Source paths leak the directory layout (below). RTTI is mostly off; ~101
  type descriptors survive.
- There is a **macOS depot** for this app. If that Mach-O ships unstripped (TT
  Mac ports often do, and someone started a Mac decomp from it), it is a free
  symbol table for the PC binary. Highest-value follow-up.

## files

| file | what |
|---|---|
| `LEGOBatman.exe` | 2025 re-link, unpacked, **target** |
| `testapp.exe` | 2008 build, SteamStub v2 packed, 8 sections (`.bind` added) |
| `binkw32.dll` | RAD Bink video, dynamic import (17 funcs) |
| `GameExplorerHelper.dll` | Games-for-Windows Explorer registration, irrelevant |
| `GAME.DAT`, `HERO{1,2,3}.DAT`, `VILLAIN{1,2,3}.DAT` | TT pack archives, header `BEGIN_APP_ID_STRINGMkDat V3.26END_APP_ID_STRING` (~3.8 GB total) |
| `Audio/*.cfg`, `Audio/_MUSIC`, `Audio/_CUTSCENES` | text audio config + OGG streams |
| `Movies/*.BIK` | Bink |
| `DirectX/` | redist; note `d3dx9_35` (Aug 2007 SDK) is the one the exe imports |

Steam: app 21000, build 19129138, depots 21001 (data), 21002 (english),
21007 (7.5 MB = the exe). `appinfo.vdf` launch config: `LEGOBatman.exe`
(windows), `LEGO Batman.app` (macos). Both exes are copied to `orig/` with sha1s in `orig/checksum.sha1`
(`make verify`). `game/` is a symlink into the Steam dir, which Steam owns and
may update under you — never read the exes from there for matching.

## PE facts (LEGOBatman.exe)

```
timestamp        2025-07-04 10:47:41 UTC (also in debug dir)
linker           8.0            image base 0x00400000
entry            0x0073d6d3     subsystem GUI 4.0
checksum         0x74391b (valid)
relocs           stripped       no TLS, no load config, no exports
pdb              c:\TT\legobatman1_clean\Batman_Resub2\batman\PC_Final\LEGOBatman.pdb
pdb guid/age     4085DF94-DFFE-493A-B354-9E39F6D72547 / 1
```

| section | VA | vsize | raw | notes |
|---|---|---|---|---|
| `.text`  | 0x401000 | 0x44e29e | 0x44f000 | 4.3 MB of code |
| `.rdata` | 0x850000 | 0x0e3266 | 0x0e4000 | strings, vtables, RTTI, embedded HLSL |
| `.data`  | 0x934000 | 0x20e2490 | 0x08a000 | **34 MB virtual** — big static pools (NuCore memory manager) |
| `.idata` | 0x2a17000 | 0x207e | | imports |
| `.shr`   | 0x2a1a000 | 0x104 | | shared RW section, zeroed |
| `CONST`  | 0x2a1b000 | 0x14b | | `nvapi.dll`/`nvpmapi.dll` names — NVIDIA PerfKit stub |
| `.rsrc`  | 0x2a1c000 | 0x172d70 | | GDF xml/thumbnails ×7 langs, two 441 KB `SPAFILE` RCDATA blobs (Xbox 360 XLAST-style config), icons, manifest |

`testapp.exe`: identical layout shifted by 0x1000 (`.rdata` is 0x800 smaller),
`.bind` at 0x278e000 (0x54000), entry inside `.bind`, checksum 0, Rich header
gone, pdb `d:\Projects\batman\PC_Final\LEGOBatman.pdb`
(guid `CDDBD912-8375-444F-8A8C-806BB5D14B67`).

## compiler (Rich header)

| prodid | tool | build | objs |
|---|---|---|---|
| 0x6e | `Utc1400_CPP` — VC8 C++ compiler | 50727 | 315 |
| 0x6d | `Utc1400_C` — VC8 C compiler | 50727 | 183 |
| 0x7d | `Masm800` | 50727 | 54 |
| 0x78 | `Linker800` | 50727 | 1 |
| 0x7c | `Cvtres800` | 50727 | 1 |
| 0x7b | `Implib800` | 50727 | 7 |
| 0x5d | `Implib710` (VS2003 import libs — DX SDK) | 4035 | 22 |
| 0x5f / 0x60 | `Utc1310_C` / `Utc1310_CPP` (VS2003-built lib objs) | 4035 | 4 / 2 |
| 0x05 | `Cvtomf600`-era objs (VC6 lib, 2 objs) | 8447 | 2 |
| 0x01 | imports | — | 213 |

What that means:

- **No `Utc1400_LTCG_*`, no `POGO_*`** → no `/GL`, no PGO. Functions are
  compiled per-TU; byte-matching is realistic.
- Build 50727 is shared by VS2005 RTM (`.42`) and SP1 (`.762`); the Rich
  header can't tell them apart. The game shipped Sep 2008, SP1 was Dec 2006,
  and the 2025 re-link reused the same toolchain → assume **SP1**. Confirm by
  matching one CRT function (e.g. `memcpy`, `_ftol2`) against both.
- The two VS2003 C/C++ objs + 22 VS2003 import libs = DirectX SDK (Aug 2007)
  libs. The two VC6 objs are some tiny ancient lib. These are **excluded** from
  the match budget.

Codegen heuristics over `.text` (byte-pattern counts, not ground truth):

| signal | count | reads as |
|---|---|---|
| `push ebp; mov ebp,esp` | 835 | frame pointer omitted almost everywhere → `/O2` (implies `/Oy`) |
| int3 padding runs | 12 271 | ≈ function count ballpark; `/O2` pads with `CC` |
| `mov eax,[__security_cookie]; xor eax,esp` | 492 | `/GS` on (VS2005 default), cookie at `0x9a78d0` |
| `mov eax,fs:[0]` | 57 | SEH frames: `__except` + C++ EH (`/EHsc`) |
| `fld dword` | 29 295 | x87 float code |
| `movss` | 14 | no `/arch:SSE`; the 14 are hand-written or intrinsic |
| `cvttss2si` | 0 | float→int goes through `_ftol2` / `fistp` (102) |
| `mov esi,ecx` at entry | 858 | `__thiscall` methods |

Entry stub is the canonical VS2005 `call __security_init_cookie (0x747a97);
jmp __tmainCRTStartup` → `WinMainCRTStartup`, static CRT.

Starting flag guess for the harness: `cl /O2 /Oy /GS /EHsc /MT /Gd` (+ `/Zi` to
get a PDB for reccmp). `/Ob1` vs `/Ob2` and `/Gy` to be determined by
matching.

## engine / source layout

Leaked `__FILE__` strings (41 of them) give the tree:

```
batman_resub2/
  nu2api/                 TT "NuCore" engine
    nucore/   nufile_gen.cpp numemblk_gen.cpp nupad_gen.cpp pc/nufile_pc.cpp
    nu3d/     nuanim_gen.cpp nucamera_gen.cpp nuinstsurfgeom_gen.cpp
              numoviegrab_gen.cpp nutexanm_gen.cpp nutimebar_gen.cpp
              nuprocesscolourfilter_gen.cpp
              pc/nugscn_dlist.cpp numtl_dlist.cpp nuqfnt_PC.cpp nustream.cpp
                 nutex_pc.cpp NuMovieGrab_PC.cpp
              Shaders/deferredFilter.hlsl
    numath/   nugraph_gen.cpp nutrig_gen.cpp
    gamelib/  listman_gen.cpp
  gameapi/                game-side framework
    ai/aisys/ AIBugPit.cpp AIBugPit.h AISupport.cpp
    cutscene/ gcutscn.cpp
    edlevel/  edspecial.cpp edspline.cpp edstring.cpp
    gamelib/  terrain.c                      <- mixed C and C++
    gui/gamemenu/apisave.c
    rtl/      rtleditor.cpp
  batman/                 (relative paths, compiled with cwd here)
    pcbatman.cpp pcapi.cpp windows.cpp d3dCore.cpp d3dCalls.cpp d3dApiCalls.cpp
    job_occlusion.cpp numem_gen.cpp pc/nusound.cpp pc/oggreader.cpp
    PC_Final/LEGOBatman.pdb
```

Conventions: `*_gen.cpp` = platform-generic, `*_pc.cpp` / `pc/` = PC backend,
`nu` prefix = engine, `Ed*` = editor. PS3/PSP strings exist (8/6 hits) — shared
multi-platform codebase; expect `#ifdef` litter.

RTTI survivors (101, grep `.?AV`): `NuFmvStreamPCBink`, `NuSSAOFilter`,
`NuDeferredFilter`, `NuMotionFilter`, `NuSpeedBlurFilter`, `NuEdgeAAFilter`
(post-process chain), `OggReader`, `WavReader`, `ShaderManagerHLSL`,
`HLSLShaderBuilder`, `CSListLink<T>` (intrusive list template),
`LevelEditor`, `ClassEditor`, `BaseEditor`, `EdRef*`, `EdSubSystem`,
`SplineObject`, `SceneInstance`, `Placeable`, `SpecialObject`, `WorldMap`,
`Fade`/`CrossFade`/`SpinWipe`, `std::*` exceptions. The editor classes mean a
chunk of dev tooling shipped in the retail exe — likely dead but linked.

RTTI is mostly compiled out (`/GR-` on most TUs). Class hierarchy will come
from vtables + constructors, not type descriptors. Only 18 `Class::Method`
strings, 0 `__FUNCTION__`-style leaks — function names are **not** free.

Data formats referenced by the exe: `.gsc` (scene), `.ghg` (model), `.an3`
(anim, 11 912 in GAME.DAT), `.scp` (script), `.txt` (level config), `.git`,
`.ai2`, `.rtl`, `.par`, `.ptl`, `.giz`, `.led`, `.qfn`/`.ufn` (fonts), `.cu2`,
`.sfx`, `.dds`. Paths like `chars\%s\%s.gsc`, `%sLevels\%s\%s\%s.ai2`.

## libraries (static, to exclude via FunctionID)

| lib | evidence |
|---|---|
| VC8 static CRT + C++ runtime | entry stub, `std::` RTTI, "Microsoft Visual C++ Runtime Library" |
| libvorbis/libogg | `Xiph.Org libVorbis I 20070622` (1.2.0) |
| dxerr9 | thousands of `D3DXERR_*`/`DMUS_E_*`/`XACT_*` description strings (`DXGetErrorDescription`) |
| NVIDIA PerfKit (nvpmapi) | `CONST` section, `NVPMQueryInterface` |
| OpenAutomate SDK | `-openautomate`, `oaInit() called more than once` (NVIDIA benchmark automation) |
| DirectX SDK Aug 2007 | `d3dx9_35`, `XINPUT1_3`, `dinput8`, `dsound` (ord 11 = `DirectSoundCreate8`) |

Not present: zlib, png, jpeg, lua, FMOD, Havok, PhysX, Scaleform, steam_api,
SecuROM, GFWL. The 2025 build has **zero Steam API** — it's just the game.

Shaders: HLSL source is embedded (1 353 source-looking lines, `#if
defined(PRELIGHT_FX)` etc.) and compiled at runtime through
`D3DXCompileShader`; targets `vs_1_1..3_0`, `ps_1_1..3_0`.

## scope numbers

Ghidra 12.1.2 headless via `make ghidra-import` (aggressive instruction finder
on, no Decompiler Parameter ID), FID with the stock `vsOlder_x86` db. First
column is the committed run; the earlier default-analyzer run is in parens.

| | functions | bytes |
|---|---|---|
| total found | 9 657 (9 202), 748 thunks | 2 747 088 (2 666 595) |
| game region (`0x401000`–`0x73c8fb`, before first CRT fn) | 7 556 (7 239) | 2 484 173 (2 417 069) |
| CRT + static libs (`0x73c8fb`–end) | 1 353 (1 258) | 262 915 (249 526) |
| FID/EH-named (excluded) | 660 | 92 289 |
| vftables recovered | 186 | — |

The aggressive finder recovered 412 new function starts (~80 KB). Still well
short of the section size, so the ML function finder (GUI) is the next lever.

Caveats that change the real numbers:

- `.text` is 4 517 888 bytes but recovered functions only sum to 2.67 MB.
  Relocs are stripped, so Ghidra can't pointer-scan; code reached only via
  vtables / function tables is missing. Rerun with *Aggressive Instruction
  Finder*, *Decompiler Parameter ID*, and the RTTI/vtable scanner, then
  re-count. Expect the real game-code figure nearer **3.0–3.3 MB / ~9–10k
  functions**.
- FID matched very little of the VC8 CRT (`vsOlder` is a poor fit). The
  1.12 MB lib span (`0x73c8fb`–`0x84f29e`) is the real "excluded" denominator,
  not 250 KB — Ghidra just didn't find most of it. Build a FID db from the
  VC8 SP1 `libcmt.lib`/`libcpmt.lib`, libvorbis, dxerr9 once the compiler is
  in hand.
- Working denominator for "% matched" until then: **≈3.39 MB** of game
  `.text` (`0x73c8fb - 0x401000`).
- Size histogram of game-region functions: ≤32 B: 1 109, ≤128 B: 2 713,
  ≤512 B: 2 474, ≤2 KB: 1 078, >2 KB: 174. **1 030 functions are 16–48
  bytes** — that is the leaf-function pool to learn the compiler on.

Raw dump (addr, size, cc, name): `ghidra/stats.tsv`, regenerated by
`make ghidra-import`. Project: `ghidra/lbtvg.gpr` (gitignored).

## tomorrow: toolchain + compare harness

1. **Get cl.exe 14.00.50727.762.** Sources: a VS2005 + SP1 install, or the
   Windows SDK 6.0 (Vista, 2006), which reportedly bundles the VC8 SP1 x86
   compiler — verify the `cl /?` banner. SDK 6.1 (Server 2008) ships VC9 and
   is the wrong one. Run under wine in a dedicated prefix. Do *not* chase
   "portable MSVC" tarballs of unknown provenance — verify the compiler by
   `cl /?` banner and by matching a CRT function.
2. **Prove the loop on one leaf.** Pick a tiny no-call math function from
   `numath/nutrig_gen.cpp` territory (Ghidra: smallest functions with no
   calls in `.text`), write it, compile with the flag guess above, diff bytes.
   Iterate flags until the CRT and one leaf both match.
3. **reccmp** (isledecomp's tool, now standalone) for the full-binary compare.
   It wants: original exe, recompiled exe, recompiled PDB, and `// FUNCTION:`
   annotations. Rich header + PDB path already tell it what it needs.
   Alternative: `objdiff` for per-object diffing without linking.
4. **decomp.me** has `msvc8.0` and `msvc8.0p` presets. Use it for
   scratch-matching single functions and for sanity-checking flags against a
   known-good compiler install.
5. **Steamless** the 2008 `testapp.exe` (SteamStub v2.0/2.0.1 supported, 32-bit)
   under wine/mono. Then byte-diff the two `.text` sections to see exactly
   what changed in 2025 (expect a handful of functions).
6. **Pull the macOS depot** (you own the game; `DepotDownloader -app 21000
   -os macos`). `nm` the Mach-O. If it's unstripped, you have ~every function
   name; map them to the PC binary with Ghidra BSim or Diaphora. This is the
   single biggest multiplier available.

## workflow suggestions

- **Ghidra project** lives outside the repo (currently
  `/var/tmp/uiop-lbtvg-recon/ghidra/lbtvg.gpr`, move it somewhere permanent).
  Ghidra 12 ships no vs2005 FID db; `vsOlder_x86.fidbf` is the one that covers
  pre-2012 CRTs — confirm it names the CRT, otherwise build a FID db from the
  VC8 `libcmt.lib` once the compiler is in hand. Named CRT/lib functions are
  excluded from the budget.
- **Ghidra MCP** (e.g. `LaurieWired/GhidraMCP` or `ghidra-mcp` servers) lets
  Claude read decompiled functions, rename, retype, and xref straight from
  the open project. Worth setting up once the project is stable; pair it with
  a `CLAUDE.md` that states the flag set, naming rules, and "never guess a
  struct layout — show the xrefs".
- **Repo layout** (mirror the leaked tree): `src/nu2api/...`, `src/gameapi/...`,
  `src/batman/...`, `tools/` (reccmp config, flake), `original/` gitignored.
- **Match tracking**: annotate every function with `// FUNCTION: LEGOBATMAN
  0x004xxxxx` (reccmp style); CI computes matched-bytes / non-library-bytes.
- **Behaviour testing before matching**: DLL injection + detour of a single
  function. Needs the game running under wine/Proton anyway.
- **nix**: a `flake.nix` devshell with `python3.withPackages (pefile lief
  capstone)`, `ghidra`, `wine`, `radare2`, `objdiff`; reccmp via pip inside
  the shell. Keep MSVC in a wine prefix under `~/.local/share/`, not the repo.
