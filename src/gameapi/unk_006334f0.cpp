// gameapi/unk_006334f0.cpp: TU of unknown name, found by its header-static
// copies (the functions after them are not matched yet).

#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/numtx_inline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x00633670
static void NuMtxSetRotationYInline(f32 *m, i32 a);

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x00633710
static void NuMtxRotateYInline(f32 *m, i32 a);

// FUNCTION: LEGOBATMAN 0x006334f0
static f32 NuSinApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x00633590
static f32 NuCosApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x006335a0
static void NuVec4Copy(f32 *dst, f32 *src);
// FUNCTION: LEGOBATMAN 0x006335c0
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_006334f0(f32 *v, f32 a, i32 i) {
  v[0] = NuSinApprox(i);
  v[1] = NuCosApprox(i);
  NuVec4Copy(v + 4, v);
  NuVec4Set(v, a, a, a, a);
}

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_unk_006334f0(f32 *v, f32 a, i32 i) {
  NuMtxRotateYInline(v + 16, i);
}

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_2_unk_006334f0(f32 *v, f32 a, i32 i) {
  NuMtxSetRotationYInline(v + 64, i);
}

// GizmoPickup gizmo callbacks (Mac GizmoPickups_* / GizmoPickup_*), named by
// their RegisterGizmo 0x636d40 slot.

#include "../batman/leveldata_unk.h"
#include "../batman/worldinfo_unk.h"
#include <stddef.h>
#include <string.h>

typedef struct GIZMOPICKUPPROGRESS_s {
  u32 collected[16]; // 0x00
  u32 enabled[16];   // 0x40
  u32 visible[16];   // 0x80
  u32 activated[16]; // 0xc0
} GIZMOPICKUPPROGRESS;

typedef struct GIZMO_s {
  void *object;
} GIZMO;

typedef struct ADDGIZMOTYPE_s {
  char *name;        // 0x00
  char *prefix;      // 0x04
  u16 progress_size; // 0x08
  void *fns[0x1c];   // 0x0c
} ADDGIZMOTYPE;

// GLOBAL: LEGOBATMAN 0x00960118
extern ADDGIZMOTYPE Default_ADDGIZMOTYPE;
// GLOBAL: LEGOBATMAN 0x00965300
extern char *GizmoPickup_OutputName;

void AddGizmo(GIZMOSYS_s *gizmo_sys, i32 type_id, void *a, void *object);
void *GameBufferAllocProgressUnk005bbaf0(VARIPTR *buf, VARIPTR *buf_end,
                                         i32 size);

// FUNCTION: LEGOBATMAN 0x00634480
i32 GizmoPickups_GetMaxGizmos(void *world_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  if (world != NULL)
    return (u16)world->current_level->max_pickups;
  return 0;
}

// FUNCTION: LEGOBATMAN 0x006344a0
void GizmoPickups_AddGizmos(GIZMOSYS_s *gizmo_sys, i32 type_id, void *world_ptr,
                            void *unused) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  for (i32 i = 0; i < world->p5220->count; i++) {
    Unk_WorldInfo5220Entry *pickup = &world->p5220->list[i];
    if (pickup->b15 & 2)
      AddGizmo(gizmo_sys, type_id, NULL, pickup);
  }
}

// FUNCTION: LEGOBATMAN 0x00634500
char *GizmoPickup_GetGizmoName(GIZMO *gizmo) {
  return gizmo != NULL ? ((Unk_WorldInfo5220Entry *)gizmo->object)->name : NULL;
}

// FUNCTION: LEGOBATMAN 0x00634510
i32 GizmoPickup_GetOutput(GIZMO *gizmo, i32 a, i32 b) {
  return ((Unk_WorldInfo5220Entry *)gizmo->object)->collected;
}

// FUNCTION: LEGOBATMAN 0x00634530
char *GizmoPickup_GetOutputName(GIZMO *gizmo, i32 output) {
  return GizmoPickup_OutputName;
}

// FUNCTION: LEGOBATMAN 0x00634540
i32 GizmoPickup_GetNumOutputs(GIZMO *gizmo) { return 1; }

struct GIZMOPICKUPTYPE_s {
  u8 pad0[4];
  char *sfx; // 0x04
  u8 pad8[0xe - 0x8];
  u8 flags; // 0x0e
  u8 padf[0x2c - 0xf];
  i16 debris; // 0x2c
  u8 pad2e[0x3c - 0x2e];
};

struct GIZMOPICKUPSYS_s {
  GIZMOPICKUPTYPE_s *types; // 0x00
  u8 pad4[0xc - 0x4];
  i8 type_c; // 0x0c
  u8 padd;
  i8 type_e; // 0x0e
};

