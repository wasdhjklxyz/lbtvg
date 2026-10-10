// gameapi/playeritems_unk.cpp: placed by tools/new.py; file name unproven.

#include "../batman/worldinfo_unk.h"
#include "../nu2api/nucore/common.h"
#include "../nu2api/nucore/nulist.h"
#include <string.h>

struct PLAYERITEMTYPESYS_s;

// GLOBAL: LEGOBATMAN 0x00ac9ff0
extern PLAYERITEMTYPESYS_s *PlayerItemTypeSys;

// FUNCTION: LEGOBATMAN 0x005ed990
static void Unk005ed990(...) {}
PLAYERITEMTYPESYS_s *PlayerItemTypeSys_Load(char *file, nugscn_s *scene,
                                            variptr_u *buffer,
                                            variptr_u *buffer_end);

// FUNCTION: LEGOBATMAN 0x005f0250
void PlayerItemTypeSys_LoadGlobal(char *file, nugscn_s *scene,
                                  variptr_u *buffer, variptr_u *buffer_end) {
  if (PlayerItemTypeSys != 0)
    Unk005ed990(file);
  PlayerItemTypeSys = PlayerItemTypeSys_Load(file, scene, buffer, buffer_end);
}

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

struct GIZMOPICKUPTYPE_s {
  u8 pad0[0x12];
  u16 score; // 0x12
  u8 pad14[0x3c - 0x14];
};

struct GIZMOPICKUPSYS_s {
  GIZMOPICKUPTYPE_s *types; // 0x00
};

// GLOBAL: LEGOBATMAN 0x009652e4
extern GIZMOPICKUPSYS_s *GizmoPickupSys;

struct PLAYERITEMTYPE_s {
  u32 pad0[0x30 / 4];
  u64 flags; // 0x30
  u32 pad38[(0xf8 - 0x38) / 4];
  i16 wf8; // 0xf8
};

// Carried-item list nodes: the item's type at +8.
struct PLAYERITEM_s {
  NULISTLNK link;
  PLAYERITEMTYPE_s *type; // 0x08
  nuvec_s pos;            // 0x0c
  u8 pad18[0x27 - 0x18];
  u8 flags; // 0x27, 1 = in use
};

