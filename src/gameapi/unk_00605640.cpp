// gameapi/unk_00605640.cpp: TU of unknown name, found by its header-static
// copies (the functions after them are not matched yet).

#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"

// FUNCTION: LEGOBATMAN 0x00605640
static f32 NuSinApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x006056e0
static f32 NuCosApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x006056f0
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_00605640(f32 *v, f32 a, i32 i) {
  v[0] = NuSinApprox(i);
  v[1] = NuCosApprox(i);
  NuVec4Set(v, a, a, a, a);
}

// The rest of this TU is the Mac's Whipper file (.. Whippers_RegisterGizmo
// 0x606240); callbacks named by their RegisterGizmo slot.

#include "../batman/leveldata_unk.h"
#include "../batman/worldinfo_unk.h"
#include "../nu2api/numath/numtx.h"
#include <stddef.h>

typedef struct WHIPPER_s {
  char name[0x10];  // 0x00
  nuvec_s position; // 0x10
  nuvec_s v1c;      // 0x1c
  numtx_s mtx;      // 0x28
  f32 f68;          // 0x68
  u16 angle;        // 0x6c
  u8 b6e;           // 0x6e
  u8 active : 1;    // 0x6f
  u8 visible : 1;   // 0x6f bit 1
  u8 whipped : 1;   // 0x6f bit 2
  u8 in_use : 1;    // 0x6f bit 3
  i16 sfx;          // 0x70
  u8 b72;           // 0x72, 3 = silent
  u8 pad73[0x74 - 0x73];
  f32 f74; // 0x74
  u8 pad78[0x8c - 0x78];
  void *p8c; // 0x8c

} WHIPPER;

typedef struct WHIPPERSTATE_s {
  u8 pad0[0x28];
  i16 sfx; // 0x28
} WHIPPERSTATE;

typedef struct WHIPPERPROGRESS_s {
  u32 visible[1]; // 0x00
  u32 active[1];  // 0x04
  u32 whipped[1]; // 0x08
} WHIPPERPROGRESS;

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
// GLOBAL: LEGOBATMAN 0x00962ea4
extern WHIPPERSTATE *Whipper_State;
// GLOBAL: LEGOBATMAN 0x00962ea8
extern char *Whipper_OutputNames[];
// GLOBAL: LEGOBATMAN 0x00960534
extern i32 g_unk00960534; // the whip special move
// GLOBAL: LEGOBATMAN 0x00ab3960
extern GameObject_s *g_unk00ab3960[8];

i32 NuStrLen(const char *s);
void AddGizmo(GIZMOSYS_s *gizmo_sys, i32 type_id, void *a, void *object);
void *GameBufferAllocProgressUnk005bbaf0(VARIPTR *buf, VARIPTR *buf_end,
                                         i32 size);
i16 Unk00573e30(numtx_s *mtx, i32 sfx);
void Unk00574270(i32 handle);
i32 Whippers_Load(void *world, void *unused);

// FUNCTION: LEGOBATMAN 0x00605710
static void Whipper_RestartSfx(WHIPPER *whipper) {
  if (whipper->sfx != -1) {
    Unk00574270(whipper->sfx);
    whipper->sfx = -1;
  }
  if (Whipper_State->sfx != -1 && whipper->b72 != 3)
    whipper->sfx = Unk00573e30(&whipper->mtx, Whipper_State->sfx);
}

// FUNCTION: LEGOBATMAN 0x006058c0
void Whippers_RestartSfxUnk006058c0(WORLDINFO_s *world) {
  for (i32 i = 0; i < world->whipper_count; i++)
    Whipper_RestartSfx(&world->whippers[i]);
}

// FUNCTION: LEGOBATMAN 0x00605950
void *Whippers_ReserveBufferSpace(void *world_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  world->whippers = NULL;
  world->whipper_count = 0;
  if (world->current_level->max_whippers > 0) {
    world->buf104.addr = (world->buf104.addr + 3) & ~3;
    world->whippers = (WHIPPER *)world->buf104.void_ptr;
    world->buf104.addr += world->current_level->max_whippers * sizeof(WHIPPER);
  }
  return world->whippers;
}

