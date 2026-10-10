// batman/unk_005b9e30.cpp: TU of unknown name, found by its header-static
// copies (the functions after them are not matched yet).

#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"

// FUNCTION: LEGOBATMAN 0x005b9e30
static f32 NuFabs(f32 f);
// FUNCTION: LEGOBATMAN 0x005b9e50
static f32 NuSinApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x005b9ef0
static f32 NuCosApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x005b9f00
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_005b9e30(f32 *v, f32 a, i32 i) {
  v[2] = NuFabs(a);
  v[0] = NuSinApprox(i);
  v[1] = NuCosApprox(i);
  NuVec4Set(v, a, a, a, a);
}

// The rest of this TU is the Mac's Lever file (.. Levers_RegisterGizmo
// 0x5bb4f0, Lever_MoveCode); callbacks named by their RegisterGizmo slot.

#include "leveldata_unk.h"
#include "worldinfo_unk.h"
#include <stddef.h>

typedef struct LEVER_s {
  u8 pad0[0x58];
  char name[0x10]; // 0x58
  u8 pad68[0x78 - 0x68];
  f32 pull; // 0x78
  u8 pad7c[0x8c - 0x7c];
  i16 plat; // 0x8c, -1 = none
  u8 pad8e[0x94 - 0x8e];
  u16 b0 : 1;
  u16 being_pulled : 1; // 0x94 bit 1
  u16 goodie : 1;       // 0x94 bit 2
  u16 baddie : 1;       // 0x94 bit 3
  u16 visible : 1;      // 0x94 bit 4
  u16 b5 : 1;
  u16 b6 : 1;
  u16 active : 1; // 0x94 bit 7
  u8 pad96[0xb0 - 0x96];
} LEVER;

typedef struct LEVERPROGRESS_s {
  u32 down[1];    // 0x00
  u32 active[1];  // 0x04
  u32 visible[1]; // 0x08
} LEVERPROGRESS;

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
// GLOBAL: LEGOBATMAN 0x00960050
extern char *Lever_OutputName;

i32 NuStrLen(const char *s);
void AddGizmo(GIZMOSYS_s *gizmo_sys, i32 type_id, void *a, void *object);
void *GameBufferAllocProgressUnk005bbaf0(VARIPTR *buf, VARIPTR *buf_end,
                                         i32 size);
extern "C" void PlatOnOff(i32 index, i32 enabled);
i32 Levers_Load(void *world, void *unused);

// FUNCTION: LEGOBATMAN 0x005ba3b0
void *Levers_ReserveBufferSpace(void *world_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  world->levers = NULL;
  world->lever_count = 0;
  if (world->current_level->max_levers > 0) {
    world->buf104.addr = (world->buf104.addr + 0xf) & ~0xf;
    world->levers = (LEVER *)world->buf104.void_ptr;
    world->buf104.addr += world->current_level->max_levers * sizeof(LEVER);
  }
  return world->levers;
}

// FUNCTION: LEGOBATMAN 0x005ba430
void Lever_SetVisibility(GIZMO *gizmo, i32 visible) {
  if (gizmo != NULL && gizmo->object != NULL) {
    LEVER *lever = (LEVER *)gizmo->object;
    lever->visible = visible != 0;
    if (lever->plat != -1)
      PlatOnOff(lever->plat, lever->visible);
  }
}

// FUNCTION: LEGOBATMAN 0x005ba490
i32 Lever_ActivateRev(GIZMO *gizmo, i32 value, i32 query) {
  if (gizmo != NULL && gizmo->object != NULL) {
    LEVER *lever = (LEVER *)gizmo->object;
    if (query & 1) {
      if (lever->active)
        return value;
      return value == 0;
    }
    lever->active = value == 0;
  }
  return 1;
}

void Levers_Reset(void *world, void *unused, void *progress);
void Levers_Update(void *world, void *unused, f32 dt);
void Levers_Draw(void *world, void *unused, f32 dt);

// FUNCTION: LEGOBATMAN 0x005babb0
i32 Levers_GetMaxGizmos(void *world_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  if (world != NULL)
    return world->current_level->max_levers;
  return 0;
}

// FUNCTION: LEGOBATMAN 0x005babd0
void Levers_AddGizmos(GIZMOSYS_s *gizmo_sys, i32 type_id, void *world_ptr,
                      void *unused) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  for (i32 i = 0; i < world->lever_count; i++) {
    if (NuStrLen(world->levers[i].name) != 0)
      AddGizmo(gizmo_sys, type_id, NULL, &world->levers[i]);
  }
}

// FUNCTION: LEGOBATMAN 0x005bac40
char *Lever_GetGizmoName(GIZMO *gizmo) {
  return gizmo != NULL ? ((LEVER *)gizmo->object)->name : NULL;
}

