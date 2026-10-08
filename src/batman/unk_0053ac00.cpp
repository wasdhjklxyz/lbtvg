// batman/, between pcbatman.cpp (0x00408900) and pcapi.cpp (0x00525100) in
// link order (docs/linkmap.md). Real file unknown.

// Two sub-objects at +0x18 and +0x320, selected by a byte flag at +0x40.
// Their type is unknown; only the offsets are.
struct Unk0053ac00
{
    unsigned char pad0[0x18];
    unsigned char primary[0x40 - 0x18];
    unsigned char flag;
    unsigned char pad1[0x320 - 0x41];
    unsigned char alternate[1];

    void* Current();
};

// Written as "default, then override": VC8 emits the branches in source
// order, and the original loads `primary` first, then conditionally
// `alternate`. `if (flag) return alternate; return primary;` gives the same
// semantics with the two leas swapped, and does not match.
// FUNCTION: LEGOBATMAN 0x0053ac00
void* Unk0053ac00::Current()
{
    void* result = primary;
    if (flag)
        result = alternate;
    return result;
}
