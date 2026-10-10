// batman/unk_0048eee0.cpp: TU of unknown name, found by its header-static
// copies (the functions after them are not matched yet).

#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"

// FUNCTION: LEGOBATMAN 0x0048eee0
static f32 NuSinApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x0048efa0
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_0048eee0(f32 *v, f32 a, i32 i) {
  v[0] = NuSinApprox(i);
  NuVec4Set(v, a, a, a, a);
}

// The rest of this TU is the Mac's GizPanel file (GizPanel_Load ..
// GizPanel_MoveCode); callbacks named by their GizPanel_RegisterGizmo slot.

#include "leveldata_unk.h"
#include "worldinfo_unk.h"
#include <stddef.h>

typedef struct GIZPANEL_s {
  u8 pad0[0x40];
  char name[0x22]; // 0x40
  u8 b62_0 : 1;    // 0x62
  u8 output : 1;   // 0x62 bit 1
  u8 b62_2 : 1;    // 0x62 bit 2
  u8 b62_3 : 1;    // 0x62 bit 3
  u8 pad63[0x94 - 0x63];
} GIZPANEL;

typedef struct GIZPANELSYS_s {
  i32 count;        // 0x00
  GIZPANEL *panels; // 0x04
} GIZPANELSYS;

i32 NuStrLen(const char *s);
void AddGizmo(GIZMOSYS_s *gizmo_sys, i32 type_id, void *a, void *object);

typedef struct GIZMO_s {
  void *object;
} GIZMO;

// FUNCTION: LEGOBATMAN 0x004906d0
i32 GizPanels_GetMaxGizmos(void *world_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  if (world != NULL)
    return world->current_level->max_gizpanels;
  return 0;
}

// FUNCTION: LEGOBATMAN 0x004906f0
void GizPanels_AddGizmos(GIZMOSYS_s *gizmo_sys, i32 type_id, void *world_ptr,
                         void *unused) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  if (world == NULL || world->gizpanel_sys == NULL)
    return;
  for (i32 i = 0; i < world->gizpanel_sys->count; i++) {
    if (NuStrLen(world->gizpanel_sys->panels[i].name) != 0)
      AddGizmo(gizmo_sys, type_id, NULL, &world->gizpanel_sys->panels[i]);
  }
}

// FUNCTION: LEGOBATMAN 0x00490780
char *GizPanel_GetGizmoName(GIZMO *gizmo) {
  GIZPANEL *panel;
  if (gizmo != NULL && (panel = (GIZPANEL *)gizmo->object) != NULL)
    return panel->name;
  return NULL;
}

// FUNCTION: LEGOBATMAN 0x004907b0
i32 GizPanel_GetOutput(GIZMO *gizmo, i32 a, i32 b) {
  GIZPANEL *panel = (GIZPANEL *)gizmo->object;
  if (panel->b62_3 && panel->b62_2)
    return panel->output;
  return 0;
}

// FUNCTION: LEGOBATMAN 0x004907e0
char *GizPanel_GetOutputName(GIZMO *gizmo, i32 output_index) {
  return output_index == 0 ? "Finished" : NULL;
}

// FUNCTION: LEGOBATMAN 0x00490800
i32 GizPanel_GetNumOutputs(GIZMO *gizmo) { return 1; }

void *GameBufferAllocProgressUnk005bbaf0(VARIPTR *buf, VARIPTR *buf_end,
                                         i32 size);

// FUNCTION: LEGOBATMAN 0x00490810
void *GizPanels_AllocateProgressData(VARIPTR *buf, VARIPTR *buf_end) {
  return GameBufferAllocProgressUnk005bbaf0(buf, buf_end, 0xc);
}

// FUNCTION: LEGOBATMAN 0x00490830
void GizPanels_ClearProgress(void *world, void *progress_ptr) {
  i32 *progress = (i32 *)progress_ptr;
  if (progress != NULL) {
    progress[0] = 0;
    progress[1] = -1;
    progress[2] = -1;
  }
}

typedef struct ADDGIZMOTYPE_s {
  char *name;        // 0x00
  char *prefix;      // 0x04
  u16 progress_size; // 0x08
  void *fns[0x1c];   // 0x0c
} ADDGIZMOTYPE;

// GLOBAL: LEGOBATMAN 0x00960118
extern ADDGIZMOTYPE Default_ADDGIZMOTYPE;

void GizPanel_Update(void *world, void *unused, f32 dt);
void GizPanels_Draw(void *world, void *unused, f32 dt);
void GizPanel_Activate(GIZMO *gizmo, i32 active);
void GizPanel_SetVisibility(GIZMO *gizmo, i32 visible);
void GizPanels_StoreProgress(void *world, void *unused, void *progress);
void GizPanels_Reset(void *world, void *unused, void *progress);
void *GizPanels_ReserveBufferSpace(void *world);
i32 GizPanels_Load(void *world, void *unused);
void GizPanels_AddLevelSfx(void *world, void *unused, i32 *sfx_ids,
                           i32 *sfx_count, i32 max_sfx);

// GLOBAL: LEGOBATMAN 0x0093e710
i32 panel_gizmotype_id = -1;

// FUNCTION: LEGOBATMAN 0x00491540
ADDGIZMOTYPE *GizPanel_RegisterGizmo(i32 type_id) {
  // GLOBAL: LEGOBATMAN 0x009c8f50
  static ADDGIZMOTYPE addtype;

  addtype = Default_ADDGIZMOTYPE;
  addtype.fns[2] = NULL;
  addtype.fns[5] = NULL;
  addtype.fns[11] = NULL;
  addtype.fns[13] = NULL;
  addtype.fns[14] = NULL;
  addtype.fns[15] = NULL;
  addtype.fns[16] = NULL;
  addtype.fns[17] = NULL;
  addtype.fns[18] = NULL;
  addtype.fns[25] = NULL;
  addtype.fns[26] = NULL;
  panel_gizmotype_id = type_id;
  addtype.name = "Panel";
  addtype.prefix = "";
  addtype.progress_size = 0xc;
  addtype.fns[0] = (void *)GizPanels_GetMaxGizmos;
  addtype.fns[1] = (void *)GizPanels_AddGizmos;
  addtype.fns[3] = (void *)GizPanel_Update;
  addtype.fns[4] = (void *)GizPanels_Draw;
  addtype.fns[6] = (void *)GizPanel_GetGizmoName;
  addtype.fns[7] = (void *)GizPanel_GetOutput;
  addtype.fns[8] = (void *)GizPanel_GetOutputName;
  addtype.fns[9] = (void *)GizPanel_GetNumOutputs;
  addtype.fns[10] = (void *)GizPanel_Activate;
  addtype.fns[12] = (void *)GizPanel_SetVisibility;
  addtype.fns[19] = (void *)GizPanels_AllocateProgressData;
  addtype.fns[20] = (void *)GizPanels_ClearProgress;
  addtype.fns[21] = (void *)GizPanels_StoreProgress;
  addtype.fns[22] = (void *)GizPanels_Reset;
  addtype.fns[23] = (void *)GizPanels_ReserveBufferSpace;
  addtype.fns[24] = (void *)GizPanels_Load;
  addtype.fns[27] = (void *)GizPanels_AddLevelSfx;
  return &addtype;
}
