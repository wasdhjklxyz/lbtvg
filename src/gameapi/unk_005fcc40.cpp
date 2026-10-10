// gameapi/unk_005fcc40.cpp: TU of unknown name, found by its header-static
// copies (the functions after them are not matched yet).

#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"

// FUNCTION: LEGOBATMAN 0x005fcc40
static f32 NuFabs(f32 f);
// FUNCTION: LEGOBATMAN 0x005fcc60
static f32 NuFsign(f32 f);
// FUNCTION: LEGOBATMAN 0x005fcdf0
static f32 NuSinApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x005fce90
static f32 NuCosApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x005fcea0
static void NuVec4Copy(f32 *dst, f32 *src);
// FUNCTION: LEGOBATMAN 0x005fcec0
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_005fcc40(f32 *v, f32 a, i32 i) {
  v[2] = NuFabs(a);
  v[5] = NuFsign(a);
  v[0] = NuSinApprox(i);
  v[1] = NuCosApprox(i);
  NuVec4Copy(v + 4, v);
  NuVec4Set(v, a, a, a, a);
}

// The rest of this TU is the Mac's ZipUp file (saga
// legoapi/props/objects/zipup.cpp, ZipUps_Load .. ZipUps_RegisterGizmo
// 0x5ff7f0); callbacks named by their RegisterGizmo slot.

#include "../batman/leveldata_unk.h"
#include "../batman/worldinfo_unk.h"
#include "../nu2api/numath/numtx.h"
#include "gameobject_unk.h"
#include <stddef.h>

typedef struct ZIPUP_s {
  numtx_s lower_mtx;      // 0x00
  numtx_s upper_mtx;      // 0x40
  char name[0x10];        // 0x80
  nuvec_s lower_position; // 0x90
  nuvec_s hook_origin;    // 0x9c
  nuvec_s upper_position; // 0xa8
  u16 hook_x_rotation;    // 0xb4
  u16 hook_y_rotation;    // 0xb6
  nuvec_s hook_position;  // 0xb8
  u8 padc4[0xdc - 0xc4];
  GameObject_s *occupant; // 0xdc
  u8 pade0[0xe2 - 0xe0];
  u16 direction;    // 0xe2
  u16 facing_angle; // 0xe4
  u8 pade6[0xe8 - 0xe6];
  u16 config0 : 1;
  u16 config1 : 1;
  u16 config2 : 1;
  u16 has_upper : 1; // 0xe8 bit 3
  u16 config4 : 1;
  u16 config5 : 1;
  u16 active : 1;  // 0xe8 bit 6
  u16 visible : 1; // 0xe8 bit 7
  u16 in_use : 1;  // 0xe8 bit 8
  u8 pade9[0xec - 0xea];
  f32 lower_ground_height; // 0xec
  f32 upper_ground_height; // 0xf0
  u16 lower_x_rotation;    // 0xf4
  u16 upper_x_rotation;    // 0xf6
  u16 lower_z_rotation;    // 0xf8
  u16 upper_z_rotation;    // 0xfa
  i16 lower_sfx;           // 0xfc
  i16 upper_sfx;           // 0xfe
  u8 lower_b100;           // 0x100
  u8 upper_b101;           // 0x101
  u8 pad102[0x104 - 0x102];
} ZIPUP;

typedef struct ZIPUPPROGRESS_s {
  u32 active[1];  // 0x00
  u32 visible[1]; // 0x04
} ZIPUPPROGRESS;

typedef struct ZIPUPSTATE_s {
  u8 pad0[0x0c];
  nuvec_s hook_offset; // 0x0c
  u8 pad18[0x30 - 0x18];
  u32 flags; // 0x30
  i16 sfx;   // 0x34
} ZIPUPSTATE;

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

i32 NuStrLen(const char *s);
void AddGizmo(GIZMOSYS_s *gizmo_sys, i32 type_id, void *a, void *object);
void *GameBufferAllocProgressUnk005bbaf0(VARIPTR *buf, VARIPTR *buf_end,
                                         i32 size);
