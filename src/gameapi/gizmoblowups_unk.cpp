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
  nuvec_s f58;          // 0x58
  nuvec_s bounds_min;   // 0x64
  nuvec_s bounds_max;   // 0x70
  nuvec_s decal_offset; // 0x7c
  u8 pad88[0x94 - 0x88];
  nuvec_s shadow_offset; // 0x94
  union {
    struct {
      u32 blown_up : 1;     // 0xa0
      u32 punched : 1;      // 0xa0 bit 1
      u32 bits2 : 12;       // 0xa0 bits 2-13
      u32 visible : 1;      // 0xa0 bit 14
      u32 b15 : 1;          // 0xa0 bit 15
      u32 b16 : 1;          // 0xa0 bit 16
      u32 b17 : 1;          // 0xa0 bit 17
      u32 b18 : 1;          // 0xa0 bit 18
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
    };
    u32 flags; // 0xa0
  };
  u32 draw_flags; // 0xa4
  u16 sa8;        // 0xa8
  u8 padaa[0xac - 0xaa];
  i32 score;                      // 0xac
  struct GIZMOBLOWUPTYPE_s *type; // 0xb0
  f32 fb4;                        // 0xb4
  u8 padb8[0xbc - 0xb8];
  f32 fbc; // 0xbc
  f32 fc0; // 0xc0
  u8 padc4[0xc8 - 0xc4];
  f32 fc8;         // 0xc8
  f32 decal_scale; // 0xcc
  u8 padd0[0xd8 - 0xd0];
  f32 respawn_timer; // 0xd8
  u8 paddc[0xe4 - 0xdc];
  void *antinode;   // 0xe4
  u16 decal_rot_x;  // 0xe8
  u16 decal_rot_y;  // 0xea
  u16 decal_rot_z;  // 0xec
  u16 shadow_rot_x; // 0xee
  u16 shadow_rot_y; // 0xf0
  u16 shadow_rot_z; // 0xf2
  u16 rot_x;        // 0xf4
  u16 rot_y;        // 0xf6
  u16 rot_z;        // 0xf8
  u8 padfa[0xfe - 0xfa];
  char name[0x10];  // 0xfe
  i16 platform_id;  // 0x10e
  i16 platform_id2; // 0x110
  u8 pad112[0x119 - 0x112];
  i8 score_mult; // 0x119
  u8 b11a;       // 0x11a
  u8 b11b;       // 0x11b
  u8 b11c;       // 0x11c
  u8 b11d;       // 0x11d
  u8 pad11e[0x122 - 0x11e];
  union {
    struct {
      u16 b122_0 : 1; // 0x122
      u16 b122_1 : 4; // 0x122 bits 1-4
      u16 b122_5 : 1; // 0x122 bit 5
      u16 b122_6 : 1; // 0x122 bit 6
      u16 b122_7 : 1; // 0x122 bit 7
    };
    u16 b122; // 0x122
  };
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
  nuhspecial_s decal_special;    // 0x3c
  u8 pad48[0x54 - 0x48];
  nuhspecial_s burst_special; // 0x54
  u8 pad60[0x7c - 0x60];
  u32 animation_flags;       // 0x7c
  i32 effect_flags;          // 0x80
  f32 f84;                   // 0x84
  f32 animation_start_frame; // 0x88
  f32 animation_end_frame;   // 0x8c
  f32 animation_base_frame;  // 0x90
  u8 pad94[0x9c - 0x94];
  f32 f9c; // 0x9c
  u8 pada0[0xa4 - 0xa0];
  i16 particle_types[9]; // 0xa4
  u8 padb6[0xb8 - 0xb6];
  u8 anti_node_data[0xdc - 0xb8]; // 0xb8
  char name[0x20];                // 0xdc
  u8 padfc[0xff - 0xfc];
  u8 bff;  // 0xff
  u8 b100; // 0x100
  u8 pad101[0x102 - 0x101];
  i16 sfx0; // 0x102
  i16 sfx1; // 0x104
  i8 b106;  // 0x106
  u8 pad107[0x10c - 0x107];
};

typedef struct NUINSTANIM_UNK_s {
  u8 pad0[0x40];
  f32 tfactor; // 0x40
  u8 pad44[0x50 - 0x44];
  u32 playing : 1;   // 0x50
  u32 bits1 : 2;     // 0x50 bits 1-2
  u32 repeating : 1; // 0x50 bit 3
  f32 fparam1;       // 0x54
  u8 pad58[0x5c - 0x58];
  u16 anim_ix; // 0x5c
} NUINSTANIM_UNK;

NUINSTANIM_UNK *NuSpecialGetInstAnim(nuhspecial_s *sp);
void GameAudio_PlaySfxById(i32 sfx_id, nuvec_s *position, i32 a, i32 b);

typedef struct UNK005EE1A0_s {
  u8 pad0[0x30];
  u64 flags; // 0x30
  u8 pad38[0xec - 0x38];
  i16 blowup_sfx; // 0xec
} UNK005EE1A0;

UNK005EE1A0 *Unk005ee1a0(i32 id);
void GameAudio_PlaySfxById(i32 sfx_id, nuvec_s *position, i32 a, i32 b);
void GameAudio_PlaySfx(i32 sfx, nuvec_s *position, i32 flags, i32 volume);

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

// GLOBAL: LEGOBATMAN 0x00aca578
extern i32 BonusArea;
// GLOBAL: LEGOBATMAN 0x00aca574
extern i32 VehicleArea;

extern "C" void NuSpecialSetVisibility(nuhspecial_s *sp, i32 visible);
int NuSpecialExistsFn(nuhspecial_s *sp);
void AddFiniteShotDebrisEffect(i32 *handle, i32 effect, nuvec_s *position,
                               i32 count);

