// nu2api/numath/nuvec_unk.cpp: x87 vector helpers between the listman_gen.cpp
// (0x0067dd00) and nugraph_gen.cpp (0x0068e660) anchors. Real file name
// unknown.

#include <math.h>

#include "./nuinline_unk.h"
#include "numath.h"
#include "nuplane.h"
#include "nutrig_unk.h"

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x00683fc0
static f32 NuFdiv(f32 a, f32 b);
// FUNCTION: LEGOBATMAN 0x00683f70
static f32 NuFabs(f32 f);
// FUNCTION: LEGOBATMAN 0x00684000
static f32 NuFsign(f32 f);
// FUNCTION: LEGOBATMAN 0x00684230
static f32 NuCosApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x00684330
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

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

// FUNCTION: LEGOBATMAN 0x00684c70
void NuVecAddScale(nuvec_s *v, nuvec_s *v0, nuvec_s *v1, f32 k) {
  v->x = v0->x + v1->x * k;
  v->y = v0->y + v1->y * k;
  v->z = v0->z + v1->z * k;
}

// FUNCTION: LEGOBATMAN 0x00684cb0
void NuVecScaleAccum(nuvec_s *v, nuvec_s *v0, f32 k) {
  v->x += v0->x * k;
  v->y += v0->y * k;
  v->z += v0->z * k;
}

// FUNCTION: LEGOBATMAN 0x00684ce0
void NuVecInvScale(nuvec_s *v, nuvec_s *v0, f32 k) {
  f32 ki;

  if (k == 0.0f) {
    ki = 0.0f;
  } else {
    ki = 1.0f / k;
  }

  v->x = v0->x * ki;
  v->y = v0->y * ki;
  v->z = v0->z * ki;
}

// x87 operand order of two products differs; VC ignores source operand order.
// STUB: LEGOBATMAN 0x00684d30
void NuVecCross(nuvec_s *v, nuvec_s *v0, nuvec_s *v1) {
  f32 y, z;
  y = v0->z * v1->x - v1->z * v0->x;
  z = v0->x * v1->y - v1->x * v0->y;
  v->x = v0->y * v1->z - v1->y * v0->z;

  v->y = y;
  v->z = z;
}

// FUNCTION: LEGOBATMAN 0x00684df0
float NuVecDot(nuvec_s *a, nuvec_s *b) {
  return a->x * b->x + a->y * b->y + a->z * b->z;
}

// FUNCTION: LEGOBATMAN 0x00684e20
f32 NuVecMagXZ(nuvec_s *v0) { return NuFsqrt(v0->x * v0->x + v0->z * v0->z); }

// FUNCTION: LEGOBATMAN 0x00684e50
void NuVecMax(nuvec_s *v, nuvec_s *v0, nuvec_s *v1) {
  v->x = (v0->x > v1->x) ? v0->x : v1->x;
  v->y = (v0->y > v1->y) ? v0->y : v1->y;
  v->z = (v0->z > v1->z) ? v0->z : v1->z;
}

