// gameapi/unk_0066c280.cpp: TU of unknown name, found by its header-static
// copies (the functions after them are not matched yet).

#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"

// FUNCTION: LEGOBATMAN 0x0066c280
static f32 NuFabs(f32 f);
// FUNCTION: LEGOBATMAN 0x0066c2a0
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_0066c280(f32 *v, f32 a, i32 i) {
  v[2] = NuFabs(a);
  NuVec4Set(v, a, a, a, a);
}

// The rest of this TU is the Mac's Teleport file (saga
// legoapi/gizmos/transport/teleport.cpp, Teleports_Load ..
// Teleport_RegisterGizmo 0x66dcb0).

#include "../batman/leveldata_unk.h"
#include "../batman/worldinfo_unk.h"
#include <stddef.h>

typedef struct TELEPORT_s {
  char name[0x10]; // 0x00
  u8 pad10[0xb0 - 0x10];
  u8 visible : 1; // 0xb0
  u8 enabled : 1; // 0xb0 bit 1
  u8 active : 1;  // 0xb0 bit 2, occupied
  u8 padb1[0x178 - 0xb1];
} TELEPORT;

typedef struct TELEPORTPROGRESS_s {
  u32 enabled[1]; // 0x00
  u32 visible[1]; // 0x04
} TELEPORTPROGRESS;

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

void AddGizmo(GIZMOSYS_s *gizmo_sys, i32 type_id, void *a, void *object);
void *GameBufferAllocProgressUnk005bbaf0(VARIPTR *buf, VARIPTR *buf_end,
                                         i32 size);

// GLOBAL: LEGOBATMAN 0x00967e74
static char *Teleport_Prefix = "TLT_";
// GLOBAL: LEGOBATMAN 0x00967e7c
static char *Teleport_OutputNames[2] = {"Occupied", "Not Occupied"};
// GLOBAL: LEGOBATMAN 0x00967e84
i32 teleport_gizmotype_id = -1;

// FUNCTION: LEGOBATMAN 0x0066c640
void *Teleports_ReserveBufferSpace(void *world_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  world->teleports = NULL;
  world->teleport_count = 0;
  if (world->current_level->max_teleports > 0) {
    world->buf104.addr = (world->buf104.addr + 0xf) & ~0xf;
    world->teleports = (TELEPORT *)world->buf104.void_ptr;
    world->buf104.addr +=
        world->current_level->max_teleports * sizeof(TELEPORT);
  }
  return world->teleports;
}

// FUNCTION: LEGOBATMAN 0x0066c690
void *Teleports_AllocateProgressData(VARIPTR *buf, VARIPTR *buf_end) {
  return GameBufferAllocProgressUnk005bbaf0(buf, buf_end,
                                            sizeof(TELEPORTPROGRESS));
}

// FUNCTION: LEGOBATMAN 0x0066c6b0
void Teleports_ClearProgress(void *world, void *progress_ptr) {
  TELEPORTPROGRESS *progress = (TELEPORTPROGRESS *)progress_ptr;
  if (progress != NULL) {
    progress->enabled[0] = 0xffffffff;
    progress->visible[0] = 0xffffffff;
  }
}

// FUNCTION: LEGOBATMAN 0x0066c6d0
void Teleports_StoreProgress(void *world_ptr, void *unused,
                             void *progress_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  TELEPORTPROGRESS *progress = (TELEPORTPROGRESS *)progress_ptr;
  if (progress == NULL)
    return;
  Teleports_ClearProgress(NULL, progress);
  if (world != NULL && world->teleports != NULL) {
    TELEPORT *teleport = world->teleports;
    for (i32 i = 0; i < world->teleport_count; i++, teleport++) {
      if (i >= 32)
        break;
      i32 word = i / 32;
      u32 bit = 1 << (i % 32);
      if (!teleport->visible)
        progress->visible[word] &= ~bit;
      if (!teleport->enabled)
        progress->enabled[word] &= ~bit;
    }
  }
}

// FUNCTION: LEGOBATMAN 0x0066dad0
i32 Teleport_ActivateRev(GIZMO *gizmo, i32 activate, i32 flags) {
  if (gizmo == NULL)
    return 0;
  TELEPORT *teleport = (TELEPORT *)gizmo->object;
  if (teleport == NULL)
    return 0;
  if ((flags & 1) != 0 && teleport->enabled != activate)
    return 0;
  if (activate != 0)
    teleport->enabled = 0;
  else if (teleport->enabled == 0)
    teleport->enabled = 1;
  return 1;
}

