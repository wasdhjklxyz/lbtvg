// gameapi/gamemessages_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nu2api/nucore/common.h"
#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"
#include "../nu2api/numath/nuvec.h"
#include <stdio.h>
#include <string.h>

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x005d30c0
static f32 NuFabs(f32 f);
// FUNCTION: LEGOBATMAN 0x005d30e0
static f32 NuFdiv(f32 a, f32 b);
// FUNCTION: LEGOBATMAN 0x005d3120
static f32 NuSinApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x005d31c0
static f32 NuCosApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x005d31d0
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

typedef struct ADDGAMEMSG_s {
  char *text;               // 0x00
  nuvec_s *position;        // 0x04
  nuvec_s *target_position; // 0x08
  f32 scale;                // 0x0c
  f32 target_scale;         // 0x10
  u8 red;                   // 0x14
  u8 green;                 // 0x15
  u8 blue;                  // 0x16
  u8 pad17;
  u32 flags;    // 0x18
  f32 duration; // 0x1c
  u32 pad20[(0x50 - 0x20) / 4];
} ADDGAMEMSG;

// GLOBAL: LEGOBATMAN 0x00960c98
extern ADDGAMEMSG AddGameMsg_Default;

void *AddGameMsg(ADDGAMEMSG *message);

// FUNCTION: LEGOBATMAN 0x005d3640
void *AddGameMessage(char *text, nuvec_s *position, float scale,
                     nuvec_s *target_position, float target_scale,
                     unsigned char red, unsigned char green, unsigned char blue,
                     u32 flags, float duration) {
  ADDGAMEMSG message = AddGameMsg_Default;
  message.text = text;
  message.position = position;
  message.target_position = target_position;
  message.scale = scale;
  message.target_scale = target_scale;
  message.red = red;
  message.green = green;
  message.blue = blue;
  message.flags = flags;
  message.duration = duration;
  return AddGameMsg(&message);
}

struct GameObject_s;

typedef struct DETONATOR_s {
  nuvec_s pos; // 0x00
  u32 padc[(0x24 - 0xc) / 4];
  GameObject_s *owner; // 0x24
  u8 pad28;
  u8 flags; // 0x29
  u8 pad2a[0x34 - 0x2a];
  GameObject_s *character; // 0x34
  u32 pad38[(0x60 - 0x38) / 4];
} DETONATOR_s;

typedef struct DETONATORSYS_s {
  DETONATOR_s *detonators; // 0x00
  struct {
    u32 pad0[3];
    i16 light; // 0x0c
    u16 pad0e[(0x40 - 0x0e) / 2];
  } bombs[3];    // 0x04, 0x40 each (GetBombLight shl 6, bound 0xc5)
  u8 count;      // 0xc4
  u8 bomb_count; // 0xc5
  u8 padc6[2];
  i16 param_c8;                             // 0xc8
  i16 param_ca;                             // 0xca
  i16 param_cc;                             // 0xcc
  i16 param_ce;                             // 0xce
  void (*callback)(DETONATOR_s *, float &); // 0xd0
} DETONATORSYS_s;

// GLOBAL: LEGOBATMAN 0x00ac6f48
extern DETONATORSYS_s *Detonator;

// GLOBAL: LEGOBATMAN 0x0095fd2c
extern nuvec_s v001;

// 0x118 bytes per message; only the fields FindGameMsgs reads are declared.
typedef struct GAMEMESSAGE_s {
  u8 pad000[0xf4];
  u8 active; // 0xf4
  u8 padf5[0xf9 - 0xf5];
  i8 player; // 0xf9
  i8 type;   // 0xfa
  u8 padfb[0x118 - 0xfb];
} GAMEMESSAGE_s;

// GLOBAL: LEGOBATMAN 0x00abe2f8
extern GAMEMESSAGE_s GameMsgs[128];

// STUB: LEGOBATMAN 0x005d3f60
// close: orig bases the pointer walk at &message->type (0xfa), ours at
// &message->active (0xf4); index form unrolls instead.
int FindGameMsgs(int type, int unused, int clear, int player,
                 GAMEMESSAGE_s *exclude, GAMEMESSAGE_s **out) {
  int count = 0;
  GAMEMESSAGE_s *message = GameMsgs;
  for (int i = 0; i < 128; i++, message++) {
    if (message->active != 0 && message->type == type && message != exclude &&
        (player == -1 || message->player == player)) {
      if (out != 0)
        out[count] = message;
      count++;
      if (clear != 0)
        message->active = 0;
    }
  }
  return count;
}

// FUNCTION: LEGOBATMAN 0x005d3fd0
void AddGameMsgCount(nuvec_s *position, i32 count, i32 total, unsigned char red,
                     unsigned char green, unsigned char blue,
                     float field_0xd4) {
  char text[32];
  sprintf(text, "%i/%i", count, total);

  void *message = position != 0
                      ? AddGameMessage(text, position, 0.6f, 0, 0.8f, red,
                                       green, blue, 0x4023, 1.0f)
                      : AddGameMessage(text, &v001, 0.6f, 0, 0.8f, red, green,
                                       blue, 0x4020, 1.0f);
  if (message != 0) {
    *(float *)((char *)message + 0xd4) = field_0xd4;
    *(float *)((char *)message + 0xd0) = 0.1f;
  }
}