// FUNCTION: LEGOBATMAN 0x00684ed0
void NuVecMin(nuvec_s *v, nuvec_s *v0, nuvec_s *v1) {
  v->x = (v0->x < v1->x) ? v0->x : v1->x;
  v->y = (v0->y < v1->y) ? v0->y : v1->y;
  v->z = (v0->z < v1->z) ? v0->z : v1->z;
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

// GLOBAL: LEGOBATMAN 0x00ad3b90
extern numtx_s numtx_zero;

// FUNCTION: LEGOBATMAN 0x00685650
void NuMtxSetZero(numtx_s *m) { *m = numtx_zero; }

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

// FUNCTION: LEGOBATMAN 0x006856e0
void NuMtxSetTranslationNeg(numtx_s *m, nuvec_s *t) {
  m->m30 = -t->x;
  m->m31 = -t->y;
  m->m32 = -t->z;
  m->m01 = m->m02 = m->m03 = m->m10 = m->m12 = m->m13 = m->m20 = m->m21 =
      m->m23 = 0.0f;
  m->m00 = m->m11 = m->m22 = m->m33 = 1.0f;
}

// FUNCTION: LEGOBATMAN 0x00685730
void NuMtxSetScale(numtx_s *m, nuvec_s *s) {
  m->m00 = s->x;
  m->m11 = s->y;
  m->m22 = s->z;
  m->m01 = m->m02 = m->m03 = m->m10 = m->m12 = m->m13 = m->m20 = m->m21 =
      m->m23 = m->m30 = m->m31 = m->m32 = 0.0f;
  m->m33 = 1.0f;
}

// FUNCTION: LEGOBATMAN 0x00685780
void NuMtxSetRotationX(numtx_s *m, i32 a) {
  m->m11 = m->m22 = NuSinApprox(a + 0x4000);
  m->m12 = NuSinApprox(a);
  m->m21 = -m->m12;
  m->m00 = 1.0f;
  m->m01 = m->m02 = m->m03 = m->m23 = m->m10 = m->m20 = m->m13 = m->m30 =
      m->m31 = m->m32 = 0.0f;
  m->m33 = 1.0f;
}

// FUNCTION: LEGOBATMAN 0x006857f0
void NuMtxSetRotationY(numtx_s *m, i32 a) {
  m->m00 = m->m22 = NuSinApprox(a + 0x4000);
  m->m20 = NuSinApprox(a);
  m->m02 = -m->m20;
  m->m11 = 1.0f;
  m->m01 = m->m10 = m->m03 = m->m23 = m->m12 = m->m21 = m->m13 = m->m30 =
      m->m31 = m->m32 = 0.0f;
  m->m33 = 1.0f;
}

// FUNCTION: LEGOBATMAN 0x00685860
void NuMtxSetRotationZ(numtx_s *m, i32 a) {
  m->m00 = m->m11 = NuSinApprox(a + 0x4000);
  m->m01 = NuSinApprox(a);
  m->m10 = -m->m01;
  m->m22 = 1.0;
  m->m02 = m->m12 = m->m03 = m->m23 = m->m20 = m->m21 = m->m13 = m->m30 =
      m->m31 = m->m32 = 0.0f;
  m->m33 = 1.0;
}

// FUNCTION: LEGOBATMAN 0x00685a50
void NuMtxTranslate(numtx_s *m, nuvec_s *v) {
  m->m30 += v->x;
  m->m31 += v->y;
  m->m32 += v->z;
}

// FUNCTION: LEGOBATMAN 0x00685a80
void NuMtxTranslateNeg(numtx_s *m, nuvec_s *t) {
  m->m30 -= t->x;
  m->m31 -= t->y;
  m->m32 -= t->z;
}

// FUNCTION: LEGOBATMAN 0x00685ab0
void NuMtxPreTranslate(numtx_s *m, nuvec_s *t) {
  m->m30 += t->x * m->m00 + t->y * m->m10 + t->z * m->m20;
  m->m31 += t->x * m->m01 + t->y * m->m11 + t->z * m->m21;
  m->m32 += t->x * m->m02 + t->y * m->m12 + t->z * m->m22;
}

// FUNCTION: LEGOBATMAN 0x00685b10
void NuMtxPreTranslateX(numtx_s *m, f32 tx) {
  m->m30 = m->m30 + m->m00 * tx;
  m->m31 = m->m31 + m->m01 * tx;
  m->m32 = m->m32 + m->m02 * tx;
}

// FUNCTION: LEGOBATMAN 0x00685b40
void NuMtxPreTranslateNeg(numtx_s *m, nuvec_s *t) {
  m->m30 -= t->x * m->m00 + t->y * m->m10 + t->z * m->m20;
  m->m31 -= t->x * m->m01 + t->y * m->m11 + t->z * m->m21;
  m->m32 -= t->x * m->m02 + t->y * m->m12 + t->z * m->m22;
}

// FUNCTION: LEGOBATMAN 0x00685ba0
void NuMtxScale(numtx_s *m, nuvec_s *s) {
  m->m00 *= s->x;
  m->m01 *= s->y;
  m->m02 *= s->z;
  m->m10 *= s->x;
  m->m11 *= s->y;
  m->m12 *= s->z;
  m->m20 *= s->x;
  m->m21 *= s->y;
  m->m22 *= s->z;
  m->m30 *= s->x;
  m->m31 *= s->y;
  m->m32 *= s->z;
}

// FUNCTION: LEGOBATMAN 0x00685c10
void NuMtxScaleU(numtx_s *m, f32 s) {
  m->m00 *= s;
  m->m01 *= s;
  m->m02 *= s;
  m->m10 *= s;
  m->m11 *= s;
  m->m12 *= s;
  m->m20 *= s;
  m->m21 *= s;
  m->m22 *= s;
  m->m30 *= s;
  m->m31 *= s;
  m->m32 *= s;
}

// FUNCTION: LEGOBATMAN 0x00685c80
void NuMtxPreScaleU(numtx_s *m, f32 s) {
  m->m00 *= s;
  m->m01 *= s;
  m->m02 *= s;
  m->m10 *= s;
  m->m11 *= s;
  m->m12 *= s;
  m->m20 *= s;
  m->m21 *= s;
  m->m22 *= s;
}

// FUNCTION: LEGOBATMAN 0x00685ce0
nuvec_s NuMtxGetScale(numtx_s *m) {
  nuvec_s scale;

  scale.x = NuFsqrt(m->m00 * m->m00 + m->m01 * m->m01 + m->m02 * m->m02);
  scale.y = NuFsqrt(m->m10 * m->m10 + m->m11 * m->m11 + m->m12 * m->m12);
  scale.z = NuFsqrt(m->m20 * m->m20 + m->m21 * m->m21 + m->m22 * m->m22);

  return scale;
}

// FUNCTION: LEGOBATMAN 0x00685d80
void NuMtxPreScale(numtx_s *m, nuvec_s *s) {
  m->m00 *= s->x;
  m->m01 *= s->x;
  m->m02 *= s->x;
  m->m10 *= s->y;
  m->m11 *= s->y;
  m->m12 *= s->y;
  m->m20 *= s->z;
  m->m21 *= s->z;
  m->m22 *= s->z;
}

// FUNCTION: LEGOBATMAN 0x00685de0
void NuMtxPreScaleX(numtx_s *m, f32 ScaleX) {
  m->m00 = m->m00 * ScaleX;
  m->m01 = m->m01 * ScaleX;
  m->m02 = m->m02 * ScaleX;
}

// saga body; x87 operand order differs in one or two rows.
// STUB: LEGOBATMAN 0x00685e40
void NuMtxRotateX(numtx_s *m, i32 a) {
  f32 cosx = NuSinApprox(a + 0x4000);
  f32 sinx = NuSinApprox(a);
  f32 m01 = m->m01;
  f32 m11 = m->m11;
  f32 m21 = m->m21;
  f32 m31 = m->m31;

  m->m01 = m01 * cosx - m->m02 * sinx;
  m->m02 = m01 * sinx + m->m02 * cosx;
  m->m11 = m11 * cosx - m->m12 * sinx;
  m->m12 = m11 * sinx + m->m12 * cosx;
  m->m21 = m21 * cosx - m->m22 * sinx;
  m->m22 = m21 * sinx + m->m22 * cosx;
  m->m31 = m31 * cosx - m->m32 * sinx;
  m->m32 = m31 * sinx + m->m32 * cosx;
}

// FUNCTION: LEGOBATMAN 0x00685f20
void NuMtxPreRotateX(numtx_s *m, i32 a) {
  f32 cosx = NuSinApprox(a + 0x4000);
  f32 sinx = NuSinApprox(a);
  f32 m10 = m->m10;
  f32 m11 = m->m11;
  f32 m12 = m->m12;

  m->m10 = cosx * m10 + m->m20 * sinx;
  m->m11 = cosx * m11 + m->m21 * sinx;
  m->m12 = cosx * m12 + m->m22 * sinx;
  m->m20 = m->m20 * cosx - sinx * m10;
  m->m21 = m->m21 * cosx - sinx * m11;
  m->m22 = m->m22 * cosx - sinx * m12;
}

// FUNCTION: LEGOBATMAN 0x006860a0
void NuMtxRotateY(numtx_s *m, i32 a) {
  f32 cosx = NuSinApprox(a + 0x4000);
  f32 sinx = NuSinApprox(a);
  f32 m00 = m->m00;
  f32 m10 = m->m10;
  f32 m20 = m->m20;
  f32 m30 = m->m30;

  m->m00 = m00 * cosx + m->m02 * sinx;
  m->m02 = m->m02 * cosx - m00 * sinx;
  m->m10 = m10 * cosx + m->m12 * sinx;
  m->m12 = m->m12 * cosx - m10 * sinx;
  m->m20 = m20 * cosx + m->m22 * sinx;
  m->m22 = m->m22 * cosx - m20 * sinx;
  m->m30 = m30 * cosx + m->m32 * sinx;
  m->m32 = m->m32 * cosx - m30 * sinx;
}

// saga body; x87 operand order differs in one or two rows.
// STUB: LEGOBATMAN 0x00686340
void NuMtxPreRotateY(numtx_s *m, i32 a) {
  f32 cosx = NuSinApprox(a + 0x4000);
  f32 sinx = NuSinApprox(a);
  f32 m00 = m->m00;
  f32 m01 = m->m01;
  f32 m02 = m->m02;

  m->m00 = cosx * m00 - m->m20 * sinx;
  m->m01 = cosx * m01 - m->m21 * sinx;
  m->m02 = cosx * m02 - m->m22 * sinx;
  m->m20 = sinx * m00 + m->m20 * cosx;
  m->m21 = sinx * m01 + m->m21 * cosx;
  m->m22 = sinx * m02 + m->m22 * cosx;
}

// saga body; x87 operand order differs in one or two rows.
// STUB: LEGOBATMAN 0x00686270
void NuMtxRotateZ(numtx_s *m, i32 a) {
  f32 cosx = NuSinApprox(a + 0x4000);
  f32 sinx = NuSinApprox(a);
  f32 m00 = m->m00;
  f32 m10 = m->m10;
  f32 m20 = m->m20;
  f32 m30 = m->m30;

  m->m00 = m00 * cosx - m->m01 * sinx;
  m->m01 = m00 * sinx + m->m01 * cosx;
  m->m10 = m10 * cosx - m->m11 * sinx;
  m->m11 = m10 * sinx + m->m11 * cosx;
  m->m20 = m20 * cosx - m->m21 * sinx;
  m->m21 = m20 * sinx + m->m21 * cosx;
  m->m30 = m30 * cosx - m->m31 * sinx;
  m->m31 = m30 * sinx + m->m31 * cosx;
}

// saga body; x87 operand order differs in one or two rows.
// STUB: LEGOBATMAN 0x00686180
void NuMtxPreRotateZ(numtx_s *m, i32 a) {
  f32 cosx = NuSinApprox(a + 0x4000);
  f32 sinx = NuSinApprox(a);
  f32 m00 = m->m00;
  f32 m01 = m->m01;
  f32 m02 = m->m02;

  m->m00 = cosx * m00 + m->m10 * sinx;
  m->m01 = cosx * m01 + m->m11 * sinx;
  m->m02 = cosx * m02 + m->m12 * sinx;
  m->m10 = m->m10 * cosx - sinx * m00;
  m->m11 = m->m11 * cosx - sinx * m01;
  m->m12 = m->m12 * cosx - sinx * m02;
}

// FUNCTION: LEGOBATMAN 0x00686230
void NuMtxPreRotateY180(numtx_s *m) {
  m->m00 = -m->m00;
  m->m01 = -m->m01;
  m->m02 = -m->m02;
  m->m20 = -m->m20;
  m->m21 = -m->m21;
  m->m22 = -m->m22;
}

// saga body; x87 operand order differs in one or two rows.
// STUB: LEGOBATMAN 0x00685fd0
void NuMtxPreRotateY180X(numtx_s *m, i32 a) {
  f32 cosx = NuSinApprox(a + 0x4000);
  f32 sinx = NuSinApprox(a);
  f32 m10 = m->m10;
  f32 m11 = m->m11;
  f32 m12 = m->m12;

  m->m00 = -m->m00;
  m->m01 = -m->m01;
  m->m02 = -m->m02;
  m->m10 = cosx * m10 - m->m20 * sinx;
  m->m11 = cosx * m11 - m->m21 * sinx;
  m->m12 = cosx * m12 - m->m22 * sinx;
  m->m20 = -(m->m20 * cosx) - sinx * m10;
  m->m21 = -(m->m21 * cosx) - sinx * m11;
  m->m22 = -(m->m22 * cosx) - sinx * m12;
}

// FUNCTION: LEGOBATMAN 0x00685e10
void NuMtxPreSkewYX(numtx_s *Mtx, f32 SkewVal) {
  Mtx->m00 = Mtx->m00 + Mtx->m10 * SkewVal;
  Mtx->m01 = Mtx->m01 + Mtx->m11 * SkewVal;
  Mtx->m02 = Mtx->m02 + Mtx->m12 * SkewVal;
}

// FUNCTION: LEGOBATMAN 0x00686b20
void NuMtxTransposeR(numtx_s *m, numtx_s *m0) {
  f32 t;

  t = m0->m01;
  m->m01 = m0->m10;
  m->m10 = t;
  t = m0->m02;
  m->m02 = m0->m20;
  m->m20 = t;
  t = m0->m12;
  m->m12 = m0->m21;
  m->m21 = t;
  m->m00 = m0->m00;
  m->m11 = m0->m11;
  m->m22 = m0->m22;
  m->m30 = m0->m30;
  m->m31 = m0->m31;
  m->m32 = m0->m32;
  m->m33 = m0->m33;
}

// FUNCTION: LEGOBATMAN 0x00686b90
void NuMtxTranspose(numtx_s *m, numtx_s *m0) {
  f32 t;

  t = m0->m01;
  m->m01 = m0->m10;
  m->m10 = t;
  t = m0->m02;
  m->m02 = m0->m20;
  m->m20 = t;
  t = m0->m03;
  m->m03 = m0->m30;
  m->m30 = t;
  t = m0->m12;
  m->m12 = m0->m21;
  m->m21 = t;
  t = m0->m13;
  m->m13 = m0->m31;
  m->m31 = t;
  t = m0->m23;
  m->m23 = m0->m32;
  m->m32 = t;
  m->m00 = m0->m00;
  m->m11 = m0->m11;
  m->m22 = m0->m22;
  m->m33 = m0->m33;
}

// FUNCTION: LEGOBATMAN 0x00686c30
void NuMtxInv(numtx_s *m, numtx_s *m0) {
  f32 t;

  f32 tx = -m0->m30;
  f32 ty = -m0->m31;
  f32 tz = -m0->m32;

  t = m0->m01;
  m->m01 = m0->m10;
  m->m10 = t;
  t = m0->m02;
  m->m02 = m0->m20;
  m->m20 = t;
  t = m0->m12;
  m->m12 = m0->m21;
  m->m21 = t;
  m->m00 = m0->m00;
  m->m11 = m0->m11;
  m->m22 = m0->m22;
  m->m30 = m->m00 * tx + m->m10 * ty + m->m20 * tz;
  m->m31 = m->m01 * tx + m->m11 * ty + m->m21 * tz;
  m->m32 = m->m02 * tx + m->m12 * ty + m->m22 * tz;
  m->m23 = 0.0f;
  m->m13 = m->m23;
  m->m03 = m->m13;
  m->m33 = 1.0f;
}

// FUNCTION: LEGOBATMAN 0x00686d10
void NuMtxInvR(numtx_s *m, numtx_s *m0) {
  f32 t;

  t = m0->m01;
  m->m01 = m0->m10;
  m->m10 = t;
  t = m0->m02;
  m->m02 = m0->m20;
  m->m20 = t;
  t = m0->m12;
  m->m12 = m0->m21;
  m->m21 = t;
  m->m00 = m0->m00;
  m->m11 = m0->m11;
  m->m22 = m0->m22;
  m->m23 = 0.0f;
  m->m13 = m->m23;
  m->m03 = m->m13;
  m->m32 = 0.0f;
  m->m31 = m->m32;
  m->m30 = m->m31;
  m->m33 = 1.0f;
}

// saga body; x87 product operand order differs (VC reorders per term).
// STUB: LEGOBATMAN 0x00686620
void NuMtxMul(numtx_s *m, numtx_s *m0, numtx_s *m1) {
  numtx_s gm;

  if ((m == m0) || (m == m1)) {
    gm.m00 = m1->m00 * m0->m00 + m1->m10 * m0->m01 + m1->m20 * m0->m02;
    gm.m01 = m1->m01 * m0->m00 + m1->m11 * m0->m01 + m1->m21 * m0->m02;
    gm.m02 = m1->m02 * m0->m00 + m1->m12 * m0->m01 + m1->m22 * m0->m02;
    gm.m03 = 0.0f;
    gm.m10 = m1->m00 * m0->m10 + m1->m10 * m0->m11 + m1->m20 * m0->m12;
    gm.m11 = m1->m01 * m0->m10 + m1->m11 * m0->m11 + m1->m21 * m0->m12;
    gm.m12 = m1->m02 * m0->m10 + m1->m12 * m0->m11 + m1->m22 * m0->m12;
    gm.m13 = 0.0f;
    gm.m20 = m1->m00 * m0->m20 + m1->m10 * m0->m21 + m1->m20 * m0->m22;
    gm.m21 = m1->m01 * m0->m20 + m1->m11 * m0->m21 + m1->m21 * m0->m22;
    gm.m22 = m1->m02 * m0->m20 + m1->m12 * m0->m21 + m1->m22 * m0->m22;
    gm.m23 = 0.0f;
    gm.m30 =
        m1->m00 * m0->m30 + m1->m10 * m0->m31 + m1->m20 * m0->m32 + m1->m30;
    gm.m31 =
        m1->m01 * m0->m30 + m1->m11 * m0->m31 + m1->m21 * m0->m32 + m1->m31;
    gm.m32 =
        m1->m02 * m0->m30 + m1->m12 * m0->m31 + m1->m22 * m0->m32 + m1->m32;
    gm.m33 = 1.0f;
    *m = gm;
  } else {
    m->m00 = m1->m00 * m0->m00 + m1->m10 * m0->m01 + m1->m20 * m0->m02;
    m->m01 = m1->m01 * m0->m00 + m1->m11 * m0->m01 + m1->m21 * m0->m02;
    m->m02 = m1->m02 * m0->m00 + m1->m12 * m0->m01 + m1->m22 * m0->m02;
    m->m03 = 0.0f;
    m->m10 = m1->m00 * m0->m10 + m1->m10 * m0->m11 + m1->m20 * m0->m12;
    m->m11 = m1->m01 * m0->m10 + m1->m11 * m0->m11 + m1->m21 * m0->m12;
    m->m12 = m1->m02 * m0->m10 + m1->m12 * m0->m11 + m1->m22 * m0->m12;
    m->m13 = 0.0f;
    m->m20 = m1->m00 * m0->m20 + m1->m10 * m0->m21 + m1->m20 * m0->m22;
    m->m21 = m1->m01 * m0->m20 + m1->m11 * m0->m21 + m1->m21 * m0->m22;
    m->m22 = m1->m02 * m0->m20 + m1->m12 * m0->m21 + m1->m22 * m0->m22;
    m->m23 = 0.0f;
    m->m30 =
        m1->m00 * m0->m30 + m1->m10 * m0->m31 + m1->m20 * m0->m32 + m1->m30;
    m->m31 =
        m1->m01 * m0->m30 + m1->m11 * m0->m31 + m1->m21 * m0->m32 + m1->m31;
    m->m32 =
        m1->m02 * m0->m30 + m1->m12 * m0->m31 + m1->m22 * m0->m32 + m1->m32;
    m->m33 = 1.0f;
  }
}

// saga body; x87 product operand order differs (VC reorders per term).
// STUB: LEGOBATMAN 0x006868f0
void NuMtxMulR(numtx_s *m, numtx_s *m0, numtx_s *m1) {
  numtx_s gm;

  if ((m == m0) || (m == m1)) {
    gm.m00 = m1->m00 * m0->m00 + m1->m10 * m0->m01 + m1->m20 * m0->m02;
    gm.m01 = m1->m01 * m0->m00 + m1->m11 * m0->m01 + m1->m21 * m0->m02;
    gm.m02 = m1->m02 * m0->m00 + m1->m12 * m0->m01 + m1->m22 * m0->m02;
    gm.m03 = 0.0;
    gm.m10 = m1->m00 * m0->m10 + m1->m10 * m0->m11 + m1->m20 * m0->m12;
    gm.m11 = m1->m01 * m0->m10 + m1->m11 * m0->m11 + m1->m21 * m0->m12;
    gm.m12 = m1->m02 * m0->m10 + m1->m12 * m0->m11 + m1->m22 * m0->m12;
    gm.m13 = 0.0;
    gm.m20 = m1->m00 * m0->m20 + m1->m10 * m0->m21 + m1->m20 * m0->m22;
    gm.m21 = m1->m01 * m0->m20 + m1->m11 * m0->m21 + m1->m21 * m0->m22;
    gm.m22 = m1->m02 * m0->m20 + m1->m12 * m0->m21 + m1->m22 * m0->m22;
    gm.m23 = 0.0;
    gm.m30 = gm.m31 = gm.m32 = 0.0f;
    gm.m33 = 1.0;
    *m = gm;
  } else {
    m->m00 = m1->m00 * m0->m00 + m1->m10 * m0->m01 + m1->m20 * m0->m02;
    m->m01 = m1->m01 * m0->m00 + m1->m11 * m0->m01 + m1->m21 * m0->m02;
    m->m02 = m1->m02 * m0->m00 + m1->m12 * m0->m01 + m1->m22 * m0->m02;
    m->m03 = 0.0;
    m->m10 = m1->m00 * m0->m10 + m1->m10 * m0->m11 + m1->m20 * m0->m12;
    m->m11 = m1->m01 * m0->m10 + m1->m11 * m0->m11 + m1->m21 * m0->m12;
    m->m12 = m1->m02 * m0->m10 + m1->m12 * m0->m11 + m1->m22 * m0->m12;
    m->m13 = 0.0;
    m->m20 = m1->m00 * m0->m20 + m1->m10 * m0->m21 + m1->m20 * m0->m22;
    m->m21 = m1->m01 * m0->m20 + m1->m11 * m0->m21 + m1->m21 * m0->m22;
    m->m22 = m1->m02 * m0->m20 + m1->m12 * m0->m21 + m1->m22 * m0->m22;
    m->m23 = 0.0;
    m->m30 = m->m31 = m->m32 = 0.0f;
    m->m33 = 1.0;
  }
}

// saga body; x87 product operand order differs (VC reorders per term).
// STUB: LEGOBATMAN 0x00686d80
void NuMtxInvRSS(numtx_s *inv, numtx_s *T) {
  numtx_s gm;

  f32 det = T->m00 * (T->m11 * T->m22 - T->m12 * T->m21) -
            T->m01 * (T->m10 * T->m22 - T->m12 * T->m20) +
            T->m02 * (T->m10 * T->m21 - T->m11 * T->m20);
  f32 invdet = det == 0.0f ? 0.0f : 1.0f / det;

  gm.m00 = (T->m11 * T->m22 - T->m12 * T->m21) * invdet;
  gm.m10 = (T->m10 * T->m22 - T->m12 * T->m20) * -invdet;
  gm.m20 = (T->m10 * T->m21 - T->m11 * T->m20) * invdet;
  gm.m01 = (T->m01 * T->m22 - T->m02 * T->m21) * -invdet;
  gm.m11 = (T->m00 * T->m22 - T->m02 * T->m20) * invdet;
  gm.m21 = (T->m00 * T->m21 - T->m01 * T->m20) * -invdet;
  gm.m02 = (T->m01 * T->m12 - T->m02 * T->m11) * invdet;
  gm.m12 = (T->m00 * T->m12 - T->m02 * T->m10) * -invdet;
  gm.m22 = (T->m00 * T->m11 - T->m01 * T->m10) * invdet;
  gm.m03 = 0.0f;
  gm.m13 = 0.0f;
  gm.m23 = 0.0f;
  gm.m33 = 1.0f;
  gm.m30 = 0.0f;
  gm.m31 = 0.0f;
  gm.m32 = 0.0f;
  *inv = gm;
}

void NuMtxAlignX(numtx_s *m, nuvec_s *v);
void NuMtxAlignY(numtx_s *m, nuvec_s *v);
void NuMtxAlignZ(numtx_s *m, nuvec_s *v);

// FUNCTION: LEGOBATMAN 0x00687d30
void NuMtxLookAtX(numtx_s *m, nuvec_s *pnt) {
  nuvec_s v;

  v.x = pnt->x - m->m30;
  v.y = pnt->y - m->m31;
  v.z = pnt->z - m->m32;

  NuVecNorm(&v, &v);
  NuMtxAlignX(m, &v);
}

// FUNCTION: LEGOBATMAN 0x00687d80
void NuMtxLookAtY(numtx_s *m, nuvec_s *pnt) {
  nuvec_s v;

  v.x = pnt->x - m->m30;
  v.y = pnt->y - m->m31;
  v.z = pnt->z - m->m32;

  NuVecNorm(&v, &v);
  NuMtxAlignY(m, &v);
}

// FUNCTION: LEGOBATMAN 0x00687dd0
void NuMtxLookAtZ(numtx_s *m, nuvec_s *pnt) {
  nuvec_s v;

  v.x = pnt->x - m->m30;
  v.y = pnt->y - m->m31;
  v.z = pnt->z - m->m32;

  NuVecNorm(&v, &v);
  NuMtxAlignZ(m, &v);
}

// FUNCTION: LEGOBATMAN 0x00687e20
void NuMtxInvLookAtX(numtx_s *m, nuvec_s *pnt) {
  nuvec_s v;

  v.x = m->m30 - pnt->x;
  v.y = m->m31 - pnt->y;
  v.z = m->m32 - pnt->z;

  NuVecNorm(&v, &v);
  NuMtxAlignX(m, &v);
}

// FUNCTION: LEGOBATMAN 0x00687e70
void NuMtxInvLookAtY(numtx_s *m, nuvec_s *pnt) {
  nuvec_s v;

  v.x = m->m30 - pnt->x;
  v.y = m->m31 - pnt->y;
  v.z = m->m32 - pnt->z;

  NuVecNorm(&v, &v);
  NuMtxAlignY(m, &v);
}

// FUNCTION: LEGOBATMAN 0x00687ec0
void NuMtxInvLookAtZ(numtx_s *m, nuvec_s *pnt) {
  nuvec_s v;

  v.x = m->m30 - pnt->x;
  v.y = m->m31 - pnt->y;
  v.z = m->m32 - pnt->z;

  NuVecNorm(&v, &v);
  NuMtxAlignZ(m, &v);
}

// FUNCTION: LEGOBATMAN 0x006881b0
void NuMtxGetXAxis(numtx_s *m, nuvec_s *x) {
  x->x = m->m00;
  x->y = m->m01;
  x->z = m->m02;
}

// FUNCTION: LEGOBATMAN 0x006881d0
void NuMtxGetYAxis(numtx_s *m, nuvec_s *y) {
  y->x = m->m10;
  y->y = m->m11;
  y->z = m->m12;
}

// FUNCTION: LEGOBATMAN 0x006881f0
void NuMtxGetZAxis(numtx_s *m, nuvec_s *z) {
  z->x = m->m20;
  z->y = m->m21;
  z->z = m->m22;
}

// FUNCTION: LEGOBATMAN 0x00688210
void NuMtxGetTranslation(numtx_s *m, nuvec_s *t) {
  t->x = m->m30;
  t->y = m->m31;
  t->z = m->m32;
}

// saga body (NuFdiv inline); one product loads its operands the other way.
// STUB: LEGOBATMAN 0x006876d0
void NuMtxAlignX(numtx_s *m, nuvec_s *v) {
  m->m00 = v->x;
  m->m01 = v->y;
  m->m02 = v->z;
  m->m20 = m->m01 * m->m12 - m->m02 * m->m11;
  m->m21 = m->m02 * m->m10 - m->m00 * m->m12;
  m->m22 = m->m00 * m->m11 - m->m01 * m->m10;

  f32 s = NuFsqrt(m->m20 * m->m20 + m->m21 * m->m21 + m->m22 * m->m22);
  s = NuFdiv(1.0f, s);

  m->m20 = m->m20 * s;
  m->m21 = m->m21 * s;
  m->m22 = m->m22 * s;
  m->m10 = m->m21 * m->m02 - m->m22 * m->m01;
  m->m11 = m->m22 * m->m00 - m->m20 * m->m02;
  m->m12 = m->m20 * m->m01 - m->m21 * m->m00;
}

// saga body (NuFdiv inline); one product loads its operands the other way.
// STUB: LEGOBATMAN 0x006877e0
void NuMtxAlignY(numtx_s *m, nuvec_s *v) {
  m->m10 = v->x;
  m->m11 = v->y;
  m->m12 = v->z;
  m->m00 = m->m11 * m->m22 - m->m12 * m->m21;
  m->m01 = m->m12 * m->m20 - m->m10 * m->m22;
  m->m02 = m->m10 * m->m21 - m->m11 * m->m20;

  f32 s = NuFsqrt(m->m00 * m->m00 + m->m01 * m->m01 + m->m02 * m->m02);
  s = NuFdiv(1.0f, s);

  m->m00 = m->m00 * s;
  m->m01 = m->m01 * s;
  m->m02 = m->m02 * s;
  m->m20 = m->m01 * m->m12 - m->m02 * m->m11;
  m->m21 = m->m02 * m->m10 - m->m00 * m->m12;
  m->m22 = m->m00 * m->m11 - m->m01 * m->m10;
}

// FUNCTION: LEGOBATMAN 0x00687f10
void NuMtxAddR(numtx_s *m, numtx_s *m0, numtx_s *m1) {
  m->m00 = m0->m00 + m1->m00;
  m->m01 = m0->m01 + m1->m01;
  m->m02 = m0->m02 + m1->m02;
  m->m03 = 0.0f;
  m->m10 = m0->m10 + m1->m10;
  m->m11 = m0->m11 + m1->m11;
  m->m12 = m0->m12 + m1->m12;
  m->m13 = 0.0f;
  m->m20 = m0->m20 + m1->m20;
  m->m21 = m0->m21 + m1->m21;
  m->m22 = m0->m22 + m1->m22;
  m->m23 = 0.0f;
  m->m30 = 0.0f;
  m->m31 = 0.0f;
  m->m32 = 0.0f;
  m->m33 = 1.0f;
}

// FUNCTION: LEGOBATMAN 0x00687f90
void NuMtxSubR(numtx_s *m, numtx_s *m0, numtx_s *m1) {
  m->m00 = m0->m00 - m1->m00;
  m->m01 = m0->m01 - m1->m01;
  m->m02 = m0->m02 - m1->m02;
  m->m03 = 0.0f;
  m->m10 = m0->m10 - m1->m10;
  m->m11 = m0->m11 - m1->m11;
  m->m12 = m0->m12 - m1->m12;
  m->m13 = 0.0f;
  m->m20 = m0->m20 - m1->m20;
  m->m21 = m0->m21 - m1->m21;
  m->m22 = m0->m22 - m1->m22;
  m->m23 = 0.0f;
  m->m30 = 0.0f;
  m->m31 = 0.0f;
  m->m32 = 0.0f;
  m->m33 = 1.0f;
}

// FUNCTION: LEGOBATMAN 0x00688010
void NuMtxSkewSymmetric(numtx_s *m, nuvec_s *v) {
  m->m00 = 0.0;
  m->m01 = -v->z;
  m->m02 = v->y;
  m->m03 = 0.0;
  m->m10 = v->z;
  m->m11 = 0.0;
  m->m12 = -v->x;
  m->m13 = 0.0;
  m->m20 = -v->y;
  m->m21 = v->x;
  m->m22 = 0.0;
  m->m23 = 0.0;
  m->m30 = 0.0;
  m->m31 = 0.0;
  m->m32 = 0.0;
  m->m33 = 1.0;
}

// FUNCTION: LEGOBATMAN 0x00688290
f32 NuMtxDet3(numtx_s *m) {
  return m->m00 * (m->m11 * m->m22 - m->m12 * m->m21) -
         m->m01 * (m->m10 * m->m22 - m->m12 * m->m20) +
         m->m02 * (m->m10 * m->m21 - m->m11 * m->m20);
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

// FUNCTION: LEGOBATMAN 0x0068b570
unsigned int NuRandIntSeeded(unsigned int *seed) {
  *seed = *seed * 0x19660d + 0x3c6ef35f;
  return *seed;
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

// FUNCTION: LEGOBATMAN 0x0068b650
float NuFloatRand(NURAND *rand) { return NuRand(rand) / 2.1474836e+09f; }

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

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_nuvec_unk(f32 *v, f32 a, i32 i) {
  v[2] = NuFabs(a);
  v[5] = NuFsign(a);
  v[1] = NuCosApprox(i);
  NuVec4Set(v, a, a, a, a);
}
