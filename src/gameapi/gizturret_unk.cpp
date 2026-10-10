// gameapi/gizturret_unk.cpp: placed by tools/new.py; file name unproven.

#include "../batman/leveldata_unk.h"
#include "../batman/worldinfo_unk.h"
#include "../nu2api/nucore/common.h"
#include "../nu2api/nucore/nustring.h"
#include <stddef.h>
#include <string.h>

typedef struct GIZTURRET_s {
  u32 pad0[2];
  char name[0x10]; // 0x08
  void *parts;     // 0x18
  u8 pad1c[0x24 - 0x1c];
  nuvec_s position; // 0x24
  u8 pad30[0x124 - 0x30];
  i16 s124;   // 0x124, resolved in PostLoad when flags bit 8 is set
  i16 sfx126; // 0x126
  u8 pad128[0x12a - 0x128];
  i16 sfx12a; // 0x12a
  u8 pad12c[0x12e - 0x12c];
  u8 b12e; // 0x12e
  u8 b12f; // 0x12f
  u8 pad130;
  u8 shots; // 0x131
  u8 fired; // 0x132
  u8 pad133[0x138 - 0x133];
  i16 sfx138; // 0x138
  u16 b0 : 1;
  u16 active : 1;  // 0x13a bit 1
  u16 visible : 1; // 0x13a bit 2
  u16 b3 : 1;
  u16 b4 : 1;
  u16 b5 : 1;
  u16 b6 : 1;
  u16 b7 : 1;
  u16 b8 : 1;
} GIZTURRET_s;

typedef struct GIZTURRETSYS_s {
  GIZTURRET_s *turrets; // 0x00
  void *pool;           // 0x04
  u16 count;            // 0x08
  u16 max;              // 0x0a
} GIZTURRETSYS_s;

typedef struct GIZTURRETPROGRESS_s {
  u32 visible[2]; // 0x00
  u32 active[2];  // 0x08
  u32 a10[2];     // 0x10
  u32 a18[2];     // 0x18
  u32 a20[2];     // 0x20
  u32 a28[2];     // 0x28
  u8 shots[0x40]; // 0x30
} GIZTURRETPROGRESS;

typedef struct GIZMO_s {
  void *object;
} GIZMO;

void AddGizmo(GIZMOSYS_s *gizmo_sys, i32 type_id, void *a, void *object);
void *GameBufferAllocProgressUnk005bbaf0(VARIPTR *buf, VARIPTR *buf_end,
                                         i32 size);
void Unk00604f10(void *parts, i32 visible);
i16 Unk005de820(void *world, i32 id);
extern "C" int sprintf(char *buf, const char *fmt, ...);

// GLOBAL: LEGOBATMAN 0x00ad1fec
extern char GizTurret_OutputNameBuf[];

// FUNCTION: LEGOBATMAN 0x0065ff20
void GizTurrets_PostLoad(void *world, void *sys_ptr) {
  GIZTURRETSYS_s *sys = (GIZTURRETSYS_s *)sys_ptr;
  if (sys == NULL)
    return;
  GIZTURRET_s *turret = sys->turrets;
  for (i32 i = 0; i < sys->count; i++, turret++) {
    if (turret->b8) {
      turret->s124 = Unk005de820(world, turret->s124);
      turret->b8 = 0;
    }
  }
}

// FUNCTION: LEGOBATMAN 0x006600f0
void GizTurret_Activate(GIZMO *gizmo, i32 active) {
  if (gizmo != NULL) {
    GIZTURRET_s *turret = (GIZTURRET_s *)gizmo->object;
    turret->active = active != 0;
    if (turret->active) {
      turret->b4 = 0;
      turret->b5 = 0;
      turret->fired = 0;
      turret->b12e = turret->b12f;
    }
  }
}

