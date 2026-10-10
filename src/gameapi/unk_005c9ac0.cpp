// gameapi/unk_005c9ac0.cpp: TU of unknown name, found by its header-static
// copies (the functions after them are not matched yet).

#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"

// FUNCTION: LEGOBATMAN 0x005c9ac0
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_005c9ac0(f32 *v, f32 a, i32 i) { NuVec4Set(v, a, a, a, a); }

// The rest of this TU is the Mac's Shard file (ShardSys_Init ..
// Shards_RegisterGizmo 0x5ca810, then Shard_Collect); callbacks named by
// their RegisterGizmo slot.

#include "../batman/leveldata_unk.h"
#include "../batman/worldinfo_unk.h"
#include "../nu2api/numath/numtx.h"
#include <stddef.h>
#include <string.h>

typedef struct SHARD_s {
  char name[0x10];       // 0x00
  nuvec_s position;      // 0x10
  nuvec_s draw_position; // 0x1c
  u8 pad28[0x34 - 0x28];
  i16 special;      // 0x34, index into ShardSys->list
  u16 rot36;        // 0x36
  u16 rot38;        // 0x38
  u16 rot3a;        // 0x3a
  u8 active : 1;    // 0x3c
  u8 visible : 1;   // 0x3c bit 1
  u8 spinning : 1;  // 0x3c bit 2
  u8 collected : 1; // 0x3c bit 3
  u8 drawn : 1;     // 0x3c bit 4
  u8 touched : 1;   // 0x3c bit 5
  u8 tilt_x : 1;    // 0x3c bit 6, the spin goes into rot36 instead of rot38
  u8 pad3d[0x40 - 0x3d];
  f32 f40;                 // 0x40
  GameObject_s *collector; // 0x44
  nuvec_s velocity;        // 0x48
} SHARD;

typedef struct SHARDSYS_s {
  i16 *list; // 0x00, -1 terminated
  u8 pad4[4];
  i32 count; // 0x08
} SHARDSYS;

typedef struct SHARDPROGRESS_s {
  u32 collected[4]; // 0x00
  u32 active[4];    // 0x10
  u32 visible[4];   // 0x20
} SHARDPROGRESS;

// 16-byte entries of WORLDINFO_s::p2b04.
typedef struct SHARDSPECIAL_s {
  u8 pad0[0xe];
  u8 loaded; // 0x0e
  u8 padf;
} SHARDSPECIAL;

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
// GLOBAL: LEGOBATMAN 0x00960964
extern SHARDSYS *ShardSys;
// GLOBAL: LEGOBATMAN 0x0096096c
extern char *Shard_OutputName;

i32 NuStrLen(const char *s);
void AddGizmo(GIZMOSYS_s *gizmo_sys, i32 type_id, void *a, void *object);
void *GameBufferAllocProgressUnk005bbaf0(VARIPTR *buf, VARIPTR *buf_end,
                                         i32 size);
i32 qrand(void);
i32 EdFileReadInt();
u16 EdFileReadUnsignedShort();
void EdFileRead(void *buf, i32 len);
void EdFileReadNuVec(nuvec_s *v);
void NuMtxSetRotationY(numtx_s *m, i32 a);
void NuMtxRotateX(numtx_s *m, i32 a);
void NuMtxRotateZ(numtx_s *m, i32 a);
void NuMtxTranslate(numtx_s *m, nuvec_s *v);
int NuSpecialDrawAt(nuhspecial_s *sp, numtx_s *mtx);
void NuVecRotateZUnk00684ad0(nuvec_s *v, nuvec_s *v0, i32 a);
void NuVecScale(nuvec_s *out, nuvec_s *v, f32 s);
unsigned __int64 Cheats_CheckFlags(unsigned __int64 flags);
void NewBuzzFrames(nupad_s *pad, i32 frames, i32);

// GLOBAL: LEGOBATMAN 0x0095fd20
extern nuvec_s g_unk0095fd20;

