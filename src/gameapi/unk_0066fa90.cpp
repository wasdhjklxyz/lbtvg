// gameapi/unk_0066fa90.cpp: TU of unknown name, found by its header-static
// copies (the functions after them are not matched yet).

#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"

// FUNCTION: LEGOBATMAN 0x0066fa90
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_0066fa90(f32 *v, f32 a, i32 i) { NuVec4Set(v, a, a, a, a); }

// The rest of this TU is the Mac's GizRandom file (saga
// legoapi/gizmos/trigger/gizrandom.cpp, GizRandom_GetMaxGizmos ..
// GizRandom_RegisterGizmo 0x66fd30).

#include "../batman/leveldata_unk.h"
#include "../batman/worldinfo_unk.h"
#include <stddef.h>

typedef struct GIZRANDOM_s {
  char name[0x10];       // 0x00
  i32 output_count;      // 0x10
  i32 output_weights[8]; // 0x14
  i32 selected_output;   // 0x34
  u8 active : 1;         // 0x38
} GIZRANDOM;

typedef struct GIZRANDOMSYS_s {
  GIZRANDOM *randoms; // 0x00
  i32 count;          // 0x04
  i32 capacity;       // 0x08
} GIZRANDOMSYS;

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
void NuStrNCpy(char *dst, const char *src, i32 n);
GIZMO *AddGizmo(GIZMOSYS_s *gizmo_sys, i32 type_id, void *a, void *object);
i32 qrand();
WORLDINFO_s *WorldInfo_CurrentlyLoading(void);

// GLOBAL: LEGOBATMAN 0x00968284
i32 gizrandom_gizmotype_id = -1;

// FUNCTION: LEGOBATMAN 0x0066fab0
i32 GizRandom_GetMaxGizmos(void *world_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  if (world == NULL || world->current_level == NULL)
    return 0;
  return world->current_level->max_gizrandoms;
}

// FUNCTION: LEGOBATMAN 0x0066fad0
void GizRandom_AddGizmos(GIZMOSYS_s *gizmo_sys, i32 type_id, void *world_ptr,
                         void *unused) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  for (i32 i = 0; i < world->giz_randoms->count; i++) {
    if (NuStrLen(world->giz_randoms->randoms[i].name) != 0)
      AddGizmo(gizmo_sys, type_id, NULL, &world->giz_randoms->randoms[i]);
  }
}

// FUNCTION: LEGOBATMAN 0x0066fb40
char *GizRandom_GetGizmoName(GIZMO *gizmo) {
  return gizmo != NULL ? ((GIZRANDOM *)gizmo->object)->name : NULL;
}

// FUNCTION: LEGOBATMAN 0x0066fb50
i32 GizRandom_GetOutput(GIZMO *gizmo, i32 output_index, i32 b) {
  GIZRANDOM *random = (GIZRANDOM *)gizmo->object;
  if (!random->active)
    return 0;
  return output_index == random->selected_output;
}

// FUNCTION: LEGOBATMAN 0x0066fb70
char *GizRandom_GetOutputName(GIZMO *gizmo, i32 output_index) {
  return "Random Output";
}

// FUNCTION: LEGOBATMAN 0x0066fb80
i32 GizRandom_GetNumOutputs(GIZMO *gizmo) {
  return ((GIZRANDOM *)gizmo->object)->output_count;
}

// FUNCTION: LEGOBATMAN 0x0066fb90
void GizRandom_Activate(GIZMO *gizmo, i32 active) {
  GIZRANDOM *random = (GIZRANDOM *)gizmo->object;
  i32 cumulative = 0;
  if (active != 0) {
    random->active = 1;
    random->selected_output = -1;
    i32 roll = 1 - (i32)(qrand() * (1.0 / 65535.0) * -100.0);
    for (i32 i = 0; i < random->output_count; i++) {
      if (roll >= cumulative &&
          roll <= cumulative + random->output_weights[i]) {
        random->selected_output = i;
        break;
      }
      cumulative += random->output_weights[i];
    }
  } else {
    random->active = 0;
  }
}

// FUNCTION: LEGOBATMAN 0x0066fc10
void *GizRandom_ReserveBufferSpace(void *world_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  world->giz_randoms = NULL;
  if (world->current_level->max_gizrandoms > 0) {
    world->buf104.addr = (world->buf104.addr + 3) & ~3;
    world->giz_randoms = (GIZRANDOMSYS *)world->buf104.void_ptr;
    world->buf104.addr = (world->buf104.addr + sizeof(GIZRANDOMSYS) + 3) & ~3;
    world->giz_randoms->randoms = (GIZRANDOM *)world->buf104.void_ptr;
    world->buf104.addr +=
        world->current_level->max_gizrandoms * sizeof(GIZRANDOM);
    world->giz_randoms->capacity = world->current_level->max_gizrandoms;
  }
  return world->giz_randoms;
}

// FUNCTION: LEGOBATMAN 0x0066fc90
GIZMO *createGizRandom(void *unused, i32 output_count, i32 *output_weights,
                       char *name) {
  WORLDINFO_s *world = WorldInfo_CurrentlyLoading();
  if (world == NULL)
    return NULL;
  if (world->giz_randoms->count == world->current_level->max_gizrandoms)
    return NULL;
  GIZRANDOM *random = &world->giz_randoms->randoms[world->giz_randoms->count];
  random->output_count = output_count;
  for (i32 i = 0; i < output_count; i++)
    random->output_weights[i] = output_weights[i];
  NuStrNCpy(random->name, name, sizeof(random->name));
  world->giz_randoms->count++;
  return AddGizmo(world->gizmoSys2b0c, gizrandom_gizmotype_id, NULL, random);
}

// FUNCTION: LEGOBATMAN 0x0066fd30
ADDGIZMOTYPE *GizRandom_RegisterGizmo(i32 type_id) {
  // GLOBAL: LEGOBATMAN 0x00ad2870
  static ADDGIZMOTYPE addtype;

  addtype = Default_ADDGIZMOTYPE;
  addtype.name = "GizRandom";
  addtype.prefix = "rnd_";
  addtype.progress_size = 0;
  addtype.fns[0] = (void *)GizRandom_GetMaxGizmos;
  addtype.fns[1] = (void *)GizRandom_AddGizmos;
  addtype.fns[5] = NULL;
  addtype.fns[6] = (void *)GizRandom_GetGizmoName;
  addtype.fns[7] = (void *)GizRandom_GetOutput;
  addtype.fns[8] = (void *)GizRandom_GetOutputName;
  addtype.fns[9] = (void *)GizRandom_GetNumOutputs;
  addtype.fns[10] = (void *)GizRandom_Activate;
  addtype.fns[11] = NULL;
  addtype.fns[12] = NULL;
  addtype.fns[13] = NULL;
  addtype.fns[14] = NULL;
  addtype.fns[15] = NULL;
  addtype.fns[16] = NULL;
  addtype.fns[17] = NULL;
  addtype.fns[18] = NULL;
  addtype.fns[19] = NULL;
  addtype.fns[20] = NULL;
  addtype.fns[21] = NULL;
  addtype.fns[22] = NULL;
  addtype.fns[23] = (void *)GizRandom_ReserveBufferSpace;
  addtype.fns[24] = NULL;
  addtype.fns[25] = NULL;
  addtype.fns[26] = NULL;
  addtype.fns[27] = NULL;
  gizrandom_gizmotype_id = type_id;
  return &addtype;
}