void Whippers_Reset(void *world, void *unused, void *progress);

// FUNCTION: LEGOBATMAN 0x00605b20
void Whipper_Activate(GIZMO *gizmo, i32 active) {
  if (gizmo != NULL)
    ((WHIPPER *)gizmo->object)->active = active != 0;
}

// FUNCTION: LEGOBATMAN 0x00605b40
i32 Whipper_ActivateRev(GIZMO *gizmo, i32 value, i32 query) {
  if (gizmo != NULL && gizmo->object != NULL) {
    WHIPPER *whipper = (WHIPPER *)gizmo->object;
    if (query & 1) {
      if (whipper->active)
        return value;
      return value == 0;
    }
    whipper->active = value == 0;
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x00605b80
static void Whipper_SetVisible(WHIPPER *whipper, i32 visible) {
  whipper->visible = visible != 0;
  if (whipper->visible) {
    if (Whipper_State->sfx != -1 && whipper->sfx == -1 && whipper->b72 != 3)
      whipper->sfx = Unk00573e30(&whipper->mtx, Whipper_State->sfx);
  } else if (whipper->sfx != -1) {
    Unk00574270(whipper->sfx);
    whipper->sfx = -1;
  }
}

// FUNCTION: LEGOBATMAN 0x00605bf0
void Whipper_SetVisibility(GIZMO *gizmo, i32 visible) {
  if (gizmo != NULL) {
    WHIPPER *whipper = (WHIPPER *)gizmo->object;
    if (whipper != NULL)
      Whipper_SetVisible(whipper, visible);
  }
}

// FUNCTION: LEGOBATMAN 0x00605c10
void Whippers_EarlyUpdate(void *world_ptr, void *unused, f32 dt) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  WHIPPER *whipper = world->whippers;
  if (whipper == NULL)
    return;
  for (i32 i = 0; i < world->whipper_count; i++, whipper++) {
    if (whipper->whipped) {
      whipper->in_use = 1;
      continue;
    }
    whipper->in_use = 0;
    if (g_unk00960534 != -1) {
      for (i32 j = 0; j < 8; j++) {
        GameObject_s *obj = g_unk00ab3960[j];
        if (obj != NULL && obj->b9db == g_unk00960534 &&
            (obj->b9e1 == 7 || obj->b9e1 == 8) &&
            (WHIPPER *)obj->techno == whipper) {
          whipper->in_use = 1;
          break;
        }
      }
    }
  }
}

void Whippers_LateUpdate(void *world, void *unused, f32 dt);
void Whippers_Draw(void *world, void *unused, f32 dt);

// FUNCTION: LEGOBATMAN 0x00606060
i32 Whippers_GetMaxGizmos(void *world_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  if (world != NULL)
    return world->current_level->max_whippers;
  return 0;
}

// FUNCTION: LEGOBATMAN 0x00606080
void Whippers_AddGizmos(GIZMOSYS_s *gizmo_sys, i32 type_id, void *world_ptr,
                        void *unused) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  for (i32 i = 0; i < world->whipper_count; i++) {
    if (NuStrLen(world->whippers[i].name) != 0)
      AddGizmo(gizmo_sys, type_id, NULL, &world->whippers[i]);
  }
}

// FUNCTION: LEGOBATMAN 0x006060f0
char *Whipper_GetGizmoName(GIZMO *gizmo) {
  return gizmo != NULL ? ((WHIPPER *)gizmo->object)->name : NULL;
}

// FUNCTION: LEGOBATMAN 0x00606100
i32 Whipper_GetOutput(GIZMO *gizmo, i32 a, i32 b) {
  return ((WHIPPER *)gizmo->object)->whipped;
}

// FUNCTION: LEGOBATMAN 0x00606120
char *Whipper_GetOutputName(GIZMO *gizmo, i32 output) {
  return Whipper_OutputNames[output];
}

// FUNCTION: LEGOBATMAN 0x00606130
i32 Whipper_GetNumOutputs(GIZMO *gizmo) { return 1; }

