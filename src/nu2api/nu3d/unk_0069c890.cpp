// nu2api/nu3d/unk_0069c890.cpp: TU of unknown name, found by its header-static
// copies (the functions after them are not matched yet).

#include "../numath/nuinline_unk.h"
#include "../numath/nutrig_unk.h"

// FUNCTION: LEGOBATMAN 0x0069c890
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);
// FUNCTION: LEGOBATMAN 0x0069ca30
static f32 NuSinApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x0069cad0
static f32 NuCosApprox(i32 angle);

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_0069c890(f32 *v, f32 a, i32 i) {
  NuVec4Set(v, a, a, a, a);
  v[0] = NuSinApprox(i);
  v[1] = NuCosApprox(i);
}
