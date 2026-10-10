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
#include <string.h>

typedef struct GRAPPLEROPEPOINT_s {
  nuvec_s pos; // 0x00
  u8 padc[0x24 - 0xc];
} GRAPPLEROPEPOINT;

typedef struct GRAPPLE_s {
  char name[0x10];         // 0x00
  nuvec_s position;        // 0x10
  u8 has_terrain_platform; // 0x1c
  u8 pad1d;
  u16 active : 1;                  // 0x1e
  u16 visible : 1;                 // 0x1e bit 1
  u16 disabled : 1;                // 0x1e bit 2
  u16 reversed : 1;                // 0x1e bit 3
  u16 b4 : 1;                      // 0x1e bit 4
  u16 no_special : 1;              // 0x1e bit 5
  u16 b6 : 2;                      // 0x1e bits 6-7
  u16 b8 : 8;                      // 0x1f
  u16 y_rotation;                  // 0x20
  u16 x_rotation;                  // 0x22
  nuvec_s ground_position;         // 0x24
  nuvec_s hook_position;           // 0x30
  nuvec_s shadow_probe_position;   // 0x3c
  u16 ground_x_rotation;           // 0x48
  u16 ground_z_rotation;           // 0x4a
  f32 f4c;                         // 0x4c
  f32 rope_length;                 // 0x50
  void *attached_object;           // 0x54
  f32 f58;                         // 0x58
  GRAPPLEROPEPOINT rope_points[6]; // 0x5c
  nuvec_s target_rope_points[6];   // 0x134
  nuhspecial_s display_special;    // 0x17c
  char name2[0x10];                // 0x188
  struct GIZMO_s *gizmo;           // 0x198
} GRAPPLE;

typedef struct GRAPPLEPROGRESS_s {
  u32 active[1];  // 0x00
  u32 visible[1]; // 0x04
} GRAPPLEPROGRESS;

typedef struct GIZMO_s {
  void *object;
  u8 pad4[2];
  u8 type_id; // 0x06
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
void Grapples_Update(void *world, void *unused, f32 dt);
void Grapples_Draw(void *world, void *unused, f32 dt);

i32 EdFileReadInt();
int NuSpecialExistsFn(nuhspecial_s *sp);
u16 EdFileReadUnsignedShort();
f32 EdFileReadFloat();
char EdFileReadChar();
u8 EdFileReadUnsignedChar();
void EdFileRead(void *buf, i32 len);
void EdFileReadNuVec(nuvec_s *v);

// FUNCTION: LEGOBATMAN 0x005a8e30
i32 Grapples_Load(void *world_ptr, void *unused) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  char special_name[256];
  if (world->grapple_count == 0) {
    i32 version = EdFileReadInt();
    world->grapple_count = EdFileReadInt();
    for (i32 index = 0; index < world->grapple_count; index++) {
      EdFileRead(world->grapples[index].name, 16);
      EdFileReadNuVec(&world->grapples[index].position);
      if (version >= 2) {
        world->grapples[index].y_rotation = EdFileReadUnsignedShort();
      } else {
        EdFileReadUnsignedShort();
        world->grapples[index].y_rotation = EdFileReadUnsignedShort();
      }
      if (version >= 3)
        world->grapples[index].f4c = EdFileReadFloat();
      else
        world->grapples[index].f4c = 1.0f;
      if (version >= 4) {
        world->grapples[index].disabled = EdFileReadChar() != 0;
        world->grapples[index].rope_length = EdFileReadFloat();
      } else {
        world->grapples[index].disabled = 0;
        world->grapples[index].rope_length = 2.0f;
      }
      if (version >= 5)
        world->grapples[index].x_rotation = EdFileReadUnsignedShort();
      else
        world->grapples[index].x_rotation = 0;
      if (version >= 6)
        world->grapples[index].reversed = EdFileReadChar() != 0;
      else
        world->grapples[index].reversed = 0;
      memset(&world->grapples[index].display_special, 0, sizeof(nuhspecial_s));
      if (version >= 7) {
        i32 name_length = EdFileReadUnsignedChar();
        if (name_length > 0) {
          EdFileRead(special_name, name_length);
          special_name[name_length] = '\0';
          NuSpecialFind(world->scn140, &world->grapples[index].display_special,
                        special_name, 1);
        }
      }
      if (version >= 8)
        world->grapples[index].no_special = EdFileReadChar();
      else if (NuSpecialExistsFn(&world->grapples[index].display_special))
        world->grapples[index].no_special = 0;
      else
        world->grapples[index].no_special = 1;
      if (version >= 9)
        world->grapples[index].b6 = EdFileReadChar();
      else
        world->grapples[index].b6 = 0;
      if (version >= 10)
        EdFileRead(world->grapples[index].name2, 16);
      else
        world->grapples[index].name2[0] = '\0';
      if (version >= 11)
        world->grapples[index].b8 = EdFileReadChar();
      else
        world->grapples[index].b8 = 0x80;
    }
    return 1;
  }
  return 0;
}

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

// FUNCTION: LEGOBATMAN 0x005a91b0
static void Grapple_ResetRopePoints(GRAPPLE *grapple) {
  f32 segment_length = grapple->rope_length / 6.0f;
  for (i32 i = 0; i < 6; i++) {
    grapple->target_rope_points[i].x = grapple->hook_position.x;
    grapple->target_rope_points[i].y =
        grapple->hook_position.y - (segment_length * (f32)i + segment_length);
    grapple->target_rope_points[i].z = grapple->hook_position.z;
  }
}

