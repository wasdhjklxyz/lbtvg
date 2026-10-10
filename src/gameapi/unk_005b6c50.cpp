// gameapi/unk_005b6c50.cpp: TU of unknown name, found by its header-static
// copies (the functions after them are not matched yet).

#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"

// FUNCTION: LEGOBATMAN 0x005b6c50
static f32 NuSinApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x005b6cf0
static f32 NuCosApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x005b6d00
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_005b6c50(f32 *v, f32 a, i32 i) {
  v[0] = NuSinApprox(i);
  v[1] = NuCosApprox(i);
  NuVec4Set(v, a, a, a, a);
}

// The rest of this TU is the Mac's GizObstacle file (GizObstacles_Load 0x5b6d60
// .. GizObstacles_RegisterGizmo 0x5b9cd0); callbacks named by their
// RegisterGizmo slot.

#include "../batman/leveldata_unk.h"
#include "../batman/worldinfo_unk.h"
#include <stddef.h>
#include <string.h>

typedef struct GAMEANIMOBJ_s {
  u8 pad0[0x18];
  i32 state; // 0x18
} GAMEANIMOBJ;

typedef struct GIZOBSTACLE_s {
  char name[0x10];  // 0x00
  nuvec_s position; // 0x10
  u8 pad1c[0x34 - 0x1c];
  GAMEANIMOBJ *parts;   // 0x34
  GameObject_s *pusher; // 0x38
  u8 pad3c[0x5c - 0x3c];
  f32 control; // 0x5c
  u8 pad60[0x80 - 0x60];
  f32 f80; // 0x80
  u8 pad84[0x90 - 0x84];
  u32 flags90; // 0x90
  u8 pad94[0xbc - 0x94];
  i16 sbc; // 0xbc, resolved in PostLoad when flags bit 12 is set
  u8 padbe[0xc0 - 0xbe];
  i16 sfx[2]; // 0xc0
  u8 bc4;     // 0xc4
  u8 padc5[0xc8 - 0xc5];
  union {
    u32 flags; // 0xc8
    struct {
      u32 active : 1;            // bit 0
      u32 visible : 1;           // bit 1
      u32 techno_controlled : 1; // bit 2
      u32 push_controlled : 1;   // bit 3
    };
  };
} GIZOBSTACLE;

typedef struct GIZOBSTACLESYS_s {
  GIZOBSTACLE *obstacles; // 0x00
  void *p4;               // 0x04
  u16 count;              // 0x08
  u16 max;                // 0x0a
  u8 padc[0x10 - 0xc];
  void *pool; // 0x10
} GIZOBSTACLESYS;

typedef struct GIZOBSTACLEPROGRESS_s {
  u32 active[4];  // 0x00
  u32 visible[4]; // 0x10
  u32 a20[4];     // 0x20, flags bit 11
  u32 a30[4];     // 0x30, flags bit 13
  u32 a40[4];     // 0x40, flags bit 14
  u32 a50[4];     // 0x50, flags bit 17
  u32 a60[4];     // 0x60, flags bit 18
  u32 a70[4];     // 0x70, flags bit 19
  u32 a80[4];     // 0x80, flags bit 20
  u32 a90[4];     // 0x90, flags90 bit 21 and f80 > 0
} GIZOBSTACLEPROGRESS;

typedef struct OBSTACLETRIGGER_s {
  i32 type;     // 0x00
  nuvec_s *pos; // 0x04
} OBSTACLETRIGGER;

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
// GLOBAL: LEGOBATMAN 0x00ab0650
extern OBSTACLETRIGGER GizObstacle_Triggers[16];
// GLOBAL: LEGOBATMAN 0x00ab06d0
extern i32 GizObstacle_TriggerCount;

i32 NuStrLen(const char *s);
int NuStrICmp(const char *a, const char *b);
void AddGizmo(GIZMOSYS_s *gizmo_sys, i32 type_id, void *a, void *object);
void *GameBufferAlloc(variptr_u *buf, variptr_u *buf_end, i32 size);
void *GameBufferAllocProgressUnk005bbaf0(VARIPTR *buf, VARIPTR *buf_end,
                                         i32 size);
void AddLevelSfxFromId(i32 sfx_id, i32 *sfx_ids, i32 *sfx_count, i32 max_sfx);
void Unk00604f10(void *parts, i32 visible);
void *Unk00603df0(variptr_u *buf, variptr_u *buf_end, i32 a, i32 count);
void *Unk00603bc0(variptr_u *buf, variptr_u *buf_end, void *pool,
                  struct GAMEANIMSYS_s *anim_sys);
