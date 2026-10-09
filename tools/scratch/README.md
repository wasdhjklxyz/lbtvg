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
