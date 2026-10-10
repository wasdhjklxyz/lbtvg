// gameapi/unk_00670260.cpp: TU of unknown name, found by its header-static
// copies (the functions after them are not matched yet).

#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"

// FUNCTION: LEGOBATMAN 0x00670260
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_00670260(f32 *v, f32 a, i32 i) { NuVec4Set(v, a, a, a, a); }

// The rest of this TU is the Mac's GizMiniCut file (saga
// legoapi/gizmos/trigger/gizminicut.cpp, GizMiniCut_Load ..
// MiniCut_RegisterGizmo 0x670b20).

#include "../batman/leveldata_unk.h"
#include "../batman/worldinfo_unk.h"
#include <stddef.h>
#include <string.h>

typedef struct MINICUTPART_s {
  u8 pad0[0x20];
  nuvec_s *resolved_position; // 0x20
  u8 pad24[0x44 - 0x24];
} MINICUTPART;

typedef struct MINICUT_s {
  char name[0x10];    // 0x00
  MINICUTPART *parts; // 0x10
  i32 part_count;     // 0x14
  u8 state;           // 0x18
  u8 pad19;
  u16 guid; // 0x1a
  f32 f1c;  // 0x1c
  f32 f20;  // 0x20
  f32 f24;  // 0x24
  f32 f28;  // 0x28
  f32 f2c;  // 0x2c
} MINICUT;

typedef struct GIZMO_s {
  void *object;
} GIZMO;

void AddGizmo(GIZMOSYS_s *gizmo_sys, i32 type_id, void *a, void *object);
int sprintf(char *buf, const char *fmt, ...);
void GizMiniCut_Reset(MINICUT *minicut, WORLDINFO_s *world);

// FUNCTION: LEGOBATMAN 0x006703d0
char *GizMiniCut_GetGizmoName(GIZMO *gizmo) {
  return gizmo != NULL ? (char *)gizmo->object : NULL;
}

// FUNCTION: LEGOBATMAN 0x006703e0
i32 GizMiniCut_GetMaxGizmos(void *world_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  return world->current_level->max_minicuts;
}

// FUNCTION: LEGOBATMAN 0x00670400
i32 GizMiniCut_GetOutput(GIZMO *gizmo, i32 output_index, i32 b) {
  if (gizmo == NULL || gizmo->object == NULL)
    return 0;
  MINICUT *minicut = (MINICUT *)gizmo->object;
  switch (output_index) {
  case 0:
    return minicut->state == 4;
  case 1:
    return minicut->state >= 3;
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x00670440
void GizMiniCut_AddGizmos(GIZMOSYS_s *gizmo_sys, i32 type_id, void *world_ptr,
                          void *unused) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  if (world->minicuts == NULL)
    return;
  for (i32 i = 0; i < world->minicut_count; i++)
    AddGizmo(gizmo_sys, type_id, NULL, &world->minicuts[i]);
}

// FUNCTION: LEGOBATMAN 0x00670490
i32 GizMiniCut_GetNumOutputs(GIZMO *gizmo) { return 2; }

// FUNCTION: LEGOBATMAN 0x006704a0
nuvec_s *GizMiniCut_GetPos(GIZMO *gizmo) {
  if (gizmo == NULL || gizmo->object == NULL)
    return NULL;
  MINICUT *minicut = (MINICUT *)gizmo->object;
  if (minicut->part_count == 0)
    return NULL;
  return minicut->parts[0].resolved_position;
}

// FUNCTION: LEGOBATMAN 0x00670580
char *GizMiniCut_GetOutputName(GIZMO *gizmo, i32 output_index) {
  switch (output_index) {
  case 0:
    return "Played";
  case 1:
    return "Playing";
  }
  return "Unknown!";
}

// FUNCTION: LEGOBATMAN 0x006705a0
void *GizMiniCut_ReserveBufferSpace(void *world_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  world->minicuts = NULL;
  world->minicut_count = 0;
  if (world->current_level->max_minicuts != 0) {
    world->buf104.addr = (world->buf104.addr + 3) & ~3;
    world->minicuts = (MINICUT *)world->buf104.void_ptr;
    world->buf104.addr += world->current_level->max_minicuts * sizeof(MINICUT);
    memset(world->minicuts, 0,
           world->current_level->max_minicuts * sizeof(MINICUT));
    world->buf104.addr = (world->buf104.addr + 3) & ~3;
    world->minicut_parts = (MINICUTPART *)world->buf104.void_ptr;
    world->buf104.addr += world->current_level->max_minicutParts *
                          world->current_level->max_minicuts *
                          sizeof(MINICUTPART);
    memset(world->minicut_parts, 0,
           world->current_level->max_minicutParts *
               world->current_level->max_minicuts * sizeof(MINICUTPART));
    for (i32 i = 0; i < world->current_level->max_minicuts; i++) {
      world->minicuts[i].parts =
          &world->minicut_parts[world->current_level->max_minicutParts * i];
      world->minicuts[i].f24 = 2.5f;
      world->minicuts[i].f28 = 2.5f;
      world->minicuts[i].f20 = 10.0f;
      world->minicuts[i].f2c = 10.0f;
      world->minicuts[i].f1c = 0.0f;
      world->minicuts[i].part_count = 0;
      sprintf(world->minicuts[i].name, "Minicut %i", i + 1);
    }
  }
  return world->minicuts;
}

// FUNCTION: LEGOBATMAN 0x006708b0
i32 GizMiniCut_GetGuid(GIZMO *gizmo) {
  if (gizmo == NULL || gizmo->object == NULL)
    return -1;
  return ((MINICUT *)gizmo->object)->guid;
}

// FUNCTION: LEGOBATMAN 0x006709b0
void GizMiniCut_ResetAll(void *world_ptr, void *a, void *b) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  for (i32 i = 0; i < world->minicut_count; i++)
    GizMiniCut_Reset(&world->minicuts[i], world);
}