i32 Unk005c2ed0(i32 a, i32 b, i32 c, i32 d, i32 e, i32 f);
i16 Unk00573e30(numtx_s *mtx, i32 sfx);
void Unk00574270(i32 handle);
void NuVecRotateX(nuvec_s *v, nuvec_s *v0, i32 a);
void NuVecRotateY(nuvec_s *v, nuvec_s *v0, i32 a);
void NuVecAdd(nuvec_s *out, nuvec_s *a, nuvec_s *b);
void NuMtxSetRotationY(numtx_s *m, i32 a);
void NuMtxTranslate(numtx_s *m, nuvec_s *v);
i32 NuAtan2D(f32 dx, f32 dy);
extern "C" void NewTerrPlatformsOff(void);
f32 GameShadow(GameObject_s *object, nuvec_s *position, f32 probe_height,
               i32 terrain_mask);
void FindAnglesZX(nuvec_s *normal, u16 *x_rotation, u16 *z_rotation);

// GLOBAL: LEGOBATMAN 0x009ca98c
extern f32 g_unk009ca98c;
// GLOBAL: LEGOBATMAN 0x009f7a44
extern nuvec_s ShadNorm;

// GLOBAL: LEGOBATMAN 0x00962aac
extern ZIPUPSTATE *ZipUp_State;

// GLOBAL: LEGOBATMAN 0x00962ab8
static char *ZipUp_OutputNames[2] = {"Active", "InUse"};

// FUNCTION: LEGOBATMAN 0x005fd040
static void ZipUp_RestartSfx(ZIPUP *zipup) {
  if (zipup->lower_sfx != -1) {
    Unk00574270(zipup->lower_sfx);
    zipup->lower_sfx = -1;
  }
  if (zipup->upper_sfx != -1) {
    Unk00574270(zipup->upper_sfx);
    zipup->upper_sfx = -1;
  }
  if (zipup->config5 && ZipUp_State->sfx != -1) {
    if (zipup->lower_b100 != 2 && zipup->lower_sfx == -1)
      zipup->lower_sfx = Unk00573e30(&zipup->lower_mtx, ZipUp_State->sfx);
    if (zipup->has_upper && zipup->upper_b101 != 2 && zipup->upper_sfx == -1)
      zipup->upper_sfx = Unk00573e30(&zipup->upper_mtx, ZipUp_State->sfx);
  }
}

// FUNCTION: LEGOBATMAN 0x005fd100
i32 ZipUp_ActivateRev(GIZMO *gizmo, i32 value, i32 query) {
  if (gizmo == NULL)
    return 0;
  ZIPUP *zipup = (ZIPUP *)gizmo->object;
  if (zipup == NULL)
    return 0;
  if (query & 1)
    return value != zipup->direction;
  if (value) {
    zipup->direction = 1;
    zipup->active = 0;
    return 1;
  }
  zipup->active = 1;
  zipup->direction = 0;
  return 1;
}

// FUNCTION: LEGOBATMAN 0x005fd440
void ZipUps_InitTerrain(WORLDINFO_s *world) {
  for (i32 i = 0; i < world->zipup_count; i++)
    ZipUp_RestartSfx(&world->zipups[i]);
}

// FUNCTION: LEGOBATMAN 0x005fd490
void *ZipUps_ReserveBufferSpace(void *world_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  world->zipups = NULL;
  world->zipup_count = 0;
  if (world->current_level->max_zipups > 0) {
    world->buf104.addr = (world->buf104.addr + 3) & ~3;
    world->zipups = (ZIPUP *)world->buf104.void_ptr;
    world->buf104.addr += world->current_level->max_zipups * sizeof(ZIPUP);
  }
  return world->zipups;
}

