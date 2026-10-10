// gameapi/unk_00629750.cpp: TU of unknown name, found by its header-static
// copies (the functions after them are not matched yet).

#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"

// FUNCTION: LEGOBATMAN 0x00629750
static f32 NuSinApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x006297f0
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_00629750(f32 *v, f32 a, i32 i) {
  v[0] = NuSinApprox(i);
  NuVec4Set(v, a, a, a, a);
}

// The rest of this TU is the Mac's SecurityDoor file (SecurityDoors_Load ..
// SecurityDoors_RegisterGizmo 0x62bd30); callbacks named by their
// RegisterGizmo slot.

#include "../batman/leveldata_unk.h"
#include "../batman/worldinfo_unk.h"
#include <stddef.h>

typedef struct SECURITYDOOR_s {
  u8 pad0[0x140];
  char name[0x10];  // 0x140
  nuvec_s position; // 0x150
  u8 pad15c[0x160 - 0x15c];
  i16 plats[5]; // 0x160, -1 = none
  u8 pad16a[0x16c - 0x16a];
  u16 active : 1;  // 0x16c
  u16 visible : 1; // 0x16c bit 1
  u16 open : 1;    // 0x16c bit 2
  u16 b3 : 1;
  u16 b4 : 1;
  u16 b5 : 1;
  u16 b6 : 1;
  u16 b7 : 1; // 0x16c bit 7, cleared every frame
  u16 b8 : 1; // 0x16c bit 8, cleared every frame
  u16 state;  // 0x16e, 8 = fully open
  u8 pad170[0x1d0 - 0x170];
} SECURITYDOOR;

typedef struct SECURITYDOORSYS_s {
  SECURITYDOOR *doors; // 0x00
  i32 count;           // 0x04
} SECURITYDOORSYS;

typedef struct SECURITYDOORPROGRESS_s {
  u32 visible[1]; // 0x00
  u32 active[1];  // 0x04
  u32 open[1];    // 0x08
} SECURITYDOORPROGRESS;

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
// GLOBAL: LEGOBATMAN 0x00964b08
extern char *SecurityDoor_OutputName;

i32 NuStrLen(const char *s);
void AddGizmo(GIZMOSYS_s *gizmo_sys, i32 type_id, void *a, void *object);
void *GameBufferAlloc(variptr_u *buf, variptr_u *buf_end, i32 size);
void *GameBufferAllocProgressUnk005bbaf0(VARIPTR *buf, VARIPTR *buf_end,
                                         i32 size);
extern "C" void PlatOnOff(i32 index, i32 enabled);
i32 SecurityDoors_Load(void *world, void *unused);

// STUB: LEGOBATMAN 0x00629e70
// Ours passes `on` in ebx; the original takes it on the stack and loads ebx
// itself (the AIScriptCopyString convention puzzle).
static void SecurityDoor_PlatsOnOff(SECURITYDOOR *door, i32 on) {
  i16 *plat = door->plats;
  i32 n = 5;
  do {
    if (*plat != -1)
      PlatOnOff(*plat, on);
    plat++;
  } while (--n);
}

// FUNCTION: LEGOBATMAN 0x00629ef0
void *SecurityDoors_ReserveBufferSpace(void *world_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  SECURITYDOORSYS *sys = NULL;
  if (world->current_level->max_securitydoors > 0) {
    sys = (SECURITYDOORSYS *)GameBufferAlloc(&world->buf104, &world->bufEnd108,
                                             sizeof(SECURITYDOORSYS));
    if (sys != NULL) {
      world->securitydoor_sys = sys;
      sys->doors = (SECURITYDOOR *)GameBufferAlloc(
          &world->buf104, &world->bufEnd108,
          world->current_level->max_securitydoors * sizeof(SECURITYDOOR));
    }
  }
  return sys;
}

// STUB: LEGOBATMAN 0x00629f60
// Matches once SecurityDoor_PlatsOnOff gets the original's convention.
void SecurityDoor_SetVisibility(GIZMO *gizmo, i32 visible) {
  if (gizmo != NULL && gizmo->object != NULL) {
    SECURITYDOOR *door = (SECURITYDOOR *)gizmo->object;
    door->visible = visible != 0;
    SecurityDoor_PlatsOnOff(door, door->visible);
  }
}

void SecurityDoors_Reset(void *world, void *sys, void *progress);

// FUNCTION: LEGOBATMAN 0x0062a5c0
void SecurityDoors_EarlyUpdate(void *world, void *sys_ptr, f32 dt) {
  SECURITYDOORSYS *sys = (SECURITYDOORSYS *)sys_ptr;
  if (world != NULL && sys != NULL) {
    SECURITYDOOR *door = sys->doors;
    if (door != NULL) {
      for (i32 i = 0; i < sys->count; i++, door++) {
        door->b7 = 0;
        door->b8 = 0;
      }
    }
  }
}

void SecurityDoors_LateUpdate(void *world, void *sys, f32 dt);
void SecurityDoors_Draw(void *world, void *sys, f32 dt);

// FUNCTION: LEGOBATMAN 0x0062b240
i32 SecurityDoors_GetMaxGizmos(void *world_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  if (world != NULL)
    return world->current_level->max_securitydoors;
  return 0;
}

// FUNCTION: LEGOBATMAN 0x0062b260
void SecurityDoors_AddGizmos(GIZMOSYS_s *gizmo_sys, i32 type_id, void *world,
                             void *sys_ptr) {
  SECURITYDOORSYS *sys = (SECURITYDOORSYS *)sys_ptr;
  if (sys == NULL)
    return;
  for (i32 i = 0; i < sys->count; i++) {
    if (NuStrLen(sys->doors[i].name) != 0)
      AddGizmo(gizmo_sys, type_id, NULL, &sys->doors[i]);
  }
}

