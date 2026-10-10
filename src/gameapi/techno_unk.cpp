// gameapi/techno_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nu2api/nu3d/nuspecial.h"
#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"
#include "gameobject_unk.h"
#include <stdio.h>

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x005a8e00
static f32 NuVecMagInline(f32 *v);

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x005a8c90
static f32 NuFabs(f32 f);
// FUNCTION: LEGOBATMAN 0x005a8cb0
static f32 NuSinApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x005a8d50
static f32 NuCosApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x005a8d60
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);
// FUNCTION: LEGOBATMAN 0x005a8dc0
static void NuVecScaleInline(f32 *dst, f32 *src, f32 s);

struct TECHNO_s {
  u8 pad0[0x83];
  u8 target_mode; // 0x83
  u8 pad84[0xb8 - 0x84];
  void *controlled_object; // 0xb8
};

i32 NuSpecialCompare(nuhspecial_s *a, nuhspecial_s *b);

// GLOBAL: LEGOBATMAN 0x00960558
extern i32 LEGOCONTEXT_TECHNO;
// GLOBAL: LEGOBATMAN 0x00ab3960
extern GameObject_s *Player[8];

// FUNCTION: LEGOBATMAN 0x005a7bd0
i32 Techno_FindOperator(void *target, Unk_GameObject112c **pad,
                        GameObject_s **operator_object) {
  if (LEGOCONTEXT_TECHNO == -1)
    return 0;
  for (i32 index = 0; index < 8; ++index) {
    if (Player[index] != 0 && Player[index]->b9db == LEGOCONTEXT_TECHNO &&
        Player[index]->techno != 0) {
      TECHNO_s *techno = Player[index]->techno;
      if ((techno->target_mode == 2 &&
           NuSpecialCompare((nuhspecial_s *)target,
                            (nuhspecial_s *)techno->controlled_object) != 0) ||
          Player[index]->techno->controlled_object == target) {
        if (pad != 0) {
          *pad = Player[index]->p112c;
        }
        if (operator_object != 0) {
          *operator_object = Player[index];
        }
        return 1;
      }
    }
  }
  return 0;
}

// Material view for the rope setup (bitfields as in menu_unk.cpp).
struct GRAPPLEMTL_s {
  u32 head; // 0x00
  u8 pad4[0x40 - 4];
  u32 filter_mode : 4; // 0x40
  u32 alpha_mode : 2;
  u32 attrib6 : 2;
  u32 attrib8 : 2;
  u32 attrib10 : 2;
  u32 attrib12 : 2;
  u32 z_mode : 2;
  u32 attrib16 : 2;
  u32 attrib18 : 1;
  u32 attrib19 : 13;
  u8 pad44[0x70 - 0x44];
  f32 f70; // 0x70
  u16 tid; // 0x74
};

// GLOBAL: LEGOBATMAN 0x00a984c0
extern GRAPPLEMTL_s *GrappleMtls[4];

GRAPPLEMTL_s *NuMtlCreate(i32 count);
void NuMtlUpdate(GRAPPLEMTL_s *mtl);
void Unk007280b0(GRAPPLEMTL_s *mtl);
i32 Unk006e6060(char *name, variptr_u *buffer, variptr_u buffer_end);

// FUNCTION: LEGOBATMAN 0x005aa1f0
void InitGrappleMtls(variptr_u *buffer, variptr_u *buffer_end) {
  char name[0x20];
  if (GrappleMtls[0] != 0)
    GrappleMtls[0]->head = 0x04040404;
  for (i32 i = 0; i < 4; i++) {
    GRAPPLEMTL_s *mtl = NuMtlCreate(1);
    if (mtl != 0) {
      mtl->f70 = 1.0f;
      mtl->filter_mode = 1;
      mtl->alpha_mode = 1;
      mtl->attrib8 = 0;
      mtl->attrib10 = 0;
      mtl->attrib12 = 2;
      mtl->z_mode = 0;
      mtl->attrib16 = 2;
      sprintf(name, "stuff\\Rope%i", i + 1);
      buffer->addr = (buffer->addr + 0xf) & ~0xf;
      i32 tid = Unk006e6060(name, buffer, *buffer_end);
      if (tid == 0) {
        Unk007280b0(mtl);
      } else {
        mtl->tid = tid;
        GrappleMtls[i] = mtl;
        NuMtlUpdate(mtl);
      }
    }
  }
}

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_techno_unk(f32 *v, f32 a, i32 i) {
  v[2] = NuFabs(a);
  v[0] = NuSinApprox(i);
  v[1] = NuCosApprox(i);
  NuVec4Set(v, a, a, a, a);
  NuVecScaleInline(v + 8, v, a);
}

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_2_techno_unk(f32 *v, f32 a, i32 i) {
  v[48] = NuVecMagInline(v);
}

