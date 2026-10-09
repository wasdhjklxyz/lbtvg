// gameapi/arcade_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nu2api/nucore/common.h"
#include <stddef.h>

#include "../nu2api/numath/nuvec.h"

struct TIMER_s {
  union {
    struct {
      f32 time_elapsed;
      f32 last_time_elapsed;
      f32 time_elapsed_mod_seconds;
    };
    NUVEC elapsed_components;
  };
  i32 update_count;
};
typedef struct TIMER_s TIMER;

i32 Arcade;

i32 Arcade_BothPlayersActive();

static f32 Arcade_NeedTwoPlayers_Scale = 1.0f;

f32 FRAMETIME;

float SeekLinearF(float current, float target, float step);

TIMER GameTimer;

void GameAudio_PlaySfx(i32, nuvec_s *, i32, i32);

// from saga legoapi/menus/screens/arcade.cpp
// FUNCTION: LEGOBATMAN 0x00648200
void Arcade_UpdatePanel(i32 paused) {
  if (Arcade == 0 || paused != 0 || Arcade_BothPlayersActive()) {
    Arcade_NeedTwoPlayers_Scale = 1.0f;
  } else {
    Arcade_NeedTwoPlayers_Scale =
        SeekLinearF(Arcade_NeedTwoPlayers_Scale, 1.0f, FRAMETIME);
    if (static_cast<i32>(GameTimer.time_elapsed * 2.0f) !=
        static_cast<i32>(GameTimer.last_time_elapsed * 2.0f)) {
      GameAudio_PlaySfx(0x48, NULL, 0, 0); // saga (TCS) plays 0x35
    }
  }
}
