// nu2api/numath/nuvec_unk.cpp: x87 vector helpers between the listman_gen.cpp
// (0x0067dd00) and nugraph_gen.cpp (0x0068e660) anchors. Real file name
// unknown.

#include <math.h>

#include "numath.h"
#include "nuplane.h"
#include "nutrig_unk.h"

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

// FUNCTION: LEGOBATMAN 0x0068b4d0
void NuRandSeed(unsigned int seed) { fseed = seed; }

// FUNCTION: LEGOBATMAN 0x0068b4e0
unsigned int NuRandGetSeed(void) { return fseed; }

// FUNCTION: LEGOBATMAN 0x0068b4f0
float NuRandFloatSeeded(unsigned int *seed) {
  unsigned int bits;
  *seed = *seed * 0x19660d + 0x3c6ef35f;
  bits = (*seed & 0x7fffff) | 0x3f800000;
  return *(float *)&bits - 1.0f;
}

// FUNCTION: LEGOBATMAN 0x0068b530
float NuRandFloat(void) {
  unsigned int bits;
  fseed = fseed * 0x19660d + 0x3c6ef35f;
  bits = (fseed & 0x7fffff) | 0x3f800000;
  return *(float *)&bits - 1.0f;
}

// FUNCTION: LEGOBATMAN 0x0068b590
unsigned int NuRandInt(void) {
  fseed = fseed * 0x19660d + 0x3c6ef35f;
  return fseed;
}

// GLOBAL: LEGOBATMAN 0x0096962c
extern int qseed;

// FUNCTION: LEGOBATMAN 0x0068b5b0
int qrand(void) {
  qseed = qseed * 0x24cd + 1 & 0xffff;
  return qseed;
}

typedef struct NURAND {
  int value;
} NURAND;

// GLOBAL: LEGOBATMAN 0x00969730
extern NURAND global_rand;

// FUNCTION: LEGOBATMAN 0x0068b5d0
void NuRandSetSeed(NURAND *rand, int seed) {
  if (rand == 0)
    rand = &global_rand;
  rand->value = seed;
}

// FUNCTION: LEGOBATMAN 0x0068b5f0
int NuRand(NURAND *rand) {
  if (rand != 0) {
    if (rand->value == 0)
      rand->value = 1;
  } else {
    rand = &global_rand;
  }
  int x = rand->value ^ 0x075bd924;
  rand->value = x * 0x41a7 - (x / 0x1f31d) * 0x7fffffff;
  if (rand->value < 0)
    rand->value += 0x7fffffff;
  rand->value ^= 0x075bd924;
  return rand->value;
}

// FUNCTION: LEGOBATMAN 0x00684190
static f32 NuSinApprox(i32 angle);

// FUNCTION: LEGOBATMAN 0x00684480
void NuVecMtxTransform(nuvec_s *out, nuvec_s *v, numtx_s *m) {
  f32 y = v->x * m->m01 + v->y * m->m11 + v->z * m->m21 + m->m31;
  f32 z = v->x * m->m02 + v->y * m->m12 + v->z * m->m22 + m->m32;
  out->x = v->x * m->m00 + v->y * m->m10 + v->z * m->m20 + m->m30;
  out->y = y;
  out->z = z;
}

// FUNCTION: LEGOBATMAN 0x00684650
void NuVecMtxRotate(nuvec_s *out, nuvec_s *v, numtx_s *m) {
  f32 y = v->x * m->m01 + v->y * m->m11 + v->z * m->m21;
  f32 z = v->x * m->m02 + v->y * m->m12 + v->z * m->m22;
  out->x = v->x * m->m00 + v->y * m->m10 + v->z * m->m20;
  out->y = y;
  out->z = z;
}

// STUB: LEGOBATMAN 0x006849f0
// 100/100 bytes but the v->y line multiplies v0->z from memory instead of
// loading it first; operand/temp order not found.
void NuVecRotateX(nuvec_s *v, nuvec_s *v0, i32 a) {
  f32 c = NuSinApprox(a + 0x4000);
  f32 s = NuSinApprox(a);
  f32 y = v0->y;
  v->x = v0->x;
  v->y = y * c - v0->z * s;
  v->z = y * s + v0->z * c;
}

// FUNCTION: LEGOBATMAN 0x00684a60
void NuVecRotateY(nuvec_s *v, nuvec_s *v0, i32 a) {
  f32 c = NuSinApprox(a + 0x4000);
  f32 s = NuSinApprox(a);
  f32 x = v0->x;
  v->x = x * c + v0->z * s;
  v->y = v0->y;
  v->z = v0->z * c - x * s;
}

// FUNCTION: LEGOBATMAN 0x0068a6a0
void NuPlnEqn(nuplane_s *out, nuvec_s *pnt0, nuvec_s *pnt1, nuvec_s *pnt2) {
  nuvec_s v1;
  nuvec_s v2;
  nuvec_s normal;
  NuVecSub(&v1, pnt1, pnt0);
  NuVecSub(&v2, pnt2, pnt0);
  normal.x = v1.y * v2.z - v1.z * v2.y;
  normal.y = v1.z * v2.x - v1.x * v2.z;
  normal.z = v1.x * v2.y - v1.y * v2.x;
  NuVecNorm((nuvec_s *)out, &normal);
  out->d = -(out->a * pnt0->x + out->b * pnt0->y + out->c * pnt0->z);
}
