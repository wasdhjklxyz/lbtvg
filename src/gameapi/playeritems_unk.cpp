// gameapi/playeritems_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nu2api/nucore/common.h"
#include "gameobject_unk.h"

// GLOBAL: LEGOBATMAN 0x00962144
extern i32 g_unk00962144;

// GLOBAL: LEGOBATMAN 0x009604d0
extern i32 LEGOCONTEXT_WEAPONIN;

// GLOBAL: LEGOBATMAN 0x009604d4
extern i32 LEGOCONTEXT_WEAPONOUT;

i32 NuAtan2D(f32 dx, f32 dy);

// FUNCTION: LEGOBATMAN 0x006394f0
i32 FaceOpponent(GameObject_s *object, nuvec_s *position) {
  if (position == 0) {
    GameObject_s *target = object->force_target;
    if (target != 0) {
      if (!(target->flags1fc & 1) || !(target->flags1fc & 0x1000)) {
        return 0;
      }
      if (target->b257 != 0) {
        return 0;
      }
      position = &target->position;
    } else {
      if (object->blowup_target == 0) {
        return 0;
      }
      position = &object->blowup_target->mid_position;
    }
  }
  if (position != 0) {
    object->facing_angle = NuAtan2D(position->x - object->position.x,
                                    position->z - object->position.z);
    return 1;
  }
  return 0;
}

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
