// gameapi/playeritems_unk.cpp: placed by tools/new.py; file name unproven.

#include "../batman/worldinfo_unk.h"
#include "../nu2api/nucore/common.h"

// GLOBAL: LEGOBATMAN 0x00962144
extern i32 g_unk00962144;

// GLOBAL: LEGOBATMAN 0x009604d0
extern i32 LEGOCONTEXT_WEAPONIN;

// GLOBAL: LEGOBATMAN 0x009604d4
extern i32 LEGOCONTEXT_WEAPONOUT;

i32 NuAtan2D(f32 dx, f32 dy);

typedef struct COINPACKET_s {
  u32 coins;              // 0x00
  f32 scale;              // 0x04
  u16 lastcoin;           // 0x08
  u8 active;              // 0x0a
  u8 field_0xb;           // 0x0b
  f32 double_score_timer; // 0x0c
} COINPACKET;

i32 NuStrICmp(const char *a, const char *b);

// FUNCTION: LEGOBATMAN 0x006350d0
Unk_WorldInfo5220Entry *GizmoPickup_FindByName(WORLDINFO_s *world, char *name) {
  if (world != 0 && name != 0) {
    Unk_WorldInfo5220Entry *pickup = world->p5220->list;
    if (pickup != 0) {
      for (i32 index = 0; index < world->p5220->count; ++index, ++pickup) {
        if (NuStrICmp(pickup->name, name) == 0)
          return pickup;
      }
    }
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x00635c80
void ResetCoinPacket(COINPACKET_s *packet) {
  if (packet != 0) {
    packet->scale = 1.0f;
    packet->double_score_timer = 0.0f;
    packet->active = 1;
  }
}

struct LEVELDATA_s {
  u32 pad0[0x64 / 4];
  u32 flags; // 0x64
};

struct AREADATA_s {
  u32 pad0[0x7c / 4];
  u32 flags; // 0x7c
};

// GLOBAL: LEGOBATMAN 0x00ab0898
extern i32 SuperStory;

// FUNCTION: LEGOBATMAN 0x00635de0
i32 CoinsGoToMainTotal() {
  WORLDINFO_s *world = WorldInfo_CurrentlyActive();
  if (world->area != 0 && (world->area->flags & 0x40) != 0)
    return 1;
  if (SuperStory != 0)
    return 1;
  if (world->area != 0 && (world->area->flags & 0x100) != 0)
    return 1;
  if (world->current_level != 0 && (world->current_level->flags & 0x400) != 0)
    return 1;
  return 0;
}

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

// FUNCTION: LEGOBATMAN 0x00639b10
void KeepWeaponOut(GameObject_s *object) {
  if (g_unk00962144 == 0) {
    object->weapon_scale = 1.0f;
    object->flags130c |= 0x80000;
    object->weapon_scale_state = 0;
    object->flags140c |= 0x100;
  }
}

// FUNCTION: LEGOBATMAN 0x00639b50
void SetWeaponIn(GameObject_s *object) {
  if (g_unk00962144 == 0) {
    char context = object->b9db;
    if (context != -1 &&
        (context == LEGOCONTEXT_WEAPONIN || context == LEGOCONTEXT_WEAPONOUT)) {
      object->b9db = -1;
    }
    object->weapon_scale = 0.0f;
    object->flags130c &= ~0x80000;
    object->weapon_scale_state = 0;
  }
}
