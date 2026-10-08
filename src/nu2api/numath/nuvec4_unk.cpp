// nu2api/numath/nuvec4_unk.cpp: VU0-named generic fallbacks after NuFsqrt
// (0x00691b30), like saga's numaths.c.

#include "numath.h"
#include "nuvec4.h"

// FUNCTION: LEGOBATMAN 0x00691db0
void NuVec4MtxTransformVU0(nuvec4_s *out, nuvec4_s *in, numtx_s *m) {
  f32 y = in->x * m->m01 + in->y * m->m11 + in->z * m->m21 + in->w * m->m31;
  f32 z = in->x * m->m02 + in->y * m->m12 + in->z * m->m22 + in->w * m->m32;
  f32 w = in->x * m->m03 + in->y * m->m13 + in->z * m->m23 + in->w * m->m33;
  out->x = in->x * m->m00 + in->y * m->m10 + in->z * m->m20 + in->w * m->m30;
  out->y = y;
  out->z = z;
  out->w = w;
}
