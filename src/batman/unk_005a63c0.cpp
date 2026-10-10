
// The rest of this TU is the Mac's Techno file (.. Technos_RegisterGizmo
// 0x5a82d0); callbacks named by their RegisterGizmo slot.

#include "leveldata_unk.h"
#include "worldinfo_unk.h"
#include <stddef.h>

typedef struct TECHNO_s {
  u8 pad0[0x40];
  char name[0x10];  // 0x40
  nuvec_s position; // 0x50
  u8 pad5c[0x6c - 0x5c];
  f32 f6c; // 0x6c
  u8 pad70[0x78 - 0x70];
  struct TECHNOTYPE_s *type; // 0x78
  i16 s7c;                   // 0x7c
  u8 pad7e[0x82 - 0x7e];
  u8 b82;  // 0x82
  u8 b83;  // 0x83
  i32 i84; // 0x84
  u8 pad88[0x8b - 0x88];
  u8 active : 1;  // 0x8b
  u8 visible : 1; // 0x8b bit 1
  u8 b2 : 1;
  u8 b3 : 1;
  u8 done : 1;       // 0x8b bit 4
  char name8c[0x10]; // 0x8c
  char name9c[0x28]; // 0x9c
  f32 fc4;           // 0xc4
  u8 padc8[0xcc - 0xc8];
} TECHNO;

typedef struct TECHNOPROGRESS_s {
  u32 active[1];  // 0x00
  u32 visible[1]; // 0x04
  u32 done[1];    // 0x08
} TECHNOPROGRESS;

typedef struct GIZMO_s {
  void *object;
} GIZMO;

i32 NuStrLen(const char *s);
void AddGizmo(GIZMOSYS_s *gizmo_sys, i32 type_id, void *a, void *object);
void *GameBufferAllocProgressUnk005bbaf0(VARIPTR *buf, VARIPTR *buf_end,
                                         i32 size);

// GLOBAL: LEGOBATMAN 0x0095fb50
static char *Techno_OutputNames[3] = {"Active", "Special Button Down",
                                      "Got Handle"};

typedef struct TECHNOTYPE_s {
  char *name; // 0x00
  u8 pad4[0x14 - 4];
  u8 b14; // 0x14
  u8 pad15[0xa0 - 0x15];
} TECHNOTYPE;

// GLOBAL: LEGOBATMAN 0x0095fb48
extern TECHNOTYPE **g_unk0095fb48;

i32 EdFileReadInt();
char EdFileReadChar();
i16 EdFileReadShort();
f32 EdFileReadFloat();
void EdFileRead(void *buf, i32 len);
void EdFileReadNuVec(nuvec_s *v);
i32 GizmoFileReadName(char *name);
int NuStrICmp(const char *a, const char *b);
int NuStrCpy(char *dst, const char *src);

// FUNCTION: LEGOBATMAN 0x005a6ae0
static TECHNOTYPE *Techno_FindTypeUnk005a6ae0(char *name) {
  if (name != NULL && *g_unk0095fb48 != NULL) {
    for (TECHNOTYPE *type = *g_unk0095fb48; type != NULL && type->name != NULL;
         type++) {
      if (!NuStrICmp(name, type->name))
        return type;
    }
  }
  return NULL;
}