// FUNCTION: LEGOBATMAN 0x005c9ae0
void ShardSys_Init(SHARDSYS *sys) {
  if (sys->list != NULL && sys->list[0] != -1) {
    ShardSys = sys;
    sys->count = 0;
    while (ShardSys->list[ShardSys->count] != -1)
      ShardSys->count++;
  }
}

// FUNCTION: LEGOBATMAN 0x005c9b30
i32 Shards_Load(void *world_ptr, void *unused) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  if (world->shard_count == 0) {
    i32 version = EdFileReadInt();
    world->shard_count = EdFileReadInt();
    for (i32 i = 0; i < world->shard_count; i++) {
      EdFileRead(world->shards[i].name, 0x10);
      EdFileReadNuVec(&world->shards[i].position);
      if (version >= 2) {
        world->shards[i].rot36 = EdFileReadUnsignedShort();
        world->shards[i].rot38 = EdFileReadUnsignedShort();
      } else {
        world->shards[i].rot36 = 0;
        world->shards[i].rot38 = 0;
      }
    }
    return 1;
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x005c9be0
void *Shards_ReserveBufferSpace(void *world_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  world->shards = NULL;
  world->shard_count = 0;
  if (world->current_level->max_shards > 0) {
    world->buf104.addr = (world->buf104.addr + 3) & ~3;
    world->shards = (SHARD *)world->buf104.void_ptr;
    world->buf104.addr += world->current_level->max_shards * sizeof(SHARD);
  }
  return world->shards;
}

// FUNCTION: LEGOBATMAN 0x005c9c30
static void Shard_Reset(SHARD *shard, WORLDINFO_s *world) {
  shard->active = 1;
  shard->visible = 1;
  shard->collected = 0;
  shard->drawn = 0;
  shard->special = qrand() / (0xffff / ShardSys->count + 1);
  for (i32 i = 0; i < ShardSys->count; i++) {
    if (((SHARDSPECIAL *)world->p2b04)[ShardSys->list[shard->special]].loaded !=
        0)
      break;
    shard->special++;
    if (shard->special == ShardSys->count)
      shard->special = 0;
  }
  shard->rot3a = qrand();
  shard->f40 = 0.0f;
  shard->spinning = 0;
  shard->touched = 0;
  shard->draw_position = shard->position;
  shard->collector = NULL;
}

// FUNCTION: LEGOBATMAN 0x005c9ce0
void Shards_Reset(void *world_ptr, void *unused, void *progress_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  SHARDPROGRESS *progress = (SHARDPROGRESS *)progress_ptr;
  if (world == NULL)
    return;
  SHARD *shard = world->shards;
  if (shard == NULL)
    return;
  for (i32 i = 0; i < world->shard_count; i++, shard++) {
    Shard_Reset(&world->shards[i], world);
    if (progress != NULL && i < 128) {
      i32 word = i / 32;
      u32 bit = 1 << (i & 31);
      shard->collected = (progress->collected[word] & bit) != 0;
      shard->visible = (progress->visible[word] & bit) != 0;
      shard->active = (progress->active[word] & bit) != 0;
    }
  }
}

// FUNCTION: LEGOBATMAN 0x005c9dc0
void Shard_Activate(GIZMO *gizmo, i32 active) {
  if (gizmo != NULL)
    ((SHARD *)gizmo->object)->active = active != 0;
}

// FUNCTION: LEGOBATMAN 0x005c9de0
void Shard_SetVisibility(GIZMO *gizmo, i32 visible) {
  if (gizmo != NULL)
    ((SHARD *)gizmo->object)->visible = visible != 0;
}

// FUNCTION: LEGOBATMAN 0x005c9e00
void Shards_UpdateBeforeCharacters(void *world_ptr, void *unused, f32 dt) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  for (i32 i = 0; i < world->shard_count; i++)
    world->shards[i].touched = 0;
}

void Shards_UpdateAfterCharacters(void *world, void *unused, f32 dt);