// FUNCTION: LEGOBATMAN 0x005fd4e0
static void ZipUp_ResetUnk005fd4e0(ZIPUP *zipup) {
  NuVecRotateX(&zipup->hook_position, &ZipUp_State->hook_offset,
               zipup->hook_x_rotation);
  NuVecRotateY(&zipup->hook_position, &zipup->hook_position,
               zipup->hook_y_rotation);
  NuVecAdd(&zipup->hook_position, &zipup->hook_origin, &zipup->hook_position);
  if (zipup->config5 && ZipUp_State->sfx != -1) {
    zipup->lower_ground_height = zipup->lower_position.y;
    zipup->lower_z_rotation = 0;
    zipup->lower_x_rotation = 0;
    zipup->upper_ground_height = zipup->upper_position.y;
    zipup->upper_z_rotation = 0;
    zipup->upper_x_rotation = 0;
  } else {
    NewTerrPlatformsOff();
    zipup->lower_ground_height =
        GameShadow(NULL, &zipup->lower_position, g_unk009ca98c, -1);
    if (zipup->lower_ground_height != 2000000.0) {
      zipup->lower_ground_height += 0.005f;
      FindAnglesZX(&ShadNorm, &zipup->lower_x_rotation,
                   &zipup->lower_z_rotation);
    }
    NewTerrPlatformsOff();
    zipup->upper_ground_height =
        GameShadow(NULL, &zipup->upper_position, g_unk009ca98c, -1);
    if (zipup->upper_ground_height != 2000000.0) {
      zipup->upper_ground_height += 0.005f;
      FindAnglesZX(&ShadNorm, &zipup->upper_x_rotation,
                   &zipup->upper_z_rotation);
    }
  }
  zipup->facing_angle =
      NuAtan2D(zipup->upper_position.x - zipup->lower_position.x,
               zipup->upper_position.z - zipup->lower_position.z);
  zipup->in_use = 0;
  zipup->occupant = NULL;
  zipup->active = 1;
  zipup->visible = 1;
  NuMtxSetRotationY(&zipup->lower_mtx, zipup->facing_angle);
  NuMtxTranslate(&zipup->lower_mtx, &zipup->lower_position);
  NuMtxSetRotationY(&zipup->upper_mtx, zipup->facing_angle + 0x8000);
  NuMtxTranslate(&zipup->upper_mtx, &zipup->upper_position);
}

// FUNCTION: LEGOBATMAN 0x005fd6e0
void ZipUps_Reset(void *world_ptr, void *unused, void *progress_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  ZIPUPPROGRESS *progress = (ZIPUPPROGRESS *)progress_ptr;
  if (world == NULL)
    return;
  ZIPUP *zipup = world->zipups;
  if (zipup == NULL)
    return;
  for (i32 i = 0; i < g_unk00960894->zipup_count; i++, zipup++) {
    ZipUp_ResetUnk005fd4e0(zipup);
    if (progress != NULL && i < 32) {
      i32 word = i / 32;
      u32 bit = 1 << (i & 31);
      zipup->visible = (progress->visible[word] & bit) != 0;
      zipup->active = (progress->active[word] & bit) != 0;
    }
  }
}

// FUNCTION: LEGOBATMAN 0x005feb80
void ZipUps_Update(void *world_ptr, void *unused, f32 dt) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  if (world == NULL)
    return;
  ZipUp_State->flags &= ~2;
  ZIPUP *zipup = world->zipups;
  if (zipup == NULL)
    return;
  if (Unk005c2ed0(0x100000, 0, 0, -1, 0, 0))
    ZipUp_State->flags |= 2;
  for (i32 i = 0; i < world->zipup_count; i++, zipup++) {
    if (zipup->occupant != NULL && zipup->occupant->b9db == -1) {
      zipup->occupant->flags9e4 &= ~0x20;
      zipup->in_use = 0;
      zipup->occupant->techno = NULL;
      zipup->occupant = NULL;
    }
  }
}

// GLOBAL: LEGOBATMAN 0x0096055c
extern i32 g_unk0096055c; // the zipup special move