// FUNCTION: LEGOBATMAN 0x005d5670
void DetonatorSys_RegisterCallbacks(void (*callback)(DETONATOR_s *, float &)) {
  if (Detonator != 0)
    Detonator->callback = callback;
}

// FUNCTION: LEGOBATMAN 0x005d5890
void DetonatorSys_Init(int count, int a, int b, int c, int d, variptr_u *buf,
                       variptr_u end) {
  if (count < 1)
    return;
  buf->addr = (buf->addr + 3) & ~3;
  Detonator = (DETONATORSYS_s *)buf->addr;
  buf->addr = (buf->addr + sizeof(DETONATORSYS_s) + 3) & ~3;
  Detonator->detonators = (DETONATOR_s *)buf->addr;
  Detonator->count = count;
  Detonator->param_c8 = a;
  Detonator->param_ca = b;
  Detonator->param_cc = c;
  Detonator->param_ce = d;
  buf->addr += count * sizeof(DETONATOR_s);
}

// FUNCTION: LEGOBATMAN 0x005d5930
DETONATOR_s *DetonatorSys_GetList(int *count) {
  if (Detonator != 0) {
    if (count != 0)
      *count = Detonator->count;
    return Detonator->detonators;
  }
  if (count != 0)
    *count = 0;
  return 0;
}

// FUNCTION: LEGOBATMAN 0x005d5970
i16 Detonators_GetBombLight(i16 bomb) {
  if (Detonator != 0 && bomb >= 0 && bomb < Detonator->bomb_count)
    return Detonator->bombs[bomb].light;
  return -1;
}

// FUNCTION: LEGOBATMAN 0x005d59a0
void Detonators_Reset() {
  if (Detonator != 0) {
    memset(Detonator->detonators, 0, Detonator->count * sizeof(DETONATOR_s));
  }
}

f32 NuVecDistSqr(nuvec_s *a, nuvec_s *b, nuvec_s *d);

struct CHEAT_s {
  u32 pad0[4];
};

// GLOBAL: LEGOBATMAN 0x00936f10
extern CHEAT_s g_unk00936f10[];
// GLOBAL: LEGOBATMAN 0x00960b00
extern i32 g_unk00960b00; // the detonator cheat

i32 Cheat_IsOn(CHEAT_s *cheat);

// What FindFreeSlot reads of the thrower: obj->p54->p24->b236.
struct DETTHROWER_s {
  u8 pad0[0x54];
  struct {
    u8 pad0[0x24];
    struct {
      u8 pad0[0x236];
      i8 detonator_type; // 0x236
    } *p24;
  } *p54;
};

// STUB: LEGOBATMAN 0x005d59d0
// close: orig keeps obj in ebx from entry and widens the i16 limit once
// (movsx ebp, ax); ours reloads obj and widens inside the loop (3 tries)
DETONATOR_s *Detonator_FindFreeSlot(GameObject_s *obj) {
  if (Detonator != 0) {
    i32 type = ((DETTHROWER_s *)obj)->p54->p24->detonator_type;
    i16 max;
    if (Cheat_IsOn(&g_unk00936f10[g_unk00960b00]) && type == 0)
      max = type + 6;
    else
      max = type == 2.0f ? 1 : 3;
    i32 owned = 0;
    i32 slot = -1;
    for (i32 i = 0; i < Detonator->count; i++) {
      DETONATOR_s *det = &Detonator->detonators[i];
      if (det->flags & 1) {
        if (det->owner == obj && ++owned == max)
          return 0;
      } else if (slot == -1) {
        slot = i;
      }
    }
    if (slot != -1)
      return &Detonator->detonators[slot];
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x005d5aa0
DETONATOR_s *Detonator_FindNearest(nuvec_s *pos, float range,
                                   GameObject_s *owner) {
  DETONATOR_s *best = 0;
  if (Detonator != 0) {
    DETONATOR_s *d = Detonator->detonators;
    f32 best_dist = range != 0.0f ? range * range : 1.0e9f;
    for (i32 i = 0; i < Detonator->count; i++, d++) {
      if ((d->flags & 1) && (owner == 0 || d->owner == owner)) {
        f32 dist = NuVecDistSqr(pos, &d->pos, 0);
        if (dist < best_dist) {
          best_dist = dist;
          best = d;
        }
      }
    }
  }
  return best;
}

// STUB: LEGOBATMAN 0x005d5c20
// close: orig walks flags (+0x29) and character (+0x34) with two separate
// pointer IVs and an extra callee-saved reg; ours folds them into one.
DETONATOR_s *Detonator_FindCharacterDetonator(GameObject_s *character) {
  if (Detonator != 0) {
    i32 count = Detonator->count;
    DETONATOR_s *list = Detonator->detonators;
    for (i32 i = 0; i < count; i++) {
      if ((list[i].flags & 1) != 0) {
        if (list[i].character == character)
          return &list[i];
      }
    }
  }
  return 0;
}

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_gamemessages_unk(f32 *v, f32 a, i32 i) {
  v[2] = NuFabs(a);
  v[3] = NuFdiv(a, v[4]);
  v[0] = NuSinApprox(i);
  v[1] = NuCosApprox(i);
  NuVec4Set(v, a, a, a, a);
}
