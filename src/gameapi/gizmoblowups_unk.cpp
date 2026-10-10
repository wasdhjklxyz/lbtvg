// gameapi/gizmoblowups_unk.cpp: placed by tools/new.py; file name unproven.

#include "../batman/leveldata_unk.h"
#include "../batman/worldinfo_unk.h"
#include "../nu2api/nu3d/nuspecial.h"
#include "../nu2api/nucore/nustring.h"
#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"
#include <string.h>

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x005d8680
static f32 NuSinApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x005d8720
static f32 NuCosApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x005d8730
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

struct GIZMOBLOWUP_s {
  u8 pad0[0x30];
  nuvec_s position; // 0x30
  u8 pad3c[0x40 - 0x3c];
  nuvec_s initial_position; // 0x40
  u8 pad4c[0x58 - 0x4c];
  nuvec_s f58; // 0x58
  u8 pad64[0xa0 - 0x64];
  u32 blown_up : 1;     // 0xa0
  u32 punched : 1;      // 0xa0 bit 1
  u32 bits2 : 12;       // 0xa0 bits 2-13
  u32 visible : 1;      // 0xa0 bit 14
  u32 b15 : 1;          // 0xa0 bit 15
  u32 b16 : 1;          // 0xa0 bit 16
  u32 bits17 : 2;       // 0xa0 bits 17-18
  u32 b19 : 1;          // 0xa0 bit 19
  u32 anim_playing : 1; // 0xa0 bit 20
  u32 b21 : 1;          // 0xa0 bit 21
  u32 b22 : 1;          // 0xa0 bit 22
  u32 activated : 1;    // 0xa0 bit 23
  u32 b24 : 1;          // 0xa0 bit 24
  u32 bits25 : 3;       // 0xa0 bits 25-27
  u32 secondary : 1;    // 0xa0 bit 28
  u32 b29 : 1;          // 0xa0 bit 29
  u32 bits30 : 2;       // 0xa0 bits 30-31
  u8 pada4[0xa8 - 0xa4];
  u16 sa8; // 0xa8
  u8 padaa[0xac - 0xaa];
  i32 score;                      // 0xac
  struct GIZMOBLOWUPTYPE_s *type; // 0xb0
  u8 padb4[0xc8 - 0xb4];
  f32 fc8; // 0xc8
  u8 padcc[0xfe - 0xcc];
  char name[0x10]; // 0xfe
  i16 platform_id; // 0x10e
  u8 pad110[0x119 - 0x110];
  i8 score_mult; // 0x119
  u8 b11a;       // 0x11a
  u8 b11b;       // 0x11b
  u8 b11c;       // 0x11c
  u8 b11d;       // 0x11d
  u8 pad11e[0x122 - 0x11e];
  u16 b122_0 : 1; // 0x122
  u16 b122_1 : 4; // 0x122 bits 1-4
  u16 b122_5 : 1; // 0x122 bit 5
  u8 pad124[0x128 - 0x124];
  f32 scale; // 0x128
};

typedef struct GIZMO_s {
  void *object;
} GIZMO;

// GLOBAL: LEGOBATMAN 0x009696f0
extern numtx_s numtx_identity;

void *GameBufferAlloc(variptr_u *buf, variptr_u *buf_end, i32 size);
void *GameBufferAllocProgressUnk005bbaf0(variptr_u *buf, variptr_u *buf_end,
                                         i32 size);
void AddGizmo(GIZMOSYS_s *gizmo_sys, i32 type_id, void *a, void *object);
void PlatOnOff(i32 id, i32 on);

// FUNCTION: LEGOBATMAN 0x005d8990
i32 InitGizmoBlowupTypes(WORLDINFO_s *world) {
  world->gizmo_blowup_types = NULL;
  world->gizmo_blowup_type_count = 0;
  if (world->current_level->max_gizmoblowuptypes == 0)
    return 0;
  world->gizmo_blowup_types = (GIZMOBLOWUPTYPE_s *)GameBufferAlloc(
      &world->buf104, &world->bufEnd108,
      (world->current_level->max_gizmoblowuptypes + 0x40) * 0x10c);
  return world->gizmo_blowup_types != NULL;
}

