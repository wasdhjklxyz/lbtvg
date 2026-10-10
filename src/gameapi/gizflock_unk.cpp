// gameapi/gizflock_unk.cpp: flock_extras config parser, between rtleditor.cpp
// and listman_gen.cpp; Mac puts it in the GizFlock file. File name unproven.

#include "../nu2api/nucore/common.h"
#include "../nu2api/numath/nuinline_unk.h"

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x00665780
static f32 NuFabs(f32 f);
// FUNCTION: LEGOBATMAN 0x00665a40
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);
// FUNCTION: LEGOBATMAN 0x00665a30
static void NuVecZeroInline(f32 *v);
// FUNCTION: LEGOBATMAN 0x00665ad0
static void NuVecScaleInline(f32 *dst, f32 *src, f32 s);

typedef struct nufpar_s NUFPAR;

f32 NuFParGetFloat(NUFPAR *parser);

typedef struct GIZFLOCKEXTRA_s {
  u32 pad0;
  f32 anim_speed; // 0x04
} GIZFLOCKEXTRA;

// GLOBAL: LEGOBATMAN 0x00ad258c
extern GIZFLOCKEXTRA *g_unk00ad258c;

// keyword "anim_speed" in table 0x00967db8
// FUNCTION: LEGOBATMAN 0x00665f60
void GizFlock_Config_anim_speed(NUFPAR *parser) {
  g_unk00ad258c->anim_speed = NuFParGetFloat(parser);
}

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_gizflock_unk(f32 *v, f32 a, i32 i) {
  v[2] = NuFabs(a);
  NuVec4Set(v, a, a, a, a);
  NuVecScaleInline(v + 8, v, a);
  NuVecZeroInline(v + 80);
}

// The gizmo callbacks (Mac GizFlock_*), named by their RegisterGizmo
// 0x66c150 slot.

#include "../batman/leveldata_unk.h"
#include "../batman/worldinfo_unk.h"
#include <stddef.h>

typedef struct sGizFlock {
  char name[0x10]; // 0x00
  u8 pad10[0x1c - 0x10];
  nuvec_s position; // 0x1c
  u8 pad28[0x1d0 - 0x28];
  u16 b0 : 1;
  u16 active : 1;    // 0x1d0 bit 1
  u16 triggered : 1; // 0x1d0 bit 2
  u16 finished : 1;  // 0x1d0 bit 3
  u8 pad1d2[0x1d4 - 0x1d2];
} GIZFLOCK;

struct sGizFlockSys {
  GIZFLOCK *flocks; // 0x00
  u16 count;        // 0x04
};

typedef struct GIZFLOCKPROGRESS_s {
  u32 a0[1]; // 0x00
  u32 a4[1]; // 0x04
  u32 a8[1]; // 0x08
  u32 ac[1]; // 0x0c
} GIZFLOCKPROGRESS;

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
void AddGizmo(GIZMOSYS_s *gizmo_sys, i32 type_id, void *a, void *object);
void *GameBufferAllocProgressUnk005bbaf0(VARIPTR *buf, VARIPTR *buf_end,
                                         i32 size);

// FUNCTION: LEGOBATMAN 0x00666400
i32 GizFlock_GetMaxGizmos(void *world_ptr) {
  return ((WORLDINFO_s *)world_ptr)->current_level->max_flocks;
}

// FUNCTION: LEGOBATMAN 0x00666420
void GizFlock_AddGizmos(GIZMOSYS_s *gizmo_sys, i32 type_id, void *world_ptr,
                        void *unused) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  if (world == NULL)
    return;
  for (i32 i = 0; i < world->gizflock_sys->count; i++) {
    if (NuStrLen(world->gizflock_sys->flocks[i].name) != 0)
      AddGizmo(gizmo_sys, type_id, NULL, &world->gizflock_sys->flocks[i]);
  }
}

// FUNCTION: LEGOBATMAN 0x00666560
char *GizFlock_GetGizmoName(GIZMO *gizmo) {
  return gizmo != NULL ? ((GIZFLOCK *)gizmo->object)->name : NULL;
}