void NuVecRotateX(nuvec_s *v, nuvec_s *v0, i32 a);
void NuVecRotateY(nuvec_s *v, nuvec_s *v0, i32 a);
void NuVecAdd(nuvec_s *out, nuvec_s *a, nuvec_s *b);
extern "C" void NewTerrPlatformsOff(void);
f32 GameShadow(GameObject_s *object, nuvec_s *position, f32 probe_height,
               i32 flags);
void FindAnglesZX(nuvec_s *normal, u16 *x_rotation, u16 *z_rotation);

// GLOBAL: LEGOBATMAN 0x009f7a44
extern nuvec_s ShadNorm;

// STUB: LEGOBATMAN 0x005a92b0
// position arrives in ebx in ours, on the stack in the original (the open
// int-in-ebx convention problem; every parameter order tried).
static void Grapple_ResetShadow(GRAPPLE *grapple, nuvec_s *offset,
                                nuvec_s *position, i32 has_terrain_platform) {
  if (offset != NULL) {
    NuVecRotateX(&grapple->hook_position, offset, grapple->x_rotation);
    NuVecRotateY(&grapple->hook_position, &grapple->hook_position,
                 grapple->y_rotation);
    NuVecAdd(&grapple->hook_position, &grapple->hook_position, position);
  } else {
    grapple->hook_position = *position;
  }
  grapple->position = *position;
  grapple->shadow_probe_position.x = grapple->hook_position.x;
  grapple->shadow_probe_position.y = grapple->hook_position.y - 0.1f;
  grapple->shadow_probe_position.z = grapple->hook_position.z;
  grapple->ground_position.x = grapple->hook_position.x;
  grapple->ground_position.z = grapple->hook_position.z;
  if (!has_terrain_platform)
    NewTerrPlatformsOff();
  f32 ground = GameShadow(NULL, &grapple->hook_position, 0.0f, -1);
  grapple->ground_position.y = ground;
  if (ground != 2000000.0) {
    grapple->ground_position.y = ground + 0.005f;
    FindAnglesZX(&ShadNorm, &grapple->ground_x_rotation,
                 &grapple->ground_z_rotation);
  }
}

// GLOBAL: LEGOBATMAN 0x00961104
extern i32 blowup_gizmotype_id;

// STUB: LEGOBATMAN 0x005a9390
// call site follows Grapple_ResetShadow's ebx convention; rope clamp also
// differs.
static void Grapple_Reset(GRAPPLE *grapple, nuvec_s *offset) {
  Grapple_ResetShadow(grapple, offset, &grapple->position,
                      grapple->has_terrain_platform);
  if (grapple->ground_position.y != 2000000.0) {
    f32 available =
        grapple->hook_position.y - (grapple->ground_position.y + 0.1f);
    if (grapple->rope_length > available) {
      if (0.7f > available)
        grapple->rope_length = 0.7f;
      else
        grapple->rope_length = available;
    }
  }
  Grapple_ResetRopePoints(grapple);
  for (i32 i = 0; i < 6; i++)
    grapple->rope_points[i].pos = grapple->target_rope_points[i];
  grapple->f58 = 0.0f;
  grapple->active = 1;
  grapple->visible = 1;
  grapple->has_terrain_platform = 0;
  if (grapple->name2[0] != '\0') {
    grapple->gizmo = GizmoFindByName(WorldInfo_CurrentlyActive()->gizmoSys2b0c,
                                     -1, grapple->name2);
    if (grapple->gizmo != NULL &&
        grapple->gizmo->type_id != blowup_gizmotype_id)
      grapple->gizmo = NULL;
  }
}

extern "C" int NuSPrintf(char *buf, char *fmt, ...);

// GLOBAL: LEGOBATMAN 0x0095fc14
extern nuvec_s GrapplePointOffset;
// GLOBAL: LEGOBATMAN 0x00a97e48
extern GRAPPLE DynamicGrapple[4];

// STUB: LEGOBATMAN 0x005a9520
// progress bit insert and the dynamic-grapple loop allocate differently.
void Grapples_Reset(void *world_ptr, void *unused, void *progress_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  GRAPPLEPROGRESS *progress = (GRAPPLEPROGRESS *)progress_ptr;
  if (world == NULL)
    return;
  GRAPPLE *grapple = world->grapples;
  if (grapple != NULL) {
    for (i32 index = 0; index < world->grapple_count; index++, grapple++) {
      Grapple_Reset(&world->grapples[index], &GrapplePointOffset);
      if (progress != NULL && index < 32) {
        i32 word = index / 32;
        u32 bit = 1 << (index & 31);
        grapple->visible = (progress->visible[word] & bit) != 0;
        grapple->active = (progress->active[word] & bit) != 0;
      }
    }
  }
  for (i32 i = 0; i < 4; i++) {
    GRAPPLE *dynamic = &DynamicGrapple[i];
    NuSPrintf(dynamic->name, "dynamic%i", i + 1);
    dynamic->active = 1;
    dynamic->visible = 0;
    dynamic->y_rotation = 0;
    dynamic->attached_object = NULL;
    dynamic->has_terrain_platform = 1;
  }
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