// FUNCTION: LEGOBATMAN 0x005f1140
i32 PlayerItems_DontPickUpItemType(GameObject_s *obj, PLAYERITEMTYPE_s *type) {
  if ((type->flags & 0x2000000000ull) != 0 && *((u8 *)obj + 0x24c) != 0xff)
    return 1;
  if ((type->flags & 0x81) != 0)
    return 0;
  if ((*(u64 *)((u8 *)obj + 0xb38) & type->flags) == type->flags) {
    NULISTHDR *list = (NULISTHDR *)((u8 *)obj + 0xb18);
    for (PLAYERITEM_s *node = (PLAYERITEM_s *)NuListGetHead(list); node != 0;
         node = (PLAYERITEM_s *)NuListGetNext(list, &node->link)) {
      if (node->type == type)
        return 1;
    }
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x00635020
u32 GizmoPickups_TotalScore(void *world) {
  Unk_WorldInfo5220 *system = ((WORLDINFO_s *)world)->p5220;
  Unk_WorldInfo5220Entry *pickup = system->list;
  u32 score = 0;
  if (pickup != 0) {
    for (i32 i = 0; i < system->count; ++i, ++pickup)
      score += GizmoPickupSys->types[pickup->type_index].score;
  }
  return score;
}

// FUNCTION: LEGOBATMAN 0x00635070
i32 GizmoPickup_NumberOfType(WORLDINFO_s *world, i32 type_index,
                             char type_code) {
  Unk_WorldInfo5220 *runtime = world->p5220;
  Unk_WorldInfo5220Entry *pickup = runtime->list;
  i32 count = 0;
  if (type_code == 0 && type_index == -1)
    return 0;
  for (i32 index = 0; index < runtime->count; ++index, ++pickup) {
    if (type_code != 0) {
      if (pickup->type_code == type_code)
        count++;
    } else if (pickup->type_index == type_index) {
      count++;
    }
  }
  return count;
}

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

// FUNCTION: LEGOBATMAN 0x00635130
void GizmoPickup_TurnOnPickup(Unk_WorldInfo5220Entry *pickup, i32 on) {
  if (pickup != 0) {
    if (on != 0) {
      pickup->enabled = 1;
      pickup->visible = 1;
      pickup->activated = 1;
    } else {
      pickup->enabled = 0;
      pickup->visible = 0;
    }
  }
}

// FUNCTION: LEGOBATMAN 0x00635150
i32 GizmoPickup_BeenTurnedOn(Unk_WorldInfo5220Entry *pickup) {
  return pickup != 0 ? pickup->activated : 0;
}

// FUNCTION: LEGOBATMAN 0x00635170
void GizmoPickups_PostLoad(void *a, void *b) {}

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
#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"
#include <stddef.h>

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x005ed9b0
static f32 NuFabs(f32 f);
// FUNCTION: LEGOBATMAN 0x005ed9d0
static f32 NuFdiv(f32 a, f32 b);
// FUNCTION: LEGOBATMAN 0x005eda10
static f32 NuSinApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x005edab0
static f32 NuCosApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x005edac0
static void NuVec4Copy(f32 *dst, f32 *src);
// FUNCTION: LEGOBATMAN 0x005edae0
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

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
    if (((Unk_GameObject50_08_204 **)object->p50->p08)[current_animation] !=
            0 &&
        (((Unk_GameObject50_08_204 **)object->p50->p08)[current_animation]
             ->flags4 &
         0x100000) != 0)
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

// FUNCTION: LEGOBATMAN 0x006396c0
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
    if (((Unk_GameObject50_08_204 **)object->p50->p08)[current_animation] !=
            0 &&
        (((Unk_GameObject50_08_204 **)object->p50->p08)[current_animation]
             ->flags4 &
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

// FUNCTION: LEGOBATMAN 0x00639ba0
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

void Unk00574270(i32 index);
void Unk005f0480(nuvec_s *pos, i32 drop);
void Unk005f13d0(GameObject_s *obj);
extern "C" void NuListRemove(NULISTHDR *list, NULISTLNK *node);
extern "C" void NuListAppend(NULISTHDR *list, NULISTLNK *node);

// FUNCTION: LEGOBATMAN 0x005f3440
void PlayerItems_RemoveItem(GameObject_s *obj, PLAYERITEM_s *item, i32 drop) {
  if (obj != 0 && item != 0 && (item->flags & 1) != 0) {
    PLAYERITEMTYPE_s *type = item->type;
    if ((type->flags & 0x8000) != 0 && type->wf8 != -1 &&
        *(i16 *)((u8 *)obj + 0xb54) != -1) {
      Unk00574270(*(i16 *)((u8 *)obj + 0xb54));
      *(i16 *)((u8 *)obj + 0xb54) = -1;
      *((u8 *)obj + 0xb57) = 0;
    }
    if (*(PLAYERITEM_s **)((u8 *)obj + 0xb20) == item)
      *(PLAYERITEM_s **)((u8 *)obj + 0xb20) = 0;
    if (drop != 0)
      Unk005f0480(&item->pos, drop);
    NuListRemove((NULISTHDR *)((u8 *)obj + 0xb18), &item->link);
    memset(item, 0, sizeof(*item));
    NuListAppend((NULISTHDR *)((u8 *)obj + 0xb10), &item->link);
    Unk005f13d0(obj);
  }
}

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_playeritems_unk(f32 *v, f32 a, i32 i) {
  v[2] = NuFabs(a);
  v[3] = NuFdiv(a, v[4]);
  v[0] = NuSinApprox(i);
  v[1] = NuCosApprox(i);
  NuVec4Copy(v + 4, v);
  NuVec4Set(v, a, a, a, a);
}