// FUNCTION: LEGOBATMAN 0x005bac60
i32 Lever_FullyPulledDown(LEVER *lever) {
  if (lever->visible && lever->being_pulled && lever->pull >= 0.6f)
    return 1;
  return 0;
}

// FUNCTION: LEGOBATMAN 0x005bac90
i32 Lever_BeingPulled(LEVER *lever) { return lever->being_pulled; }

// FUNCTION: LEGOBATMAN 0x005bacb0
i32 Lever_GetOutput(GIZMO *gizmo, i32 output, i32 b) {
  LEVER *lever = (LEVER *)gizmo->object;
  if ((lever->active && lever->visible) || b != 0) {
    switch (output) {
    case 0:
      if (Lever_FullyPulledDown(lever))
        return 1;
      break;
    case 1:
      if (lever->goodie && Lever_FullyPulledDown(lever))
        return 1;
      break;
    case 2:
      if (lever->baddie && Lever_FullyPulledDown(lever))
        return 1;
      break;
    }
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x005bad20
char *Lever_GetOutputName(GIZMO *gizmo, i32 output) {
  switch (output) {
  case 0:
    return Lever_OutputName;
  case 1:
    return "Down(Goodie)";
  case 2:
    return "Down(Baddie)";
  }
  return NULL;
}

// FUNCTION: LEGOBATMAN 0x005bad50
i32 Lever_GetNumOutputs(GIZMO *gizmo) { return 3; }

// FUNCTION: LEGOBATMAN 0x005bad60
void *Levers_AllocateProgressData(VARIPTR *buf, VARIPTR *buf_end) {
  return GameBufferAllocProgressUnk005bbaf0(buf, buf_end,
                                            sizeof(LEVERPROGRESS));
}

// FUNCTION: LEGOBATMAN 0x005bad80
void Levers_ClearProgress(void *world, void *progress_ptr) {
  LEVERPROGRESS *progress = (LEVERPROGRESS *)progress_ptr;
  if (progress != NULL) {
    progress->down[0] = 0;
    progress->active[0] = 0xffffffff;
    progress->visible[0] = 0xffffffff;
  }
}

void Levers_StoreProgress(void *world, void *unused, void *progress);
void Lever_Activate(GIZMO *gizmo, i32 active);

// GLOBAL: LEGOBATMAN 0x00960048
i32 lever_gizmotype_id = -1;

// FUNCTION: LEGOBATMAN 0x005bb4f0
ADDGIZMOTYPE *Levers_RegisterGizmo(i32 type_id) {
  // GLOBAL: LEGOBATMAN 0x00960100
  static char *name = "Lever";
  // GLOBAL: LEGOBATMAN 0x00ab0778
  static ADDGIZMOTYPE addtype;

  addtype = Default_ADDGIZMOTYPE;
  addtype.name = name;
  addtype.prefix = "";
  addtype.progress_size = sizeof(LEVERPROGRESS);
  addtype.fns[0] = (void *)Levers_GetMaxGizmos;
  addtype.fns[1] = (void *)Levers_AddGizmos;
  addtype.fns[2] = NULL;
  addtype.fns[3] = (void *)Levers_Update;
  addtype.fns[4] = (void *)Levers_Draw;
  addtype.fns[5] = NULL;
  addtype.fns[6] = (void *)Lever_GetGizmoName;
  addtype.fns[7] = (void *)Lever_GetOutput;
  addtype.fns[8] = (void *)Lever_GetOutputName;
  addtype.fns[9] = (void *)Lever_GetNumOutputs;
  addtype.fns[10] = (void *)Lever_Activate;
  addtype.fns[11] = (void *)Lever_ActivateRev;
  addtype.fns[12] = (void *)Lever_SetVisibility;
  addtype.fns[13] = NULL;
  addtype.fns[14] = NULL;
  addtype.fns[15] = NULL;
  addtype.fns[16] = NULL;
  addtype.fns[17] = NULL;
  addtype.fns[18] = NULL;
  addtype.fns[19] = (void *)Levers_AllocateProgressData;
  addtype.fns[20] = (void *)Levers_ClearProgress;
  addtype.fns[21] = (void *)Levers_StoreProgress;
  addtype.fns[22] = (void *)Levers_Reset;
  addtype.fns[23] = (void *)Levers_ReserveBufferSpace;
  addtype.fns[24] = (void *)Levers_Load;
  addtype.fns[25] = NULL;
  addtype.fns[26] = NULL;
  addtype.fns[27] = NULL;
  lever_gizmotype_id = type_id;
  return &addtype;
}
