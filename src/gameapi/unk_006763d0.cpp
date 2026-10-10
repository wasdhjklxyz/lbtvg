// gameapi/unk_006763d0.cpp: TU of unknown name, found by its header-static
// copies (the functions after them are not matched yet).

#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"

// FUNCTION: LEGOBATMAN 0x006763d0
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_006763d0(f32 *v, f32 a, i32 i) { NuVec4Set(v, a, a, a, a); }

// The rest of this TU is the Mac's Puzzle file (Puzzle_PreLoad ..
// Puzzle_Draw; Puzzle_RegisterGizmo at 0x678a70); callbacks named by their
// RegisterGizmo slot.

#include "../batman/leveldata_unk.h"
#include "../batman/worldinfo_unk.h"
#include <stddef.h>

typedef struct PUZZLE_s {
  char name[0x10]; // 0x00
  u8 pad10[0x54 - 0x10];
  u16 active : 1;  // 0x54
  u16 visible : 1; // 0x54 bit 1
  u16 solved : 1;  // 0x54 bit 2
  u8 pad56[0x94 - 0x56];
} PUZZLE;

typedef struct PUZZLESYS_s {
  PUZZLE *puzzles; // 0x00
  i32 count;       // 0x04
  u8 pad8[0x74 - 8];
} PUZZLESYS;

typedef struct PUZZLEPROGRESS_s {
  u32 active[1];  // 0x00
  u32 visible[1]; // 0x04
} PUZZLEPROGRESS;

typedef struct GIZMO_s {
  void *object;
} GIZMO;

void AddGizmo(GIZMOSYS_s *gizmo_sys, i32 type_id, void *a, void *object);
void *GameBufferAllocProgressUnk005bbaf0(VARIPTR *buf, VARIPTR *buf_end,
                                         i32 size);

// GLOBAL: LEGOBATMAN 0x00968a80
static char *Puzzle_OutputName[1] = {"Solved"};

// FUNCTION: LEGOBATMAN 0x006781b0
void *Puzzle_AllocateProgress(VARIPTR *buf, VARIPTR *buf_end) {
  return GameBufferAllocProgressUnk005bbaf0(buf, buf_end,
                                            sizeof(PUZZLEPROGRESS));
}

// FUNCTION: LEGOBATMAN 0x006781d0
void Puzzle_ClearProgress(void *world, void *progress_ptr) {
  PUZZLEPROGRESS *progress = (PUZZLEPROGRESS *)progress_ptr;
  if (progress != NULL) {
    progress->active[0] = 0xffffffff;
    progress->visible[0] = 0xffffffff;
  }
}

// FUNCTION: LEGOBATMAN 0x006781f0
void Puzzle_StoreProgress(void *world_ptr, void *unused, void *progress_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  PUZZLEPROGRESS *progress = (PUZZLEPROGRESS *)progress_ptr;
  if (progress == NULL)
    return;
  Puzzle_ClearProgress(NULL, progress);
  if (world == NULL)
    return;
  PUZZLE *puzzle = world->puzzle_sys->puzzles;
  if (puzzle != NULL) {
    for (i32 i = 0; i < world->puzzle_sys->count; i++, puzzle++) {
      if (i >= 4)
        break;
      i32 word = i / 32;
      u32 bit = 1 << (i & 31);
      if (!puzzle->visible)
        progress->visible[word] &= ~bit;
      if (!puzzle->active)
        progress->active[word] &= ~bit;
    }
  }
}

// FUNCTION: LEGOBATMAN 0x00678740
i32 Puzzle_GetMaxGizmos(void *world_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  return world != NULL ? world->current_level->max_puzzles : 0;
}

// FUNCTION: LEGOBATMAN 0x00678760
void Puzzle_AddGizmos(GIZMOSYS_s *gizmo_sys, i32 type_id, void *world_ptr,
                      void *unused) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  if (world != NULL && world->puzzle_sys != NULL &&
      world->puzzle_sys->puzzles != NULL) {
    for (i32 i = 0; i < world->puzzle_sys->count; i++)
      AddGizmo(gizmo_sys, type_id, NULL, &world->puzzle_sys->puzzles[i]);
  }
}

// FUNCTION: LEGOBATMAN 0x006787c0
char *Puzzle_GetGizmoName(GIZMO *gizmo) {
  if (gizmo != NULL && gizmo->object != NULL)
    return (char *)gizmo->object;
  return NULL;
}

// FUNCTION: LEGOBATMAN 0x006787e0
i32 Puzzle_GetOutput(GIZMO *gizmo, i32 output_index, i32 b) {
  if (gizmo != NULL && gizmo->object != NULL && output_index == 0 &&
      ((PUZZLE *)gizmo->object)->solved)
    return 1;
  return 0;
}

// FUNCTION: LEGOBATMAN 0x00678810
char *Puzzle_GetOutputName(GIZMO *gizmo, i32 output_index) {
  if ((u32)output_index <= 0)
    return Puzzle_OutputName[output_index];
  return NULL;
}

// FUNCTION: LEGOBATMAN 0x00678830
i32 Puzzle_GetNumOutputs(GIZMO *gizmo) { return 1; }

// FUNCTION: LEGOBATMAN 0x00678840
void Puzzle_Activate(GIZMO *gizmo, i32 active) {
  PUZZLE *puzzle = (PUZZLE *)gizmo->object;
  if (active)
    puzzle->active = 1;
  else
    puzzle->active = 0;
}

// FUNCTION: LEGOBATMAN 0x00678860
void Puzzle_SetVisibility(GIZMO *gizmo, i32 visible) {
  ((PUZZLE *)gizmo->object)->visible = visible;
}

// FUNCTION: LEGOBATMAN 0x00678880
i32 Puzzle_GetVisibility(GIZMO *gizmo) {
  if (gizmo != NULL && ((PUZZLE *)gizmo->object)->visible)
    return 1;
  return 0;
}
