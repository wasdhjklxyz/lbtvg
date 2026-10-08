// batman/, file unknown: status screen stages (0x00481070).

#include "../gameapi/sfx_unk.h"

struct STATUS_STAGE_s {
  u8 pad0[0x20];
  i32 state;    // 0x20
  f32 timer;    // 0x24
  f32 duration; // 0x28
};

struct STATUSPACKET_s {
  u8 pad0[0x6c];
  f32 f6c; // 0x6c
  u8 pad1[0xc6 - 0x70];
  char bc6; // 0xc6
};

void SetDrawGoldBrick(STATUSPACKET_s *packet, i32 draw);
void NextStatusStage(STATUSPACKET_s *packet);
void NewStatusRumbleBuzz(i32 pad, f32 duration, f32 delay, i32 mode);

// FUNCTION: LEGOBATMAN 0x00481070
void BonusTime_Update(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, f32 dt) {
  f32 prev;
  f32 now;
  switch (stage->state) {
  case 0:
    stage->state = 1;
    stage->timer = 0.0f;
    stage->duration = 4.0f;
    break;
  case 1:
    SetDrawGoldBrick(packet, packet->bc6);
    prev = stage->timer;
    now = stage->timer + dt;
    stage->timer = now;
    if (stage->duration <= now) {
      NextStatusStage(packet);
      return;
    }
    if (prev < 0.5f && now >= 0.5f) {
      if (packet->f6c != 0.0f)
        PlaySfx("StatusAward", 0);
      else
        GameAudio_PlaySfx(0x45, 0, 0, 0);
      NewStatusRumbleBuzz(-1, 0.6f, 0.0f, 0);
    }
    break;
  }
}
