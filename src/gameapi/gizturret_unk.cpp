// gameapi/gizturret_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nu2api/nucore/common.h"
#include "../nu2api/nucore/nustring.h"
#include <stddef.h>

typedef struct GIZTURRET_s {
  u32 pad0[2];
  char name[0x13c - 8]; // 0x08
} GIZTURRET_s;

typedef struct GIZTURRETSYS_s {
  GIZTURRET_s *turrets; // 0x00
  u32 pad4;
  u16 count; // 0x08
} GIZTURRETSYS_s;

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
