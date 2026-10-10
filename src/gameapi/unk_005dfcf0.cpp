// gameapi/unk_005dfcf0.cpp: TU of unknown name, found by its header-static
// copies (the functions after them are not matched yet).

#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"

// FUNCTION: LEGOBATMAN 0x005dfcf0
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_005dfcf0(f32 *v, f32 a, i32 i) { NuVec4Set(v, a, a, a, a); }

// The rest of this TU is the Mac's Ledge file (Ledges_Load ..
// Ledges_RegisterGizmo 0x5e0750, then the Ledge_* move code); callbacks
// named by their RegisterGizmo slot.

#include "../batman/leveldata_unk.h"
#include "../batman/worldinfo_unk.h"
#include "../nu2api/numath/numtx.h"
#include <stddef.h>
#include <string.h>

struct nuhspecial_s {
  void *scene;
  void *special;
  void *display_special;
};

typedef struct LEDGETYPE_s {
  char *name; // 0x00
  u8 pad4[0xa - 0x4];
  char code; // 0x0a
  u8 padb;
  nuvec_s v0c; // 0x0c
  nuvec_s v18; // 0x18
  u8 pad24[0x28 - 0x24];
  nuhspecial_s special;     // 0x28
  nuhspecial_s hot_special; // 0x34
} LEDGETYPE;

typedef struct LEDGE_s {
  char name[8];           // 0x00
  nuvec_s position;       // 0x08
  nuvec_s min;            // 0x14
  nuvec_s max;            // 0x20
  nuhspecial_s special;   // 0x2c, the ledge rides on this special
  nuvec_s local_position; // 0x38
  u16 angle;              // 0x44
  char code;              // 0x46
  u8 active : 1;          // 0x47
  u8 visible : 1;         // 0x47 bit 1
  u8 type;                // 0x48
  u8 hot : 1;             // 0x49
  u8 pad4a[0x4e - 0x4a];
  u16 local_angle; // 0x4e
  u16 turn;        // 0x50
  u8 pad52[0x54 - 0x52];
} LEDGE;

typedef struct LEDGEPROGRESS_s {
  u32 active[8];  // 0x00
  u32 visible[8]; // 0x20
} LEDGEPROGRESS;

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
// GLOBAL: LEGOBATMAN 0x00961398
extern LEDGETYPE LedgeTypes[8];
// GLOBAL: LEGOBATMAN 0x0096048c
extern i32 g_unk0096048c;
// GLOBAL: LEGOBATMAN 0x00ab3960
extern GameObject_s *g_unk00ab3960[8];

i32 NuStrLen(const char *s);
void AddGizmo(GIZMOSYS_s *gizmo_sys, i32 type_id, void *a, void *object);
void *GameBufferAlloc(variptr_u *buf, variptr_u *buf_end, i32 size);
void *GameBufferAllocProgressUnk005bbaf0(VARIPTR *buf, VARIPTR *buf_end,
                                         i32 size);
void NuMtxSetRotationY(numtx_s *m, i32 a);
void NuMtxTranslate(numtx_s *m, nuvec_s *v);
int NuSpecialExistsFn(nuhspecial_s *sp);
int NuSpecialDrawAt(nuhspecial_s *sp, numtx_s *mtx);
numtx_s *NuSpecialGetDrawMtx(nuhspecial_s *sp);
void NuMtxMul(numtx_s *m, numtx_s *m0, numtx_s *m1);
void NuVecMtxRotate(nuvec_s *out, nuvec_s *v, numtx_s *m);
i32 NuAtan2D(f32 dx, f32 dy);
i32 RotDiff(u16 current, u16 target);
float NuVecDistSqr(nuvec_s *a, nuvec_s *b, nuvec_s *d);

struct Unk_GameObject1144 {
  u8 pad0[0x14];
  u8 flags14; // 0x14
};

// GLOBAL: LEGOBATMAN 0x0095fd2c
extern nuvec_s v001;
void NuVecRotateY(nuvec_s *v, nuvec_s *v0, i32 a);
void NuVecAdd(nuvec_s *out, nuvec_s *a, nuvec_s *b);
i32 NuStrCpy(char *dst, const char *src);
void NuStrCat(char *dst, const char *src);
extern "C" void NuMemCpy(void *dst, void *src, i32 n);

// GLOBAL: LEGOBATMAN 0x00a958c8
extern nugscn_s *area_scene;
// GLOBAL: LEGOBATMAN 0x00a958ac
extern nugscn_s *things_scene;

