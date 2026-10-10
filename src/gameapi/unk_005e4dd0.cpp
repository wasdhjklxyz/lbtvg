// gameapi/unk_005e4dd0.cpp: TU of unknown name, found by its header-static
// copies (the functions after them are not matched yet).

#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"

// FUNCTION: LEGOBATMAN 0x005e4dd0
static f32 NuFabs(f32 f);
// FUNCTION: LEGOBATMAN 0x005e4df0
static f32 NuSinApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x005e4e90
static f32 NuCosApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x005e4ea0
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);
// FUNCTION: LEGOBATMAN 0x005e4ee0
static void NuVecScaleInline(f32 *dst, f32 *src, f32 s);

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_005e4dd0(f32 *v, f32 a, i32 i) {
  v[2] = NuFabs(a);
  v[0] = NuSinApprox(i);
  v[1] = NuCosApprox(i);
  NuVec4Set(v, a, a, a, a);
  NuVecScaleInline(v + 8, v, a);
}
