// gameapi/unk_00637650.cpp: TU of unknown name, found by its header-static
// copies (the functions after them are not matched yet).

#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x006375b0
static f32 NuSinApprox(i32 angle);

// FUNCTION: LEGOBATMAN 0x00637650
static f32 NuCosApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x00637660
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_00637650(f32 *v, f32 a, i32 i) {
  v[1] = NuCosApprox(i);
  NuVec4Set(v, a, a, a, a);
}

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_unk_00637650(f32 *v, f32 a, i32 i) {
  v[0] = NuSinApprox(i);
}