// FUNCTION: LEGOBATMAN 0x005d8a00
i32 InitGizmoBlowups(WORLDINFO_s *world) {
  world->gizmo_blowups = NULL;
  world->gizmo_blowup_count = 0;
  if (world->current_level->max_gizmoblowups == 0)
    return 0;
  world->buf104.addr = (world->buf104.addr + 0xf) & ~0xf;
  world->gizmo_blowups = (GIZMOBLOWUP_s *)GameBufferAlloc(
      &world->buf104, &world->bufEnd108,
      world->current_level->max_gizmoblowups * sizeof(GIZMOBLOWUP_s));
  return world->gizmo_blowups != NULL;
}

// FUNCTION: LEGOBATMAN 0x005d8a70
i32 InitGizmoBlowupsMtxBuffer(WORLDINFO_s *world) {
  world->gizmo_blowup_mtx_buffer = NULL;
  world->buf104.addr = (world->buf104.addr + 0x7f) & ~0x7f;
  world->gizmo_blowup_mtx_buffer =
      (numtx_s *)GameBufferAlloc(&world->buf104, &world->bufEnd108, 0x8000);
  numtx_s *mtx = world->gizmo_blowup_mtx_buffer;
  for (i32 i = 0x200; i != 0; i--)
    *mtx++ = numtx_identity;
  return world->gizmo_blowup_mtx_buffer != NULL;
}

// FUNCTION: LEGOBATMAN 0x005d8af0
void *gizmoblowup_reservebuffers(void *world_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  InitGizmoBlowupTypes(world);
  InitGizmoBlowups(world);
  InitGizmoBlowupsMtxBuffer(world);
  return world->gizmo_blowups;
}

// FUNCTION: LEGOBATMAN 0x005d8b20
i32 Blowup_GetMaxGizmos(void *world_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  return world != NULL ? world->current_level->max_gizmoblowups : 0;
}

// FUNCTION: LEGOBATMAN 0x005d8b40
void Blowup_AddGizmos(GIZMOSYS_s *gizmo_sys, i32 type_id, void *world_ptr,
                      void *unused) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  for (i32 index = 0; index < world->gizmo_blowup_count; index++) {
    if (NuStrLen(world->gizmo_blowups[index].name) != 0)
      AddGizmo(gizmo_sys, type_id, NULL, &world->gizmo_blowups[index]);
  }
}

// FUNCTION: LEGOBATMAN 0x005d8bb0
char *Blowup_GetGizmoName(GIZMO *gizmo) {
  return gizmo != NULL ? ((GIZMOBLOWUP_s *)gizmo->object)->name : NULL;
}

// FUNCTION: LEGOBATMAN 0x005d8bd0
i32 Blowup_GetOutput(GIZMO *gizmo, i32 output_index, i32 unused) {
  GIZMOBLOWUP_s *blowup = (GIZMOBLOWUP_s *)gizmo->object;
  switch (output_index) {
  case 0:
    if (blowup->blown_up)
      return 1;
    break;
  case 1:
    if (blowup->punched)
      return 1;
    break;
  case 2:
    if (blowup->secondary)
      return 1;
    break;
  case 3:
    if (blowup->b122_0)
      return 1;
    break;
  }
  return 0;
}

// GLOBAL: LEGOBATMAN 0x00961210
extern char *Blowup_OutputName[4];

// FUNCTION: LEGOBATMAN 0x005d8c40
char *Blowup_GetOutputName(GIZMO *gizmo, i32 output_index) {
  return output_index >= 0 && output_index < 4 ? Blowup_OutputName[output_index]
                                               : NULL;
}

// FUNCTION: LEGOBATMAN 0x005d8c60
i32 Blowup_GetNumOutputs(GIZMO *gizmo) { return 4; }

// FUNCTION: LEGOBATMAN 0x005d8c70
void GizmoBlowup_SetScale(GIZMOBLOWUP_s *blowup, f32 scale) {
  blowup->scale = scale;
}

