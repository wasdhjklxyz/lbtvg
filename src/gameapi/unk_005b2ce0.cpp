// gameapi/unk_005b2ce0.cpp: TU of unknown name, found by its header-static
// copies (the functions after them are not matched yet).

#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/numtx_inline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x005b2df0
static void NuMtxSetRotationYInline(f32 *m, i32 a);

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x005b2e90
static void NuMtxRotateYInline(f32 *m, i32 a);

// FUNCTION: LEGOBATMAN 0x005b2ce0
static f32 NuFabs(f32 f);
// FUNCTION: LEGOBATMAN 0x005b2d00
static f32 NuSinApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x005b2da0
static f32 NuCosApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x005b2db0
static void NuVec4Copy(f32 *dst, f32 *src);
// FUNCTION: LEGOBATMAN 0x005b2dd0
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_005b2ce0(f32 *v, f32 a, i32 i) {
  v[2] = NuFabs(a);
  v[0] = NuSinApprox(i);
  v[1] = NuCosApprox(i);
  NuVec4Copy(v + 4, v);
  NuVec4Set(v, a, a, a, a);
}

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_unk_005b2ce0(f32 *v, f32 a, i32 i) {
  NuMtxRotateYInline(v + 16, i);
}

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_2_unk_005b2ce0(f32 *v, f32 a, i32 i) {
  NuMtxSetRotationYInline(v + 64, i);
}

// The rest of this TU is the Mac's Spinner file (Spinners_Load 0x5b30f0 ..
// Spinners_RegisterGizmo 0x5b6b00); callbacks named by their RegisterGizmo
// slot.

#include "../batman/leveldata_unk.h"
#include "../batman/worldinfo_unk.h"
#include <stddef.h>
#include <string.h>

typedef struct SPINNER_s {
  u8 pad0[0x40];
  char name[0x10]; // 0x40
  u8 pad50[0x68 - 0x50];
  void *parts; // 0x68
  u8 pad6c[0xa8 - 0x6c];
  u8 num_outputs; // 0xa8
  u8 pada9;
  u8 add_gizmo : 1; // 0xaa
  u8 padab[0x314 - 0xab];
} SPINNER;

typedef struct SPINNERPROGRESS_s {
  u8 data[0x100];
} SPINNERPROGRESS;

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
// GLOBAL: LEGOBATMAN 0x0095fe64
extern char Spinner_OutputNameBuf[];

i32 NuStrLen(const char *s);
i32 NuStrCpy(char *dst, const char *src);
void NuStrCat(char *dst, const char *src);
char *NuIToA(i32 value, char *buffer, i32 radix);
void AddGizmo(GIZMOSYS_s *gizmo_sys, i32 type_id, void *a, void *object);
void *GameBufferAlloc(variptr_u *buf, variptr_u *buf_end, i32 size);
void *GameBufferAllocProgressUnk005bbaf0(VARIPTR *buf, VARIPTR *buf_end,
                                         i32 size);
void *Unk00603df0(variptr_u *buf, variptr_u *buf_end, i32 a, i32 count);
void *Unk00603bc0(variptr_u *buf, variptr_u *buf_end, void *pool,
                  struct GAMEANIMSYS_s *anim_sys);

// FUNCTION: LEGOBATMAN 0x005b39b0
void *Spinners_ReserveBufferSpace(void *world_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  world->spinners = NULL;
  world->spinner_count = 0;
  if (world->current_level->max_spinners != 0) {
    world->spinner_pool =
        Unk00603df0(&world->buf104, &world->bufEnd108, 4,
                    world->current_level->max_spinneranim_objs);
    world->spinners = (SPINNER *)GameBufferAlloc(
        &world->buf104, &world->bufEnd108,
        world->current_level->max_spinners * sizeof(SPINNER));
    for (i32 i = 0; i < world->current_level->max_spinners; i++)
      world->spinners[i].parts =
          Unk00603bc0(&world->buf104, &world->bufEnd108, world->spinner_pool,
                      world->game_anim_sys);
    world->buf104.addr = (world->buf104.addr + 3) & ~3;
  }
  return world->spinners;
}

// FUNCTION: LEGOBATMAN 0x005b3fb0
void Spinners_PanelDraw(void *world, void *unused, f32 dt) {}

// FUNCTION: LEGOBATMAN 0x005b3fc0
i32 Spinners_GetMaxGizmos(void *world_ptr) {
  return ((WORLDINFO_s *)world_ptr)->current_level->max_spinners;
}

// FUNCTION: LEGOBATMAN 0x005b3fe0
void Spinners_AddGizmos(GIZMOSYS_s *gizmo_sys, i32 type_id, void *world_ptr,
                        void *unused) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  if (world->spinners == NULL)
    return;
  for (i32 i = 0; i < world->current_level->max_spinners; i++) {
    if (world->spinners[i].add_gizmo && NuStrLen(world->spinners[i].name) != 0)
      AddGizmo(gizmo_sys, type_id, NULL, &world->spinners[i]);
  }
}

