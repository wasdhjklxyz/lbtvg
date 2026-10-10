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
void GizmoPickup_Activate(GIZMO *gizmo, i32 active);
void GizmoPickup_SetVisibility(GIZMO *gizmo, i32 visible);
nuvec_s *GizmoPickup_GetPos(GIZMO *gizmo);
void GizmoPickups_Reset(void *world, void *unused, void *progress);
void *GizmoPickups_ReserveBufferSpace(void *world);
i32 GizmoPickups_Load(void *world, void *unused);
void GizmoPickups_PostLoad(void *a, void *b);

// GLOBAL: LEGOBATMAN 0x009652c4
i32 pickup_gizmotype_id = -1;

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
