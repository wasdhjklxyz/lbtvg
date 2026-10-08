// gameapi, between terrain.c (0x005632c0) and rtleditor.cpp (0x0058f350) in
// link order (docs/linkmap.md). Real file unknown; move when an anchor says.

// GLOBAL: LEGOBATMAN 0x0095e230
int g_unk_0095e230;

// Called with 0 / 1 around level loading (see callers in ghidra); returns the
// previous value, so it is a save/restore style flag setter.
// FUNCTION: LEGOBATMAN 0x0058b6d0
int SetUnk0095e230(int value)
{
    int old = g_unk_0095e230;
    g_unk_0095e230 = value;
    return old;
}