// FUNCTION: LEGOBATMAN 0x005b4070
char *Spinner_GetGizmoName(GIZMO *gizmo) {
  return gizmo != NULL ? ((SPINNER *)gizmo->object)->name : NULL;
}

// FUNCTION: LEGOBATMAN 0x005b4090
char *Spinner_GetOutputName(GIZMO *gizmo, i32 output) {
  SPINNER *spinner = (SPINNER *)gizmo->object;
  if (output < 0 || output > spinner->num_outputs)
    return NULL;
  if (spinner->num_outputs >= 2) {
    f32 pct = (f32)output / (spinner->num_outputs - 1);
    pct *= 100.0f;
    NuIToA((i32)pct, Spinner_OutputNameBuf, 10);
    NuStrCat(Spinner_OutputNameBuf, "%% Complete");
    return Spinner_OutputNameBuf;
  }
  if (spinner->num_outputs != 0) {
    NuStrCpy(Spinner_OutputNameBuf, "100%% Complete");
    return Spinner_OutputNameBuf;
  }
  NuStrCpy(Spinner_OutputNameBuf, "");
  return Spinner_OutputNameBuf;
}

// FUNCTION: LEGOBATMAN 0x005b4140
i32 Spinner_GetNumOutputs(GIZMO *gizmo) {
  return ((SPINNER *)gizmo->object)->num_outputs;
}

// FUNCTION: LEGOBATMAN 0x005b4c10
void *Spinners_AllocateProgressData(VARIPTR *buf, VARIPTR *buf_end) {
  return GameBufferAllocProgressUnk005bbaf0(buf, buf_end,
                                            sizeof(SPINNERPROGRESS));
}

// FUNCTION: LEGOBATMAN 0x005b4c30
void Spinners_ClearProgress(void *world, void *progress_ptr) {
  SPINNERPROGRESS *progress = (SPINNERPROGRESS *)progress_ptr;
  if (progress != NULL)
    memset(progress, 0, sizeof(SPINNERPROGRESS));
}

i32 Spinners_Load(void *world, void *unused);
void Spinners_Reset(void *world, void *unused, void *progress);
void Spinners_StoreProgress(void *world, void *unused, void *progress);
void Spinners_Update(void *world, void *unused, f32 dt);
void Spinners_Draw(void *world, void *unused, f32 dt);
i32 Spinner_GetOutput(GIZMO *gizmo, i32 output, i32 b);
void Spinner_Activate(GIZMO *gizmo, i32 active);
void Spinner_SetVisibility(GIZMO *gizmo, i32 visible);
nuvec_s *Spinner_GetPos(GIZMO *gizmo);
i32 Spinner_UsingSpecial(void);
void Spinners_BoltHitPlat(void);
void Spinners_GetBestBoltTarget(void);
void Spinners_BoltHit(void);

// GLOBAL: LEGOBATMAN 0x0095fe74
i32 spinner_gizmotype_id = -1;

// FUNCTION: LEGOBATMAN 0x005b6b00
ADDGIZMOTYPE *Spinners_RegisterGizmo(i32 type_id) {
  // GLOBAL: LEGOBATMAN 0x00ab05c8
  static ADDGIZMOTYPE addtype;

  addtype = Default_ADDGIZMOTYPE;
  addtype.fns[3] = NULL;
  addtype.fns[11] = NULL;
  addtype.fns[13] = NULL;
  addtype.fns[25] = NULL;
  addtype.fns[26] = NULL;
  addtype.fns[27] = NULL;
  spinner_gizmotype_id = type_id;
  addtype.name = "Spinner";
  addtype.prefix = "";
  addtype.progress_size = sizeof(SPINNERPROGRESS);
  addtype.fns[0] = (void *)Spinners_GetMaxGizmos;
  addtype.fns[1] = (void *)Spinners_AddGizmos;
  addtype.fns[2] = (void *)Spinners_Update;
  addtype.fns[4] = (void *)Spinners_Draw;
  addtype.fns[5] = (void *)Spinners_PanelDraw;
  addtype.fns[6] = (void *)Spinner_GetGizmoName;
  addtype.fns[7] = (void *)Spinner_GetOutput;
  addtype.fns[8] = (void *)Spinner_GetOutputName;
  addtype.fns[9] = (void *)Spinner_GetNumOutputs;
  addtype.fns[10] = (void *)Spinner_Activate;
  addtype.fns[12] = (void *)Spinner_SetVisibility;
  addtype.fns[14] = (void *)Spinner_GetPos;
  addtype.fns[15] = (void *)Spinner_UsingSpecial;
  addtype.fns[16] = (void *)Spinners_BoltHitPlat;
  addtype.fns[17] = (void *)Spinners_GetBestBoltTarget;
  addtype.fns[18] = (void *)Spinners_BoltHit;
  addtype.fns[19] = (void *)Spinners_AllocateProgressData;
  addtype.fns[20] = (void *)Spinners_ClearProgress;
  addtype.fns[21] = (void *)Spinners_StoreProgress;
  addtype.fns[22] = (void *)Spinners_Reset;
  addtype.fns[23] = (void *)Spinners_ReserveBufferSpace;
  addtype.fns[24] = (void *)Spinners_Load;
  return &addtype;
}
