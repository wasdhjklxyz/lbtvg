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
  u32 special5c[3]; // 0x5c, nuhspecial_s
  i16 s68;          // 0x68
  u8 pad6a[0x74 - 0x6a];
  char b74; // 0x74
  char b75; // 0x75
  u8 pad76;
  u8 visible : 1;      // 0x77
  u8 active : 1;       // 0x77 bit 1
  u8 b77_2 : 1;        // 0x77 bit 2
  u8 has_platform : 1; // 0x77 bit 3
  u8 pad78[0x9c - 0x78];
  nuvec_s v9c;     // 0x9c
  i16 platform_id; // 0xa8
  u16 saa;         // 0xaa
  u8 padac[0xb0 - 0xac];
  struct GIZMOBLOWUP_s *blowups[(0x3d0 - 0xb0) / 4]; // 0xb0
  f32 f3d0;                                          // 0x3d0
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

i32 EdFileReadInt();
char EdFileReadChar();
i16 EdFileReadShort();
void EdFileRead(void *buf, i32 len);
void EdFileReadNuVec(nuvec_s *v);
u8 EdFileReadUnsignedChar();
int NuSpecialFind(nugscn_s *scene, nuhspecial_s *out, char *name, i32 a);
int NuSpecialExistsFn(nuhspecial_s *sp);
struct GIZMOBLOWUP_s *GizmoBlowUp_FindByName(WORLDINFO_s *world, char *name);

// FUNCTION: LEGOBATMAN 0x004f0100
i32 Signals_Load(void *world_ptr, void *unused) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  char special_name[0x100];
  char blowup_name[0x100];
  i32 i = 0;
  if (world->signal_count == 0) {
    i32 version = EdFileReadInt();
    world->signal_count = EdFileReadInt();
    for (i = 0; i < world->signal_count; i++) {
      SIGNAL *sig = &world->signals[i];
      EdFileRead(world->signals[i].name, 0x10);
      EdFileReadNuVec(&world->signals[i].position);
      world->signals[i].b74 = EdFileReadChar();
      if (version >= 2) {
        world->signals[i].b75 = EdFileReadChar();
        world->signals[i].s68 = EdFileReadShort();
      } else {
        world->signals[i].b75 = 'b';
        world->signals[i].s68 = 0;
      }
      if (version >= 4) {
        world->signals[i].saa = EdFileReadShort();
        EdFileReadNuVec(&world->signals[i].v9c);
        if (version < 6 && !(world->signals[i].saa & 1)) {
          world->signals[i].saa |= 1;
          world->signals[i].v9c = world->signals[i].position;
        }
      } else {
        world->signals[i].saa = 0;
        world->signals[i].v9c = world->signals[i].position;
      }
      if (version >= 5) {
        i32 len = EdFileReadChar();
        if (len != 0) {
          memset(special_name, 0, sizeof(special_name));
          EdFileRead(special_name, len);
          NuSpecialFind(world->scn140,
                        (nuhspecial_s *)world->signals[i].special5c,
                        special_name, 0);
          if (!NuSpecialExistsFn((nuhspecial_s *)world->signals[i].special5c))
            memset(world->signals[i].special5c, 0,
                   sizeof(world->signals[i].special5c));
        }
      }
      if (version >= 7) {
        i32 n = EdFileReadUnsignedChar();
        i32 k = 0;
        for (i32 j = 0; j < n; j++) {
          i32 len = EdFileReadChar();
          if (len != 0) {
            EdFileRead(blowup_name, len);
            blowup_name[len] = 0;
            struct GIZMOBLOWUP_s *b =
                GizmoBlowUp_FindByName(world, blowup_name);
            if (b != NULL) {
              world->signals[i].blowups[k] = b;
              k++;
            }
          }
        }
      }
    }
    return 1;
  }
  return 0;
}

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

i16 NewPlatPickupInst(void *m, i32 type);
void DeletePlatinst(i32 id);