// GLOBAL: LEGOBATMAN 0x009652e4
extern GIZMOPICKUPSYS_s *GizmoPickupSys;
// GLOBAL: LEGOBATMAN 0x00ab084c
extern i32 ChallengeMode;

WORLDINFO_s *WorldInfo_CurrentlyActive(void);
void SuperCounter_ActivateGizmoPickup(GIZMO *gizmo,
                                      Unk_WorldInfo5220Entry *pickup);
void *Mission_Active(void *ms);
void PlaySfx(char *name, nuvec_s *position);
i32 AddGameDebris(void *system, i32 type, nuvec_s *position);

// FUNCTION: LEGOBATMAN 0x00634550
void GizmoPickup_Activate(GIZMO *gizmo, i32 active) {
  WORLDINFO_s *world = WorldInfo_CurrentlyActive();
  if (gizmo == NULL)
    return;
  Unk_WorldInfo5220Entry *pickup = (Unk_WorldInfo5220Entry *)gizmo->object;
  pickup->enabled = active != 0;
  if (active) {
    SuperCounter_ActivateGizmoPickup(gizmo, pickup);
    if (pickup->state & 2) {
      pickup->state |= 0x80;
      GIZMOPICKUPSYS_s *sys = GizmoPickupSys;
      i32 type;
      if ((pickup->state & 0x40) && sys->type_c != -1)
        type = sys->type_c;
      else if ((pickup->state & 0x100) && sys->type_e != -1)
        type = sys->type_e;
      else
        type = pickup->type_index;
      GIZMOPICKUPTYPE_s *t = &sys->types[type];
      if (ChallengeMode) {
        if (!(t->flags & 0x20))
          goto done;
      } else if (t->flags & 0x20) {
        goto done;
      }
      if (Mission_Active(NULL) == NULL) {
        if (t->sfx != NULL)
          PlaySfx(t->sfx, &pickup->position);
        if (t->debris != -1)
          AddGameDebris(world->p138, t->debris, &pickup->position);
      }
    done:;
    }
  }
  if (pickup->b17 != 0) {
    Unk_WorldInfo5220Entry *other = world->p5220->list;
    for (i32 i = 0; i < world->p5220->count; i++, other++) {
      if (other->b17 == pickup->b17) {
        u32 on = active & 1;
        other->enabled = on;
        other->visible = on;
      }
    }
  }
}

// FUNCTION: LEGOBATMAN 0x006346a0
void *GizmoPickups_AllocateProgressData(VARIPTR *buf, VARIPTR *buf_end) {
  return GameBufferAllocProgressUnk005bbaf0(buf, buf_end,
                                            sizeof(GIZMOPICKUPPROGRESS));
}

// FUNCTION: LEGOBATMAN 0x006346c0
void GizmoPickups_ClearProgress(void *world, void *progress_ptr) {
  GIZMOPICKUPPROGRESS *progress = (GIZMOPICKUPPROGRESS *)progress_ptr;
  if (progress != NULL) {
    memset(progress->visible, -1, sizeof(progress->visible));
    memset(progress->enabled, -1, sizeof(progress->enabled));
    memset(progress->collected, 0, sizeof(progress->collected));
    memset(progress->activated, 0, sizeof(progress->activated));
  }
}

// FUNCTION: LEGOBATMAN 0x00634710
void GizmoPickups_StoreProgress(void *world_ptr, void *unused,
                                void *progress_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  GIZMOPICKUPPROGRESS *progress = (GIZMOPICKUPPROGRESS *)progress_ptr;
  if (progress == NULL)
    return;
  memset(progress->visible, -1, sizeof(progress->visible));
  memset(progress->enabled, -1, sizeof(progress->enabled));
  memset(progress->collected, 0, sizeof(progress->collected));
  memset(progress->activated, 0, sizeof(progress->activated));
  if (world == NULL)
    return;
  Unk_WorldInfo5220Entry *pickup = world->p5220->list;
  if (pickup != NULL) {
    for (i32 i = 0; i < world->p5220->count; i++, pickup++) {
      if (i >= 512)
        break;
      i32 word = i / 32;
      u32 bit = 1 << (i & 31);
      if (!pickup->visible)
        progress->visible[word] &= ~bit;
      if (!pickup->enabled)
        progress->enabled[word] &= ~bit;
      if (pickup->collected)
        progress->collected[word] |= bit;
      if (pickup->activated)
        progress->activated[word] |= bit;
    }
  }
}

void GizmoPickups_Draw(void *world, void *unused, f32 dt);
void GizmoPickups_Update(void *world, void *unused, f32 dt);
typedef struct AREADATA_s {
  u8 pad0[0x7c];
  u32 flags; // 0x7c
  u8 pad80[0xbc - 0x80];
} AREADATA;