// FUNCTION: LEGOBATMAN 0x005d8fd0
void GizBlowup_Respawn(GIZMOBLOWUP_s *blowup) {
  if (blowup == NULL)
    return;
  Blowup_Reset(blowup);
  if (BonusArea != 0 && VehicleArea != 0 && (blowup->draw_flags & 0x1000000)) {
    if (NuSpecialExistsFn(&blowup->type->animated_special))
      NuSpecialSetVisibility(&blowup->type->animated_special, 0);
  } else if (blowup->visible) {
    NuSpecialSetVisibility(&blowup->type->animated_special, 1);
  }
  if (NuSpecialExistsFn(&blowup->type->decal_special))
    NuSpecialSetVisibility(&blowup->type->decal_special, 0);
  blowup->fc8 = 0.0f;
  blowup->flags &= 0xfdf1e401;
  blowup->b11a = blowup->b11b;
  blowup->b11c = blowup->b11d;
  NUINSTANIM_UNK *anim = NuSpecialGetInstAnim(&blowup->type->animated_special);
  if (anim != NULL) {
    if (BonusArea != 0 && VehicleArea != 0 &&
        (blowup->draw_flags & 0x1000000) && blowup->anim_playing)
      anim->playing = 1;
    if (anim->playing && anim->repeating) {
      blowup->b19 = 1;
      blowup->b22 = 1;
    }
  }
  if (blowup->platform_id != -1)
    PlatOnOff(blowup->platform_id, 1);
  for (i32 i = 2; i < 4; i++) {
    if (blowup->type->particle_types[i] != -1) {
      i32 handle = -1;
      AddFiniteShotDebrisEffect(&handle, blowup->type->particle_types[i],
                                &blowup->f58, 1);
    }
  }
}

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

i32 NuSpecialCompare(nuhspecial_s *a, nuhspecial_s *b);
char *NuSpecialGetName(nuhspecial_s *sp);
f32 NuAnimEndFrameOld(void *anim);
void GameAntiNodeData_Init(void *data, nuhspecial_s *special);

// STUB: LEGOBATMAN 0x005d9210
// stores right; the original reloads `special` each loop pass, keeps -1 in
// ebx for every store and shares the `return NULL` epilogue.
GIZMOBLOWUPTYPE_s *GizmoBlowupTypeAdd(WORLDINFO_s *world, nuhspecial_s *special,
                                      i32 flags, i32 *result) {
  for (i32 i = 0; i < world->gizmo_blowup_type_count; i++) {
    if (NuSpecialCompare(&world->gizmo_blowup_types[i].animated_special,
                         special) != 0 &&
        ((world->gizmo_blowup_types[i].animation_flags & 0x2000) ||
         flags != 0)) {
      *result = 0;
      return NULL;
    }
  }
  if (world->gizmo_blowup_type_count >=
      world->current_level->max_gizmoblowuptypes)
    return NULL;
  GIZMOBLOWUPTYPE_s *type =
      &world->gizmo_blowup_types[world->gizmo_blowup_type_count];
  if (special != NULL && NuSpecialExistsFn(special)) {
    type->animated_special = *special;
    NuStrCpy(type->name, NuSpecialGetName(&type->animated_special));
  }
  memset(type->particle_types, 0xff, 7 * sizeof(i16));
  type->effect_flags = 100;
  type->bff = 1;
  type->animation_flags = 0xc000;
  type->b106 = -1;
  if (flags != 0)
    type->animation_flags = 0xe000;
  type->animation_start_frame = 1.0f;
  NUINSTANIM_UNK *anim = NuSpecialGetInstAnim(&type->animated_special);
  if (anim != NULL)
    type->animation_end_frame = NuAnimEndFrameOld((
        *(void ***)((u8 *)type->animated_special.scene + 0x54))[anim->anim_ix]);
  else
    type->animation_end_frame = 1.0f;
  type->animation_base_frame = 1.0f;
  type->f9c = 0.0f;
  type->sfx0 = -1;
  type->f84 = 1.0f;
  type->sfx1 = -1;
  GameAntiNodeData_Init(type->anti_node_data, special);
  world->gizmo_blowup_type_count++;
  return type;
}

// STUB: LEGOBATMAN 0x005d93c0
// the original keeps `moved` in ebp and spills `last` to the world slot; ours
// the other way round (decl order, while, end-- forms tried).
void GizmoBlowupTypeRemove(GIZMOBLOWUPTYPE_s *type, WORLDINFO_s *world) {
  if (world == NULL || world->gizmo_blowup_type_count <= 0)
    return;
  if (type == NULL)
    return;
  GIZMOBLOWUPTYPE_s *types = world->gizmo_blowup_types;
  if (type < types)
    return;
  GIZMOBLOWUPTYPE_s *end = types + world->current_level->max_gizmoblowuptypes;
  if (type > end)
    return;
  GIZMOBLOWUPTYPE_s *last = end - 1;
  if (type < last) {
    for (GIZMOBLOWUPTYPE_s *moved = type + 1; moved <= last; moved++) {
      for (i32 i = 0; i < world->gizmo_blowup_count; i++) {
        GIZMOBLOWUP_s *blowup = &world->gizmo_blowups[i];
        if (blowup->type == moved)
          blowup->type--;
      }
    }
  }
  while (type !=
         &world->gizmo_blowup_types[world->gizmo_blowup_type_count - 1]) {
    GIZMOBLOWUPTYPE_s *next = type + 1;
    *type = *next;
    type = next;
  }
  memset(type, 0, sizeof(*type));
  world->gizmo_blowup_type_count--;
}

// GLOBAL: LEGOBATMAN 0x009ca98c
extern f32 g_unk009ca98c;

f32 GameShadow(GameObject_s *object, nuvec_s *position, f32 probe_height,
               i32 flags);

