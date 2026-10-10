
// The rest of this TU is the Mac's Tubes file (Tubes_Load ..
// Tubes_RegisterGizmo 0x5e5d70); callbacks named by their RegisterGizmo slot.

#include "leveldata_unk.h"
#include "worldinfo_unk.h"
#include <stddef.h>

typedef struct TUBE_s {
  char name[0x10]; // 0x00
  u8 pad10[0x1c - 0x10];
  nuvec_s position; // 0x1c
  u8 pad28[0x44 - 0x28];
  u8 active : 1;  // 0x44
  u8 visible : 1; // 0x44 bit 1
  u8 b2 : 1;
  u8 reversed : 1; // 0x44 bit 3
  u8 pad45[0x50 - 0x45];
} TUBE;

typedef struct TUBEPROGRESS_s {
  u32 visible[1]; // 0x00
  u32 active[1];  // 0x04
} TUBEPROGRESS;

typedef struct GIZMO_s {
  void *object;
} GIZMO;

i32 NuStrLen(const char *s);
void AddGizmo(GIZMOSYS_s *gizmo_sys, i32 type_id, void *a, void *object);
void *GameBufferAllocProgressUnk005bbaf0(VARIPTR *buf, VARIPTR *buf_end,
                                         i32 size);

// GLOBAL: LEGOBATMAN 0x009617e4
static char *Tube_OutputName = "Active";

// FUNCTION: LEGOBATMAN 0x005e5140
void *Tubes_ReserveBufferSpace(void *world_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  world->tubes = NULL;
  world->tube_count = 0;
  if (world->current_level->max_tubes > 0) {
    world->buf104.addr = (world->buf104.addr + 0xf) & ~0xf;
    world->tubes = (TUBE *)world->buf104.void_ptr;
    world->buf104.addr += world->current_level->max_tubes * sizeof(TUBE);
  }
  return world->tubes;
}

// FUNCTION: LEGOBATMAN 0x005e5190
void Tube_Activate(GIZMO *gizmo, i32 active) {
  if (gizmo != NULL)
    ((TUBE *)gizmo->object)->active = active != 0;
}

// FUNCTION: LEGOBATMAN 0x005e51b0
i32 Tube_ActivateRev(GIZMO *gizmo, i32 value, i32 query) {
  if (gizmo == NULL || gizmo->object == NULL)
    return 0;
  TUBE *tube = (TUBE *)gizmo->object;
  if (query & 1)
    return value != tube->reversed;
  if (value) {
    tube->active = 0;
    tube->reversed = 1;
    return 1;
  }
  tube->reversed = 0;
  tube->active = 1;
  return 1;
}

// FUNCTION: LEGOBATMAN 0x005e5210
void Tube_SetVisibility(GIZMO *gizmo, i32 visible) {
  if (gizmo != NULL)
    ((TUBE *)gizmo->object)->visible = visible != 0;
}

// FUNCTION: LEGOBATMAN 0x005e5300
void Tubes_Draw(void *world, void *unused, f32 dt) {}

// FUNCTION: LEGOBATMAN 0x005e5330
void Tubes_AddGizmos(GIZMOSYS_s *gizmo_sys, i32 type_id, void *world_ptr,
                     void *unused) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  for (i32 i = 0; i < world->tube_count; i++) {
    if (NuStrLen(world->tubes[i].name) != 0)
      AddGizmo(gizmo_sys, type_id, NULL, &world->tubes[i]);
  }
}

// FUNCTION: LEGOBATMAN 0x005e53a0
char *Tube_GetGizmoName(GIZMO *gizmo) {
  return gizmo != NULL ? ((TUBE *)gizmo->object)->name : NULL;
}

// FUNCTION: LEGOBATMAN 0x005e53b0
i32 Tube_GetOutput(GIZMO *gizmo, i32 a, i32 b) {
  TUBE *tube = (TUBE *)gizmo->object;
  if (tube->visible && tube->active)
    return 1;
  return 0;
}

// FUNCTION: LEGOBATMAN 0x005e53d0
char *Tube_GetOutputName(GIZMO *gizmo, i32 output) { return Tube_OutputName; }

// FUNCTION: LEGOBATMAN 0x005e53e0
i32 Tube_GetNumOutputs(GIZMO *gizmo) { return 1; }

// FUNCTION: LEGOBATMAN 0x005e53f0
void *Tubes_AllocateProgressData(VARIPTR *buf, VARIPTR *buf_end) {
  return GameBufferAllocProgressUnk005bbaf0(buf, buf_end, sizeof(TUBEPROGRESS));
}

// FUNCTION: LEGOBATMAN 0x005e5410
void Tubes_ClearProgress(void *world, void *progress_ptr) {
  TUBEPROGRESS *progress = (TUBEPROGRESS *)progress_ptr;
  if (progress != NULL) {
    progress->visible[0] = 0xffffffff;
    progress->active[0] = 0xffffffff;
  }
}

// FUNCTION: LEGOBATMAN 0x005e5430
void Tubes_StoreProgress(void *world_ptr, void *unused, void *progress_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  TUBEPROGRESS *progress = (TUBEPROGRESS *)progress_ptr;
  if (progress == NULL)
    return;
  Tubes_ClearProgress(NULL, progress);
  if (world != NULL && world->tubes != NULL) {
    TUBE *tube = world->tubes;
    for (i32 i = 0; i < world->tube_count; i++, tube++) {
      if (i >= 32)
        break;
      i32 word = i / 32;
      u32 bit = 1 << (i & 31);
      if (!tube->visible)
        progress->visible[word] &= ~bit;
      if (!tube->active)
        progress->active[word] &= ~bit;
    }
  }
}

// FUNCTION: LEGOBATMAN 0x005e54b0
nuvec_s *Tube_GetPos(GIZMO *gizmo) {
  if (gizmo == NULL)
    return NULL;
  return &((TUBE *)gizmo->object)->position;
}
