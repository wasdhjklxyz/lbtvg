// gameapi/unk_00649780.cpp: TU of unknown name, found by its header-static
// copies (the functions after them are not matched yet).

#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x00649850
static void NuVecZeroInline(f32 *v);

// FUNCTION: LEGOBATMAN 0x00649780
static f32 NuFabs(f32 f);
// FUNCTION: LEGOBATMAN 0x006497a0
static f32 NuSinApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x00649840
static f32 NuCosApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x00649860
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_00649780(f32 *v, f32 a, i32 i) {
  v[2] = NuFabs(a);
  v[0] = NuSinApprox(i);
  v[1] = NuCosApprox(i);
  NuVec4Set(v, a, a, a, a);
}

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_unk_00649780(f32 *v, f32 a, i32 i) {
  NuVecZeroInline(v + 80);
}

// The rest of this TU is the Mac's TightRope file (TightRopes_Load ..
// TightRopes_RegisterGizmo 0x64ac10); callbacks named by their
// RegisterGizmo slot.

#include "../batman/leveldata_unk.h"
#include "../batman/worldinfo_unk.h"
#include <stddef.h>

typedef struct TIGHTROPE_s {
  nuvec_s position; // 0x00
  u8 padc[0x3c - 0xc];
  char name[0x10]; // 0x3c
  u8 pad4c[0x5e - 0x4c];
  u8 visible : 1; // 0x5e
  u8 active : 1;  // 0x5e bit 1
  u8 users;       // 0x5f
  f32 appear;     // 0x60
} TIGHTROPE;

typedef struct TIGHTROPEPROGRESS_s {
  u32 active[1];  // 0x00
  u32 visible[1]; // 0x04
} TIGHTROPEPROGRESS;

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
// GLOBAL: LEGOBATMAN 0x009604ec
extern i32 g_unk009604ec; // tightrope special moves
// GLOBAL: LEGOBATMAN 0x009604f0
extern i32 g_unk009604f0;
// GLOBAL: LEGOBATMAN 0x009604f4
extern i32 g_unk009604f4;
// GLOBAL: LEGOBATMAN 0x00a95fe0
extern f32 FRAMETIME;

i32 NuStrLen(const char *s);
void AddGizmo(GIZMOSYS_s *gizmo_sys, i32 type_id, void *a, void *object);
void *GameBufferAllocProgressUnk005bbaf0(VARIPTR *buf, VARIPTR *buf_end,
                                         i32 size);
void PlaySfx(char *name, nuvec_s *position);
void AddLevelSfxFromName(char *sfx_name, i32 *sfx_ids, i32 *sfx_count,
                         i32 max_sfx);
i32 TightRopes_Load(void *world, void *unused);

// FUNCTION: LEGOBATMAN 0x006499d0
void *TightRopes_ReserveBufferSpace(void *world_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  world->tightropes = NULL;
  world->tightrope_count = 0;
  if (world->current_level->max_tightropes > 0) {
    world->buf104.addr = (world->buf104.addr + 3) & ~3;
    world->tightropes = (TIGHTROPE *)world->buf104.void_ptr;
    world->buf104.addr +=
        world->current_level->max_tightropes * sizeof(TIGHTROPE);
  }
  return world->tightropes;
}

// FUNCTION: LEGOBATMAN 0x00649ac0
void TightRope_Release(GameObject_s *obj) {
  if (obj->b9db != -1 &&
      (obj->b9db == g_unk009604ec || obj->b9db == g_unk009604f0 ||
       obj->b9db == g_unk009604f4)) {
    if (obj->techno != NULL && ((TIGHTROPE *)obj->techno)->users > 0)
      ((TIGHTROPE *)obj->techno)->users--;
  }
}

void TightRopes_Reset(void *world, void *unused, void *progress);

// FUNCTION: LEGOBATMAN 0x0064a000
void TightRopes_EarlyUpdate(void *world_ptr, void *unused, f32 dt) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  TIGHTROPE *rope = world->tightropes;
  if (rope == NULL)
    return;
  for (i32 i = 0; i < world->tightrope_count; i++, rope++) {
    if (rope->active && rope->appear > 0.0f) {
      rope->appear = rope->appear - FRAMETIME * 1.5;
      if (rope->appear < 0.0f)
        rope->appear = 0.0f;
    }
  }
}

void TightRopes_LateUpdate(void *world, void *unused, f32 dt);
void TightRopes_Draw(void *world, void *unused, f32 dt);

// FUNCTION: LEGOBATMAN 0x0064a9d0
void TightRope_Activate(GIZMO *gizmo, i32 active) {
  if (gizmo != NULL && gizmo->object != NULL) {
    TIGHTROPE *rope = (TIGHTROPE *)gizmo->object;
    rope->active = active != 0;
    if (active) {
      rope->appear = 1.0f;
      PlaySfx("TightRope_App", &rope->position);
    }
  }
}

// FUNCTION: LEGOBATMAN 0x0064aa10
void TightRope_SetVisibility(GIZMO *gizmo, i32 visible) {
  if (gizmo != NULL)
    ((TIGHTROPE *)gizmo->object)->visible = visible != 0;
}

// FUNCTION: LEGOBATMAN 0x0064aa30
i32 TightRopes_GetMaxGizmos(void *world_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  if (world != NULL)
    return world->current_level->max_tightropes;
  return 0;
}

