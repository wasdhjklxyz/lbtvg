# workflow

How one function goes from `FUN_xxxxxxxx` to matched. Everything below runs
inside `nix develop` (or the manual environment from `docs/setup.md`) with
the compiler fetched (`make vc8`) and the ghidra project built
(`make ghidra-import`, `make fid`).

## 0. the short way

`make new FUNC=0x...` (or plain `make new` for a random easy todo) does steps
2–4 for you: picks the file (written
neighbours, then subsystem, then saga's file name), inserts saga's body in
address order (pulling missing globals from saga), compiles it and runs the
match. No saga body: it leaves a TODO with the Mac signature. Then fix the
DIFF if there is one, `make fmt`, commit.

## 1. pick a function

Small first. `ghidra/LEGOBatman.exe.stats.tsv` lists every function with its
size; ~475 of them in the game region are 16–48 bytes and make no calls.
Open it in ghidra (`make ghidra`): decompiler for the shape, listing for the
truth, callers for what it is *for*.

## 2. find its home

Source files are laid out in link order and 62 functions reference a leaked
`__FILE__` string, so `docs/linkmap.md` brackets every address between two
known files:

```
tools/linkmap.py 0x0058b6d0
0058b6d0: after [005632c0 terrain.c]  before [0058f350 rtleditor.cpp]
```

- If an anchor pins the file, use it: `src/nu2api/numath/nutrig_gen.cpp`.
- Otherwise use the directory the bracket implies and a placeholder file
  named after the function: `src/gameapi/unk_0058b6d0.cpp`. Move it when
  evidence arrives; the annotation is the identity, not the file.

The tree under `src/` mirrors TT's (`nu2api/`, `gameapi/`, `batman/`), see
`docs/recon.md`.

## 3. write it

reccmp-style annotations, one per function, directly above the definition:

```cpp
// FUNCTION: LEGOBATMAN 0x0058b6d0
int SetUnk0095e230(int value)
{
    int old = g_unk_0095e230;
    g_unk_0095e230 = value;
    return old;
}
```

`// GLOBAL: LEGOBATMAN 0x...` above globals, `// VTABLE:` / `// STRING:`
when those come up (same grammar as reccmp, so its tools apply later).

Naming when the real name is unknown: describe what it does, keep the
address in the name (`SetUnk0095e230`, `struct Unk0051e910`). Rename
everywhere when a caller or a string reveals the real name. Never invent a
plausible-sounding name; that is how wrong names become permanent.

Structs: only declare the fields you have evidence for; pad the rest with
`unsigned char pad[N]` so offsets are explicit. The compiler tells you
types: a field zeroed through `fldz`/`fst` is a float, one zeroed through
`eax` is an int.

## 4. match

```
make match                # every annotated function
make match FUNC=0x0058b6d0
make match-v FUNC=...     # side-by-side disassembly even on a match
```

`tools/match.py` compiles the TU with the VC8 flag set (one place, `CFLAGS`
in the script), finds the function in the `.obj` by demangled name, masks
the 4 bytes at every relocation (the object does not know final addresses;
they show as `??`), and compares against `orig/LEGOBatman.exe` at the
annotated address. Exit code = number of differing functions.

A `DIFF` prints original and recompiled disassembly side by side with `!`
on differing lines. Common causes, in order of likelihood: wrong operation
order or types in the source, a struct field type (`int` vs `float` changes
the instruction), a missing `const`, inlining (`/Ob1` vs `/Ob2`), the
calling convention (`__thiscall` needs a member function).

## 5. commit

`git add src/ && git commit` (no `-m`): with the hooks installed (`make
hooks`, once per clone) the message is pre-filled as `src: <names>` plus the
saga files they came from, and every commit that touches `src/` is
clang-formatted, re-checked with the harness (refused if a `FUNCTION` no
longer matches), and regenerates the README badge, `docs/todo.md` and `site/data.json` (the
progress map at https://wasdhjklxyz.github.io/lbtvg/, published on push)
into the same commit.

## 6. pick the next one

`docs/todo.md` lists every function with a known name that is not done,
grouped by subsystem, smallest first, with the saga file when
opensagadev/saga already has a body for it. It updates itself; never edit it.

## what the numbers mean

`N/M bytes matched` counts only annotated functions. The project-wide
denominator is the game's non-library `.text` (≈3.39 MB, `docs/recon.md`);
progress = matched bytes / that. A progress target will land once there is
enough matched code for the percentage to mean something.

## VC8 habits learned so far

- **Branch order is source order.** `x = a; if (c) x = b;` and
  `if (c) return b; return a;` are the same program and different bytes.
- **`mov eax, ecx` at entry** of a member function usually means it returns
  `this`.
- **Float literals in float arithmetic are promoted to double** (`/fp:precise`):
  the constant lands in `.rdata` as a qword holding the float's value, and
  each float assignment rounds via `fstp`/`fld dword`. Compares and call
  arguments keep dword constants.
- **`x = 0; if (c) x = v;` differs from `if (c) x = v; else x = 0;`** on the
  x87 stack: the first stores the 0 early, the second keeps it and uses `fxch`.
- **Extra stack frame (`sub esp, N`) in a small maths function** means
  address-taken locals, i.e. a same-TU helper like `NuVecSub(&tmp, ...)` was
  inlined, not hand-written temporaries.
- **Duplicate epilogues** (two identical "clear and return 0" tails) mean the
  source had two separate returns; merging them moves callee-saved pushes.
- **Argument pushes are reused** across consecutive calls with identical
  trailing arguments, and `add esp` is deferred over several calls: call order
  in the source must be exact.
- **Static functions can get custom register conventions** even without
  `/GL` (arguments in `esi`/`edi`). They only exist if a caller in the same
  TU references them; match the caller and the static together.
- **Parentheses around a product are visible**: `x + (x3 * -c)` keeps `faddp`
  with a negative constant; `x + x3 * -c` becomes `fsubp` with a positive one.
- **Comparison spelling matters even against zero**: `d < 0.0f` compares
  through `fcomp [mem]`; `0.0f > d` loads a qword 0.0 and compares differently.
  `?:` and if/else differ too (the ternary shares the final `ret`).
- **`test byte [x+1], 4` on a flags word** means the source tested a byte (or
  bitfield), not `u32 & 0x400`.
- **`mov eax, 1; cmp [local], eax`** (the constant parked in the return
  register) comes from wrapping the body in `if (flags) { ... } return 1;`; an
  early `if (!flags) return 1;` gives `cmp [local], 1`.
- **Dead parameter home slots get reused for locals**, and which local lands
  there is not declaration order.
- **Two calls in if/else with different string arguments tail-merge** into
  one call with the shared trailing arguments pushed before the branch; a
  `c ? "A" : "B"` argument instead gives `mov eax, str` in each arm.
- **`static` globals are not reloaded after float stores**; `extern` ones are.
  If a global is re-read in yours but not in the original, make it `static`
  in its file.
- **Callees from another file that appear inlined** (`NuStrCpy`, small file
  readers) match when copied in as a `static inline` helper or an open-coded
  loop; large ones need `__forceinline`.
- **A `/GS` cookie on a local struct** with `char` padding arrays: switch the
  padding to `u32` arrays and the cookie goes away.
- `make new` skips addresses listed in `tools/symbols/skip.txt` when picking at
  random: add the ones you gave up on, with a reason.
- **Local initialisers are stored in declaration order, floats first.**
  Reordering declarations fixes the store order.
- **Distinct globals don't alias**: writing each field of `tab[count]` keeps
  `count` in a register; going through a pointer may not.
- **`s = s + NuStrLen("x") + 1; f(s);`** and `f(s + NuStrLen("x") + 1)`
  allocate registers differently.
- **Adjacent 1-bit bitfields** set from variables merge into one
  and/or/shl sequence.
- **With a /GS buffer, VC8 copies pointer parameters into locals.**
- **A pointer walk (`obj++`)** strength-reduces; `Obj[i]` reloads `Obj` after
  every call.
- **Unsolved:** some originals keep a separate epilogue per early return where
  our build merges them (Action_SetPath, CheckGizAIMessage,
  GetNamedAPIObject). Possibly a per-file flag difference; worth testing
  `/Ob`/`/Oy` variants on one of them.
- **Check then reload**: `if (p->x == 0) return; obj = p->x;` gives
  `cmp [mem], 0` followed by a reload of the field.
- **A loop guard read straight from memory** (`cmp [count]`) means no explicit
  `count > 0` check before the `for`.
- **`b = (x < 0)`** stores through `al`; `if (x < 0) b = 1; else b = 0;`
  stores immediate bytes.
- **`a > b + c` vs `b + c < a`** change the x87 load order.
- **`memset(&node, 0, 8)`** is the `xor eax; mov; mov` that zeroes a list link.
- **A callee defined in the same file changes the caller's register
  allocation**, even without `/GL`: if a caller stops matching when its callee
  is added next to it, they were in different files.
- **Empty varargs debug statics vanish** when called from an inline helper;
  declare them `extern` to keep the call.
- **The same array access written twice** (`a->p->arr[i] != 0 &&
  a->p->arr[i]->f`) gives "cmp [mem], 0 then reload"; a local gives one load.
  Swapping a repeated expression for a local (or back) is a cheap first try
  on a register-allocation diff.
- **An empty static varargs stub** keeps its calls (and the compiler knows it
  clobbers nothing) only if a call passing it a pointer appears earlier in the
  file; an `extern` declaration keeps the call but loses that knowledge.
- **`x = NULL; if ... else if ... x = f(); return x;`** gives a separate
  epilogue per branch.
- **A 3-byte `char` padding array** also triggers the /GS cookie.
- **Inlined copies keep the source's store order** even when the standalone
  copy of the callee was scheduled differently: trust the inlined one.
- `make match FUNC=0x...` on a STUB test-matches it (`STUB-MATCH` means flip it
  to `// FUNCTION:`); stubs never count otherwise.
- **`return c ? 1.0f : 0.0f;`** goes through a stack temporary;
  `if (c) return 1.0f; return 0.0f;` doesn't. A store/reload through the
  parameter slot comes from a *double* ternary (`c ? 1.0 : 0.0`).
- **xor/and/xor on a flag word** is a bitfield assignment: use a bitfield
  struct.
- **A function pointer tested then called** is copied to a local first.
- **`if (a) return 1; if (b) return 1; return 0;`** lays out differently from
  `if (a || b)`.
- **`flags & 0x80000000`** gives `jns`; `(i32)flags < 0` gives `jge`.
- **Copying a global table pointer into a local** keeps the loop's compare
  value in a register.
- **Ghidra misses many small table-reached functions**: scan for `int3`
  padding between known functions to find their starts, then pair them with
  the Mac order.
