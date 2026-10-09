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

struct ARCADE_MODE_s {
  i16 *text;
  i32 target;
  i32 field8_0x8;
};

struct ARCADEITEM_s {
  i16 *level_text;
  i8 level;
  u8 level_count;
  u16 pad_06;
  i16 *mode_text;
  char field_c_0xc;
  u8 mode_count;
  u16 pad_0e;
  i16 *play_text;
  i8 play;
  u8 play_count;
  u16 pad_16;
};

ARCADE_MODE_s Arcade_Mode[];

ARCADEITEM_s ArcadeItem;

// name is a Mac pairing hint (gapfill): verify
// from saga legoapi/menus/screens/arcade.cpp
// FUNCTION: LEGOBATMAN 0x006481b0
i32 Arcade_GetMode(u32 *flags) {
  if (Arcade == 0) {
    if (flags != NULL) {
      *flags = 0;
    }
    return -1;
  }
  if (flags != NULL) {
    *flags = Arcade_Mode[ArcadeItem.field_c_0xc].field8_0x8;
  }
  return ArcadeItem.field_c_0xc;
}

// name is a Mac pairing hint (order): verify
// from saga legoapi/menus/screens/arcade.cpp
// FUNCTION: LEGOBATMAN 0x006481f0
void Arcade_ResetPanel() { Arcade_NeedTwoPlayers_Scale = 1.0f; }

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