i16 Unk005de820(void *world, i32 id);
void Unk00604a80(GAMEANIMOBJ *parts);
void Unk00604b00(GAMEANIMOBJ *parts);
void Unk00604b40(GAMEANIMOBJ *parts);

i32 GizObstacles_Load(void *world, void *sys);

// FUNCTION: LEGOBATMAN 0x005b7110
void GizObstacles_PostLoad(void *world, void *sys_ptr) {
  GIZOBSTACLESYS *sys = (GIZOBSTACLESYS *)sys_ptr;
  if (sys == NULL)
    return;
  GIZOBSTACLE *obstacle = sys->obstacles;
  for (i32 i = 0; i < sys->count; i++, obstacle++) {
    if (obstacle->flags & 0x1000) {
      obstacle->sbc = Unk005de820(world, obstacle->sbc);
      obstacle->flags &= ~0x1000;
    }
  }
}

// FUNCTION: LEGOBATMAN 0x005b7170
void GizObstacles_AddLevelSfx(void *world, void *sys_ptr, i32 *sfx_ids,
                              i32 *sfx_count, i32 max_sfx) {
  GIZOBSTACLESYS *sys = (GIZOBSTACLESYS *)sys_ptr;
  if (sys == NULL)
    return;
  GIZOBSTACLE *obstacle = sys->obstacles;
  for (i32 i = 0; i < sys->count; i++, obstacle++) {
    if (obstacle->sfx[0] != -1)
      AddLevelSfxFromId(obstacle->sfx[0], sfx_ids, sfx_count, max_sfx);
    if (obstacle->sfx[1] != -1)
      AddLevelSfxFromId(obstacle->sfx[1], sfx_ids, sfx_count, max_sfx);
  }
}

// FUNCTION: LEGOBATMAN 0x005b71f0
void *GizObstacles_ReserveBufferSpace(void *world_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  GIZOBSTACLESYS *sys = (GIZOBSTACLESYS *)GameBufferAlloc(
      &world->buf104, &world->bufEnd108, sizeof(GIZOBSTACLESYS));
  sys->max = world->current_level->max_obstacles;
  sys->obstacles = (GIZOBSTACLE *)GameBufferAlloc(
      &world->buf104, &world->bufEnd108, sys->max * sizeof(GIZOBSTACLE));
  sys->p4 = GameBufferAlloc(&world->buf104, &world->bufEnd108, sys->max * 4);
  sys->pool = Unk00603df0(&world->buf104, &world->bufEnd108, 4,
                          (u16)world->current_level->max_obstacle_objects);
  for (i32 i = 0; i < sys->max; i++)
    sys->obstacles[i].parts = (GAMEANIMOBJ *)Unk00603bc0(
        &world->buf104, &world->bufEnd108, sys->pool, world->game_anim_sys);
  world->giz_obstacle_sys = sys;
  return sys;
}

// FUNCTION: LEGOBATMAN 0x005b72f0
void GizmoObstacle_SetVisibility(GIZMO *gizmo, i32 visible) {
  if (gizmo != NULL) {
    GIZOBSTACLE *obstacle = (GIZOBSTACLE *)gizmo->object;
    if (obstacle != NULL) {
      Unk00604f10(obstacle->parts, visible);
      obstacle->visible = visible != 0;
    }
  }
}

// FUNCTION: LEGOBATMAN 0x005b7330
nuvec_s *GizmoObstacle_GetPos(GIZMO *gizmo) {
  if (gizmo != NULL && gizmo->object != NULL)
    return &((GIZOBSTACLE *)gizmo->object)->position;
  return NULL;
}

// FUNCTION: LEGOBATMAN 0x005b76b0
void GizObstacle_Stop(GIZOBSTACLE *obstacle) {
  if (obstacle != NULL)
    Unk00604a80(obstacle->parts);
}

// FUNCTION: LEGOBATMAN 0x005b8990
void GizObstacles_AddTrigger(nuvec_s *pos, i32 type) {
  if (pos != NULL && GizObstacle_TriggerCount < 16) {
    GizObstacle_Triggers[GizObstacle_TriggerCount].pos = pos;
    GizObstacle_Triggers[GizObstacle_TriggerCount].type = type;
    GizObstacle_TriggerCount++;
  }
}

// FUNCTION: LEGOBATMAN 0x005b89c0
void GizObstacle_SetTechnoControlled(GIZOBSTACLE *obstacle, f32 control) {
  obstacle->techno_controlled = 1;
  obstacle->control = control;
}

