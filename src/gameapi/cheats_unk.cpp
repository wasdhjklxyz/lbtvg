// gameapi/cheats_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nu2api/nucore/common.h"

typedef struct CHEAT_s {
  u32 pad0[2];
  unsigned __int64 flags; // 0x08
} CHEAT_s;

typedef struct CHEATENTRY_s {
  CHEAT_s *cheat; // 0x00
  u32 pad4[5];
  u8 enabled; // 0x18
  u8 pad19[3];
} CHEATENTRY_s;

typedef struct CHEATSYSTEM_s {
  CHEATENTRY_s *cheats; // 0x00
  i32 cheats_count;     // 0x04
} CHEATSYSTEM_s;

// GLOBAL: LEGOBATMAN 0x00960af4
extern CHEATSYSTEM_s *CheatSystem;
// GLOBAL: LEGOBATMAN 0x00960b14
extern i32 ONEPLAYERPOWERUPS;
// GLOBAL: LEGOBATMAN 0x00abe2b8
extern f32 Cheat_PowerUpTime;
// GLOBAL: LEGOBATMAN 0x00aca574
extern i32 VehicleArea;

// STUB: LEGOBATMAN 0x005ce980
// close: orig compares VehicleArea against a fresh xor ecx,ecx (the u64 high
// half); this reuses ebp (ONEPLAYERPOWERUPS, known 0 on that path).
i32 Cheat_IsOn(CHEAT_s *cheat) {
  for (i32 i = 0; i < CheatSystem->cheats_count; i++) {
    if (CheatSystem->cheats[i].cheat == cheat) {
      if (CheatSystem->cheats[i].enabled != 0) {
        return 1;
      }
      if (ONEPLAYERPOWERUPS == 0 && Cheat_PowerUpTime > 0.0f) {
        if ((VehicleArea != 0 ? cheat->flags & 0x20000
                              : cheat->flags & 0x10000) != 0) {
          return 1;
        }
      }
    }
  }
  return 0;
}
