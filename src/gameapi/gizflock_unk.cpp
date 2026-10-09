// gameapi/gizflock_unk.cpp: flock_extras config parser, between rtleditor.cpp
// and listman_gen.cpp; Mac puts it in the GizFlock file. File name unproven.

#include "../nu2api/nucore/common.h"

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
