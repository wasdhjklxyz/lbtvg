// gameapi/cheats_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nu2api/nucore/common.h"

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
