// batman/jumping_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nu2api/nucore/common.h"
#include "worldinfo_unk.h"
#include <stddef.h>

char DefinedLocators_FindIX(char *name);

// FUNCTION: LEGOBATMAN 0x004c0cb0
void StartBallooning(GameObject_s *object, i32 movement_state) {
  object->b9db = 0x5d;
  object->b9d9 = movement_state;
  if (object->p50->p0c->i2c8 != 0) {
    object->s9d0 = 0xb2;
  } else {
    object->s9d0 = object->s162c;
  }
  object->f988 = 1000000000.0f;
  object->b9da = DefinedLocators_FindIX("right_backpack");
  object->b9da = object->p54->p24->b1dc[object->b9da];
  object->f998 = 1.0f;
  object->b9d9 = 1;
  if (g_unk00960894->p2b04->bd7e) {
    object->s9d2 = 0xd7;
  } else if (g_unk00960894->p2b04->bd8e) {
    object->s9d2 = 0xd8;
  } else {
    object->s9d2 = g_unk00960894->p2b04->bd9e ? 0xd9 : -1;
  }
}
