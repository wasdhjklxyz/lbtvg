// gameapi/unk_0066f160.cpp: TU of unknown name, found by its header-static
// copies (the functions after them are not matched yet).

#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"

// FUNCTION: LEGOBATMAN 0x0066f160
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_0066f160(f32 *v, f32 a, i32 i) { NuVec4Set(v, a, a, a, a); }

// The rest of this TU is the Mac's Portal gizmo file (saga
// legoapi/gizmos/transport/gizportal.cpp, Portal_GetMaxGizmos ..
// Portal_RegisterGizmo 0x66f460).

#include "../batman/worldinfo_unk.h"
#include <stddef.h>

typedef struct NUPORTAL_s {
  u8 pad0[0x1a];
  i8 id; // 0x1a
  u8 pad1b;
  u8 is_active; // 0x1c, bit 0
  u8 pad1d[0x20 - 0x1d];
} NUPORTAL;

struct nugscn_s {
  u8 pad0[0x6c];
  i32 max_portals;   // 0x6c
  NUPORTAL *portals; // 0x70
};

typedef struct GIZPORTALPROGRESS_s {
  u32 progress_mask[1];
} GIZPORTALPROGRESS;

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
// GLOBAL: LEGOBATMAN 0x009680d4
extern char *g_unk009680d4; // "portal_"

void AddGizmo(GIZMOSYS_s *gizmo_sys, i32 type_id, void *a, void *object);
void *GameBufferAllocProgressUnk005bbaf0(VARIPTR *buf, VARIPTR *buf_end,
                                         i32 size);
void NuPortalSetActiveDirect(NUPORTAL *portal, i32 active);
void PortalDoors_Reset(WORLDINFO_s *world);
int sprintf(char *buf, const char *fmt, ...);

// GLOBAL: LEGOBATMAN 0x00ad271c
i32 portal_gizmotype_id;

// FUNCTION: LEGOBATMAN 0x0066f180
i32 Portal_GetMaxGizmos(void *world_info) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_info;
  if (world == NULL || world->scn140 == NULL)
    return 0;
  return world->scn140->max_portals;
}

// FUNCTION: LEGOBATMAN 0x0066f1a0
void Portal_AddGizmos(GIZMOSYS_s *gizmo_sys, i32 type_id, void *world_info,
                      void *unused) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_info;
  if (world == NULL || world->scn140 == NULL)
    return;
  for (i32 i = 0; i < world->scn140->max_portals; i++) {
    if (world->scn140->portals[i].id == 0)
      continue;
    AddGizmo(gizmo_sys, type_id, NULL, &world->scn140->portals[i]);
  }
}

// FUNCTION: LEGOBATMAN 0x0066f200
char *Portal_GetGizmoName(GIZMO *gizmo) {
  // GLOBAL: LEGOBATMAN 0x00ad2720
  static char name[16];
  if (gizmo == NULL || gizmo->object == NULL)
    return NULL;
  NUPORTAL *portal = (NUPORTAL *)gizmo->object;
  sprintf(name, "%s%d", g_unk009680d4, portal->id);
  return name;
}

// FUNCTION: LEGOBATMAN 0x0066f240
i32 Portal_GetOutput(GIZMO *gizmo, i32 a, i32 b) {
  if (gizmo != NULL && gizmo->object != NULL &&
      (((NUPORTAL *)gizmo->object)->is_active & 1))
    return 1;
  return 0;
}

// FUNCTION: LEGOBATMAN 0x0066f260
i32 Portal_GetNumOutputs(GIZMO *gizmo) { return 1; }

// FUNCTION: LEGOBATMAN 0x0066f270
char *Portal_GetOutputName(GIZMO *gizmo, i32 output_index) {
  // GLOBAL: LEGOBATMAN 0x00968180
  static char name[64] = "Active";
  return name;
}

// FUNCTION: LEGOBATMAN 0x0066f280
void Portal_Activate(GIZMO *gizmo, i32 active) {
  if (gizmo == NULL || gizmo->object == NULL)
    return;
  NuPortalSetActiveDirect((NUPORTAL *)gizmo->object, active);
}

// FUNCTION: LEGOBATMAN 0x0066f2a0
i32 Portal_ActivateRev(GIZMO *gizmo, i32 is_inactive, i32 query) {
  if (gizmo == NULL || gizmo->object == NULL)
    return 0;
  NUPORTAL *portal = (NUPORTAL *)gizmo->object;
  if ((query & 1) != 0) {
    if (is_inactive != 0 && is_inactive != (portal->is_active & 1))
      return 0;
    return 1;
  }
  NuPortalSetActiveDirect(portal, is_inactive == 0);
  return 1;
}