// FUNCTION: LEGOBATMAN 0x005d94f0
f32 GizmoBlowup_SetAutoSetReflectY(GIZMOBLOWUP_s *blowup, nuvec_s *position) {
  if (blowup == NULL)
    return 0.0f;
  if (blowup->platform_id)
    PlatOnOff(blowup->platform_id, 0);
  f32 ground = GameShadow(NULL, position, g_unk009ca98c, -1);
  if (blowup->platform_id)
    PlatOnOff(blowup->platform_id, 1);
  if (ground != 2000000.0)
    return ground - 0.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x005d9750
void BlowupObjEmit_Stop(void *part) {}

// FUNCTION: LEGOBATMAN 0x005d9760
void GizmoBlowUp_PlaySfx(GIZMOBLOWUP_s *blowup, nuvec_s *position) {
  if (blowup == NULL || blowup->type == NULL || position == NULL)
    return;
  if (blowup->type->b106 != -1) {
    UNK005EE1A0 *info = Unk005ee1a0(blowup->type->b106);
    if (info != NULL && info->blowup_sfx != -1) {
      GameAudio_PlaySfxById(info->blowup_sfx, position, 0, 0);
      return;
    }
  }
  if (blowup->type->sfx0 != -1)
    GameAudio_PlaySfxById(blowup->type->sfx0, position, 0, 0);
  else if (blowup->draw_flags & 0x40)
    GameAudio_PlaySfx(0xc6, position, 0, 0);
  else
    GameAudio_PlaySfx(0x46, position, 0, 0);
}

// FUNCTION: LEGOBATMAN 0x005d9810
void GizmoBlowupAddDefaultExplosionDebris(nuvec_s *position) {
  if (g_unk00966624 != -1)
    AddGameDebris(g_unk00960894->p138, g_unk00966624, position);
}

// FUNCTION: LEGOBATMAN 0x005d9df0
void Blowup_ResetAtMtx(GIZMOBLOWUP_s *blowup, numtx_s *mtx) {
  if (blowup != NULL && mtx != NULL) {
    Blowup_Reset(blowup);
    *(numtx_s *)blowup = *mtx;
  }
}

// GLOBAL: LEGOBATMAN 0x00a95fe0
extern f32 FRAMETIME;
// GLOBAL: LEGOBATMAN 0x00a9602c
extern u32 PickupFlickerFrame;
// GLOBAL: LEGOBATMAN 0x00ad10a4
extern i32 g_unk00ad10a4;
// GLOBAL: LEGOBATMAN 0x00aca000
extern void (*g_unk00aca000)(i32, i32, GIZMOBLOWUP_s *, UNK005EE1A0 *, f32, i32,
                             i32);

typedef struct BLOWUPPLUG_s {
  u8 pad0[0x28];
  i32 blowup_ix; // 0x28
  u8 pad2c[0x44 - 0x2c];
} BLOWUPPLUG;

typedef struct BLOWUPPLUGSYS_s {
  BLOWUPPLUG *plugs; // 0x00
  i32 count;         // 0x04
} BLOWUPPLUGSYS;

void PushAway(nuvec_s *pos, f32 radius, nuvec_s *min, nuvec_s *max, void *a,
              void *b, void *c, f32 strength, u32 flags);
void UpdateMidPos(GIZMOBLOWUP_s *blowup);
void GizmoBlowupEvalAnim(GIZMOBLOWUP_s *blowup, GIZMOBLOWUPTYPE_s *type,
                         f32 base_frame, numtx_s *m, NUINSTANIM_UNK *anim,
                         f32 end_frame);
numtx_s *NuSpecialGetInstanceMtx(nuhspecial_s *special);
i32 GizmoBlowupBlowup(GIZMOBLOWUP_s *blowup, i32 effects, i32 hit_type,
                      i32 damage, GameObject_s *object, i32 hit_context);
extern "C" void AddVariableShotDebrisEffectTimed1(i32 effect, nuvec_s *pos,
                                                  i32 count, f32 time, i16 rz,
                                                  i16 ry, numtx_s *m);
i32 Unk00558ee0(i32 sfx);
void *GameAntiNode_Add(void *sys, nuvec_s *pos, u16 rot, void *data, f32 f,
                       i32 flag);
void GameAntiNode_Update(void *sys, void *node, nuvec_s *pos, u16 rot,
                         void *data, f32 f, i32 flag);
void GameAntiNode_Remove(void *sys, void *node);

// FUNCTION: LEGOBATMAN 0x005da880
void GizmoBlowupEarlyUpdate(void *world_ptr, void *unused, f32 dt) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  if (world == NULL)
    return;
  GIZMOBLOWUP_s *blowup = world->gizmo_blowups;
  if (blowup == NULL)
    return;
  for (i32 i = 0; i < world->gizmo_blowup_count; i++, blowup++) {
    i32 updated = 0;
    if (blowup->blown_up && (blowup->flags & 0x30000000) == 0 &&
        blowup->b122_5 &&
        (blowup->respawn_timer > 0.0f ||
         (g_unk00ad10a4 == 0 && (blowup->sa8 & 5)))) {
      if (blowup->respawn_timer > 0.0f)
        blowup->respawn_timer -= FRAMETIME;
      f32 limit =
          (blowup->type->b106 == -1 && !(blowup->sa8 & 1)) ? 2.0f : 1.0f;
      i32 plugged = 0;
      BLOWUPPLUGSYS *plugs = (BLOWUPPLUGSYS *)world->plug_sys;
      if (plugs != NULL && plugs->plugs != NULL) {
        BLOWUPPLUG *plug = plugs->plugs;
        for (i32 j = 0; j < plugs->count; j++, plug++) {
          if (plug->blowup_ix != -1 &&
              plug->blowup_ix < world->gizmo_blowup_count &&
              blowup == &world->gizmo_blowups[plug->blowup_ix]) {
            plugged = 1;
            break;
          }
        }
      }
      if (blowup->respawn_timer <= limit && !plugged) {
        blowup->visible = 1;
        if (blowup->respawn_timer <= 0.0f) {
          GizBlowup_Respawn(blowup);
        } else if (PickupFlickerFrame % 6 >= 3) {
          blowup->visible = 0;
        }
        f32 rx = (blowup->bounds_max.x - blowup->bounds_min.x) * 0.5f;
        f32 rz = (blowup->bounds_max.z - blowup->bounds_min.z) * 0.5f;
        if (rz > rx)
          rx = rz;
        PushAway(&blowup->f58, rx, &blowup->bounds_min, &blowup->bounds_max,
                 NULL, NULL, NULL, 1.0f, 4);
      }
    }

    NUINSTANIM_UNK *anim =
        NuSpecialGetInstAnim(&blowup->type->animated_special);
    u32 activated = blowup->activated;
    if (!((activated && !blowup->blown_up) || (blowup->flags & 0x20000))) {
      if (!activated && anim != NULL && anim->playing)
        anim->playing = 0;
    } else {
      if (blowup->draw_flags & 0x400000) {
        *(numtx_s *)blowup =
            *NuSpecialGetInstanceMtx(&blowup->type->animated_special);
        blowup->b16 = 1;
      }
      if (blowup->sa8 != 0)
        blowup->b16 = 1;
      if (blowup->b16) {
        UpdateMidPos(blowup);
        updated = 1;
      }
      if (anim != NULL) {
        f32 end_frame = NuAnimEndFrameOld(
            (*(void ***)((u8 *)blowup->type->animated_special.scene +
                         0x54))[anim->anim_ix]);
        f32 base_frame = blowup->type->animation_base_frame;
        if (blowup->b19) {
          if (!(blowup->type->b100 & 1) &&
              ((blowup->type->animation_flags & 0x2000) || blowup->b22)) {
            blowup->type->animation_base_frame +=
                FRAMETIME * 60.0f * anim->tfactor;
            blowup->b16 = 1;
            if (blowup->type->animation_base_frame >= end_frame) {
              if (anim->repeating)
                blowup->type->animation_base_frame = 1.0f;
              else
                blowup->b24 = 1;
            }
            blowup->type->b100 |= 1;
          }
          blowup->fc8 += FRAMETIME * 60.0f * anim->tfactor;
          numtx_s m;
          GizmoBlowupEvalAnim(blowup, blowup->type, base_frame, &m, anim,
                              end_frame);
          blowup->f58.x = m.m30;
          blowup->f58.y = m.m31;
          blowup->f58.z = m.m32;
          if (blowup->fc8 >= end_frame) {
            if (!anim->repeating) {
              blowup->fc8 = blowup->type->animation_start_frame;
              blowup->b24 = 1;
            } else {
              blowup->fc8 = 0.0f;
            }
          }
        }
      }
      if (blowup->type->sfx1 != -1 && Unk00558ee0(blowup->type->sfx1) == 1)
        GameAudio_PlaySfxById(blowup->type->sfx1, &blowup->f58, 0, 0);
      if (blowup->b18) {
        if (blowup->fc0 > 0.0f) {
          if ((blowup->fc0 -= FRAMETIME) <= 0.0f) {
            if (anim == NULL || blowup->b19)
              blowup->b24 = 1;
            else
              blowup->b19 = 1;
            blowup->b18 = 0;
          }
        }
      } else if (blowup->flags & 0x20000) {
        if (blowup->fc8 > 0.0f)
          blowup->fc8 -= FRAMETIME;
        else
          blowup->flags &= ~0x20000;
      }
      if (blowup->b24)
        GizmoBlowupBlowup(blowup, 1, -1, 1, NULL, 1);
      for (i32 k = 7; k < 9; k++) {
        if (blowup->type->particle_types[k] != -1)
          AddVariableShotDebrisEffectTimed1(blowup->type->particle_types[k],
                                            &blowup->f58, 60, FRAMETIME, 0, 0,
                                            NULL);
      }
      if (g_unk00aca000 != NULL && blowup->type->b106 != -1) {
        UNK005EE1A0 *info = Unk005ee1a0(blowup->type->b106);
        if (info != NULL)
          g_unk00aca000(0, 0, blowup, info, 1000000000.0f, 0, i);
        if ((info->flags & 0x20000000000ULL) && blowup->platform_id == -1) {
          f32 r = blowup->fb4 * 0.5f;
          PushAway(&blowup->f58, r, NULL, NULL, NULL, NULL, NULL, 3.0f, 5);
        }
      }
    }

    blowup->flags &= 0x7fffffff;
    blowup->b122_7 = 0;
    if (blowup->visible) {
      if (blowup->antinode != NULL) {
        if (updated)
          GameAntiNode_Update(
              g_unk00960894->game_antinode_sys, blowup->antinode, &blowup->f58,
              blowup->rot_z + blowup->rot_y, blowup->type->anti_node_data, 0.0f,
              !((blowup->draw_flags >> 14) & 1));
      } else {
        if (!updated)
          UpdateMidPos(blowup);
        blowup->antinode = GameAntiNode_Add(
            g_unk00960894->game_antinode_sys, &blowup->f58,
            blowup->rot_z + blowup->rot_y, blowup->type->anti_node_data, 0.0f,
            !((blowup->draw_flags >> 14) & 1));
        if (blowup->antinode != NULL && (blowup->b122 & 0x200))
          ((u8 *)blowup->antinode)[0x37] |= 1;
      }
    } else if (blowup->antinode != NULL) {
      GameAntiNode_Remove(g_unk00960894->game_antinode_sys, blowup->antinode);
      blowup->antinode = NULL;
    }
  }
}

