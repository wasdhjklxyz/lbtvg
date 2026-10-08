# lbtvg

A work-in-progress decompilation of LEGO® Batman™: The Videogame (2008, PC). Not
affiliated with, endorsed by, or sponsored by TT Games, Warner Bros., DC, or the
LEGO Group.

Progress: 123 functions matched, 7,220 / 2,483,972 bytes of game code (0.29%); 14 stubs.

Status: recon done, environment, compiler and ghidra project set up, no code
matched yet. Start at `docs/setup.md`.

- `docs/setup.md` — dependencies and environment, nix or manual.
- `docs/workflow.md` — how one function goes from `FUN_` to matched.
- `docs/toolchain.md` — compiler, compare loop, running the game on Linux.
- `docs/linkmap.md` — which source file owns which address range (generated).
- `docs/recon.md` — what the binary is, which compiler built it, scope numbers.
- `docs/plan.md` — how the decomp is approached and what "done" means.
- `docs/credits.md` — symbol donors, sister decomps, docs and tools this leans on.
