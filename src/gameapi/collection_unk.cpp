// gameapi/collection_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nu2api/nucore/common.h"
#include "gameobject_unk.h"

typedef struct COINPACKET_s {
  u32 coins;              // 0x00
  f32 scale;              // 0x04
  u16 lastcoin;           // 0x08
  u8 active;              // 0x0a
  u8 field_0xb;           // 0x0b
  f32 double_score_timer; // 0x0c
} COINPACKET;

// GLOBAL: LEGOBATMAN 0x00aca578
extern i32 BonusArea;
// GLOBAL: LEGOBATMAN 0x00a95fe0
extern f32 FRAMETIME;
// GLOBAL: LEGOBATMAN 0x00960528
extern i32 g_unk00960528;
// GLOBAL: LEGOBATMAN 0x00ab3960
extern GameObject_s *Player[8];

f32 SeekLinearF(f32 current, f32 target, f32 step);

// FUNCTION: LEGOBATMAN 0x00635ca0
void UpdateCoinPacket(COINPACKET_s *packet, i32 active, i32 player_index) {
  if (packet == 0)
    return;
  if (active != 0) {
    packet->scale = SeekLinearF(packet->scale, 1.0f, FRAMETIME * 3.0f);
    if (packet->double_score_timer > 0.0f && g_unk00960528 != -1 &&
        Player[player_index]->b9db != g_unk00960528) {
      packet->double_score_timer -= FRAMETIME;
      if (packet->double_score_timer <= 0.0f)
        packet->active = 1;
    }
  } else {
    packet->active = 1;
    packet->scale = 1.0f;
    packet->double_score_timer = 0.0f;
    if (BonusArea != 0) {
      if (packet->coins >= 10) {
        u32 decrement = (u32)(FRAMETIME * 20000.0f);
        decrement = (decrement / 10) * 10;
        if (decrement < 10)
          decrement = 10;
        if (decrement > packet->coins)
          packet->coins = 0;
        else
          packet->coins -= decrement;
      } else {
        packet->coins = 0;
      }
    }
  }
}
