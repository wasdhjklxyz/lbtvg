// gameapi/unk_00631d30.cpp: TU of unknown name, found by its header-static
// copies (the functions after them are not matched yet).

#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"

// FUNCTION: LEGOBATMAN 0x00631d30
static f32 NuFabs(f32 f);
// FUNCTION: LEGOBATMAN 0x00631d50
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_00631d30(f32 *v, f32 a, i32 i) {
  v[2] = NuFabs(a);
  NuVec4Set(v, a, a, a, a);
}