// GLOBAL: LEGOBATMAN 0x00961598
static char *Ledge_OutputNames[2] = {"CanUse", "Occupied"};

i32 Ledges_Load(void *world, void *unused);

// FUNCTION: LEGOBATMAN 0x005dfec0
void *Ledges_ReserveBufferSpace(void *world_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  world->ledges = NULL;
  world->ledge_count = 0;
  if (world->current_level->max_ledges > 0) {
    world->ledges = (LEDGE *)GameBufferAlloc(&world->buf104, &world->bufEnd108,
                                             world->current_level->max_ledges *
                                                 sizeof(LEDGE));
  }
  return world->ledges;
}

// FUNCTION: LEGOBATMAN 0x005dff10
static void Ledge_Reset(LEDGE *ledge) {
  nuvec_s v;
  i32 i;
  for (i = 0; i < 8; i++) {
    if (ledge->code == LedgeTypes[i].code) {
      ledge->type = i;
      break;
    }
  }
  if (i == 8) {
    ledge->type = 2;
    ledge->code = LedgeTypes[2].code;
  }
  ledge->min.x = ledge->min.y = ledge->min.z = 999.0f;
  ledge->max.x = ledge->max.y = ledge->max.z = -999.0f;
  for (i = 0; i < 2; i++) {
    NuVecRotateY(&v,
                 i == 0 ? &LedgeTypes[ledge->type].v0c
                        : &LedgeTypes[ledge->type].v18,
                 ledge->angle);
    if (v.x < ledge->min.x)
      ledge->min.x = v.x;
    if (v.x > ledge->max.x)
      ledge->max.x = v.x;
    if (v.y < ledge->min.y)
      ledge->min.y = v.y;
    if (v.y > ledge->max.y)
      ledge->max.y = v.y;
    if (v.z < ledge->min.z)
      ledge->min.z = v.z;
    if (v.z > ledge->max.z)
      ledge->max.z = v.z;
  }
  ledge->min.x -= 0.05f;
  ledge->min.y -= 0.05f;
  ledge->min.z -= 0.05f;
  ledge->max.x += 0.05f;
  ledge->max.y += 0.05f;
  ledge->max.z += 0.05f;
  NuVecAdd(&ledge->min, &ledge->min, &ledge->position);
  NuVecAdd(&ledge->max, &ledge->max, &ledge->position);
  ledge->active = 1;
  ledge->visible = 1;
  ledge->turn = 0;
}

// FUNCTION: LEGOBATMAN 0x005e0080
void Ledge_SetVisibility(GIZMO *gizmo, i32 visible) {
  if (gizmo != NULL)
    ((LEDGE *)gizmo->object)->visible = visible != 0;
}

// FUNCTION: LEGOBATMAN 0x005e00a0
void Ledge_Activate(GIZMO *gizmo, i32 active) {
  if (gizmo != NULL)
    ((LEDGE *)gizmo->object)->active = active != 0;
}

// FUNCTION: LEGOBATMAN 0x005e00c0
void Ledges_Reset(void *world_ptr, void *unused, void *progress_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  LEDGEPROGRESS *progress = (LEDGEPROGRESS *)progress_ptr;
  char name[64];
  if (world == NULL)
    return;
  for (i32 i = 0; i < 8; i++) {
    LEDGETYPE *type = &LedgeTypes[i];
    memset(&type->special, 0, sizeof(nuhspecial_s));
    if ((world->scn140 == NULL ||
         !NuSpecialFind(world->scn140, &type->special, type->name, 1)) &&
        (area_scene == NULL ||
         !NuSpecialFind(area_scene, &type->special, type->name, 1)) &&
        things_scene != NULL)
      NuSpecialFind(things_scene, &type->special, type->name, 1);
    NuMemCpy(&type->hot_special, &type->special, sizeof(nuhspecial_s));
    NuStrCpy(name, type->name);
    NuStrCat(name, "_hot");
    if ((world->scn140 == NULL ||
         !NuSpecialFind(world->scn140, &type->hot_special, name, 1)) &&
        (area_scene == NULL ||
         !NuSpecialFind(area_scene, &type->hot_special, name, 1)) &&
        things_scene != NULL)
      NuSpecialFind(things_scene, &type->hot_special, name, 1);
  }
  LEDGE *ledge = world->ledges;
  if (ledge != NULL) {
    for (i32 i = 0; i < world->ledge_count; i++, ledge++) {
      Ledge_Reset(ledge);
      if (progress != NULL && i < 256) {
        i32 word = i / 32;
        u32 bit = 1 << (i & 31);
        ledge->visible = (progress->visible[word] & bit) != 0;
        ledge->active = (progress->active[word] & bit) != 0;
      }
    }
  }
}