// STUB: LEGOBATMAN 0x005ca470
// The original loads rot3a once above the spinning branch (both
// NuMtxSetRotationY calls use it); ours loads it per branch.
void Shards_Draw(void *world_ptr, void *unused, f32 dt) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  numtx_s mtx;
  SHARD *shard = world->shards;
  if (shard == NULL)
    return;
  for (i32 i = 0; i < world->shard_count; i++, shard++) {
    shard->drawn = 0;
    if (shard->collected || !shard->visible)
      continue;
    u16 rx;
    if (shard->spinning) {
      NuMtxSetRotationY(&mtx, (u16)(shard->f40 * 98304.0f + shard->rot3a));
      u16 spin = (u16)(shard->f40 * 60620.0f);
      u16 rz = shard->rot38;
      if (!shard->tilt_x)
        rz += spin;
      if (rz)
        NuMtxRotateZ(&mtx, rz);
      rx = shard->rot36;
      if (shard->tilt_x)
        rx += spin;
    } else {
      NuMtxSetRotationY(&mtx, shard->rot3a);
      if (shard->rot38)
        NuMtxRotateZ(&mtx, shard->rot38);
      rx = shard->rot36;
    }
    if (rx)
      NuMtxRotateX(&mtx, rx);
    NuMtxTranslate(&mtx, &shard->draw_position);
    SHARDSPECIAL *special =
        &((SHARDSPECIAL *)world->p2b04)[ShardSys->list[shard->special]];
    if (special->loaded)
      shard->drawn = NuSpecialDrawAt((nuhspecial_s *)special, &mtx);
  }
}

// FUNCTION: LEGOBATMAN 0x005ca610
i32 Shards_GetMaxGizmos(void *world_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  if (world != NULL)
    return world->current_level->max_shards;
  return 0;
}

// FUNCTION: LEGOBATMAN 0x005ca630
void Shards_AddGizmos(GIZMOSYS_s *gizmo_sys, i32 type_id, void *world_ptr,
                      void *unused) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  for (i32 i = 0; i < world->shard_count; i++) {
    if (NuStrLen(world->shards[i].name) != 0)
      AddGizmo(gizmo_sys, type_id, NULL, &world->shards[i]);
  }
}

// FUNCTION: LEGOBATMAN 0x005ca6a0
char *Shard_GetGizmoName(GIZMO *gizmo) {
  return gizmo != NULL ? ((SHARD *)gizmo->object)->name : NULL;
}

// FUNCTION: LEGOBATMAN 0x005ca6b0
i32 Shard_GetOutput(GIZMO *gizmo, i32 a, i32 b) {
  return ((SHARD *)gizmo->object)->collected;
}

// FUNCTION: LEGOBATMAN 0x005ca6d0
char *Shard_GetOutputName(GIZMO *gizmo, i32 output) { return Shard_OutputName; }

// FUNCTION: LEGOBATMAN 0x005ca6e0
i32 Shard_GetNumOutputs(GIZMO *gizmo) { return 1; }

// FUNCTION: LEGOBATMAN 0x005ca6f0
void *Shards_AllocateProgressData(VARIPTR *buf, VARIPTR *buf_end) {
  return GameBufferAllocProgressUnk005bbaf0(buf, buf_end,
                                            sizeof(SHARDPROGRESS));
}

// FUNCTION: LEGOBATMAN 0x005ca710
void Shards_ClearProgress(void *world, void *progress_ptr) {
  SHARDPROGRESS *progress = (SHARDPROGRESS *)progress_ptr;
  if (progress != NULL) {
    memset(progress->collected, 0, sizeof(progress->collected));
    memset(progress->active, 0xff, sizeof(progress->active));
    memset(progress->visible, 0xff, sizeof(progress->visible));
  }
}

