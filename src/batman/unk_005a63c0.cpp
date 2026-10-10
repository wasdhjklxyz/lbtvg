
// The rest of this TU is the Mac's Techno file (.. Technos_RegisterGizmo
// 0x5a82d0); callbacks named by their RegisterGizmo slot.

#include "leveldata_unk.h"
#include "worldinfo_unk.h"
#include <stddef.h>

typedef struct TECHNO_s {
  u8 pad0[0x40];
  char name[0x10];  // 0x40
  nuvec_s position; // 0x50
  u8 pad5c[0x8b - 0x5c];
  u8 active : 1;  // 0x8b
  u8 visible : 1; // 0x8b bit 1
  u8 b2 : 1;
  u8 b3 : 1;
  u8 done : 1; // 0x8b bit 4
  u8 pad8c[0xcc - 0x8c];
} TECHNO;

typedef struct TECHNOPROGRESS_s {
  u32 active[1];  // 0x00
  u32 visible[1]; // 0x04
  u32 done[1];    // 0x08
} TECHNOPROGRESS;

typedef struct GIZMO_s {
  void *object;
} GIZMO;

i32 NuStrLen(const char *s);
void AddGizmo(GIZMOSYS_s *gizmo_sys, i32 type_id, void *a, void *object);
void *GameBufferAllocProgressUnk005bbaf0(VARIPTR *buf, VARIPTR *buf_end,
                                         i32 size);

// GLOBAL: LEGOBATMAN 0x0095fb50
static char *Techno_OutputNames[3] = {"Active", "Special Button Down",
                                      "Got Handle"};

// FUNCTION: LEGOBATMAN 0x005a6fe0
void Techno_Activate(GIZMO *gizmo, i32 active) {
  if (gizmo != NULL)
    ((TECHNO *)gizmo->object)->active = active != 0;
}

// FUNCTION: LEGOBATMAN 0x005a7730
void Technos_AddGizmos(GIZMOSYS_s *gizmo_sys, i32 type_id, void *world_info,
                       void *unused) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_info;
  for (i32 i = 0; i < world->techno_count; i++) {
    if (NuStrLen(world->technos[i].name) != 0)
      AddGizmo(gizmo_sys, type_id, NULL, &world->technos[i]);
  }
}

// FUNCTION: LEGOBATMAN 0x005a77a0
char *Techno_GetGizmoName(GIZMO *gizmo) {
  return gizmo != NULL ? ((TECHNO *)gizmo->object)->name : NULL;
}

// FUNCTION: LEGOBATMAN 0x005a77c0
i32 Techno_GetNumOutputs(GIZMO *gizmo) { return 3; }

// FUNCTION: LEGOBATMAN 0x005a77d0
char *Techno_GetOutputName(GIZMO *gizmo, i32 output_index) {
  if ((u32)output_index <= 2)
    return Techno_OutputNames[output_index];
  return "Unknown";
}

// FUNCTION: LEGOBATMAN 0x005a77f0
void *Technos_AllocateProgressData(VARIPTR *buf, VARIPTR *buf_end) {
  return GameBufferAllocProgressUnk005bbaf0(buf, buf_end,
                                            sizeof(TECHNOPROGRESS));
}

// FUNCTION: LEGOBATMAN 0x005a7810
void Technos_ClearProgress(void *world, void *progress_ptr) {
  TECHNOPROGRESS *progress = (TECHNOPROGRESS *)progress_ptr;
  if (progress != NULL) {
    progress->active[0] = 0xffffffff;
    progress->visible[0] = 0xffffffff;
    progress->done[0] = 0;
  }
}

// FUNCTION: LEGOBATMAN 0x005a7830
void Technos_StoreProgress(void *world_ptr, void *unused, void *progress_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  TECHNOPROGRESS *progress = (TECHNOPROGRESS *)progress_ptr;
  if (progress == NULL)
    return;
  Technos_ClearProgress(NULL, progress);
  if (world != NULL && world->technos != NULL) {
    TECHNO *techno = world->technos;
    for (i32 i = 0; i < world->techno_count; i++, techno++) {
      if (i >= 32)
        break;
      i32 word = i / 32;
      u32 bit = 1 << (i & 31);
      if (!techno->visible)
        progress->visible[word] &= ~bit;
      if (!techno->active)
        progress->active[word] &= ~bit;
      if (techno->done)
        progress->done[word] |= bit;
    }
  }
}

// FUNCTION: LEGOBATMAN 0x005a78d0
nuvec_s *Techno_GetPos(GIZMO *gizmo) {
  if (gizmo != NULL && gizmo->object != NULL)
    return &((TECHNO *)gizmo->object)->position;
  return NULL;
}
