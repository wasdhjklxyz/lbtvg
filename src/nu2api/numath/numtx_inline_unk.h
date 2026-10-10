#pragma once
// Matrix header statics that user TUs get their own copies of (see
// nuinline_unk.h). NuMtxRotateYInline after saga numtx_inline.h (angle in
// esi, matrix in edx); NuMtxCopyInline's name is unproven (dst in ecx, src in
// eax). Raw float pointers: m[0..15] is m00..m33.

#include "nutrig_unk.h"

static void NuMtxRotateYInline(f32 *m, i32 a) {
  f32 cosx = NuCosApprox(a);
  f32 sinx = NuSinApprox(a);
  f32 m00 = m[0];
  f32 m10 = m[4];
  f32 m20 = m[8];
  f32 m30 = m[12];

  m[0] = m00 * cosx + m[2] * sinx;
  m[2] = m[2] * cosx - m00 * sinx;
  m[4] = m10 * cosx + m[6] * sinx;
  m[6] = m[6] * cosx - m10 * sinx;
  m[8] = m20 * cosx + m[10] * sinx;
  m[10] = m[10] * cosx - m20 * sinx;
  m[12] = m30 * cosx + m[14] * sinx;
  m[14] = m[14] * cosx - m30 * sinx;
}

static void NuMtxCopyInline(f32 *dst, f32 *src) {
  dst[0] = src[0];
  dst[1] = src[1];
  dst[2] = src[2];
  dst[3] = src[3];
  dst[4] = src[4];
  dst[5] = src[5];
  dst[6] = src[6];
  dst[7] = src[7];
  dst[8] = src[8];
  dst[9] = src[9];
  dst[10] = src[10];
  dst[11] = src[11];
  dst[12] = src[12];
  dst[13] = src[13];
  dst[14] = src[14];
  dst[15] = src[15];
}

extern struct numtx_s numtx_identity;

// saga NuMtxSetRotationYInline; rows 1 and 3 come from numtx_identity.
static void NuMtxSetRotationYInline(f32 *m, i32 a) {
  f32 *id = (f32 *)&numtx_identity;
  f32 c = NuCosApprox(a);
  f32 s = NuSinApprox(a);
  m[0] = c;
  m[1] = 0.0f;
  m[2] = -s;
  m[3] = 0.0f;
  m[4] = id[4];
  m[5] = id[5];
  m[6] = id[6];
  m[7] = id[7];
  m[8] = s;
  m[9] = 0.0f;
  m[11] = 0.0f;
  m[10] = c;
  m[12] = id[12];
  m[13] = id[13];
  m[14] = id[14];
  m[15] = id[15];
}