// FUNCTION: LEGOBATMAN 0x005d8c80
void GizmoBlowup_SetVisibility(GIZMOBLOWUP_s *blowup, i32 visible) {
  blowup->visible = visible != 0;
  if (blowup->platform_id != -1)
    PlatOnOff(blowup->platform_id, visible);
}

// FUNCTION: LEGOBATMAN 0x005d8cd0
void Blowup_SetVisibility(GIZMO *gizmo, i32 visible) {
  if (gizmo != NULL)
    GizmoBlowup_SetVisibility((GIZMOBLOWUP_s *)gizmo->object, visible);
}

// FUNCTION: LEGOBATMAN 0x005d8d20
i32 Blowup_GetVisibility(GIZMO *gizmo) {
  if (gizmo != NULL && ((GIZMOBLOWUP_s *)gizmo->object)->visible)
    return 1;
  return 0;
}

// FUNCTION: LEGOBATMAN 0x005d8d40
nuvec_s *GizmoBlowup_GetPos(GIZMO *gizmo) {
  if (gizmo == NULL)
    return NULL;
  return &((GIZMOBLOWUP_s *)gizmo->object)->position;
}

// FUNCTION: LEGOBATMAN 0x005d8d50
void *Blowup_AllocateProgressData(variptr_u *buffer, variptr_u *buffer_end) {
  return GameBufferAllocProgressUnk005bbaf0(buffer, buffer_end, 0x180);
}

typedef struct BLOWUPPROGRESS_s {
  u32 blown_up[16];  // 0x00
  u32 activated[16]; // 0x40
  u32 visible[16];   // 0x80
  u32 secondary[16]; // 0xc0
  u32 b122[16];      // 0x100
  u32 f140[16];      // 0x140
} BLOWUPPROGRESS;

// FUNCTION: LEGOBATMAN 0x005d8d70
void Blowup_ClearProgress(void *world, void *progress_ptr) {
  BLOWUPPROGRESS *progress = (BLOWUPPROGRESS *)progress_ptr;
  if (progress != NULL) {
    memset(progress->blown_up, 0, sizeof(progress->blown_up));
    memset(progress->activated, -1, sizeof(progress->activated));
    memset(progress->visible, -1, sizeof(progress->visible));
    memset(progress->secondary, 0, sizeof(progress->secondary));
    memset(progress->b122, 0, sizeof(progress->b122));
    memset(progress->f140, -1, sizeof(progress->f140));
  }
}

// FUNCTION: LEGOBATMAN 0x005d8de0
void Blowup_StoreProgress(void *world_ptr, void *unused, void *progress_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  BLOWUPPROGRESS *progress = (BLOWUPPROGRESS *)progress_ptr;
  Blowup_ClearProgress(world, progress);
  if (progress == NULL || world == NULL || world->gizmo_blowups == NULL)
    return;
  GIZMOBLOWUP_s *blowup = world->gizmo_blowups;
  for (i32 i = 0; i < world->gizmo_blowup_count; i++, blowup++) {
    if (i >= 512)
      break;
    i32 word = i / 32;
    u32 bit = 1 << (i & 31);
    if (blowup->blown_up)
      progress->blown_up[word] |= bit;
    if (!blowup->visible)
      progress->visible[word] &= ~bit;
    if (!blowup->activated)
      progress->activated[word] &= ~bit;
    if (blowup->secondary)
      progress->secondary[word] |= bit;
    if (blowup->b122_0)
      progress->b122[word] |= bit;
    if (blowup->b122_5)
      progress->f140[word] |= bit;
  }
}

struct GIZMOBLOWUPTYPE_s {
  u8 pad0[0x30];
  nuhspecial_s animated_special; // 0x30
  u8 pad3c[0x102 - 0x3c];
  i16 sfx0; // 0x102
  i16 sfx1; // 0x104
  i8 b106;  // 0x106
  u8 pad107[0x10c - 0x107];
};