// STUB: LEGOBATMAN 0x005e0260
// The original keeps `best` in world's dead home slot and the rounding temp
// in a local; ours swaps the two.
LEDGE *Ledge_FindNearest(WORLDINFO_s *world, nuvec_s *pos, GameObject_s *obj,
                         f32 *dist) {
  LEDGE *nearest = NULL;
  f32 best = 1000000000.0f;
  LEDGE *ledge = world->ledges;
  for (i32 i = 0; i < world->ledge_count; i++, ledge++) {
    f32 d;
    if (obj != NULL) {
      if (!ledge->visible || !ledge->active)
        continue;
      if (ledge->hot && (obj->p1144 == NULL || !(obj->p1144->flags14 & 1)))
        continue;
      d = NuVecDistSqr(pos, &ledge->position, NULL);
    } else {
      d = NuVecDistSqr(pos, &ledge->position, NULL);
    }
    if (d < best) {
      best = d;
      nearest = ledge;
    }
  }
  if (dist != NULL)
    *dist = best;
  return nearest;
}

// FUNCTION: LEGOBATMAN 0x005e0320
void Ledges_Update(void *world_ptr, void *unused, f32 dt) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  numtx_s mtx;
  nuvec_s dir;
  if (world->ledges == NULL)
    return;
  LEDGE *ledge = world->ledges;
  for (i32 i = 0; i < world->ledge_count; i++, ledge++) {
    if (NuSpecialExistsFn(&ledge->special)) {
      u16 old_angle = ledge->angle;
      u8 visible = ledge->visible;
      u8 active = ledge->active;
      NuMtxSetRotationY(&mtx, ledge->local_angle);
      NuMtxTranslate(&mtx, &ledge->local_position);
      NuMtxMul(&mtx, &mtx, NuSpecialGetDrawMtx(&ledge->special));
      ledge->position = *(nuvec_s *)&mtx.m30;
      NuVecMtxRotate(&dir, &v001, &mtx);
      ledge->angle = NuAtan2D(dir.x, dir.z);
      Ledge_Reset(ledge);
      ledge->visible = visible;
      ledge->active = active;
      ledge->turn = RotDiff(old_angle, ledge->angle);
    }
  }
}

// FUNCTION: LEGOBATMAN 0x005e0450
void Ledges_Draw(void *world_ptr, void *unused, f32 dt) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  numtx_s mtx;
  LEDGE *ledge = world->ledges;
  if (ledge == NULL)
    return;
  for (i32 i = 0; i < world->ledge_count; i++, ledge++) {
    if (ledge->visible) {
      nuhspecial_s *special;
      if (ledge->hot)
        special = &LedgeTypes[ledge->type].hot_special;
      else
        special = &LedgeTypes[ledge->type].special;
      if (NuSpecialExistsFn(special)) {
        NuMtxSetRotationY(&mtx, ledge->angle);
        NuMtxTranslate(&mtx, &ledge->position);
        if (ledge->hot)
          NuSpecialDrawAt(special, &mtx);
        else
          NuSpecialDrawAt(special, &mtx);
      }
    }
  }
}

// FUNCTION: LEGOBATMAN 0x005e0500
i32 Ledges_GetMaxGizmos(void *world_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  if (world != NULL)
    return world->current_level->max_ledges;
  return 0;
}

// FUNCTION: LEGOBATMAN 0x005e0520
void Ledges_AddGizmos(GIZMOSYS_s *gizmo_sys, i32 type_id, void *world_ptr,
                      void *unused) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  for (i32 i = 0; i < world->ledge_count; i++) {
    if (NuStrLen(world->ledges[i].name) != 0)
      AddGizmo(gizmo_sys, type_id, NULL, &world->ledges[i]);
  }
}

// FUNCTION: LEGOBATMAN 0x005e0590
char *Ledge_GetGizmoName(GIZMO *gizmo) {
  return gizmo != NULL ? ((LEDGE *)gizmo->object)->name : NULL;
}

// FUNCTION: LEGOBATMAN 0x005e05a0
i32 Ledge_GetNumOutputs(GIZMO *gizmo) { return 2; }

// FUNCTION: LEGOBATMAN 0x005e05b0
char *Ledge_GetOutputName(GIZMO *gizmo, i32 output) {
  if ((u32)output <= 1)
    return Ledge_OutputNames[output];
  return NULL;
}

