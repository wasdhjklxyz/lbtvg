# lbtvg

![match progress](https://img.shields.io/badge/match%20progress-0.29%25-red)

A matching decompilation of *LEGO® Batman™: The Videogame* (PC, 2008).

The goal is C/C++ source that compiles, with the original Visual C++ 2005
SP1 compiler, into the exact machine code of the shipped game. Byte-identical
code is the proof of correctness: same bytes, same behaviour.

## where it is

- The target is the Steam release (`LEGOBatman.exe`, ~3.4 MB of game code on
  TT Games' Nu2 engine). The 2008 original and the PC demo are kept as
  references.
- The exact compiler, a per-function compare harness and a ghidra project are
  set up and reproducible from a fresh clone.
- Function names come from the unstripped 2009 macOS port of the same game,
  paired automatically with the PC build.
- Functions are being matched file by file, mirroring TT's original source
  tree (`nu2api/`, `gameapi/`, `batman/`) as leaked by the binary.

## where it's headed

A complete, matching source tree; then, built on it, a native and portable
version of the game.

## building

You need your own copy of the game. See [docs/setup.md](docs/setup.md).

## credits

Builds on other people's work on TT's engine, notably
[saga](https://github.com/opensagadev/saga) (LEGO Star Wars: The Complete
Saga). Full list in [docs/credits.md](docs/credits.md).

## legal

Not affiliated with, endorsed by, or sponsored by TT Games, Warner Bros., DC,
or the LEGO Group. No game assets or binaries are distributed; the original
game remains the property of its owners.
