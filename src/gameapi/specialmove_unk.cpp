// gameapi/specialmove_unk.cpp: SpecialMove_* (Mac); file name unproven.

#include "../batman/worldinfo_unk.h"
#include "../nu2api/nucore/common.h"

// 0x9c bytes per special move; only the phase byte is evidenced.
struct SPECIALMOVE_s {
  u8 pad00[0x98];
  i8 next_phase; // 0x98, -1 = single phase
  u8 pad99[0x9c - 0x99];
};

// GLOBAL: LEGOBATMAN 0x00acd800
extern SPECIALMOVE_s *SpecialMoves;

// GLOBAL: LEGOBATMAN 0x00960528
extern i32 LEGOCONTEXT_SPECIALMOVE;

// GLOBAL: LEGOBATMAN 0x0096052c
extern i32 LEGOCONTEXT_SPECIALMOVE2;

// FUNCTION: LEGOBATMAN 0x0062dee0
i32 SpecialMove_IsInMultiPhase(GameObject_s *object) {
  if (object != 0 &&
      (object->b9db == LEGOCONTEXT_SPECIALMOVE ||
       object->b9db == LEGOCONTEXT_SPECIALMOVE2) &&
      object->special_move != -1 &&
      SpecialMoves[object->special_move].next_phase != -1)
    return 1;
  return 0;
}
