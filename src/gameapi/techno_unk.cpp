// gameapi/techno_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nu2api/nu3d/nuspecial.h"
#include "gameobject_unk.h"

struct TECHNO_s {
  u8 pad0[0x83];
  u8 target_mode; // 0x83
  u8 pad84[0xb8 - 0x84];
  void *controlled_object; // 0xb8
};

i32 NuSpecialCompare(nuhspecial_s *a, nuhspecial_s *b);

// GLOBAL: LEGOBATMAN 0x00960558
extern i32 LEGOCONTEXT_TECHNO;
// GLOBAL: LEGOBATMAN 0x00ab3960
extern GameObject_s *Player[8];

// FUNCTION: LEGOBATMAN 0x005a7bd0
i32 Techno_FindOperator(void *target, Unk_GameObject112c **pad,
                        GameObject_s **operator_object) {
  if (LEGOCONTEXT_TECHNO == -1)
    return 0;
  for (i32 index = 0; index < 8; ++index) {
    if (Player[index] != 0 && Player[index]->b9db == LEGOCONTEXT_TECHNO &&
        Player[index]->techno != 0) {
      TECHNO_s *techno = Player[index]->techno;
      if ((techno->target_mode == 2 &&
           NuSpecialCompare((nuhspecial_s *)target,
                            (nuhspecial_s *)techno->controlled_object) != 0) ||
          Player[index]->techno->controlled_object == target) {
        if (pad != 0) {
          *pad = Player[index]->p112c;
        }
        if (operator_object != 0) {
          *operator_object = Player[index];
        }
        return 1;
      }
    }
  }
  return 0;
}
