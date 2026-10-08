# toolchain

Getting the matching compiler, a compare loop, and a running game on Linux.
Status 2026-10-08: none of this is in the devshell yet. This is the map;
`docs/recon.md` has the evidence for every version named here.

## 1. the compiler: VC8 (Visual C++ 2005 SP1)

Target: `cl.exe` 14.00.50727.762 (SP1) with `link.exe` 8.00.50727.762.
RTM is `.42`; the Rich header can't distinguish them, SP1 is the bet.

**You have to source the install media yourself.** Microsoft's compilers are
not redistributable, so no repo (not this one, not decomp.me, whose compiler
images on ghcr.io are private for exactly this reason) will hand you one.
Options, in order of preference:

1. **Visual Studio 2005 + SP1** (any edition incl. Express, which had the
   same `cl`). Microsoft no longer hosts it; archive copies exist. Verify
   what you got: `cl /?` banner must say `14.00.50727.762`, and keep the
   SHA-256 of `cl.exe`, `c1.dll`, `c1xx.dll`, `c2.dll`, `link.exe`,
   `libcmt.lib`, `libcpmt.lib` in `tools/compiler.sha256` so everyone can
   prove they have the same bytes.
2. **Windows SDK 6.0** (Vista, 2006) reportedly bundles the VC8 SP1 x86
   compiler. Unverified by us; check the banner. SDK 6.1 ships VC9, wrong.

Layout the harness will expect (gitignored, outside the repo):

```
~/.local/share/lbtvg/vc8/
  Bin/   CL.EXE C1.DLL C1XX.DLL C2.DLL LINK.EXE mspdb80.dll ...
  Include/
  Lib/   libcmt.lib libcpmt.lib oldnames.lib ...
```

Also needed from the DirectX SDK **August 2007** (`d3dx9_35`): headers and
import libs. Same story, source it yourself, pin hashes.

### running it on Linux

Two ways. Try wibo first; it's what decomp.me uses for every MSVC it offers,
including `msvc8.0p`:

- **wibo** (`pkgs.wibo`, 0.6.14): a minimal Win32 PE loader, no prefix, no
  registry, starts in milliseconds. decomp.me's exact invocation:
  ```
  wibo "$VC8/Bin/CL.EXE" /c /nologo /I"Z:$VC8/Include/" <flags> \
       /Fd"Z:/tmp/" /Bk"Z:/tmp/" /Fo"Z:out.obj" "Z:in.cpp"
  ```
  Paths inside the guest are `Z:` + host path.
- **wine** (`pkgs.wineWowPackages.stable`, 11.0, needs the 32-bit half):
  slower, but runs `link.exe`, `mspdb80.dll` and anything wibo chokes on.
  Dedicated prefix: `WINEPREFIX=~/.local/share/lbtvg/wine`.

Starting flag set (from codegen heuristics, to be proven on a leaf):
`/O2 /Oy /GS /EHsc /MT /Gd /Zi`. Unknown until matched: `/Ob1` vs `/Ob2`,
`/Gy`, `/GR-` per TU (RTTI is off for most of the binary but not all).

## 2. the compare loop

Minimum viable loop, before any harness:

```
cl  → foo.obj
objdump -d foo.obj              # or ghidra on the .obj
cmp against orig bytes at the function's address
```

Then a real harness. Two candidates, different shapes:

| | objdiff | reccmp |
|---|---|---|
| compares | object vs object | whole exe vs whole exe, via PDB |
| needs from the original | `.obj` files — produced by the **ghidra-delinker-extension** (export a function range as COFF) | nothing but the exe |
| needs from the rebuild | `.obj` | exe + PDB + `// FUNCTION:` annotations |
| MSVC 2005 status | format-agnostic, works | README: newer MSVC "in progress" |
| in nixpkgs | yes (3.8.2, in the devshell) | no, `pip install reccmp` in the venv |

Start with objdiff + delinker: it works per function with zero annotation
overhead, and it's what the devshell already has. Move to reccmp when there
is enough matched code that a whole-binary percentage means something, and
test whether its VC8 PDB parsing holds up before relying on it.

**decomp.me** has `msvc8.0` and `msvc8.0p` presets. Use it to sanity-check
flag guesses against a known-good compiler install before blaming your own.

## 3. running and testing the game on Linux

- You're already running it through Steam with **GE-Proton 11-7**
  (`steamapps/compatdata/21000`). That prefix is the test environment.
- Launch options worth knowing: `PROTON_LOG=1 %command%` writes
  `~/steam-<appid>.log`; `WINEDLLOVERRIDES="dinput8=n,b" %command%` makes
  Proton load a `dinput8.dll` sitting next to the exe before the builtin.
- **Function replacement without an injector**: the exe imports
  `DINPUT8.dll` (1 function) and `XINPUT1_3.dll` (3 ordinals). A proxy
  `dinput8.dll` that forwards `DirectInput8Create` and, in `DllMain`,
  detours one original function to your reimplementation is the whole test
  rig. Build it with the same VC8 so the ABI is boring. Game still plays →
  your function is behaviorally right. (Don't do this on the Steam install
  dir; copy the game to a scratch dir first.)
- Outside Steam: `pkgs.umu-launcher` runs Proton against any prefix, useful
  for scripting "launch, wait N seconds, check log, kill".

## 4. ghidra on an MSVC binary

What already ran on import (no action needed):
`PE loader` (imports, Rich header stays raw bytes), `Windows x86 PE RTTI`
(the 186 vftables and 101 class names), `Demangler Microsoft`
(`.?AV` names → classes), `Function ID` (CRT naming, weak with
`vsOlder_x86`), `ApplyDataArchive` (`windows_vs12_32`: closest shipped
type archive; VS2005 types are mostly identical for Win32 API), `Aggressive
Instruction Finder` (ours, via `PreAnalysis.java`).

What to do by hand:
- **Decompiler Parameter ID** (Analysis > One Shot): resolves `__thiscall`
  vs `__cdecl` properly. Slow, run once, then save.
- **Random Forest Function Finder** (MachineLearning ext): recover the
  ~40% of `.text` that isn't in a known function yet.
- **FID database from your own VC8** once you have `libcmt.lib`: Function ID
  > Create new empty FidDb, then populate from the lib's `.obj`s. Names the
  CRT exactly, excludes it from the match budget.
- **Delinker**: select a function → Relocation table synthesizer → Export
  Program as COFF. That `.obj` is objdiff's "target" side.

Not useful here: PDB analyzers (no PDB), symbol servers.
