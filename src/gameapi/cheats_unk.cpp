// gameapi/cheats_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nu2api/nucore/common.h"
#include "gameobject_unk.h"

typedef struct CHEAT_s {
  char *name; // 0x00
  u32 pad4;
  unsigned __int64 flags; // 0x08
} CHEAT_s;

typedef struct CHEATENTRY_s {
  CHEAT_s *cheat; // 0x00
  u8 pad4[0xf - 4];
  i8 b0f; // 0x0f
  u8 pad10[0x18 - 0x10];
  u8 enabled; // 0x18
  i8 b19;     // 0x19
  u8 pad1a[2];
} CHEATENTRY_s;

typedef struct CHEATSYSTEM_s {
  CHEATENTRY_s *cheats; // 0x00
  i32 cheats_count;     // 0x04
  i32 i08;              // 0x08
  u32 pad0c;
  unsigned __int64 flags; // 0x10
} CHEATSYSTEM_s;

// GLOBAL: LEGOBATMAN 0x00960af4
extern CHEATSYSTEM_s *CheatSystem;
// GLOBAL: LEGOBATMAN 0x00960b14
extern i32 ONEPLAYERPOWERUPS;
// GLOBAL: LEGOBATMAN 0x00abe2b8
static f32 Cheat_PowerUpTime;
// GLOBAL: LEGOBATMAN 0x00aca574
extern i32 VehicleArea;

int NuStrICmp(const char *a, const char *b);

// FUNCTION: LEGOBATMAN 0x005ce730
void CheatSys_Init(CHEATENTRY_s *list) {
  CheatSystem->cheats = list;
  CheatSystem->cheats_count = 0;
  CheatSystem->i08 = 0;
  if (list != 0) {
    while (list->cheat != 0) {
      list->enabled = 0;
      list->b0f = -1;
      list->b19 = -1;
      CheatSystem->cheats_count++;
      list++;
    }
  }
}

// FUNCTION: LEGOBATMAN 0x005ce780
i32 Cheats_CountUpgrades(i32 *list) {
  i32 count = 0;
  for (i32 i = 0; i < CheatSystem->cheats_count; i++) {
    if ((CheatSystem->cheats[i].cheat->flags & 0x2000000) != 0) {
      if (list != 0)
        list[count] = i;
      count++;
    }
  }
  return count;
}

// FUNCTION: LEGOBATMAN 0x005ce7e0
i32 Cheat_FindByName(char *name) {
  for (i32 i = 0; i < CheatSystem->cheats_count; i++) {
    if (NuStrICmp(CheatSystem->cheats[i].cheat->name, name) == 0)
      return i;
  }
  return -1;
}

// FUNCTION: LEGOBATMAN 0x005ce830
void Cheats_SetFlags(void) {
  CheatSystem->flags = 0;
  for (i32 i = 0; i < CheatSystem->cheats_count; i++) {
    if (CheatSystem->cheats[i].enabled != 0 ||
        (ONEPLAYERPOWERUPS == 0 && Cheat_PowerUpTime > 0.0f &&
         (VehicleArea != 0
              ? (CheatSystem->cheats[i].cheat->flags & 0x20000) != 0
              : (CheatSystem->cheats[i].cheat->flags & 0x10000) != 0)))
      CheatSystem->flags |= CheatSystem->cheats[i].cheat->flags;
  }
}

// FUNCTION: LEGOBATMAN 0x005ce8d0
unsigned __int64 Cheats_CheckFlags(unsigned __int64 flags) {
  return CheatSystem->flags & flags;
}

// FUNCTION: LEGOBATMAN 0x005ce930
i32 Cheat_SetOn(CHEAT_s *cheat, i32 on, i32 unused) {
  for (i32 i = 0; i < CheatSystem->cheats_count; i++) {
    if (CheatSystem->cheats[i].cheat == cheat) {
      CheatSystem->cheats[i].enabled = on != 0;
      Cheats_SetFlags();
      return i;
    }
  }
  return -1;
}

// FUNCTION: LEGOBATMAN 0x005ce980
i32 Cheat_IsOn(CHEAT_s *cheat) {
  for (i32 i = 0; i < CheatSystem->cheats_count; i++) {
    if (CheatSystem->cheats[i].cheat == cheat) {
      if (CheatSystem->cheats[i].enabled != 0) {
        return 1;
      }
      if (ONEPLAYERPOWERUPS == 0 && Cheat_PowerUpTime > 0.0f) {
        if (VehicleArea != 0 ? (cheat->flags & 0x20000) != 0
                             : (cheat->flags & 0x10000) != 0) {
          return 1;
        }
      }
    }
  }
  return 0;
}

void *memset(void *dst, int c, unsigned int n);

// FUNCTION: LEGOBATMAN 0x005cea00
void Cheat_GetOnOffBitfield(i32 *onoffs, i32 count) {
  memset(onoffs, 0, ((count + 31) / 32) * 4);
  for (i32 i = 0; i < count; i++) {
    if (CheatSystem->cheats[i].enabled != 0)
      onoffs[(u32)i >> 5] |= 1 << (i & 31);
  }
}

// FUNCTION: LEGOBATMAN 0x005cea70
void Cheat_SetOnOffBitfield(i32 *onoffs, i32 count) {
  for (i32 i = 0; i < count; i++) {
    if (onoffs[(u32)i >> 5] & (1 << (i & 31)))
      CheatSystem->cheats[i].enabled = 1;
    else
      CheatSystem->cheats[i].enabled = 0;
  }
}