// GLOBAL: LEGOBATMAN 0x0085d378
extern const f32 BlowupDetectRange;
// GLOBAL: LEGOBATMAN 0x00ac7394
static i32 g_unk00ac7394;
// GLOBAL: LEGOBATMAN 0x00ac7398
static u8 *g_unk00ac7398;
// GLOBAL: LEGOBATMAN 0x00ac739c
extern GIZMOBLOWUPTYPE_s *g_unk00ac739c;
// GLOBAL: LEGOBATMAN 0x00ac73a0
extern GIZMOBLOWUPTYPE_s *g_unk00ac73a0;
// GLOBAL: LEGOBATMAN 0x00960e1c
extern i32 g_unk00960e1c;
// GLOBAL: LEGOBATMAN 0x00ab3980
extern GameObject_s *player;
// GLOBAL: LEGOBATMAN 0x00ab3984
extern GameObject_s *player2;
// GLOBAL: LEGOBATMAN 0x00aca594
extern u8 *g_unk00aca594;
// GLOBAL: LEGOBATMAN 0x00966628
extern i32 g_unk00966628;
// GLOBAL: LEGOBATMAN 0x0096662c
extern i32 g_unk0096662c;
// GLOBAL: LEGOBATMAN 0x00961224
extern f32 g_unk00961224;
// GLOBAL: LEGOBATMAN 0x009652cc
extern f32 AreaPickupScale;
// GLOBAL: LEGOBATMAN 0x00960c94
extern f32 g_unk00960c94;
// GLOBAL: LEGOBATMAN 0x00a95800
extern char *g_unk00a95800;

