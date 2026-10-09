// gameapi/gamemessages_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nu2api/nucore/common.h"
#include "../nu2api/numath/nuvec.h"
#include <stdio.h>
#include <string.h>

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
