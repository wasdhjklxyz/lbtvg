// gameapi/playeritems_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nu2api/nucore/common.h"
#include "gameobject_unk.h"

// GLOBAL: LEGOBATMAN 0x00962144
extern i32 g_unk00962144;

// GLOBAL: LEGOBATMAN 0x009604d0
extern i32 LEGOCONTEXT_WEAPONIN;

// GLOBAL: LEGOBATMAN 0x009604d4
extern i32 LEGOCONTEXT_WEAPONOUT;

// FUNCTION: LEGOBATMAN 0x00639670
void SetWeaponOut(GameObject_s *object) {
  if (g_unk00962144 == 0) {
    char context = object->b9db;
    if (context != -1 &&
        (context == LEGOCONTEXT_WEAPONIN || context == LEGOCONTEXT_WEAPONOUT)) {
      object->b9db = -1;
    }
    object->weapon_scale = 1.0f;
    object->flags130c |= 0x80000;
    object->weapon_scale_state = 0;
  }
}