// The Grapple gizmo callbacks (Mac Grapples_*), named by their
// RegisterGizmo 0x5ab830 slot.

#include "../batman/leveldata_unk.h"
#include "../batman/worldinfo_unk.h"
#include <stddef.h>

typedef struct GRAPPLE_s {
  char name[0x10]; // 0x00
  u8 pad10[0x1e - 0x10];
  u16 active : 1;  // 0x1e
  u16 visible : 1; // 0x1e bit 1
  u8 pad20[0x58 - 0x20];
  f32 f58; // 0x58
  u8 pad5c[0x19c - 0x5c];
} GRAPPLE;

typedef struct GRAPPLEPROGRESS_s {
  u32 active[1];  // 0x00
  u32 visible[1]; // 0x04
} GRAPPLEPROGRESS;

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
void AddGizmo(GIZMOSYS_s *gizmo_sys, i32 type_id, void *a, void *object);
void *GameBufferAllocProgressUnk005bbaf0(VARIPTR *buf, VARIPTR *buf_end,
                                         i32 size);
i32 Grapple_OccupiedUnk005a9640(GRAPPLE *grapple, i32 a, i32 b);
i32 Grapples_Load(void *world, void *unused);
void Grapples_Reset(void *world, void *unused, void *progress);
void Grapples_Update(void *world, void *unused, f32 dt);
void Grapples_Draw(void *world, void *unused, f32 dt);

// FUNCTION: LEGOBATMAN 0x005a9160
void *Grapples_ReserveBufferSpace(void *world_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  world->grapples = NULL;
  world->grapple_count = 0;
  if (world->current_level->max_grapples > 0) {
    world->buf104.addr = (world->buf104.addr + 3) & ~3;
    world->grapples = (GRAPPLE *)world->buf104.void_ptr;
    world->buf104.addr += world->current_level->max_grapples * sizeof(GRAPPLE);
  }
  return world->grapples;
}

// FUNCTION: LEGOBATMAN 0x005aac40
void Grapple_Activate(GIZMO *gizmo, i32 active) {
  if (gizmo != NULL) {
    GRAPPLE *grapple = (GRAPPLE *)gizmo->object;
    grapple->active = active != 0;
    if (grapple->active)
      grapple->f58 = 1.0f;
  }
}

// FUNCTION: LEGOBATMAN 0x005aac70
void Grapple_SetVisibility(GIZMO *gizmo, i32 visible) {
  if (gizmo != NULL)
    ((GRAPPLE *)gizmo->object)->visible = visible != 0;
}

// FUNCTION: LEGOBATMAN 0x005aac90
i32 Grapples_GetMaxGizmos(void *world_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  if (world != NULL)
    return world->current_level->max_grapples;
  return 0;
}

// FUNCTION: LEGOBATMAN 0x005aacb0
void Grapples_AddGizmos(GIZMOSYS_s *gizmo_sys, i32 type_id, void *world_ptr,
                        void *unused) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  for (i32 i = 0; i < world->grapple_count; i++) {
    if (NuStrLen(world->grapples[i].name) != 0)
      AddGizmo(gizmo_sys, type_id, NULL, &world->grapples[i]);
  }
}

// FUNCTION: LEGOBATMAN 0x005aad20
char *Grapple_GetGizmoName(GIZMO *gizmo) {
  return gizmo != NULL ? ((GRAPPLE *)gizmo->object)->name : NULL;
}

