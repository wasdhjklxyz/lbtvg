// gameapi, between rtleditor.cpp (0x0058f540) and listman_gen.cpp
// (0x0067dd00) in link order (docs/linkmap.md). Real file unknown.

// Only the one field at +0xb68 is known so far.
struct Unk0062bf50
{
    unsigned char pad[0xb68];
    int field_b68;
};

// FUNCTION: LEGOBATMAN 0x0062bf50
bool IsUnk0062bf50Clear(Unk0062bf50* p)
{
    return p->field_b68 == 0;
}
