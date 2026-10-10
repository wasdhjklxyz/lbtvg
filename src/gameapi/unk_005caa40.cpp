// gameapi/unk_005caa40.cpp: TU of unknown name, found by its header-static
// copies (the functions after them are not matched yet).

#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"

// FUNCTION: LEGOBATMAN 0x005caa40
static f32 NuFabs(f32 f);
// FUNCTION: LEGOBATMAN 0x005caa60
static f32 NuFdiv(f32 a, f32 b);
// FUNCTION: LEGOBATMAN 0x005caaa0
static f32 NuSinApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x005cab40
static f32 NuCosApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x005cab50
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_005caa40(f32 *v, f32 a, i32 i) {
  v[2] = NuFabs(a);
  v[3] = NuFdiv(a, v[4]);
  v[0] = NuSinApprox(i);
  v[1] = NuCosApprox(i);
  NuVec4Set(v, a, a, a, a);
}

// The rest of this TU is the Mac's GizBuildIt file (saga
// legoapi/gizmo/object/gizbuildit.cpp, .. GizBuildIts_RegisterGizmo
// 0x5ce220); callbacks named by their RegisterGizmo slot.

#include "../batman/leveldata_unk.h"
#include "../batman/worldinfo_unk.h"
#include <stddef.h>
#include <string.h>

typedef struct GIZBUILDIT_s {
  char name[0x10]; // 0x00
  void *parts;     // 0x10
  u8 pad14[0x18 - 0x14];
  void *p18; // 0x18
  u8 pad1c[0x34 - 0x1c];
  nuvec_s position; // 0x34
  u8 pad40[0x44 - 0x40];
  f32 step_timer;    // 0x44
  f32 step_duration; // 0x48
  u8 pad4c[0x5a - 0x4c];
  u16 completion_score; // 0x5a
  u8 pad5c[0x70 - 0x5c];
  u8 anim_object_count;  // 0x70
  u8 build_state;        // 0x71, 2 = complete
  u8 b72;                // 0x72
  u8 built_object_count; // 0x73
  u8 pad74[0x76 - 0x74];
  u8 b76_0 : 1;
  u8 turn_off_when_complete : 1; // 0x76 bit 1
  u8 pad77[0x7c - 0x77];
  i16 s7c;         // 0x7c
  u16 active : 1;  // 0x7e
  u16 visible : 1; // 0x7e bit 1
  u16 b2 : 1;      // 0x7e bit 2
  u16 b3 : 1;
  u16 b4 : 1;
  u16 b5 : 1;
  u16 b6 : 1;
  u16 b7 : 1; // 0x7e bit 7, s7c needs resolving in PostLoad
  u16 b8 : 1;
  u16 b9 : 1; // 0x7e bit 9
} GIZBUILDIT;

typedef struct GIZBUILDITSYS_s {
  GIZBUILDIT *buildits; // 0x00
  u16 count;            // 0x04
  u16 max;              // 0x06
  u16 max_parts;        // 0x08
  void *p0c;            // 0x0c
  void *p10;            // 0x10
} GIZBUILDITSYS;

typedef struct GIZBUILDITPROGRESS_s {
  u32 complete[2]; // 0x00
  u32 active[2];   // 0x08
  u32 visible[2];  // 0x10
  u32 b9[2];       // 0x18
  u8 built[0x40];  // 0x20
} GIZBUILDITPROGRESS;

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
// GLOBAL: LEGOBATMAN 0x009604c8
extern i32 g_unk009604c8; // the build-it special move
// GLOBAL: LEGOBATMAN 0x00abe218
extern void (*GizBuildIt_FinishFn)(GIZBUILDIT *buildit);

i32 NuStrLen(const char *s);
void AddGizmo(GIZMOSYS_s *gizmo_sys, i32 type_id, void *a, void *object);
void *GameBufferAlloc(variptr_u *buf, variptr_u *buf_end, i32 size);
void *GameBufferAllocProgressUnk005bbaf0(VARIPTR *buf, VARIPTR *buf_end,
                                         i32 size);
i32 Player_HasFastBuild(GameObject_s *player);
void GameCam_Blend(GAMECAMERA_s *camera, f32 duration, f32 curve, i32 mode);
void Unk00604f10(void *parts, i32 visible);
void *Unk00603df0(variptr_u *buf, variptr_u *buf_end, i32 a, i32 count);
void *Unk00603bc0(variptr_u *buf, variptr_u *buf_end, void *pool,
                  struct GAMEANIMSYS_s *anim_sys);