// FUNCTION: LEGOBATMAN 0x0062b2c0
char *SecurityDoor_GetGizmoName(GIZMO *gizmo) {
  return gizmo != NULL ? ((SECURITYDOOR *)gizmo->object)->name : NULL;
}

// FUNCTION: LEGOBATMAN 0x0062b2e0
i32 SecurityDoor_GetOutput(GIZMO *gizmo, i32 a, i32 b) {
  SECURITYDOOR *door = (SECURITYDOOR *)gizmo->object;
  if (door->open && door->state == 8)
    return 1;
  return 0;
}

// FUNCTION: LEGOBATMAN 0x0062b310
char *SecurityDoor_GetOutputName(GIZMO *gizmo, i32 output) {
  return SecurityDoor_OutputName;
}

// FUNCTION: LEGOBATMAN 0x0062b320
i32 SecurityDoor_GetNumOutputs(GIZMO *gizmo) { return 1; }

// FUNCTION: LEGOBATMAN 0x0062b330
void *SecurityDoors_AllocateProgressData(VARIPTR *buf, VARIPTR *buf_end) {
  return GameBufferAllocProgressUnk005bbaf0(buf, buf_end,
                                            sizeof(SECURITYDOORPROGRESS));
}

// FUNCTION: LEGOBATMAN 0x0062b350
void SecurityDoors_ClearProgress(void *world, void *progress_ptr) {
  SECURITYDOORPROGRESS *progress = (SECURITYDOORPROGRESS *)progress_ptr;
  if (progress != NULL) {
    progress->visible[0] = 0xffffffff;
    progress->active[0] = 0xffffffff;
    progress->open[0] = 0;
  }
}

// FUNCTION: LEGOBATMAN 0x0062b370
void SecurityDoors_StoreProgress(void *world, void *sys_ptr,
                                 void *progress_ptr) {
  SECURITYDOORPROGRESS *progress = (SECURITYDOORPROGRESS *)progress_ptr;
  SECURITYDOORSYS *sys = (SECURITYDOORSYS *)sys_ptr;
  SecurityDoors_ClearProgress(NULL, progress);
  if (sys != NULL && progress != NULL) {
    SECURITYDOOR *door = sys->doors;
    for (i32 i = 0; i < sys->count; i++, door++) {
      if (i >= 32)
        break;
      i32 word = i / 32;
      u32 bit = 1 << (i & 31);
      if (!door->visible)
        progress->visible[word] &= ~bit;
      if (!door->active)
        progress->active[word] &= ~bit;
      if (door->open)
        progress->open[word] |= bit;
    }
  }
}

// FUNCTION: LEGOBATMAN 0x0062b400
nuvec_s *SecurityDoor_GetPos(GIZMO *gizmo) {
  if (gizmo == NULL)
    return NULL;
  return &((SECURITYDOOR *)gizmo->object)->position;
}

void SecurityDoors_AddLevelSfx(void *world, void *sys);
void SecurityDoor_Activate(GIZMO *gizmo, i32 active);

// FUNCTION: LEGOBATMAN 0x0062bd30
ADDGIZMOTYPE *SecurityDoors_RegisterGizmo(i32 type_id) {
  // GLOBAL: LEGOBATMAN 0x00964bb0
  static char *name = "SecurityDoor";
  // GLOBAL: LEGOBATMAN 0x00acd778
  static ADDGIZMOTYPE addtype;

  addtype = Default_ADDGIZMOTYPE;
  addtype.name = name;
  addtype.prefix = "";
  addtype.progress_size = sizeof(SECURITYDOORPROGRESS);
  addtype.fns[0] = (void *)SecurityDoors_GetMaxGizmos;
  addtype.fns[1] = (void *)SecurityDoors_AddGizmos;
  addtype.fns[2] = (void *)SecurityDoors_EarlyUpdate;
  addtype.fns[3] = (void *)SecurityDoors_LateUpdate;
  addtype.fns[4] = (void *)SecurityDoors_Draw;
  addtype.fns[5] = NULL;
  addtype.fns[6] = (void *)SecurityDoor_GetGizmoName;
  addtype.fns[7] = (void *)SecurityDoor_GetOutput;
  addtype.fns[8] = (void *)SecurityDoor_GetOutputName;
  addtype.fns[9] = (void *)SecurityDoor_GetNumOutputs;
  addtype.fns[10] = (void *)SecurityDoor_Activate;
  addtype.fns[11] = NULL;
  addtype.fns[12] = (void *)SecurityDoor_SetVisibility;
  addtype.fns[13] = NULL;
  addtype.fns[14] = (void *)SecurityDoor_GetPos;
  addtype.fns[15] = NULL;
  addtype.fns[16] = NULL;
  addtype.fns[17] = NULL;
  addtype.fns[18] = NULL;
  addtype.fns[19] = (void *)SecurityDoors_AllocateProgressData;
  addtype.fns[20] = (void *)SecurityDoors_ClearProgress;
  addtype.fns[21] = (void *)SecurityDoors_StoreProgress;
  addtype.fns[22] = (void *)SecurityDoors_Reset;
  addtype.fns[23] = (void *)SecurityDoors_ReserveBufferSpace;
  addtype.fns[24] = (void *)SecurityDoors_Load;
  addtype.fns[25] = NULL;
  addtype.fns[26] = NULL;
  addtype.fns[27] = (void *)SecurityDoors_AddLevelSfx;
  return &addtype;
}
