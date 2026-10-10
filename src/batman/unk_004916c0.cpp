// batman/unk_004916c0.cpp: TU of unknown name, found by its header-static
// copies (the functions after them are not matched yet).

#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"

// FUNCTION: LEGOBATMAN 0x004916c0
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_004916c0(f32 *v, f32 a, i32 i) { NuVec4Set(v, a, a, a, a); }

// The rest of this TU is saga legoapi/gizmo/object/giztorpedo.cpp; PC order
// = Mac order (GizTorp_Load .. GizTorpMachine_RegisterGizmo).

#include <stddef.h>
#include <string.h>

typedef struct GIZTORPMACHINE_s {
  char name[0x10];     // 0x00
  f32 position[3];     // 0x10
  f32 activation_time; // 0x1c
  u8 pad20[0x2d - 0x20];
  u8 active : 1;  // 0x2d bit 0
  u8 visible : 1; // 0x2d bit 1
  u8 pad2e[2];
} GIZTORPMACHINE;

typedef struct GIZTORPMACHINESYS_s {
  GIZTORPMACHINE *machines; // 0x00
  i32 count;                // 0x04
} GIZTORPMACHINESYS;

typedef struct GIZMO_s {
  void *object;
} GIZMO;

typedef struct GTLEVEL_s {
  u8 pad0[0x116];
  u8 max_torp_machines; // 0x116
} GTLEVEL;

typedef struct GTWORLD_s {
  u8 pad0[0x12c];
  GTLEVEL *current_level; // 0x12c
  u8 pad130[0x5274 - 0x130];
  GIZTORPMACHINESYS *giz_torp_machine_sys; // 0x5274
} GTWORLD;

// GLOBAL: LEGOBATMAN 0x00a95fe0
extern f32 FRAMETIME;

// from saga giztorpedo.cpp
// FUNCTION: LEGOBATMAN 0x00491900
void GizTorp_SetVisibility(GIZMO *gizmo, i32 visible) {
  if (gizmo != NULL) {
    GIZTORPMACHINE *machine = (GIZTORPMACHINE *)gizmo->object;
    machine->visible = visible != 0;
  }
}

// from saga giztorpedo.cpp
// FUNCTION: LEGOBATMAN 0x00491930
void GizTorp_Activate(GIZMO *gizmo, i32 active) {
  if (gizmo != NULL) {
    GIZTORPMACHINE *machine = (GIZTORPMACHINE *)gizmo->object;
    machine->active = active != 0;
  }
}

// Batman's form of saga giztorpedo.cpp's GizTorp_Update
// STUB: LEGOBATMAN 0x00491a90
// the original keeps 0.5 as one dword constant in st(0) for the whole loop;
// ours loads a qword 0.5 and compares the other way round (3 forms tried).
void GizTorp_Update(void *world_ptr, void *unused, f32 dt) {
  GIZTORPMACHINESYS *system = ((GTWORLD *)world_ptr)->giz_torp_machine_sys;
  if (system == NULL)
    return;
  for (i32 i = 0; i < system->count; i++) {
    GIZTORPMACHINE *machine = &system->machines[i];
    if (0.5f > machine->activation_time) {
      machine->activation_time = FRAMETIME + machine->activation_time;
      if (0.5f < machine->activation_time)
        machine->activation_time = 0.5f;
    }
  }
}

// from saga giztorpedo.cpp
// FUNCTION: LEGOBATMAN 0x00491d70
i32 GizTorp_GetMaxGizmos(void *world_ptr) {
  GTWORLD *world = (GTWORLD *)world_ptr;
  return world != NULL ? world->current_level->max_torp_machines : 0;
}

typedef struct GIZMOSYS_s GIZMOSYS;
i32 NuStrLen(const char *s);
void AddGizmo(GIZMOSYS *gizmo_sys, i32 type_id, void *a, void *object);

// Batman's form of saga giztorpedo.cpp's GizTorp_AddGizmos
// FUNCTION: LEGOBATMAN 0x00491d90
void GizTorp_AddGizmos(GIZMOSYS *gizmo_sys, i32 type_id, void *world_ptr,
                       void *unused) {
  GTWORLD *world = (GTWORLD *)world_ptr;
  if (world != NULL && world->giz_torp_machine_sys != NULL) {
    for (i32 i = 0; i < world->giz_torp_machine_sys->count; i++) {
      if (NuStrLen(world->giz_torp_machine_sys->machines[i].name) != 0)
        AddGizmo(gizmo_sys, type_id, NULL,
                 &world->giz_torp_machine_sys->machines[i]);
    }
  }
}

// FUNCTION: LEGOBATMAN 0x00491e10
char *GizTorp_GetGizmoName(GIZMO *gizmo) {
  GIZTORPMACHINE *machine;
  if (gizmo != NULL && (machine = (GIZTORPMACHINE *)gizmo->object) != NULL)
    return machine->name;
  return NULL;
}

// FUNCTION: LEGOBATMAN 0x00491e30
i32 GizTorp_GetOutput(GIZMO *gizmo, i32 a, i32 b) { return 0; }

