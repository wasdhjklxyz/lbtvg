// nu2api/gamelib/unk_0067e270.cpp: TU of unknown name, found by its
// header-static copies (the functions after them are not matched yet).

#include "../numath/nuinline_unk.h"
#include "../numath/nutrig_unk.h"

// FUNCTION: LEGOBATMAN 0x0067e270
static f32 NuFabs(f32 f);
// FUNCTION: LEGOBATMAN 0x0067e290
static f32 NuFdiv(f32 a, f32 b);
// FUNCTION: LEGOBATMAN 0x0067e2d0
static f32 NuSinApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x0067e370
static f32 NuCosApprox(i32 angle);

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_0067e270(f32 *v, f32 a, i32 i) {
  v[2] = NuFabs(a);
  v[3] = NuFdiv(a, v[4]);
  v[0] = NuSinApprox(i);
  v[1] = NuCosApprox(i);
}