typedef struct NUINSTANIM_UNK_s {
  u8 pad0[0x50];
  u32 playing : 1;   // 0x50
  u32 bits1 : 2;     // 0x50 bits 1-2
  u32 repeating : 1; // 0x50 bit 3
} NUINSTANIM_UNK;

NUINSTANIM_UNK *NuSpecialGetInstAnim(nuhspecial_s *sp);
typedef struct UNK005EE1A0_s {
  u8 pad0[0x30];
  u64 flags; // 0x30
} UNK005EE1A0;

UNK005EE1A0 *Unk005ee1a0(i32 id);
void GameAudio_PlaySfxById(i32 sfx_id, nuvec_s *position, i32 a, i32 b);

// FUNCTION: LEGOBATMAN 0x005d8ed0
void Blowup_Reset(GIZMOBLOWUP_s *blowup) {
  blowup->scale = 1.0f;
  blowup->b122_5 = 1;
  blowup->blown_up = 0;
  blowup->b15 = 0;
  blowup->b24 = 0;
  blowup->b29 = 0;
  blowup->visible = 1;
  blowup->activated = 1;
  blowup->b11c = blowup->b11d;
  blowup->b11a = blowup->b11b;
  NUINSTANIM_UNK *anim = NuSpecialGetInstAnim(&blowup->type->animated_special);
  if (anim != NULL && anim->playing) {
    blowup->anim_playing = 1;
    if (anim->repeating) {
      blowup->b19 = 1;
      blowup->b22 = 1;
    }
  }
  blowup->position = blowup->initial_position;
  blowup->b16 = 1;
  if (blowup->type != NULL && blowup->type->b106 != -1) {
    blowup->sa8 = 0;
    UNK005EE1A0 *info = Unk005ee1a0(blowup->type->b106);
    if (info != NULL) {
      if (info->flags & 0x80000)
        blowup->sa8 = 1;
      else
        blowup->sa8 = 4;
    }
  } else {
    blowup->sa8 &= ~4;
  }
}

void AddLevelSfxFromId(i32 sfx_id, i32 *sfx_ids, i32 *sfx_count, i32 max_sfx);

// FUNCTION: LEGOBATMAN 0x005d9150
u32 GizmoBlowups_TotalScore(void *context) {
  WORLDINFO_s *world = (WORLDINFO_s *)context;
  GIZMOBLOWUP_s *blowup = world->gizmo_blowups;
  u32 total = 0;
  if (blowup != NULL) {
    for (i32 i = 0; i < world->gizmo_blowup_count; i++, blowup++)
      total += blowup->score_mult * blowup->score;
  }
  return total;
}

// FUNCTION: LEGOBATMAN 0x005d9190
void GizmoBlowups_AddLevelSfx(void *world_ptr, void *unused, i32 *sfx_ids,
                              i32 *sfx_count, i32 max_sfx) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  GIZMOBLOWUPTYPE_s *type = world->gizmo_blowup_types;
  for (i32 i = 0; i < world->gizmo_blowup_type_count; i++, type++) {
    if (type->sfx0 != -1)
      AddLevelSfxFromId(type->sfx0, sfx_ids, sfx_count, max_sfx);
    if (type->sfx1 != -1)
      AddLevelSfxFromId(type->sfx1, sfx_ids, sfx_count, max_sfx);
  }
}

// GLOBAL: LEGOBATMAN 0x00966624
extern i32 g_unk00966624; // default explosion debris type, -1 = none

i32 AddGameDebris(void *system, i32 type, nuvec_s *position);

// FUNCTION: LEGOBATMAN 0x005d9810
void GizmoBlowupAddDefaultExplosionDebris(nuvec_s *position) {
  if (g_unk00966624 != -1)
    AddGameDebris(g_unk00960894->p138, g_unk00966624, position);
}