// FUNCTION: LEGOBATMAN 0x005ca750
void Shards_StoreProgress(void *world_ptr, void *unused, void *progress_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  SHARDPROGRESS *progress = (SHARDPROGRESS *)progress_ptr;
  if (progress == NULL)
    return;
  Shards_ClearProgress(NULL, progress);
  if (world != NULL && world->shards != NULL) {
    SHARD *shard = world->shards;
    for (i32 i = 0; i < world->shard_count; i++, shard++) {
      if (i >= 128)
        break;
      i32 word = i / 32;
      u32 bit = 1 << (i & 31);
      if (shard->collected)
        progress->collected[word] |= bit;
      if (!shard->visible)
        progress->visible[word] &= ~bit;
      if (!shard->active)
        progress->active[word] &= ~bit;
    }
  }
}

// GLOBAL: LEGOBATMAN 0x00960a10
static char *Shard_Name = "Shard";

// FUNCTION: LEGOBATMAN 0x005ca810
ADDGIZMOTYPE *Shards_RegisterGizmo(i32 type_id) {
  // GLOBAL: LEGOBATMAN 0x00abe188
  static ADDGIZMOTYPE addtype;

  addtype = Default_ADDGIZMOTYPE;
  addtype.name = Shard_Name;
  addtype.prefix = "";
  addtype.progress_size = sizeof(SHARDPROGRESS);
  addtype.fns[0] = (void *)Shards_GetMaxGizmos;
  addtype.fns[1] = (void *)Shards_AddGizmos;
  addtype.fns[2] = (void *)Shards_UpdateBeforeCharacters;
  addtype.fns[3] = (void *)Shards_UpdateAfterCharacters;
  addtype.fns[4] = (void *)Shards_Draw;
  addtype.fns[5] = NULL;
  addtype.fns[6] = (void *)Shard_GetGizmoName;
  addtype.fns[7] = (void *)Shard_GetOutput;
  addtype.fns[8] = (void *)Shard_GetOutputName;
  addtype.fns[9] = (void *)Shard_GetNumOutputs;
  addtype.fns[10] = (void *)Shard_Activate;
  addtype.fns[11] = NULL;
  addtype.fns[12] = (void *)Shard_SetVisibility;
  addtype.fns[13] = NULL;
  addtype.fns[14] = NULL;
  addtype.fns[15] = NULL;
  addtype.fns[16] = NULL;
  addtype.fns[17] = NULL;
  addtype.fns[18] = NULL;
  addtype.fns[19] = (void *)Shards_AllocateProgressData;
  addtype.fns[20] = (void *)Shards_ClearProgress;
  addtype.fns[21] = (void *)Shards_StoreProgress;
  addtype.fns[22] = (void *)Shards_Reset;
  addtype.fns[23] = (void *)Shards_ReserveBufferSpace;
  addtype.fns[24] = (void *)Shards_Load;
  addtype.fns[25] = NULL;
  addtype.fns[26] = NULL;
  addtype.fns[27] = NULL;
  return &addtype;
}

// FUNCTION: LEGOBATMAN 0x005ca930
void Shard_Collect(SHARD *shard, GameObject_s *obj) {
  shard->f40 = 0.0f;
  shard->spinning = 1;
  shard->collector = obj;
  NuVecRotateZUnk00684ad0(&shard->velocity, &g_unk0095fd20, shard->rot38);
  NuVecRotateZUnk00684ad0(&shard->velocity, &shard->velocity, shard->rot36);
  f32 speed = Cheats_CheckFlags(0x80000000) ? 5.0f : 3.0f;
  NuVecScale(&shard->velocity, &shard->velocity, speed);
  NewBuzzFrames(shard->collector->p112c->pad0, 1, 0);
  shard->tilt_x = qrand() < 0x8000;
}

// FUNCTION: LEGOBATMAN 0x005ca9d0
void Shards_HandleLostObj(WORLDINFO_s *world, GameObject_s *obj) {
  SHARD *shard = world->shards;
  for (i32 i = 0; i < world->shard_count; i++, shard++) {
    if (!shard->collected && shard->spinning && shard->collector == obj) {
      shard->spinning = 0;
      shard->f40 = 0.0f;
      shard->collector = NULL;
    }
  }
}
