// gameapi/gizturret_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nu2api/nucore/common.h"
#include "../nu2api/nucore/nustring.h"

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
  u8 flags; // 0x36
  u8 field_0x37;
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

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_gizturret_unk(f32 *v, f32 a, i32 i) {
  v[0] = NuSinApprox(i);
  v[1] = NuCosApprox(i);
  NuVec4Set(v, a, a, a, a);
}
