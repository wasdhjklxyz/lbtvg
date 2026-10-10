#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x0051c6c0
static f32 NuFabs(f32 f);
// FUNCTION: LEGOBATMAN 0x0051c6e0
static f32 NuSinApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x0051c780
static f32 NuCosApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x0051c790
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);
// batman/, file unknown (between pcbatman.cpp and pcapi.cpp by link order).

// +0 and +8 are zeroed via fldz/fst (float), +4 and +0xc via eax (int).
struct Unk0051e910 {
  float f0;
  int i4;
  float f8;
  int ic;

  void Reset();
};

// FUNCTION: LEGOBATMAN 0x0051e910
void Unk0051e910::Reset() {
  f0 = 0.0f;
  i4 = 0;
  f8 = 0.0f;
  ic = 0;
}

// GLOBAL: LEGOBATMAN 0x009d02a9
extern signed char g_pcInputUnk009d02a9;

// FUNCTION: LEGOBATMAN 0x0051f540
int PcInput_GetCtrlStringPlayer() { return g_pcInputUnk009d02a9; }

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_unk_0051e910(f32 *v, f32 a, i32 i) {
  v[2] = NuFabs(a);
  v[0] = NuSinApprox(i);
  v[1] = NuCosApprox(i);
  NuVec4Set(v, a, a, a, a);
}