// FUNCTION: LEGOBATMAN 0x005fed80
void ZipUp_Release(GameObject_s *obj) {
  if (obj->b9db == g_unk0096055c && obj->techno != NULL) {
    ((ZIPUP *)obj->techno)->in_use = 0;
    obj->techno = NULL;
  }
}

void ZipUps_Draw(void *world, void *unused, f32 dt);

// FUNCTION: LEGOBATMAN 0x005ff4c0
void ZipUp_Activate(GIZMO *gizmo, i32 active) {
  ZIPUP *zipup = (ZIPUP *)gizmo->object;
  if (active) {
    zipup->direction = 0;
    zipup->active = 1;
  } else {
    zipup->active = 0;
  }
}

// FUNCTION: LEGOBATMAN 0x005ff4f0
static void ZipUp_SetVisible(ZIPUP *zipup, i32 visible) {
  zipup->visible = visible != 0;
  if (zipup->visible) {
    if (ZipUp_State->sfx == -1)
      return;
    if (zipup->lower_sfx == -1 && zipup->lower_b100 != 2)
      zipup->lower_sfx = Unk00573e30(&zipup->lower_mtx, ZipUp_State->sfx);
    if (zipup->has_upper && zipup->upper_sfx == -1 && zipup->upper_b101 != 2)
      zipup->upper_sfx = Unk00573e30(&zipup->upper_mtx, ZipUp_State->sfx);
  } else {
    if (zipup->lower_sfx != -1) {
      Unk00574270(zipup->lower_sfx);
      zipup->lower_sfx = -1;
    }
    if (zipup->upper_sfx != -1) {
      Unk00574270(zipup->upper_sfx);
      zipup->upper_sfx = -1;
    }
  }
}

// FUNCTION: LEGOBATMAN 0x005ff5e0
void ZipUp_SetVisibility(GIZMO *gizmo, i32 visible) {
  if (gizmo != NULL) {
    ZIPUP *zipup = (ZIPUP *)gizmo->object;
    if (zipup != NULL)
      ZipUp_SetVisible(zipup, visible);
  }
}

// FUNCTION: LEGOBATMAN 0x005ff600
i32 ZipUps_GetMaxGizmos(void *world_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  if (world != NULL)
    return world->current_level->max_zipups;
  return 0;
}

// FUNCTION: LEGOBATMAN 0x005ff620
void ZipUps_AddGizmos(GIZMOSYS_s *gizmo_sys, i32 type_id, void *world_ptr,
                      void *unused) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  for (i32 i = 0; i < world->zipup_count; i++) {
    if (NuStrLen(world->zipups[i].name) != 0)
      AddGizmo(gizmo_sys, type_id, NULL, &world->zipups[i]);
  }
}

// FUNCTION: LEGOBATMAN 0x005ff690
char *ZipUp_GetGizmoName(GIZMO *gizmo) {
  return gizmo != NULL ? ((ZIPUP *)gizmo->object)->name : NULL;
}

// FUNCTION: LEGOBATMAN 0x005ff6b0
void *ZipUps_AllocateProgressData(VARIPTR *buf, VARIPTR *buf_end) {
  return GameBufferAllocProgressUnk005bbaf0(buf, buf_end,
                                            sizeof(ZIPUPPROGRESS));
}

// FUNCTION: LEGOBATMAN 0x005ff6d0
void ZipUps_ClearProgress(void *world, void *progress_ptr) {
  ZIPUPPROGRESS *progress = (ZIPUPPROGRESS *)progress_ptr;
  if (progress != NULL) {
    progress->active[0] = 0xffffffff;
    progress->visible[0] = 0xffffffff;
  }
}

