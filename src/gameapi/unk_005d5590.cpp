// gameapi/unk_005d5590.cpp: TU of unknown name, found by its header-static
// copies (the functions after them are not matched yet).

#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x005d5640
static void NuVecZeroInline(f32 *v);

// FUNCTION: LEGOBATMAN 0x005d5590
static f32 NuSinApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x005d5630
static f32 NuCosApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x005d5650
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_005d5590(f32 *v, f32 a, i32 i) {
  v[0] = NuSinApprox(i);
  v[1] = NuCosApprox(i);
  NuVec4Set(v, a, a, a, a);
}

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_unk_005d5590(f32 *v, f32 a, i32 i) {
  NuVecZeroInline(v + 80);
}
