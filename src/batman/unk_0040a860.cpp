// batman/unk_0040a860.cpp: TU of unknown name, found by its header-static
// copies (the functions after them are not matched yet).

#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"

// FUNCTION: LEGOBATMAN 0x0040a860
static f32 NuFabs(f32 f);
// FUNCTION: LEGOBATMAN 0x0040a880
static f32 NuSinApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x0040a940
static f32 NuCosApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x0040a960
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_0040a860(f32 *v, f32 a, i32 i) {
  v[2] = NuFabs(a);
  v[0] = NuSinApprox(i);
  v[1] = NuCosApprox(i);
  NuVec4Set(v, a, a, a, a);
}

#include "../nu2api/numath/nuvec.h"
#include "leveldata_unk.h"
#include "worldinfo_unk.h"
#include <stddef.h>
#include <string.h>

// The rest of this TU is the Mac's Attracto file (saga
// legoapi/gizmos/traps/attracto.cpp, Attractos_Load .. Attractos_RegisterGizmo
// 0x40bf60); callbacks named by their RegisterGizmo slot.

typedef struct ATTRACTO_s {
  u8 pad0[0x40];
  char name[0x10]; // 0x40
  nuvec_s pos;     // 0x50
  u8 pad5c[0x5f - 0x5c];
  u8 collected_count; // 0x5f
  u8 pad60[0x63 - 0x60];
  u8 active : 1;  // 0x63
  u8 visible : 1; // 0x63 bit 1
  u8 filled : 1;  // 0x63 bit 2
  u8 pad64[0x7c - 0x64];
} ATTRACTO;

typedef struct ATTRACTOPROGRESS_s {
  u8 counts[32];  // 0x00
  u32 visible[1]; // 0x20, bit per attracto
  u32 active[1];  // 0x24
  u32 filled[1];  // 0x28
} ATTRACTOPROGRESS;

typedef struct GIZMO_s {
  void *object;
} GIZMO;

void *GameBufferAllocProgressUnk005bbaf0(VARIPTR *buf, VARIPTR *buf_end,
                                         i32 size);

typedef struct GIZMOSYS_s GIZMOSYS;
i32 NuStrLen(const char *s);
void AddGizmo(GIZMOSYS *gizmo_sys, i32 type_id, void *a, void *object);

typedef struct ADDGIZMOTYPE_s {
  char *name;        // 0x00
  char *prefix;      // 0x04
  u16 progress_size; // 0x08
  void *fns[0x1c];   // 0x0c
} ADDGIZMOTYPE;

// GLOBAL: LEGOBATMAN 0x00960118
extern ADDGIZMOTYPE Default_ADDGIZMOTYPE;

// GLOBAL: LEGOBATMAN 0x0093578c
static char *Attracto_OutputName = "Filled";

// FUNCTION: LEGOBATMAN 0x0040ab10
void *Attractos_ReserveBufferSpace(void *context) {
  WORLDINFO_s *world = (WORLDINFO_s *)context;
  world->attractos = NULL;
  world->attracto_count = 0;
  if (world->current_level->max_attractos > 0) {
    world->buf104.addr = (world->buf104.addr + 0xf) & ~0xf;
    world->attractos = (ATTRACTO *)world->buf104.void_ptr;
    world->buf104.addr +=
        world->current_level->max_attractos * sizeof(ATTRACTO);
  }
  return world->attractos;
}

// FUNCTION: LEGOBATMAN 0x0040ab70
void Attracto_Activate(GIZMO *gizmo, i32 active) {
  if (gizmo != NULL) {
    ATTRACTO *attracto = (ATTRACTO *)gizmo->object;
    attracto->active = active != 0;
  }
}

// FUNCTION: LEGOBATMAN 0x0040bcc0
i32 Attractos_GetMaxGizmos(void *context) {
  WORLDINFO_s *world = (WORLDINFO_s *)context;
  return world != NULL ? world->current_level->max_attractos : 0;
}

// FUNCTION: LEGOBATMAN 0x0040bce0
void Attractos_AddGizmos(GIZMOSYS *gizmo_sys, i32 type, void *context,
                         void *unused) {
  WORLDINFO_s *world = (WORLDINFO_s *)context;
  for (i32 i = 0; i < world->attracto_count; i++) {
    if (NuStrLen(world->attractos[i].name) != 0)
      AddGizmo(gizmo_sys, type, NULL, &world->attractos[i]);
  }
}

// FUNCTION: LEGOBATMAN 0x0040bd60
char *Attracto_GetGizmoName(GIZMO *gizmo) {
  return gizmo != NULL ? ((ATTRACTO *)gizmo->object)->name : NULL;
}

// FUNCTION: LEGOBATMAN 0x0040bd80
i32 Attracto_GetOutput(GIZMO *gizmo, i32 a, i32 b) {
  return ((ATTRACTO *)gizmo->object)->filled;
}