// FUNCTION: LEGOBATMAN 0x0066f2f0
void Portals_Reset(void *world_info, void *unused, void *progress_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_info;
  GIZPORTALPROGRESS *progress = (GIZPORTALPROGRESS *)progress_ptr;
  if (world == NULL || world->scn140 == NULL)
    return;
  PortalDoors_Reset(world);
  i32 index = 0;
  for (i32 i = 0; i < world->scn140->max_portals; i++) {
    if (world->scn140->portals[i].id == 0)
      continue;
    NuPortalSetActiveDirect(&world->scn140->portals[i], 1);
    if (progress != NULL && index < 32) {
      i32 word = index / 32;
      u32 bit = 1 << (index & 31);
      if (world->scn140->portals[i].is_active & 1)
        progress->progress_mask[word] |= bit;
    }
    index++;
  }
}

// FUNCTION: LEGOBATMAN 0x0066f3a0
void *Portals_AllocateProgressData(VARIPTR *buf, VARIPTR *buf_end) {
  return GameBufferAllocProgressUnk005bbaf0(buf, buf_end,
                                            sizeof(GIZPORTALPROGRESS));
}

// FUNCTION: LEGOBATMAN 0x0066f3c0
void Portals_ClearProgress(void *world, void *progress_ptr) {
  GIZPORTALPROGRESS *progress = (GIZPORTALPROGRESS *)progress_ptr;
  if (progress == NULL)
    return;
  progress->progress_mask[0] = ~0u;
}

// FUNCTION: LEGOBATMAN 0x0066f3d0
void Portals_StoreProgress(void *world_info, void *unused, void *progress_ptr) {
  GIZPORTALPROGRESS *progress = (GIZPORTALPROGRESS *)progress_ptr;
  if (progress != NULL)
    progress->progress_mask[0] = ~0u;
  WORLDINFO_s *world = (WORLDINFO_s *)world_info;
  if (world == NULL || world->scn140 == NULL || progress == NULL)
    return;
  i32 index = 0;
  for (i32 i = 0; i < world->scn140->max_portals; i++) {
    if (world->scn140->portals[i].id != 0 && index < 32) {
      i32 word = index / 32;
      u32 bit = 1 << (index & 31);
      if (world->scn140->portals[i].is_active & 1)
        progress->progress_mask[word] |= bit;
      index++;
    }
  }
}

// FUNCTION: LEGOBATMAN 0x0066f460
ADDGIZMOTYPE *Portal_RegisterGizmo(i32 type_id) {
  // GLOBAL: LEGOBATMAN 0x00ad2730
  static ADDGIZMOTYPE addtype;

  addtype = Default_ADDGIZMOTYPE;
  addtype.name = "Portal";
  addtype.prefix = g_unk009680d4;
  addtype.progress_size = 0;
  addtype.fns[0] = (void *)Portal_GetMaxGizmos;
  addtype.fns[1] = (void *)Portal_AddGizmos;
  addtype.fns[2] = NULL;
  addtype.fns[3] = NULL;
  addtype.fns[4] = NULL;
  addtype.fns[5] = NULL;
  addtype.fns[6] = (void *)Portal_GetGizmoName;
  addtype.fns[7] = (void *)Portal_GetOutput;
  addtype.fns[8] = (void *)Portal_GetOutputName;
  addtype.fns[9] = (void *)Portal_GetNumOutputs;
  addtype.fns[10] = (void *)Portal_Activate;
  addtype.fns[11] = (void *)Portal_ActivateRev;
  addtype.fns[12] = NULL;
  addtype.fns[13] = NULL;
  addtype.fns[14] = NULL;
  addtype.fns[15] = NULL;
  addtype.fns[16] = NULL;
  addtype.fns[17] = NULL;
  addtype.fns[18] = NULL;
  addtype.fns[19] = (void *)Portals_AllocateProgressData;
  addtype.fns[20] = (void *)Portals_ClearProgress;
  addtype.fns[21] = (void *)Portals_StoreProgress;
  addtype.fns[22] = (void *)Portals_Reset;
  addtype.fns[23] = NULL;
  addtype.fns[24] = NULL;
  addtype.fns[25] = NULL;
  addtype.fns[26] = NULL;
  addtype.fns[27] = NULL;
  portal_gizmotype_id = type_id;
  return &addtype;
}
