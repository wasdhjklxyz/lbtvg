// batman/unk_0040d270.cpp: TU of unknown name, found by its header-static
// copies (the functions after them are not matched yet).

#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"

// FUNCTION: LEGOBATMAN 0x0040d270
static f32 NuFabs(f32 f);
// FUNCTION: LEGOBATMAN 0x0040d290
static f32 NuFdiv(f32 a, f32 b);
// FUNCTION: LEGOBATMAN 0x0040d2e0
static f32 NuFsign(f32 f);
// FUNCTION: LEGOBATMAN 0x0040d4e0
static f32 NuSinApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x0040d5a0
static f32 NuCosApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x0040d5c0
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_0040d270(f32 *v, f32 a, i32 i) {
  v[2] = NuFabs(a);
  v[3] = NuFdiv(a, v[4]);
  v[5] = NuFsign(a);
  v[0] = NuSinApprox(i);
  v[1] = NuCosApprox(i);
  NuVec4Set(v, a, a, a, a);
}