// from saga giztorpedo.cpp
// FUNCTION: LEGOBATMAN 0x00491e40
char *GizTorp_GetOutputName(GIZMO *gizmo, i32 output_index) {
  return output_index == 0 ? "Ready" : NULL;
}

// FUNCTION: LEGOBATMAN 0x00491e60
i32 GizTorp_GetNumOutputs(GIZMO *gizmo) { return 1; }

void *GameBufferAllocProgressUnk005bbaf0(VARIPTR *buf, VARIPTR *buf_end,
                                         i32 size);

// FUNCTION: LEGOBATMAN 0x00491e70
void *GizTorpedo_AllocateProgressData(VARIPTR *buf, VARIPTR *buf_end) {
  return GameBufferAllocProgressUnk005bbaf0(buf, buf_end, 0x18);
}

// FUNCTION: LEGOBATMAN 0x00491e90
void GizTorpedo_ClearProgress(void *world, void *progress) {
  if (progress != NULL) {
    memset(progress, 0xff, 0xc);
    memset((u8 *)progress + 0xc, 0xff, 0xc);
  }
}

typedef struct GIZTORPPROGRESS_s {
  u32 active[3];  // 0x00
  u32 visible[3]; // 0x0c
} GIZTORPPROGRESS;

// FUNCTION: LEGOBATMAN 0x00491ec0
void GizTorpedo_StoreProgress(void *world, void *system_ptr,
                              void *progress_ptr) {
  GIZTORPMACHINESYS *system = (GIZTORPMACHINESYS *)system_ptr;
  GIZTORPPROGRESS *progress = (GIZTORPPROGRESS *)progress_ptr;
  if (progress != NULL) {
    memset(progress->active, 0xff, sizeof(progress->active));
    memset(progress->visible, 0xff, sizeof(progress->visible));
    GIZTORPMACHINE *machine = system->machines;
    for (i32 i = 0; i < system->count; i++, machine++) {
      if (i >= 3)
        break;
      i32 word = i / 32;
      u32 bit = 1 << (i & 31);
      if (!machine->visible)
        progress->visible[word] &= ~bit;
      if (!machine->active)
        progress->active[word] &= ~bit;
    }
  }
}

// FUNCTION: LEGOBATMAN 0x00491f60
void GizTorpedoes_SetReset(void *world, void *system_ptr, void *progress_ptr) {
  GIZTORPMACHINESYS *system = (GIZTORPMACHINESYS *)system_ptr;
  GIZTORPPROGRESS *progress = (GIZTORPPROGRESS *)progress_ptr;
  GIZTORPMACHINE *machine = system->machines;
  for (i32 i = 0; i < system->count; i++, machine++) {
    if (i >= 3)
      break;
    machine->visible = progress->visible[i / 32];
    machine->active = progress->active[i / 32];
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
// GLOBAL: LEGOBATMAN 0x009c8fe4
i32 giztorpmachine_gizmotype_id;

void GizTorp_ActivateRev(GIZMO *gizmo, i32 a, i32 b);
void GizTorp_Draw(void *world, void *unused, f32 dt);
i32 GizTorp_Load(void *world, void *unused);
void *GizTorps_ReserveBufferSpace(void *world);

// saga giztorpedo.cpp's GizTorpMachine_RegisterGizmo with Batman's callbacks
// FUNCTION: LEGOBATMAN 0x00491fd0
ADDGIZMOTYPE *GizTorpMachine_RegisterGizmo(i32 type_id) {
  static ADDGIZMOTYPE addtype;

  addtype = Default_ADDGIZMOTYPE;
  addtype.name = "Torp Machine";
  addtype.prefix = "";
  addtype.progress_size = 0x18;
  addtype.fns[0] = (void *)GizTorp_GetMaxGizmos;
  addtype.fns[1] = (void *)GizTorp_AddGizmos;
  addtype.fns[2] = NULL;
  addtype.fns[3] = (void *)GizTorp_Update;
  addtype.fns[4] = (void *)GizTorp_Draw;
  addtype.fns[5] = NULL;
  addtype.fns[6] = (void *)GizTorp_GetGizmoName;
  addtype.fns[7] = (void *)GizTorp_GetOutput;
  addtype.fns[8] = (void *)GizTorp_GetOutputName;
  addtype.fns[9] = (void *)GizTorp_GetNumOutputs;
  addtype.fns[10] = (void *)GizTorp_Activate;
  addtype.fns[11] = (void *)GizTorp_ActivateRev;
  addtype.fns[12] = (void *)GizTorp_SetVisibility;
  addtype.fns[13] = NULL;
  addtype.fns[14] = NULL;
  addtype.fns[15] = NULL;
  addtype.fns[16] = NULL;
  addtype.fns[17] = NULL;
  addtype.fns[18] = NULL;
  addtype.fns[19] = (void *)GizTorpedo_AllocateProgressData;
  addtype.fns[20] = (void *)GizTorpedo_ClearProgress;
  addtype.fns[21] = (void *)GizTorpedo_StoreProgress;
  addtype.fns[22] = (void *)GizTorpedoes_SetReset;
  addtype.fns[23] = (void *)GizTorps_ReserveBufferSpace;
  addtype.fns[24] = (void *)GizTorp_Load;
  giztorpmachine_gizmotype_id = type_id;
  return &addtype;
}