// FUNCTION: LEGOBATMAN 0x005e05d0
i32 Ledge_GetOutput(GIZMO *gizmo, i32 output, i32 b) {
  LEDGE *ledge = (LEDGE *)gizmo->object;
  switch (output) {
  case 0:
    if (ledge->visible && ledge->active)
      return 1;
    break;
  case 1:
    if (ledge->visible && ledge->active && g_unk0096048c != -1) {
      for (i32 i = 0; i < 8; i++) {
        GameObject_s *obj = g_unk00ab3960[i];
        if (obj != NULL && obj->b9db == g_unk0096048c &&
            (LEDGE *)obj->techno == ledge)
          return 1;
      }
    }
    break;
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x005e0640
void *Ledges_AllocateProgressData(VARIPTR *buf, VARIPTR *buf_end) {
  return GameBufferAllocProgressUnk005bbaf0(buf, buf_end,
                                            sizeof(LEDGEPROGRESS));
}

// FUNCTION: LEGOBATMAN 0x005e0660
void Ledges_ClearProgress(void *world, void *progress_ptr) {
  LEDGEPROGRESS *progress = (LEDGEPROGRESS *)progress_ptr;
  if (progress != NULL) {
    memset(progress->active, 0xff, sizeof(progress->active));
    memset(progress->visible, 0xff, sizeof(progress->visible));
  }
}

// FUNCTION: LEGOBATMAN 0x005e06a0
void Ledges_StoreProgress(void *world_ptr, void *unused, void *progress_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  LEDGEPROGRESS *progress = (LEDGEPROGRESS *)progress_ptr;
  if (progress == NULL)
    return;
  Ledges_ClearProgress(NULL, progress);
  if (world != NULL && world->ledges != NULL) {
    LEDGE *ledge = world->ledges;
    for (i32 i = 0; i < world->ledge_count; i++, ledge++) {
      if (i >= 256)
        break;
      i32 word = i / 32;
      u32 bit = 1 << (i & 31);
      if (!ledge->visible)
        progress->visible[word] &= ~bit;
      if (!ledge->active)
        progress->active[word] &= ~bit;
    }
  }
}

// GLOBAL: LEGOBATMAN 0x00961394
i32 ledge_gizmotype_id = -1;

// FUNCTION: LEGOBATMAN 0x005e0750
ADDGIZMOTYPE *Ledges_RegisterGizmo(i32 type_id) {
  // GLOBAL: LEGOBATMAN 0x00961650
  static char *name = "Ledge";
  // GLOBAL: LEGOBATMAN 0x00ac7470
  static ADDGIZMOTYPE addtype;

  addtype = Default_ADDGIZMOTYPE;
  addtype.name = name;
  addtype.prefix = "";
  addtype.progress_size = sizeof(LEDGEPROGRESS);
  addtype.fns[0] = (void *)Ledges_GetMaxGizmos;
  addtype.fns[1] = (void *)Ledges_AddGizmos;
  addtype.fns[2] = (void *)Ledges_Update;
  addtype.fns[3] = NULL;
  addtype.fns[4] = (void *)Ledges_Draw;
  addtype.fns[5] = NULL;
  addtype.fns[6] = (void *)Ledge_GetGizmoName;
  addtype.fns[7] = (void *)Ledge_GetOutput;
  addtype.fns[8] = (void *)Ledge_GetOutputName;
  addtype.fns[9] = (void *)Ledge_GetNumOutputs;
  addtype.fns[10] = (void *)Ledge_Activate;
  addtype.fns[11] = NULL;
  addtype.fns[12] = (void *)Ledge_SetVisibility;
  addtype.fns[13] = NULL;
  addtype.fns[14] = NULL;
  addtype.fns[15] = NULL;
  addtype.fns[16] = NULL;
  addtype.fns[17] = NULL;
  addtype.fns[18] = NULL;
  addtype.fns[19] = (void *)Ledges_AllocateProgressData;
  addtype.fns[20] = (void *)Ledges_ClearProgress;
  addtype.fns[21] = (void *)Ledges_StoreProgress;
  addtype.fns[22] = (void *)Ledges_Reset;
  addtype.fns[23] = (void *)Ledges_ReserveBufferSpace;
  addtype.fns[24] = (void *)Ledges_Load;
  addtype.fns[25] = NULL;
  addtype.fns[26] = NULL;
  addtype.fns[27] = NULL;
  ledge_gizmotype_id = type_id;
  return &addtype;
}
