// batman/, between pcbatman.cpp (0x00408900) and pcapi.cpp (0x00525100) in
// link order (docs/linkmap.md). Real file unknown.

// 16 bytes: the compiler zeroes +0 and +8 through the FPU (fldz/fst) and +4
// and +0xc through eax, so the layout is float, int, float, int.
struct Unk0051e910
{
    float f0;
    int   i4;
    float f8;
    int   ic;

    void Reset();
};

// FUNCTION: LEGOBATMAN 0x0051e910
void Unk0051e910::Reset()
{
    f0 = 0.0f;
    i4 = 0;
    f8 = 0.0f;
    ic = 0;
}
