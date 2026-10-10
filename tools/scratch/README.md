Unpolished scripts from the overnight matching runs, kept so the techniques
are not lost. Run inside `nix develop` from the repo root.

- `thunk.py PREFIX TABLE_VA`: walk a {keyword string, code pointer} table in
  .data (following the incremental-link `jmp` thunks) and print keyword ->
  function address, which names parser callbacks exactly
  (e.g. `thunk.py Action_ 0x93bb98`).
- `kwall.py`, `kw2.py`: find all such keyword tables in .data and pair their
  entries with Mac names.
- `gen*.py`: generators used to emit batches of small parser callbacks.
- `condtab.py`: walk the AI condition table ({keyword, condition, init}
  triples around 0x93b1b4) and print the entries with no FUNCTION/STUB yet,
  named `Condition_<keyword>[Init]`.
- `kwsumm.py`: rank keyword tables by unannotated callbacks, counting the
  ones ghidra missed (run `kwall.py` first).
- `callers.py ADDR...`: every `e8`/`e9` rel32 caller of ADDR with the
  containing function's bounds (from int3 padding). A register-convention
  static and all its callers are one TU.
- `scan.py LO HI [MAXSZ]`: unannotated, unskipped small functions in an
  address range, including ones ghidra missed; good for sweeping a file's
  neighbourhood.
- `vtable.py ADDR`: find the vtable(s) holding ADDR (directly or through an
  incremental-link `jmp` thunk) and print every slot resolved. Pair slots
  with the Mac binary's inline emission order to name inline virtuals.
- `rtti.py NAME...`: a class's vtable(s) from its RTTI `.?AV` name, slots
  resolved through thunks.
- `libmap.py LO HI file.c...` then `libmap_check.py`: place a third-party
  library's real source in the exe (each function's exact address, then a
  relocation cross-check). How libvorbis/libogg were mapped.
- `hdrstatics_annotate.py [FILE...]`, `hdrstatics_newtu.py` (both exec
  `hdrstatics_core.py`): VC8 emits an out-of-line copy of every *referenced*
  header static in each TU, even when all calls were inlined (NuSinApprox,
  NuCosApprox, NuFabs, NuFdiv, NuFsign, NuVec4Set, NuVec4Copy,
  NuVecScaleInline; bodies in `nutrig_unk.h` / `nuinline_unk.h`). The first
  annotates the copies in the TU's existing file (prototype under the
  annotation plus an `Unk_InlineUser_*` keep-alive), verifying the whole file
  with match.py and reverting on any regression; the second creates
  `unk_<addr>.cpp` TU files for clusters with no annotated neighbour. Add a
  family to `F` (prefix bytes, exact length) and to the FN/USE tables.
- `tugaps.py [MAXBYTES]`: for each placeholder TU file (one with an
  `Unk_InlineUser_*`), the unannotated functions between its copies and the
  next annotated function of another file, smallest first.
- `rng.py LO HI`: every function start in a range with size, annotation state,
  our name/file and the pc-names.csv Mac pairing.
- `macnb.py MACADDR|NAME [N]`: Mac text symbols around an address or name,
  with the PC address pc-names.csv pairs them to.
- `macdis.py NAME|MACADDR`: disassemble one Mac i386 function (call targets
  named). Function pointers passed as arguments show the Mac's argument
  order, which names PC thunk targets.
- `thk.py ADDR...`: resolve incremental-link `jmp` thunks to their target and
  name.
