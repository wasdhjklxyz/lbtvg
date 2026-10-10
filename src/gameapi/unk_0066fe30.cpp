// gameapi/unk_0066fe30.cpp: TU of unknown name, found by its header-static
// copies (the functions after them are not matched yet).

#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"

// FUNCTION: LEGOBATMAN 0x0066fe30
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_0066fe30(f32 *v, f32 a, i32 i) { NuVec4Set(v, a, a, a, a); }

// The rest of this TU is the Mac's GizTimer file (saga
// legoapi/gizmos/trigger/giztimers.cpp, GizTimer_GetMaxGizmos ..
// GizTimer_RegisterGizmo 0x670150).

#include "../batman/leveldata_unk.h"
#include "../batman/worldinfo_unk.h"
#include <stddef.h>

typedef struct GIZTIMER_s {
  f32 time_remaining; // 0x00
  f32 start_time;     // 0x04
  u8 pad8[2];
  u8 active : 1;      // 0x0a
  u8 random_time : 1; // 0x0a bit 1
  u8 padb;
  char name[0x10]; // 0x0c
} GIZTIMER;

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

i32 NuStrLen(const char *s);
void NuStrNCpy(char *dst, const char *src, i32 n);
GIZMO *AddGizmo(GIZMOSYS_s *gizmo_sys, i32 type_id, void *a, void *object);
i32 qrand();
i32 EdFileReadInt();
f32 EdFileReadFloat();
u16 EdFileReadUnsignedShort();
void EdFileRead(void *buf, i32 len);
void EdFileReadNuVec(nuvec_s *v);
WORLDINFO_s *WorldInfo_CurrentlyLoading(void);

// GLOBAL: LEGOBATMAN 0x00968338
i32 giztimer_gizmotype_id = -1;

// FUNCTION: LEGOBATMAN 0x0066fe50
i32 GizTimer_GetMaxGizmos(void *world_info) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_info;
  if (world == NULL || world->current_level == NULL)
    return 0;
  return world->current_level->max_timers;
}

// FUNCTION: LEGOBATMAN 0x0066fe70
void GizTimer_AddGizmos(GIZMOSYS_s *gizmo_sys, i32 type_id, void *world_info,
                        void *unused) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_info;
  for (i32 i = 0; i < world->giz_timer_count; i++) {
    if (NuStrLen(world->giz_timers[i].name) == 0)
      continue;
    AddGizmo(gizmo_sys, type_id, NULL, &world->giz_timers[i]);
  }
}

// FUNCTION: LEGOBATMAN 0x0066fee0
void GizTimer_Update(void *world_info, void *unused, f32 delta_time) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_info;
  for (i32 i = 0; i < world->giz_timer_count; i++) {
    GIZTIMER *timer = &world->giz_timers[i];
    if (timer->time_remaining >= 0.0f)
      timer->time_remaining -= delta_time;
  }
}

// FUNCTION: LEGOBATMAN 0x0066ff30
char *GizTimer_GetGizmoName(GIZMO *gizmo) {
  return gizmo != NULL ? ((GIZTIMER *)gizmo->object)->name : NULL;
}

// FUNCTION: LEGOBATMAN 0x0066ff50
i32 GizTimer_GetOutput(GIZMO *gizmo, i32 a, i32 b) {
  GIZTIMER *timer = (GIZTIMER *)gizmo->object;
  if (timer->active)
    return timer->time_remaining <= 0.0f;
  return 0;
}

// FUNCTION: LEGOBATMAN 0x0066ff70
char *GizTimer_GetOutputName(GIZMO *gizmo, i32 output_index) { return "Ping"; }

// FUNCTION: LEGOBATMAN 0x0066ff80
i32 GizTimer_GetNumOutputs(GIZMO *gizmo) { return 1; }

// FUNCTION: LEGOBATMAN 0x0066ff90
void GizTimer_Activate(GIZMO *gizmo, i32 active) {
  GIZTIMER *timer = (GIZTIMER *)gizmo->object;
  if (timer->random_time)
    timer->time_remaining = qrand() * (1.0 / 65535.0) * timer->start_time;
  else
    timer->time_remaining = timer->start_time;
  if (active)
    timer->active = 1;
  else
    timer->active = 0;
}

