// gameapi/unk_00662a10.cpp: TU of unknown name, found by its header-static
// copies (the functions after them are not matched yet).

#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"

// FUNCTION: LEGOBATMAN 0x00662a10
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_00662a10(f32 *v, f32 a, i32 i) { NuVec4Set(v, a, a, a, a); }

// The rest of this TU is the Mac's GizSpecial file (saga
// legoapi/gizmos/trigger/gizspecial.cpp, GizSpecial_GetMaxGizmos ..
// GizSpecial_RegisterGizmo 0x6631c0).

#include "../batman/leveldata_unk.h"
#include "../batman/worldinfo_unk.h"
#include <stddef.h>
#include <string.h>

typedef struct GIZSPECIAL_s {
  char name[0x20];    // 0x00
  void *anim_set;     // 0x20
  u8 is_reversed : 1; // 0x24
  u8 is_active : 1;   // 0x24 bit 1
  u8 pad25[3];
} GIZSPECIAL;

typedef struct GIZSPECIALSYS_s {
  GIZSPECIAL *specials; // 0x00
  i32 count;            // 0x04
} GIZSPECIALSYS;

typedef struct GIZSPECIALPROGRESS_s {
  u32 active[8];   // 0x00
  u32 reversed[8]; // 0x20
} GIZSPECIALPROGRESS;

typedef struct GIZMO_s {
  void *object;
} GIZMO;

i32 NuStrLen(const char *s);
i32 NuStrICmp(const char *a, const char *b);
void AddGizmo(GIZMOSYS_s *gizmo_sys, i32 type_id, void *a, void *object);
void *GameBufferAllocProgressUnk005bbaf0(VARIPTR *buf, VARIPTR *buf_end,
                                         i32 size);
void GameAnimSet_JumpToStart(void *anim_set);
void GameAnimSet_Play(void *anim_set, f32 speed, i32 a);
void GameAnimSet_Stop(void *anim_set);
void GameAnimSet_SetVisibility(void *anim_set, i32 visible);

typedef struct ADDGIZMOTYPE_s {
  char *name;        // 0x00
  char *prefix;      // 0x04
  u16 progress_size; // 0x08
  void *fns[0x1c];   // 0x0c
} ADDGIZMOTYPE;

// GLOBAL: LEGOBATMAN 0x00960118
extern ADDGIZMOTYPE Default_ADDGIZMOTYPE;

// GLOBAL: LEGOBATMAN 0x00967ae4
static char gizSpec_prefix[] = "qaz_";
// GLOBAL: LEGOBATMAN 0x00967aec
i32 gizspecial_gizmotype_id = -1;

// FUNCTION: LEGOBATMAN 0x00662a30
i32 GizSpecial_GetMaxGizmos(void *special) {
  WORLDINFO_s *world = (WORLDINFO_s *)special;
  return world->current_level->max_gizspecials;
}

// FUNCTION: LEGOBATMAN 0x00662a50
void GizSpecial_AddGizmos(GIZMOSYS_s *gizmo_sys, i32 type_id, void *world_ptr,
                          void *unused) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  if (world != NULL) {
    for (i32 i = 0; i < world->giz_special_sys->count; i++) {
      if (NuStrLen(world->giz_special_sys->specials[i].name) != 0)
        AddGizmo(gizmo_sys, type_id, NULL,
                 &world->giz_special_sys->specials[i]);
    }
  }
}

// FUNCTION: LEGOBATMAN 0x00662c20
char *GizSpecial_GetName(GIZSPECIAL *special) { return special->name; }

// STUB: LEGOBATMAN 0x00662c30
// found path: ecx/edx/eax swapped in the final address computation (return
// in loop, goto found, pointer add tried).
GIZSPECIAL *GizSpecial_FindByName(char *name, WORLDINFO_s *world) {
  for (i32 i = 0; i < world->giz_special_sys->count; i++) {
    if (NuStrICmp(world->giz_special_sys->specials[i].name, name) == 0)
      return &world->giz_special_sys->specials[i];
  }
  return NULL;
}

// FUNCTION: LEGOBATMAN 0x00662c90
char *GizSpecial_GetGizmoName(GIZMO *gizmo) {
  if (gizmo != NULL)
    return GizSpecial_GetName((GIZSPECIAL *)gizmo->object);
  return NULL;
}

// FUNCTION: LEGOBATMAN 0x00662d20
char *GizSpecial_GetOutputName(GIZMO *gizmo, i32 output_index) {
  switch (output_index) {
  case 0:
    return "AtEnd";
  case 1:
    return "NotAtStart";
  }
  return NULL;
}

// FUNCTION: LEGOBATMAN 0x00662d40
i32 GizSpecial_GetNumOutputs(GIZMO *gizmo) { return 2; }

// FUNCTION: LEGOBATMAN 0x00662d50
void GizSpecial_Activate(GIZMO *gizmo, i32 active) {
  if (gizmo == NULL)
    return;
  GIZSPECIAL *special = (GIZSPECIAL *)gizmo->object;
  if (special == NULL)
    return;
  if (active != 0) {
    GameAnimSet_JumpToStart(special->anim_set);
    GameAnimSet_Play(special->anim_set, 1.0f, 1);
    special->is_active = 1;
    return;
  }
  GameAnimSet_Stop(special->anim_set);
  special->is_active = 0;
}

// FUNCTION: LEGOBATMAN 0x00662da0
i32 GizSpecial_ActivateRev(GIZMO *gizmo, i32 reversed, i32 test_only) {
  if (gizmo == NULL)
    return 0;
  GIZSPECIAL *special = (GIZSPECIAL *)gizmo->object;
  if (special == NULL)
    return 0;
  if ((test_only & 1) != 0)
    return special->is_reversed != reversed;
  if (reversed != 0) {
    GameAnimSet_Play(special->anim_set, -1.0f, 1);
    special->is_reversed = 1;
    return 1;
  }
  GameAnimSet_Play(special->anim_set, 1.0f, 1);
  special->is_reversed = 0;
  return 1;
}