// FUNCTION: LEGOBATMAN 0x00666570
i32 GizFlock_GetOutput(GIZMO *gizmo, i32 output, i32 b) {
  if (gizmo != NULL && gizmo->object != NULL) {
    GIZFLOCK *flock = (GIZFLOCK *)gizmo->object;
    if (flock->active || b != 0) {
      switch (output) {
      case 0:
        if (flock->triggered)
          return 1;
        break;
      case 1:
        if (flock->finished)
          return 1;
        break;
      }
    }
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x006665c0
char *GizFlock_GetOutputName(GIZMO *gizmo, i32 output) {
  switch (output) {
  case 0:
    return "Triggered";
  case 1:
    return "Finished";
  }
  return NULL;
}

// FUNCTION: LEGOBATMAN 0x006665e0
i32 GizFlock_GetNumOutputs(GIZMO *gizmo) { return 2; }

// FUNCTION: LEGOBATMAN 0x006665f0
nuvec_s *GizFlock_GetPos(GIZMO *gizmo) {
  if (gizmo != NULL && gizmo->object != NULL)
    return &((GIZFLOCK *)gizmo->object)->position;
  return NULL;
}

// FUNCTION: LEGOBATMAN 0x00667300
void *GizFlock_AllocateProgressData(VARIPTR *buf, VARIPTR *buf_end) {
  return GameBufferAllocProgressUnk005bbaf0(buf, buf_end,
                                            sizeof(GIZFLOCKPROGRESS));
}

// FUNCTION: LEGOBATMAN 0x00667320
void GizFlock_ClearProgress(void *world, void *progress_ptr) {
  GIZFLOCKPROGRESS *progress = (GIZFLOCKPROGRESS *)progress_ptr;
  if (progress != NULL) {
    progress->a0[0] = 0xffffffff;
    progress->a4[0] = 0xffffffff;
    progress->a8[0] = 0;
    progress->ac[0] = 0;
  }
}

void GizFlock_EarlyUpdate(void *world, void *sys, f32 dt);
void GizFlock_Draw(void *world, void *sys, f32 dt);
void GizFlock_Activate(GIZMO *gizmo, i32 active);
void GizFlock_StoreProgress(void *world, void *unused, void *progress);
void GizFlock_Reset(void *world, void *unused, void *progress);
void *GizFlock_ReserveBufferSpace(void *world);
i32 GizFlock_Load(void *world, void *unused);

// GLOBAL: LEGOBATMAN 0x00967db4
i32 flock_gizmotype_id = -1;

// FUNCTION: LEGOBATMAN 0x0066c150
ADDGIZMOTYPE *GizFlock_RegisterGizmo(i32 type_id) {
  // GLOBAL: LEGOBATMAN 0x00ad2598
  static ADDGIZMOTYPE addtype;

  addtype = Default_ADDGIZMOTYPE;
  addtype.progress_size = 0;
  addtype.fns[3] = NULL;
  addtype.fns[5] = NULL;
  addtype.fns[11] = NULL;
  addtype.fns[12] = NULL;
  addtype.fns[13] = NULL;
  addtype.fns[15] = NULL;
  addtype.fns[16] = NULL;
  addtype.fns[17] = NULL;
  addtype.fns[18] = NULL;
  addtype.fns[25] = NULL;
  addtype.fns[26] = NULL;
  addtype.fns[27] = NULL;
  flock_gizmotype_id = type_id;
  addtype.name = "GizFlock";
  addtype.prefix = "";
  addtype.fns[0] = (void *)GizFlock_GetMaxGizmos;
  addtype.fns[1] = (void *)GizFlock_AddGizmos;
  addtype.fns[2] = (void *)GizFlock_EarlyUpdate;
  addtype.fns[4] = (void *)GizFlock_Draw;
  addtype.fns[6] = (void *)GizFlock_GetGizmoName;
  addtype.fns[7] = (void *)GizFlock_GetOutput;
  addtype.fns[8] = (void *)GizFlock_GetOutputName;
  addtype.fns[9] = (void *)GizFlock_GetNumOutputs;
  addtype.fns[10] = (void *)GizFlock_Activate;
  addtype.fns[14] = (void *)GizFlock_GetPos;
  addtype.fns[19] = (void *)GizFlock_AllocateProgressData;
  addtype.fns[20] = (void *)GizFlock_ClearProgress;
  addtype.fns[21] = (void *)GizFlock_StoreProgress;
  addtype.fns[22] = (void *)GizFlock_Reset;
  addtype.fns[23] = (void *)GizFlock_ReserveBufferSpace;
  addtype.fns[24] = (void *)GizFlock_Load;
  return &addtype;
}
