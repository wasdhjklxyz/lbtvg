#pragma once
// Small header statics that every user TU gets its own out-of-line copy of:
// VC8 emits a referenced static even when every call to it was inlined, and
// the incremental link keeps the unreferenced COMDAT. Each copy is annotated
// in its own .cpp by re-declaring the prototype under the annotation (as for
// NuSinApprox in nutrig_unk.h). NuFabs/NuFdiv after saga nufloat.h; the other
// names are unproven (the Mac inlines them all). The vector helpers take raw
// float pointers so this header does not pull in the vector types.

#include "../nucore/common.h"

static f32 NuFabs(f32 f) {
  u32 bits = *(u32 *)&f & 0x7fffffff;
  return *(f32 *)&bits;
}

static f32 NuFdiv(f32 a, f32 b) {
  if (a == 0.0f || b == 0.0f)
    return 0.0f;
  return a / b;
}

// -1, 0 or 1 (the global NuFsign has no 0 case).
static f32 NuFsign(f32 f) {
  if (f > 0.0f)
    return 1.0f;
  if (f < 0.0f)
    return -1.0f;
  return 0.0f;
}

// 28 bytes, v in eax.
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w) {
  v[0] = x;
  v[1] = y;
  v[2] = z;
  v[3] = w;
}

// 23 bytes, dst in ecx, src in eax.
static void NuVec4Copy(f32 *dst, f32 *src) {
  dst[0] = src[0];
  dst[1] = src[1];
  dst[2] = src[2];
  dst[3] = src[3];
}

// 29 bytes, dst in ecx, src in eax.
static void NuVecScaleInline(f32 *dst, f32 *src, f32 s) {
  dst[0] = src[0] * s;
  dst[1] = src[1] * s;
  dst[2] = src[2] * s;
}