// FUNCTION: LEGOBATMAN 0x0066db30
void Teleport_Activate(GIZMO *gizmo, i32 activate) {
  TELEPORT *teleport = (TELEPORT *)gizmo->object;
  if (activate != 0) {
    teleport->active = 0;
    teleport->enabled = 1;
  } else {
    teleport->enabled = 0;
  }
}

// FUNCTION: LEGOBATMAN 0x0066db60
i32 Teleport_GetMaxGizmos(void *world_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  return world != NULL ? world->current_level->max_teleports : 0;
}

// FUNCTION: LEGOBATMAN 0x0066db80
void Teleport_AddGizmos(GIZMOSYS_s *gizmo_sys, i32 type_id, void *world_ptr,
                        void *unused) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  if (world != NULL && world->teleports != NULL) {
    for (i32 i = 0; i < world->teleport_count; i++)
      AddGizmo(gizmo_sys, type_id, NULL, &world->teleports[i]);
  }
}

// FUNCTION: LEGOBATMAN 0x0066dbe0
char *Teleport_GetGizmoName(GIZMO *gizmo) {
  if (gizmo != NULL && gizmo->object != NULL)
    return (char *)gizmo->object;
  return NULL;
}

// FUNCTION: LEGOBATMAN 0x0066dc00
i32 Teleport_GetOutput(GIZMO *gizmo, i32 output_index, i32 b) {
  if (gizmo != NULL && gizmo->object != NULL) {
    TELEPORT *teleport = (TELEPORT *)gizmo->object;
    switch (output_index) {
    case 0:
      if (teleport->active)
        return 1;
      break;
    case 1:
      if (!teleport->active)
        return 1;
      break;
    }
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x0066dc40
char *Teleport_GetOutputName(GIZMO *gizmo, i32 output_index) {
  if ((u32)output_index <= 1)
    return Teleport_OutputNames[output_index];
  return NULL;
}

// FUNCTION: LEGOBATMAN 0x0066dc60
i32 Teleport_GetNumOutputs(GIZMO *gizmo) { return 2; }

// FUNCTION: LEGOBATMAN 0x0066dc70
void Teleport_SetVisibility(GIZMO *gizmo, i32 visible) {
  ((TELEPORT *)gizmo->object)->visible = visible;
}

// FUNCTION: LEGOBATMAN 0x0066dc90
i32 Teleport_GetVisibility(GIZMO *gizmo) {
  if (gizmo != NULL && ((TELEPORT *)gizmo->object)->visible)
    return 1;
  return 0;
}

void Teleports_Draw(void *world, void *unused, f32 dt);
void Teleports_Reset(void *world, void *unused, void *progress);
i32 Teleports_Load(void *world, void *unused);

// FUNCTION: LEGOBATMAN 0x0066dcb0
ADDGIZMOTYPE *Teleport_RegisterGizmo(i32 type_id) {
  // GLOBAL: LEGOBATMAN 0x00ad2618
  static ADDGIZMOTYPE addtype;

  addtype = Default_ADDGIZMOTYPE;
  addtype.name = "Teleport";
  addtype.prefix = Teleport_Prefix;
  addtype.progress_size = 0;
  addtype.fns[0] = (void *)Teleport_GetMaxGizmos;
  addtype.fns[1] = (void *)Teleport_AddGizmos;
  addtype.fns[2] = NULL;
  addtype.fns[3] = NULL;
  addtype.fns[4] = (void *)Teleports_Draw;
  addtype.fns[5] = NULL;
  addtype.fns[6] = (void *)Teleport_GetGizmoName;
  addtype.fns[7] = (void *)Teleport_GetOutput;
  addtype.fns[8] = (void *)Teleport_GetOutputName;
  addtype.fns[9] = (void *)Teleport_GetNumOutputs;
  addtype.fns[10] = (void *)Teleport_Activate;
  addtype.fns[11] = (void *)Teleport_ActivateRev;
  addtype.fns[12] = (void *)Teleport_SetVisibility;
  addtype.fns[13] = (void *)Teleport_GetVisibility;
  addtype.fns[14] = NULL;
  addtype.fns[15] = NULL;
  addtype.fns[19] = (void *)Teleports_AllocateProgressData;
  addtype.fns[20] = (void *)Teleports_ClearProgress;
  addtype.fns[21] = (void *)Teleports_StoreProgress;
  addtype.fns[22] = (void *)Teleports_Reset;
  addtype.fns[23] = (void *)Teleports_ReserveBufferSpace;
  addtype.fns[24] = (void *)Teleports_Load;
  addtype.fns[25] = NULL;
  addtype.fns[26] = NULL;
  addtype.fns[27] = NULL;
  teleport_gizmotype_id = type_id;
  return &addtype;
}
