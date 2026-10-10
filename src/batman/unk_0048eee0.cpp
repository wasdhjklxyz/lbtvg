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

#include "../nu2api/numath/nuvec.h"
#include "leveldata_unk.h"
#include "worldinfo_unk.h"
#include <stddef.h>
#include <string.h>

typedef struct GIZPANEL_s {
  u8 pad0[0x40];
  char name[0x10];  // 0x40
  nuvec_s position; // 0x50
  u16 y_rotation;   // 0x5c
  u8 pad5e[0x61 - 0x5e];
  u8 model_variant;       // 0x61
  u8 b62_0 : 1;           // 0x62
  u8 output : 1;          // 0x62 bit 1
  u8 b62_2 : 1;           // 0x62 bit 2
  u8 b62_3 : 1;           // 0x62 bit 3
  u8 b62_4 : 1;           // 0x62 bit 4
  u8 hide_base : 1;       // 0x62 bit 5
  u8 hide_target : 1;     // 0x62 bit 6
  u8 baddie : 1;          // 0x62 bit 7
  u8 alt_model : 1;       // 0x63
  u8 no_shadow_reset : 1; // 0x63 bit 1
  u8 legacy : 1;          // 0x63 bit 2
  u8 pad64[0x70 - 0x64];
  nuvec_s target_offset; // 0x70
  u8 pad7c[0x86 - 0x7c];
  i16 platform_id; // 0x86
  u8 pad88[0x8c - 0x88];
  f32 target_scale; // 0x8c
  u8 pad90[0x94 - 0x90];
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

void DeletePlatinst(i32 id);
i16 NewPlatPickupInst(void *m, i32 type);
void PlatInstRotate(i32 id, i32 rotate);

// FUNCTION: LEGOBATMAN 0x0048efd0
static void GizPanel_CreateTerrain(GIZPANEL *panel) {
  if (panel == NULL)
    return;
  if (panel->platform_id != -1)
    DeletePlatinst(panel->platform_id);
  if (panel->model_variant == 0)
    panel->platform_id = NewPlatPickupInst(panel, 7);
  PlatInstRotate(panel->platform_id, 1);
}

i32 EdFileReadInt();
u16 EdFileReadUnsignedShort();
f32 EdFileReadFloat();
char EdFileReadChar();
void EdFileRead(void *buf, i32 len);
void EdFileReadNuVec(nuvec_s *v);

// STUB: LEGOBATMAN 0x0048f120
// body matches; the original re-reads gizpanel_sys after the null check and
// shrink-wraps the esi push (nested, split and ternary forms tried).
i32 GizPanels_Load(void *world_ptr, void *unused) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  GIZPANELSYS *sys;
  if (world == NULL || world->gizpanel_sys == NULL)
    return 0;
  sys = world->gizpanel_sys;
  if (sys->count != 0)
    return 0;
  i32 version = EdFileReadInt();
  sys->count = EdFileReadInt();
  for (i32 i = 0; i < sys->count; i++) {
    EdFileRead(sys->panels[i].name, EdFileReadInt());
    EdFileReadNuVec(&sys->panels[i].position);
    sys->panels[i].y_rotation = EdFileReadUnsignedShort();
    sys->panels[i].model_variant = EdFileReadChar();
    if (version >= 3)
      sys->panels[i].hide_base = EdFileReadChar();
    if (version >= 4) {
      EdFileReadNuVec(&sys->panels[i].target_offset);
      sys->panels[i].target_scale = EdFileReadFloat();
    } else {
      sys->panels[i].target_offset.x = sys->panels[i].target_offset.y = 0.0f;
      sys->panels[i].target_offset.z = -0.4f;
      sys->panels[i].target_scale = 1.0f;
    }
    if (version >= 5)
      sys->panels[i].hide_target = EdFileReadChar();
    if (version >= 6) {
      sys->panels[i].baddie = EdFileReadChar();
      sys->panels[i].alt_model = EdFileReadChar();
    }
    if (version >= 7)
      sys->panels[i].no_shadow_reset = EdFileReadChar();
    if (version >= 8)
      sys->panels[i].legacy = EdFileReadChar() != 0;
    else
      sys->panels[i].legacy = 0;
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x0048f340
void *GizPanels_ReserveBufferSpace(void *world_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  world->gizpanel_sys = NULL;
  if (world->current_level->max_gizpanels > 0) {
    world->buf104.addr = (world->buf104.addr + 0xf) & ~0xf;
    world->gizpanel_sys = (GIZPANELSYS *)world->buf104.void_ptr;
    world->buf104.addr += sizeof(GIZPANELSYS);
    memset(world->gizpanel_sys, 0, sizeof(GIZPANELSYS));
    world->buf104.addr = (world->buf104.addr + 0xf) & ~0xf;
    world->gizpanel_sys->panels = (GIZPANEL *)world->buf104.void_ptr;
    world->buf104.addr +=
        world->current_level->max_gizpanels * sizeof(GIZPANEL);
    memset(world->gizpanel_sys->panels, 0,
           world->current_level->max_gizpanels * sizeof(GIZPANEL));
  }
  return world->gizpanel_sys;
}

// FUNCTION: LEGOBATMAN 0x0048f410
void GizPanel_SetVisibility(GIZMO *gizmo, i32 visible) {
  if (gizmo == NULL || gizmo->object == NULL)
    return;
  GIZPANEL *panel = (GIZPANEL *)gizmo->object;
  i32 was_visible = panel->b62_2;
  panel->b62_2 = visible != 0;
  if (panel->b62_2 && !was_visible)
    GizPanel_CreateTerrain(panel);
  else if (!panel->b62_2 && was_visible)
    DeletePlatinst(panel->platform_id);
}

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
void *GizPanels_ReserveBufferSpace(void *world);

// GLOBAL: LEGOBATMAN 0x0093e710
i32 panel_gizmotype_id = -1;

// STUB: LEGOBATMAN 0x00490850
// only `or eax, -1` is scheduled before the `progress[0] = 0` store in the
// original (same as Attractos_StoreProgress; clear call/order/type tried).
void GizPanels_StoreProgress(void *world_ptr, void *unused, void *data) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  u32 *progress = (u32 *)data;
  if (progress == NULL)
    return;
  progress[0] = 0;
  progress[1] = -1;
  progress[2] = -1;
  if (world == NULL || world->gizpanel_sys == NULL ||
      world->gizpanel_sys->panels == NULL)
    return;
  for (i32 i = 0; i < world->gizpanel_sys->count; i++) {
    if (i >= 32)
      break;
    GIZPANEL *panel = &world->gizpanel_sys->panels[i];
    i32 word = i / 32;
    u32 bit = 1 << (i % 32);
    if (panel->output)
      progress[word] |= bit;
    if (!panel->b62_2)
      progress[word + 2] &= ~bit;
    if (!panel->b62_3)
      progress[word + 1] &= ~bit;
  }
}

// GLOBAL: LEGOBATMAN 0x0093e748
extern char *g_unk0093e748;
// GLOBAL: LEGOBATMAN 0x0093e74c
extern char *g_unk0093e74c;

void AddLevelSfxFromName(char *sfx_name, i32 *sfx_ids, i32 *sfx_count,
                         i32 max_sfx_count);

// FUNCTION: LEGOBATMAN 0x00490940
void GizPanels_AddLevelSfx(void *world_ptr, void *unused, i32 *sfx_ids,
                           i32 *sfx_count, i32 max_sfx) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  if (world->gizpanel_sys == NULL || world->gizpanel_sys->count == 0)
    return;
  if (g_unk0093e748 != NULL)
    AddLevelSfxFromName(g_unk0093e748, sfx_ids, sfx_count, max_sfx);
  if (g_unk0093e74c != NULL)
    AddLevelSfxFromName(g_unk0093e74c, sfx_ids, sfx_count, max_sfx);
}

void GizPanel_Reset(GIZPANEL *panel);

// FUNCTION: LEGOBATMAN 0x004913b0
void GizPanels_Reset(void *world_ptr, void *unused, void *data) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  u32 *progress = (u32 *)data;
  if (world == NULL)
    return;
  GIZPANELSYS *sys = world->gizpanel_sys;
  if (sys == NULL || sys->panels == NULL)
    return;
  for (i32 i = 0; i < sys->count; i++) {
    GIZPANEL *panel = &sys->panels[i];
    GizPanel_Reset(panel);
    if (progress != NULL && i < 32) {
      i32 word = i / 32;
      u32 bit = 1 << (i % 32);
      if (progress[word] & bit)
        panel->output = 1;
      panel->b62_2 = (progress[word + 2] & bit) != 0;
      panel->b62_3 = (progress[word + 1] & bit) != 0;
    }
  }
}

// FUNCTION: LEGOBATMAN 0x00491500
void GizPanel_Activate(GIZMO *gizmo, i32 active) {
  if (gizmo != NULL && gizmo->object != NULL) {
    GIZPANEL *panel = (GIZPANEL *)gizmo->object;
    panel->b62_3 = active != 0;
    if (active)
      GizPanel_Reset(panel);
  }
}

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
