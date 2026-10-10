// batman/unk_00485150.cpp: TU of unknown name, found by its header-static
// copies (the functions after them are not matched yet).

#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"
#include "../nu2api/numath/nuvec.h"
#include <stddef.h>
#include <string.h>

// FUNCTION: LEGOBATMAN 0x00485150
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_00485150(f32 *v, f32 a, i32 i) { NuVec4Set(v, a, a, a, a); }

// The rest of this TU is the Mac's GizBombGen file (saga
// legoapi/gizmos/traps/gizbombgen.cpp, Gizbombgen_ReadAnimSetData ..
// GizBombGen_RegisterGizmo 0x485e90); callbacks named by their
// RegisterGizmo slot.

#include "leveldata_unk.h"
#include "worldinfo_unk.h"

typedef struct GIZBOMBGEN_s {
  char name[0x10];  // 0x00
  nuvec_s position; // 0x10
  u8 pad1c[0x28 - 0x1c];
  void *anim_set; // 0x28
  i32 interval;   // 0x2c
  u8 pad30[0x34 - 0x30];
  f32 f34; // 0x34
  u8 pad38[0x3a - 0x38];
  u16 active : 1;  // 0x3a
  u16 visible : 1; // 0x3a bit 1
} GIZBOMBGEN;

typedef struct GIZBOMBGENSYS_s {
  GIZBOMBGEN *bomb_generators; // 0x00
  u16 count;                   // 0x04
  u16 capacity;                // 0x06
  void *anim_object_pool;      // 0x08
  u16 reset : 1;               // 0x0c
  u16 kill_bombs_on_reset : 1; // 0x0c bit 1
  void *progress;              // 0x10
} GIZBOMBGENSYS;

typedef struct GIZBOMBGENPROGRESS_s {
  u32 active_masks[3];  // 0x00
  u32 visible_masks[3]; // 0x0c
} GIZBOMBGENPROGRESS;

typedef struct GIZMO_s {
  void *object;
} GIZMO;

typedef struct GIZMOSYS_s GIZMOSYS;

typedef struct ADDGIZMOTYPE_s {
  char *name;        // 0x00
  char *prefix;      // 0x04
  u16 progress_size; // 0x08
  void *fns[0x1c];   // 0x0c
} ADDGIZMOTYPE;

// GLOBAL: LEGOBATMAN 0x00960118
extern ADDGIZMOTYPE Default_ADDGIZMOTYPE;
// GLOBAL: LEGOBATMAN 0x009c5a60
extern i32 g_unk009c5a60; // saga: come_from_an_editor

i32 NuStrLen(const char *s);
i32 NuStrICmp(const char *a, const char *b);
void AddGizmo(GIZMOSYS *gizmo_sys, i32 type_id, void *a, void *object);
void *GameBufferAllocProgressUnk005bbaf0(VARIPTR *buf, VARIPTR *buf_end,
                                         i32 size);
void GameAnimSet_SetVisibility(void *anim_set, i32 visible);

char EdFileReadChar();
i16 EdFileReadShort();
i32 EdFileReadInt();
f32 EdFileReadFloat();
void EdFileRead(void *buf, i32 len);
void EdFileReadNuVec(nuvec_s *v);
void *GameBufferAlloc(variptr_u *buf, variptr_u *buf_end, i32 size);
void *GameAnimSet_CreateObjectPool(variptr_u *buf, variptr_u *buf_end, i32 type,
                                   u16 count);
void *GameAnimSet_Create(variptr_u *buf, variptr_u *buf_end, void *pool,
                         struct GAMEANIMSYS_s *sys);
void GizmoFileReadGameAnimSet(void *anim_set, void *world,
                              void (*read)(void *obj, u8 version), u8 version,
                              char *type, char *name);

typedef struct GAMEANIMOBJ_s {
  u8 pad0[0x24];
  i16 *data; // 0x24
} GAMEANIMOBJ;

// FUNCTION: LEGOBATMAN 0x00485180
static void Gizbombgen_ReadAnimSetData(void *obj_ptr, u8 version) {
  GAMEANIMOBJ *obj = (GAMEANIMOBJ *)obj_ptr;
  if (obj != NULL) {
    i16 dummy;
    i16 *data = obj->data;
    if (data == NULL)
      data = &dummy;
    if (version > 1)
      *data = EdFileReadShort();
  }
}