// FUNCTION: LEGOBATMAN 0x005b89e0
void GizObstacle_SetPushControlled(GIZOBSTACLE *obstacle, GameObject_s *pusher,
                                   f32 control) {
  if (obstacle->bc4 != 0 || obstacle->parts->state != 2 ||
      (obstacle->flags90 & 0x100)) {
    obstacle->push_controlled = 1;
    obstacle->control = control;
    obstacle->pusher = pusher;
  }
}

void GizObstacles_Update(void *world, void *sys, f32 dt);
void GizObstacles_Draw(void *world, void *sys, f32 dt);

// FUNCTION: LEGOBATMAN 0x005b8bc0
i32 GizObstacles_GetMaxGizmos(void *world_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  if (world != NULL)
    return world->current_level->max_obstacles;
  return 0;
}

// FUNCTION: LEGOBATMAN 0x005b8be0
void GizObstacles_AddGizmos(GIZMOSYS_s *gizmo_sys, i32 type_id, void *world,
                            void *sys_ptr) {
  GIZOBSTACLESYS *sys = (GIZOBSTACLESYS *)sys_ptr;
  if (sys == NULL)
    return;
  for (i32 i = 0; i < sys->count; i++) {
    if (NuStrLen(sys->obstacles[i].name) != 0)
      AddGizmo(gizmo_sys, type_id, NULL, &sys->obstacles[i]);
  }
}

// FUNCTION: LEGOBATMAN 0x005b8c40
char *GizmoObstacle_GetGizmoName(GIZMO *gizmo) {
  return gizmo != NULL ? ((GIZOBSTACLE *)gizmo->object)->name : NULL;
}

i32 GizmoObstacle_GetOutput(GIZMO *gizmo, i32 output, i32 b);

// FUNCTION: LEGOBATMAN 0x005b8d90
char *GizmoObstacle_GetOutputName(GIZMO *gizmo, i32 output) {
  switch (output) {
  case 0:
    return "AtEnd";
  case 1:
    return "NotAtStart";
  case 2:
    return "Proximity";
  case 3:
    return "AtStart";
  case 4:
    return "PlayingForward";
  case 5:
    return "Destroyed";
  case 6:
    return "WithinActiveFrames";
  }
  return NULL;
}

// FUNCTION: LEGOBATMAN 0x005b8df0
i32 GizmoObstacle_GetNumOutputs(GIZMO *gizmo) { return 7; }

// FUNCTION: LEGOBATMAN 0x005b8e00
void *GizObstacles_AllocateProgressData(VARIPTR *buf, VARIPTR *buf_end) {
  return GameBufferAllocProgressUnk005bbaf0(buf, buf_end,
                                            sizeof(GIZOBSTACLEPROGRESS));
}

// FUNCTION: LEGOBATMAN 0x005b8e20
void GizObstacles_ClearProgress(void *world, void *progress_ptr) {
  GIZOBSTACLEPROGRESS *progress = (GIZOBSTACLEPROGRESS *)progress_ptr;
  if (progress != NULL) {
    memset(progress->active, 0xff, sizeof(progress->active));
    memset(progress->visible, 0xff, sizeof(progress->visible));
    memset(progress->a20, 0, sizeof(progress->a20));
    memset(progress->a30, 0, sizeof(progress->a30));
    memset(progress->a40, 0, sizeof(progress->a40));
    memset(progress->a50, 0, sizeof(progress->a50));
    memset(progress->a60, 0, sizeof(progress->a60));
    memset(progress->a70, 0, sizeof(progress->a70));
    memset(progress->a80, 0, sizeof(progress->a80));
    memset(progress->a90, 0, sizeof(progress->a90));
  }
}

// FUNCTION: LEGOBATMAN 0x005b8ed0
GIZOBSTACLE *GizObstacle_FindByName(GIZOBSTACLESYS *sys, char *name) {
  if (sys != NULL && name != NULL) {
    GIZOBSTACLE *obstacle = sys->obstacles;
    for (i32 i = 0; i < sys->count; i++, obstacle++) {
      if (NuStrICmp(obstacle->name, name) == 0)
        return obstacle;
    }
  }
  return NULL;
}

// FUNCTION: LEGOBATMAN 0x005b8f20
void GizObstacle_JumpToStart(GIZOBSTACLE *obstacle) {
  if (obstacle != NULL && obstacle->parts != NULL)
    Unk00604b40(obstacle->parts);
}

// FUNCTION: LEGOBATMAN 0x005b8f40
void GizObstacle_JumpToEnd(GIZOBSTACLE *obstacle) {
  if (obstacle != NULL && obstacle->parts != NULL)
    Unk00604b00(obstacle->parts);
}

