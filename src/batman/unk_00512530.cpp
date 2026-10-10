// batman/unk_00512530.cpp: TU of unknown name, found by its header-static
// copies (the functions after them are not matched yet).

#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"

// FUNCTION: LEGOBATMAN 0x00512530
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);
// FUNCTION: LEGOBATMAN 0x00512620
static f32 NuSinApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x005126e0
static f32 NuCosApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x00512700
static void NuVec4Copy(f32 *dst, f32 *src);

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_00512530(f32 *v, f32 a, i32 i) {
  NuVec4Set(v, a, a, a, a);
  v[0] = NuSinApprox(i);
  v[1] = NuCosApprox(i);
  NuVec4Copy(v + 4, v);
}
