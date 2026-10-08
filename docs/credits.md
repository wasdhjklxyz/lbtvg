# credits and sources

Things this project leans on that it did not produce. Add to it when you use
something new.

## symbol donors

- **LEGO® Batman™: The Videogame, macOS, v1.0.1 (2009-04-24, Feral Interactive
  port).** Shipped unstripped: 22 118 defined symbols, ~12 000 functions,
  C++ names with full signatures. Same source, GCC/Xcode instead of MSVC, so
  names, classes and layouts transfer; bytes do not. Listed on the TT
  modding wiki's "Debug symbols" section
  (https://ttmodding.fandom.com/wiki/EXE_file) and mirrored by JFranco at
  https://www.smakdev.net/share/@JFranco/LegoDebugSymbols/ . The binary is
  not in this repo (`orig/donors/`, gitignored); its symbol table is, at
  `tools/symbols/mac-1.0.1.nm` (+ `.demangled.nm`). sha256 of the binary:
  `0eea1a078d22b15cfd281ef7f8b238c7527b016edccd1314638dbf87b74b83fe`.
- The Oct 2008 PC demo (`orig/LEGOBatmanDemo.exe`), unpacked, with debug-menu
  strings retail lacks. Archived at
  https://hiddenpalace.org/LEGO_Batman:_The_Videogame_(Oct_9,_2008_demo).

## other decompilations of the same engine (Nu2 / "nu2api")

Read for conventions, struct names and workflow; do not copy code without
checking the licence and the engine revision.

- **saga** — LEGO® Star Wars™: The Complete Saga (Android x86), matching
  decomp, GPL-3.0. https://github.com/opensagadev/saga . Same class names
  as our RTTI (`NuSSAOFilter`, `NuDeferredFilter`, `NuPostFilterGen`...),
  `src/nu2api/{nucore,nu3d,numath,nusound,nufile}` layout, `nupad.cpp`.
- **matohero** — BIONICLE® Heroes (PC 2006, MSVC 7.1).
  https://github.com/ChristopherJMiller/matohero . Closest workflow
  analogue: pairs PC functions with PS2-prototype symbols by strings, call
  order, link order and features (`tools/ps2names.py`, `docs/ps2-prototype.md`).
- **lsw1-decomp** — LEGO Star Wars (GameCube).
  https://github.com/ellierocks/lsw1-decomp . `docs/nu2_engine_reference.md`
  documents the Nu2 conventions: `Nu{Subsystem}{Verb}` functions,
  `nu{name}_s` engine structs, `NU{NAME}_s` game structs, `u8/s8/u16/s16/
  u32/s32/f32/f64` types.
- **crashwoc-decomp** — Crash: Wrath of Cortex (GC/PS2), the first Nu2
  game. https://github.com/denzi-gh/crashwoc-decomp
- **OpenCrashWOC** — reimplementation. https://github.com/Open-Travelers/OpenCrashWOC
- **lego-batman-recomp** — experimental static recompilation of this exe.
  https://github.com/Heus-Sueh/lego-batman-recomp

## engine and format documentation

- TT modding wiki: https://ttmodding.fandom.com/wiki/Engine (GSC, GHG,
  glossary, the "Holy Trinity" of moddable Nu2 LEGO games).
- TTGames-Explorer-Rebirth (DAT/GHG/GSC):
  https://github.com/AcK77/TTGames-Explorer-Rebirth
- Bionicle Heroes PS2 prototype with ELF symbols, `.mdebug` and a linker
  map, the authority on original TT names and types for the 2006 engine:
  https://hiddenpalace.org/Bionicle_Heroes_(Sep_4,_2006_prototype) ,
  parsed with https://github.com/chaoticgd/ccc .

## tooling

- Ghidra (NSA), ReVa (cyberkaida), ghidra-delinker-extension (boricj),
  wibo (decompals), objdiff (encounter), reccmp (isledecomp; the annotation
  grammar), Steamless (atom0s), encounter/winedll (Wine's msvcr80 build).