typedef struct ADDGAMEMSG_s {
  char *text;               // 0x00
  nuvec_s *position;        // 0x04
  nuvec_s *target_position; // 0x08
  f32 scale;                // 0x0c
  f32 target_scale;         // 0x10
  u8 red;                   // 0x14
  u8 green;                 // 0x15
  u8 blue;                  // 0x16
  u8 pad17;
  u32 flags;    // 0x18
  f32 duration; // 0x1c
  u32 pad20[(0x44 - 0x20) / 4];
  void *callback; // 0x44
  u32 pad48;
  u8 pad4c[3];
  u8 b4f; // 0x4f
} ADDGAMEMSG;

// GLOBAL: LEGOBATMAN 0x00960c98
extern ADDGAMEMSG AddGameMsg_Default;

void *AddGameMsg(ADDGAMEMSG *message);
u8 *Hint_FindHint(i32 id);
f32 NuVecDistSqr(nuvec_s *a, nuvec_s *b, nuvec_s *d);
i32 Transform_TargettedByObj(GIZMOBLOWUP_s *blowup);
f32 SeekLinearF(f32 current, f32 target, f32 step);
u64 Cheats_CheckFlags(u64 flags);
void MiniKitDetector(nuvec_s *position);
i32 qrand(void);
i32 ParticlesPerSecond(f32 particles_per_second, f32 frame_time);
void NuVecRotateY(nuvec_s *v, nuvec_s *v0, i32 a);
void NuVecAdd(nuvec_s *out, nuvec_s *a, nuvec_s *b);
i32 NewShadowOnPlatform(void);
extern "C" void AddVariableShotDebrisEffect(i32 effect, nuvec_s *pos, i32 count,
                                            i32 a, i32 b);
void Unk005d4220(void);

#define CABLE_P4(p) (*(u8 **)((u8 *)(p)->cable157c + 4))
#define CABLE_FLAGS(c) ((*(u8 **)(*(u8 **)((c) + 0x54) + 0x24))[0x144])

// FUNCTION: LEGOBATMAN 0x005db050
void GizmoBlowupLateUpdate(void *world_ptr, void *unused, f32 dt) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  i32 ncand;
  i32 near;
  nuvec_s v;
  nuvec_s mn[2];
  nuvec_s mx[2];
  i16 cand[512];
  u8 *c;

  g_unk00ac7394 = 0;
  if (world == NULL)
    return;
  ncand = 0;
  GIZMOBLOWUPTYPE_s *type = world->gizmo_blowup_types;
  for (i32 i = 0; i < world->gizmo_blowup_type_count; i++, type++) {
    if (NuSpecialExistsFn(&type->animated_special) &&
        NuSpecialGetInstAnim(&type->animated_special) != NULL &&
        (type->b100 & 1))
      type->b100 &= ~1;
  }
  if (g_unk00ac7398 == NULL && g_unk00960e1c != -1)
    g_unk00ac7398 = Hint_FindHint(g_unk00960e1c);
  if (g_unk00ac7398 != NULL && g_unk00ac7398[0x16] == 0) {
    if (player != NULL) {
      mn[0].x = player->v80.x - BlowupDetectRange;
      mn[0].y = player->v80.y - BlowupDetectRange;
      mn[0].z = player->v80.z - BlowupDetectRange;
      mx[0].x = player->v80.x + BlowupDetectRange;
      mx[0].y = player->v80.y + BlowupDetectRange;
      mx[0].z = player->v80.z + BlowupDetectRange;
    }
    if (player2 != NULL) {
      mn[1].x = player2->v80.x - BlowupDetectRange;
      mn[1].y = player2->v80.y - BlowupDetectRange;
      mn[1].z = player2->v80.z - BlowupDetectRange;
      mx[1].x = player2->v80.x + BlowupDetectRange;
      mx[1].y = player2->v80.y + BlowupDetectRange;
      mx[1].z = player2->v80.z + BlowupDetectRange;
    }
  }
  if ((player != NULL && player->cable157c != NULL &&
       (c = CABLE_P4(player)) != NULL && (CABLE_FLAGS(c) & 8)) ||
      (player2 != NULL && player2->cable157c != NULL &&
       (c = CABLE_P4(player2)) != NULL && (CABLE_FLAGS(c) & 8)))
    near = 1;
  else
    near = 0;

  GIZMOBLOWUP_s *blowup = world->gizmo_blowups;
  for (i32 i = 0; i < world->gizmo_blowup_count; i++, blowup++) {
    if (blowup->b15 && blowup->activated && blowup->visible &&
        !blowup->blown_up) {
      if ((blowup->draw_flags & 0x40) && g_unk00ac7398 != NULL &&
          g_unk00ac7398[0x16] == 0) {
        if (player != NULL && blowup->f58.x > mn[0].x &&
            blowup->f58.x < mx[0].x && blowup->f58.z > mn[0].z &&
            blowup->f58.z < mx[0].z && blowup->f58.y > mn[0].y &&
            blowup->f58.y < mx[0].y &&
            NuVecDistSqr(&blowup->f58, &player->v80, NULL) <
                BlowupDetectRange * BlowupDetectRange) {
          g_unk00ac7394++;
        } else if (player2 != NULL && blowup->f58.x > mn[1].x &&
                   blowup->f58.x < mx[1].x && blowup->f58.z > mn[1].z &&
                   blowup->f58.z < mx[1].z && blowup->f58.y > mn[1].y &&
                   blowup->f58.y < mx[1].y &&
                   NuVecDistSqr(&blowup->f58, &player2->v80, NULL) <
                       BlowupDetectRange * BlowupDetectRange) {
          g_unk00ac7394++;
        }
      }
      u32 df = blowup->draw_flags & 0x4018083;
      if (df == 2 || df == 0x8002 || (df & 0x4000000))
        cand[ncand++] = i;
      if (near && (blowup->draw_flags & 0x4000000)) {
        ADDGAMEMSG msg = AddGameMsg_Default;
        msg.position = &blowup->f58;
        f32 scale = world->area != NULL ? *(f32 *)((u8 *)world->area + 0xb8)
                                        : AreaPickupScale;
        msg.flags = 0x87;
        msg.text = g_unk00a95800;
        msg.scale = scale * g_unk00960c94;
        msg.b4f = 4;
        msg.red = 0xff;
        msg.green = 0xbf;
        msg.blue = 0;
        msg.callback = (void *)Unk005d4220;
        AddGameMsg(&msg);
      }
    }
    if (blowup->draw_flags & 0x10) {
      if (Transform_TargettedByObj(blowup))
        blowup->flags |= 0x80000000;
      else if (blowup->scale < 1.0f)
        blowup->scale = SeekLinearF(blowup->scale, 1.0f, FRAMETIME);
    }
    if (Cheats_CheckFlags(0x40000) &&
        (world->i124 == -1 || g_unk00aca594 == NULL ||
         !(g_unk00aca594[world->i124 * 12 + 7] & 1))) {
      if (blowup->type == g_unk00ac739c && !blowup->b29) {
        if (blowup->visible)
          MiniKitDetector(&blowup->position);
      } else if (blowup->type == g_unk00ac73a0 && !blowup->blown_up) {
        MiniKitDetector(&blowup->position);
      }
    }
  }

  if (ncand > 0) {
    i32 effect = VehicleArea ? g_unk0096662c : g_unk00966628;
    if (effect != -1 &&
        ((i32 *)*(u8 **)((u8 *)world->p138 + 8))[effect * 5] != -1) {
      GIZMOBLOWUP_s *b =
          &world->gizmo_blowups[cand[qrand() / (0xffff / ncand + 1)]];
      if (ParticlesPerSecond(ncand * g_unk00961224, FRAMETIME) > 0) {
        v.x = v.y = 0.0f;
        v.z = qrand() * (1.0f / 65535.0f) * b->fb4;
        NuVecRotateY(&v, &v, qrand());
        NuVecAdd(&v, &v, &b->f58);
        f32 h = GameShadow(NULL, &v, VehicleArea ? 6.0f : 2.0f, -1);
        if (h != 2000000.0) {
          i32 plat = NewShadowOnPlatform();
          if (b->platform_id == -1 || plat == b->platform_id) {
            v.y = h;
            AddVariableShotDebrisEffect(
                ((i32 *)*(u8 **)((u8 *)world->p138 + 8))[effect * 5], &v, 1, 0,
                0);
          }
        }
      }
    }
  }
}

