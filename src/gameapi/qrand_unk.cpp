// gameapi/qrand_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nu2api/nucore/common.h"
#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"
#include <stddef.h>

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x005adfa0
static f32 NuFabs(f32 f);
// FUNCTION: LEGOBATMAN 0x005adfc0
static f32 NuFdiv(f32 a, f32 b);
// FUNCTION: LEGOBATMAN 0x005ae000
static f32 NuSinApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x005ae0a0
static f32 NuCosApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x005ae0b0
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

i32 qseed = 0x3039;

// from saga legoapi/core/input/qrand.cpp
// FUNCTION: LEGOBATMAN 0x005ae160
i32 qrand(void) {
  qseed = qseed * 0x24cd + 1 & 0xffff;

  return qseed;
}

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_qrand_unk(f32 *v, f32 a, i32 i) {
  v[2] = NuFabs(a);
  v[3] = NuFdiv(a, v[4]);
  v[0] = NuSinApprox(i);
  v[1] = NuCosApprox(i);
  NuVec4Set(v, a, a, a, a);
}