// FUNCTION: LEGOBATMAN 0x005a6b20
i32 Technos_Load(void *world_ptr, void *unused) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  char name[0x40];
  if (world->techno_count == 0) {
    i32 version = EdFileReadInt();
    world->techno_count = EdFileReadInt();
    for (i32 i = 0; i < world->techno_count; i++) {
      EdFileRead(world->technos[i].name, 0x10);
      EdFileReadNuVec(&world->technos[i].position);
      world->technos[i].s7c = EdFileReadShort();
      if (version >= 8 && GizmoFileReadName(name))
        world->technos[i].type = Techno_FindTypeUnk005a6ae0(name);
      if (world->technos[i].type == NULL)
        world->technos[i].type = *g_unk0095fb48;
      if (version >= 2) {
        world->technos[i].b83 = EdFileReadChar();
        EdFileRead(world->technos[i].name9c, EdFileReadInt());
        NuStrCpy(world->technos[i].name8c, world->technos[i].name9c);
        if (version >= 3) {
          world->technos[i].b82 = EdFileReadChar();
          if (version < 6) {
            u8 *p = &world->technos[i].b82;
            if (*p & 1)
              *p = 1;
            else if (*p & 2)
              *p = 2;
            else if (*p & 4)
              *p = 3;
            else if (*p & 8)
              *p = 4;
          }
        } else {
          world->technos[i].b82 = 1;
        }
      }
      if (version >= 4)
        world->technos[i].f6c = EdFileReadFloat();
      else
        world->technos[i].f6c = 1.0f;
      if (version >= 5)
        world->technos[i].i84 = EdFileReadInt();
      else
        world->technos[i].i84 = 0;
      if (version >= 7)
        world->technos[i].fc4 = EdFileReadFloat();
      else
        world->technos[i].fc4 = 100.0f;
      if (world->technos[i].type->b14 != 0xff)
        world->technos[i].b82 = world->technos[i].type->b14;
    }
    return 1;
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x005a6fe0
void Techno_Activate(GIZMO *gizmo, i32 active) {
  if (gizmo != NULL)
    ((TECHNO *)gizmo->object)->active = active != 0;
}

// FUNCTION: LEGOBATMAN 0x005a7710
i32 Technos_GetMaxGizmos(void *world_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  if (world != NULL)
    return world->current_level->max_technos;
  return 0;
}

// FUNCTION: LEGOBATMAN 0x005a7730
void Technos_AddGizmos(GIZMOSYS_s *gizmo_sys, i32 type_id, void *world_info,
                       void *unused) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_info;
  for (i32 i = 0; i < world->techno_count; i++) {
    if (NuStrLen(world->technos[i].name) != 0)
      AddGizmo(gizmo_sys, type_id, NULL, &world->technos[i]);
  }
}

// FUNCTION: LEGOBATMAN 0x005a77a0
char *Techno_GetGizmoName(GIZMO *gizmo) {
  return gizmo != NULL ? ((TECHNO *)gizmo->object)->name : NULL;
}

// FUNCTION: LEGOBATMAN 0x005a77c0
i32 Techno_GetNumOutputs(GIZMO *gizmo) { return 3; }

// FUNCTION: LEGOBATMAN 0x005a77d0
char *Techno_GetOutputName(GIZMO *gizmo, i32 output_index) {
  if ((u32)output_index <= 2)
    return Techno_OutputNames[output_index];
  return "Unknown";
}

// FUNCTION: LEGOBATMAN 0x005a77f0
void *Technos_AllocateProgressData(VARIPTR *buf, VARIPTR *buf_end) {
  return GameBufferAllocProgressUnk005bbaf0(buf, buf_end,
                                            sizeof(TECHNOPROGRESS));
}

// FUNCTION: LEGOBATMAN 0x005a7810
void Technos_ClearProgress(void *world, void *progress_ptr) {
  TECHNOPROGRESS *progress = (TECHNOPROGRESS *)progress_ptr;
  if (progress != NULL) {
    progress->active[0] = 0xffffffff;
    progress->visible[0] = 0xffffffff;
    progress->done[0] = 0;
  }
}

// FUNCTION: LEGOBATMAN 0x005a7830
void Technos_StoreProgress(void *world_ptr, void *unused, void *progress_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  TECHNOPROGRESS *progress = (TECHNOPROGRESS *)progress_ptr;
  if (progress == NULL)
    return;
  Technos_ClearProgress(NULL, progress);
  if (world != NULL && world->technos != NULL) {
    TECHNO *techno = world->technos;
    for (i32 i = 0; i < world->techno_count; i++, techno++) {
      if (i >= 32)
        break;
      i32 word = i / 32;
      u32 bit = 1 << (i & 31);
      if (!techno->visible)
        progress->visible[word] &= ~bit;
      if (!techno->active)
        progress->active[word] &= ~bit;
      if (techno->done)
        progress->done[word] |= bit;
    }
  }
}