// STUB: LEGOBATMAN 0x005aad30
// The original keeps two calls; case 2 pushes eax (known 0), so its
// arguments are some expression we have not guessed.
i32 Grapple_GetOutput(GIZMO *gizmo, i32 output, i32 b) {
  GRAPPLE *grapple = (GRAPPLE *)gizmo->object;
  if (grapple->visible && grapple->active) {
    switch (output) {
    case 1:
      if (Grapple_OccupiedUnk005a9640(grapple, 0, 0))
        return 1;
      break;
    case 2:
      if (Grapple_OccupiedUnk005a9640(grapple, 0, 0))
        return 1;
      break;
    default:
      return 1;
    }
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x005aad80
char *Grapple_GetOutputName(GIZMO *gizmo, i32 output) {
  switch (output) {
  case 1:
    return "Occupied";
  case 2:
    return "Occupied By 2";
  }
  return "Active";
}

// FUNCTION: LEGOBATMAN 0x005aada0
i32 Grapple_GetNumOutputs(GIZMO *gizmo) { return 3; }

// FUNCTION: LEGOBATMAN 0x005aadb0
void *Grapples_AllocateProgressData(VARIPTR *buf, VARIPTR *buf_end) {
  return GameBufferAllocProgressUnk005bbaf0(buf, buf_end,
                                            sizeof(GRAPPLEPROGRESS));
}

// FUNCTION: LEGOBATMAN 0x005aadd0
void Grapples_ClearProgress(void *world, void *progress_ptr) {
  GRAPPLEPROGRESS *progress = (GRAPPLEPROGRESS *)progress_ptr;
  if (progress != NULL) {
    progress->active[0] = 0xffffffff;
    progress->visible[0] = 0xffffffff;
  }
}

// FUNCTION: LEGOBATMAN 0x005aadf0
void Grapples_StoreProgress(void *world_ptr, void *unused, void *progress_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  GRAPPLEPROGRESS *progress = (GRAPPLEPROGRESS *)progress_ptr;
  if (progress == NULL)
    return;
  Grapples_ClearProgress(NULL, progress);
  if (world != NULL && world->grapples != NULL) {
    GRAPPLE *grapple = world->grapples;
    for (i32 i = 0; i < world->grapple_count; i++, grapple++) {
      if (i >= 32)
        break;
      i32 word = i / 32;
      u32 bit = 1 << (i & 31);
      if (!grapple->visible)
        progress->visible[word] &= ~bit;
      if (!grapple->active)
        progress->active[word] &= ~bit;
    }
  }
}

// GLOBAL: LEGOBATMAN 0x0095fc30
i32 grapple_gizmotype_id = -1;

// FUNCTION: LEGOBATMAN 0x005ab830
ADDGIZMOTYPE *Grapples_RegisterGizmo(i32 type_id) {
  // GLOBAL: LEGOBATMAN 0x0095fd04
  static char *name = "Grapple";
  // GLOBAL: LEGOBATMAN 0x00a984d8
  static ADDGIZMOTYPE addtype;

  addtype = Default_ADDGIZMOTYPE;
  addtype.name = name;
  addtype.prefix = "";
  addtype.progress_size = sizeof(GRAPPLEPROGRESS);
  addtype.fns[0] = (void *)Grapples_GetMaxGizmos;
  addtype.fns[1] = (void *)Grapples_AddGizmos;
  addtype.fns[2] = NULL;
  addtype.fns[3] = (void *)Grapples_Update;
  addtype.fns[4] = (void *)Grapples_Draw;
  addtype.fns[5] = NULL;
  addtype.fns[6] = (void *)Grapple_GetGizmoName;
  addtype.fns[7] = (void *)Grapple_GetOutput;
  addtype.fns[8] = (void *)Grapple_GetOutputName;
  addtype.fns[9] = (void *)Grapple_GetNumOutputs;
  addtype.fns[10] = (void *)Grapple_Activate;
  addtype.fns[11] = NULL;
  addtype.fns[12] = (void *)Grapple_SetVisibility;
  addtype.fns[13] = NULL;
  addtype.fns[14] = NULL;
  addtype.fns[15] = NULL;
  addtype.fns[16] = NULL;
  addtype.fns[17] = NULL;
  addtype.fns[18] = NULL;
  addtype.fns[19] = (void *)Grapples_AllocateProgressData;
  addtype.fns[20] = (void *)Grapples_ClearProgress;
  addtype.fns[21] = (void *)Grapples_StoreProgress;
  addtype.fns[22] = (void *)Grapples_Reset;
  addtype.fns[23] = (void *)Grapples_ReserveBufferSpace;
  addtype.fns[24] = (void *)Grapples_Load;
  addtype.fns[25] = NULL;
  addtype.fns[26] = NULL;
  addtype.fns[27] = NULL;
  grapple_gizmotype_id = type_id;
  return &addtype;
}
