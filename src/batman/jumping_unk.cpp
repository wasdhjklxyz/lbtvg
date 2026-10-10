// batman/jumping_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nu2api/nucore/common.h"
#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/numtx_inline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"
#include "worldinfo_unk.h"
#include <stddef.h>

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x004b8100
static void NuMtxRotateYInline(f32 *m, i32 a);

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x004b7b00
static f32 NuFabs(f32 f);
// FUNCTION: LEGOBATMAN 0x004b7b20
static f32 NuFsign(f32 f);
// FUNCTION: LEGOBATMAN 0x004b7d20
static f32 NuSinApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x004b7de0
static f32 NuCosApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x004b7e00
static void NuVec4Copy(f32 *dst, f32 *src);
// FUNCTION: LEGOBATMAN 0x004b7e20
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);
// FUNCTION: LEGOBATMAN 0x004b7e80
static void NuVecScaleInline(f32 *dst, f32 *src, f32 s);

char DefinedLocators_FindIX(char *name);

// FUNCTION: LEGOBATMAN 0x004c0cb0
void StartBallooning(GameObject_s *object, i32 movement_state) {
  object->b9db = 0x5d;
  object->b9d9 = movement_state;
  if (object->p50->p0c->i2c8 != 0) {
    object->s9d0 = 0xb2;
  } else {
    object->s9d0 = object->s162c;
  }
  object->f988 = 1000000000.0f;
  object->b9da = DefinedLocators_FindIX("right_backpack");
  object->b9da = object->p54->p24->b1dc[object->b9da];
  object->f998 = 1.0f;
  object->b9d9 = 1;
  if (g_unk00960894->p2b04->bd7e) {
    object->s9d2 = 0xd7;
  } else if (g_unk00960894->p2b04->bd8e) {
    object->s9d2 = 0xd8;
  } else {
    object->s9d2 = g_unk00960894->p2b04->bd9e ? 0xd9 : -1;
  }
}

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_jumping_unk(f32 *v, f32 a, i32 i) {
  v[2] = NuFabs(a);
  v[5] = NuFsign(a);
  v[0] = NuSinApprox(i);
  v[1] = NuCosApprox(i);
  NuVec4Copy(v + 4, v);
  NuVec4Set(v, a, a, a, a);
  NuVecScaleInline(v + 8, v, a);
}

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_2_jumping_unk(f32 *v, f32 a, i32 i) {
  NuMtxRotateYInline(v + 16, i);
}
