// gameapi/gizflock_unk.cpp: flock_extras config parser, between rtleditor.cpp
// and listman_gen.cpp; Mac puts it in the GizFlock file. File name unproven.

#include "../nu2api/nucore/common.h"
#include "../nu2api/numath/nuinline_unk.h"

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x00665780
static f32 NuFabs(f32 f);
// FUNCTION: LEGOBATMAN 0x00665a40
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);
// FUNCTION: LEGOBATMAN 0x00665ad0
static void NuVecScaleInline(f32 *dst, f32 *src, f32 s);

typedef struct nufpar_s NUFPAR;

f32 NuFParGetFloat(NUFPAR *parser);

typedef struct GIZFLOCKEXTRA_s {
  u32 pad0;
  f32 anim_speed; // 0x04
} GIZFLOCKEXTRA;

// GLOBAL: LEGOBATMAN 0x00ad258c
extern GIZFLOCKEXTRA *g_unk00ad258c;

// keyword "anim_speed" in table 0x00967db8
// FUNCTION: LEGOBATMAN 0x00665f60
void GizFlock_Config_anim_speed(NUFPAR *parser) {
  g_unk00ad258c->anim_speed = NuFParGetFloat(parser);
}

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_gizflock_unk(f32 *v, f32 a, i32 i) {
  v[2] = NuFabs(a);
  NuVec4Set(v, a, a, a, a);
  NuVecScaleInline(v + 8, v, a);
}
