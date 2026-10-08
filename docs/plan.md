# plan

How this decompilation is approached and what "done" means. `docs/recon.md`
has the findings, `docs/toolchain.md` the tooling, `docs/setup.md` the
environment.

## recon (done)

- Packed? Steam builds get a SteamStub wrapper; `steamless` strips it.
- Rich header in the PE gives the exact MSVC build numbers that compiled and
  linked it.
- RTTI? C++ from 2008, so expected: type descriptors give class names,
  vtables give hierarchies for free.
- Debug/assert strings with source paths leak the original directory layout.
- Import table lists external libs. Statically linked libs get identified
  via Ghidra FunctionID (or IDA FLIRT) and excluded from the work.

## toolchain and compare harness (done)

The matching `cl.exe` runs on Linux under wibo (`make vc8`), and
`make match` compiles every annotated function and diffs its bytes against
the original (`docs/workflow.md`). `objdiff` + the Ghidra delinker remain
available for object-level diffs; `reccmp` later for whole-binary
percentages. https://decomp.me has `msvc8.0p` for scratch-matching.

## where to start in the binary

Leaf functions first: tiny, no calls out, math / containers / string utils.
Learn the compiler's codegen habits on things with one obvious answer. Then
walk up: constructors (vtables tell the classes), then subsystems. The first
hundred functions are slow; then pattern recognition kicks in.

## definition of done

Matched bytes / total non-library bytes. Not function count. Keep buckets:
`matching`, `decompiled but not matching`, `untouched`, `library (excluded)`.
A script computes the number from annotations and it goes in the README
(see isledecomp's progress script for the shape).

## testing

For a matching decomp, the match *is* the test: identical bytes, identical
behavior. The final test is a full rebuild diffing clean.

Before that, for non-matching functions or sanity: function replacement.
Inject a DLL into the original game (a proxy `dinput8.dll` works, see
`docs/toolchain.md`), detour one original function to the reimplementation,
play. Game still works → the function is behaviorally right.

## long-term ideas

Not goals, just where a finished decomp could go:

- Native Linux build: `nu2api`'s `pc/` layer is the platform boundary;
  replace Win32/D3D9 behind it. The macOS port proves the engine survives it.
- Online co-op (the game only has local co-op).
