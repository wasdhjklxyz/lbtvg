// batman/unk_00608b00.cpp: TU of unknown name, found by its header-static
// copies (the functions after them are not matched yet).

#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"

// FUNCTION: LEGOBATMAN 0x00608b00
static f32 NuFabs(f32 f);
// FUNCTION: LEGOBATMAN 0x00608b20
static f32 NuSinApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x00608bc0
static f32 NuCosApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x00608bd0
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_00608b00(f32 *v, f32 a, i32 i) {
  v[2] = NuFabs(a);
  v[0] = NuSinApprox(i);
  v[1] = NuCosApprox(i);
  NuVec4Set(v, a, a, a, a);
}

// The rest of this TU is the Mac's PushBlocks file (.. PushBlocks
// RegisterGizmo 0x60c5d0); callbacks named by their RegisterGizmo slot.

#include "leveldata_unk.h"
#include "worldinfo_unk.h"
#include <stddef.h>
#include <string.h>

struct nuhspecial_s {
  void *scene;
  void *special;
  void *display_special;
};

typedef struct PUSHBLOCK_s {
  u8 pad0[0x24];
  nuhspecial_s special; // 0x24
  u8 pad30[0x40 - 0x30];
  f32 f40;         // 0x40
  char name[0x10]; // 0x44
  u8 pad54[0x5c - 0x54];
  nuhspecial_s parts[1]; // 0x5c, num_parts of them
  u8 pad68[0xe0 - 0x68];
  f32 fe0; // 0xe0
  u32 flags_lo : 18;
  u32 visible : 1; // 0xe4 bit 18
  u32 b19 : 1;
  u32 b20 : 1;
  u32 b21 : 1;
  u32 b22 : 1;
  u32 b23 : 1;
  u32 active : 1; // 0xe4 bit 24
  u32 b25 : 1;
  u32 b26 : 1;
  u32 b27 : 1;
  u32 b28 : 1;
  u32 b29 : 1;
  u32 b30 : 1;
  u32 b31 : 1;
  u32 c0 : 1; // 0xe8
  u32 c1 : 1;
  u32 c2 : 1;
  u32 c3 : 1;
  u8 complete;    // 0xec, bit n = target n reached
  u8 num_parts;   // 0xed
  u8 num_outputs; // 0xee
  u8 padef[0xf4 - 0xef];
} PUSHBLOCK;

typedef struct PUSHBLOCKPROGRESS_s {
  u32 a0[4]; // 0x00
  i32 f10;   // 0x10
  i32 f14;   // 0x14
  i32 f18;   // 0x18
  u8 a1c[0xdc - 0x1c];
  u8 adc[0x180]; // 0xdc
} PUSHBLOCKPROGRESS;

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
// GLOBAL: LEGOBATMAN 0x00962f9c
extern char PushBlock_OutputNameBuf[];

i32 NuStrLen(const char *s);
void NuStrCat(char *dst, const char *src);
char *NuIToA(i32 value, char *buffer, i32 radix);
extern "C" void NuSpecialSetVisibility(nuhspecial_s *sp, i32 visible);
void AddGizmo(GIZMOSYS_s *gizmo_sys, i32 type_id, void *a, void *object);
void *GameBufferAllocProgressUnk005bbaf0(VARIPTR *buf, VARIPTR *buf_end,
                                         i32 size);
void PushBlocks_ResetUnk00608d30(void *world, void *progress);
void PushBlock_ActivateUnk00608c40(WORLDINFO_s *world, PUSHBLOCK *block,
                                   i32 index);
void PushBlocks_Update(void *world, void *unused, f32 dt);

// FUNCTION: LEGOBATMAN 0x0060ba80
void *PushBlocks_ReserveBufferSpace(void *world_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  world->pushblocks = NULL;
  world->pushblock_count = 0;
  if (world->current_level->max_pushblocks > 0) {
    world->buf104.addr = (world->buf104.addr + 3) & ~3;
    world->pushblocks = (PUSHBLOCK *)world->buf104.void_ptr;
    world->buf104.addr +=
        world->current_level->max_pushblocks * sizeof(PUSHBLOCK);
  }
  return world->pushblocks;
}