// FUNCTION: LEGOBATMAN 0x005dd9f0
GIZMOBLOWUP_s *GizmoBlowUp_FindFromPlatID(WORLDINFO_s *world, i32 platform_id) {
  GIZMOBLOWUP_s *blowup = world->gizmo_blowups;
  if (blowup != NULL) {
    for (i32 i = 0; i < world->gizmo_blowup_count; i++, blowup++) {
      if (blowup->platform_id == platform_id)
        return blowup;
    }
  }
  return NULL;
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

f32 SeekLinearF(f32 current, f32 target, f32 step);

// FUNCTION: LEGOBATMAN 0x005dafe0
void PlayAnim(NUINSTANIM_UNK *animation, f32 *playback, f32 speed,
              i32 backwards) {
  animation->playing = 1;
  if (speed >= 0.0f)
    *playback = SeekLinearF(*playback, backwards ? 1.0f : -1.0f, speed);
  animation->tfactor = *playback * animation->fparam1;
}

// FUNCTION: LEGOBATMAN 0x005db7a0
i32 GizmoBlowupTypeNameBlank(char *name) {
  for (i32 i = 0; i < 8; i++) {
    if (((i32 *)name)[i] != 0)
      return 0;
  }
  return 1;
}

void DeletePlatinst(i32 id);
i32 NuSpecialGetInstanceix(nuhspecial_s *special);
i32 FindPlatInst(i32 instance);
i16 NewPlatPickupInst(void *m, i32 type);
i16 Unk00573890(void *object, i32 instance);
void PlatInstRotate(i32 id, i32 rotate);

typedef struct UNK005EE030_s {
  u8 pad0[0xf8];
  i16 pickup_type; // 0xf8
} UNK005EE030;

UNK005EE030 *Unk005ee030(nuhspecial_s *special);

// FUNCTION: LEGOBATMAN 0x005db7c0
i32 GizBlowup_InitSingleTerrain(GIZMOBLOWUP_s *blowup) {
  if (blowup != NULL && (!blowup->blown_up || blowup->sa8 != 0) &&
      (blowup->draw_flags & 4)) {
    if (blowup->platform_id == -1) {
      i32 instance = NuSpecialGetInstanceix(&blowup->type->animated_special);
      UNK005EE030 *info = Unk005ee030(&blowup->type->animated_special);
      if (info != NULL) {
        if (blowup->type->b106 != -1 && info->pickup_type != -1)
          blowup->platform_id = NewPlatPickupInst(blowup, info->pickup_type);
      } else if ((blowup->draw_flags & 0x20000000) &&
                 !(blowup->draw_flags & 0x800000)) {
        blowup->platform_id = Unk00573890(blowup, instance);
      } else if (blowup->draw_flags & 0x20000000) {
        blowup->platform_id = FindPlatInst(instance);
      } else {
        blowup->platform_id = Unk00573890(blowup, instance);
      }
      if (blowup->platform_id == -1)
        return 0;
      if (!blowup->visible)
        PlatOnOff(blowup->platform_id, 0);
    }
    PlatInstRotate(blowup->platform_id, 1);
    if (blowup->platform_id2 == -1 &&
        NuSpecialExistsFn(&blowup->type->burst_special)) {
      blowup->platform_id2 = Unk00573890(
          blowup, NuSpecialGetInstanceix(&blowup->type->burst_special));
      if (blowup->platform_id2 == -1)
        return -1;
    }
    PlatInstRotate(blowup->platform_id2, 1);
    return 1;
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x005db930
void GizBlowup_InitTerrain() {
  if (g_unk00960894->gizmo_blowups != NULL &&
      g_unk00960894->gizmo_blowup_count > 0) {
    GIZMOBLOWUP_s *blowup = g_unk00960894->gizmo_blowups;
    for (i32 i = 0; i < g_unk00960894->gizmo_blowup_count; i++, blowup++) {
      blowup->platform_id = -1;
      blowup->platform_id2 = -1;
      if (blowup->draw_flags & 4)
        GizBlowup_InitSingleTerrain(blowup);
    }
  }
}

// FUNCTION: LEGOBATMAN 0x005db9a0
void GizBlowup_DeleteSingleTerrain(GIZMOBLOWUP_s *blowup) {
  if (blowup == NULL)
    return;
  if (blowup->platform_id != -1) {
    DeletePlatinst(blowup->platform_id);
    blowup->platform_id = -1;
  }
  if (blowup->platform_id2 != -1) {
    DeletePlatinst(blowup->platform_id2);
    blowup->platform_id2 = -1;
  }
}

// FUNCTION: LEGOBATMAN 0x005db9f0
void GizBlowup_DeleteTerrain() {
  if (g_unk00960894->gizmo_blowups != NULL &&
      g_unk00960894->gizmo_blowup_count > 0) {
    GIZMOBLOWUP_s *blowup = g_unk00960894->gizmo_blowups;
    for (i32 i = 0; i < g_unk00960894->gizmo_blowup_count; i++, blowup++)
      GizBlowup_DeleteSingleTerrain(blowup);
  }
}

numtx_s *NuSpecialGetDrawMtx(nuhspecial_s *sp);
void NuMtxRotateY(numtx_s *m, i32 a);
void NuMtxPreRotateX(numtx_s *m, i32 a);
void NuMtxPreRotateZ(numtx_s *m, i32 a);
void NuMtxTranslate(numtx_s *m, nuvec_s *v);
void NuMtxSetIdentity(numtx_s *m);
void NuMtxSetScale(numtx_s *m, nuvec_s *s);

// GLOBAL: LEGOBATMAN 0x00ab056c
extern nuvec_s v000;
// GLOBAL: LEGOBATMAN 0x0095fd38
extern nuvec_s g_unk0095fd38;

// FUNCTION: LEGOBATMAN 0x005dba80
void GizmoBlowupUpdateMatrix(GIZMOBLOWUP_s *blowup) {
  if (blowup == NULL || blowup->type == NULL ||
      !NuSpecialExistsFn(&blowup->type->animated_special))
    return;
  nuvec_s position = blowup->position;
  *(numtx_s *)blowup = *NuSpecialGetDrawMtx(&blowup->type->animated_special);
  blowup->position = v000;
  NuMtxRotateY((numtx_s *)blowup, blowup->rot_y);
  NuMtxPreRotateX((numtx_s *)blowup, blowup->rot_x);
  NuMtxPreRotateZ((numtx_s *)blowup, blowup->rot_z);
  NuMtxTranslate((numtx_s *)blowup, &position);
}

void NuMtxScaleU(numtx_s *m, f32 s);

// FUNCTION: LEGOBATMAN 0x005dbb40
void GizmoBlowupGenDecalMatrix(GIZMOBLOWUP_s *blowup, numtx_s *m,
                               i32 alternate) {
  if (blowup == NULL)
    return;
  NuMtxSetIdentity(m);
  if (alternate != 0)
    NuMtxScaleU(m, blowup->decal_scale);
  else
    NuMtxScaleU(m, (1.0f - 10.0f * blowup->fc8) * blowup->decal_scale);
  m->m32 = m->m31 = m->m30 = 0.0f;
  NuMtxRotateY(m, blowup->decal_rot_y);
  NuMtxPreRotateX(m, blowup->decal_rot_x);
  NuMtxPreRotateZ(m, blowup->decal_rot_z);
  nuvec_s v;
  v.x = blowup->decal_offset.x + blowup->position.x;
  v.y = blowup->decal_offset.y + blowup->position.y;
  v.z = blowup->decal_offset.z + blowup->position.z;
  NuMtxTranslate(m, &v);
}

// FUNCTION: LEGOBATMAN 0x005dbc10
void GizmoBlowupGenShadowMatrix(GIZMOBLOWUP_s *blowup, numtx_s *m) {
  if (blowup == NULL)
    return;
  NuMtxSetIdentity(m);
  NuMtxSetScale(m, &g_unk0095fd38);
  m->m32 = m->m31 = m->m30 = 0.0f;
  NuMtxRotateY(m, blowup->shadow_rot_y);
  NuMtxPreRotateX(m, blowup->shadow_rot_x);
  NuMtxPreRotateZ(m, blowup->shadow_rot_z);
  nuvec_s v;
  v.x = blowup->shadow_offset.x + blowup->position.x;
  v.y = blowup->shadow_offset.y + blowup->position.y;
  v.z = blowup->shadow_offset.z + blowup->position.z;
  NuMtxTranslate(m, &v);
}

// FUNCTION: LEGOBATMAN 0x005dbcb0
GIZMOBLOWUPTYPE_s *GizmoBlowup_FindType(char *name, WORLDINFO_s *world) {
  if (world == NULL)
    return NULL;
  if (world->gizmo_blowup_types == NULL)
    return NULL;
  if (world->gizmo_blowup_type_count == 0 &&
      world->gizmo_blowup_extra_types == 0)
    return NULL;
  if (name != NULL) {
    for (i32 i = 0; i < world->current_level->max_gizmoblowuptypes +
                            world->gizmo_blowup_extra_types;
         i++) {
      if (NuStrICmp(world->gizmo_blowup_types[i].name, name) == 0)
        return &world->gizmo_blowup_types[i];
    }
  }
  return NULL;
}

// FUNCTION: LEGOBATMAN 0x005dbd60
void gizmoblowup_PreLoad(void *world_ptr) {
  ((WORLDINFO_s *)world_ptr)->f525c = 100.0f;
}

// FUNCTION: LEGOBATMAN 0x005de530
void GizmoBlowupVisibilityOverrides(WORLDINFO_s *world) {
  GIZMOBLOWUP_s *blowup = world->gizmo_blowups;
  for (i32 i = 0; i < world->gizmo_blowup_count; i++, blowup++) {
    if ((blowup->type->animation_flags & 0x2000) &&
        !(blowup->draw_flags & 0x800000))
      NuSpecialSetVisibility(&blowup->type->animated_special, 0);
  }
  for (i32 i = 0; i < world->gizmo_blowup_type_count; i++) {
    GIZMOBLOWUPTYPE_s *type = &world->gizmo_blowup_types[i];
    if (NuSpecialExistsFn(&type->decal_special))
      NuSpecialSetVisibility(&type->decal_special, 0);
  }
}

// FUNCTION: LEGOBATMAN 0x005de5d0
i32 GizmoBlowupTypeGetCount(WORLDINFO_s *world) {
  if (world == NULL)
    return -1;
  return world->gizmo_blowup_type_count;
}

// FUNCTION: LEGOBATMAN 0x005de5f0
char *GizmoBlowupTypeGetNameFromIndex(WORLDINFO_s *world, i32 index) {
  if (world == NULL || index == -1 || index > world->gizmo_blowup_type_count)
    return NULL;
  GIZMOBLOWUPTYPE_s *type = &world->gizmo_blowup_types[index];
  return NuStrLen(type->name) != 0 ? type->name : NULL;
}

// FUNCTION: LEGOBATMAN 0x005de630
i32 GizmoBlowupTypeGetIndexFromName(WORLDINFO_s *world, char *name) {
  if (name == NULL || world == NULL)
    return -1;
  GIZMOBLOWUPTYPE_s *type = world->gizmo_blowup_types;
  for (i32 i = 0; i < world->gizmo_blowup_type_count; i++, type++) {
    if (NuStrICmp(name, type->name) == 0)
      return i;
  }
  return -1;
}

void GizmoBlowUp_AddEffects(nuvec_s *position, GIZMOBLOWUP_s *blowup, i32 count,
                            i32 flags, GameObject_s *object);

// FUNCTION: LEGOBATMAN 0x005de6a0
void GizmoBlowUpTypeBlowUp(WORLDINFO_s *world, i32 type_index,
                           nuvec_s *position) {
  char *name = GizmoBlowupTypeGetNameFromIndex(world, type_index);
  if (name != NULL) {
    GIZMOBLOWUPTYPE_s *type = GizmoBlowup_FindType(name, world);
    if (type != NULL) {
      GIZMOBLOWUP_s blowup;
      memset(&blowup, 0, sizeof(blowup));
      blowup.type = type;
      i32 flags = 7;
      if (blowup.fbc > 0.0f)
        flags = 15;
      GizmoBlowUp_AddEffects(position, &blowup, 0, flags, NULL);
      GameAudio_PlaySfx(0x46, position, 0, 0);
    }
  }
}

void NuStrNCpy(char *dst, const char *src, i32 n);

// GLOBAL: LEGOBATMAN 0x00ac6f70
static i32 gizmoblowupnametable_numids;
// GLOBAL: LEGOBATMAN 0x00ac6f80
static char gizmoblowupnametable[32][32];

// FUNCTION: LEGOBATMAN 0x005de790
void GizmoBlowupResetNameTable() {
  memset(gizmoblowupnametable, 0, sizeof(gizmoblowupnametable));
  gizmoblowupnametable_numids = 0;
}

// FUNCTION: LEGOBATMAN 0x005de7b0
i32 GizmoBlowupGetNameTableId(char *name) {
  for (i32 id = 0; id < gizmoblowupnametable_numids; id++) {
    if (NuStrICmp(gizmoblowupnametable[id], name) == 0)
      return id;
  }
  if (gizmoblowupnametable_numids < 32) {
    NuStrNCpy(gizmoblowupnametable[gizmoblowupnametable_numids], name,
              sizeof(gizmoblowupnametable[0]));
    gizmoblowupnametable_numids++;
    return gizmoblowupnametable_numids - 1;
  }
  return -1;
}

// FUNCTION: LEGOBATMAN 0x005de820
i32 GizmoBlowupGetTypeFromNameTableId(WORLDINFO_s *world, i32 name_id) {
  if (name_id >= 0 && name_id < gizmoblowupnametable_numids) {
    char *name = gizmoblowupnametable[name_id];
    if (name != NULL) {
      GIZMOBLOWUPTYPE_s *type = world->gizmo_blowup_types;
      for (i32 i = 0; i < world->gizmo_blowup_type_count; i++, type++) {
        if (NuStrICmp(type->name, name) == 0)
          return i;
      }
    }
  }
  return -1;
}

typedef struct TRANSFORM_TYPE_s {
  char *name; // 0x00
  u8 *id;     // 0x04
} TRANSFORM_TYPE_s;

typedef struct TORPEDO_TYPE_s {
  char *name; // 0x00
  u8 *id;     // 0x04
} TORPEDO_TYPE_s;

// GLOBAL: LEGOBATMAN 0x00ac73b8
TRANSFORM_TYPE_s *TransformTypes;
// GLOBAL: LEGOBATMAN 0x00ac73bc
i32 NumTransformTypes;
// GLOBAL: LEGOBATMAN 0x00ac73c0
TORPEDO_TYPE_s *TorpedoTypes;
// GLOBAL: LEGOBATMAN 0x00ac73c4
i32 NumTorpedoTypes;

// FUNCTION: LEGOBATMAN 0x005ded50
void RegisterTransformTypes(TRANSFORM_TYPE_s *types, i32 count) {
  NumTransformTypes = 0;
  if (types != NULL && count >= 1 && count <= 3) {
    TransformTypes = types;
    for (; types->name != NULL; types++) {
      NumTransformTypes++;
      *types->id = NumTransformTypes;
    }
  }
}

// FUNCTION: LEGOBATMAN 0x005deda0
void RegisterTorpedoTypes(TORPEDO_TYPE_s *types, i32 count) {
  NumTorpedoTypes = 0;
  if (types != NULL && count >= 1 && count <= 7) {
    TorpedoTypes = types;
    for (; types->name != NULL; types++) {
      NumTorpedoTypes++;
      *types->id = NumTorpedoTypes;
    }
  }
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
  addtype.fns[25] = (void *)gizmoblowup_PreLoad;
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
