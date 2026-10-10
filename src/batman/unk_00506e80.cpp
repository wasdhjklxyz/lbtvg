// batman/unk_00506e80.cpp: TU of unknown name, found by its header-static
// copies (the functions after them are not matched yet).

#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"

// FUNCTION: LEGOBATMAN 0x00506e80
static f32 NuFabs(f32 f);
// FUNCTION: LEGOBATMAN 0x00506ea0
static f32 NuFsign(f32 f);
// FUNCTION: LEGOBATMAN 0x005070b0
static void NuVec4Copy(f32 *dst, f32 *src);
// FUNCTION: LEGOBATMAN 0x005070d0
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_00506e80(f32 *v, f32 a, i32 i) {
  v[2] = NuFabs(a);
  v[5] = NuFsign(a);
  NuVec4Copy(v + 4, v);
  NuVec4Set(v, a, a, a, a);
}
