# LEGO Batman: The Videogame Decompilation

## Recon

- Is it packed? Steam builds have SteamStub wrapper `steamless` strips it.
- Rich header in PE tells exact MSVC build numbers that compiled and linked it.
- RTTI present? It's C++ from 2008, so almost certainly yes: type descriptors
  give class names, vtables give hierarchies for free. Biggest gift.
- Debug/assert strings with source path leaks original dir layout and funcs.
- Import table tells you external libs. Statically linked libs get identified
  via Ghidra FunctionID or IDA FLIRT and excluded from work.

## Toolchain & Compare Harness

Get matching `cl.exe`, run it under wine, and prove the loop works on one
trivial function. Write C++ -> compile -> diff against original bytes -> match.

See `reccmp` for compare tooling or `objdiff`. https://decomp.me for
scratch-matching individual functions if it has your MSVC version.

## Where to Start in Binary

Leaf functions first: tiny, no calls out, math/containers/string utils. Learn
the compiler's codegen habits on things with one obvious answer. Then walk up:
constructors (RTTI tells classes), then subsystems. First hundred functions are
slow then pattern recognition kicks in.

## Done

Matched bytes/total non-library bytes. Not function count. Keep buckets:
`matching`, `decompiled but not matching`, `untouched`, `library (excluded)`.
Script computs it from annotations and you put the number in the README. See
isledecomp copy that script.

## Testing

Matching decomp, the match *is* the test. Identical bytes, identical behavior,
no further argument. The final test is the full rebuild diffing clean.

Behavior testing before that (for non-matching functions or sanity), use
function replacement: inject a DLL into the original game, detour one original
function to your reimplementation, play. Game still works -> function is
behaviorally right.

## Ideas

Multiplayer play the co-op thing online would be cool.