// FUNCTION: LEGOBATMAN 0x0060bad0
void PushBlocks_Reset(void *world, void *unused, void *progress) {
  PushBlocks_ResetUnk00608d30(world, progress);
}

// FUNCTION: LEGOBATMAN 0x0060baf0
void PushBlock_Activate(GIZMO *gizmo, i32 active) {
  if (gizmo == NULL)
    return;
  PUSHBLOCK *block = (PUSHBLOCK *)gizmo->object;
  block->active = active != 0;
  if (active) {
    WORLDINFO_s *world = g_unk00960894;
    i32 i;
    for (i = 0; i < world->pushblock_count; i++) {
      if (block == &world->pushblocks[i])
        break;
    }
    if (i < world->pushblock_count)
      PushBlock_ActivateUnk00608c40(world, block, i);
    block->visible = 1;
  }
}

// FUNCTION: LEGOBATMAN 0x0060bb70
void PushBlock_SetVisibility(GIZMO *gizmo, i32 visible) {
  PUSHBLOCK *block = (PUSHBLOCK *)gizmo->object;
  block->visible = visible != 0;
  NuSpecialSetVisibility(&block->special, visible);
  for (i32 i = 0; i < block->num_parts; i++)
    NuSpecialSetVisibility(&block->parts[i], block->visible);
}

// FUNCTION: LEGOBATMAN 0x0060bbe0
i32 PushBlocks_GetMaxGizmos(void *world_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  if (world != NULL)
    return world->current_level->max_pushblocks;
  return 0;
}

// FUNCTION: LEGOBATMAN 0x0060bc00
void PushBlocks_AddGizmos(GIZMOSYS_s *gizmo_sys, i32 type_id, void *world_ptr,
                          void *unused) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  for (i32 i = 0; i < world->pushblock_count; i++) {
    if (NuStrLen(world->pushblocks[i].name) != 0)
      AddGizmo(gizmo_sys, type_id, NULL, &world->pushblocks[i]);
  }
}

// FUNCTION: LEGOBATMAN 0x0060bc70
char *PushBlock_GetGizmoName(GIZMO *gizmo) {
  return gizmo != NULL ? ((PUSHBLOCK *)gizmo->object)->name : NULL;
}

// FUNCTION: LEGOBATMAN 0x0060bc90
i32 PushBlock_IsCompleteUnk0060bc90(PUSHBLOCK *block, i32 target) {
  if (block != NULL) {
    if (target == 0) {
      if (block->complete != 0)
        return 1;
      return 0;
    }
    if (block->complete & (1 << target))
      return 1;
    return 0;
  }
  return -1;
}