// FUNCTION: LEGOBATMAN 0x00662e20
void GizSpecial_SetVisibility(GIZMO *gizmo, i32 visibility) {
  if (gizmo != NULL) {
    GIZSPECIAL *special = (GIZSPECIAL *)gizmo->object;
    if (special != NULL)
      GameAnimSet_SetVisibility(special->anim_set, visibility);
  }
}

// FUNCTION: LEGOBATMAN 0x00662f20
void GizSpecial_Reset(void *world_ptr, void *unused, void *progress_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  GIZSPECIALPROGRESS *progress = (GIZSPECIALPROGRESS *)progress_ptr;
  if (world == NULL || world->giz_special_sys == NULL ||
      world->giz_special_sys->count == 0)
    return;
  GIZSPECIAL *special = world->giz_special_sys->specials;
  for (i32 i = 0; i < world->giz_special_sys->count; i++, special++) {
    special->is_active = 1;
    special->is_reversed = 0;
    if (progress != NULL && i < 256) {
      i32 word = i / 32;
      u32 bit = 1 << (i & 31);
      special->is_active = (progress->active[word] & bit) != 0;
      special->is_reversed = (progress->reversed[word] & bit) != 0;
    }
  }
}

// FUNCTION: LEGOBATMAN 0x00662fc0
void *GizSpecial_AllocateProgressData(VARIPTR *buf, VARIPTR *buf_end) {
  return GameBufferAllocProgressUnk005bbaf0(buf, buf_end,
                                            sizeof(GIZSPECIALPROGRESS));
}

// FUNCTION: LEGOBATMAN 0x00662fe0
void GizSpecial_ClearProgress(void *world, void *progress_ptr) {
  GIZSPECIALPROGRESS *progress = (GIZSPECIALPROGRESS *)progress_ptr;
  if (progress != NULL) {
    memset(progress->active, 0xff, sizeof(progress->active));
    memset(progress->reversed, 0, sizeof(progress->reversed));
  }
}

// FUNCTION: LEGOBATMAN 0x00663020
void GizSpecial_StoreProgress(void *world_ptr, void *unused,
                              void *progress_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  GIZSPECIALPROGRESS *progress = (GIZSPECIALPROGRESS *)progress_ptr;
  if (progress == NULL)
    return;
  GizSpecial_ClearProgress(NULL, progress);
  if (world != NULL && world->giz_special_sys != NULL &&
      world->giz_special_sys->count != 0) {
    GIZSPECIAL *special = world->giz_special_sys->specials;
    for (i32 i = 0; i < world->giz_special_sys->count; i++, special++) {
      i32 word = i / 32;
      u32 bit = 1 << (i & 31);
      if (!special->is_active)
        progress->active[word] &= ~bit;
      if (special->is_reversed)
        progress->reversed[word] |= bit;
    }
  }
}

i32 GizSpecial_GetOutput(GIZMO *gizmo, i32 output_index, i32 include_inactive);
nuvec_s *GizSpecial_GetPos(GIZMO *gizmo);
i32 GizSpecial_UsingSpecial(GIZMO **result, void *world_ptr, i32 capacity,
                            char *name);
void *GizSpecial_ReserveBuffer(void *world_ptr);

// FUNCTION: LEGOBATMAN 0x006631c0
ADDGIZMOTYPE *GizSpecial_RegisterGizmo(i32 type_id) {
  // GLOBAL: LEGOBATMAN 0x00ad2090
  static ADDGIZMOTYPE addtype;

  addtype = Default_ADDGIZMOTYPE;
  addtype.name = "NuSpecial";
  addtype.prefix = gizSpec_prefix;
  addtype.progress_size = 0;
  addtype.fns[0] = (void *)GizSpecial_GetMaxGizmos;
  addtype.fns[1] = (void *)GizSpecial_AddGizmos;
  addtype.fns[2] = NULL;
  addtype.fns[3] = NULL;
  addtype.fns[4] = NULL;
  addtype.fns[5] = NULL;
  addtype.fns[6] = (void *)GizSpecial_GetGizmoName;
  addtype.fns[7] = (void *)GizSpecial_GetOutput;
  addtype.fns[8] = (void *)GizSpecial_GetOutputName;
  addtype.fns[9] = (void *)GizSpecial_GetNumOutputs;
  addtype.fns[10] = (void *)GizSpecial_Activate;
  addtype.fns[11] = (void *)GizSpecial_ActivateRev;
  addtype.fns[12] = (void *)GizSpecial_SetVisibility;
  addtype.fns[13] = NULL;
  addtype.fns[14] = (void *)GizSpecial_GetPos;
  addtype.fns[15] = (void *)GizSpecial_UsingSpecial;
  addtype.fns[16] = NULL;
  addtype.fns[17] = NULL;
  addtype.fns[18] = NULL;
  addtype.fns[19] = (void *)GizSpecial_AllocateProgressData;
  addtype.fns[20] = (void *)GizSpecial_ClearProgress;
  addtype.fns[21] = (void *)GizSpecial_StoreProgress;
  addtype.fns[22] = (void *)GizSpecial_Reset;
  addtype.fns[23] = (void *)GizSpecial_ReserveBuffer;
  addtype.fns[24] = NULL;
  addtype.fns[25] = NULL;
  addtype.fns[26] = NULL;
  addtype.fns[27] = NULL;
  gizspecial_gizmotype_id = type_id;
  return &addtype;
}