// FUNCTION: LEGOBATMAN 0x00606140
void *Whippers_AllocateProgressData(VARIPTR *buf, VARIPTR *buf_end) {
  return GameBufferAllocProgressUnk005bbaf0(buf, buf_end,
                                            sizeof(WHIPPERPROGRESS));
}

// FUNCTION: LEGOBATMAN 0x00606160
void Whippers_ClearProgress(void *world, void *progress_ptr) {
  WHIPPERPROGRESS *progress = (WHIPPERPROGRESS *)progress_ptr;
  if (progress != NULL) {
    progress->active[0] = 0xffffffff;
    progress->visible[0] = 0xffffffff;
    progress->whipped[0] = 0;
  }
}

// FUNCTION: LEGOBATMAN 0x00606180
void Whippers_StoreProgress(void *world_ptr, void *unused, void *progress_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  WHIPPERPROGRESS *progress = (WHIPPERPROGRESS *)progress_ptr;
  if (progress == NULL)
    return;
  Whippers_ClearProgress(NULL, progress);
  if (world != NULL && world->whippers != NULL) {
    WHIPPER *whipper = world->whippers;
    for (i32 i = 0; i < world->whipper_count; i++, whipper++) {
      if (i >= 32)
        break;
      i32 word = i / 32;
      u32 bit = 1 << (i & 31);
      if (!whipper->active)
        progress->active[word] &= ~bit;
      if (!whipper->visible)
        progress->visible[word] &= ~bit;
      if (whipper->p8c == NULL && whipper->whipped && whipper->f74 <= 0.0f)
        progress->whipped[word] |= bit;
    }
  }
}

// GLOBAL: LEGOBATMAN 0x00aca6b0
i32 whipper_gizmotype_id;

// FUNCTION: LEGOBATMAN 0x00606240
ADDGIZMOTYPE *Whippers_RegisterGizmo(i32 type_id) {
  // GLOBAL: LEGOBATMAN 0x00962f70
  static char *name = "Whipper";
  // GLOBAL: LEGOBATMAN 0x00aca6c0
  static ADDGIZMOTYPE addtype;

  addtype = Default_ADDGIZMOTYPE;
  addtype.name = name;
  addtype.prefix = "";
  addtype.progress_size = sizeof(WHIPPERPROGRESS);
  addtype.fns[0] = (void *)Whippers_GetMaxGizmos;
  addtype.fns[1] = (void *)Whippers_AddGizmos;
  addtype.fns[2] = (void *)Whippers_EarlyUpdate;
  addtype.fns[3] = (void *)Whippers_LateUpdate;
  addtype.fns[4] = (void *)Whippers_Draw;
  addtype.fns[5] = NULL;
  addtype.fns[6] = (void *)Whipper_GetGizmoName;
  addtype.fns[7] = (void *)Whipper_GetOutput;
  addtype.fns[8] = (void *)Whipper_GetOutputName;
  addtype.fns[9] = (void *)Whipper_GetNumOutputs;
  addtype.fns[10] = (void *)Whipper_Activate;
  addtype.fns[11] = (void *)Whipper_ActivateRev;
  addtype.fns[12] = (void *)Whipper_SetVisibility;
  addtype.fns[13] = NULL;
  addtype.fns[14] = NULL;
  addtype.fns[15] = NULL;
  addtype.fns[16] = NULL;
  addtype.fns[17] = NULL;
  addtype.fns[18] = NULL;
  addtype.fns[19] = (void *)Whippers_AllocateProgressData;
  addtype.fns[20] = (void *)Whippers_ClearProgress;
  addtype.fns[21] = (void *)Whippers_StoreProgress;
  addtype.fns[22] = (void *)Whippers_Reset;
  addtype.fns[23] = (void *)Whippers_ReserveBufferSpace;
  addtype.fns[24] = (void *)Whippers_Load;
  addtype.fns[25] = NULL;
  addtype.fns[26] = NULL;
  addtype.fns[27] = NULL;
  whipper_gizmotype_id = type_id;
  return &addtype;
}