// FUNCTION: LEGOBATMAN 0x005ceac0
u32 Cheat_MultiplyScore(u32 score) {
  score = Cheats_CheckFlags(0x4) ? score * 2 : score;
  score = Cheats_CheckFlags(0x8) ? score * 4 : score;
  score = Cheats_CheckFlags(0x10) ? score * 6 : score;
  score = Cheats_CheckFlags(0x20) ? score * 8 : score;
  score = Cheats_CheckFlags(0x40) ? score * 10 : score;
  return score;
}

// FUNCTION: LEGOBATMAN 0x005ceb30
void Cheats_TurnOff(i32 cheat) {
  for (i32 i = 0; i < CheatSystem->cheats_count; i++) {
    if (cheat == 0 || (CheatSystem->cheats[i].cheat->flags & 0x2200000) != 0)
      CheatSystem->cheats[i].enabled = 0;
  }
  Cheats_SetFlags();
}

// GLOBAL: LEGOBATMAN 0x00960af8
extern f32 CHEAT_POWERUPTIME;
// GLOBAL: LEGOBATMAN 0x00abe2bc
extern i16 POWERUP_TEXTID;
// GLOBAL: LEGOBATMAN 0x00a957f8
extern char **TTab;

void NewRumble(nupad_s *pad, float strength, i32 frames);
void NewRumbleAllPlayers(f32 strength, f32 duration, i32 frames, i32 flags);
void GameCam_HitRoll(void);
void *AddGameMessage(char *text, nuvec_s *position, float scale,
                     nuvec_s *target_position, float target_scale,
                     unsigned char red, unsigned char green, unsigned char blue,
                     u32 flags, float duration);
void GameAudio_PlaySfx(i32 sfx, nuvec_s *position, i32 flags, i32 volume);

// FUNCTION: LEGOBATMAN 0x005ceb80
void Cheat_StartPowerUp(nuvec_s *position, GameObject_s *object) {
  void *display;
  if (ONEPLAYERPOWERUPS != 0) {
    if (object == 0)
      return;
    object->f1278 = CHEAT_POWERUPTIME;
    NewRumble(object->p112c->pad0, 0.7f, 0);
    GameCam_HitRoll();
  } else {
    NewRumbleAllPlayers(0.7f, 0.0f, 0, 0);
    Cheat_PowerUpTime = CHEAT_POWERUPTIME;
  }
  display = AddGameMessage(TTab[POWERUP_TEXTID], position, 0.5f, position,
                           0.75f, 0xff, 0xff, 0xff, 0x4023, 1.0f);
  if (display != 0)
    *(f32 *)((char *)display + 0xd4) = 0.75f;
  display = AddGameMessage(TTab[POWERUP_TEXTID], position, 0.5f, position,
                           0.25f, 0xff, 0xff, 0xff, 0x4023, 1.0f);
  if (display != 0)
    *(f32 *)((char *)display + 0xd4) = 0.75f;
  GameAudio_PlaySfx(0x73, 0, 0, 0);
}

// GLOBAL: LEGOBATMAN 0x00ab3960
extern GameObject_s *Player[8];

// FUNCTION: LEGOBATMAN 0x005cecc0
i32 Cheat_PowerUpActive(i32 index) {
  if (ONEPLAYERPOWERUPS != 0) {
    if (index == 0 || index == 1) {
      for (i32 i = 0; i < 2; i++) {
        if (Player[i] != 0 && Player[i]->f1278 > 0.0f &&
            (index == -1 || Player[i]->b24c == index))
          return 1;
      }
    }
  } else if (Cheat_PowerUpTime > 0.0f) {
    return 1;
  }
  return 0;
}

// GLOBAL: LEGOBATMAN 0x00a95fe0
extern f32 FRAMETIME;

void GameAudio_PlaySfx(i32 sfx, nuvec_s *position, i32 flags, i32 volume);
void ConstantRumble(GameObject_s *object, f32 a, f32 b);
i32 qrand(void);

// FUNCTION: LEGOBATMAN 0x005ced30
void Cheats_UpdatePowerUp005ced30(void) {
  if (ONEPLAYERPOWERUPS == 0 && Cheat_PowerUpTime > 0.0f) {
    Cheat_PowerUpTime -= FRAMETIME;
    if (Cheat_PowerUpTime <= 0.0f) {
      GameAudio_PlaySfx(0x75, 0, 0, 0);
    } else {
      GameAudio_PlaySfx(0x74, 0, 0, 0);
      ConstantRumble(0, qrand() * 1.5259021893143654e-05f * 0.5f, 0.0f);
    }
  }
}

// FUNCTION: LEGOBATMAN 0x005cedc0
void Cheats_Reset(void) {
  Cheat_PowerUpTime = 0.0f;
  Cheats_SetFlags();
}

// FUNCTION: LEGOBATMAN 0x005cedd0
void Cheats_Update(void) {
  Cheats_UpdatePowerUp005ced30();
  Cheats_SetFlags();
}

// GLOBAL: LEGOBATMAN 0x00aca560
extern i32 AREACOUNT;

// FUNCTION: LEGOBATMAN 0x005cede0
void Cheat_SetArea(i32 cheat, i32 area_id) {
  if (cheat >= 0 && cheat < CheatSystem->cheats_count && area_id >= 0 &&
      area_id < AREACOUNT) {
    CheatSystem->cheats[cheat].b0f = area_id;
    CheatSystem->cheats[cheat].b19 = CheatSystem->i08;
    CheatSystem->i08++;
  }
}