// GLOBAL: LEGOBATMAN 0x00aca554
extern AREADATA *ADataList;
// GLOBAL: LEGOBATMAN 0x00aca560
extern i32 AREACOUNT;
// GLOBAL: LEGOBATMAN 0x00aca894
extern LEVELDATA_s *LDataList;
// GLOBAL: LEGOBATMAN 0x00aca578
extern i32 BonusArea;

// GLOBAL: LEGOBATMAN 0x009652cc
extern f32 AreaPickupScale;
// GLOBAL: LEGOBATMAN 0x009652d0
extern f32 AreaPickupGravity;

// FUNCTION: LEGOBATMAN 0x006347e0
static f32 GizmoPickups_AreaScale(i32 area) {
  if (area >= 0 && area < AREACOUNT) {
    u32 flags = ADataList[area].flags;
    if (flags & 0x200)
      return 6.0f;
    if (flags & 1) {
      if (BonusArea != 0)
        return 3.0f;
      return 5.0f;
    }
  }
  return 1.0f;
}

// FUNCTION: LEGOBATMAN 0x00634830
static f32 GizmoPickups_AreaGravity(i32 area, i32 level) {
  if (area >= 0 && area < AREACOUNT) {
    u32 flags = ADataList[area].flags;
    if ((flags & 0x200) && !(LDataList[level].flags & 0x400000))
      return 0.0f;
    if (flags & 1)
      return -20.0f;
  }
  return -6.0f;
}

// STUB: LEGOBATMAN 0x00634880
// open register-arg problem: orig takes version on the stack, ours in eax.
static void GizmoPickups_FixDrawDistance(i32 area, f32 *dist, i32 version) {
  if (version > 6)
    return;
  if (version <= 5 && *dist < 10.0f)
    *dist = 10.0f;
  if (area >= 0 && area < AREACOUNT && (ADataList[area].flags & 1) &&
      *dist < 100.0)
    *dist = 100.0f;
}

// FUNCTION: LEGOBATMAN 0x006348e0
void GizmoPickups_SetAreaGlobals(WORLDINFO_s *world) {
  if (world->p5220 != NULL) {
    AreaPickupScale = world->p5220->pickup_scale;
    AreaPickupGravity = world->p5220->gravity;
  } else {
    AreaPickupScale = GizmoPickups_AreaScale(world->i124);
    AreaPickupGravity = GizmoPickups_AreaGravity(world->i124, world->level_idx);
  }
}

// FUNCTION: LEGOBATMAN 0x00634940
void *GizmoPickups_ReserveBufferSpace(void *world_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  world->buf104.addr = (world->buf104.addr + 3) & ~3;
  world->p5220 = (Unk_WorldInfo5220 *)world->buf104.addr;
  world->buf104.addr += sizeof(Unk_WorldInfo5220);
  memset(world->p5220, 0, sizeof(Unk_WorldInfo5220));
  world->p5220->draw_distance =
      world->area != NULL && (world->area->flags & 1) ? 100.0f : 10.0f;
  f32 scale;
  if (world->i124 >= 0 && world->i124 < AREACOUNT) {
    u32 flags = ADataList[world->i124].flags;
    if (flags & 0x200)
      scale = 6.0f;
    else if (flags & 1)
      scale = BonusArea != 0 ? 3.0f : 5.0f;
    else
      scale = 1.0f;
  } else {
    scale = 1.0f;
  }
  world->p5220->pickup_scale = scale;
  f32 gravity;
  if (world->i124 >= 0 && world->i124 < AREACOUNT) {
    u32 flags = ADataList[world->i124].flags;
    if ((flags & 0x200) && !(LDataList[world->level_idx].flags & 0x400000))
      gravity = 0.0f;
    else if (flags & 1)
      gravity = -20.0f;
    else
      gravity = -6.0f;
  } else {
    gravity = -6.0f;
  }
  world->p5220->gravity = gravity;
  if ((u16)world->current_level->max_pickups > 0) {
    world->buf104.addr = (world->buf104.addr + 3) & ~3;
    world->p5220->list = (Unk_WorldInfo5220Entry *)world->buf104.addr;
    world->buf104.addr +=
        (u16)world->current_level->max_pickups * sizeof(Unk_WorldInfo5220Entry);
  }
  world->buf104.addr = (world->buf104.addr + 3) & ~3;
  world->p5220->temp_list = (Unk_WorldInfo5220Entry *)world->buf104.addr;
  world->buf104.addr += 0x40 * sizeof(Unk_WorldInfo5220Entry);
  return world->p5220;
}