i16 Unk005de820(void *world, i32 id);

// STUB: LEGOBATMAN 0x005cc110
// Register swap: the original keeps player in edi and build_amount in esi.
f32 GizBuildItMul(GameObject_s *player) {
  if ((player->b1fc & 0x80) && Player_HasFastBuild(player))
    return 3.0f;
  GIZBUILDIT *buildit = (GIZBUILDIT *)player->techno;
  i32 build_amount = player->b9da;
  i32 left = buildit->built_object_count - build_amount;
  if (left + build_amount >= (buildit->anim_object_count + left) / 2)
    build_amount = buildit->anim_object_count - left - build_amount;
  if (build_amount > 10)
    build_amount = 10;
  else if (build_amount < 0)
    build_amount = 0;
  return build_amount / 10.0 + 1.0;
}

// FUNCTION: LEGOBATMAN 0x005cc1a0
void ReleaseBuildIt(GameObject_s *player, i32 blend) {
  if (g_unk009604c8 != -1 && player->b9db == g_unk009604c8) {
    player->b9db = -1;
    GameCam_Blend(g_unk0095f624, 0.5f, blend ? 0.6f : 0.0f, 1);
  }
}

// FUNCTION: LEGOBATMAN 0x005cc200
void GizBuildIts_PostLoad(void *world, void *sys_ptr) {
  GIZBUILDITSYS *sys = (GIZBUILDITSYS *)sys_ptr;
  if (sys == NULL)
    return;
  GIZBUILDIT *buildit = sys->buildits;
  for (i32 i = 0; i < sys->count; i++, buildit++) {
    if (buildit->b7) {
      buildit->s7c = Unk005de820(world, buildit->s7c);
      buildit->b7 = 0;
    }
  }
}

// FUNCTION: LEGOBATMAN 0x005cc260
void *GizBuildIts_ReserveBufferSpace(void *world_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  GIZBUILDITSYS *sys = (GIZBUILDITSYS *)GameBufferAlloc(
      &world->buf104, &world->bufEnd108, sizeof(GIZBUILDITSYS));
  sys->max = world->current_level->max_buildits;
  sys->max_parts = world->current_level->max_buildit_objects;
  sys->buildits = (GIZBUILDIT *)GameBufferAlloc(
      &world->buf104, &world->bufEnd108, sys->max * sizeof(GIZBUILDIT));
  sys->p0c =
      Unk00603df0(&world->buf104, &world->bufEnd108, 200, sys->max_parts);
  sys->p10 =
      GameBufferAlloc(&world->buf104, &world->bufEnd108, sys->max_parts * 4);
  for (i32 i = 0; i < sys->max; i++)
    sys->buildits[i].parts = Unk00603bc0(&world->buf104, &world->bufEnd108,
                                         sys->p0c, world->game_anim_sys);
  world->giz_buildit_sys = sys;
  return sys;
}

// FUNCTION: LEGOBATMAN 0x005cc340
void GizBuildit_SetVisibility(GIZBUILDIT *buildit, i32 visible) {
  if (buildit != NULL) {
    Unk00604f10(buildit->parts, visible);
    buildit->visible = visible != 0;
  }
}

// FUNCTION: LEGOBATMAN 0x005cc380
void GizmoBuildit_SetVisibility(GIZMO *gizmo, i32 visible) {
  if (gizmo != NULL)
    GizBuildit_SetVisibility((GIZBUILDIT *)gizmo->object, visible);
}

// FUNCTION: LEGOBATMAN 0x005cc3c0
nuvec_s *GizmoBuildit_GetPos(GIZMO *gizmo) {
  if (gizmo != NULL && gizmo->object != NULL)
    return &((GIZBUILDIT *)gizmo->object)->position;
  return NULL;
}

void GizBuildit_Reset(GIZBUILDIT *buildit, void *world);

// FUNCTION: LEGOBATMAN 0x005cc7e0
void GizBuildIts_Reset(void *world, void *sys_ptr, void *progress_ptr) {
  GIZBUILDITSYS *sys = (GIZBUILDITSYS *)sys_ptr;
  GIZBUILDITPROGRESS *progress = (GIZBUILDITPROGRESS *)progress_ptr;
  GIZBUILDIT *buildit = sys->buildits;
  for (i32 i = 0; i < sys->count; i++, buildit++) {
    GizBuildit_Reset(buildit, world);
    if (progress != NULL && i < 64) {
      i32 word = i / 32;
      u32 bit = 1 << (i & 31);
      buildit->build_state = (progress->complete[word] & bit) ? 2 : 0;
      buildit->visible = (progress->visible[word] & bit) != 0;
      buildit->active = (progress->active[word] & bit) != 0;
      buildit->b9 = (progress->b9[word] & bit) != 0;
      buildit->built_object_count = progress->built[i];
    }
  }
}

