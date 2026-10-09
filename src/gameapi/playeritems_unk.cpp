// gameapi/playeritems_unk.cpp: placed by tools/new.py; file name unproven.

#include "../batman/worldinfo_unk.h"
#include "../nu2api/nucore/common.h"

// FUNCTION: LEGOBATMAN 0x005f1120
unsigned __int64 PlayerItems_GetAllCarriedItemFlags(GameObject_s *object) {
  if (object != 0)
    return object->carried_item_flags;
  return 0;
}

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

#include "../nu2api/nu3d/nuspecial.h"
#include <stddef.h>

typedef struct nufpar_s {
  u8 pad0[0x910];
  char *word_buf; // 0x910
} NUFPAR;

struct CHARPLATFORM_s {
  nuhspecial_s special; // 0x00
  i16 object_id;        // 0x0c
  i16 platform_id;      // 0x0e
  GameObject_s *object; // 0x10
};

struct CHARPLATFORMSYS_s {
  nugscn_s *scene; // 0x00
  i32 platform_count;
  CHARPLATFORM_s platforms[1];
};

NUFPAR *NuFParCreateMem(char *name, char *buffer, i32 bufferSize);
i32 NuFParGetLine(NUFPAR *parser);
i32 NuFParGetWord(NUFPAR *parser);
i32 CharIDFromName(char *name);
void NuFParDestroy(NUFPAR *parser);

// from saga legoapi/characters/core/charplatforms.cpp
// FUNCTION: LEGOBATMAN 0x00638f90
void CharPlatforms_Configure(WORLDINFO_s *world, char *config) {
  world->char_platform_sys = NULL;
  if (world->scn140 == NULL)
    return;

  NUFPAR *parser = NuFParCreateMem("CharPlatforms", config, 0xffff);
  if (parser == NULL)
    return;

  world->buf104.addr = (world->buf104.addr + 3) & ~3;
  CHARPLATFORMSYS_s *system = (CHARPLATFORMSYS_s *)world->buf104.void_ptr;
  world->char_platform_sys = system;
  system->scene = world->scn140;
  world->char_platform_sys->platform_count = 0;

  while (NuFParGetLine(parser) != 0) {
    if (NuFParGetWord(parser) == 0)
      break;
    if (NuStrICmp(parser->word_buf, "char_platform") != 0 ||
        NuFParGetWord(parser) == 0)
      continue;

    system = world->char_platform_sys;
    CHARPLATFORM_s *platform = &system->platforms[system->platform_count];
    platform->object_id = CharIDFromName(parser->word_buf);
    if (platform->object_id == -1 || NuFParGetWord(parser) == 0)
      continue;
    if (NuSpecialFind(world->scn140, &platform->special, parser->word_buf, 1) ==
        0)
      continue;

    platform->platform_id = -1;
    platform->object = NULL;
    world->char_platform_sys->platform_count++;
  }

  NuFParDestroy(parser);
  if (world->char_platform_sys->platform_count > 0) {
    world->buf104.addr = (world->buf104.addr + sizeof(CHARPLATFORMSYS_s) +
                          (world->char_platform_sys->platform_count - 1) *
                              sizeof(CHARPLATFORM_s) +
                          3) &
                         ~3;
  } else {
    world->char_platform_sys = NULL;
  }
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

struct ANIMPACKET_s;
i32 CurrentAnim(ANIMPACKET_s *packet);
i32 GameAudio_GetPlrSfxBits(void *object_ptr);
void GameAudio_PlaySfx(i32 sfx, nuvec_s *position, i32 flags, i32 volume);

// GLOBAL: LEGOBATMAN 0x00ace08c
extern i32 WeaponInOut_NoAIJediSfx;

static __forceinline void FastWeaponOutSfx(GameObject_s *object) {
  if (object->weapon_scale_state == 1)
    return;
  i32 current_animation = CurrentAnim((ANIMPACKET_s *)((u8 *)object + 8));
  if (current_animation != -1) {
    Unk_GameObject50_08_204 **slot =
        &((Unk_GameObject50_08_204 **)object->p50->p08)[current_animation];
    if (*slot != 0 && ((*slot)->flags4 & 0x100000) != 0)
      return;
  }
  u32 model_flags = object->p54->model_flags;
  if ((model_flags & 8) != 0) {
    if (object->b24c != -1 || WeaponInOut_NoAIJediSfx == 0) {
      GameAudio_PlaySfx(0x53, &object->v80, GameAudio_GetPlrSfxBits(object), 1);
    }
  } else if ((model_flags & 0x80) != 0) {
    GameAudio_PlaySfx(0x5b, &object->v80, 0, 1);
  }
}

// STUB: LEGOBATMAN 0x006396c0
// close: orig re-reads the anim slot (cmp [slot],0 then mov eax,[slot]);
// this loads it once. Rest (inlined FastWeaponOutSfx) lines up.
void FastWeaponOut(GameObject_s *object, i32 force_sound) {
  if (g_unk00962144 == 0) {
    char context = object->b9db;
    if (context != -1 &&
        (context == LEGOCONTEXT_WEAPONIN || context == LEGOCONTEXT_WEAPONOUT)) {
      object->b9db = -1;
    }
    if (force_sound != 0 && object->weapon_scale == 0.0f)
      FastWeaponOutSfx(object);
    object->weapon_scale_rate = 5.0f;
    object->weapon_scale_state = 1;
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

static __forceinline void FastWeaponInSfx(GameObject_s *object) {
  if (object->weapon_scale_state == 2)
    return;
  i32 current_animation = CurrentAnim((ANIMPACKET_s *)((u8 *)object + 8));
  if (current_animation != -1) {
    void **anims = (void **)object->p50->p08;
    if (anims[current_animation] != 0 &&
        (((Unk_GameObject50_08_204 *)anims[current_animation])->flags4 &
         0x200000) != 0)
      return;
  }
  u32 model_flags = object->p54->model_flags;
  if ((model_flags & 8) != 0) {
    if (object->b24c != -1 || WeaponInOut_NoAIJediSfx == 0) {
      GameAudio_PlaySfx(0x52, &object->v80, GameAudio_GetPlrSfxBits(object), 1);
    }
  } else if ((model_flags & 0x80) != 0) {
    GameAudio_PlaySfx(0x5a, &object->v80, 0, 1);
  }
}

// STUB: LEGOBATMAN 0x00639ba0
// close: same anim-slot re-read as FastWeaponOut (cmp [slot],0 then reload)
void FastWeaponIn(GameObject_s *object, i32 force_sound) {
  if (g_unk00962144 == 0) {
    char context = object->b9db;
    if (context != -1 &&
        (context == LEGOCONTEXT_WEAPONIN || context == LEGOCONTEXT_WEAPONOUT)) {
      object->b9db = -1;
    }
    if (force_sound != 0 && object->weapon_scale == 1.0f)
      FastWeaponInSfx(object);
    object->weapon_scale_rate = 5.0f;
    object->weapon_scale_state = 2;
  }
}
