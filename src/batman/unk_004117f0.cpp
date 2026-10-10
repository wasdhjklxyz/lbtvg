// batman/unk_004117f0.cpp: TU of unknown name, found by its header-static
// copies (the functions after them are not matched yet).

#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"

// FUNCTION: LEGOBATMAN 0x004117f0
static f32 NuFabs(f32 f);
// FUNCTION: LEGOBATMAN 0x00411810
static f32 NuFdiv(f32 a, f32 b);
// FUNCTION: LEGOBATMAN 0x00411860
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_004117f0(f32 *v, f32 a, i32 i) {
  v[2] = NuFabs(a);
  v[3] = NuFdiv(a, v[4]);
  NuVec4Set(v, a, a, a, a);
}