// FUNCTION: LEGOBATMAN 0x00634af0
void GizmoPickup_SetVisibility(GIZMO *gizmo, i32 visible) {
  if (gizmo != NULL)
    ((Unk_WorldInfo5220Entry *)gizmo->object)->visible = visible != 0;
}

// FUNCTION: LEGOBATMAN 0x00634ee0
nuvec_s *GizmoPickup_GetPos(GIZMO *gizmo) {
  if (gizmo == NULL)
    return NULL;
  return &((Unk_WorldInfo5220Entry *)gizmo->object)->position;
}

void GizmoPickups_Reset(void *world, void *unused, void *progress);
void GizmoPickups_PostLoad(void *a, void *b);

// GLOBAL: LEGOBATMAN 0x009652c4
i32 pickup_gizmotype_id = -1;

i32 EdFileReadInt();
char EdFileReadChar();
f32 EdFileReadFloat();
void EdFileRead(void *buf, i32 len);
void EdFileReadNuVec(nuvec_s *v);

// STUB: LEGOBATMAN 0x00634ef0
// only the GizmoPickups_FixDrawDistance call differs (push vs eax arg).
i32 GizmoPickups_Load(void *world_ptr, void *unused) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  if (world->p5220->count == 0) {
    i32 version = EdFileReadInt();
    world->p5220->count = EdFileReadInt();
    if (version >= 3)
      world->p5220->f10 = EdFileReadInt();
    if (version >= 5) {
      world->p5220->draw_distance = EdFileReadFloat();
      world->p5220->pickup_scale = EdFileReadFloat();
      if (version == 5)
        world->p5220->pickup_scale = GizmoPickups_AreaScale(world->i124);
    } else {
      world->p5220->pickup_scale = 1.0f;
    }
    GizmoPickups_FixDrawDistance(world->i124, &world->p5220->draw_distance,
                                 version);
    world->p5220->gravity =
        GizmoPickups_AreaGravity(world->i124, world->level_idx);
    Unk_WorldInfo5220Entry *pickup = world->p5220->list;
    for (i32 i = 0; i < world->p5220->count; i++, pickup++) {
      EdFileRead(pickup->name, 8);
      EdFileReadNuVec(&pickup->position);
      pickup->type_code = EdFileReadChar();
      if (version >= 2)
        pickup->b15 = EdFileReadChar();
      if (version >= 4)
        pickup->b17 = EdFileReadChar();
    }
    return 1;
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x00636d40
ADDGIZMOTYPE *GizmoPickups_RegisterGizmo(i32 type_id) {
  // GLOBAL: LEGOBATMAN 0x009653c0
  static char *name = "GizmoPickup";
  // GLOBAL: LEGOBATMAN 0x00acd9d0
  static ADDGIZMOTYPE addtype;

  addtype = Default_ADDGIZMOTYPE;
  addtype.name = name;
  addtype.prefix = "";
  addtype.progress_size = sizeof(GIZMOPICKUPPROGRESS);
  addtype.fns[0] = (void *)GizmoPickups_GetMaxGizmos;
  addtype.fns[1] = (void *)GizmoPickups_AddGizmos;
  addtype.fns[2] = NULL;
  addtype.fns[3] = (void *)GizmoPickups_Update;
  addtype.fns[4] = (void *)GizmoPickups_Draw;
  addtype.fns[5] = NULL;
  addtype.fns[6] = (void *)GizmoPickup_GetGizmoName;
  addtype.fns[7] = (void *)GizmoPickup_GetOutput;
  addtype.fns[8] = (void *)GizmoPickup_GetOutputName;
  addtype.fns[9] = (void *)GizmoPickup_GetNumOutputs;
  addtype.fns[10] = (void *)GizmoPickup_Activate;
  addtype.fns[11] = NULL;
  addtype.fns[12] = (void *)GizmoPickup_SetVisibility;
  addtype.fns[13] = NULL;
  addtype.fns[14] = (void *)GizmoPickup_GetPos;
  addtype.fns[15] = NULL;
  addtype.fns[16] = NULL;
  addtype.fns[17] = NULL;
  addtype.fns[18] = NULL;
  addtype.fns[19] = (void *)GizmoPickups_AllocateProgressData;
  addtype.fns[20] = (void *)GizmoPickups_ClearProgress;
  addtype.fns[21] = (void *)GizmoPickups_StoreProgress;
  addtype.fns[22] = (void *)GizmoPickups_Reset;
  addtype.fns[23] = (void *)GizmoPickups_ReserveBufferSpace;
  addtype.fns[24] = (void *)GizmoPickups_Load;
  addtype.fns[25] = NULL;
  addtype.fns[26] = (void *)GizmoPickups_PostLoad;
  addtype.fns[27] = NULL;
  pickup_gizmotype_id = type_id;
  return &addtype;
}