// FUNCTION: LEGOBATMAN 0x005cc8c0
void GizBuildIts_EarlyUpdate(void *world, void *sys_ptr, f32 dt) {
  GIZBUILDITSYS *sys = (GIZBUILDITSYS *)sys_ptr;
  if (sys == NULL)
    return;
  GIZBUILDIT *buildit = sys->buildits;
  for (i32 i = 0; i < sys->count; i++, buildit++) {
    buildit->b2 = 0;
    buildit->b72 = 0;
  }
}

void GizBuildIts_Draw(void *world, void *sys, f32 dt);

// FUNCTION: LEGOBATMAN 0x005ccc30
i32 GizBuildIts_GetMaxGizmos(void *world_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  if (world != NULL)
    return world->current_level->max_buildits;
  return 0;
}

// FUNCTION: LEGOBATMAN 0x005ccc50
void GizBuildIts_AddGizmos(GIZMOSYS_s *gizmo_sys, i32 type_id, void *world,
                           void *sys_ptr) {
  GIZBUILDITSYS *sys = (GIZBUILDITSYS *)sys_ptr;
  if (sys == NULL)
    return;
  for (i32 i = 0; i < sys->count; i++) {
    if (NuStrLen(sys->buildits[i].name) != 0)
      AddGizmo(gizmo_sys, type_id, NULL, &sys->buildits[i]);
  }
}

// FUNCTION: LEGOBATMAN 0x005cccb0
char *GizmoBuildit_GetGizmoName(GIZMO *gizmo) {
  return gizmo != NULL ? ((GIZBUILDIT *)gizmo->object)->name : NULL;
}

// FUNCTION: LEGOBATMAN 0x005cccc0
i32 GizmoBuildit_GetOutput(GIZMO *gizmo, i32 a, i32 b) {
  GIZBUILDIT *buildit = (GIZBUILDIT *)gizmo->object;
  if (((buildit->visible && buildit->active) || b != 0) &&
      buildit->build_state == 2)
    return 1;
  return 0;
}

// FUNCTION: LEGOBATMAN 0x005cccf0
char *GizmoBuildit_GetOutputName(GIZMO *gizmo, i32 output) {
  return "Finished";
}

// FUNCTION: LEGOBATMAN 0x005ccd00
i32 GizmoBuildit_GetNumOutputs(GIZMO *gizmo) { return 1; }

// FUNCTION: LEGOBATMAN 0x005ccd10
void *GizBuildIts_AllocateProgressData(VARIPTR *buf, VARIPTR *buf_end) {
  return GameBufferAllocProgressUnk005bbaf0(buf, buf_end,
                                            sizeof(GIZBUILDITPROGRESS));
}

// FUNCTION: LEGOBATMAN 0x005ccd30
void GizBuildIts_ClearProgress(void *world, void *progress_ptr) {
  GIZBUILDITPROGRESS *progress = (GIZBUILDITPROGRESS *)progress_ptr;
  if (progress != NULL) {
    memset(progress->complete, 0, sizeof(progress->complete));
    memset(progress->active, 0xff, sizeof(progress->active));
    memset(progress->visible, 0xff, sizeof(progress->visible));
    memset(progress->b9, 0, sizeof(progress->b9));
    memset(progress->built, 0, sizeof(progress->built));
  }
}

// FUNCTION: LEGOBATMAN 0x005ccd70
void GizBuildIts_StoreProgress(void *world, void *sys_ptr, void *progress_ptr) {
  GIZBUILDITSYS *sys = (GIZBUILDITSYS *)sys_ptr;
  GIZBUILDITPROGRESS *progress = (GIZBUILDITPROGRESS *)progress_ptr;
  if (progress == NULL)
    return;
  GizBuildIts_ClearProgress(NULL, progress);
  GIZBUILDIT *buildit = sys->buildits;
  for (i32 i = 0; i < sys->count; i++, buildit++) {
    if (i >= 64)
      break;
    i32 word = i / 32;
    u32 bit = 1 << (i & 31);
    if (buildit->build_state != 0)
      progress->complete[word] |= bit;
    if (!buildit->visible)
      progress->visible[word] &= ~bit;
    if (!buildit->active)
      progress->active[word] &= ~bit;
    if (buildit->b9)
      progress->b9[word] |= bit;
    progress->built[i] = buildit->built_object_count;
  }
}