// STUB: LEGOBATMAN 0x0060bcd0
// One jump differs: ours re-uses the output==0 jne, the original jumps
// straight to the return-0 tail.
i32 PushBlock_GetOutput(GIZMO *gizmo, i32 output, i32 b) {
  PUSHBLOCK *block = (PUSHBLOCK *)gizmo->object;
  if (block != NULL) {
    if (output == 0) {
      if (block->complete == 0)
        return 0;
    } else if ((block->complete & (1 << output)) == 0) {
      return 0;
    }
    return 1;
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x0060bd10
char *PushBlock_GetOutputName(GIZMO *gizmo, i32 output) {
  if (output < 0 || output > ((PUSHBLOCK *)gizmo->object)->num_outputs)
    return NULL;
  if (output == 0)
    return "Any Complete";
  NuIToA(output, PushBlock_OutputNameBuf, 10);
  NuStrCat(PushBlock_OutputNameBuf, " Complete");
  return PushBlock_OutputNameBuf;
}

// FUNCTION: LEGOBATMAN 0x0060bd60
i32 PushBlock_GetNumOutputs(GIZMO *gizmo) {
  return ((PUSHBLOCK *)gizmo->object)->num_outputs;
}

// FUNCTION: LEGOBATMAN 0x0060bd70
void *PushBlocks_AllocateProgressData(VARIPTR *buf, VARIPTR *buf_end) {
  return GameBufferAllocProgressUnk005bbaf0(buf, buf_end,
                                            sizeof(PUSHBLOCKPROGRESS));
}

// STUB: LEGOBATMAN 0x0060bd90
// The three header stores interleave with the memset pushes differently.
void PushBlocks_ClearProgress(void *world, void *progress_ptr) {
  PUSHBLOCKPROGRESS *progress = (PUSHBLOCKPROGRESS *)progress_ptr;
  if (progress != NULL) {
    progress->f14 = 0;
    progress->f10 = -1;
    progress->f18 = 0;
    memset(progress->a1c, 0, 0);
    memset(progress->a0, 0, sizeof(progress->a0));
    memset(progress->adc, 0, sizeof(progress->adc));
  }
}

void PushBlocks_StoreProgress(void *world, void *unused, void *progress);
void PushBlock_BoltHitPlat(void);
// GLOBAL: LEGOBATMAN 0x00963070
extern i32 g_unk00963070; // pushblock file version

i32 EdFileReadInt();
char EdFileReadChar();
f32 EdFileReadFloat();
void EdFileRead(void *buf, i32 len);
char *NuSpecialGetName(nuhspecial_s *sp);
int NuStrCpy(char *dst, const char *src);
i32 Unk_FindSpecial005bd230(nugscn_s *scene, nuhspecial_s *out, char *name,
                            i32 a, GIZMOSYS_s *gizmo_sys, char *type_name,
                            char *gizmo_name);

// STUB: LEGOBATMAN 0x0060bfa0
// body matches; register choice differs (orig world in ebx, never spilled;
// ours world in ebp spilled to the stack, name pointer in ebx).
i32 PushBlocks_Load(void *world_ptr, void *unused) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  char name[0x100];
  i32 i = 0;
  i32 count = 0;
  if (world->pushblock_count == 0) {
    g_unk00963070 = EdFileReadInt();
    world->pushblock_count = EdFileReadInt();
    for (i = 0; i < world->pushblock_count; i++) {
      PUSHBLOCK *pb = &world->pushblocks[count];
      i32 failed = 0;
      memset(name, 0, sizeof(name));
      EdFileRead(name, EdFileReadChar());
      if (!Unk_FindSpecial005bd230(world->scn140, &pb->special, name, 1,
                                   world->gizmoSys2b0c, "PushBlock", pb->name))
        failed = 1;
      else
        NuStrCpy(pb->name, NuSpecialGetName(&pb->special));
      if (g_unk00963070 >= 8) {
        memset(name, 0, sizeof(name));
        EdFileRead(name, EdFileReadChar());
        NuStrCpy(pb->name, name);
      }
      pb->f40 = EdFileReadFloat();
      pb->b26 = 0;
      if (EdFileReadChar())
        pb->b26 = 1;
      pb->b27 = 0;
      if (EdFileReadChar())
        pb->b27 = 1;
      pb->b20 = 0;
      if (EdFileReadChar())
        pb->b20 = 1;
      pb->b21 = 0;
      if (EdFileReadChar())
        pb->b21 = 1;
      if (g_unk00963070 >= 4) {
        pb->b29 = 0;
        if (EdFileReadChar())
          pb->b29 = 1;
        pb->b30 = 0;
        if (EdFileReadChar())
          pb->b30 = 1;
      }
      if (g_unk00963070 >= 5) {
        pb->b22 = 0;
        if (EdFileReadChar())
          pb->b22 = 1;
        pb->b23 = 0;
        if (EdFileReadChar())
          pb->b23 = 1;
      }
      pb->num_parts = 0;
      if (g_unk00963070 >= 3) {
        pb->num_parts = EdFileReadChar();
        for (i32 j = 0; j < pb->num_parts; j++) {
          memset(name, 0, sizeof(name));
          EdFileRead(name, EdFileReadChar());
          name[0x24] = 0;
          if (!Unk_FindSpecial005bd230(world->scn140, &pb->parts[j], name, 1,
                                       world->gizmoSys2b0c,
                                       "PushBlockLinkObject", pb->name))
            pb->num_parts--;
        }
      }
      pb->b31 = 0;
      if (g_unk00963070 >= 6) {
        if (EdFileReadChar())
          pb->b31 = 1;
        pb->fe0 = EdFileReadFloat();
      } else {
        pb->fe0 = 1.0f;
      }
      if (g_unk00963070 >= 7) {
        pb->c0 = 0;
        if (EdFileReadChar())
          pb->c0 = 1;
        pb->c1 = 0;
        if (EdFileReadChar())
          pb->c1 = 1;
        pb->c2 = 0;
        if (EdFileReadChar())
          pb->c2 = 1;
        pb->c3 = 0;
        if (EdFileReadChar())
          pb->c3 = 1;
      } else {
        pb->c0 = 0;
        pb->c1 = 0;
        pb->c2 = 0;
        pb->c3 = 1;
      }
      if (!failed) {
        pb->active = 1;
        count++;
      }
    }
    world->pushblock_count = count;
    return 1;
  }
  return 0;
}

void PushBlocks_PostLoad(void *world, void *unused);

// GLOBAL: LEGOBATMAN 0x00aca758
i32 pushblock_gizmotype_id;

// FUNCTION: LEGOBATMAN 0x0060c5d0
ADDGIZMOTYPE *PushBlocks_RegisterGizmo(i32 type_id) {
  // GLOBAL: LEGOBATMAN 0x00963074
  static char *name = "PushBlocks";
  // GLOBAL: LEGOBATMAN 0x00aca798
  static ADDGIZMOTYPE addtype;

  addtype = Default_ADDGIZMOTYPE;
  addtype.name = name;
  addtype.prefix = "";
  addtype.progress_size = sizeof(PUSHBLOCKPROGRESS);
  addtype.fns[0] = (void *)PushBlocks_GetMaxGizmos;
  addtype.fns[1] = (void *)PushBlocks_AddGizmos;
  addtype.fns[2] = (void *)PushBlocks_Update;
  addtype.fns[3] = NULL;
  addtype.fns[4] = NULL;
  addtype.fns[5] = NULL;
  addtype.fns[6] = (void *)PushBlock_GetGizmoName;
  addtype.fns[7] = (void *)PushBlock_GetOutput;
  addtype.fns[8] = (void *)PushBlock_GetOutputName;
  addtype.fns[9] = (void *)PushBlock_GetNumOutputs;
  addtype.fns[10] = (void *)PushBlock_Activate;
  addtype.fns[12] = (void *)PushBlock_SetVisibility;
  addtype.fns[13] = NULL;
  addtype.fns[14] = NULL;
  addtype.fns[15] = NULL;
  addtype.fns[16] = (void *)PushBlock_BoltHitPlat;
  addtype.fns[17] = NULL;
  addtype.fns[18] = NULL;
  addtype.fns[19] = (void *)PushBlocks_AllocateProgressData;
  addtype.fns[20] = (void *)PushBlocks_ClearProgress;
  addtype.fns[21] = (void *)PushBlocks_StoreProgress;
  addtype.fns[22] = (void *)PushBlocks_Reset;
  addtype.fns[23] = (void *)PushBlocks_ReserveBufferSpace;
  addtype.fns[24] = (void *)PushBlocks_Load;
  addtype.fns[25] = NULL;
  addtype.fns[26] = (void *)PushBlocks_PostLoad;
  addtype.fns[27] = NULL;
  pushblock_gizmotype_id = type_id;
  return &addtype;
}