// FUNCTION: LEGOBATMAN 0x0040bda0
char *Attracto_GetOutputName(GIZMO *gizmo, i32 output) {
  return Attracto_OutputName;
}

// FUNCTION: LEGOBATMAN 0x0040bdb0
i32 Attracto_GetNumOutputs(GIZMO *gizmo) { return 1; }

// FUNCTION: LEGOBATMAN 0x0040bdc0
void *Attractos_AllocateProgressData(VARIPTR *buf, VARIPTR *buf_end) {
  return GameBufferAllocProgressUnk005bbaf0(buf, buf_end,
                                            sizeof(ATTRACTOPROGRESS));
}

// FUNCTION: LEGOBATMAN 0x0040bde0
void Attractos_ClearProgress(void *world, void *data) {
  if (data != NULL) {
    ATTRACTOPROGRESS *progress = (ATTRACTOPROGRESS *)data;
    memset(progress->counts, 0, sizeof(progress->counts));
    progress->visible[0] = 0xffffffff;
    progress->active[0] = 0xffffffff;
    progress->filled[0] = 0;
  }
}

// STUB: LEGOBATMAN 0x0040be20
// only the inlined clear's `or eax, -1` is scheduled after `push ebp` in ours
// (call, open-coded, wrapped and break-loop forms tried).
void Attractos_StoreProgress(void *context, void *unused, void *data) {
  WORLDINFO_s *world = (WORLDINFO_s *)context;
  ATTRACTOPROGRESS *progress = (ATTRACTOPROGRESS *)data;
  if (progress == NULL)
    return;
  Attractos_ClearProgress(NULL, progress);
  if (world != NULL && world->attractos != NULL) {
    ATTRACTO *attracto = world->attractos;
    for (i32 i = 0; i < world->attracto_count; i++, attracto++) {
      if (i >= 32)
        break;
      progress->counts[i] = attracto->collected_count;
      i32 word = i / 32;
      u32 mask = 1 << (i & 31);
      if (!attracto->visible)
        progress->visible[word] &= ~mask;
      if (!attracto->active)
        progress->active[word] &= ~mask;
      if (attracto->filled)
        progress->filled[word] |= mask;
    }
  }
}

// FUNCTION: LEGOBATMAN 0x0040bf00
nuvec_s *Attracto_GetPos(GIZMO *gizmo) {
  if (gizmo == NULL)
    return NULL;
  return &((ATTRACTO *)gizmo->object)->pos;
}

void Attracto_SetVisibility(GIZMO *gizmo, i32 visible);
void Attractos_Reset(void *context, void *a, void *b);
i32 Attractos_Load(void *context, void *unused);
void Attractos_Update(void *context, void *unused, f32 dt);
void Attractos_Draw(void *context, void *unused, f32 dt);

// FUNCTION: LEGOBATMAN 0x0040bf60
ADDGIZMOTYPE *Attractos_RegisterGizmo(i32 type_id) {
  // GLOBAL: LEGOBATMAN 0x00935874
  static char *name = "Attracto";
  // GLOBAL: LEGOBATMAN 0x009be5e0
  static ADDGIZMOTYPE addtype;

  addtype = Default_ADDGIZMOTYPE;
  addtype.name = name;
  addtype.prefix = "";
  addtype.progress_size = sizeof(ATTRACTOPROGRESS);
  addtype.fns[0] = (void *)Attractos_GetMaxGizmos;
  addtype.fns[1] = (void *)Attractos_AddGizmos;
  addtype.fns[2] = NULL;
  addtype.fns[3] = (void *)Attractos_Update;
  addtype.fns[4] = (void *)Attractos_Draw;
  addtype.fns[5] = NULL;
  addtype.fns[6] = (void *)Attracto_GetGizmoName;
  addtype.fns[7] = (void *)Attracto_GetOutput;
  addtype.fns[8] = (void *)Attracto_GetOutputName;
  addtype.fns[9] = (void *)Attracto_GetNumOutputs;
  addtype.fns[10] = (void *)Attracto_Activate;
  addtype.fns[11] = NULL;
  addtype.fns[12] = (void *)Attracto_SetVisibility;
  addtype.fns[13] = NULL;
  addtype.fns[14] = (void *)Attracto_GetPos;
  addtype.fns[15] = NULL;
  addtype.fns[16] = NULL;
  addtype.fns[17] = NULL;
  addtype.fns[18] = NULL;
  addtype.fns[19] = (void *)Attractos_AllocateProgressData;
  addtype.fns[20] = (void *)Attractos_ClearProgress;
  addtype.fns[21] = (void *)Attractos_StoreProgress;
  addtype.fns[22] = (void *)Attractos_Reset;
  addtype.fns[23] = (void *)Attractos_ReserveBufferSpace;
  addtype.fns[24] = (void *)Attractos_Load;
  addtype.fns[25] = NULL;
  addtype.fns[26] = NULL;
  addtype.fns[27] = NULL;
  return &addtype;
}
