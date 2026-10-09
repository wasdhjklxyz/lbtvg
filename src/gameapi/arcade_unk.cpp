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

i32 Players_BothActive();
void Arcade_AwardPoint(i32 player_index, i32 a, i32 extra);

// GLOBAL: LEGOBATMAN 0x00aca440
extern i32 Arcade_PlayerKills[2];

// STUB: LEGOBATMAN 0x00648290
// heavy x87; Batman calls a sine helper FUN_00648080 and reads the text id via
// a pointer at 0xad117c
#if 0
f32 NuFmod(f32 a, f32 b);

f32 NuTrigTable[NUTRIGTABLE_COUNT];

char **TTab;

i16 tARCADE_NEEDTWOPLAYERS;

void SmartTextEx(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u32 alignment, u8 red,
                     u8 green, u8 blue, f32 max_width, i32 max_lines, void *message_box, i32 suppress_draw, u32 alpha);

// from saga legoapi/menus/screens/arcade.cpp
void Arcade_DrawPanel(i32 paused) {
    if (Arcade == 0 || paused != 0 || Arcade_BothPlayersActive()) {
        return;
    }
    f32 phase = NuFmod(GameTimer.time_elapsed_mod_seconds, 0.5f);
    i32 angle = static_cast<i32>((phase + phase) * 65536.0f);
    i32 alpha = static_cast<i32>((NuTrigTable[(angle >> 1) & 0x7fff] * 0.2f + 0.8f) * 128.0f);
    f32 scale = Arcade_NeedTwoPlayers_Scale * 0.6f;
    SmartTextEx(TTab[tARCADE_NEEDTWOPLAYERS], 0.0f, -0.55f, 1.0f, scale, scale, scale, 0, 255, 0, 0,
                Arcade_NeedTwoPlayers_Scale * 1.7f, 1, NULL, 0, alpha);
}
#endif

// FUNCTION: LEGOBATMAN 0x006486e0
void Arcade_PlayerKilled(i32 player_index, i32 extra) {
  if ((player_index == 0 || player_index == 1) &&
      (Arcade_Mode[ArcadeItem.field_c_0xc].field8_0x8 & 1) != 0) {
    if (Players_BothActive()) {
      ++Arcade_PlayerKills[player_index];
      Arcade_AwardPoint(player_index, 0, extra);
    } else {
      Arcade_AwardPoint(player_index, 0, 0);
    }
  }
}