// FUNCTION: LEGOBATMAN 0x00660150
i32 GizTurret_ActivateRev(GIZMO *gizmo, i32 value, i32 query) {
  if (gizmo != NULL && gizmo->object != NULL) {
    GIZTURRET_s *turret = (GIZTURRET_s *)gizmo->object;
    if (query & 1) {
      if (turret->active)
        return value;
      turret->b4 = 0;
      return value == 0;
    }
    turret->active = value == 0;
    return 1;
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x006601c0
static void GizTurret_SetVisible(GIZTURRET_s *turret, i32 visible) {
  if (turret != NULL) {
    Unk00604f10(turret->parts, visible);
    turret->visible = visible != 0;
  }
}

// FUNCTION: LEGOBATMAN 0x006601f0
void GizTurret_SetVisibility(GIZMO *gizmo, i32 visible) {
  if (gizmo != NULL)
    GizTurret_SetVisible((GIZTURRET_s *)gizmo->object, visible);
}

// FUNCTION: LEGOBATMAN 0x00660230
nuvec_s *GizTurret_GetPos(GIZMO *gizmo) {
  if (gizmo != NULL && gizmo->object != NULL)
    return &((GIZTURRET_s *)gizmo->object)->position;
  return NULL;
}

// FUNCTION: LEGOBATMAN 0x00661a70
i32 GizTurrets_GetMaxGizmos(void *world_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  if (world != NULL)
    return world->current_level->max_turrets;
  return 0;
}

// FUNCTION: LEGOBATMAN 0x00661a90
void GizTurrets_AddGizmos(GIZMOSYS_s *gizmo_sys, i32 type_id, void *world,
                          void *sys_ptr) {
  GIZTURRETSYS_s *sys = (GIZTURRETSYS_s *)sys_ptr;
  if (sys == NULL)
    return;
  for (i32 i = 0; i < sys->count; i++) {
    if (NuStrLen(sys->turrets[i].name) != 0)
      AddGizmo(gizmo_sys, type_id, NULL, &sys->turrets[i]);
  }
}

// FUNCTION: LEGOBATMAN 0x00661af0
char *GizTurret_GetGizmoName(GIZMO *gizmo) {
  return gizmo != NULL ? ((GIZTURRET_s *)gizmo->object)->name : NULL;
}

// FUNCTION: LEGOBATMAN 0x00661b10
i32 GizTurret_GetOutput(GIZMO *gizmo, i32 output, i32 b) {
  GIZTURRET_s *turret = (GIZTURRET_s *)gizmo->object;
  switch (output) {
  case 0:
    if (turret->b4 || turret->b5)
      return 1;
    break;
  case 1:
    if (turret->b7)
      return 1;
    break;
  case 2:
    if (turret->fired >= turret->shots)
      return 1;
    break;
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x00661b60
char *GizTurret_GetOutputName(GIZMO *gizmo, i32 output) {
  switch (output) {
  case 0:
    return "destroyed";
  case 1:
    return "fired";
  case 2:
    if (gizmo->object != NULL) {
      sprintf(GizTurret_OutputNameBuf, "Fired %d Shots",
              ((GIZTURRET_s *)gizmo->object)->shots);
      return GizTurret_OutputNameBuf;
    }
    break;
  }
  return NULL;
}

// FUNCTION: LEGOBATMAN 0x00661bb0
i32 GizTurret_GetNumOutputs(GIZMO *gizmo) { return 3; }

// FUNCTION: LEGOBATMAN 0x00661bc0
void *GizTurrets_AllocateProgressData(VARIPTR *buf, VARIPTR *buf_end) {
  return GameBufferAllocProgressUnk005bbaf0(buf, buf_end,
                                            sizeof(GIZTURRETPROGRESS));
}

// FUNCTION: LEGOBATMAN 0x00661be0
void GizTurrets_ClearProgress(void *world, void *progress_ptr) {
  GIZTURRETPROGRESS *progress = (GIZTURRETPROGRESS *)progress_ptr;
  if (progress != NULL) {
    memset(progress->visible, 0xff, sizeof(progress->visible));
    memset(progress->active, 0xff, sizeof(progress->active));
    memset(progress->a10, 0, sizeof(progress->a10));
    memset(progress->a18, 0, sizeof(progress->a18));
    memset(progress->a20, 0, sizeof(progress->a20));
    memset(progress->a28, 0, sizeof(progress->a28));
    memset(progress->shots, 0xff, sizeof(progress->shots));
  }
}

// FUNCTION: LEGOBATMAN 0x00661c30
GIZTURRET_s *GizTurret_FindByName(GIZTURRETSYS_s *system, char *name) {
  GIZTURRET_s *turret = 0;
  if (system != 0 && name != 0) {
    turret = system->turrets;
    for (i32 i = 0; i < system->count; ++i, ++turret) {
      if (NuStrICmp(turret->name, name) == 0) {
        return turret;
      }
    }
  }
  return turret;
}

#include "../nu2api/nucore/nulist.h"
#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x0065f9d0
static f32 NuSinApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x0065fa70
static f32 NuCosApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x0065faa0
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

struct GIZAIMESSAGE_s {
  NULISTLNK links;     // 0x00
  char name[0x20];     // 0x08
  f32 value;           // 0x28
  i8 output_values[8]; // 0x2c
  i8 output_count;     // 0x34
  u8 field_0x35;
  u16 flags; // 0x36, bit 0 add as gizmo, bit 1 gizmo added
};

struct GIZAIMESSAGESYS_s {
  i32 count;                // 0x00
  GIZAIMESSAGE_s *messages; // 0x04
  NULISTHDR free_list;      // 0x08
  NULISTHDR active_list;    // 0x10
};

// GLOBAL: LEGOBATMAN 0x00967b98
extern char gizaimessage_prefix[];

extern "C" void NuLinkedListRemove(NULISTHDR *list, NULISTLNK *node);
extern "C" void NuLinkedListAppend(NULISTHDR *list, NULISTLNK *node);
i32 NuStrNICmp(const char *a, const char *b, i32 n);
i32 NuStrNCpy(char *dst, const char *src, i32 n);
int sprintf(char *buf, const char *fmt, ...);

// from saga legoapi/gizmo/base/gizmessage.cpp
// STUB: LEGOBATMAN 0x006633c0
// body right; orig gives the "return out" path its own /GS epilogue (with a
// dead "mov esi, eax"), ours tail-merges it into the main one.
GIZAIMESSAGE_s *CheckGizAIMessage(GIZAIMESSAGESYS_s *sys, char const *name,
                                  GIZAIMESSAGE_s *out) {
  GIZAIMESSAGE_s *msg = 0;
  char local[0x20];
  if (sys != 0) {
    msg = out;
    if (out != 0)
      return out;
    if (name != 0) {
      if (NuStrIStr((char *)name, gizaimessage_prefix) != 0) {
        sprintf(local, "%s", name);
      } else {
        if (NuStrLen(name) + NuStrLen(gizaimessage_prefix) + 1 >= 0x20)
          return 0;
        sprintf(local, "%s%s", gizaimessage_prefix, name);
      }
      for (msg = (GIZAIMESSAGE_s *)NuListGetHead(&sys->active_list); msg != 0;
           msg = (GIZAIMESSAGE_s *)NuListGetNext(&sys->active_list,
                                                 &msg->links)) {
        if (NuStrNICmp(local, msg->name, 0x20) == 0)
          break;
      }
      if (msg != 0)
        return msg;
      msg = (GIZAIMESSAGE_s *)NuListGetHead(&sys->free_list);
      if (msg != 0) {
        NuLinkedListRemove(&sys->free_list, &msg->links);
        NuLinkedListAppend(&sys->active_list, &msg->links);
        NuStrNCpy(msg->name, local, 0x20);
      }
    }
  }
  return msg;
}

typedef struct GIZMOSYS_s GIZMOSYS;

typedef struct ADDGIZMOTYPE_s {
  char *name;        // 0x00
  char *prefix;      // 0x04
  u16 progress_size; // 0x08
  void *fns[0x1c];   // 0x0c
} ADDGIZMOTYPE;

// GLOBAL: LEGOBATMAN 0x00960118
extern ADDGIZMOTYPE Default_ADDGIZMOTYPE;
// GLOBAL: LEGOBATMAN 0x00ad210c
extern GIZAIMESSAGESYS_s *gizaimessagesys;
// GLOBAL: LEGOBATMAN 0x00967b94
i32 gizaimessage_gizmotype_id = -1;

void AddGizmo(GIZMOSYS *gizmo_sys, i32 type_id, void *a, void *object);

// FUNCTION: LEGOBATMAN 0x00663540
i32 GizAIMessage_GetMaxGizmos(void *world) { return 0x40; }

// FUNCTION: LEGOBATMAN 0x00663550
void GizAIMessage_AddGizmos(GIZMOSYS *gizmo_sys, i32 type_id, void *world,
                            void *unused) {
  if (gizaimessagesys == NULL)
    return;
  GIZAIMESSAGE_s *message =
      (GIZAIMESSAGE_s *)NuListGetHead(&gizaimessagesys->active_list);
  while (message != NULL) {
    if ((message->flags & 1) != 0) {
      AddGizmo(gizmo_sys, gizaimessage_gizmotype_id, NULL, message);
      message->flags |= 2;
    }
    message = (GIZAIMESSAGE_s *)NuListGetNext(&gizaimessagesys->active_list,
                                              &message->links);
  }
}

// FUNCTION: LEGOBATMAN 0x006635b0
char *GizAIMessage_GetName(GIZAIMESSAGE_s *message) {
  return message != NULL ? message->name : NULL;
}

// FUNCTION: LEGOBATMAN 0x006635c0
char *GizAIMessage_GetGizmoName(GIZMO *gizmo) {
  if (gizmo == NULL || gizmo->object == NULL)
    return NULL;
  return ((GIZAIMESSAGE_s *)gizmo->object)->name;
}

// FUNCTION: LEGOBATMAN 0x006635e0
i32 GizAIMessage_GetOutput(GIZMO *gizmo, i32 output_index, i32 b) {
  if (gizmo == NULL || gizmo->object == NULL || (u32)output_index > 7)
    return 0;
  GIZAIMESSAGE_s *message = (GIZAIMESSAGE_s *)gizmo->object;
  return message->value == (f32)message->output_values[output_index];
}

// FUNCTION: LEGOBATMAN 0x00663620
char *GizAIMessage_GetOutputName(GIZMO *gizmo, i32 output_index) {
  // GLOBAL: LEGOBATMAN 0x00ad2114
  static char returnstr[4];
  if (gizmo == NULL || gizmo->object == NULL || (u32)output_index > 7)
    return NULL;
  GIZAIMESSAGE_s *message = (GIZAIMESSAGE_s *)gizmo->object;
  sprintf(returnstr, "%d", (i32)message->output_values[output_index]);
  return returnstr;
}

// FUNCTION: LEGOBATMAN 0x00663660
i32 GizAIMessage_GetNumOutputs(GIZMO *gizmo) {
  if (gizmo == NULL || gizmo->object == NULL)
    return 0;
  return ((GIZAIMESSAGE_s *)gizmo->object)->output_count;
}

// FUNCTION: LEGOBATMAN 0x00663680
ADDGIZMOTYPE *GizAIMessage_RegisterGizmo(i32 type_id) {
  // GLOBAL: LEGOBATMAN 0x00ad2118
  static ADDGIZMOTYPE addtype;

  addtype = Default_ADDGIZMOTYPE;
  addtype.name = "Message";
  addtype.prefix = gizaimessage_prefix;
  addtype.progress_size = 0;
  addtype.fns[0] = (void *)GizAIMessage_GetMaxGizmos;
  addtype.fns[1] = (void *)GizAIMessage_AddGizmos;
  addtype.fns[2] = NULL;
  addtype.fns[3] = NULL;
  addtype.fns[4] = NULL;
  addtype.fns[5] = NULL;
  addtype.fns[6] = (void *)GizAIMessage_GetGizmoName;
  addtype.fns[7] = (void *)GizAIMessage_GetOutput;
  addtype.fns[8] = (void *)GizAIMessage_GetOutputName;
  addtype.fns[9] = (void *)GizAIMessage_GetNumOutputs;
  addtype.fns[10] = NULL;
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
  addtype.fns[23] = NULL;
  addtype.fns[24] = NULL;
  addtype.fns[25] = NULL;
  addtype.fns[26] = NULL;
  addtype.fns[27] = NULL;
  gizaimessage_gizmotype_id = type_id;
  return &addtype;
}

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_gizturret_unk(f32 *v, f32 a, i32 i) {
  v[0] = NuSinApprox(i);
  v[1] = NuCosApprox(i);
  NuVec4Set(v, a, a, a, a);
}

void GizTurrets_LateUpdate(void *world, void *sys, f32 dt);
void GizTurrets_Draw(void *world, void *sys, f32 dt);
void GizTurrets_BoltHitPlat(void);
void GizTurrets_GetBestBoltTarget(void);
void GizTurrets_BoltHit(void);
void GizTurrets_StoreProgress(void *world, void *sys, void *progress);
void GizTurrets_Reset(void *world, void *sys, void *progress);
void *GizTurrets_ReserveBufferSpace(void *world);
i32 GizTurrets_Load(void *world, void *sys);
void GizTurrets_AddLevelSfx(void *world, void *sys, i32 *sfx_ids,
                            i32 *sfx_count, i32 max_sfx);

// GLOBAL: LEGOBATMAN 0x00967a14
i32 turret_gizmotype_id = -1;

// FUNCTION: LEGOBATMAN 0x006628b0
ADDGIZMOTYPE *GizTurrets_RegisterGizmo(i32 type_id) {
  // GLOBAL: LEGOBATMAN 0x00967ad4
  static char *name = "GizTurret";
  // GLOBAL: LEGOBATMAN 0x00ad2010
  static ADDGIZMOTYPE addtype;

  addtype = Default_ADDGIZMOTYPE;
  addtype.name = name;
  addtype.prefix = "";
  addtype.progress_size = sizeof(GIZTURRETPROGRESS);
  addtype.fns[0] = (void *)GizTurrets_GetMaxGizmos;
  addtype.fns[1] = (void *)GizTurrets_AddGizmos;
  addtype.fns[2] = NULL;
  addtype.fns[3] = (void *)GizTurrets_LateUpdate;
  addtype.fns[4] = (void *)GizTurrets_Draw;
  addtype.fns[5] = NULL;
  addtype.fns[6] = (void *)GizTurret_GetGizmoName;
  addtype.fns[7] = (void *)GizTurret_GetOutput;
  addtype.fns[8] = (void *)GizTurret_GetOutputName;
  addtype.fns[9] = (void *)GizTurret_GetNumOutputs;
  addtype.fns[10] = (void *)GizTurret_Activate;
  addtype.fns[11] = (void *)GizTurret_ActivateRev;
  addtype.fns[12] = (void *)GizTurret_SetVisibility;
  addtype.fns[13] = NULL;
  addtype.fns[14] = (void *)GizTurret_GetPos;
  addtype.fns[15] = NULL;
  addtype.fns[16] = (void *)GizTurrets_BoltHitPlat;
  addtype.fns[17] = (void *)GizTurrets_GetBestBoltTarget;
  addtype.fns[18] = (void *)GizTurrets_BoltHit;
  addtype.fns[19] = (void *)GizTurrets_AllocateProgressData;
  addtype.fns[20] = (void *)GizTurrets_ClearProgress;
  addtype.fns[21] = (void *)GizTurrets_StoreProgress;
  addtype.fns[22] = (void *)GizTurrets_Reset;
  addtype.fns[23] = (void *)GizTurrets_ReserveBufferSpace;
  addtype.fns[24] = (void *)GizTurrets_Load;
  addtype.fns[25] = NULL;
  addtype.fns[26] = (void *)GizTurrets_PostLoad;
  addtype.fns[27] = (void *)GizTurrets_AddLevelSfx;
  turret_gizmotype_id = type_id;
  return &addtype;
}
