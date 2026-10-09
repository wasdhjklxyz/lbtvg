// gameapi/carrying_unk.cpp: placed by tools/new.py; file name unproven.

#include "gameobject_unk.h"

// GLOBAL: LEGOBATMAN 0x009604fc
extern i32 LEGOCONTEXT_SUPERCARRY;

// FUNCTION: LEGOBATMAN 0x00645450
i32 SuperCarry_Carrying(GameObject_s *object) {
  if (LEGOCONTEXT_SUPERCARRY != -1 && object->b9db == LEGOCONTEXT_SUPERCARRY) {
    if (object->b9d9 == 2 || object->b9d9 == 3 || object->b9d9 == 6)
      return 1;
  }
  return 0;
}