// FUNCTION: LEGOBATMAN 0x005dda30
GIZMOBLOWUP_s *GizmoBlowUp_FindByName(WORLDINFO_s *world, char *name) {
  GIZMOBLOWUP_s *blowup = world->gizmo_blowups;
  if (blowup != 0) {
    for (i32 i = 0; i < world->gizmo_blowup_count; ++i, ++blowup) {
      if (NuStrICmp(blowup->name, name) == 0)
        return blowup;
    }
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x005dbd60
void Blowups_SetUnk525c(void *world_ptr) {
  ((WORLDINFO_s *)world_ptr)->f525c = 100.0f;
}

// FUNCTION: LEGOBATMAN 0x005dedf0
void Blowup_Activate(GIZMO *gizmo, i32 enabled) {
  if (gizmo == NULL)
    return;
  GIZMOBLOWUP_s *blowup = (GIZMOBLOWUP_s *)gizmo->object;
  if (!blowup->activated && enabled) {
    blowup->b122_0 = 0;
    blowup->secondary = 0;
  }
  blowup->activated = enabled != 0;
  blowup->b122_5 = enabled != 0;
  NUINSTANIM_UNK *anim = NuSpecialGetInstAnim(&blowup->type->animated_special);
  if (anim != NULL) {
    if (!enabled) {
      anim->playing = 0;
      return;
    }
    if (blowup->anim_playing)
      anim->playing = 1;
  }
  if (enabled) {
    Blowup_Reset(blowup);
    if (blowup->type->sfx1 != -1)
      GameAudio_PlaySfxById(blowup->type->sfx1, &blowup->f58, 0, 0);
  }
}

// FUNCTION: LEGOBATMAN 0x005deed0
i32 Blowup_ActivateRev(GIZMO *gizmo, i32 active, i32 flags) {
  if (gizmo == NULL || gizmo->object == NULL)
    return 1;
  GIZMOBLOWUP_s *blowup = (GIZMOBLOWUP_s *)gizmo->object;
  if (flags & 1) {
    if (blowup->activated)
      return active;
    return active == 0;
  }
  Blowup_Activate(gizmo, active == 0);
  return 1;
}

i32 GizBlowup_InitSingleTerrain(GIZMOBLOWUP_s *blowup);
void GizBlowup_InitTerrain();
void GizBlowup_DeleteSingleTerrain(GIZMOBLOWUP_s *blowup);
void GizBlowup_DeleteTerrain();

// FUNCTION: LEGOBATMAN 0x005def20
void Blowups_Reset(void *world_ptr, void *unused, void *progress_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  BLOWUPPROGRESS *progress = (BLOWUPPROGRESS *)progress_ptr;
  if (world == NULL)
    return;
  GIZMOBLOWUP_s *blowup = world->gizmo_blowups;
  if (blowup != NULL) {
    for (i32 i = 0; i < world->gizmo_blowup_count; i++, blowup++) {
      Blowup_Reset(blowup);
      if (progress != NULL && i < 512) {
        i32 word = i / 32;
        u32 bit = 1 << (i & 31);
        blowup->blown_up = (progress->blown_up[word] & bit) != 0;
        i32 was_visible = blowup->visible;
        blowup->visible = (progress->visible[word] & bit) != 0;
        if (!was_visible) {
          if (blowup->visible)
            GizBlowup_InitSingleTerrain(blowup);
        } else if (!blowup->visible) {
          GizBlowup_DeleteSingleTerrain(blowup);
        }
        blowup->activated = (progress->activated[word] & bit) != 0;
        blowup->secondary = (progress->secondary[word] & bit) != 0;
        blowup->b122_0 = (progress->b122[word] & bit) != 0;
        blowup->b122_5 = (progress->f140[word] & bit) != 0;
      }
      if (blowup->blown_up)
        blowup->fc8 = 0.0f;
    }
  }
  GizBlowup_DeleteTerrain();
  GizBlowup_InitTerrain();
}

typedef struct ADDGIZMOTYPE_s {
  char *name;        // 0x00
  char *prefix;      // 0x04
  u16 progress_size; // 0x08
  void *fns[0x1c];   // 0x0c
} ADDGIZMOTYPE;

// GLOBAL: LEGOBATMAN 0x00960118
extern ADDGIZMOTYPE Default_ADDGIZMOTYPE;
// GLOBAL: LEGOBATMAN 0x00961104
extern i32 blowup_gizmotype_id;

void GizmoBlowupEarlyUpdate(void *world_ptr, void *unused, f32 dt);
void GizmoBlowupLateUpdate(void *world_ptr, void *unused, f32 dt);
void GizmoBlowupBurstDraw(void *world_ptr, void *unused, f32 dt);
i32 gizmoblowup_Load(void *world_ptr, void *unused);

// FUNCTION: LEGOBATMAN 0x005df900
ADDGIZMOTYPE *NewBlowup_RegisterGizmo(i32 type_id) {
  // GLOBAL: LEGOBATMAN 0x009612d0
  static char *name = "blowup";
  // GLOBAL: LEGOBATMAN 0x00ac73c8
  static ADDGIZMOTYPE addtype;

  addtype = Default_ADDGIZMOTYPE;
  addtype.name = name;
  addtype.prefix = "";
  addtype.progress_size = sizeof(BLOWUPPROGRESS);
  addtype.fns[2] = (void *)GizmoBlowupEarlyUpdate;
  addtype.fns[5] = NULL;
  addtype.fns[13] = (void *)Blowup_GetVisibility;
  addtype.fns[0] = (void *)Blowup_GetMaxGizmos;
  addtype.fns[14] = (void *)GizmoBlowup_GetPos;
  addtype.fns[15] = NULL;
  addtype.fns[1] = (void *)Blowup_AddGizmos;
  addtype.fns[16] = NULL;
  addtype.fns[17] = NULL;
  addtype.fns[3] = (void *)GizmoBlowupLateUpdate;
  addtype.fns[18] = NULL;
  addtype.fns[4] = (void *)GizmoBlowupBurstDraw;
  addtype.fns[6] = (void *)Blowup_GetGizmoName;
  addtype.fns[7] = (void *)Blowup_GetOutput;
  addtype.fns[8] = (void *)Blowup_GetOutputName;
  addtype.fns[9] = (void *)Blowup_GetNumOutputs;
  addtype.fns[10] = (void *)Blowup_Activate;
  addtype.fns[11] = (void *)Blowup_ActivateRev;
  addtype.fns[12] = (void *)Blowup_SetVisibility;
  addtype.fns[19] = (void *)Blowup_AllocateProgressData;
  addtype.fns[20] = (void *)Blowup_ClearProgress;
  addtype.fns[21] = (void *)Blowup_StoreProgress;
  addtype.fns[22] = (void *)Blowups_Reset;
  addtype.fns[23] = (void *)gizmoblowup_reservebuffers;
  addtype.fns[24] = (void *)gizmoblowup_Load;
  addtype.fns[25] = (void *)Blowups_SetUnk525c;
  addtype.fns[26] = NULL;
  addtype.fns[27] = (void *)GizmoBlowups_AddLevelSfx;
  blowup_gizmotype_id = type_id;
  return &addtype;
}

i32 Techno_FindOperator(void *object, void **pad, GameObject_s **out);

// STUB: LEGOBATMAN 0x005dfa60
// close: orig keeps 0 in esi (push esi / lea eax,[esi+2] / mov eax,esi);
// this pushes 0 and uses immediates.
i32 ObjHitObj_Flags(GameObject_s *object) {
  i32 flags = 0;
  if (object != 0) {
    Techno_FindOperator(object, 0, &object);
    if (object != 0) {
      if ((object->flags1fc & 0x80) || object->f135c > 0.0f) {
        flags = 0x804;
      } else {
        flags |= 2;
      }
      if (object->flags1f8 & 0x10001)
        return flags | 0x10;
      if (object->flags1f8 & 4)
        return flags | 0x20;
      return flags | 8;
    }
  }
  return flags;
}

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_gizmoblowups_unk(f32 *v, f32 a, i32 i) {
  v[0] = NuSinApprox(i);
  v[1] = NuCosApprox(i);
  NuVec4Set(v, a, a, a, a);
}