// FUNCTION: LEGOBATMAN 0x005b8f60
void GizObstacles_StoreProgress(void *world, void *sys_ptr,
                                void *progress_ptr) {
  GIZOBSTACLESYS *sys = (GIZOBSTACLESYS *)sys_ptr;
  GIZOBSTACLEPROGRESS *progress = (GIZOBSTACLEPROGRESS *)progress_ptr;
  GizObstacles_ClearProgress(world, progress);
  if (progress == NULL)
    return;
  GIZOBSTACLE *obstacle = sys->obstacles;
  for (i32 i = 0; i < sys->count; i++, obstacle++) {
    if (i >= 128)
      break;
    i32 word = i / 32;
    u32 bit = 1 << (i & 31);
    if (!obstacle->visible)
      progress->visible[word] &= ~bit;
    if (!obstacle->active)
      progress->active[word] &= ~bit;
    if (obstacle->flags & 0x800)
      progress->a20[word] |= bit;
    if (obstacle->flags & 0x2000)
      progress->a30[word] |= bit;
    if (obstacle->flags & 0x4000)
      progress->a40[word] |= bit;
    if (obstacle->flags & 0x20000)
      progress->a50[word] |= bit;
    if (obstacle->flags & 0x40000)
      progress->a60[word] |= bit;
    if (obstacle->flags & 0x80000)
      progress->a70[word] |= bit;
    if (obstacle->flags & 0x100000)
      progress->a80[word] |= bit;
    if ((obstacle->flags90 & 0x200000) && obstacle->f80 > 0.0f)
      progress->a90[word] |= bit;
  }
}

void GizObstacles_Reset(void *world, void *sys, void *progress);
void GizmoObstacle_Activate(GIZMO *gizmo, i32 active);
i32 GizmoObstacle_ActivateRev(GIZMO *gizmo, i32 value, i32 query);
void GizObstacles_BoltHitPlat(void);
void GizObstacles_GetBestBoltTarget(void);
void GizObstacles_BoltHit(void);

// GLOBAL: LEGOBATMAN 0x0095ff24
i32 obstacle_gizmotype_id = -1;

// FUNCTION: LEGOBATMAN 0x005b9cd0
ADDGIZMOTYPE *GizObstacles_RegisterGizmo(i32 type_id) {
  // GLOBAL: LEGOBATMAN 0x00960030
  static char *name = "GizObstacle";
  // GLOBAL: LEGOBATMAN 0x00ab06f0
  static ADDGIZMOTYPE addtype;

  addtype = Default_ADDGIZMOTYPE;
  addtype.name = name;
  addtype.prefix = "";
  addtype.progress_size = sizeof(GIZOBSTACLEPROGRESS);
  addtype.fns[0] = (void *)GizObstacles_GetMaxGizmos;
  addtype.fns[1] = (void *)GizObstacles_AddGizmos;
  addtype.fns[2] = NULL;
  addtype.fns[3] = (void *)GizObstacles_Update;
  addtype.fns[4] = (void *)GizObstacles_Draw;
  addtype.fns[5] = NULL;
  addtype.fns[6] = (void *)GizmoObstacle_GetGizmoName;
  addtype.fns[7] = (void *)GizmoObstacle_GetOutput;
  addtype.fns[8] = (void *)GizmoObstacle_GetOutputName;
  addtype.fns[9] = (void *)GizmoObstacle_GetNumOutputs;
  addtype.fns[10] = (void *)GizmoObstacle_Activate;
  addtype.fns[11] = (void *)GizmoObstacle_ActivateRev;
  addtype.fns[12] = (void *)GizmoObstacle_SetVisibility;
  addtype.fns[13] = NULL;
  addtype.fns[14] = (void *)GizmoObstacle_GetPos;
  addtype.fns[15] = NULL;
  addtype.fns[16] = (void *)GizObstacles_BoltHitPlat;
  addtype.fns[17] = (void *)GizObstacles_GetBestBoltTarget;
  addtype.fns[18] = (void *)GizObstacles_BoltHit;
  addtype.fns[19] = (void *)GizObstacles_AllocateProgressData;
  addtype.fns[20] = (void *)GizObstacles_ClearProgress;
  addtype.fns[21] = (void *)GizObstacles_StoreProgress;
  addtype.fns[22] = (void *)GizObstacles_Reset;
  addtype.fns[23] = (void *)GizObstacles_ReserveBufferSpace;
  addtype.fns[24] = (void *)GizObstacles_Load;
  addtype.fns[25] = NULL;
  addtype.fns[26] = (void *)GizObstacles_PostLoad;
  addtype.fns[27] = (void *)GizObstacles_AddLevelSfx;
  obstacle_gizmotype_id = type_id;
  return &addtype;
}