// FUNCTION: LEGOBATMAN 0x0066ffd0
void *GizTimer_ReserveBufferSpace(void *world_info) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_info;
  world->giz_timers = NULL;
  world->giz_timer_count = 0;
  if (world->current_level->max_timers > 0) {
    world->buf104.addr = (world->buf104.addr + 3) & ~3;
    world->giz_timers = (GIZTIMER *)world->buf104.void_ptr;
    world->buf104.addr += world->current_level->max_timers * sizeof(GIZTIMER);
  }
  return world->giz_timers;
}

// FUNCTION: LEGOBATMAN 0x00670030
GIZMO *createGizTimer(void *unused, f32 time, i32 random_time, char *name) {
  WORLDINFO_s *world = WorldInfo_CurrentlyLoading();
  if (world == NULL || world->giz_timers == NULL ||
      world->giz_timer_count == world->current_level->max_timers)
    return NULL;
  GIZTIMER *timer = &world->giz_timers[world->giz_timer_count];
  timer->start_time = time;
  timer->random_time = random_time;
  NuStrNCpy(timer->name, name, sizeof(timer->name));
  world->giz_timer_count++;
  return AddGizmo(world->gizmoSys2b0c, giztimer_gizmotype_id, NULL, timer);
}

// FUNCTION: LEGOBATMAN 0x006700c0
i32 GizTimer_Load(void *world_info, void *unused) {
  nuvec_s vec;
  char buffer[16];
  WORLDINFO_s *world = (WORLDINFO_s *)world_info;
  if (world->giz_timer_count == 0) {
    EdFileReadInt();
    i32 count = EdFileReadInt();
    for (i32 i = 0; i < count; i++) {
      i32 length = EdFileReadInt();
      EdFileRead(buffer, length);
      EdFileReadFloat();
      EdFileReadUnsignedShort();
      EdFileReadNuVec(&vec);
    }
    return 1;
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x00670150
ADDGIZMOTYPE *GizTimer_RegisterGizmo(i32 type_id) {
  // GLOBAL: LEGOBATMAN 0x00ad28f0
  static ADDGIZMOTYPE addtype;

  addtype = Default_ADDGIZMOTYPE;
  addtype.name = "GizTimer";
  addtype.prefix = "";
  addtype.progress_size = 0;
  addtype.fns[0] = (void *)GizTimer_GetMaxGizmos;
  addtype.fns[1] = (void *)GizTimer_AddGizmos;
  addtype.fns[2] = (void *)GizTimer_Update;
  addtype.fns[3] = NULL;
  addtype.fns[4] = NULL;
  addtype.fns[5] = NULL;
  addtype.fns[6] = (void *)GizTimer_GetGizmoName;
  addtype.fns[7] = (void *)GizTimer_GetOutput;
  addtype.fns[8] = (void *)GizTimer_GetOutputName;
  addtype.fns[9] = (void *)GizTimer_GetNumOutputs;
  addtype.fns[10] = (void *)GizTimer_Activate;
  addtype.fns[11] = NULL;
  addtype.fns[12] = NULL;
  addtype.fns[13] = NULL;
  addtype.fns[14] = NULL;
  addtype.fns[15] = NULL;
  addtype.fns[16] = NULL;
  addtype.fns[17] = NULL;
  addtype.fns[18] = NULL;
  addtype.fns[19] = NULL;
  addtype.fns[20] = NULL;
  addtype.fns[21] = NULL;
  addtype.fns[22] = NULL;
  addtype.fns[23] = (void *)GizTimer_ReserveBufferSpace;
  addtype.fns[24] = (void *)GizTimer_Load;
  addtype.fns[25] = NULL;
  addtype.fns[26] = NULL;
  addtype.fns[27] = NULL;
  giztimer_gizmotype_id = type_id;
  return &addtype;
}