// FUNCTION: LEGOBATMAN 0x0064aa50
void TightRopes_AddGizmos(GIZMOSYS_s *gizmo_sys, i32 type_id, void *world_ptr,
                          void *unused) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  for (i32 i = 0; i < world->tightrope_count; i++) {
    if (NuStrLen(world->tightropes[i].name) != 0)
      AddGizmo(gizmo_sys, type_id, NULL, &world->tightropes[i]);
  }
}

// FUNCTION: LEGOBATMAN 0x0064aac0
char *TightRope_GetGizmoName(GIZMO *gizmo) {
  return gizmo != NULL ? ((TIGHTROPE *)gizmo->object)->name : NULL;
}

// FUNCTION: LEGOBATMAN 0x0064aae0
i32 TightRope_GetOutput(GIZMO *gizmo, i32 a, i32 b) {
  TIGHTROPE *rope = (TIGHTROPE *)gizmo->object;
  if (rope->visible && rope->active)
    return 1;
  return 0;
}

// FUNCTION: LEGOBATMAN 0x0064ab00
char *TightRope_GetOutputName(GIZMO *gizmo, i32 output) { return "Active"; }

// FUNCTION: LEGOBATMAN 0x0064ab10
i32 TightRope_GetNumOutputs(GIZMO *gizmo) { return 1; }

// FUNCTION: LEGOBATMAN 0x0064ab20
void *TightRopes_AllocateProgressData(VARIPTR *buf, VARIPTR *buf_end) {
  return GameBufferAllocProgressUnk005bbaf0(buf, buf_end,
                                            sizeof(TIGHTROPEPROGRESS));
}

// FUNCTION: LEGOBATMAN 0x0064ab40
void TightRopes_ClearProgress(void *world, void *progress_ptr) {
  TIGHTROPEPROGRESS *progress = (TIGHTROPEPROGRESS *)progress_ptr;
  if (progress != NULL) {
    progress->active[0] = 0xffffffff;
    progress->visible[0] = 0xffffffff;
  }
}

// FUNCTION: LEGOBATMAN 0x0064ab60
void TightRopes_StoreProgress(void *world_ptr, void *unused,
                              void *progress_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  TIGHTROPEPROGRESS *progress = (TIGHTROPEPROGRESS *)progress_ptr;
  if (progress == NULL)
    return;
  TightRopes_ClearProgress(NULL, progress);
  if (world != NULL && world->tightropes != NULL) {
    TIGHTROPE *rope = world->tightropes;
    for (i32 i = 0; i < world->tightrope_count; i++, rope++) {
      if (i >= 32)
        break;
      i32 word = i / 32;
      u32 bit = 1 << (i & 31);
      if (!rope->visible)
        progress->visible[word] &= ~bit;
      if (!rope->active)
        progress->active[word] &= ~bit;
    }
  }
}

// FUNCTION: LEGOBATMAN 0x0064abe0
void TightRopes_AddLevelSfx(void *world_ptr, void *unused, i32 *sfx_ids,
                            i32 *sfx_count, i32 max_sfx) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  if (world->tightrope_count != 0)
    AddLevelSfxFromName("TightRope_App", sfx_ids, sfx_count, max_sfx);
}

// FUNCTION: LEGOBATMAN 0x0064ac10
ADDGIZMOTYPE *TightRopes_RegisterGizmo(i32 type_id) {
  // GLOBAL: LEGOBATMAN 0x00966b30
  static char *name = "TightRope";
  // GLOBAL: LEGOBATMAN 0x00ad11d8
  static ADDGIZMOTYPE addtype;

  addtype = Default_ADDGIZMOTYPE;
  addtype.name = name;
  addtype.prefix = "";
  addtype.progress_size = sizeof(TIGHTROPEPROGRESS);
  addtype.fns[0] = (void *)TightRopes_GetMaxGizmos;
  addtype.fns[1] = (void *)TightRopes_AddGizmos;
  addtype.fns[2] = (void *)TightRopes_EarlyUpdate;
  addtype.fns[3] = (void *)TightRopes_LateUpdate;
  addtype.fns[4] = (void *)TightRopes_Draw;
  addtype.fns[5] = NULL;
  addtype.fns[6] = (void *)TightRope_GetGizmoName;
  addtype.fns[7] = (void *)TightRope_GetOutput;
  addtype.fns[8] = (void *)TightRope_GetOutputName;
  addtype.fns[9] = (void *)TightRope_GetNumOutputs;
  addtype.fns[10] = (void *)TightRope_Activate;
  addtype.fns[11] = NULL;
  addtype.fns[12] = (void *)TightRope_SetVisibility;
  addtype.fns[13] = NULL;
  addtype.fns[14] = NULL;
  addtype.fns[15] = NULL;
  addtype.fns[16] = NULL;
  addtype.fns[17] = NULL;
  addtype.fns[18] = NULL;
  addtype.fns[19] = (void *)TightRopes_AllocateProgressData;
  addtype.fns[20] = (void *)TightRopes_ClearProgress;
  addtype.fns[21] = (void *)TightRopes_StoreProgress;
  addtype.fns[22] = (void *)TightRopes_Reset;
  addtype.fns[23] = (void *)TightRopes_ReserveBufferSpace;
  addtype.fns[24] = (void *)TightRopes_Load;
  addtype.fns[25] = NULL;
  addtype.fns[26] = NULL;
  addtype.fns[27] = (void *)TightRopes_AddLevelSfx;
  return &addtype;
}