// FUNCTION: LEGOBATMAN 0x004f05a0
static void Signal_SetVisibility(SIGNAL *signal, i32 visible) {
  u8 was_visible = signal->visible;
  signal->visible = visible != 0;
  if (signal->visible && !was_visible) {
    signal->platform_id = NewPlatPickupInst(signal, 2);
    signal->has_platform = 1;
  } else if (!signal->visible && was_visible) {
    DeletePlatinst(signal->platform_id);
  }
}

// FUNCTION: LEGOBATMAN 0x004f0610
void Signal_SetGizmoVisibility(GIZMO *gizmo, i32 visible) {
  if (gizmo != NULL)
    Signal_SetVisibility((SIGNAL *)gizmo->object, visible);
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

// FUNCTION: LEGOBATMAN 0x004f1f10
i32 Signals_GetMaxGizmos(void *world_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  if (world != NULL)
    return world->current_level->max_signals;
  return 0;
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

typedef struct ADDGIZMOTYPE_s {
  char *name;        // 0x00
  char *prefix;      // 0x04
  u16 progress_size; // 0x08
  void *fns[0x1c];   // 0x0c
} ADDGIZMOTYPE;

// GLOBAL: LEGOBATMAN 0x00960118
extern ADDGIZMOTYPE Default_ADDGIZMOTYPE;

void Signals_LateUpdate(void *world, void *unused, f32 dt);
void Signals_Draw(void *world, void *unused, f32 dt);
void Signals_StoreProgress(void *world, void *unused, void *progress);
void Signals_Reset(void *world, void *unused, void *progress);
i32 Signals_Load(void *world, void *unused);

// FUNCTION: LEGOBATMAN 0x004f2170
ADDGIZMOTYPE *Signals_RegisterGizmo(i32 type_id) {
  // GLOBAL: LEGOBATMAN 0x00943f64
  static char *name = "Signal";
  // GLOBAL: LEGOBATMAN 0x009ccdb8
  static ADDGIZMOTYPE addtype;

  addtype = Default_ADDGIZMOTYPE;
  addtype.name = name;
  addtype.prefix = "";
  addtype.progress_size = sizeof(SIGNALPROGRESS);
  addtype.fns[0] = (void *)Signals_GetMaxGizmos;
  addtype.fns[1] = (void *)Signals_AddGizmos;
  addtype.fns[2] = (void *)Signals_EarlyUpdate;
  addtype.fns[3] = (void *)Signals_LateUpdate;
  addtype.fns[4] = (void *)Signals_Draw;
  addtype.fns[5] = NULL;
  addtype.fns[6] = (void *)Signal_GetGizmoName;
  addtype.fns[7] = (void *)Signal_GetOutput;
  addtype.fns[8] = (void *)Signal_GetOutputName;
  addtype.fns[9] = (void *)Signal_GetNumOutputs;
  addtype.fns[10] = (void *)Signal_Activate;
  addtype.fns[11] = (void *)Signal_ActivateRev;
  addtype.fns[12] = (void *)Signal_SetGizmoVisibility;
  addtype.fns[13] = NULL;
  addtype.fns[14] = (void *)Signal_GetPos;
  addtype.fns[15] = NULL;
  addtype.fns[16] = NULL;
  addtype.fns[17] = NULL;
  addtype.fns[18] = NULL;
  addtype.fns[19] = (void *)Signals_AllocateProgressData;
  addtype.fns[20] = (void *)Signals_ClearProgress;
  addtype.fns[21] = (void *)Signals_StoreProgress;
  addtype.fns[22] = (void *)Signals_Reset;
  addtype.fns[23] = (void *)Signals_ReserveBufferSpace;
  addtype.fns[24] = (void *)Signals_Load;
  addtype.fns[25] = NULL;
  addtype.fns[26] = NULL;
  addtype.fns[27] = NULL;
  return &addtype;
}