// FUNCTION: LEGOBATMAN 0x005a78d0
nuvec_s *Techno_GetPos(GIZMO *gizmo) {
  if (gizmo != NULL && gizmo->object != NULL)
    return &((TECHNO *)gizmo->object)->position;
  return NULL;
}

typedef struct ADDGIZMOTYPE_s {
  char *name;        // 0x00
  char *prefix;      // 0x04
  u16 progress_size; // 0x08
  void *fns[0x1c];   // 0x0c
} ADDGIZMOTYPE;

// GLOBAL: LEGOBATMAN 0x00960118
extern ADDGIZMOTYPE Default_ADDGIZMOTYPE;

void Technos_EarlyUpdate(void *world, void *unused, f32 dt);
void Technos_LateUpdate(void *world, void *unused, f32 dt);
void Technos_Draw(void *world, void *unused, f32 dt);
i32 Techno_GetOutput(GIZMO *gizmo, i32 output, i32 b);
void Techno_SetVisibility(GIZMO *gizmo, i32 visible);
void Technos_Reset(void *world, void *unused, void *progress);
void *Technos_ReserveBufferSpace(void *world);
i32 Technos_Load(void *world, void *unused);

// GLOBAL: LEGOBATMAN 0x0095fb5c
i32 techno_gizmotype_id = -1;

// FUNCTION: LEGOBATMAN 0x005a82d0
ADDGIZMOTYPE *Technos_RegisterGizmo(i32 type_id) {
  // GLOBAL: LEGOBATMAN 0x0095fc00
  static char *name = "Techno";
  // GLOBAL: LEGOBATMAN 0x00a97dc8
  static ADDGIZMOTYPE addtype;

  addtype = Default_ADDGIZMOTYPE;
  addtype.name = name;
  addtype.prefix = "";
  addtype.progress_size = sizeof(TECHNOPROGRESS);
  addtype.fns[0] = (void *)Technos_GetMaxGizmos;
  addtype.fns[1] = (void *)Technos_AddGizmos;
  addtype.fns[2] = (void *)Technos_EarlyUpdate;
  addtype.fns[3] = (void *)Technos_LateUpdate;
  addtype.fns[4] = (void *)Technos_Draw;
  addtype.fns[5] = NULL;
  addtype.fns[6] = (void *)Techno_GetGizmoName;
  addtype.fns[7] = (void *)Techno_GetOutput;
  addtype.fns[8] = (void *)Techno_GetOutputName;
  addtype.fns[9] = (void *)Techno_GetNumOutputs;
  addtype.fns[10] = (void *)Techno_Activate;
  addtype.fns[11] = NULL;
  addtype.fns[12] = (void *)Techno_SetVisibility;
  addtype.fns[13] = NULL;
  addtype.fns[14] = (void *)Techno_GetPos;
  addtype.fns[15] = NULL;
  addtype.fns[16] = NULL;
  addtype.fns[17] = NULL;
  addtype.fns[18] = NULL;
  addtype.fns[19] = (void *)Technos_AllocateProgressData;
  addtype.fns[20] = (void *)Technos_ClearProgress;
  addtype.fns[21] = (void *)Technos_StoreProgress;
  addtype.fns[22] = (void *)Technos_Reset;
  addtype.fns[23] = (void *)Technos_ReserveBufferSpace;
  addtype.fns[24] = (void *)Technos_Load;
  addtype.fns[25] = NULL;
  addtype.fns[26] = NULL;
  addtype.fns[27] = NULL;
  techno_gizmotype_id = type_id;
  return &addtype;
}
