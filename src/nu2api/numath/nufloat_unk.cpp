// nu2api/numath/nufloat_unk.cpp: NuFsqrt sits between NuVecDist (0x00690410)
// and NuVec4MtxTransformVU0 (0x00691db0) yet is not inlined into either, so it
// is its own TU.

#include <math.h>

#include "../nucore/common.h"
#include "./nuinline_unk.h"

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x00691b10
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

// FUNCTION: LEGOBATMAN 0x00691b30
f32 NuFsqrt(f32 f) {
  if (f < 0.0f)
    return 0.0f;
  return sqrtf(f);
}

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_nufloat_unk(f32 *v, f32 a, i32 i) {
  NuVec4Set(v, a, a, a, a);
}
