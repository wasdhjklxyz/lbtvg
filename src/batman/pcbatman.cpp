// batman/pcbatman.cpp: the first object the linker saw (WinMain lives here).
// Functions between 0x00401000 and the __FILE__ anchor at 0x00408900 are
// placed here by link order (docs/linkmap.md); nothing in that range
// references any other file.

// Four floats at +0..+0xc, no other evidence yet.
struct Unk00404420
{
    float f0;
    float f4;
    float f8;
    float fc;

    Unk00404420* Set(float a, float b, float c, float d);
};

// Returns this: the original copies ecx to eax before the stores, and eax
// is the return register. Without the return value the mov disappears.
// FUNCTION: LEGOBATMAN 0x00404420
Unk00404420* Unk00404420::Set(float a, float b, float c, float d)
{
    f0 = a;
    f4 = b;
    f8 = c;
    fc = d;
    return this;
}