// FUNCTION: LEGOBATMAN 0x004851b0
i32 GizBombGens_Load(void *world_ptr, void *system_ptr) {
  u8 version = EdFileReadChar();
  GIZBOMBGENSYS *system = (GIZBOMBGENSYS *)system_ptr;
  system->count = EdFileReadShort();
  if (system->count != 0) {
    GIZBOMBGEN *generator = system->bomb_generators;
    for (i32 i = 0; i < system->count; i++, generator++) {
      EdFileRead(generator->name, sizeof(generator->name));
      EdFileReadNuVec(&generator->position);
      generator->interval = EdFileReadInt();
      if (version > 1)
        generator->f34 = EdFileReadFloat();
      generator->active = 1;
      generator->visible = 1;
      GizmoFileReadGameAnimSet(generator->anim_set, world_ptr,
                               Gizbombgen_ReadAnimSetData, version,
                               "BombGenerator", generator->name);
    }
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x00485290
void *GizBombGens_ReserveBufferSpace(void *world_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  GIZBOMBGENSYS *system = (GIZBOMBGENSYS *)GameBufferAlloc(
      &world->buf104, &world->bufEnd108, sizeof(GIZBOMBGENSYS));
  system->capacity = world->current_level->max_bombgens;
  system->bomb_generators = (GIZBOMBGEN *)GameBufferAlloc(
      &world->buf104, &world->bufEnd108, system->capacity * sizeof(GIZBOMBGEN));
  system->anim_object_pool =
      GameAnimSet_CreateObjectPool(&world->buf104, &world->bufEnd108, 4,
                                   world->current_level->max_bombgen_objects);
  for (i32 i = 0; i < system->capacity; i++) {
    system->bomb_generators[i].anim_set =
        GameAnimSet_Create(&world->buf104, &world->bufEnd108,
                           system->anim_object_pool, world->game_anim_sys);
  }
  world->giz_bombgen_sys = system;
  return system;
}

// FUNCTION: LEGOBATMAN 0x00485390
void GizmoBombGen_Activate(GIZMO *gizmo, i32 active) {
  if (gizmo != NULL) {
    GIZBOMBGEN *bomb_generator = (GIZBOMBGEN *)gizmo->object;
    bomb_generator->active = active != 0;
  }
}

// FUNCTION: LEGOBATMAN 0x004853f0
void GizmoBombGen_SetVisibility(GIZMO *gizmo, i32 visible) {
  if (gizmo != NULL && gizmo->object != NULL) {
    GIZBOMBGEN *bomb_generator = (GIZBOMBGEN *)gizmo->object;
    GameAnimSet_SetVisibility(bomb_generator->anim_set, visible);
    bomb_generator->visible = visible != 0;
  }
}

// FUNCTION: LEGOBATMAN 0x00485440
nuvec_s *GizmoBombGen_GetPos(GIZMO *gizmo) {
  if (gizmo == NULL || gizmo->object == NULL)
    return NULL;
  return &((GIZBOMBGEN *)gizmo->object)->position;
}

// STUB: LEGOBATMAN 0x004854e0
// the original masks bit 1 only on the editor path and inserts the whole
// reset byte on the other; &&, if/else and both ternaries tried.
void GizBombGens_SetResetFlag(void *world_ptr, void *system_ptr,
                              void *progress_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  GIZBOMBGENSYS *system = (GIZBOMBGENSYS *)system_ptr;
  system->reset = 1;
  system->kill_bombs_on_reset =
      g_unk009c5a60 == 0 && (world->reset_flags & 1) != 0;
  system->progress = progress_ptr;
}

// FUNCTION: LEGOBATMAN 0x00485c50
i32 GizBombGens_GetMaxGizmos(void *world_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  return world != NULL ? world->current_level->max_bombgens : 0;
}

// FUNCTION: LEGOBATMAN 0x00485c70
void GizBombGens_AddGizmos(GIZMOSYS *gizmo_sys, i32 type_id, void *world,
                           void *system_ptr) {
  GIZBOMBGENSYS *system = (GIZBOMBGENSYS *)system_ptr;
  if (system == NULL)
    return;
  for (i32 i = 0; i < system->count; i++) {
    if (NuStrLen(system->bomb_generators[i].name) != 0)
      AddGizmo(gizmo_sys, type_id, NULL, &system->bomb_generators[i]);
  }
}

// FUNCTION: LEGOBATMAN 0x00485ce0
char *GizmoBombGen_GetGizmoName(GIZMO *gizmo) {
  return gizmo != NULL ? ((GIZBOMBGEN *)gizmo->object)->name : NULL;
}

// FUNCTION: LEGOBATMAN 0x00485d00
i32 GizmoBombGen_GetOutput(GIZMO *gizmo, i32 a, i32 b) {
  return ((GIZBOMBGEN *)gizmo->object)->active;
}

// FUNCTION: LEGOBATMAN 0x00485d20
char *GizmoBombGen_GetOutputName(GIZMO *gizmo, i32 output) { return "Active"; }

// FUNCTION: LEGOBATMAN 0x00485d30
i32 GizmoBombGen_GetNumOutputs(GIZMO *gizmo) { return 1; }

// FUNCTION: LEGOBATMAN 0x00485d40
void *GizBombGens_AllocateProgressData(VARIPTR *buf, VARIPTR *buf_end) {
  return GameBufferAllocProgressUnk005bbaf0(buf, buf_end,
                                            sizeof(GIZBOMBGENPROGRESS));
}

// FUNCTION: LEGOBATMAN 0x00485d60
void GizBombGens_ClearProgress(void *world, void *progress_ptr) {
  GIZBOMBGENPROGRESS *progress = (GIZBOMBGENPROGRESS *)progress_ptr;
  if (progress != NULL) {
    memset(progress->active_masks, 0xff, sizeof(progress->active_masks));
    memset(progress->visible_masks, 0xff, sizeof(progress->visible_masks));
  }
}

// STUB: LEGOBATMAN 0x00485d90
// the original clears the result and pushes every register before the null
// checks; ours returns early (&&, nested ifs, early return tried).
GIZBOMBGEN *GizBombGen_FindByName(GIZBOMBGENSYS *system, char *name) {
  GIZBOMBGEN *generator = NULL;
  if (system == NULL || name == NULL)
    return generator;
  generator = system->bomb_generators;
  for (i32 i = 0; i < system->count; i++, generator++) {
    if (NuStrICmp(generator->name, name) == 0)
      break;
  }
  return generator;
}

// FUNCTION: LEGOBATMAN 0x00485df0
void GizBombGens_StoreProgress(void *world, void *system_ptr,
                               void *progress_ptr) {
  GIZBOMBGENSYS *system = (GIZBOMBGENSYS *)system_ptr;
  GIZBOMBGENPROGRESS *progress = (GIZBOMBGENPROGRESS *)progress_ptr;
  if (progress == NULL)
    return;
  memset(progress->active_masks, 0xff, sizeof(progress->active_masks));
  memset(progress->visible_masks, 0xff, sizeof(progress->visible_masks));
  GIZBOMBGEN *generator = system->bomb_generators;
  for (i32 i = 0; i < system->count && i < 96; i++, generator++) {
    i32 word = i / 32;
    u32 mask = 1 << (i & 31);
    if (!generator->visible)
      progress->visible_masks[word] &= ~mask;
    if (!generator->active)
      progress->active_masks[word] &= ~mask;
  }
}

void GizBombGens_Update(void *world, void *system, f32 dt);

// GLOBAL: LEGOBATMAN 0x0093e064
i32 bombgen_gizmotype_id = -1;

// FUNCTION: LEGOBATMAN 0x00485e90
ADDGIZMOTYPE *GizBombGen_RegisterGizmo(i32 type_id) {
  // GLOBAL: LEGOBATMAN 0x0093e134
  static char *name = "BombGenerator";
  // GLOBAL: LEGOBATMAN 0x009c6240
  static ADDGIZMOTYPE addtype;

  addtype = Default_ADDGIZMOTYPE;
  addtype.name = name;
  addtype.prefix = "";
  addtype.progress_size = sizeof(GIZBOMBGENPROGRESS);
  addtype.fns[0] = (void *)GizBombGens_GetMaxGizmos;
  addtype.fns[1] = (void *)GizBombGens_AddGizmos;
  addtype.fns[2] = NULL;
  addtype.fns[3] = (void *)GizBombGens_Update;
  addtype.fns[4] = NULL;
  addtype.fns[5] = NULL;
  addtype.fns[6] = (void *)GizmoBombGen_GetGizmoName;
  addtype.fns[7] = (void *)GizmoBombGen_GetOutput;
  addtype.fns[8] = (void *)GizmoBombGen_GetOutputName;
  addtype.fns[9] = (void *)GizmoBombGen_GetNumOutputs;
  addtype.fns[10] = (void *)GizmoBombGen_Activate;
  addtype.fns[11] = NULL;
  addtype.fns[12] = (void *)GizmoBombGen_SetVisibility;
  addtype.fns[13] = NULL;
  addtype.fns[14] = (void *)GizmoBombGen_GetPos;
  addtype.fns[15] = NULL;
  addtype.fns[16] = NULL;
  addtype.fns[17] = NULL;
  addtype.fns[18] = NULL;
  addtype.fns[19] = (void *)GizBombGens_AllocateProgressData;
  addtype.fns[20] = (void *)GizBombGens_ClearProgress;
  addtype.fns[21] = (void *)GizBombGens_StoreProgress;
  addtype.fns[22] = (void *)GizBombGens_SetResetFlag;
  addtype.fns[23] = (void *)GizBombGens_ReserveBufferSpace;
  addtype.fns[24] = (void *)GizBombGens_Load;
  addtype.fns[25] = NULL;
  addtype.fns[26] = NULL;
  addtype.fns[27] = NULL;
  bombgen_gizmotype_id = type_id;
  return &addtype;
}
