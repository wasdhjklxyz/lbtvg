// nu2api/numath/nuvec4_unk.cpp: VU0-named generic fallbacks after NuFsqrt
// (0x00691b30), like saga's numaths.c.

#include "./nuinline_unk.h"
#include "./nutrig_unk.h"
#include "numath.h"
#include "nuvec4.h"

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x00691c20
static f32 NuSinApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x00691cc0
static f32 NuCosApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x00691cd0
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

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

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_nuvec4_unk(f32 *v, f32 a, i32 i) {
  v[0] = NuSinApprox(i);
  v[1] = NuCosApprox(i);
  NuVec4Set(v, a, a, a, a);
}
