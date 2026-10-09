Unpolished scripts from the overnight matching runs, kept so the techniques
are not lost. Run inside `nix develop` from the repo root.

- `thunk.py PREFIX TABLE_VA`: walk a {keyword string, code pointer} table in
  .data (following the incremental-link `jmp` thunks) and print keyword ->
  function address, which names parser callbacks exactly
  (e.g. `thunk.py Action_ 0x93bb98`).
- `kwall.py`, `kw2.py`: find all such keyword tables in .data and pair their
  entries with Mac names.
- `gen*.py`: generators used to emit batches of small parser callbacks.