// FUNCTION: LEGOBATMAN 0x005ff6f0
void ZipUps_StoreProgress(void *world_ptr, void *unused, void *progress_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  ZIPUPPROGRESS *progress = (ZIPUPPROGRESS *)progress_ptr;
  if (progress == NULL)
    return;
  ZipUps_ClearProgress(NULL, progress);
  if (world != NULL && world->zipups != NULL) {
    ZIPUP *zipup = world->zipups;
    for (i32 i = 0; i < world->zipup_count; i++, zipup++) {
      if (i >= 32)
        break;
      i32 word = i / 32;
      u32 bit = 1 << (i & 31);
      if (!zipup->visible)
        progress->visible[word] &= ~bit;
      if (!zipup->active)
        progress->active[word] &= ~bit;
    }
  }
}

// FUNCTION: LEGOBATMAN 0x005ff780
char *ZipUp_GetOutputName(GIZMO *gizmo, i32 output) {
  if ((u32)output <= 1)
    return ZipUp_OutputNames[output];
  return NULL;
}

// FUNCTION: LEGOBATMAN 0x005ff7a0
i32 ZipUp_GetNumOutputs(GIZMO *gizmo) { return 2; }

// FUNCTION: LEGOBATMAN 0x005ff7b0
i32 ZipUp_GetOutput(GIZMO *gizmo, i32 output, i32 b) {
  if (gizmo != NULL) {
    ZIPUP *zipup = (ZIPUP *)gizmo->object;
    switch (output) {
    case 0:
      if (zipup->active)
        return 1;
      break;
    case 1:
      if (zipup->in_use)
        return 1;
      break;
    }
  }
  return 0;
}

i32 ZipUps_Load(void *world, void *unused);

// GLOBAL: LEGOBATMAN 0x00962ab0
i32 zipup_gizmotype_id = -1;

// FUNCTION: LEGOBATMAN 0x005ff7f0
ADDGIZMOTYPE *ZipUps_RegisterGizmo(i32 type_id) {
  // GLOBAL: LEGOBATMAN 0x00962b6c
  static char *name = "ZipUp";
  // GLOBAL: LEGOBATMAN 0x00aca5b8
  static ADDGIZMOTYPE addtype;

  addtype = Default_ADDGIZMOTYPE;
  addtype.name = name;
  addtype.prefix = "";
  addtype.progress_size = sizeof(ZIPUPPROGRESS);
  addtype.fns[0] = (void *)ZipUps_GetMaxGizmos;
  addtype.fns[1] = (void *)ZipUps_AddGizmos;
  addtype.fns[2] = NULL;
  addtype.fns[3] = (void *)ZipUps_Update;
  addtype.fns[4] = (void *)ZipUps_Draw;
  addtype.fns[5] = NULL;
  addtype.fns[6] = (void *)ZipUp_GetGizmoName;
  addtype.fns[7] = (void *)ZipUp_GetOutput;
  addtype.fns[8] = (void *)ZipUp_GetOutputName;
  addtype.fns[9] = (void *)ZipUp_GetNumOutputs;
  addtype.fns[10] = (void *)ZipUp_Activate;
  addtype.fns[11] = (void *)ZipUp_ActivateRev;
  addtype.fns[12] = (void *)ZipUp_SetVisibility;
  addtype.fns[13] = NULL;
  addtype.fns[14] = NULL;
  addtype.fns[15] = NULL;
  addtype.fns[16] = NULL;
  addtype.fns[17] = NULL;
  addtype.fns[18] = NULL;
  addtype.fns[19] = (void *)ZipUps_AllocateProgressData;
  addtype.fns[20] = (void *)ZipUps_ClearProgress;
  addtype.fns[21] = (void *)ZipUps_StoreProgress;
  addtype.fns[22] = (void *)ZipUps_Reset;
  addtype.fns[23] = (void *)ZipUps_ReserveBufferSpace;
  addtype.fns[24] = (void *)ZipUps_Load;
  addtype.fns[25] = NULL;
  addtype.fns[26] = NULL;
  addtype.fns[27] = NULL;
  zipup_gizmotype_id = type_id;
  return &addtype;
}
