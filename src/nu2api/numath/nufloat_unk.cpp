// nu2api/numath/nufloat_unk.cpp: NuFsqrt sits between NuVecDist (0x00690410)
// and NuVec4MtxTransformVU0 (0x00691db0) yet is not inlined into either, so it
// is its own TU.

#include <math.h>

#include "../nucore/common.h"

// FUNCTION: LEGOBATMAN 0x00691b30
f32 NuFsqrt(f32 f) {
  if (f < 0.0f)
    return 0.0f;
  return sqrtf(f);
}
