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
