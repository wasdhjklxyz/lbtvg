// nu2api/numath/nuvec_unk.cpp: x87 vector helpers between the listman_gen.cpp
// (0x0067dd00) and nugraph_gen.cpp (0x0068e660) anchors. Real file name
// unknown.

#include <math.h>

#include "numath.h"

float NuFsqrt(float f);

// FUNCTION: LEGOBATMAN 0x00684620
void NuVecMtxTranslate(nuvec_s *out, nuvec_s *v, numtx_s *m) {
  out->x = v->x + m->m30;
  out->y = v->y + m->m31;
  out->z = v->z + m->m32;
}

// FUNCTION: LEGOBATMAN 0x00684bc0
void NuVecNeg(nuvec_s *out, nuvec_s *v) {
  out->x = -v->x;
  out->y = -v->y;
  out->z = -v->z;
}

// FUNCTION: LEGOBATMAN 0x00684be0
void NuVecAdd(nuvec_s *out, nuvec_s *a, nuvec_s *b) {
  out->x = a->x + b->x;
  out->y = a->y + b->y;
  out->z = a->z + b->z;
}

// FUNCTION: LEGOBATMAN 0x00684c10
void NuVecSub(nuvec_s *out, nuvec_s *a, nuvec_s *b) {
  out->x = a->x - b->x;
  out->y = a->y - b->y;
  out->z = a->z - b->z;
}

// FUNCTION: LEGOBATMAN 0x00684c40
void NuVecScale(nuvec_s *out, nuvec_s *v, float s) {
  out->x = s * v->x;
  out->y = s * v->y;
  out->z = s * v->z;
}

// FUNCTION: LEGOBATMAN 0x00684df0
float NuVecDot(nuvec_s *a, nuvec_s *b) {
  return a->x * b->x + a->y * b->y + a->z * b->z;
}

// FUNCTION: LEGOBATMAN 0x00684f50
float NuVecMagSqr(nuvec_s *v) {
  return v->x * v->x + v->y * v->y + v->z * v->z;
}

// FUNCTION: LEGOBATMAN 0x00684f80
float NuVecNorm(nuvec_s *out, nuvec_s *v) {
  float mag = NuFsqrt(v->x * v->x + v->y * v->y + v->z * v->z);
  float inv;
  if (mag > 0.0f)
    inv = 1.0f / mag;
  else
    inv = 0.0f;
  out->x = inv * v->x;
  out->y = inv * v->y;
  out->z = inv * v->z;
  return mag;
}

// FUNCTION: LEGOBATMAN 0x00685000
void NuVecSurfaceNormal(nuvec_s *out, nuvec_s *a, nuvec_s *b, nuvec_s *c) {
  nuvec_s d1;
  nuvec_s d2;
  NuVecSub(&d1, a, b);
  NuVecSub(&d2, a, c);
  out->x = d1.z * d2.y - d1.y * d2.z;
  out->y = d2.z * d1.x - d1.z * d2.x;
  out->z = d1.y * d2.x - d1.x * d2.y;
  NuVecNorm(out, out);
}

// FUNCTION: LEGOBATMAN 0x006850a0
float NuVecDistSqr(nuvec_s *a, nuvec_s *b, nuvec_s *d) {
  nuvec_s t;
  if (d) {
    NuVecSub(d, a, b);
    return NuVecMagSqr(d);
  }
  NuVecSub(&t, a, b);
  return NuVecMagSqr(&t);
}

// FUNCTION: LEGOBATMAN 0x00685140
float NuVecXZDistSqr(nuvec_s *a, nuvec_s *b, nuvec_s *d) {
  nuvec_s t;
  if (d) {
    d->x = a->x - b->x;
    d->y = 0.0f;
    d->z = a->z - b->z;
    return NuVecMagSqr(d);
  }
  t.x = a->x - b->x;
  t.y = 0.0f;
  t.z = a->z - b->z;
  return NuVecMagSqr(&t);
}

// FUNCTION: LEGOBATMAN 0x00685670
void NuMtxSetIdentity(numtx_s *m) { *m = numtx_identity; }

// FUNCTION: LEGOBATMAN 0x00685690
void NuMtxSetTranslation(numtx_s *m, nuvec_s *v) {
  m->m30 = v->x;
  m->m31 = v->y;
  m->m32 = v->z;
  m->m23 = 0.0f;
  m->m21 = 0.0f;
  m->m20 = 0.0f;
  m->m13 = 0.0f;
  m->m12 = 0.0f;
  m->m10 = 0.0f;
  m->m03 = 0.0f;
  m->m02 = 0.0f;
  m->m01 = 0.0f;
  m->m33 = 1.0f;
  m->m22 = 1.0f;
  m->m11 = 1.0f;
  m->m00 = 1.0f;
}

// FUNCTION: LEGOBATMAN 0x00685a50
void NuMtxTranslate(numtx_s *m, nuvec_s *v) {
  m->m30 += v->x;
  m->m31 += v->y;
  m->m32 += v->z;
}

// FUNCTION: LEGOBATMAN 0x00689100
float NuFmod(float a, float b) { return a - b * (int)(a / b); }

// GLOBAL: LEGOBATMAN 0x00ad3b6c
unsigned int fseed;

// FUNCTION: LEGOBATMAN 0x0068b530
float NuRandFloat(void) {
  unsigned int bits;
  fseed = fseed * 0x19660d + 0x3c6ef35f;
  bits = (fseed & 0x7fffff) | 0x3f800000;
  return *(float *)&bits - 1.0f;
}