// FUNCTION: LEGOBATMAN 0x005cce30
u32 GizBuildIts_TotalScore(void *world) {
  u32 total = 0;
  GIZBUILDITSYS *sys = ((WORLDINFO_s *)world)->giz_buildit_sys;
  if (sys != NULL) {
    GIZBUILDIT *buildit = sys->buildits;
    if (buildit != NULL) {
      for (i32 i = 0; i < sys->count; i++, buildit++)
        total += buildit->completion_score;
    }
  }
  return total;
}

// FUNCTION: LEGOBATMAN 0x005cce70
void GizBuildIt_Finish(GIZBUILDIT *buildit) {
  buildit->build_state = 2;
  if (buildit->turn_off_when_complete) {
    if (buildit->p18 == NULL)
      Unk00604f10(buildit->parts, 0);
    buildit->visible = 0;
  }
  if (GizBuildIt_FinishFn != NULL)
    GizBuildIt_FinishFn(buildit);
}

// FUNCTION: LEGOBATMAN 0x005cceb0
void GizBuildIt_SetStepTime(GIZBUILDIT *buildit, GameObject_s *player) {
  buildit->step_duration = 0.3f;
  if (player != NULL)
    buildit->step_duration /= GizBuildItMul(player);
  buildit->step_timer = buildit->step_duration;
}

i32 GizBuildIts_Load(void *world, void *sys);
void GizmoBuildit_Activate(GIZMO *gizmo, i32 active);
void GizBuildIts_LateUpdate(void *world, void *sys, f32 dt);

// GLOBAL: LEGOBATMAN 0x00960a3c
i32 buildit_gizmotype_id = -1;

// FUNCTION: LEGOBATMAN 0x005ce220
ADDGIZMOTYPE *GizBuildIts_RegisterGizmo(i32 type_id) {
  // GLOBAL: LEGOBATMAN 0x00960ae4
  static char *name = "GizBuildit";
  // GLOBAL: LEGOBATMAN 0x00abe220
  static ADDGIZMOTYPE addtype;

  addtype = Default_ADDGIZMOTYPE;
  addtype.name = name;
  addtype.prefix = "";
  addtype.progress_size = sizeof(GIZBUILDITPROGRESS);
  addtype.fns[0] = (void *)GizBuildIts_GetMaxGizmos;
  addtype.fns[1] = (void *)GizBuildIts_AddGizmos;
  addtype.fns[2] = (void *)GizBuildIts_EarlyUpdate;
  addtype.fns[3] = (void *)GizBuildIts_LateUpdate;
  addtype.fns[4] = (void *)GizBuildIts_Draw;
  addtype.fns[5] = NULL;
  addtype.fns[6] = (void *)GizmoBuildit_GetGizmoName;
  addtype.fns[7] = (void *)GizmoBuildit_GetOutput;
  addtype.fns[8] = (void *)GizmoBuildit_GetOutputName;
  addtype.fns[9] = (void *)GizmoBuildit_GetNumOutputs;
  addtype.fns[10] = (void *)GizmoBuildit_Activate;
  addtype.fns[11] = NULL;
  addtype.fns[12] = (void *)GizmoBuildit_SetVisibility;
  addtype.fns[13] = NULL;
  addtype.fns[14] = (void *)GizmoBuildit_GetPos;
  addtype.fns[15] = NULL;
  addtype.fns[16] = NULL;
  addtype.fns[17] = NULL;
  addtype.fns[18] = NULL;
  addtype.fns[19] = (void *)GizBuildIts_AllocateProgressData;
  addtype.fns[20] = (void *)GizBuildIts_ClearProgress;
  addtype.fns[21] = (void *)GizBuildIts_StoreProgress;
  addtype.fns[22] = (void *)GizBuildIts_Reset;
  addtype.fns[23] = (void *)GizBuildIts_ReserveBufferSpace;
  addtype.fns[24] = (void *)GizBuildIts_Load;
  addtype.fns[25] = NULL;
  addtype.fns[26] = (void *)GizBuildIts_PostLoad;
  addtype.fns[27] = NULL;
  buildit_gizmotype_id = type_id;
  return &addtype;
}
