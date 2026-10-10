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
  u8 pad8[0xc - 0x8];
  char code;   // 0x0c
  u8 variants; // 0x0d
  u8 flags;    // 0x0e
  u8 padf[0x14 - 0xf];
  f32 f14; // 0x14
  f32 f18; // 0x18
  f32 f1c; // 0x1c
  u8 pad20[0x2c - 0x20];
  i16 debris; // 0x2c
  u8 pad2e[0x34 - 0x2e];
  f32 f34; // 0x34
  f32 f38; // 0x38
};

struct GIZMOPICKUPSYS_s {
  GIZMOPICKUPTYPE_s *types; // 0x00
  u8 pad4[0x8 - 0x4];
  u16 count; // 0x08
  u8 pada;
  i8 type_b; // 0x0b
  i8 type_c; // 0x0c
  i8 type_d; // 0x0d
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

struct PICKUPLEVELLIST_s {
  char names[10][8]; // 0x00
  u8 count;          // 0x50
  u8 pad51[3];
};

// GLOBAL: LEGOBATMAN 0x009ca98c
extern f32 g_unk009ca98c;
// GLOBAL: LEGOBATMAN 0x009f7a44
extern nuvec_s g_unk009f7a44;
// GLOBAL: LEGOBATMAN 0x00ab0898
extern i32 SuperStory;
// GLOBAL: LEGOBATMAN 0x00aca8fc
extern PICKUPLEVELLIST_s *g_unk00aca8fc;
// GLOBAL: LEGOBATMAN 0x00aca594
extern u8 *g_unk00aca594;

extern "C" void NewTerrPlatformsOff(void);
i32 NuStrICmp(const char *a, const char *b);
f32 GameShadow(void *object, nuvec_s *position, f32 probe_height, i32 a);
void FindAnglesZX(nuvec_s *normal, u16 *x_rotation, u16 *z_rotation);
i32 qrand(void);
i16 NuPortalWhichRoom(nugscn_s *scene, nuvec_s *pos);
void *SuperCounter_FindFromNameAndLevel(char *name, WORLDINFO_s *world,
                                        void **pickup);
i32 SuperCounter_AnyCollected(void *counter, WORLDINFO_s *world);

static inline i32 GizmoPickup_FindType(char code) {
  for (i32 i = 0; i < GizmoPickupSys->count; i++) {
    if (GizmoPickupSys->types[i].code == code)
      return i;
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x00634b20
static void GizmoPickup_Init(WORLDINFO_s *world,
                             Unk_WorldInfo5220Entry *pickup) {
  pickup->type_index = GizmoPickup_FindType(pickup->type_code);
  NewTerrPlatformsOff();
  f32 y = GameShadow(NULL, &pickup->position, g_unk009ca98c, -1);
  pickup->shadow_y = y;
  if (y != 2000000.0) {
    if (pickup->position.y <= y)
      pickup->shadow_y = 2000000.0f;
    else
      FindAnglesZX(&g_unk009f7a44, &pickup->rot_x, &pickup->rot_z);
  }
  pickup->state = (pickup->state & ~0x18) | 7;
  if (GizmoPickupSys->types[pickup->type_index].variants > 0)
    pickup->b16 =
        qrand() /
        (0xffff / GizmoPickupSys->types[pickup->type_index].variants + 1);
  else
    pickup->b16 = 0;
  pickup->b24 = world->scn140 != NULL
                    ? NuPortalWhichRoom(world->scn140, &pickup->position)
                    : -1;
  pickup->s26 = qrand();
  pickup->f28 = 0.0f;
  pickup->state = (pickup->state & ~0x40) | 0x20;
}

// STUB: LEGOBATMAN 0x00634c70
// 2 instructions: type_index/type_d compare loads get edx/ecx swapped.
void GizmoPickups_Reset(void *world_ptr, void *unused, void *progress_ptr) {
  for (i32 i = 0; i < GizmoPickupSys->count; i++) {
    GizmoPickupSys->types[i].f34 =
        GizmoPickupSys->types[i].f1c * GizmoPickupSys->types[i].f14;
    GizmoPickupSys->types[i].f38 =
        GizmoPickupSys->types[i].f1c * GizmoPickupSys->types[i].f18;
  }
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  if (world == NULL)
    return;
  Unk_WorldInfo5220Entry *pickup = world->p5220->list;
  if (pickup != NULL) {
    GIZMOPICKUPPROGRESS *progress = (GIZMOPICKUPPROGRESS *)progress_ptr;
    for (i32 i = 0; i < world->p5220->count; i++, pickup++) {
      GizmoPickup_Init(world, pickup);
      if (progress != NULL && i < 0x200) {
        i32 word = i / 32;
        u32 bit = 1 << (i & 31);
        pickup->visible = (progress->visible[word] & bit) != 0;
        pickup->enabled = (progress->enabled[word] & bit) != 0;
        pickup->collected = (progress->collected[word] & bit) != 0;
        pickup->activated = (progress->activated[word] & bit) != 0;
        if ((pickup->b15 & 1) && !pickup->activated)
          pickup->state &= ~6;
      } else if (pickup->b15 & 1) {
        pickup->state &= ~6;
      }
      if (GizmoPickupSys->type_b != -1 &&
          pickup->type_index == GizmoPickupSys->type_b) {
        if (SuperStory != 0)
          goto collected;
        if (g_unk00aca8fc != NULL) {
          for (i32 j = 0; j < g_unk00aca8fc[world->level_idx].count; j++) {
            if (NuStrICmp(pickup->name,
                          g_unk00aca8fc[world->level_idx].names[j]) == 0)
              goto collected;
          }
        }
        goto check;
      collected:
        pickup->state |= 0x40;
      check:
        if (!(pickup->state & 0x40)) {
          void *counter =
              SuperCounter_FindFromNameAndLevel(pickup->name, world, NULL);
          if (counter != NULL && SuperCounter_AnyCollected(counter, world))
            pickup->state |= 0x40;
        }
      }
      if (GizmoPickupSys->type_d != -1 &&
          GizmoPickupSys->type_d == pickup->type_index) {
        if (SuperStory != 0 || (g_unk00aca594 != NULL &&
                                (g_unk00aca594[world->i124 * 12 + 7] & 1)))
          pickup->state |= 0x100;
      }
    }
  }
  if (world->p5220->temp_list != NULL)
    memset(world->p5220->temp_list, 0, 0xb00);
}

// FUNCTION: LEGOBATMAN 0x00634ee0
nuvec_s *GizmoPickup_GetPos(GIZMO *gizmo) {
  if (gizmo == NULL)
    return NULL;
  return &((Unk_WorldInfo5220Entry *)gizmo->object)->position;
}

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
