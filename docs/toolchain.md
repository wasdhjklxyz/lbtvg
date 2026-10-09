# toolchain

The matching compiler, the compare loop, and running the game on Linux.
State: compiler fetched and verified (section 1), compare harness not built
yet (section 2). `docs/recon.md` has the evidence for every version named
here.

## 1. the compiler: VC8 (Visual C++ 2005 SP1)

Target: `cl.exe` 14.00.50727.762 (SP1), `link.exe` 8.00.50727.762. RTM is
`.42`; the Rich header cannot distinguish them, SP1 is the bet.

**Verified source, 100% Microsoft-hosted:** the *Windows SDK Update for
Windows Vista* (Feb 2007) DVD ISO, still on download.microsoft.com with a
published SHA-1. Its release notes state the C++ compilers are the VS2005 SP1
ones, and the extracted `cl.exe` says `14.00.50727.762 (SP.050727-7600)`. The
same ISO carries the SP1 CRT DLLs and the Win32 headers/import libs.

```
make vc8            # tools/vc8.sh: download (1.2 GB), verify, extract, verify, smoke-test
```

It produces, in `toolchain/` (gitignored; `LBTVG_TOOLCHAIN=/elsewhere` to move it):

```
toolchain/vc8/      Bin/ (cl, c1, c1xx, c2, link, lib, ml, mspdb80 ...)
                    INCLUDE/ LIB/ (CRT headers, libcmt/libcpmt)
toolchain/winsdk6/  Include/ Lib/ (windows.h, kernel32.lib, d3d9.h ...)
```

Every load-bearing binary is pinned in `tools/compiler.sha256`, so two
contributors can prove they hold the same bytes. Compilers are not
redistributable, so the repo holds hashes, never the files.

One non-Microsoft file: `Bin/msvcr80.dll` is Wine's reimplementation (from
`encounter/winedll`, LGPL). Microsoft's own `msvcr80.dll` aborts with R6034
unless loaded through a side-by-side activation context, which wibo does not
implement. The Microsoft copy is kept as `msvcr80.dll.ms` for real Wine.

**DirectX SDK, August 2007** (the exe imports `d3dx9_35.dll`, first shipped in
that release): `make dxsdk` (`tools/dxsdk.sh`). Microsoft no longer hosts it;
the script downloads the original installer from the Internet Archive and
refuses it unless its SHA-1 is the one Microsoft published
(`c812c18e2972bdb1d9cbb544be9ced9370a4656f`), i.e. byte-identical to
Microsoft's file. Headers and x86 libs land in `toolchain/dxsdk/`; the harness,
`make new` and clangd put them ahead of the Windows SDK.

### running it on Linux

**wibo** (in the devshell, static 1.2.0 release binary; nixpkgs' 0.6.14 lacks
kernel32 stubs VC8 needs). No prefix, no registry, starts instantly. It is
what decomp.me runs every MSVC on, including `msvc8.0p`.

```
# $VC8 and $WINSDK6 are exported by the devshell
wibo $VC8/Bin/cl.exe /nologo /c /O2 /Oy /GS /EHsc /MT /Gd /Z7 \
     /I"Z:$VC8/INCLUDE" /I"Z:$WINSDK6/Include" \
     /Fo"Z:out.obj" "Z:in.cpp"
```

Guest paths are `Z:` + host path. Verified under wibo: C++ (`c1xx`), C
(`c1`), `windows.h`, `/Z7`. **`/Zi` segfaults** (it spawns `mspdbsrv.exe`, an
RPC server wibo cannot host): use `/Z7` and let `link /DEBUG` build the PDB.
`link.exe` itself is untested under wibo.

**wine** (`pkgs.wineWowPackages.stable`, needs the 32-bit half) is the
fallback for anything wibo cannot do. Use the Microsoft `msvcr80.dll.ms`
there; wine implements activation contexts.

Starting flag set (from codegen heuristics, to be proven on a leaf):
`/O2 /Oy /GS /EHsc /MT /Gd /Z7`. Unknown until matched: `/Ob1` vs `/Ob2`,
`/Gy`, `/GR-` per TU (RTTI is off for most of the binary but not all).

## 2. the compare loop

Minimum viable loop, before any harness:

```
cl  → foo.obj
llvm-objdump -d --x86-asm-syntax=intel foo.obj   # GNU objdump cannot read COFF
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

- Steam runs it under Proton (GE-Proton 11-7 is known to work). The prefix
  at `steamapps/compatdata/21000` is the test environment.
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
(`.?AV` names → classes), `Function ID` (CRT naming; `make fid` adds the exact VC8 SP1 db), `ApplyDataArchive` (`windows_vs12_32`: closest shipped
type archive; VS2005 types are mostly identical for Win32 API), `Aggressive
Instruction Finder` (ours, via `PreAnalysis.java`).

What to do by hand:
- **Decompiler Parameter ID** (Analysis > One Shot): resolves `__thiscall`
  vs `__cdecl` properly. Slow, run once, then save.
- **Random Forest Function Finder** (MachineLearning ext): recover the
  ~40% of `.text` that isn't in a known function yet.
- **FID database from the VC8 CRT**: `make fid` (import `libcmt.lib`/
  `libcpmt.lib` as programs, hash into `ghidra/vc8.fidb`, run the FID
  analyzer). Names the CRT exactly, excludes it from the match budget.
  Re-run `make fid-apply` after recovering more functions.
- **Delinker**: select a function → Relocation table synthesizer → Export
  Program as COFF. That `.obj` is objdiff's "target" side.

Not useful here: PDB analyzers (no PDB), symbol servers.
