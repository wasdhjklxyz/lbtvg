// gameapi/gamemessages_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nu2api/nucore/common.h"
#include "../nu2api/numath/nuvec.h"
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

typedef struct DETONATOR_s {
  u32 data[0x60 / 4];
} DETONATOR_s;

typedef struct DETONATORSYS_s {
  DETONATOR_s *detonators; // 0x00
  u32 pad4[(0xc4 - 4) / 4];
  u8 count; // 0xc4
} DETONATORSYS_s;

// GLOBAL: LEGOBATMAN 0x00ac6f48
extern DETONATORSYS_s *Detonator;

// FUNCTION: LEGOBATMAN 0x005d59a0
void Detonators_Reset() {
  if (Detonator != 0) {
    memset(Detonator->detonators, 0, Detonator->count * sizeof(DETONATOR_s));
  }
}
