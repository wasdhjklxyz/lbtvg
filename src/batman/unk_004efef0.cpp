// batman/unk_004efef0.cpp: TU of unknown name, found by its header-static
// copies (the functions after them are not matched yet).

#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"

// FUNCTION: LEGOBATMAN 0x004efef0
static f32 NuSinApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x004effb0
static f32 NuCosApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x004effd0
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_004efef0(f32 *v, f32 a, i32 i) {
  v[0] = NuSinApprox(i);
  v[1] = NuCosApprox(i);
  NuVec4Set(v, a, a, a, a);
}

// The rest of this TU is the Mac's Signal file (saga
// legoapi/props/objects/signal.cpp, .. Signals_RegisterGizmo 0x4f2170);
// callbacks named by their RegisterGizmo slot.

#include "leveldata_unk.h"
#include "worldinfo_unk.h"
#include <stddef.h>
#include <string.h>

typedef struct SIGNAL_s {
  u8 pad0[0x40];
  char name[0x10];  // 0x40
  nuvec_s position; // 0x50
  u8 pad5c[0x77 - 0x5c];
  u8 visible : 1; // 0x77
  u8 active : 1;  // 0x77 bit 1
  u8 pad78[0x3d0 - 0x78];
  f32 f3d0; // 0x3d0
  u8 pad3d4[0x3d8 - 0x3d4];
} SIGNAL;

typedef struct SIGNALPROGRESS_s {
  u8 suit_letters[0x20]; // 0x00
  u32 active_mask;       // 0x20
  u32 visible_mask;      // 0x24
} SIGNALPROGRESS;

typedef struct GIZMO_s {
  void *object;
} GIZMO;

i32 NuStrLen(const char *s);
void AddGizmo(GIZMOSYS_s *gizmo_sys, i32 type_id, void *a, void *object);
void *GameBufferAllocProgressUnk005bbaf0(VARIPTR *buf, VARIPTR *buf_end,
                                         i32 size);

// FUNCTION: LEGOBATMAN 0x004f04a0
void *Signals_ReserveBufferSpace(void *world_info) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_info;
  world->signals = NULL;
  world->signal_count = 0;
  if (world->current_level->max_signals > 0) {
    world->buf104.addr = (world->buf104.addr + 3) & ~3;
    world->signals = (SIGNAL *)world->buf104.void_ptr;
    world->buf104.addr += world->current_level->max_signals * sizeof(SIGNAL);
  }
  return world->signals;
}

// FUNCTION: LEGOBATMAN 0x004f0510
void Signal_Activate(GIZMO *gizmo, i32 active) {
  if (gizmo != NULL)
    ((SIGNAL *)gizmo->object)->active = active != 0;
}

// FUNCTION: LEGOBATMAN 0x004f0540
i32 Signal_ActivateRev(GIZMO *gizmo, i32 active, i32 reverse) {
  if (gizmo == NULL || gizmo->object == NULL)
    return 0;
  SIGNAL *signal = (SIGNAL *)gizmo->object;
  if ((reverse & 1) != 0)
    return active == signal->active;
  signal->active = active == 0;
  return 1;
}

// FUNCTION: LEGOBATMAN 0x004f0630
nuvec_s *Signal_GetPos(GIZMO *gizmo) {
  if (gizmo != NULL && gizmo->object != NULL)
    return &((SIGNAL *)gizmo->object)->position;
  return NULL;
}

// FUNCTION: LEGOBATMAN 0x004f0d60
void Signals_EarlyUpdate(void *world_info, void *unused, f32 dt) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_info;
  for (i32 i = 0; i < world->signal_count; i++)
    world->signals[i].f3d0 = 1.0f;
}

// FUNCTION: LEGOBATMAN 0x004f1f30
void Signals_AddGizmos(GIZMOSYS_s *gizmo_sys, i32 type_id, void *world_info,
                       void *unused) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_info;
  for (i32 i = 0; i < world->signal_count; i++) {
    if (NuStrLen(world->signals[i].name) != 0)
      AddGizmo(gizmo_sys, type_id, NULL, &world->signals[i]);
  }
}

// FUNCTION: LEGOBATMAN 0x004f1fb0
char *Signal_GetGizmoName(GIZMO *gizmo) {
  return gizmo != NULL ? ((SIGNAL *)gizmo->object)->name : NULL;
}

// FUNCTION: LEGOBATMAN 0x004f1fd0
i32 Signal_GetOutput(GIZMO *gizmo, i32 a, i32 b) {
  SIGNAL *signal = (SIGNAL *)gizmo->object;
  if (signal->visible && signal->active)
    return 1;
  return 0;
}

// FUNCTION: LEGOBATMAN 0x004f1ff0
char *Signal_GetOutputName(GIZMO *gizmo, i32 output) { return "Active"; }

// FUNCTION: LEGOBATMAN 0x004f2000
i32 Signal_GetNumOutputs(GIZMO *gizmo) { return 1; }

// FUNCTION: LEGOBATMAN 0x004f2010
void *Signals_AllocateProgressData(VARIPTR *buf, VARIPTR *buf_end) {
  return GameBufferAllocProgressUnk005bbaf0(buf, buf_end,
                                            sizeof(SIGNALPROGRESS));
}

// FUNCTION: LEGOBATMAN 0x004f2030
void Signals_ClearProgress(void *world, void *progress_data) {
  SIGNALPROGRESS *progress = (SIGNALPROGRESS *)progress_data;
  if (progress != NULL) {
    memset(progress->suit_letters, 0, sizeof(progress->suit_letters));
    progress->active_mask = 0xffffffff;
    progress->visible_mask = 0xffffffff;
  }
}