typedef struct ADDGIZMOTYPE_s {
  char *name;        // 0x00
  char *prefix;      // 0x04
  u16 progress_size; // 0x08
  void *fns[0x1c];   // 0x0c
} ADDGIZMOTYPE;

// GLOBAL: LEGOBATMAN 0x00960118
extern ADDGIZMOTYPE Default_ADDGIZMOTYPE;

i32 GizMiniCut_Load(void *world, void *unused);
void GizMiniCut_Update(void *world, void *unused, f32 dt);
void GizMiniCut_Activate(GIZMO *gizmo, i32 active);
i32 GizMiniCut_ActivateRev(GIZMO *gizmo, i32 value, i32 query);
i32 GizMiniCut_UsingSpecial(void);

// GLOBAL: LEGOBATMAN 0x009683e4
i32 minicut_gizmotype_id = -1;

// FUNCTION: LEGOBATMAN 0x00670b20
ADDGIZMOTYPE *GizMiniCut_RegisterGizmo(i32 type_id) {
  // GLOBAL: LEGOBATMAN 0x00ad2970
  static ADDGIZMOTYPE addtype;

  addtype = Default_ADDGIZMOTYPE;
  addtype.progress_size = 0;
  addtype.fns[2] = NULL;
  addtype.fns[4] = NULL;
  addtype.fns[5] = NULL;
  addtype.fns[12] = NULL;
  addtype.fns[13] = NULL;
  addtype.fns[16] = NULL;
  addtype.fns[17] = NULL;
  addtype.fns[18] = NULL;
  addtype.fns[19] = NULL;
  addtype.fns[20] = NULL;
  addtype.fns[21] = NULL;
  addtype.fns[25] = NULL;
  addtype.fns[26] = NULL;
  addtype.fns[27] = NULL;
  minicut_gizmotype_id = type_id;
  addtype.name = "MiniCut";
  addtype.prefix = "";
  addtype.fns[0] = (void *)GizMiniCut_GetMaxGizmos;
  addtype.fns[1] = (void *)GizMiniCut_AddGizmos;
  addtype.fns[3] = (void *)GizMiniCut_Update;
  addtype.fns[6] = (void *)GizMiniCut_GetGizmoName;
  addtype.fns[7] = (void *)GizMiniCut_GetOutput;
  addtype.fns[8] = (void *)GizMiniCut_GetOutputName;
  addtype.fns[9] = (void *)GizMiniCut_GetNumOutputs;
  addtype.fns[10] = (void *)GizMiniCut_Activate;
  addtype.fns[11] = (void *)GizMiniCut_ActivateRev;
  addtype.fns[14] = (void *)GizMiniCut_GetPos;
  addtype.fns[15] = (void *)GizMiniCut_UsingSpecial;
  addtype.fns[22] = (void *)GizMiniCut_ResetAll;
  addtype.fns[23] = (void *)GizMiniCut_ReserveBufferSpace;
  addtype.fns[24] = (void *)GizMiniCut_Load;
  return &addtype;
}
