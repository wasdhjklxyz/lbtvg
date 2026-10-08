// nu2api/numath/numaths_unk.cpp: the TU after the nutrig_gen.cpp anchor
// (0x006900c0); NuVecMag is inlined into NuVecDist, so they share it.

#include "numath.h"

f32 NuFsqrt(f32 f);

// Same-TU copy: NuVecDist only matches with this inlined, not hand-expanded.
static void NuVecSub(nuvec_s *v, nuvec_s *v0, nuvec_s *v1) {
  v->x = v0->x - v1->x;
  v->y = v0->y - v1->y;
  v->z = v0->z - v1->z;
}

// FUNCTION: LEGOBATMAN 0x006903d0
f32 NuVecMag(nuvec_s *v) {
  f32 sqr = v->x * v->x + v->y * v->y + v->z * v->z;
  return NuFsqrt(sqr);
}

// FUNCTION: LEGOBATMAN 0x00690410
f32 NuVecDist(nuvec_s *v0, nuvec_s *v1, nuvec_s *d) {
  nuvec_s dist;
  if (d) {
    NuVecSub(d, v0, v1);
    return NuVecMag(d);
  }
  NuVecSub(&dist, v0, v1);
  return NuVecMag(&dist);
}
