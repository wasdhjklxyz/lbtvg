// batman/unk_004fbc90.cpp: TU of unknown name, found by its header-static
// copies (the functions after them are not matched yet).

#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/numtx_inline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x004fc060
static void NuMtxCopyInline(f32 *dst, f32 *src);
// FUNCTION: LEGOBATMAN 0x004fc1a0
static void NuMtxRotateYInline(f32 *m, i32 a);

// FUNCTION: LEGOBATMAN 0x004fbc90
static f32 NuFabs(f32 f);
// FUNCTION: LEGOBATMAN 0x004fbcb0
static f32 NuFdiv(f32 a, f32 b);
// FUNCTION: LEGOBATMAN 0x004fbd00
static f32 NuFsign(f32 f);
// FUNCTION: LEGOBATMAN 0x004fbf00
static f32 NuSinApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x004fbfc0
static f32 NuCosApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x004fc010
static void NuVec4Copy(f32 *dst, f32 *src);
// FUNCTION: LEGOBATMAN 0x004fc030
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_004fbc90(f32 *v, f32 a, i32 i) {
  v[2] = NuFabs(a);
  v[3] = NuFdiv(a, v[4]);
  v[5] = NuFsign(a);
  v[0] = NuSinApprox(i);
  v[1] = NuCosApprox(i);
  NuVec4Copy(v + 4, v);
  NuVec4Set(v, a, a, a, a);
}

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_unk_004fbc90(f32 *v, f32 a, i32 i) {
  NuMtxCopyInline(v + 32, v + 16);
  NuMtxRotateYInline(v + 16, i);
}
