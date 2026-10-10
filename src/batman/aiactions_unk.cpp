// batman/, file unknown: AI script Action_* handlers
// (0x004556b0..0x004717b0).

#include "../gameapi/ai/aisys_unk.h"
#include "../nu2api/nucore/nulist.h"
#include "../nu2api/nucore/nustring.h"
#include "worldinfo_unk.h"
#include <string.h>

// GLOBAL: LEGOBATMAN 0x00ad6918
extern i32 (*AIActionParseSpeedFn)(char *str, u8 *out);

void AIMoveInstruction(AIPACKET_s *packet, nuvec_s *pos, f32 height,
                       AIPATHINFO *path_info, i32 type, f32 param);
void Player_ClearContext(GameObject_s *obj, i32 mode);
extern u8 aicreature_sets_alive[16];

void Detonate(nuvec_s *pos, i32 type, f32 scale);
void AddMiscPickups(nuvec_s *pos, i32 player_id, i32 coins, i32 torpedoes,
                    i32 a);
void StunGameObject(GameObject_s *target, GameObject_s *by, f32 time,
                    i32 flags);

// Most actions inline this; some (Action_AddPartDebris) call the out-of-line
// GetNamedGameObject at 0x44c930 instead.
GameObject_s *GetNamedGameObject(AISYS_s *sys, char *name);
static inline GameObject_s *GetNamedGameObjectInl(AISYS_s *sys, char *name) {
  Unk_AIPacketObj *api;
  if (GetNamedAPIObjectFn && (api = GetNamedAPIObjectFn(sys, name)))
    return api->obj;
  return 0;
}

// GLOBAL: LEGOBATMAN 0x00ad68ec
extern u8 (*g_unk00ad68ec)(char *name);
// GLOBAL: LEGOBATMAN 0x00ad68fc
extern i32 (*g_unk00ad68fc)(i32 id);
AISTATE *AIStateFind(char *name, AISCRIPT *script);
f32 NuVecMagSqr(nuvec_s *v);
void NuVecSub(nuvec_s *out, nuvec_s *a, nuvec_s *b);

// 0xa8-byte creature origins at AISYS_s +0x224 (GameObject_s::b3c8 indexes).
struct AICREATURE_s {
  u8 pad0[0x20];
  nuvec_s pos; // 0x20
  u16 y_rot;   // 0x2c
  u8 pad2e[0x50 - 0x2e];
  i16 type; // 0x50, apicharsys character index
  u8 pad52[0xa8 - 0x52];
};

struct APICHARDATA_s {
  u8 pad0[4];
  u32 model_flags; // 0x04, 0x200 = goody, 4 = baddy
  u8 pad8[0x48 - 8];
};

struct APICHARSYS_s {
  u8 pad0[0x20];
  APICHARDATA_s *char_data; // 0x20
};

// GLOBAL: LEGOBATMAN 0x00a94740
extern APICHARSYS_s *apicharsys;
// GLOBAL: LEGOBATMAN 0x00ab0818
extern u32 LEGO_AIPATHCNX_FORGOODIES;
// GLOBAL: LEGOBATMAN 0x00ab081c
extern u32 LEGO_AIPATHCNX_FORBADDIES;

static __forceinline void ActionApplySide(AISYS_s *sys, GameObject_s *obj,
                                          i32 side) {
  u32 old_flags = obj->flags1f8;
  u32 old_side = old_flags & 0x10005;
  obj->flags1f8 = old_flags & 0xfffefffa;
  switch (side) {
  case 0:
    obj->flags1f8 |= 4;
    break;
  case 1:
    obj->flags1f8 |= 1;
    break;
  case 2:
    obj->flags1f8 |= 0x10000;
    break;
  case 3:
    if ((obj->flags1f8 & 0x400) && obj->b3c8 != 0xff) {
      AICREATURE_s *creature =
          &(*(AICREATURE_s **)((u8 *)sys + 0x224))[obj->b3c8];
      u32 model_flags = apicharsys->char_data[creature->type].model_flags;
      if (model_flags & 0x200)
        obj->flags1f8 |= 4;
      else if (model_flags & 4)
        obj->flags1f8 |= 1;
    }
    break;
  }
  u32 flags = obj->flags1f8;
  obj->capabilities &= ~(LEGO_AIPATHCNX_FORGOODIES | LEGO_AIPATHCNX_FORBADDIES);
  if (flags & 1)
    obj->capabilities |= LEGO_AIPATHCNX_FORBADDIES;
  else if (!(flags & 4))
    obj->capabilities |= LEGO_AIPATHCNX_FORGOODIES;
  if ((old_side & flags) == 0) {
    ((AIPACKET_s *)obj->process290)->pe4 = NULL;
    obj->alert_target_timer = 0.0f;
    obj->flags480 &= ~0x800;
    obj->alert_target = NULL;
  }
}

// STUB: LEGOBATMAN 0x004541f0
// Only diff: the creature lookup in both inlined ActionApplySide copies loads
// sys->creatures into a register ([ecx+esi+0x50]); the original adds it to
// the scaled index (add ecx, [esi+0x224]). Pointer/int/local shapes tried.
i32 Action_SetSide(AISYS_s *sys, AISCRIPTPROCESS_s *process, AIPACKET_s *packet,
                   char **args, int argc, int flags, f32 time) {
  f32 range_squared = 0.0f;
  f32 distance_squared = 0.0f;
  i32 side = 0;
  i32 type_ids[10];
  i32 type_count = 0;
  GameObject_s *object = NULL;
  if (flags != 0) {
    if (packet != NULL && packet->pd0 != NULL && (packet->pd0->flags1fc & 1) &&
        packet->pd0->obj != NULL)
      object = packet->pd0->obj;
    for (i32 i = 0; i < argc; i++) {
      char *value;
      if (NuStrICmp(args[i], "neutral") == 0) {
        side = 0;
      } else if (NuStrICmp(args[i], "goodie") == 0 ||
                 NuStrICmp(args[i], "goody") == 0) {
        side = -1;
      } else if (NuStrICmp(args[i], "baddie") == 0 ||
                 NuStrICmp(args[i], "baddy") == 0) {
        side = 1;
      } else if (NuStrICmp(args[i], "goodiebaddie") == 0 ||
                 NuStrICmp(args[i], "goodybaddy") == 0) {
        side = 2;
      } else if (NuStrICmp(args[i], "default") == 0) {
        side = 3;
      } else if ((value = NuStrIStr(args[i], "type")) != NULL) {
        if (g_unk00ad68ec != NULL && g_unk00ad68fc != NULL) {
          i32 local_type = g_unk00ad68ec(value + 5);
          if (local_type != 0xff) {
            i32 global_type = g_unk00ad68fc(local_type);
            if (global_type != -1 && type_count < 10)
              type_ids[type_count++] = global_type;
          }
        }
      } else if ((value = NuStrIStr(args[i], "character=")) != NULL) {
        object = GetNamedGameObject(sys, value + 10);
      } else if ((value = NuStrIStr(args[i], "range=")) != NULL) {
        range_squared = AIParamToFloat(process, value + 6);
        range_squared *= range_squared;
      }
    }
    if (type_count <= 0 && !(range_squared > 0.0f)) {
      if (object != NULL)
        ActionApplySide(sys, object, side);
      return 1;
    }
    for (i32 i = 0; i < HIGHGAMEOBJECT; i++) {
      GameObject_s *candidate = &Obj[i];
      i32 match = 0;
      if (!(candidate->flags1fc & 1) || !(candidate->flags1fc & 0x1000) ||
          !(candidate->flags1f8 & 0x400))
        continue;
      for (i32 j = 0; j < type_count; j++) {
        if (candidate->type15b0 == type_ids[j])
          match = 1;
      }
      if (range_squared > 0.0f && object != NULL) {
        nuvec_s d;
        NuVecSub(&d, &object->position, &candidate->position);
        distance_squared = d.x * d.x + d.y * d.y + d.z * d.z;
      }
      if ((match == 1 &&
           (distance_squared < range_squared || range_squared == 0.0f)) ||
          (type_count == 0 && distance_squared < range_squared))
        ActionApplySide(sys, candidate, side);
    }
    return 1;
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x00454950
i32 Action_SetStateArea(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                        AIPACKET_s *packet, char **params, i32 param_count,
                        i32 first_time, f32 dt) {
  f32 range_sq = 0.0f;
  char *state_name = NULL;
  i32 types[10];
  i32 type_count = 0;
  nuvec_s origin;
  GameObject_s *src;
  if (!first_time)
    return 1;
  {
    for (i32 index = 0; index < param_count; index++) {
      char *value = NuStrIStr(params[index], "range=");
      if (value != NULL) {
        range_sq = AIParamToFloat(process, value + 6);
        range_sq *= range_sq;
      } else if ((value = NuStrIStr(params[index], "type")) != NULL) {
        if (g_unk00ad68ec != NULL && g_unk00ad68fc != NULL) {
          i32 local_type = g_unk00ad68ec(value + 5);
          if (local_type != 0xff) {
            i32 global_type = g_unk00ad68fc(local_type);
            if (global_type != -1 && type_count < 10)
              types[type_count++] = global_type;
          }
        }
      } else if ((value = NuStrIStr(params[index], "State=")) != NULL) {
        state_name = value + 6;
      }
    }
    if (state_name == NULL || (type_count <= 0 && !(range_sq > 0.0f))) {
      process->next_state = AIStateFind(state_name, process->script);
      return 1;
    }
    if (packet != NULL && packet->pd0 != NULL && packet->pd0->obj != NULL) {
      src = packet->pd0->obj;
      origin = src->v80;
    }
    for (i32 i = 0; i < HIGHGAMEOBJECT; i++) {
      GameObject_s *obj = &Obj[i];
      if (!(obj->flags1fc & 1) || !(obj->flags1fc & 0x1000) ||
          !(obj->flags1f8 & 0x400))
        continue;
      nuvec_s diff;
      NuVecSub(&diff, &origin, &obj->position);
      f32 dist = NuVecMagSqr(&diff);
      if (type_count != 0) {
        i32 match = 0;
        for (i32 j = 0; j < type_count; j++) {
          if (obj->type15b0 == types[j])
            match = 1;
        }
        if (match && (range_sq == 0.0f || dist < range_sq)) {
          ((AISCRIPTPROCESS_s *)obj->p4)->next_state =
              AIStateFind(state_name, ((AISCRIPTPROCESS_s *)obj->p4)->script);
        }
      } else if (range_sq > dist) {
        ((AISCRIPTPROCESS_s *)obj->p4)->next_state =
            AIStateFind(state_name, ((AISCRIPTPROCESS_s *)obj->p4)->script);
      }
    }
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x004556b0
i32 Action_Explode(AISYS_s *sys, AISCRIPTPROCESS_s *process, AIPACKET_s *packet,
                   char **args, int argc, int flags, f32 time) {
  i32 nonlethal = 0;
  i32 type;
  i32 i;
  if (argc) {
    for (i = 0; i < argc; i++)
      if (!NuStrICmp(args[i], "nonlethal"))
        nonlethal = 1;
    if (nonlethal)
      type = 7;
    else
      type = 0x27;
  } else {
    type = 0x27;
  }
  if (packet && packet->pd0)
    Detonate(&packet->pd0->pos5c, type, 1.0f);
  return 1;
}

AISTATE *AIStateFind(char *name, AISCRIPT *script);
void AIScriptProcessorInit(AISYS_s *sys, AISCRIPTPROCESS_s *ai,
                           AISCRIPTPROCESS_s *process, void *a, void *b,
                           void *c, i32 d, AISCRIPT *script, AISTATE *state);

// FUNCTION: LEGOBATMAN 0x00455750
i32 Action_SetScriptState(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                          AIPACKET_s *packet, char **args, int argc, int flags,
                          f32 time) {
  i32 set = 0;
  char *state_name = 0;
  Unk_AIPacketObj *target = 0;
  AISTATE *state;
  char *s;
  i32 i;
  if (flags) {
    if (packet && packet->pd0)
      target = packet->pd0;
    for (i = 0; i < argc; i++) {
      if ((s = NuStrIStr(args[i], "character="))) {
        if (GetNamedAPIObjectFn)
          target = GetNamedAPIObjectFn(sys, s + 10);
      } else if ((s = NuStrIStr(args[i], "set="))) {
        set = (i32)AIParamToFloat(process, s + 4);
        if (set < 0 || set > 16)
          set = 0;
      } else if ((s = NuStrIStr(args[i], "state="))) {
        state_name = s + 6;
      }
    }
    if (state_name != 0) {
      if (set != 0) {
        GameObject_s *object = Obj;
        for (i = 0; i < HIGHGAMEOBJECT; i++, object++) {
          Unk_AIPacketObj *obj = (Unk_AIPacketObj *)object;
          if ((object->flags1fc & 1) && (object->flags1fc & 0x1000) &&
              object->process290[0x344 - 0x290] == set) {
            state = AIStateFind(state_name, obj->ai->base_script);
            if (state != 0) {
              obj->ai->active_ref_count = 0;
              AIScriptProcessorInit(sys, obj->ai, obj->ai, 0, 0, 0, 0,
                                    obj->ai->base_script, state);
            }
          }
        }
      } else if (target && target->ai && target->ai->base_script) {
        state = AIStateFind(state_name, target->ai->base_script);
        if (state != 0) {
          target->ai->active_ref_count = 0;
          AIScriptProcessorInit(sys, target->ai, target->ai, 0, 0, 0, 0,
                                target->ai->base_script, state);
        }
      }
    }
  }
  return 1;
}

int sprintf(char *buf, const char *fmt, ...);
i32 NuRand(void *seed);
f32 NuVecDistSqr(nuvec_s *a, nuvec_s *b, nuvec_s *d);
struct AILOCATOR_s *AIPathFindLocator(AISYS_s *sys, char *name);

// FUNCTION: LEGOBATMAN 0x00456060
i32 Action_SetLocator(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                      AIPACKET_s *packet, char **params, i32 param_count,
                      i32 first_time, f32 dt) {
  f32 best_dist = 1000000000.0f;
  char *name = NULL;
  i32 personal = 0;
  i32 indexed = 0;
  i32 nearest = 0;
  i32 random_count = 0;
  Unk_AIPacketObj *target = NULL;
  AILOCATOR_s *locator = NULL;
  AILOCATOR_s *best = NULL;
  char buf[64];
  nuvec_s d;
  if (!first_time)
    return 1;
  if (packet != NULL && packet->pd0 != NULL)
    target = packet->pd0;
  if (param_count == 0)
    return 1;
  for (i32 index = 0; index < param_count; index++) {
    char *value = NuStrIStr(params[index], "name=");
    if (value != NULL)
      name = value + 5;
    else if (NuStrICmp(params[index], "personal") == 0)
      personal = 1;
    else if (NuStrICmp(params[index], "indexed") == 0)
      indexed = 1;
    else if (NuStrICmp(params[index], "nearest") == 0)
      nearest = 1;
    else if ((value = NuStrIStr(params[index], "random=")) != NULL)
      random_count = (i32)AIParamToFloat(process, value + 7);
    else if ((value = NuStrIStr(params[index], "character=")) != NULL &&
             GetNamedAPIObjectFn != NULL)
      target = GetNamedAPIObjectFn(sys, value + 10);
  }
  if (name != NULL) {
    if (nearest) {
      AILOCATOR_s *loc;
      i32 i = 0;
      do {
        sprintf(buf, "%s_%d", name, i++);
        loc = AIPathFindLocator(sys, buf);
        if (loc != NULL) {
          f32 dist =
              NuVecDistSqr(&packet->pos14c, (nuvec_s *)((u8 *)loc + 0x10), &d);
          if (dist < best_dist) {
            best_dist = dist;
            best = loc;
          }
        }
      } while (loc != NULL);
      locator = best;
    } else if (packet != NULL) {
      if (indexed && packet->pd0->b24c != -1)
        sprintf(buf, "%s_%d", name, (i8)packet->pd0->b24c);
      else if (personal && packet->pd0->character != NULL)
        sprintf(buf, "%s_%s", name, packet->pd0->character->file);
      else if (random_count != 0)
        sprintf(buf, "%s_%d", name, NuRand(0) % random_count);
      else
        sprintf(buf, name);
      locator = AIPathFindLocator(sys, buf);
    }
  }
  if (target != NULL)
    ((AIPACKET_s *)target->ai)->locator = locator;
  return 1;
}

i32 StartBigJump(GameObject_s *obj, nuvec_s *dest, i32 mode, f32 height_scale,
                 f32 speed_scale, i32 anim, i32 flags);

// GLOBAL: LEGOBATMAN 0x00ab3790
extern nuvec_s plr_lastpos;
extern nuvec_s v000;

void InitSurfaceInfo(GameObject_s *obj);
void SetObjOnSurface(GameObject_s *obj, i32 a);
// GLOBAL: LEGOBATMAN 0x00ab3960
extern GameObject_s *Player[8];

// FUNCTION: LEGOBATMAN 0x00456570
i32 Action_SnapToLocator(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                         AIPACKET_s *packet, char **params, i32 param_count,
                         i32 first_time, f32 dt) {
  GameObject_s *obj = NULL;
  i32 personal = 0;
  i32 indexed = 0;
  i32 random_count = 0;
  i32 check_terrain = 1;
  i32 height_only = 0;
  char *name = NULL;
  char buf[64];
  if (first_time) {
    if (packet != NULL && packet->pd0 != NULL && packet->pd0->obj != NULL)
      obj = packet->pd0->obj;
    for (i32 i = 0; i < param_count; i++) {
      char *value;
      if ((value = NuStrIStr(params[i], "name")) != NULL) {
        name = value + 5;
      } else if (NuStrIStr(params[i], "personal") != NULL) {
        personal = 1;
      } else if (NuStrIStr(params[i], "indexed") != NULL) {
        indexed = 1;
      } else if ((value = NuStrIStr(params[i], "random")) != NULL) {
        random_count = (i32)AIParamToFloat(process, value + 9);
      } else if ((value = NuStrIStr(params[i], "character")) != NULL) {
        obj = GetNamedGameObject(sys, value + 10);
      } else if ((value = NuStrIStr(params[i], "player=")) != NULL) {
        i32 player = (i32)AIParamToFloat(process, value + 7) - 1;
        if ((u32)player <= 1)
          obj = Player[player];
      } else if (NuStrIStr(params[i], "dont_check_terrain") != NULL) {
        check_terrain = 0;
      } else if (NuStrIStr(params[i], "height_only") != NULL) {
        height_only = 1;
        check_terrain = 0;
      }
    }
    if (obj == NULL) {
      if (packet == NULL || packet->pd0 == NULL || packet->pd0->obj == NULL)
        return 1;
      obj = packet->pd0->obj;
    }
    if (obj != NULL) {
      AILOCATOR_s *loc = ((AIPACKET_s *)obj->process290)->locator;
      if (name != NULL) {
        if (indexed && obj->b24c != -1)
          sprintf(buf, "%s_%d", name, obj->b24c);
        else if (personal && obj->p54 != NULL)
          sprintf(buf, "%s_%s", name, obj->p54->file);
        else if (random_count != 0)
          sprintf(buf, "%s_%d", name, NuRand(0) % random_count);
        else
          sprintf(buf, name);
        loc = AIPathFindLocator(sys, buf);
      }
      if (loc != NULL) {
        if (height_only)
          obj->position.y = ((nuvec_s *)((u8 *)loc + 0x10))->y;
        else
          obj->position = *(nuvec_s *)((u8 *)loc + 0x10);
        ((AIPACKET_s *)obj->process290)->pos14c = obj->position;
        obj->u246 = *(u16 *)((u8 *)loc + 0x1c);
        obj->yaw58 = *(u16 *)((u8 *)loc + 0x1c);
        obj->facing_angle = *(u16 *)((u8 *)loc + 0x1c);
        *(AIPATHINFO *)((u8 *)obj + 0x3e8) = *(AIPATHINFO *)((u8 *)loc + 0x20);
        obj->v80 = obj->position;
        obj->v98 = obj->position;
        plr_lastpos = obj->v80;
        obj->v8c = obj->v80;
        ((Unk_AIPacketObj *)obj)->respawn_pos = obj->position;
        ((Unk_AIPacketObj *)obj)->last_safe_pos = obj->position;
        obj->saved_position = obj->position;
        obj->velocity = v000;
        InitSurfaceInfo(obj);
        if (check_terrain)
          SetObjOnSurface(obj, 0);
      }
    }
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x00456a90
i32 Action_BigJumpToLocator(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                            AIPACKET_s *packet, char **params, i32 param_count,
                            i32 first_time, f32 dt) {
  f32 jump_factor = 1.0f;
  i32 personal = 0;
  i32 indexed = 0;
  i32 random_count = 0;
  i32 slam = 0;
  char *name = NULL;
  char buf[64];
  GameObject_s *obj;
  if (packet == NULL || packet->pd0 == NULL || packet->pd0->obj == NULL)
    return 1;
  obj = packet->pd0->obj;
  if (first_time) {
    process->action_data_3 = process->locator;
    for (i32 index = 0; index < param_count; index++) {
      char *value = NuStrIStr(params[index], "name");
      if (value != NULL)
        name = value + 5;
      else if (NuStrICmp(params[index], "personal") == 0)
        personal = 1;
      else if (NuStrICmp(params[index], "indexed") == 0)
        indexed = 1;
      else if (NuStrICmp(params[index], "slam") == 0)
        slam = 1;
      else if ((value = NuStrIStr(params[index], "random")) != NULL)
        random_count = (i32)AIParamToFloat(process, value + 7);
      else if ((value = NuStrIStr(params[index], "jump_factor=")) != NULL) {
        jump_factor = AIParamToFloat(process, value + 12);
        if (jump_factor < 0.0f)
          jump_factor = 0.0f;
      }
    }
    if (name != NULL) {
      if (indexed && obj != NULL && obj->b24c != -1)
        sprintf(buf, "%s_%d", name, (i8)obj->b24c);
      else if (personal && obj != NULL && obj->p54 != NULL)
        sprintf(buf, "%s_%s", name, obj->p54->file);
      else if (random_count != 0)
        sprintf(buf, "%s_%d", name, NuRand(0) % random_count);
      else
        sprintf(buf, name);
      process->action_data_3 = AIPathFindLocator(sys, buf);
    }
    AILOCATOR_s *loc = (AILOCATOR_s *)process->action_data_3;
    if (loc != NULL) {
      nuvec_s *pos = (nuvec_s *)((u8 *)loc + 0x10);
      StartBigJump(obj, pos, slam, jump_factor, 1.0f, 0, 1);
      *(AIPATHINFO *)((u8 *)obj + 0x3e8) = *(AIPATHINFO *)((u8 *)loc + 0x20);
      *(nuvec_s *)((u8 *)obj + 0x404) = *pos;
    }
  }
  return obj->b9db != 0x1f;
}

struct Unk_WorldApiObjSys {
  u8 pad0[8];
  unsigned __int64 line_of_sight[64]; // 0x8, bit per object index
  u8 pad208[0x210 - 0x208];
  u32 flags210; // 0x210
};
// GLOBAL: LEGOBATMAN 0x0095f72c
extern u32 g_unk0095f72c;
// GLOBAL: LEGOBATMAN 0x0093b11c
extern f32 direction_scale;

f32 NuVecNorm(nuvec_s *out, nuvec_s *v);
f32 NuRandFloat(void);
void NuVecAdd(nuvec_s *out, nuvec_s *a, nuvec_s *b);
void NuVecScale(nuvec_s *out, nuvec_s *v, f32 s);

f32 NuVecDist(nuvec_s *v0, nuvec_s *v1, nuvec_s *d);
i32 SuperCarry_Carrying(GameObject_s *object);
// GLOBAL: LEGOBATMAN 0x00851344
extern const f32 g_unk00851344; // nearest-player start distance

// GLOBAL: LEGOBATMAN 0x00ab3980
extern GameObject_s *player;
// GLOBAL: LEGOBATMAN 0x00ab3984
extern GameObject_s *player2;
// GLOBAL: LEGOBATMAN 0x0095fd2c
extern nuvec_s v001;
void NuVecRotateY(nuvec_s *v, nuvec_s *v0, i32 a);
nuvec_s *NuSpecialGetDrawPos(nuhspecial_s *special);
i32 NuSpecialExistsFn(nuhspecial_s *special);

// STUB: LEGOBATMAN 0x00457750
// Only diff: in the x/y/z branches the original loads process before the
// lea [len+value+1] (x: edx, y: ecx) or after the push (z); ours always
// computes the lea first. Argument/temporary shapes tried.
i32 Action_SnapToPosition(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                          AIPACKET_s *packet, char **args, int argc, int flags,
                          f32 time) {
  nuvec_s pos = {0.0f, 0.0f, 0.0f};
  i32 check_terrain = 1;
  i32 rotation = 0;
  GameObject_s *obj = NULL;
  nuhspecial_s special = {0};
  WORLDINFO_s *world = WorldInfo_CurrentlyActive();
  if (flags == 0)
    return 1;
  if (packet != NULL && packet->pd0 != NULL && packet->pd0->obj != NULL)
    obj = packet->pd0->obj;
  for (i32 i = 0; i < argc; i++) {
    char *value;
    if (NuStrIStr(args[i], "player1") != NULL ||
        NuStrIStr(args[i], "player") != NULL) {
      obj = player;
    } else if (NuStrIStr(args[i], "player2") != NULL) {
      obj = player2;
    } else if ((value = NuStrIStr(args[i], "roty")) != NULL) {
      value += NuStrLen("roty") + 1;
      rotation =
          (i32)((i32)AIParamToFloat(process, value) * 182.04444444444445);

    } else if (NuStrICmp(args[i], "UseCurrentPos") == 0) {
      if (obj != NULL)
        pos = obj->position;
    } else if (NuStrICmp(args[i], "BehindCamera") == 0) {
      if (obj != NULL) {
        nuvec_s offset = v001;
        pos = *(nuvec_s *)((u8 *)g_unk0095f624 + 0x11c);
        NuVecRotateY(&offset, &offset,
                     *(i32 *)((u8 *)g_unk0095f624 + 0x1fc) + 0x8000);
        NuVecScale(&offset, &offset, 2.0f);
        NuVecAdd(&pos, &offset, &pos);
      }
    } else if ((value = NuStrIStr(args[i], "special_xz")) != NULL) {
      char *name = value + NuStrLen("special_xz") + 1;
      NuSpecialFind(world->scn140, &special, name, 0);
      if (NuSpecialExistsFn(&special)) {
        nuvec_s *p = NuSpecialGetDrawPos(&special);
        pos.x = p->x;
        pos.z = p->z;
      }
    } else if ((value = NuStrIStr(args[i], "special")) != NULL) {
      char *name = value + NuStrLen("special") + 1;
      NuSpecialFind(world->scn140, &special, name, 0);
      if (NuSpecialExistsFn(&special)) {
        nuvec_s *p = NuSpecialGetDrawPos(&special);
        pos.x = p->x;
        pos.y = p->y;
        pos.z = p->z;
      }
    } else if ((value = NuStrIStr(args[i], "x")) != NULL) {
      pos.x = AIParamToFloat(process, value + NuStrLen("x") + 1);
    } else if ((value = NuStrIStr(args[i], "y")) != NULL) {
      pos.y = AIParamToFloat(process, value + NuStrLen("y") + 1);
    } else if ((value = NuStrIStr(args[i], "z")) != NULL) {
      pos.z = AIParamToFloat(process, value + NuStrLen("z") + 1);
    } else if (NuStrIStr(args[i], "dont_check_terrain") != NULL) {
      check_terrain = 0;
    }
  }
  if (obj == NULL)
    return 1;
  obj->position = pos;
  obj->u246 = rotation;
  obj->v80 = obj->position;
  obj->v98 = obj->position;
  obj->u246 = rotation;
  obj->yaw58 = rotation;
  obj->facing_angle = rotation;
  plr_lastpos = obj->v80;
  obj->v8c = obj->v80;
  ((Unk_AIPacketObj *)obj)->respawn_pos = obj->position;
  ((Unk_AIPacketObj *)obj)->last_safe_pos = obj->position;
  obj->saved_position = obj->position;
  obj->velocity = v000;
  InitSurfaceInfo(obj);
  if (check_terrain)
    SetObjOnSurface(obj, 0);
  return 1;
}

// FUNCTION: LEGOBATMAN 0x004586e0
i32 Action_SetOpponent(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                       AIPACKET_s *packet, char **args, int argc, int flags,
                       f32 time) {
  GameObject_s *opponent = NULL;
  if (packet == NULL || packet->pd0 == NULL || packet->pd0->obj == NULL)
    return 1;
  GameObject_s *obj = packet->pd0->obj;
  if (flags == 0)
    return 1;
  GameObject_s *droids[8];
  i32 droid_count = 0;
  // GLOBAL: LEGOBATMAN 0x009c61d0
  static i32 prev_droid_ix;
  for (i32 i = 0; i < argc; i++) {
    char *value;
    if (NuStrIStr(args[i], "opponent=droid") != NULL) {
      for (i32 j = 0; j < 8; j++) {
        GameObject_s *player = Player[j];
        if (player != NULL && (player->flags1fc & 1) &&
            (player->flags1fc & 0x1000) &&
            (player->b257 == 0 || player->f1544 > 0.0f) &&
            !player->not_with_party && (player->p54->model_flags & 0x10))
          droids[droid_count++] = player;
      }
      if (droid_count != 0) {
        prev_droid_ix = (prev_droid_ix + 1) % droid_count;
        opponent = droids[prev_droid_ix];
      }
    } else if (NuStrIStr(args[i], "opponent=nearest_enemy") != NULL) {
      obj->opponent = NULL;
    } else if (NuStrIStr(args[i], "opponent=nearest_player") != NULL) {
      f32 best = g_unk00851344;
      GameObject_s *nearest = NULL;
      for (i32 j = 0; j < 8; j++) {
        GameObject_s *player = Player[j];
        if (player != NULL && (player->flags1fc & 1) &&
            (player->flags1fc & 0x1000) && (player->flags1fc & 0x80) != 0 &&
            (player->b257 == 0 || player->f1544 > 0.0f) &&
            !player->not_with_party) {
          nuvec_s d;
          f32 dist = NuVecDist(&obj->v80, &player->v80, &d);
          if (dist < best) {
            nearest = Player[j];
            best = dist;
          }
        }
      }
      if (nearest != NULL)
        opponent = nearest;
    } else if (NuStrICmp(args[i], "opponent=opponent") == 0) {
      if (packet->pe4 != NULL && packet->pe4->ai != NULL)
        opponent = packet->pe4->obj;
    } else if ((value = NuStrIStr(args[i], "opponent=")) != NULL) {
      opponent = GetNamedGameObject(sys, value + 9);
    } else if (NuStrIStr(args[i], "super_carrying") != NULL) {
      for (i32 j = 0; j < 8; j++) {
        GameObject_s *player = Player[j];
        if (player != NULL && (player->flags1fc & 1) &&
            (player->flags1fc & 0x1000) &&
            (player->b257 == 0 || player->f1544 > 0.0f) &&
            SuperCarry_Carrying(player))
          opponent = Player[j];
      }
    } else if (NuStrICmp(args[i], "last_attacker") == 0) {
      opponent = obj->last_attacker;
    } else if ((value = NuStrIStr(args[i], "opponentType")) != NULL) {
      i32 type = 0xff;
      if (g_unk00ad68ec != NULL && g_unk00ad68fc != NULL) {
        type = g_unk00ad68ec(value + 13);
        if (type != 0xff)
          type = g_unk00ad68fc(type);
      }
      // The original increments the argument index instead of j: it only
      // ever compares Obj[0] (and spins if that does not match).
      for (i32 j = 0; j < HIGHGAMEOBJECT; i++) {
        if (Obj[j].type15b0 == type) {
          opponent = &Obj[j];
          break;
        }
      }
    }
  }
  obj->opponent = opponent;
  return 1;
}

// FUNCTION: LEGOBATMAN 0x0045a990
i32 Action_FollowDirection(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                           AIPACKET_s *packet, char **params, i32 param_count,
                           i32 first_time, f32 dt) {
  AILOCATOR_s *start = NULL;
  AILOCATOR_s *end = NULL;
  GameObject_s *obj;
  f32 random;
  nuvec_s dest;
  nuvec_s diff;
  if (packet == NULL || packet->pd0 == NULL || packet->pd0->obj == NULL)
    return 1;
  obj = packet->pd0->obj;
  if (first_time) {
    process->action_pos.x = 0.0f;
    process->action_pos.y = 0.0f;
    process->action_pos.z = 1.0f;
    random = NuRandFloat();
    for (i32 i = 0; i < param_count; i++) {
      char *value;
      if ((value = NuStrIStr(params[i], "start=")) != NULL)
        start = AIPathFindLocator(sys, value + 6);
      else if ((value = NuStrIStr(params[i], "end=")) != NULL)
        end = AIPathFindLocator(sys, value + 4);
      else if ((value = NuStrIStr(params[i], "firerange")) != NULL)
        process->follow_direction_fire_range =
            AIParamToFloat(process, value + 10);
      else if ((value = NuStrIStr(params[i], "fireinterval")) != NULL)
        process->follow_direction_fire_interval =
            AIParamToFloat(process, value + 13);
    }
    if (start != NULL && end != NULL) {
      NuVecSub(&process->action_pos, (nuvec_s *)((u8 *)end + 0x10),
               (nuvec_s *)((u8 *)start + 0x10));
      NuVecNorm(&process->action_pos, &process->action_pos);
    }
    process->action_timer = process->follow_direction_fire_interval * random;
    NuVecScale(&process->action_pos, &process->action_pos, direction_scale);
  }
  NuVecAdd(&dest, &obj->position, &process->action_pos);
  AIMoveInstruction(packet, &dest, 0.0f, NULL, 1, 0.0f);
  if (process->follow_direction_fire_range > 0.0f) {
    Unk_AIPacketObj *opp = packet->pe4;
    if (opp != NULL && opp->ai != NULL && opp->obj != NULL) {
      f32 dist = NuVecDistSqr(&packet->pd0->pos5c, &opp->pos5c, &diff);
      if ((g_unk00960894->api_object_sys->line_of_sight[obj->b259] &
           ((unsigned __int64)1 << packet->pe4->b259)) != 0 &&
          obj->b254 != 0 &&
          dist < process->follow_direction_fire_range *
                     process->follow_direction_fire_range) {
        if (packet->pe4->b257 == 0 || packet->pe4->obj->f1544 > 0.0f) {
          process->action_timer -= dt;
          if (packet->pe4->b257 == 0 && process->action_timer <= 0.0f) {
            process->action_timer =
                process->follow_direction_fire_interval * 0.5 +
                NuRandFloat() * process->follow_direction_fire_interval;
            obj->p112c->buttons8 |= g_unk0095f72c;
            obj->fire_target13b8 = packet->pe4->obj;
          }
        }
      }
    }
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x0045af10
i32 Action_SetIgnorePlayerItemsTargetting(AISYS_s *sys,
                                          AISCRIPTPROCESS_s *process,
                                          AIPACKET_s *packet, char **args,
                                          int argc, int flags, f32 time) {
  GameObject_s *obj = 0;
  u32 ignore = 0;
  i32 reset = 0;
  Unk_AIPacketObj *api;
  char *s;
  i32 i;
  if (flags) {
    if (packet && packet->pd0 && packet->pd0->obj)
      obj = packet->pd0->obj;
    for (i = 0; i < argc; i++) {
      if ((s = NuStrIStr(args[i], "character="))) {
        if (GetNamedAPIObjectFn && (api = GetNamedAPIObjectFn(sys, s + 10)))
          obj = api->obj;
        else
          obj = 0;
      } else if (NuStrIStr(args[i], "RESET")) {
        reset = 1;
      } else if ((s = NuStrIStr(args[i], "ignore="))) {
        if (!NuStrICmp(s + 7, "all"))
          ignore |= 1;
      }
    }
    if (reset == 1)
      obj->flags1430 = ignore;
    else if (obj)
      obj->flags1430 |= ignore;
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x0045b070
i32 Action_SetInvulnerable(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                           AIPACKET_s *packet, char **args, int argc, int flags,
                           f32 time) {
  u32 except = 0;
  i32 enabled = 1;
  i32 type_count = 0;
  i32 still_take_hit = 0;
  GameObject_s *obj = NULL;
  i32 types[10];
  if (flags == 0)
    return 1;
  if (packet != NULL && packet->pd0 != NULL && packet->pd0->obj != NULL)
    obj = packet->pd0->obj;
  for (i32 i = 0; i < argc; i++) {
    char *value = NuStrIStr(args[i], "type");
    if (value != NULL) {
      if (g_unk00ad68ec != NULL && g_unk00ad68fc != NULL) {
        i32 local_type = g_unk00ad68ec(value + 5);
        if (local_type != 0xff) {
          i32 global_type = g_unk00ad68fc(local_type);
          if (global_type != -1 && type_count < 10)
            types[type_count++] = global_type;
        }
      }
    } else if ((value = NuStrIStr(args[i], "character=")) != NULL) {
      obj = GetNamedGameObjectInl(sys, value + 10);
    } else if ((value = NuStrIStr(args[i], "except=")) != NULL) {
      value += 7;
      if (NuStrICmp(value, "ALL") == 0)
        except = 0xffffffff;
      else if (NuStrICmp(value, "HIT_BY_AI") == 0)
        except |= 2;
      else if (NuStrICmp(value, "HIT_BY_PLAYER") == 0)
        except |= 4;
      else if (NuStrICmp(value, "HIT_BY_GOODY") == 0)
        except |= 8;
      else if (NuStrICmp(value, "HIT_BY_BADDY") == 0)
        except |= 0x10;
      else if (NuStrICmp(value, "HIT_BY_NEUTRAL") == 0)
        except |= 0x20;
      else if (NuStrICmp(value, "PUNCH") == 0)
        except |= 0x40;
      else if (NuStrICmp(value, "BOLT") == 0)
        except |= 0x80;
      else if (NuStrICmp(value, "STREAK") == 0)
        except |= 0x100;
      else if (NuStrICmp(value, "SLAM") == 0)
        except |= 0x200;
      else if (NuStrICmp(value, "ZAP") == 0)
        except |= 0x400;
      else if (NuStrICmp(value, "BASH") == 0)
        except |= 0x2000;
      else if (NuStrICmp(value, "HITBYWHIP") == 0)
        except |= 0x8000;
      else if (NuStrICmp(value, "HITBYTHROWNITEM") == 0)
        except |= 0x20000;
      else if (NuStrICmp(value, "TORPEDO") == 0)
        except |= 0x100000;
      else if (NuStrICmp(value, "DRAGBOMB") == 0)
        except |= 0x200000;
    } else if ((value = NuStrIStr(args[i], "include=")) != NULL) {
      value += 8;
      if (NuStrICmp(value, "HIT_BY_AI") == 0)
        except &= ~2;
      else if (NuStrICmp(value, "HIT_BY_PLAYER") == 0)
        except &= ~4;
      else if (NuStrICmp(value, "HIT_BY_GOODY") == 0)
        except &= ~8;
      else if (NuStrICmp(value, "HIT_BY_BADDY") == 0)
        except &= ~0x10;
      else if (NuStrICmp(value, "HIT_BY_NEUTRAL") == 0)
        except &= ~0x20;
      else if (NuStrICmp(value, "PUNCH") == 0)
        except &= ~0x40;
      else if (NuStrICmp(value, "BOLT") == 0)
        except &= ~0x80;
      else if (NuStrICmp(value, "STREAK") == 0)
        except &= ~0x100;
      else if (NuStrICmp(value, "SLAM") == 0)
        except &= ~0x200;
      else if (NuStrICmp(value, "ZAP") == 0)
        except &= ~0x400;
      else if (NuStrICmp(value, "BASH") == 0)
        except &= ~0x2000;
      else if (NuStrICmp(value, "HITBYWHIP") == 0)
        except &= ~0x8000;
      else if (NuStrICmp(value, "HITBYTHROWNITEM") == 0)
        except &= ~0x20000;
      else if (NuStrICmp(value, "TORPEDO") == 0)
        except &= ~0x100000;
      else if (NuStrICmp(value, "DRAGBOMB") == 0)
        except &= ~0x200000;
    } else if (NuStrICmp(args[i], "FALSE") == 0) {
      enabled = 0;
    } else if (NuStrICmp(args[i], "still_do_take_hit_anim") == 0) {
      still_take_hit = 1;
    }
  }
  if (type_count != 0) {
    GameObject_s *candidate = Obj;
    for (i32 i = 0; i < HIGHGAMEOBJECT; i++, candidate++) {
      if (!(candidate->flags1fc & 1) || !(candidate->flags1fc & 0x1000))
        continue;
      for (i32 j = 0; j < type_count; j++) {
        if (candidate->type15b0 == types[j]) {
          candidate->invulnerable_except = except;
          if (enabled) {
            candidate->invulnerable = 1;
            candidate->still_do_take_hit_anim = still_take_hit;
          } else {
            candidate->invulnerable = 0;
          }
        }
      }
    }
  } else if (obj != NULL) {
    obj->invulnerable_except = except;
    if (enabled) {
      obj->invulnerable = 1;
      obj->still_do_take_hit_anim = still_take_hit;
    } else {
      obj->invulnerable = 0;
    }
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x0045b7e0
i32 Action_DontAvoidCharacter(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                              AIPACKET_s *packet, char **args, int argc,
                              int flags, f32 time) {
  i32 enabled = 1;
  GameObject_s *dont_avoid = 0;
  GameObject_s *obj = 0;
  Unk_AIPacketObj *api;
  char *s;
  i32 i;
  if (flags) {
    if (packet && packet->pd0 && packet->pd0->obj)
      obj = packet->pd0->obj;
    for (i = 0; i < argc; i++) {
      if (NuStrIStr(args[i], "character=myself")) {
      } else if ((s = NuStrIStr(args[i], "character="))) {
        if (GetNamedAPIObjectFn && (api = GetNamedAPIObjectFn(sys, s + 10)))
          obj = api->obj;
        else
          obj = 0;
      } else if (NuStrIStr(args[i], "dont_avoid=myself")) {
        dont_avoid = obj;
      } else if ((s = NuStrIStr(args[i], "dont_avoid="))) {
        if (GetNamedAPIObjectFn && (api = GetNamedAPIObjectFn(sys, s + 11)))
          dont_avoid = api->obj;
        else
          dont_avoid = 0;
      } else if (!NuStrICmp(args[i], "FALSE")) {
        enabled = 0;
      }
    }
    if (obj && dont_avoid)
      *(GameObject_s **)(obj->process290 + 0xf4) = enabled ? dont_avoid : 0;
  }
  return 1;
}

struct GIZAIMESSAGE_s {
  u8 pad0[0x28];
  f32 value; // 0x28
};

extern GIZAIMESSAGESYS_s *gizaimessagesys;
i32 qrand(void);
void LoseHitPoints(GameObject_s *obj, i32 amount);

float SeekValF(float current, float target, float step);

static inline f32 RunSpeedFabs(f32 f) {
  u32 bits = *(u32 *)&f & 0x7fffffff;
  return *(f32 *)&bits;
}

// FUNCTION: LEGOBATMAN 0x0045d520
i32 Action_SetRunSpeed(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                       AIPACKET_s *packet, char **args, int argc, int flags,
                       f32 time) {
  f32 multiplier = 1.0f;
  f32 maximum = 1000000000.0f;
  i32 multiply = 0;
  f32 minimum = 0.0f;
  if (packet == NULL || packet->pd0 == NULL || packet->pd0->obj == NULL)
    return 1;
  GameObject_s *obj = packet->pd0->obj;
  if (flags != 0) {
    f32 speed = 1000000000.0f;
    if (argc != 0)
      for (i32 i = 0; i < argc; i++) {
        char *value = NuStrIStr(args[i], "multiply=");
        if (value != NULL) {
          multiplier = AIParamToFloat(process, value + 9);
          multiply = 1;
        } else if (NuStrICmp(args[i], "max=default") == 0)
          maximum = obj->p54->p24->run_speed;
        else if (NuStrICmp(args[i], "default") == 0)
          speed = obj->p54->p24->run_speed;
        else if (NuStrICmp(args[i], "clear") == 0) {
        } else if ((value = NuStrIStr(args[i], "max=")) != NULL)
          maximum = AIParamToFloat(process, value + 4);
        else if ((value = NuStrIStr(args[i], "min=")) != NULL)
          minimum = AIParamToFloat(process, value + 4);
        else if ((value = NuStrIStr(args[i], "seek=")) != NULL)
          process->action_data_4 = AIParamToFloat(process, value + 5);
        else if (NuStrIStr(args[i], "player_run_speed") != NULL)
          speed = player->p54->p24->run_speed;
        else
          speed = AIParamToFloat(process, args[i]);
      }
    minimum = RunSpeedFabs(minimum);
    maximum = RunSpeedFabs(maximum);
    if (process->action_data_4 == 0.0f) {
      if (multiply) {
        if (obj->run_speed_override == 1000000000.0f)
          obj->run_speed_override = obj->p54->p24->run_speed;
        obj->run_speed_override = multiplier * obj->run_speed_override;
        if (obj->run_speed_override < 0.0f) {
          if (obj->run_speed_override < -maximum)
            obj->run_speed_override = -maximum;
          else if (maximum > minimum && obj->run_speed_override > -minimum)
            obj->run_speed_override = -minimum;
        } else {
          if (obj->run_speed_override > maximum)
            obj->run_speed_override = maximum;
          else if (maximum > minimum && obj->run_speed_override < minimum)
            obj->run_speed_override = minimum;
        }
      } else
        obj->run_speed_override = speed;
      return 1;
    }
    if (multiply) {
      if (obj->run_speed_override == 1000000000.0f)
        process->action_data_5 = obj->p54->p24->run_speed;
      process->action_data_5 *= multiplier;
      if (process->action_data_5 < 0.0f) {
        if (process->action_data_5 < -maximum)
          process->action_data_5 = -maximum;
        else if (maximum > minimum && process->action_data_5 > -minimum)
          process->action_data_5 = -minimum;
      } else {
        if (process->action_data_5 > maximum)
          process->action_data_5 = maximum;
        else if (maximum > minimum && process->action_data_5 < minimum)
          process->action_data_5 = minimum;
      }
    } else
      process->action_data_5 = speed;
  }
  if (process->action_data_4 > 0.0f &&
      obj->run_speed_override < 1000000000.0f) {
    obj->run_speed_override =
        SeekValF(obj->run_speed_override, process->action_data_5,
                 process->action_data_4);
    if (RunSpeedFabs(obj->run_speed_override - process->action_data_5) <
        0.01f) {
      obj->run_speed_override = process->action_data_5;
      return 1;
    }
    return 0;
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x0045dc80
i32 Action_SetHitPoints(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                        AIPACKET_s *packet, char **params, i32 param_count,
                        i32 first_time, f32 dt) {
  GameObject_s *obj = NULL;
  i32 set_max = 1;
  i32 hp = -1;
  i32 min = -1;
  i32 max = -1;
  i32 decrement = 0;
  i32 max_hp = -1;
  if (!first_time)
    return 1;
  if (packet != NULL && packet->pd0 != NULL)
    obj = packet->pd0->obj;
  for (i32 i = 0; i < param_count; i++) {
    char *value;
    if ((value = NuStrIStr(params[i], "character=")) != NULL) {
      value += NuStrLen("character=");
      Unk_AIPacketObj *api;
      if (GetNamedAPIObjectFn != NULL &&
          (api = GetNamedAPIObjectFn(sys, value)) != NULL)
        obj = api->obj;
      else
        obj = NULL;
    } else if ((value = NuStrIStr(params[i], "messageval=")) != NULL) {
      value += NuStrLen("messageval=");
      GIZAIMESSAGE_s *msg = CheckGizAIMessage(gizaimessagesys, value, NULL);
      if (msg != NULL)
        hp = (i32)msg->value;
    } else if ((value = NuStrIStr(params[i], "decrement=")) != NULL) {
      value += NuStrLen("decrement=");
      decrement = (i32)AIParamToFloat(process, value);
    } else if ((value = NuStrIStr(params[i], "max_hit_points=")) != NULL) {
      value += NuStrLen("max_hit_points=");
      max_hp = (i32)AIParamToFloat(process, value);
    } else if (NuStrICmp(params[i], "dont_set_max") == 0) {
      set_max = 0;
    } else if ((value = NuStrIStr(params[i], "min")) != NULL) {
      value += NuStrLen("min") + 1;
      min = (i32)AIParamToFloat(process, value);
    } else if ((value = NuStrIStr(params[i], "max")) != NULL) {
      value += NuStrLen("max") + 1;
      max = (i32)AIParamToFloat(process, value);
    } else if (NuStrICmp(params[i], "default") != 0) {
      hp = (i32)AIParamToFloat(process, params[i]);
    }
  }
  if (obj != NULL) {
    i32 lose = 0;
    if (decrement != 0) {
      i32 cur = obj->current_hp;
      hp = cur - decrement;
      if (hp < 0)
        hp = 0;
      lose = cur - hp;
    } else if (min != -1 && max != -1 && max >= min) {
      hp = qrand() / (0xffff / (max - min + 1) + 1) + min;
    } else if (hp == -1 && max_hp == -1) {
      hp = obj->p54->p24->b226;
    }
    if (max_hp >= 0)
      obj->hitpoints = max_hp;
    else if (set_max)
      obj->hitpoints = hp;
    if (hp >= 0) {
      if (lose != 0) {
        LoseHitPoints(obj, lose);
        return 1;
      }
      obj->current_hp = hp;
    }
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x0045e190
i32 Action_SetMessage(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                      AIPACKET_s *packet, char **params, i32 param_count,
                      i32 first_time, f32 dt) {
  f32 amount = 0.0f;
  i32 op = 0;
  char *name = NULL;
  if (first_time && gizaimessagesys != NULL && param_count > 0) {
    for (i32 i = 0; i < param_count; i++) {
      char *value;
      if ((value = NuStrIStr(params[i], "name=")) != NULL) {
        name = value + NuStrLen("name=");
      } else if (NuStrICmp(params[i], "value=hitpoints") == 0) {
        if (packet != NULL && packet->pd0 != NULL)
          amount = packet->pd0->obj->current_hp;
      } else if ((value = NuStrIStr(params[i], "value=")) != NULL) {
        value += NuStrLen("value=");
        amount = AIParamToFloat(process, value);
      } else if ((value = NuStrIStr(params[i], "increment=")) != NULL) {
        value += NuStrLen("increment=");
        amount = AIParamToFloat(process, value);
        op = 1;
      } else if ((value = NuStrIStr(params[i], "decrement=")) != NULL) {
        value += NuStrLen("decrement=");
        amount = AIParamToFloat(process, value);
        op = -1;
      } else if ((value = NuStrIStr(params[i], "bit=")) != NULL) {
        value += NuStrLen("bit=");
        amount = AIParamToFloat(process, value);
        op = 2;
      } else if (NuStrICmp(params[i], "negate") == 0) {
        op = 3;
      }
    }
    if (name != NULL) {
      GIZAIMESSAGE_s *msg = CheckGizAIMessage(gizaimessagesys, name, NULL);
      if (msg != NULL) {
        switch (op) {
        case -1:
          msg->value -= amount;
          break;
        case 0:
          msg->value = amount;
          break;
        case 1:
          msg->value = msg->value + amount;
          break;
        case 2: {
          i32 bit = (i32)amount - 1;
          if ((u32)bit <= 31)
            msg->value = (f32)((1 << bit) | (i32)msg->value);
          break;
        }
        case 3:
          msg->value = -msg->value;
          break;
        }
      }
    }
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x0045e500
i32 Action_SetScriptParam(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                          AIPACKET_s *packet, char **args, int argc, int flags,
                          f32 time) {
  f32 amount = 0.0f;
  i32 operation = 0;
  i32 index = -1;
  if (flags && argc != 0 && process->script != 0) {
    for (i32 i = 0; i < argc; i++) {
      char *value = NuStrIStr(args[i], "name=");
      if (value != 0) {
        value += 5;
        for (i32 j = 0; j < 4; j++) {
          if (NuStrICmp(process->script->params[j].name, value) == 0) {
            index = j;
            break;
          }
        }
      } else if ((value = NuStrIStr(args[i], "ix=")) != 0) {
        index = (i32)AIParamToFloat(process, value + 3);
        if (index >= 4)
          index = -1;
      } else if ((value = NuStrIStr(args[i], "value=")) != 0) {
        amount = AIParamToFloat(process, value + 6);
      } else if ((value = NuStrIStr(args[i], "increment=")) != 0) {
        amount = AIParamToFloat(process, value + 10);
        operation = 1;
      } else if ((value = NuStrIStr(args[i], "decrement=")) != 0) {
        amount = AIParamToFloat(process, value + 10);
        operation = -1;
      }
    }
    if (index >= 0) {
      switch (operation) {
      case 0:
        process->params[index] = amount;
        break;
      case 1:
        process->params[index] = process->params[index] + amount;
        break;
      case -1:
        process->params[index] -= amount;
        break;
      }
    }
  }
  return 1;
}

// GLOBAL: LEGOBATMAN 0x009c596c
extern f32 DEFAULT_MOVE_RANGE;

i32 PARTLookupType(char *name);
i32 AddFiniteShotPART(i32 effect, nuvec_s *position, i32 count);
void NuVecAdd(nuvec_s *out, nuvec_s *a, nuvec_s *b);

// FUNCTION: LEGOBATMAN 0x0045eac0
i32 Action_AddPartDebris(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                         AIPACKET_s *packet, char **params, i32 param_count,
                         i32 first_time, f32 dt) {
  nuvec_s offset;
  nuvec_s pos;
  i32 type = -1;
  GameObject_s *obj = NULL;
  offset.x = 0.0f;
  offset.y = 0.0f;
  offset.z = 0.0f;
  if (first_time) {
    for (i32 i = 0; i < param_count; i++) {
      char *value;
      if ((value = NuStrIStr(params[i], "name")) != NULL)
        type = PARTLookupType(value + 5);
      else if ((value = NuStrIStr(params[i], "dx=")) != NULL)
        offset.x = AIParamToFloat(process, value + 3);
      else if ((value = NuStrIStr(params[i], "dy=")) != NULL)
        offset.y = AIParamToFloat(process, value + 3);
      else if ((value = NuStrIStr(params[i], "dz=")) != NULL)
        offset.z = AIParamToFloat(process, value + 3);
      else if ((value = NuStrIStr(params[i], "x=")) != NULL)
        pos.x = AIParamToFloat(process, value + 2);
      else if ((value = NuStrIStr(params[i], "y=")) != NULL)
        pos.y = AIParamToFloat(process, value + 2);
      else if ((value = NuStrIStr(params[i], "z=")) != NULL)
        pos.z = AIParamToFloat(process, value + 2);
      else if ((value = NuStrIStr(params[i], "character=")) != NULL) {
        obj = GetNamedGameObject(sys, value + 10);
        if (obj != NULL)
          pos = obj->v80;
      } else if (NuStrIStr(params[i], "me") != NULL) {
        if (packet != NULL && packet->pd0 != NULL && packet->pd0->obj != NULL)
          obj = packet->pd0->obj;
        pos = obj->v80;
      } else if ((value = NuStrIStr(params[i], "locator=")) != NULL) {
        AILOCATOR_s *loc = AIPathFindLocator(sys, value + 8);
        if (loc != NULL)
          pos = *(nuvec_s *)((u8 *)loc + 0x10);
      }
    }
    if (type != -1) {
      NuVecAdd(&pos, &pos, &offset);
      AddFiniteShotPART(type, &pos, 1);
    }
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x0045f9e0
i32 Action_SetMaxMovementRange(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                               AIPACKET_s *packet, char **args, int argc,
                               int flags, f32 time) {
  f32 range = 0.0f;
  i32 all_non_party = 0;
  i32 range_type = 1;
  if (flags) {
    for (i32 i = 0; i < argc; i++) {
      if (NuStrICmp("Default", args[i]) == 0) {
        range = DEFAULT_MOVE_RANGE;
      } else if (NuStrICmp("All_Non_Party", args[i]) == 0) {
        all_non_party = 1;
      } else if (NuStrICmp("Locator", args[i]) == 0) {
        range_type = 2;
      } else {
        range = AIParamToFloat(process, args[i]);
      }
    }
    if (all_non_party) {
      GameObject_s *object = Obj;
      for (i32 i = 0; i < HIGHGAMEOBJECT; i++, object++) {
        if ((object->flags1fc & 1) && (object->flags1fc & 0x1000) &&
            (object->flags1f8 & 0x400)) {
          object->move_range = range;
          if (range > 0.0f)
            object->move_range_type = 1;
          else
            object->move_range_type = 0;
        }
      }
    } else if (packet && packet->pd0 && packet->pd0->obj) {
      GameObject_s *object = packet->pd0->obj;
      object->move_range = range;
      if (range > 0.0f)
        object->move_range_type = range_type;
      else
        object->move_range_type = 0;
    }
  }
  return 1;
}

struct SetPathAIPath_s {
  char name[1];
};

struct SetPathAIPathSys_s {
  u8 path_count;                // 0x00
  SetPathAIPath_s **paths;      // 0x04
  SetPathAIPath_s *active_path; // 0x08
};

struct SetPathAISys_s {
  u8 pad0[0x21c];
  SetPathAIPathSys_s *path_sys; // 0x21c
};

void AISysCharacterSetPath(void *ai, SetPathAIPath_s *path);
void AISysGetCharacterPathPos(AISYS_s *sys, GameObject_s *obj, void *ai, i32 a,
                              i32 b);

struct GIZMO_s {
  void *object; // 0x00
};

GIZMO_s *GizmoFindByName(GIZMOSYS_s *gizmo_sys, i32 type_id, char *name);
// GLOBAL: LEGOBATMAN 0x0095fc30
extern i32 grapple_gizmotype_id;
// GLOBAL: LEGOBATMAN 0x00961394
extern i32 ledge_gizmotype_id;

struct AIPATHCNXHELPER_s {
  u8 pad0[0x10];
  f32 jump_off_dy; // 0x10
};

struct AIPATH_s *AISysFindPath(AISYS_s *sys, char *name);
void *AIPAthFindPathCnx(AISYS_s *sys, struct AIPATH_s *path, char *from,
                        char *to, i32 *dir);
AIPATHCNXHELPER_s *AIPathCnxHelperSys_AddHelper(void *sys, void *cnx, u8 dir,
                                                void *grapple, u8 a);
void AddClimbHelper(WORLDINFO_s *world, void *cnx, AILOCATOR_s *locator);
void AddLedgeHelper(WORLDINFO_s *world, void *cnx, i32 dir, void *ledge1,
                    void *ledge2, i32 ledge_dir);

// FUNCTION: LEGOBATMAN 0x00461af0
i32 Action_CnxHelper(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                     AIPACKET_s *packet, char **params, i32 param_count,
                     i32 first_time, f32 dt) {
  f32 jump_off_dy = 0.0f;
  char *to = NULL;
  i32 both = 0;
  char *from = NULL;
  struct AIPATH_s *path = NULL;
  void *grapple = NULL;
  void *ledge1 = NULL;
  void *ledge2 = NULL;
  AILOCATOR_s *climb = NULL;
  i32 ledge_dir = -1;
  if (!first_time)
    return 1;
  for (i32 i = 0; i < param_count; i++) {
    char *value;
    if ((value = NuStrIStr(params[i], "pathname=")) != NULL) {
      path = AISysFindPath(sys, value + 9);
    } else if (NuStrIStr(params[i], "bothways") != NULL) {
      both = 1;
    } else if ((value = NuStrIStr(params[i], "from=")) != NULL) {
      from = value + NuStrLen("from=");
    } else if ((value = NuStrIStr(params[i], "to=")) != NULL) {
      to = value + NuStrLen("to=");
    } else if ((value = NuStrIStr(params[i], "jump_off_dy=")) != NULL) {
      value += NuStrLen("jump_off_dy=");
      jump_off_dy = AIParamToFloat(process, value);
    } else if ((value = NuStrIStr(params[i], "grapple=")) != NULL) {
      value += NuStrLen("grapple=");
      GIZMO_s *gizmo = GizmoFindByName(g_unk00960894->gizmoSys2b0c,
                                       grapple_gizmotype_id, value);
      if (gizmo != NULL)
        grapple = gizmo->object;
    } else if ((value = NuStrIStr(params[i], "ledge1")) != NULL) {
      value += NuStrLen("ledge1") + 1;
      GIZMO_s *gizmo = GizmoFindByName(g_unk00960894->gizmoSys2b0c,
                                       ledge_gizmotype_id, value);
      if (gizmo != NULL)
        ledge1 = gizmo->object;
    } else if ((value = NuStrIStr(params[i], "ledge2")) != NULL) {
      value += NuStrLen("ledge2") + 1;
      GIZMO_s *gizmo = GizmoFindByName(g_unk00960894->gizmoSys2b0c,
                                       ledge_gizmotype_id, value);
      if (gizmo != NULL)
        ledge2 = gizmo->object;
    } else if ((value = NuStrIStr(params[i], "ledge_dir")) != NULL) {
      value += NuStrLen("ledge_dir") + 1;
      ledge_dir = (i32)AIParamToFloat(process, value);
    } else if ((value = NuStrIStr(params[i], "climb_locator=")) != NULL) {
      value += NuStrLen("climb_locator=");
      climb = AIPathFindLocator(sys, value);
    }
  }
  if (from != NULL && to != NULL) {
    void *cnx = AIPAthFindPathCnx(sys, path, from, to, &first_time);
    if (cnx != NULL) {
      if (both)
        first_time = 0xff;
      if (grapple != NULL) {
        AIPATHCNXHELPER_s *helper = AIPathCnxHelperSys_AddHelper(
            g_unk00960894->ai_path_cnx_helper_sys, cnx, first_time, grapple, 1);
        if (helper != NULL)
          helper->jump_off_dy = jump_off_dy;
      } else if (climb != NULL) {
        AddClimbHelper(g_unk00960894, cnx, climb);
      } else if (ledge1 != NULL || ledge2 != NULL) {
        first_time = 0xff;
        AddLedgeHelper(g_unk00960894, cnx, first_time, ledge1, ledge2,
                       ledge_dir);
      }
    }
  }
  return 1;
}

// STUB: LEGOBATMAN 0x00461ff0
// body right; orig keeps three separate early-return epilogues, ours
// tail-merges them (and loads argc into eax before the loop).
// Nesting the three checks (if (flags) { if (sys) { if (path_sys) {...}}})
// gives three separate epilogues but in inner-first order.
i32 Action_SetPath(AISYS_s *sys, AISCRIPTPROCESS_s *process, AIPACKET_s *packet,
                   char **args, int argc, int flags, f32 time) {
  SetPathAIPath_s *path = 0;
  GameObject_s *obj = 0;
  if (flags == 0)
    return 1;
  if (sys == 0)
    return 1;
  SetPathAIPathSys_s *path_sys = ((SetPathAISys_s *)sys)->path_sys;
  if (path_sys == 0)
    return 1;
  if (packet && packet->pd0)
    obj = packet->pd0->obj;
  for (i32 i = 0; i < argc; i++) {
    char *value = NuStrIStr(args[i], "character=");
    if (value != 0) {
      obj = GetNamedGameObjectInl(sys, value + 10);
    } else if (NuStrIStr(args[i], "path=LevelPath") != 0) {
      path = ((SetPathAISys_s *)sys)->path_sys->active_path;
    } else if ((value = NuStrIStr(args[i], "path")) != 0) {
      value += 5;
      for (i32 j = 0; j < path_sys->path_count; j++) {
        if (NuStrICmp(path_sys->paths[j]->name, value) == 0) {
          path = path_sys->paths[j];
          break;
        }
      }
    }
  }
  if (obj != 0 && path != 0) {
    AISysCharacterSetPath(obj->process290, path);
    AISysGetCharacterPathPos(g_unk00960894->aiSys2bf8, obj, obj->process290,
                             0xff, 1);
  }
  return 1;
}

i32 NuRand(void *seed);
int sprintf(char *buf, const char *fmt, ...);

// STUB: LEGOBATMAN 0x00457460
// one diff: orig loads argc into eax for the loop-entry test (mov eax, argc;
// cmp eax, esi), ours compares memory directly. Same as Action_SetPath.
i32 Action_SetDoomedEscapeLocator(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                  AIPACKET_s *packet, char **args, int argc,
                                  int flags, f32 time) {
  char *name = 0;
  GameObject_s *obj = 0;
  i32 personal = 0;
  i32 indexed = 0;
  i32 random_count = 0;
  i32 take_damage = 0;
  char *s;
  if (flags) {
    if (packet && packet->pd0 && packet->pd0->obj)
      obj = packet->pd0->obj;
    for (i32 i = 0; i < argc; i++) {
      if ((s = NuStrIStr(args[i], "character=")))
        obj = GetNamedGameObjectInl(sys, s + 10);
      else if ((s = NuStrIStr(args[i], "name")))
        name = s + 5;
      else if (NuStrIStr(args[i], "personal"))
        personal = 1;
      else if (NuStrIStr(args[i], "indexed"))
        indexed = 1;
      else if (NuStrIStr(args[i], "take_damage"))
        take_damage = 1;
      else if ((s = NuStrIStr(args[i], "random")))
        random_count = (i32)AIParamToFloat(process, s + 9);
    }
    if (obj) {
      obj->doomed_take_damage = 0;
      obj->doomed_escape_locator = 0;
      if (name) {
        char locator_name[64];
        if (indexed && obj->b24c != -1)
          sprintf(locator_name, "%s_%d", name, obj->b24c);
        else if (personal && obj->p54 != 0)
          sprintf(locator_name, "%s_%s", name, obj->p54->file);
        else if (random_count != 0)
          sprintf(locator_name, "%s_%d", name, NuRand(0) % random_count);
        else
          sprintf(locator_name, name);
        obj->doomed_escape_locator = AIPathFindLocator(sys, locator_name);
        if (obj->doomed_escape_locator)
          obj->doomed_take_damage = take_damage;
      }
    }
  }
  return 1;
}

struct GIZOBSTACLE_s {
  u8 pad0[0xc8];
  u32 flags_c8_lo : 13;
  u32 stay_open : 1; // 0xc8 bit 13
  u32 stay_shut : 1; // 0xc8 bit 14
  u32 flags_c8_hi : 17;
};

// GLOBAL: LEGOBATMAN 0x0095ff24
extern i32 obstacle_gizmotype_id;

void GizObstacle_JumpToEnd(GIZOBSTACLE_s *obstacle);
void GizObstacle_PlayForwards(GIZOBSTACLE_s *obstacle);
void GizObstacle_JumpToStart(GIZOBSTACLE_s *obstacle);
void GizObstacle_PlayBackwards(GIZOBSTACLE_s *obstacle);

// FUNCTION: LEGOBATMAN 0x00462e90
i32 Action_PlayGizObstacle(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                           AIPACKET_s *packet, char **args, int argc, int flags,
                           f32 time) {
  i32 stay_shut = 0;
  GIZOBSTACLE_s *obstacle = 0;
  i32 backwards = 0;
  i32 stay_open = 0;
  i32 snap = 0;
  if (flags != 0 && argc != 0) {
    for (i32 index = 0; index < argc; ++index) {
      char *value = NuStrIStr(args[index], "name=");
      if (value != 0) {
        GIZMO_s *gizmo = GizmoFindByName(g_unk00960894->gizmoSys2b0c,
                                         obstacle_gizmotype_id, value + 5);
        if (gizmo != 0)
          obstacle = (GIZOBSTACLE_s *)gizmo->object;
      } else if (NuStrICmp(args[index], "backwards") == 0) {
        backwards = 1;
      } else if (NuStrICmp(args[index], "stayopen") == 0) {
        stay_open = 1;
      } else if (NuStrICmp(args[index], "stayshut") == 0) {
        stay_shut = 1;
      } else if (NuStrICmp(args[index], "SNAP") == 0) {
        snap = 1;
      }
    }

    if (obstacle != 0) {
      if (backwards || stay_shut) {
        if (snap)
          GizObstacle_JumpToStart(obstacle);
        else
          GizObstacle_PlayBackwards(obstacle);
      } else if (snap) {
        GizObstacle_JumpToEnd(obstacle);
      } else {
        GizObstacle_PlayForwards(obstacle);
      }
      obstacle->stay_open = stay_open;
      obstacle->stay_shut = stay_shut;
    }
  }
  return 1;
}

struct GAMEANIMSET_s;

struct GIZSPECIAL_s {
  u8 pad0[0x20];
  GAMEANIMSET_s *anim_set; // 0x20
};

// GLOBAL: LEGOBATMAN 0x00967aec
extern i32 gizspecial_gizmotype_id;

void GameAnimSet_JumpToStart(GAMEANIMSET_s *set);
void GameAnimSet_JumpToEnd(GAMEANIMSET_s *set);
void GameAnimSet_Play(GAMEANIMSET_s *set, f32 speed, i32 a);

// FUNCTION: LEGOBATMAN 0x00463060
i32 Action_PlayGizSpecial(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                          AIPACKET_s *packet, char **args, int argc, int flags,
                          f32 time) {
  GIZSPECIAL_s *special = 0;
  i32 backwards = 0;
  i32 snap = 0;
  i32 restart = 0;
  if (flags != 0 && argc != 0) {
    for (i32 index = 0; index < argc; ++index) {
      char *value = NuStrIStr(args[index], "name=");
      if (value != 0) {
        GIZMO_s *gizmo = GizmoFindByName(g_unk00960894->gizmoSys2b0c,
                                         gizspecial_gizmotype_id, value + 5);
        if (gizmo != 0)
          special = (GIZSPECIAL_s *)gizmo->object;
      } else if (NuStrICmp(args[index], "backwards") == 0) {
        backwards = 1;
      } else if (NuStrICmp(args[index], "SNAP") == 0) {
        snap = 1;
      } else if (NuStrICmp(args[index], "Restart") == 0) {
        restart = 1;
      }
    }

    if (special != 0) {
      if (backwards != 0) {
        if (snap != 0) {
          GameAnimSet_JumpToStart(special->anim_set);
        } else {
          if (restart != 0)
            GameAnimSet_JumpToEnd(special->anim_set);
          GameAnimSet_Play(special->anim_set, -1.0f, 1);
        }
      } else if (snap != 0) {
        GameAnimSet_JumpToEnd(special->anim_set);
      } else {
        if (restart != 0)
          GameAnimSet_JumpToStart(special->anim_set);
        GameAnimSet_Play(special->anim_set, 1.0f, 1);
      }
    }
  }
  return 1;
}

void SetForceBack(GameObject_s *obj, nuvec_s *position, f32 radius, i32 type);
void ResetForceBack(void);

// FUNCTION: LEGOBATMAN 0x00463f00
i32 Action_SetAnimSpeedMul(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                           AIPACKET_s *packet, char **params, i32 count,
                           i32 first, f32 dt) {
  f32 multiply_by = 1.0f;
  f32 minimum = 0.0f;
  f32 maximum = 1.0e9f;
  GameObject_s *object;
  if (packet == 0 || packet->pd0 == 0 || packet->pd0->obj == 0)
    return 1;
  object = packet->pd0->obj;
  if (count != 0) {
    for (i32 i = 0; i < count; i++) {
      char *value = NuStrIStr(params[i], "value=");
      if (value != 0) {
        object->anim_speed_mul = AIParamToFloat(process, value + 6);
      } else if ((value = NuStrIStr(params[i], "multiply_by=")) != 0) {
        multiply_by = AIParamToFloat(process, value + 12);
      } else if ((value = NuStrIStr(params[i], "max=")) != 0) {
        maximum = AIParamToFloat(process, value + 4);
      } else if ((value = NuStrIStr(params[i], "min=")) != 0) {
        minimum = AIParamToFloat(process, value + 4);
      }
    }
    if (multiply_by != 1.0f) {
      object->anim_speed_mul *= multiply_by;
      if (object->anim_speed_mul > maximum)
        object->anim_speed_mul = maximum;
      else if (object->anim_speed_mul < minimum)
        object->anim_speed_mul = minimum;
    }
  }
  return 1;
}

struct GIZFORCE_s {
  u8 pad0[0x1c];
  nuvec_s position; // 0x1c
  u8 pad28[0xa0 - 0x28];
  u32 flags; // 0xa0, bit 0 = enabled, bit 1 = visible, bit 15 = pending
};

// GLOBAL: LEGOBATMAN 0x0093e144
extern i32 force_gizmotype_id;

i32 qrand(void);
i32 GizForce_Complete(GIZFORCE_s *force);

i32 ObjHitObj(GameObject_s *attacker, GameObject_s *target, i32 damage,
              u16 flags, i32 probe, i32 context);
void NewRumble(nupad_s *pad, f32 strength, i32 frames);
void NewBuzz(nupad_s *pad, f32 duration, i32 frames);
AIAREA_s *AISysFindArea(AISYS_s *sys, char *name);

// 0x40-byte trigger areas; the owner (AISYS_s) holds the array at +0x244.
struct AIAreaOwner_s;
struct AIAREA_s {
  u8 pad0[0x38];
  struct AIAreaOwner_s *owner; // 0x38
  u8 pad3c[0x40 - 0x3c];
};
struct AIAreaOwner_s {
  u8 pad0[0x244];
  AIAREA_s *areas; // 0x244
};

// GLOBAL: LEGOBATMAN 0x00ab3988
extern i32 g_unk00ab3988; // player count

// FUNCTION: LEGOBATMAN 0x004646a0
i32 Action_TakeDamage(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                      AIPACKET_s *packet, char **params, i32 param_count,
                      i32 first_time, f32 dt) {
  f32 range = 0.0f;
  GameObject_s *hitter = NULL;
  AIAREA_s *area = NULL;
  i32 damage = 1;
  i32 set = 0;
  GameObject_s *obj = NULL;
  nuvec_s d;
  if (first_time) {
    if (packet != NULL && packet->pd0 != NULL && packet->pd0->obj != NULL)
      obj = packet->pd0->obj;
    if (param_count > 0) {
      for (i32 i = 0; i < param_count; i++) {
        char *value;
        if ((value = NuStrIStr(params[i], "character=")) != NULL) {
          obj = GetNamedGameObjectInl(sys, value + 10);
        } else if ((value = NuStrIStr(params[i], "hitter=")) != NULL) {
          hitter = GetNamedGameObjectInl(sys, value + 7);
        } else if ((value = NuStrIStr(params[i], "damage=")) != NULL) {
          damage = (i32)AIParamToFloat(process, value + 7);
        } else if ((value = NuStrIStr(params[i], "PlayersInTriggerArea=")) !=
                   NULL) {
          area = AISysFindArea(g_unk00960894->aiSys2bf8, value + 21);
        } else if ((value = NuStrIStr(params[i], "PlayersInRange=")) != NULL) {
          range = AIParamToFloat(process, value + 15);
        } else if (NuStrIStr(params[i], "opponent") != NULL) {
          if (packet != NULL && packet->pe4 != NULL &&
              packet->pe4->obj != NULL) {
            obj = packet->pe4->obj;
            if (packet->pd0 != NULL && packet->pd0->obj != NULL)
              hitter = packet->pd0->obj;
          }
        } else if ((value = NuStrIStr(params[i], "set=")) != NULL) {
          set = (i32)AIParamToFloat(process, value + 4);
          if (set < 0 || set > 16)
            set = 0;
        }
      }
    }
    if (set != 0) {
      GameObject_s *o = Obj;
      for (i32 i = 0; i < HIGHGAMEOBJECT; i++, o++) {
        if ((o->flags1fc & 1) && o->process290[0xb4] == set)
          ObjHitObj(hitter, o, damage, 0, 0, 1);
      }
    } else if (area != NULL) {
      for (i32 i = 0; i < g_unk00ab3988; i++) {
        GameObject_s *p = Player[i];
        if (p != NULL && (p->flags1fc & 1) && (p->flags1fc & 0x1000) &&
            p->b257 == 0 && (p->flags1fc & 0x80) &&
            (p->area_mask &
             ((unsigned __int64)1 << (area - area->owner->areas))) != 0) {
          ObjHitObj(hitter, p, damage, 0, 0, 1);
          NewRumble(p->p112c->pad0, 0.5f, 0);
          NewBuzz(p->p112c->pad0, 0.1f, 0);
        }
      }
    } else if (range != 0.0f) {
      for (i32 i = 0; i < g_unk00ab3988; i++) {
        GameObject_s *p = Player[i];
        if (p != NULL && (p->flags1fc & 1) && (p->flags1fc & 0x1000) &&
            p->b257 == 0 && (p->flags1fc & 0x80) &&
            NuVecDistSqr(&p->position, &packet->pd0->pos5c, &d) < range)
          ObjHitObj(hitter, p, damage, 0, 0, 1);
      }
    } else if (obj != NULL) {
      ObjHitObj(hitter, obj, damage, 0, 0, 1);
    }
  }
  return 1;
}

// STUB: LEGOBATMAN 0x00464d10
// close: orig keeps a separate (store-interleaved) epilogue for the
// triggered_by_hit return 0; ours tail-merges it with the final return 0.
i32 Action_UseForce(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                    AIPACKET_s *packet, char **params, i32 count, i32 first,
                    f32 dt) {
  GameObject_s *object;
  i32 triggered_by_hit = 0;
  i32 candidate_count = 0;
  GIZFORCE_s *force;

  if (packet == 0 || packet->pd0 == 0 || packet->pd0->obj == 0)
    return 1;
  object = packet->pd0->obj;

  if (first != 0) {
    process->action_data_3 = 0;
    {
      GIZFORCE_s *candidates[16];
      for (i32 i = 0; i < count; i++) {
        if (NuStrICmp(params[i], "throwable") == 0) {
          process->action_data_1 = 1;
        } else if (NuStrICmp(params[i], "inrange") == 0) {
          process->action_data_2 = 1;
        } else if (NuStrICmp(params[i], "triggered_by_hit") == 0) {
          triggered_by_hit = 1;
        } else {
          char *name = NuStrIStr(params[i], "name");
          if (name != 0)
            name += 5;
          else
            name = params[i];
          if (name != 0 && candidate_count < 16) {
            GIZMO_s *gizmo = GizmoFindByName(g_unk00960894->gizmoSys2b0c,
                                             force_gizmotype_id, name);
            if (gizmo != 0 && gizmo->object != 0) {
              force = (GIZFORCE_s *)gizmo->object;
              candidates[candidate_count] = force;
              if ((force->flags & 2) && !(force->flags & 0x10000))
                candidate_count++;
            }
          }
        }
      }
      if (candidate_count != 0)
        process->action_data_3 =
            candidates[qrand() / (0xffff / candidate_count + 1)];
    }
  }

  force = (GIZFORCE_s *)process->action_data_3;
  if (force == 0)
    return 1;
  if (force->flags & 1) {
    if (triggered_by_hit != 0) {
      force->flags |= 0x8000;
      return 0;
    }
    packet->look_target = &force->position;
    object->p112c->flags5a |= 4;
    object->gizforce_target = force;
    if (process->action_data_1 != 0) {
      if (packet->pd4 == 0) {
        object->p112c->flags5a &= ~4;
        object->gizforce_target = 0;
      }
      return 1;
    }
    if (GizForce_Complete(force) != 0)
      return 1;
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x00467f50
i32 Action_SetForceBack(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                        AIPACKET_s *packet, char **args, int argc, int flags,
                        f32 time) {
  GameObject_s *obj = 0;
  i32 enabled = 1;
  i32 type = 0;
  nuvec_s *position = 0;
  f32 radius = 1.5f;
  char *s;
  i32 i;
  if (flags) {
    if (packet && packet->pd0 && packet->pd0->obj)
      obj = packet->pd0->obj;
    for (i = 0; i < argc; i++) {
      if ((s = NuStrIStr(args[i], "character="))) {
        obj = GetNamedGameObjectInl(sys, s + 10);
      } else if ((s = NuStrIStr(args[i], "locator="))) {
        u8 *locator = (u8 *)AIPathFindLocator(sys, s + 8);
        if (locator)
          position = (nuvec_s *)(locator + 0x10);
      } else if ((s = NuStrIStr(args[i], "radius="))) {
        radius = AIParamToFloat(process, s + 7);
      } else if (!NuStrICmp(args[i], "FALSE")) {
        enabled = 0;
      } else if (!NuStrICmp(args[i], "type=CHOKE")) {
        type = 1;
      } else if (!NuStrICmp(args[i], "type=DROID")) {
        type = 2;
      } else if (!NuStrICmp(args[i], "type=ComboOpponent")) {
        type = 3;
      }
    }
    if (enabled) {
      if (obj || position)
        SetForceBack(obj, position, radius, type);
    } else {
      ResetForceBack();
    }
  }
  return 1;
}

struct AILOCATOR_s {
  u8 pad0[0x44];
};

struct AILOCATORSET_s {
  u8 pad0[0x10];
  i8 locator_count; // 0x10
  u8 pad11[3];
  u8 *locator_entries; // 0x14
  u8 *assigned;        // 0x18
};

struct AISysPathSys_s {
  u8 pad0[8];
  AIPATH_s *active_path; // 0x08
};

struct AssignLocatorAISys_s {
  u8 pad0[0x21c];
  AISysPathSys_s *path_sys; // 0x21c
  u8 pad220[0x234 - 0x220];
  AILOCATOR_s *locators;        // 0x234
  i32 locator_set_count;        // 0x238
  AILOCATORSET_s *locator_sets; // 0x23c
};

AILOCATORSET_s *AIPathFindLocatorSet(AISYS_s *sys, char *name);

// FUNCTION: LEGOBATMAN 0x0046bb20
i32 Action_AssignLocatorInSet(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                              AIPACKET_s *packet, char **args, int argc,
                              int flags, f32 time) {
  i32 assignment = -1;
  GameObject_s *obj = 0;
  AILOCATOR_s *locator = 0;
  AILOCATORSET_s *locator_set = 0;
  char *s;
  i32 i;
  if (flags) {
    if (packet && packet->pd0) {
      obj = packet->pd0->obj;
      assignment = obj->b259;
    }
    for (i = 0; i < argc; i++) {
      if (NuStrICmp(args[i], "locator=mylocator") == 0) {
        if (obj)
          locator = *(AILOCATOR_s **)(obj->process290 + 0xa8);
      } else if ((s = NuStrIStr(args[i], "locator"))) {
        s = s + NuStrLen("locator") + 1;
        locator = AIPathFindLocator(sys, s);
      } else if ((s = NuStrIStr(args[i], "character"))) {
        s = s + NuStrLen("character") + 1;
        obj = GetNamedGameObjectInl(sys, s);
        assignment = obj->b259;
      } else if ((s = NuStrIStr(args[i], "set"))) {
        s = s + NuStrLen("set") + 1;
        locator_set = AIPathFindLocatorSet(sys, s);
      } else if (NuStrICmp(args[i], "reserve") == 0) {
        assignment = 0x80;
      } else if (NuStrICmp(args[i], "unreserve") == 0) {
        assignment = 0xff;
      }
    }
    if (locator_set && locator) {
      i32 count = locator_set->locator_count;
      for (i = 0; i < count; i++) {
        if (&((AssignLocatorAISys_s *)sys)
                 ->locators[locator_set->locator_entries[i]] == locator)
          break;
      }
      locator_set->assigned[i] = assignment;
    }
  }
  return 1;
}

void AlertSurroundingCreatures(GameObject_s *obj, nuvec_s *pos, i32 a);

// FUNCTION: LEGOBATMAN 0x0046c630
i32 Action_AlertCreatures(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                          AIPACKET_s *packet, char **args, int argc, int flags,
                          f32 time) {
  GameObject_s *obj = NULL;
  if (flags != 0) {
    i32 i;
    if (packet != NULL && packet->pd0 != NULL && packet->pd0->obj != NULL)
      obj = packet->pd0->obj;
    for (i = 0; i < argc; i++) {
      char *value = NuStrIStr(args[i], "character=");
      if (value != NULL)
        obj = GetNamedGameObjectInl(sys, value + 10);
    }
    if (obj != NULL)
      AlertSurroundingCreatures(obj, &obj->v80, 1);
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x0046d090
i32 Action_CanBeCarried(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                        AIPACKET_s *packet, char **args, int argc, int flags,
                        f32 time) {
  GameObject_s *obj = NULL;
  i32 enabled = 1;
  if (flags != 0) {
    if (packet != NULL && packet->pd0 != NULL)
      obj = packet->pd0->obj;
    if (argc != 0) {
      for (i32 i = 0; i < argc; i++) {
        if (NuStrICmp(args[i], "FALSE") == 0) {
          enabled = 0;
        } else {
          char *value = NuStrIStr(args[i], "character=");
          if (value != NULL)
            obj = GetNamedGameObjectInl(sys, value + 10);
        }
      }
    }
    if (obj != NULL) {
      if (obj->b9db == 0x3c || obj->p1158 != NULL)
        Player_ClearContext(obj, 1);
      obj->can_be_carried = enabled;
    }
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x0046d1b0
i32 Action_CanBeTargettedByCable(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                 AIPACKET_s *packet, char **args, int argc,
                                 int flags, f32 time) {
  GameObject_s *obj = NULL;
  i32 enabled = 1;
  if (flags != 0) {
    if (packet != NULL && packet->pd0 != NULL)
      obj = packet->pd0->obj;
    if (argc != 0) {
      for (i32 i = 0; i < argc; i++) {
        if (NuStrICmp(args[i], "FALSE") == 0) {
          enabled = 0;
        } else {
          char *value = NuStrIStr(args[i], "character=");
          if (value != NULL)
            obj = GetNamedGameObjectInl(sys, value + 10);
        }
      }
    }
    if (obj != NULL)
      obj->can_be_targetted_by_cable = enabled;
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x0046d2b0
i32 Action_IgnoreLastSafePathPos(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                 AIPACKET_s *packet, char **args, int argc,
                                 int flags, f32 time) {
  GameObject_s *obj = NULL;
  i32 ignore = 1;
  if (flags != 0) {
    if (packet != NULL && packet->pd0 != NULL)
      obj = packet->pd0->obj;
    if (argc != 0) {
      for (i32 i = 0; i < argc; i++) {
        if (NuStrICmp(args[i], "FALSE") == 0) {
          ignore = 0;
        } else {
          char *value = NuStrIStr(args[i], "character=");
          if (value != NULL)
            obj = GetNamedGameObjectInl(sys, value + 10);
        }
      }
    }
    if (obj != NULL) {
      if (obj->b9db == 0x3c || obj->p1158 != NULL)
        Player_ClearContext(obj, 1);
      obj->ignore_last_safe_path_pos = ignore;
    }
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x0046d6f0
i32 Action_AddMiscPickups(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                          AIPACKET_s *packet, char **args, int argc, int flags,
                          f32 time) {
  GameObject_s *obj = 0;
  i32 value = 0;
  i32 torpedo = 0;
  Unk_AIPacketObj *api;
  char *s;
  i32 i;
  if (!flags)
    return 1;
  if (packet && packet->pd0 && packet->pd0->obj)
    obj = packet->pd0->obj;
  for (i = 0; i < argc; i++) {
    if ((s = NuStrIStr(args[i], "character="))) {
      if (GetNamedAPIObjectFn && (api = GetNamedAPIObjectFn(sys, s + 10)))
        obj = api->obj;
      else
        obj = 0;
    } else if ((s = NuStrIStr(args[i], "value="))) {
      value = (i32)AIParamToFloat(process, s + 6);
    } else if ((s = NuStrIStr(args[i], "torpedo="))) {
      torpedo = (i32)AIParamToFloat(process, s + 8);
    }
  }
  if (obj && &obj->v80 && (value || torpedo))
    AddMiscPickups(&obj->v80, -1, value, torpedo, 1);
  return 1;
}

// FUNCTION: LEGOBATMAN 0x0046d870
i32 Action_SetLayer(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                    AIPACKET_s *packet, char **args, int argc, int flags,
                    f32 time) {
  u32 set_layers = 0;
  u32 clear_layers = 0;
  u32 remove_layers = 0;
  u32 clear_remove_layers = 0;
  GameObject_s *obj = 0;
  char *s;
  i32 layer;
  i32 i;
  if (flags) {
    if (packet && packet->pd0 && packet->pd0->obj)
      obj = packet->pd0->obj;
    for (i = 0; i < argc; i++) {
      if ((s = NuStrIStr(args[i], "character="))) {
        obj = GetNamedGameObjectInl(sys, s + 10);
      } else if ((s = NuStrIStr(args[i], "set_layer="))) {
        layer = (i32)AIParamToFloat(process, s + 10);
        if ((u32)(layer - 1) <= 31)
          set_layers |= 1 << (layer - 1);
      } else if ((s = NuStrIStr(args[i], "remove_layer="))) {
        layer = (i32)AIParamToFloat(process, s + 13);
        if ((u32)(layer - 1) <= 31)
          remove_layers |= 1 << (layer - 1);
      }
      if ((s = NuStrIStr(args[i], "clear_remove_layer="))) {
        layer = (i32)AIParamToFloat(process, s + 19);
        if ((u32)(layer - 1) <= 31)
          clear_remove_layers |= 1 << (layer - 1);
      } else if ((s = NuStrIStr(args[i], "clear_layer="))) {
        layer = (i32)AIParamToFloat(process, s + 12);
        if ((u32)(layer - 1) <= 31)
          clear_layers |= 1 << (layer - 1);
      }
    }
    if (obj) {
      u32 *layers = (u32 *)((u8 *)obj + 0x1588);
      layers[0] = (layers[0] | set_layers) & ~clear_layers;
      layers[1] = (layers[1] | remove_layers) & ~clear_remove_layers;
    }
  }
  return 1;
}

struct TECHNO_s {
  u8 pad0[0x50];
  nuvec_s position;        // 0x50
  nuvec_s ground_position; // 0x5c
  u8 pad68[0x8b - 0x68];
  u8 active : 1;  // 0x8b bit 0
  u8 visible : 1; // 0x8b bit 1
  u8 flags8b_b2 : 1;
  u8 complete : 1; // 0x8b bit 3
  u8 flags8b_hi : 4;
};

i32 GizmoGetTypeIDByName(GIZMOSYS_s *gizmo_sys, char *name);
TECHNO_s *Technos_FindControllingTechno(GameObject_s *object);

// GLOBAL: LEGOBATMAN 0x009cb0b4
extern i32 lightning_type;
// GLOBAL: LEGOBATMAN 0x00942de8
extern f32 lightning_sizew[2];
// GLOBAL: LEGOBATMAN 0x00942df0
extern f32 lightning_sizel[2];
// GLOBAL: LEGOBATMAN 0x00942df8
extern f32 lightning_sizewab[2];
// GLOBAL: LEGOBATMAN 0x00942e00
extern f32 lightning_endw[2];
// GLOBAL: LEGOBATMAN 0x00942e08
extern i32 lightning_col[2];

void ForceLightning_Origin(GameObject_s *obj, nuvec_s *primary,
                           nuvec_s *secondary);
f32 NuRandFloat(void);
f32 NuVecDist(nuvec_s *v0, nuvec_s *v1, nuvec_s *d);
extern "C" void NuLgtLaser(i32 type, f32 sizew, f32 sizel, f32 sizewab,
                           nuvec_s *pos, nuvec_s *dir, i32 col, f32 endw,
                           f32 len);
void PlaySfx(char *name, nuvec_s *pos);

// GLOBAL: LEGOBATMAN 0x00961104
extern i32 blowup_gizmotype_id;
void FastWeaponIn(GameObject_s *obj, i32 force_sound);
void FastWeaponOut(GameObject_s *obj, i32 force_sound);
extern "C" f32 AnimDuration(i32 character_id, i32 animation, f32 start_frame,
                            f32 end_frame, i32 subtract_frame_time);

GameObject_s *AddDynamicCreature(i32 type, nuvec_s *pos, i32 angle,
                                 char *script, AIPATHINFO *path_info,
                                 void *group, i32 set_on_surface, void *spline,
                                 nuvec_s *spline_offset, i32 spline_mode,
                                 i32 creature_set, void *item);
void TakeOverGameObject(GameObject_s *rider, GameObject_s *vehicle,
                        i32 blend_camera, i32 immediate);

// FUNCTION: LEGOBATMAN 0x0046dac0
i32 Action_CreateRider(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                       AIPACKET_s *packet, char **params, i32 param_count,
                       i32 first_time, f32 dt) {
  nuvec_s pos;
  i32 type = -1;
  i32 takeover_type = -1;
  GameObject_s *obj = NULL;
  char *script = "default";
  char *takeover_script = "default";
  i32 set = 0;
  i32 keep = 0;
  AILOCATOR_s *locator = NULL;
  AILOCATOR_s *locator_after = NULL;
  u16 angle = 0;
  pos.x = 0.0f;
  pos.y = 0.0f;
  pos.z = 0.0f;
  if (packet != NULL && packet->pd0 != NULL && packet->pd0->obj != NULL)
    obj = packet->pd0->obj;
  if (first_time) {
    for (i32 i = 0; i < param_count; i++) {
      char *value;
      if ((value = NuStrIStr(params[i], "setlocator_afterspawn")) != NULL) {
        locator_after = AIPathFindLocator(sys, value + 22);
      } else if ((value = NuStrIStr(params[i], "locator")) != NULL) {
        locator = AIPathFindLocator(sys, value + 8);
      } else if (NuStrIStr(params[i], "keepLastTakeOver") != NULL) {
        keep = 1;
      } else if ((value = NuStrIStr(params[i], "TakeOverType")) != NULL) {
        if (g_unk00ad68ec != NULL && g_unk00ad68fc != NULL) {
          takeover_type = g_unk00ad68ec(value + 13);
          if (takeover_type != 0xff)
            takeover_type = g_unk00ad68fc(takeover_type);
        }
      } else if ((value = NuStrIStr(params[i], "TakeOverScript")) != NULL) {
        takeover_script = value + 15;
      } else if ((value = NuStrIStr(params[i], "type")) != NULL) {
        if (g_unk00ad68ec != NULL && g_unk00ad68fc != NULL) {
          type = g_unk00ad68ec(value + 5);
          if (type != 0xff)
            type = g_unk00ad68fc(type);
        }
      } else if ((value = NuStrIStr(params[i], "script")) != NULL) {
        script = value + 7;
      } else if ((value = NuStrIStr(params[i], "addtoset")) != NULL) {
        set = (i32)AIParamToFloat(process, value + 9);
      }
    }
  }
  if (locator != NULL) {
    pos = *(nuvec_s *)((u8 *)locator + 0x10);
    angle = *(u16 *)((u8 *)locator + 0x1c);
  } else {
    if (packet != NULL && packet->pd0 != NULL)
      pos = packet->pd0->pos5c;
  }
  if (takeover_type != -1) {
    obj = AddDynamicCreature(takeover_type, &pos, angle, takeover_script, NULL,
                             NULL, 0, NULL, NULL, 0, 0, NULL);
    if (obj != NULL) {
      if (locator_after != NULL)
        ((AIPACKET_s *)obj->process290)->locator = locator_after;
      else if (locator != NULL)
        ((AIPACKET_s *)obj->process290)->locator = locator;
    }
  }
  if (type != 0xff && obj != NULL && obj->p1158 == NULL) {
    GameObject_s *rider = AddDynamicCreature(type, &obj->v80, 0, script, NULL,
                                             NULL, 0, NULL, NULL, 0, set, NULL);
    if (rider != NULL) {
      if (locator_after != NULL)
        ((AIPACKET_s *)rider->process290)->locator = locator_after;
      else if (locator != NULL)
        ((AIPACKET_s *)rider->process290)->locator = locator;
      if (keep == 1)
        rider->flags1580 |= 8;
      TakeOverGameObject(rider, obj, 0, 1);
    }
  }
  return 1;
}

// STUB: LEGOBATMAN 0x0046e4e0
// Only diff: explode/targettable land in swapped stack slots (any
// declaration order).
i32 Action_ThrowDetonator(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                          AIPACKET_s *packet, char **params, i32 param_count,
                          i32 first_time, f32 dt) {
  BlowupTarget_s *blowup = NULL;
  i32 explode = 0;
  i32 targettable = 1;
  GameObject_s *opponent = NULL;
  GameObject_s *obj;
  if (packet == NULL || packet->pd0 == NULL || packet->pd0->obj == NULL)
    return 1;
  obj = packet->pd0->obj;
  if (first_time) {
    for (i32 i = 0; i < param_count; i++) {
      char *value;
      if (NuStrIStr(params[i], "opponent=last_attacker") != NULL) {
        opponent = obj->last_attacker;
      } else if (NuStrIStr(params[i], "opponent=opponent") != NULL) {
        if (((AIPACKET_s *)obj->process290)->pe4 != NULL)
          opponent = ((AIPACKET_s *)obj->process290)->pe4->obj;
      } else if ((value = NuStrIStr(params[i], "opponent=")) != NULL) {
        opponent = GetNamedGameObject(sys, value + 9);
      } else if ((value = NuStrIStr(params[i], "blowup=")) != NULL) {
        GIZMO_s *gizmo = GizmoFindByName(g_unk00960894->gizmoSys2b0c,
                                         blowup_gizmotype_id, value + 7);
        if (gizmo != NULL)
          blowup = (BlowupTarget_s *)gizmo->object;
      } else if (NuStrICmp(params[i], "explode_on_impact") == 0) {
        explode = 1;
      } else if (NuStrICmp(params[i], "targettable=FALSE") == 0) {
        targettable = 0;
      }
    }
  }
  obj->b9db = 0x2e;
  memset(&obj->force_target, 0, 0x10);
  if (opponent != NULL)
    obj->force_target = opponent;
  else if (blowup != NULL)
    obj->blowup_target = blowup;
  obj->b9e1 = 0;
  if (explode)
    obj->b9e1 = 1;
  if (targettable)
    obj->b9e1 |= 2;
  if (obj->b9db == 6) {
    FastWeaponIn(obj, 0);
    obj->s9d0 = 0x65;
  } else if (obj->b9db == 7) {
    FastWeaponOut(obj, 0);
    obj->s9d0 = 0x6e;
  } else if (obj->b131c == 1) {
    obj->s9d0 = 0x6f;
  } else if (obj->flags130c & 0x80000) {
    obj->s9d0 = 0x6e;
  } else {
    obj->s9d0 = 0x65;
  }
  if (((void **)obj->p50->p0c)[obj->s9d0] == NULL)
    obj->s9d0 = 0x65;
  obj->flags9e4 &= ~0x40;
  if (((void **)obj->p50->p0c)[obj->s9d0] != NULL)
    obj->f98c = AnimDuration(obj->type15b0, obj->s9d0, 0.0f, 0.0f, 1);
  else
    obj->f98c = 1.0f;
  return 1;
}

// FUNCTION: LEGOBATMAN 0x0046eee0
i32 Action_ForceLightning(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                          AIPACKET_s *packet, char **params, i32 param_count,
                          i32 first_time, f32 dt) {
  GameObject_s *obj = NULL;
  nuvec_s target;
  nuvec_s dir;
  nuvec_s origin[2];
  if (packet != NULL && packet->pd0 != NULL && packet->pd0->obj != NULL)
    obj = packet->pd0->obj;
  if (first_time && param_count > 0) {
    for (i32 i = 0; i < param_count; i++) {
      char *value = NuStrIStr(params[i], "locator=");
      if (value != NULL) {
        AILOCATOR_s *loc = AIPathFindLocator(sys, value + 8);
        if (loc != NULL) {
          process->action_pos = *(nuvec_s *)((u8 *)loc + 0x10);
          process->action_data_1 = 1;
        }
      } else if ((value = NuStrIStr(params[i], "randxz=")) != NULL) {
        process->action_data_4 = AIParamToFloat(process, value + 7);
      }
    }
  }
  if (obj == NULL || process->action_data_1 == 0)
    return 0;
  ForceLightning_Origin(obj, &origin[0], &origin[1]);
  target = process->action_pos;
  if (process->action_data_4 > 0.0f) {
    target.x +=
        process->action_data_4 -
        NuRandFloat() * (process->action_data_4 + process->action_data_4);
    target.z +=
        process->action_data_4 -
        NuRandFloat() * (process->action_data_4 + process->action_data_4);
  }
  for (i32 i = 0; i < 2; i++) {
    if (origin[i].y != 1000000000.0) {
      f32 dist = NuVecDist(&target, &origin[i], &dir);
      NuLgtLaser(lightning_type, lightning_sizew[0], lightning_sizel[0],
                 lightning_sizewab[0], &origin[i], &dir, lightning_col[0],
                 lightning_endw[0], dist);
      PlaySfx("ForceLightningLp", &process->action_pos);
    }
  }
  packet->look_target = &process->action_pos;
  return 0;
}

// FUNCTION: LEGOBATMAN 0x0046f500
i32 Action_SetTechnoComplete(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                             AIPACKET_s *packet, char **args, int argc,
                             int flags, f32 time) {
  i32 complete = 1;
  TECHNO_s *techno = 0;
  GameObject_s *obj = 0;
  Unk_AIPacketObj *api;
  char *s;
  i32 i;
  if (packet && packet->pd0)
    obj = packet->pd0->obj;
  if (flags) {
    for (i = 0; i < argc; i++) {
      if ((s = NuStrIStr(args[i], "techno"))) {
        s = s + NuStrLen("techno") + 1;
        GIZMO_s *gizmo = GizmoFindByName(
            g_unk00960894->gizmoSys2b0c,
            GizmoGetTypeIDByName(g_unk00960894->gizmoSys2b0c, "Techno"), s);
        if (gizmo != 0 && gizmo->object != 0)
          techno = (TECHNO_s *)gizmo->object;
      } else if (!NuStrICmp(args[i], "FALSE")) {
        complete = 0;
      } else if ((s = NuStrIStr(args[i], "controlling"))) {
        s = s + NuStrLen("controlling") + 1;
        obj = GetNamedGameObjectInl(g_unk00960894->aiSys2bf8, s);
      }
    }
    if (obj)
      techno = Technos_FindControllingTechno(obj);
    if (techno)
      techno->complete = complete;
  }
  return 1;
}

struct LEVELDATA_s {
  u8 pad0[0x62];
  i16 idx;  // 0x62
  u8 flags; // 0x64
};

// GLOBAL: LEGOBATMAN 0x00ab0894
extern i32 FreePlay;

LEVELDATA_s *Level_FindByName(char *name, i32 *idx_out);
void *NewCutScene(void *a, void *cutscene_sys, char *name, i32 b);
LEVELDATA_s *Area_FindStatusLevel(AREADATA_s *area, i32 *index);
void GoToNewLevel(i32 idx);
void CompleteLevel(WORLDINFO_s *world);

// FUNCTION: LEGOBATMAN 0x0046f880
i32 Action_CompleteLevel(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                         AIPACKET_s *packet, char **args, int argc, int flags,
                         f32 time) {
  char *cutscene = 0;
  LEVELDATA_s *level = 0;
  LEVELDATA_s *freeplay_level = 0;
  if (flags != 0) {
    for (i32 index = 0; index < argc; ++index) {
      char *value = NuStrIStr(args[index], "cutscene=");
      if (value != 0) {
        cutscene = value + NuStrLen("cutscene=");
      } else if ((value = NuStrIStr(args[index], "newlevel=")) != 0) {
        value += NuStrLen("newlevel=");
        level = Level_FindByName(value, 0);
      } else if ((value = NuStrIStr(args[index], "freeplay_level=")) != 0) {
        value += NuStrLen("freeplay_level=");
        freeplay_level = Level_FindByName(value, 0);
      }
    }
    if (FreePlay == 0 && cutscene != 0 &&
        NewCutScene(0, g_unk00960894->cutscene_sys, cutscene, 0) != 0)
      return 1;
    if (FreePlay != 0) {
      if (freeplay_level != 0) {
        GoToNewLevel(freeplay_level->idx);
        return 1;
      }
      if (level != 0 && (level->flags & 0xe0) != 0)
        level = Area_FindStatusLevel(g_unk00960894->area, 0);
    }
    if (level != 0)
      GoToNewLevel(level->idx);
    else
      CompleteLevel(g_unk00960894);
  }
  return 1;
}

struct TORPEDOPACKET_s {
  u8 count;  // 0x00
  u8 flags1; // 0x01
  u8 pad2;
  u8 field_03; // 0x03
  u8 field_04; // 0x04
  u8 pad5[3];
  f32 field_08; // 0x08
  u8 padc[0x3c - 0xc];
  nuvec_s pickup_positions[6]; // 0x3c
  i32 field_84;                // 0x84
};

i32 getMaxTorpedos(void *a);
void FreeTorpedoPacket(TORPEDOPACKET_s **packet);
TORPEDOPACKET_s *GetTorpedoPacket(void);

// FUNCTION: LEGOBATMAN 0x00470370
i32 Action_AddTorpedoPacket(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                            AIPACKET_s *packet, char **args, int argc,
                            int flags, f32 time) {
  GameObject_s *obj = 0;
  i32 torpedo_count = 0;
  u8 packet_flags = 0;
  char *s;
  if (packet && packet->pd0 && packet->pd0->obj)
    obj = packet->pd0->obj;
  if (flags) {
    for (i32 i = 0; i < argc; i++) {
      if ((s = NuStrIStr(args[i], "torpedo"))) {
        torpedo_count = (i32)AIParamToFloat((AISCRIPTPROCESS_s *)packet, s + 8);
        if (torpedo_count < 0)
          torpedo_count = 0;
        else if (torpedo_count > getMaxTorpedos(0))
          torpedo_count = getMaxTorpedos(0);
      } else if (NuStrICmp(args[i], "CANBESTOLEN") == 0) {
        packet_flags |= 0x20;
      }
    }
  }
  if (obj) {
    if (obj->torpedo)
      FreeTorpedoPacket(&obj->torpedo);
    obj->torpedo = GetTorpedoPacket();
    if (obj->torpedo && torpedo_count) {
      obj->torpedo->count = torpedo_count;
      obj->torpedo->flags1 |= packet_flags;
      for (i32 i = 0; i < torpedo_count; i++) {
        obj->torpedo->pickup_positions[i] = obj->v80;
        obj->torpedo->field_08 = 0.4f;
        obj->torpedo->field_03 = 0;
      }
    }
  }
  return 1;
}

// STUB: LEGOBATMAN 0x004717b0
// original keeps `target` in the dead `packet` home slot and `stun_flags` in
// a local; this form allocates them the other way round (edi/esi swapped too)
i32 Action_StunOpponent(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                        AIPACKET_s *packet, char **args, int argc, int flags,
                        f32 time) {
  GameObject_s *target = packet->pe4->obj;
  GameObject_s *self = packet->pd0->obj;
  f32 stun_time = 5.0f;
  i32 stun_flags = 0;
  char *s;
  i32 i;
  for (i = 0; i < argc; i++) {
    if ((s = NuStrIStr(args[i], "stun_time")))
      stun_time =
          AIParamToFloatEx(packet, process, s + NuStrLen("stun_time") + 1);
    else if (NuStrIStr(args[i], "stun_player"))
      stun_flags |= 0x80000;
    else if (NuStrIStr(args[i], "nearest"))
      target = packet->pd4->obj;
  }
  StunGameObject(target, self, stun_time, stun_flags);
  return 1;
}

// FUNCTION: LEGOBATMAN 0x00463850
i32 Action_PrefersPlayers(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                          AIPACKET_s *packet, char **args, int argc, int flags,
                          f32 time) {
  if (packet == 0 || packet->pd0 == 0 || packet->pd0->obj == 0)
    return 1;
  GameObject_s *object = packet->pd0->obj;
  *(u32 *)((u8 *)object + 0x1410) |= 4;
  if (argc != 0) {
    for (i32 i = 0; i < argc; i++) {
      if (NuStrICmp(args[i], "FALSE") == 0)
        *(u32 *)((u8 *)object + 0x1410) &= ~4;
    }
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x004638d0
i32 Action_SetBoltsDontGetDeflectedBack(AISYS_s *sys,
                                        AISCRIPTPROCESS_s *process,
                                        AIPACKET_s *packet, char **args,
                                        int argc, int flags, f32 time) {
  if (packet == 0 || packet->pd0 == 0 || packet->pd0->obj == 0)
    return 1;
  GameObject_s *object = packet->pd0->obj;
  *(u32 *)((u8 *)object + 0x1410) |= 0x80;
  if (argc != 0) {
    for (i32 i = 0; i < argc; i++) {
      if (NuStrICmp(args[i], "FALSE") == 0)
        *(u32 *)((u8 *)object + 0x1410) &= ~0x80;
    }
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x00463960
i32 Action_CanShootObstructions(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                AIPACKET_s *packet, char **args, int argc,
                                int flags, f32 time) {
  if (packet == 0 || packet->pd0 == 0 || packet->pd0->obj == 0)
    return 1;
  GameObject_s *object = packet->pd0->obj;
  *(u32 *)((u8 *)object + 0x1410) |= 0x1;
  if (argc != 0) {
    for (i32 i = 0; i < argc; i++) {
      if (NuStrICmp(args[i], "FALSE") == 0)
        *(u32 *)((u8 *)object + 0x1410) &= ~0x1;
    }
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x004639e0
i32 Action_UseBigJumpToJump(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                            AIPACKET_s *packet, char **args, int argc,
                            int flags, f32 time) {
  if (packet == 0 || packet->pd0 == 0 || packet->pd0->obj == 0)
    return 1;
  GameObject_s *object = packet->pd0->obj;
  *(u32 *)((u8 *)object + 0x1418) |= 0x1;
  if (argc != 0) {
    for (i32 i = 0; i < argc; i++) {
      if (NuStrICmp(args[i], "FALSE") == 0)
        *(u32 *)((u8 *)object + 0x1418) &= ~0x1;
    }
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x00463a60
i32 Action_IgnoreTurnAroundSpline(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                  AIPACKET_s *packet, char **args, int argc,
                                  int flags, f32 time) {
  if (packet == 0 || packet->pd0 == 0 || packet->pd0->obj == 0)
    return 1;
  GameObject_s *object = packet->pd0->obj;
  *(u32 *)((u8 *)object + 0x1418) |= 0x2;
  if (argc != 0) {
    for (i32 i = 0; i < argc; i++) {
      if (NuStrICmp(args[i], "FALSE") == 0)
        *(u32 *)((u8 *)object + 0x1418) &= ~0x2;
    }
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x00463ae0
i32 Action_CanMoveWhenDeactivated(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                  AIPACKET_s *packet, char **args, int argc,
                                  int flags, f32 time) {
  if (packet == 0 || packet->pd0 == 0 || packet->pd0->obj == 0)
    return 1;
  GameObject_s *object = packet->pd0->obj;
  *(u32 *)((u8 *)object + 0x1418) |= 0x4;
  if (argc != 0) {
    for (i32 i = 0; i < argc; i++) {
      if (NuStrICmp(args[i], "FALSE") == 0)
        *(u32 *)((u8 *)object + 0x1418) &= ~0x4;
    }
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x00463c50
i32 Action_DontAttack(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                      AIPACKET_s *packet, char **args, int argc, int flags,
                      f32 time) {
  if (packet == 0 || packet->pd0 == 0 || packet->pd0->obj == 0)
    return 1;
  GameObject_s *object = packet->pd0->obj;
  *(u32 *)((u8 *)object + 0x1418) |= 0x10;
  if (argc != 0) {
    for (i32 i = 0; i < argc; i++) {
      if (NuStrICmp(args[i], "FALSE") == 0)
        *(u32 *)((u8 *)object + 0x1418) &= ~0x10;
    }
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x00463cd0
i32 Action_CanPullLevers(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                         AIPACKET_s *packet, char **args, int argc, int flags,
                         f32 time) {
  if (packet == 0 || packet->pd0 == 0 || packet->pd0->obj == 0)
    return 1;
  GameObject_s *object = packet->pd0->obj;
  *(u32 *)((u8 *)object + 0x1410) |= 0x8000000;
  if (argc != 0) {
    for (i32 i = 0; i < argc; i++) {
      if (NuStrICmp(args[i], "FALSE") == 0)
        *(u32 *)((u8 *)object + 0x1410) &= ~0x8000000;
    }
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x00463d60
i32 Action_CanHitForceObjects(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                              AIPACKET_s *packet, char **args, int argc,
                              int flags, f32 time) {
  if (packet == 0 || packet->pd0 == 0 || packet->pd0->obj == 0)
    return 1;
  GameObject_s *object = packet->pd0->obj;
  *(u32 *)((u8 *)object + 0x140c) |= 0x400;
  if (argc != 0) {
    for (i32 i = 0; i < argc; i++) {
      if (NuStrICmp(args[i], "FALSE") == 0)
        *(u32 *)((u8 *)object + 0x140c) &= ~0x400;
    }
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x00463df0
i32 Action_AlwaysBackFlip(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                          AIPACKET_s *packet, char **args, int argc, int flags,
                          f32 time) {
  if (packet == 0 || packet->pd0 == 0 || packet->pd0->obj == 0)
    return 1;
  GameObject_s *object = packet->pd0->obj;
  *(u32 *)((u8 *)object + 0x140c) |= 0x1000;
  if (argc != 0) {
    for (i32 i = 0; i < argc; i++) {
      if (NuStrICmp(args[i], "FALSE") == 0)
        *(u32 *)((u8 *)object + 0x140c) &= ~0x1000;
    }
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x00463e80
i32 Action_PlayerSpeederHack(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                             AIPACKET_s *packet, char **args, int argc,
                             int flags, f32 time) {
  if (packet == 0 || packet->pd0 == 0 || packet->pd0->obj == 0)
    return 1;
  GameObject_s *object = packet->pd0->obj;
  *(u32 *)((u8 *)object + 0x1418) |= 0x40;
  if (argc != 0) {
    for (i32 i = 0; i < argc; i++) {
      if (NuStrICmp(args[i], "FALSE") == 0)
        *(u32 *)((u8 *)object + 0x1418) &= ~0x40;
    }
  }
  return 1;
}

extern void (*SetPlayerControlFn)(GameObject_s *object);
void AIScriptSetBaseScriptStateByName(AIPACKET_s *packet, char *name);
void ResetAICreature(GameObject_s *obj, AISYS_s *sys);

// FUNCTION: LEGOBATMAN 0x00453a10
i32 Action_Activate(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                    AIPACKET_s *packet, char **args, int argc, int flags,
                    f32 time) {
  i32 set = 0;
  GameObject_s *obj = NULL;
  if (flags == 0)
    return 1;
  if (packet != NULL && packet->pd0 != NULL && packet->pd0->obj != NULL)
    obj = packet->pd0->obj;
  for (i32 i = 0; i < argc; i++) {
    char *value = NuStrIStr(args[i], "character=");
    if (value != NULL) {
      obj = GetNamedGameObjectInl(sys, value + 10);
    } else if ((value = NuStrIStr(args[i], "set=")) != NULL) {
      set = (i32)AIParamToFloat(process, value + 4);
      if (set < 0 || set > 16)
        set = 0;
    }
  }
  if (set != 0) {
    GameObject_s *o = Obj;
    for (i32 i = 0; i < HIGHGAMEOBJECT; i++, o++) {
      if ((o->flags1fc & 1) && o->process290[0xb4] == set) {
        if (o->b3c8 == 0xff) {
          o->flags1fc |= 0x1000;
          if (SetPlayerControlFn != NULL)
            SetPlayerControlFn(o);
          else
            AIScriptSetBaseScriptStateByName((AIPACKET_s *)o->process290,
                                             "Base");
        } else {
          ResetAICreature(o, sys);
        }
        aicreature_sets_alive[set - 1]++;
      }
    }
    return 1;
  }
  if (obj != NULL) {
    if (obj->b3c8 == 0xff) {
      obj->flags1fc |= 0x1000;
      if (SetPlayerControlFn != NULL)
        SetPlayerControlFn(obj);
      else
        AIScriptSetBaseScriptStateByName((AIPACKET_s *)obj->process290, "Base");
    } else {
      ResetAICreature(obj, sys);
    }
    if (obj->process290[0xb4] != 0 && obj->process290[0xb4] <= 16)
      aicreature_sets_alive[obj->process290[0xb4] - 1]++;
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x00453dc0
i32 Action_CanDefend(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                     AIPACKET_s *packet, char **args, int argc, int flags,
                     f32 time) {
  if (packet == 0 || packet->pd0 == 0 || packet->pd0->obj == 0)
    return 1;
  GameObject_s *object = packet->pd0->obj;
  if (flags != 0) {
    *(u32 *)((u8 *)object + 0x140c) |= 0x20;
    for (i32 i = 0; i < argc; i++) {
      if (NuStrICmp(args[i], "false") == 0)
        *(u32 *)((u8 *)object + 0x140c) &= ~0x20;
    }
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x00453e50
i32 Action_DontAimAt(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                     AIPACKET_s *packet, char **args, int argc, int flags,
                     f32 time) {
  if (packet == 0 || packet->pd0 == 0 || packet->pd0->obj == 0)
    return 1;
  GameObject_s *object = packet->pd0->obj;
  if (flags != 0) {
    *(u32 *)((u8 *)object + 0x1414) |= 0x8;
    for (i32 i = 0; i < argc; i++) {
      if (NuStrICmp(args[i], "false") == 0)
        *(u32 *)((u8 *)object + 0x1414) &= ~0x8;
    }
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x00453ee0
i32 Action_ImmuneToKillTerrain(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                               AIPACKET_s *packet, char **args, int argc,
                               int flags, f32 time) {
  if (packet == 0 || packet->pd0 == 0 || packet->pd0->obj == 0)
    return 1;
  GameObject_s *object = packet->pd0->obj;
  if (flags != 0) {
    *(u32 *)((u8 *)object + 0x140c) |= 0x400000;
    for (i32 i = 0; i < argc; i++) {
      if (NuStrICmp(args[i], "false") == 0)
        *(u32 *)((u8 *)object + 0x140c) &= ~0x400000;
    }
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x00453f70
i32 Action_ImmuneToBolts(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                         AIPACKET_s *packet, char **args, int argc, int flags,
                         f32 time) {
  if (packet == 0 || packet->pd0 == 0 || packet->pd0->obj == 0)
    return 1;
  GameObject_s *object = packet->pd0->obj;
  if (flags != 0) {
    *(u32 *)((u8 *)object + 0x140c) |= 0x800000;
    for (i32 i = 0; i < argc; i++) {
      if (NuStrICmp(args[i], "false") == 0)
        *(u32 *)((u8 *)object + 0x140c) &= ~0x800000;
    }
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x004540d0
i32 Action_CanUseWeapon(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                        AIPACKET_s *packet, char **args, int argc, int flags,
                        f32 time) {
  if (packet == 0 || packet->pd0 == 0 || packet->pd0->obj == 0)
    return 1;
  GameObject_s *object = packet->pd0->obj;
  if (flags != 0) {
    *(u32 *)((u8 *)object + 0x140c) |= 0x80;
    for (i32 i = 0; i < argc; i++) {
      if (NuStrICmp(args[i], "false") == 0)
        *(u32 *)((u8 *)object + 0x140c) &= ~0x80;
    }
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x00454160
i32 Action_SetBoss(AISYS_s *sys, AISCRIPTPROCESS_s *process, AIPACKET_s *packet,
                   char **args, int argc, int flags, f32 time) {
  if (packet == 0 || packet->pd0 == 0 || packet->pd0->obj == 0)
    return 1;
  GameObject_s *object = packet->pd0->obj;
  if (flags != 0) {
    *(u32 *)((u8 *)object + 0x140c) |= 0x80000000;
    for (i32 i = 0; i < argc; i++) {
      if (NuStrICmp(args[i], "false") == 0)
        *(u32 *)((u8 *)object + 0x140c) &= ~0x80000000;
    }
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x00454c80
i32 Action_UpdateSockPos(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                         AIPACKET_s *packet, char **args, int argc, int flags,
                         f32 time) {
  if (packet == 0 || packet->pd0 == 0 || packet->pd0->obj == 0)
    return 1;
  GameObject_s *object = packet->pd0->obj;
  if (flags != 0) {
    *(u32 *)((u8 *)object + 0x140c) |= 0x40000;
    for (i32 i = 0; i < argc; i++) {
      if (NuStrICmp(args[i], "false") == 0)
        *(u32 *)((u8 *)object + 0x140c) &= ~0x40000;
    }
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x00460180
i32 Action_ApplyGravity(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                        AIPACKET_s *packet, char **args, int argc, int flags,
                        f32 time) {
  if (packet == 0 || packet->pd0 == 0 || packet->pd0->obj == 0)
    return 1;
  GameObject_s *object = packet->pd0->obj;
  if (flags != 0) {
    *(u32 *)((u8 *)object + 0x1410) &= ~0x8;
    for (i32 i = 0; i < argc; i++) {
      if (NuStrICmp(args[i], "FALSE") == 0)
        *(u32 *)((u8 *)object + 0x1410) |= 0x8;
    }
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x00460210
i32 Action_ConveyorOverride(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                            AIPACKET_s *packet, char **args, int argc,
                            int flags, f32 time) {
  if (packet == 0 || packet->pd0 == 0 || packet->pd0->obj == 0)
    return 1;
  GameObject_s *object = packet->pd0->obj;
  if (flags != 0) {
    *(u32 *)((u8 *)object + 0x1418) |= 0x800000;
    for (i32 i = 0; i < argc; i++) {
      if (NuStrICmp(args[i], "FALSE") == 0)
        *(u32 *)((u8 *)object + 0x1418) &= ~0x800000;
    }
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x004602a0
i32 Action_IgnoreShoveSystem(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                             AIPACKET_s *packet, char **args, int argc,
                             int flags, f32 time) {
  if (packet == 0 || packet->pd0 == 0 || packet->pd0->obj == 0)
    return 1;
  GameObject_s *object = packet->pd0->obj;
  if (flags != 0) {
    *(u32 *)((u8 *)object + 0x1410) |= 0x10;
    for (i32 i = 0; i < argc; i++) {
      if (NuStrICmp(args[i], "FALSE") == 0)
        *(u32 *)((u8 *)object + 0x1410) &= ~0x10;
    }
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x00460330
i32 Action_CannotBeForcedBack(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                              AIPACKET_s *packet, char **args, int argc,
                              int flags, f32 time) {
  if (packet == 0 || packet->pd0 == 0 || packet->pd0->obj == 0)
    return 1;
  GameObject_s *object = packet->pd0->obj;
  if (flags != 0) {
    *(u32 *)((u8 *)object + 0x1414) |= 0x1;
    for (i32 i = 0; i < argc; i++) {
      if (NuStrICmp(args[i], "FALSE") == 0)
        *(u32 *)((u8 *)object + 0x1414) &= ~0x1;
    }
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x004603c0
i32 Action_CanTurn(AISYS_s *sys, AISCRIPTPROCESS_s *process, AIPACKET_s *packet,
                   char **args, int argc, int flags, f32 time) {
  if (packet == 0 || packet->pd0 == 0 || packet->pd0->obj == 0)
    return 1;
  GameObject_s *object = packet->pd0->obj;
  if (flags != 0) {
    *(u32 *)((u8 *)object + 0x1414) |= 0x2;
    for (i32 i = 0; i < argc; i++) {
      if (NuStrICmp(args[i], "FALSE") == 0)
        *(u32 *)((u8 *)object + 0x1414) &= ~0x2;
    }
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x00460560
i32 Action_CannotBeSeen(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                        AIPACKET_s *packet, char **args, int argc, int flags,
                        f32 time) {
  if (packet == 0 || packet->pd0 == 0 || packet->pd0->obj == 0)
    return 1;
  GameObject_s *object = packet->pd0->obj;
  if (flags != 0) {
    *(u32 *)((u8 *)object + 0x1410) |= 0x20;
    for (i32 i = 0; i < argc; i++) {
      if (NuStrICmp(args[i], "FALSE") == 0)
        *(u32 *)((u8 *)object + 0x1410) &= ~0x20;
    }
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x00463450
i32 Action_OnlyActiveWhenTakenOver(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                   AIPACKET_s *packet, char **args, int argc,
                                   int flags, f32 time) {
  if (packet == 0 || packet->pd0 == 0 || packet->pd0->obj == 0)
    return 1;
  GameObject_s *object = packet->pd0->obj;
  if (flags != 0) {
    *(u32 *)((u8 *)object + 0x1580) |= 0x10;
    for (i32 i = 0; i < argc; i++) {
      if (NuStrICmp(args[i], "FALSE") == 0)
        *(u32 *)((u8 *)object + 0x1580) &= ~0x10;
    }
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x00470140
i32 Action_CanHelpWithTriggers(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                               AIPACKET_s *packet, char **args, int argc,
                               int flags, f32 time) {
  if (packet == 0 || packet->pd0 == 0 || packet->pd0->obj == 0)
    return 1;
  GameObject_s *object = packet->pd0->obj;
  if (flags != 0) {
    *(u32 *)((u8 *)object + 0x1414) |= 0x100000;
    for (i32 i = 0; i < argc; i++) {
      if (NuStrICmp(args[i], "FALSE") == 0)
        *(u32 *)((u8 *)object + 0x1414) &= ~0x100000;
    }
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x004702e0
i32 Action_CanCollideWithObjects(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                 AIPACKET_s *packet, char **args, int argc,
                                 int flags, f32 time) {
  if (packet == 0 || packet->pd0 == 0 || packet->pd0->obj == 0)
    return 1;
  GameObject_s *object = packet->pd0->obj;
  if (flags != 0) {
    *(u32 *)((u8 *)object + 0x1414) &= ~0x200000;
    for (i32 i = 0; i < argc; i++) {
      if (NuStrICmp(args[i], "TRUE") == 0)
        *(u32 *)((u8 *)object + 0x1414) |= 0x200000;
    }
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x00470730
i32 Action_SetShootOpponents(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                             AIPACKET_s *packet, char **args, int argc,
                             int flags, f32 time) {
  if (packet == 0 || packet->pd0 == 0 || packet->pd0->obj == 0)
    return 1;
  GameObject_s *object = packet->pd0->obj;
  if (flags != 0) {
    *(u32 *)((u8 *)object + 0x140c) |= 0x800;
    for (i32 i = 0; i < argc; i++) {
      if (NuStrICmp(args[i], "false") == 0)
        *(u32 *)((u8 *)object + 0x140c) &= ~0x800;
    }
  }
  return 1;
}

void Hint_CancelCurrent(void);

// FUNCTION: LEGOBATMAN 0x004611c0
i32 Action_CancelHint(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                      AIPACKET_s *packet, char **args, int argc, int flags,
                      f32 time) {
  Hint_CancelCurrent();
  return 1;
}

void GameCam_Reset(GAMECAMERA_s *camera);

// FUNCTION: LEGOBATMAN 0x00460ec0
i32 Action_ResetGameCamera(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                           AIPACKET_s *packet, char **args, int argc, int flags,
                           f32 time) {
  if (flags != 0)
    GameCam_Reset(g_unk0095f624);
  return 1;
}

// FUNCTION: LEGOBATMAN 0x0045bbf0
i32 Action_AddToSet(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                    AIPACKET_s *packet, char **args, int argc, int flags,
                    f32 time) {
  GameObject_s *obj;
  i32 i;
  if (flags == 0 || packet == NULL || packet->pd0 == NULL ||
      packet->pd0->obj == NULL)
    return 1;
  obj = packet->pd0->obj;
  if (obj != NULL) {
    for (i = 0; i < argc; i++) {
      if (NuStrICmp(args[i], "Reset") == 0) {
        obj->process290[0xb4] = 0;
        continue;
      }
      i32 set = (i32)AIParamToFloat(process, args[i]);
      if ((u32)(set - 1) <= 15) {
        obj->process290[0xb4] = (u8)set;
        aicreature_sets_alive[set - 1]++;
      }
    }
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x0045bf80
i32 Action_BreakFormation(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                          AIPACKET_s *packet, char **args, int argc, int flags,
                          f32 time) {
  // 0x144: the packet's formation; +0x50 bit 3: moving.
  if (packet != 0 && *(u8 **)((u8 *)packet + 0x144) != 0)
    *(u32 *)(*(u8 **)((u8 *)packet + 0x144) + 0x50) &= ~8;
  return 1;
}

// FUNCTION: LEGOBATMAN 0x0045bfb0
i32 Action_FormationMove(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                         AIPACKET_s *packet, char **args, int argc, int flags,
                         f32 time) {
  if (packet != 0 && *(u8 **)((u8 *)packet + 0x144) != 0)
    *(u32 *)(*(u8 **)((u8 *)packet + 0x144) + 0x50) |= 8;
  return 1;
}

void LetGoOfBalloon(GameObject_s *object);
void EatVictim(GameObject_s *object);
void ReleaseEat(GameObject_s *object);
void StartLaunch(GameObject_s *object);

// FUNCTION: LEGOBATMAN 0x0046f6e0
i32 Action_LetGoOfBalloon(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                          AIPACKET_s *packet, char **args, int argc, int flags,
                          f32 time) {
  if (packet != 0 && packet->pd0 != 0)
    LetGoOfBalloon(packet->pd0->obj);
  return 1;
}

// FUNCTION: LEGOBATMAN 0x0045a910
i32 Action_EatVictim(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                     AIPACKET_s *packet, char **args, int argc, int flags,
                     f32 time) {
  if (packet != 0 && packet->pd0 != 0 && packet->pd0->obj != 0)
    EatVictim(packet->pd0->obj);
  return 1;
}

// FUNCTION: LEGOBATMAN 0x0045a950
i32 Action_ReleaseVictim(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                         AIPACKET_s *packet, char **args, int argc, int flags,
                         f32 time) {
  if (packet != 0 && packet->pd0 != 0 && packet->pd0->obj != 0)
    ReleaseEat(packet->pd0->obj);
  return 1;
}

// FUNCTION: LEGOBATMAN 0x0045c530
i32 Action_Launch(AISYS_s *sys, AISCRIPTPROCESS_s *process, AIPACKET_s *packet,
                  char **args, int argc, int flags, f32 time) {
  if (flags != 0 && packet != 0 && packet->pd0 != 0 && packet->pd0->obj != 0)
    StartLaunch(packet->pd0->obj);
  return 1;
}

// FUNCTION: LEGOBATMAN 0x0045aed0
i32 Action_UseWeapon(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                     AIPACKET_s *packet, char **args, int argc, int flags,
                     f32 time) {
  if (packet != 0 && packet->pd0 != 0 && packet->pd0->obj != 0) {
    GameObject_s *object = packet->pd0->obj;
    if (object != 0)
      *(u32 *)((u8 *)object + 0x1410) |= 2;
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x00462b40
i32 Action_AIScriptAnimContextReset(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                    AIPACKET_s *packet, char **args, int argc,
                                    int flags, f32 time) {
  if (packet != 0 && packet->pd0 != 0 && packet->pd0->obj != 0) {
    GameObject_s *object = packet->pd0->obj;
    if (object != 0)
      object->b9db = 0x73;
  }
  return 1;
}

extern "C" void SetAnimTimeRandom(void *anim, void *time);

// FUNCTION: LEGOBATMAN 0x00462b80
i32 Action_AnimTimeRandom(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                          AIPACKET_s *packet, char **args, int argc, int flags,
                          f32 time) {
  if (packet != 0 && packet->pd0 != 0 && packet->pd0->obj != 0) {
    GameObject_s *object = packet->pd0->obj;
    SetAnimTimeRandom(object->p50, (u8 *)object + 8);
  }
  return 1;
}

// Pad button bits ORed into the AI pad (+0x112c, +8).
// GLOBAL: LEGOBATMAN 0x0095f734
extern u32 g_unk0095f734;
// GLOBAL: LEGOBATMAN 0x0095f728
extern u32 g_unk0095f728;

// FUNCTION: LEGOBATMAN 0x0045ae40
i32 Action_PressTagButton(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                          AIPACKET_s *packet, char **args, int argc, int flags,
                          f32 time) {
  if (packet != 0 && packet->pd0 != 0 && packet->pd0->obj != 0) {
    GameObject_s *object = packet->pd0->obj;
    if (object != 0)
      *(u32 *)((u8 *)object->p112c + 8) |= g_unk0095f734;
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x0045ae80
i32 Action_PressActionButton(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                             AIPACKET_s *packet, char **args, int argc,
                             int flags, f32 time) {
  if (packet != 0 && packet->pd0 != 0 && packet->pd0->obj != 0) {
    GameObject_s *object = packet->pd0->obj;
    if (object != 0) {
      *(u32 *)((u8 *)object->p112c + 8) |= g_unk0095f72c;
      object->flags140c |= 0x4200;
    }
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x0045bbb0
i32 Action_PressJumpButton(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                           AIPACKET_s *packet, char **args, int argc, int flags,
                           f32 time) {
  if (packet != 0 && packet->pd0 != 0 && packet->pd0->obj != 0) {
    GameObject_s *object = packet->pd0->obj;
    if (object != 0)
      *(u32 *)((u8 *)object->p112c + 8) |= g_unk0095f728;
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x004539c0
i32 Action_ClearTakeOverTarget(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                               AIPACKET_s *packet, char **args, int argc,
                               int flags, f32 time) {
  // 0x13bc: take-over target, linked both ways.
  if (flags != 0) {
    GameObject_s *object = packet->pd0->obj;
    if (object != 0) {
      GameObject_s *target = *(GameObject_s **)((u8 *)object + 0x13bc);
      if (target != 0) {
        *(GameObject_s **)((u8 *)object + 0x13bc) = 0;
        *(GameObject_s **)((u8 *)target + 0x13bc) = 0;
      }
    }
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x00471c50
i32 Action_ResetContextAIAnimation(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                   AIPACKET_s *packet, char **args, int argc,
                                   int flags, f32 time) {
  if (flags != 0 && packet != 0 && packet->pd0 != 0 && packet->pd0->obj != 0) {
    GameObject_s *object = packet->pd0->obj;
    if (object != 0 && object->b9db == 0x73)
      object->b9db = -1;
  }
  return 1;
}

// GLOBAL: LEGOBATMAN 0x00a9637c
extern i32 g_unk00a9637c;

// FUNCTION: LEGOBATMAN 0x004611d0
i32 Action_DisableNarrowSocks(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                              AIPACKET_s *packet, char **args, int argc,
                              int flags, f32 time) {
  if (args != 0 && argc != 0 && args[0] != 0 &&
      NuStrICmp("FALSE", args[0]) == 0) {
    g_unk00a9637c = 0;
    return 1;
  }
  g_unk00a9637c = 1;
  return 1;
}

extern GameObject_s *Player[8];
i32 SuperCarry_Carrying(GameObject_s *object);
void SuperCarry_Release(GameObject_s *object);

// FUNCTION: LEGOBATMAN 0x00471c00
i32 Action_ForceDropSuperCarryItems(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                    AIPACKET_s *packet, char **args, int argc,
                                    int flags, f32 time) {
  for (i32 i = 0; i < 2; i++) {
    if (Player[i] != 0 && (((u8 *)Player[i])[0x1fc] & 0x80) &&
        SuperCarry_Carrying(Player[i]) != 0)
      SuperCarry_Release(Player[i]);
  }
  return 1;
}

// GLOBAL: LEGOBATMAN 0x00960594
extern char g_unk00960594;

// FUNCTION: LEGOBATMAN 0x00472ff0
i32 Action_GetOutRideObject(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                            AIPACKET_s *packet, char **args, int argc,
                            int flags, f32 time) {
  GameObject_s *object = 0;
  if (packet != 0 && packet->pd0 != 0 && packet->pd0->obj != 0)
    object = packet->pd0->obj;
  if (object == 0)
    return 0;
  char context = g_unk00960594;
  *(f32 *)((u8 *)object + 0x98c) = 0.0f;
  object->b9db = context;
  object->b9d9 = 3;
  object->s9d0 = -1;
  return 1;
}

// GLOBAL: LEGOBATMAN 0x009ccae0
extern i32 g_unk009ccae0;

// FUNCTION: LEGOBATMAN 0x0046f820
i32 Action_AlwaysDrawBossHitPoints(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                   AIPACKET_s *packet, char **args, int argc,
                                   int flags, f32 time) {
  g_unk009ccae0 = 1;
  if (flags != 0) {
    for (i32 i = 0; i < argc; i++) {
      if (NuStrICmp(args[i], "FALSE") == 0)
        g_unk009ccae0 = 0;
    }
  }
  return 1;
}

// GLOBAL: LEGOBATMAN 0x00aca8f8
extern i32 g_unk00aca8f8;

// FUNCTION: LEGOBATMAN 0x004728d0
i32 Action_NoFightingZone(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                          AIPACKET_s *packet, char **args, int argc, int flags,
                          f32 time) {
  if (flags != 0) {
    g_unk00aca8f8 = 1;
    for (i32 i = 0; i < argc; i++) {
      if (NuStrIStr(args[i], "FALSE") != 0)
        g_unk00aca8f8 = 0;
    }
  }
  return 1;
}

// GLOBAL: LEGOBATMAN 0x009c61c4
extern i32 g_unk009c61c4;

// FUNCTION: LEGOBATMAN 0x00472940
i32 Action_AllowFightingInMiniCut(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                  AIPACKET_s *packet, char **args, int argc,
                                  int flags, f32 time) {
  if (flags != 0) {
    g_unk009c61c4 = 1;
    for (i32 i = 0; i < argc; i++) {
      if (NuStrIStr(args[i], "FALSE") != 0)
        g_unk009c61c4 = 0;
    }
  }
  return 1;
}

extern PART_s *Part;
extern i32 MAXPARTS;
void KillPart(PART_s *part, i32 reason);
void PartKill_ForceThrow(PART_s *part, i32 reason);

// FUNCTION: LEGOBATMAN 0x0045c050
i32 Action_RemoveThrownForceObjects(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                    AIPACKET_s *packet, char **args, int argc,
                                    int flags, f32 time) {
  PART_s *part = Part;
  for (i32 i = 0; i < MAXPARTS; i++, part++) {
    if ((part->flags148 & 1) != 0 && part->kill_callback == PartKill_ForceThrow)
      KillPart(part, 0);
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x0045fbe0
i32 Action_SetDefaultMovementRange(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                   AIPACKET_s *packet, char **args, int argc,
                                   int flags, f32 time) {
  if (flags != 0) {
    for (i32 i = 0; i < argc; i++)
      DEFAULT_MOVE_RANGE = AIParamToFloat(process, args[i]);
  }
  return 1;
}

void Unk005d52f0(i32 id);

// FUNCTION: LEGOBATMAN 0x00461140
i32 Action_SetHintComplete(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                           AIPACKET_s *packet, char **args, int argc, int flags,
                           f32 time) {
  i32 id = -1;
  if (flags != 0) {
    for (i32 i = 0; i < argc; i++) {
      char *s = NuStrIStr(args[i], "id");
      if (s != 0)
        id = (i32)AIParamToFloat(process, s + 3);
    }
    Unk005d52f0(id);
  }
  return 1;
}

// GLOBAL: LEGOBATMAN 0x009c5fe8
extern i32 party_cant_be_under_cover;

// FUNCTION: LEGOBATMAN 0x004707c0
i32 Action_PartyCanBeUnderCover(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                AIPACKET_s *packet, char **args, int argc,
                                int flags, f32 time) {
  if (flags != 0) {
    party_cant_be_under_cover = 0;
    for (i32 i = 0; i < argc; i++) {
      if (NuStrICmp(args[i], "FALSE") == 0)
        party_cant_be_under_cover = 1;
    }
  }
  return 1;
}

struct GIZMOBLOWUP_s {
  u8 pad0[0xa0];
  u8 output_flags; // 0xa0, bit 0: already blown up
};

void GizmoBlowupBlowup(GIZMOBLOWUP_s *blowup, i32 a, i32 b, i32 damage,
                       GameObject_s *obj, i32 c);

// FUNCTION: LEGOBATMAN 0x00464f80
i32 Action_TriggerBlowUp(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                         AIPACKET_s *packet, char **args, int argc, int flags,
                         f32 time) {
  GIZMOBLOWUP_s *blowup = NULL;
  if (flags != 0) {
    i32 i;
    for (i = 0; i < argc; i++) {
      char *value = NuStrIStr(args[i], "name=");
      if (value != NULL) {
        GIZMO_s *gizmo = GizmoFindByName(g_unk00960894->gizmoSys2b0c,
                                         blowup_gizmotype_id, value + 5);
        if (gizmo != NULL)
          blowup = (GIZMOBLOWUP_s *)gizmo->object;
      }
    }
    if (blowup != NULL && (blowup->output_flags & 1) == 0)
      GizmoBlowupBlowup(blowup, 1, -1, 1, NULL, 1);
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x00465730
i32 Action_DeflectPlayersPart(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                              AIPACKET_s *packet, char **args, int argc,
                              int flags, f32 time) {
  if (packet == 0 || packet->pd0 == 0 || packet->pd0->obj == 0)
    return 1;
  if (flags != 0) {
    GameObject_s *obj = packet->pd0->obj;
    obj->flags1410_lo |= 0x1000;
    if (argc != 0) {
      for (i32 i = 0; i < argc; i++) {
        if (NuStrICmp(args[i], "FALSE") == 0)
          obj->flags1410_lo &= ~0x1000;
      }
    }
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x00467eb0
i32 Action_DontRaycastLOS(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                          AIPACKET_s *packet, char **args, int argc, int flags,
                          f32 time) {
  if (flags != 0 && g_unk00960894->api_object_sys != 0) {
    g_unk00960894->api_object_sys->flags210 |= 1;
    for (i32 i = 0; i < argc; i++) {
      if (NuStrICmp(args[i], "false") == 0)
        g_unk00960894->api_object_sys->flags210 &= ~1;
    }
  }
  return 1;
}

void Unk00448b80(f32 dist);

// FUNCTION: LEGOBATMAN 0x0046e840
i32 Action_UseTimeBasedUpdate(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                              AIPACKET_s *packet, char **args, int argc,
                              int flags, f32 time) {
  i32 enabled = 0;
  if (packet != NULL && packet->pd0 != NULL) {
    GameObject_s *obj = packet->pd0->obj;
    if (flags) {
      i32 i;
      for (i = 0; i < argc; i++) {
        if (NuStrICmp(args[i], "TRUE") == 0)
          enabled = 1;
        else if (NuStrICmp(args[i], "FALSE") == 0)
          enabled = 0;
      }
    }
    obj->no_time_based_update = enabled == 0;
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x0046e900
i32 Action_WalkBackwards(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                         AIPACKET_s *packet, char **args, int argc, int flags,
                         f32 time) {
  if (packet == NULL)
    return 1;
  if (flags != 0) {
    packet->movement_param = 1.0f;
    for (i32 i = 0; i < argc; i++) {
      if (AIActionParseSpeedFn == 0 ||
          AIActionParseSpeedFn(args[i], &packet->goal_speed_mode) == 0)
        packet->movement_param = AIParamToFloatEx(packet, process, args[i]);
    }
  }
  if (packet->pe4 != NULL && packet->pe4->ai != NULL) {
    GameObject_s *owner = packet->pd0->obj;
#define OPPONENT ((u8 *)packet->pe4->ai) // the opponent's AIPACKET_s
    AIMoveInstruction(
        packet, (nuvec_s *)(OPPONENT + 0x174), *(f32 *)(OPPONENT + 0x120),
        (AIPATHINFO *)(OPPONENT + 0x158), 2, packet->movement_param);
#undef OPPONENT
    *(u32 *)((u8 *)owner + 0x1410) |= 0x80000;
    owner->goal_speed_mode = 1; // WALK
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x0046f160
i32 Action_SetUseOneAtOnce(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                           AIPACKET_s *packet, char **args, int argc, int flags,
                           f32 time) {
  GameObject_s *obj = NULL;
  i32 enabled = 1;
  if (flags != 0) {
    if (packet != NULL)
      obj = packet->pd0->obj;
    if (argc != 0) {
      for (i32 i = 0; i < argc; i++) {
        char *value = NuStrIStr(args[i], "character");
        if (value != NULL) {
          value += 10;
          if (NuStrICmp(value, "myself") == 0 && packet != NULL)
            obj = packet->pd0->obj;
          else
            obj = GetNamedGameObjectInl(g_unk00960894->aiSys2bf8, value);
        } else if (NuStrICmp(args[i], "FALSE") == 0) {
          enabled = 0;
        }
      }
    }
    if (obj != NULL)
      obj->use_one_at_once = enabled;
  }
  return 1;
}

// STUB: LEGOBATMAN 0x0046fc00
// from the goalrange call on, temporaries land one register later than the
// original (eax/ecx/edx rotated); layout matches (6 tries)
i32 Action_CircleLocator(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                         AIPACKET_s *packet, char **args, int argc, int flags,
                         f32 time) {
  u8 *locator;
  if (packet == NULL || packet->pd0 == NULL || packet->pd0->obj == NULL ||
      packet->path_set == NULL || packet->path_node == NULL)
    return 1;
  if (flags != 0) {
    process->action_data_1 = 0;
    process->action_data_3 = process->locator;
    if (argc != 0) {
      for (i32 i = 0; i < argc; i++) {
        char *value;
        if (AIActionParseSpeedFn != 0 &&
            AIActionParseSpeedFn(args[i], &packet->goal_speed_mode) != 0)
          continue;
        if ((value = NuStrIStr(args[i], "name=")) != NULL ||
            (value = NuStrIStr(args[i], "teleport")) != NULL) {
          process->action_data_3 =
              AIPathFindLocator(sys, value + NuStrLen("name="));
        } else if ((value = NuStrIStr(args[i], "goalrange")) != NULL) {
          packet->movement_param = AIParamToFloatEx(
              packet, process, value + NuStrLen("goalrange") + 1);
        } else if (NuStrICmp(args[i], "reverse") == 0) {
          packet->circle_clockwise ^= 1;
        } else if (NuStrICmp(args[i], "anticlockwise") == 0) {
          packet->circle_clockwise = 0;
        } else if (NuStrICmp(args[i], "clockwise") == 0) {
          packet->circle_clockwise = 1;
        } else {
          packet->movement_param = AIParamToFloatEx(packet, process, args[i]);
        }
      }
    }
  }
  locator = (u8 *)process->action_data_3;
  if (locator != NULL) {
    AIMoveInstruction(packet, (nuvec_s *)(locator + 0x10),
                      packet->movement_param, (AIPATHINFO *)(locator + 0x20), 3,
                      packet->movement_param);
    return 0;
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x0046f470
i32 Action_SetAO_InitRowDist(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                             AIPACKET_s *packet, char **args, int argc,
                             int flags, f32 time) {
  if (flags != 0) {
    for (i32 i = 0; i < argc; i++) {
      char *s = NuStrIStr(args[i], "Dist");
      if (s != 0)
        Unk00448b80(AIParamToFloat(process, s + 5));
    }
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x00472830
i32 Action_PlayerItemIgnoreLOS(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                               AIPACKET_s *packet, char **args, int argc,
                               int flags, f32 time) {
  i32 on = 1;
  if (packet == 0 || packet->pd0 == 0)
    return on;
  GameObject_s *obj = packet->pd0->obj;
  if (flags != 0) {
    for (i32 i = 0; i < argc; i++) {
      if (NuStrIStr(args[i], "FALSE") != 0)
        on = 0;
    }
  }
  obj->item_ignore_los = on;
  return 1;
}

// FUNCTION: LEGOBATMAN 0x004633a0
i32 Action_CanShootOffScreen(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                             AIPACKET_s *packet, char **args, int argc,
                             int flags, f32 time) {
  if (packet == 0 || packet->pd0 == 0 || packet->pd0->obj == 0)
    return 1;
  GameObject_s *obj = packet->pd0->obj;
  if (flags != 0) {
    obj->flags1580 |= 4;
    for (i32 i = 0; i < argc; i++) {
      if (NuStrICmp(args[i], "TRUE") == 0)
        obj->flags1580 |= 4;
      else if (NuStrICmp(args[i], "FALSE") == 0)
        obj->flags1580 &= ~4;
    }
  }
  return 1;
}

u16 GetSfxId(char *name);

// FUNCTION: LEGOBATMAN 0x00465a10
i32 Action_SetLoopingSfx(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                         AIPACKET_s *packet, char **args, int argc, int flags,
                         f32 time) {
  i32 sfx = -1;
  char *name = 0;
  if (flags != 0) {
    for (i32 i = 0; i < argc; i++) {
      char *s = NuStrIStr(args[i], "name=");
      if (s != 0)
        name = s + 5;
    }
  }
  GameObject_s *obj = packet->pd0->obj;
  if (obj != 0) {
    if (name != 0)
      sfx = GetSfxId(name);
    obj->looping_sfx = sfx;
  }
  return 1;
}

struct NuMusic {
  i32 GetTrackHandle(u32 type, const char *name);
  void SelectTrackByHandle(u32 type, i32 handle);
  void PlayTrack(u32 type);
};

extern NuMusic music_man;

// FUNCTION: LEGOBATMAN 0x004687f0
i32 Action_PlayOverlay(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                       AIPACKET_s *packet, char **args, int argc, int flags,
                       f32 time) {
  char *name = 0;
  if (flags != 0) {
    for (i32 i = 0; i < argc; i++) {
      char *s = NuStrIStr(args[i], "name");
      if (s != 0)
        name = s + NuStrLen("name") + 1;
    }
    if (name != 0) {
      i32 handle = music_man.GetTrackHandle(8, name);
      if (handle != -1) {
        music_man.SelectTrackByHandle(8, handle);
        music_man.PlayTrack(8);
      }
    }
  }
  return 1;
}

void RegisterTakeOverObject(GameObject_s *obj);

// FUNCTION: LEGOBATMAN 0x0046cce0
i32 Action_RegisterTakeOverObject(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                  AIPACKET_s *packet, char **args, int argc,
                                  int flags, f32 time) {
  GameObject_s *obj = 0;
  if (flags == 0)
    return 1;
  if (packet != 0 && packet->pd0 != 0 && packet->pd0->obj != 0)
    obj = packet->pd0->obj;
  for (i32 i = 0; i < argc; i++) {
    char *s = NuStrIStr(args[i], "character=");
    if (s != 0)
      obj = GetNamedGameObjectInl(sys, s + 10);
  }
  if (obj != 0)
    RegisterTakeOverObject(obj);
  return 1;
}

void PlayerItems_DropCurrentItem(GameObject_s *obj, i32 can_pickup_again,
                                 nuvec_s *vel);

// FUNCTION: LEGOBATMAN 0x00453900
i32 Action_DropCurrentItem(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                           AIPACKET_s *packet, char **args, int argc, int flags,
                           f32 time) {
  if (packet == 0 || packet->pd0 == 0 || packet->pd0->obj == 0)
    return 1;
  nuvec_s vel;
  vel.x = vel.y = vel.z = 0.0f;
  i32 can_pickup_again = 0;
  if (flags != 0) {
    GameObject_s *obj = packet->pd0->obj;
    for (i32 i = 0; i < argc; i++) {
      if (NuStrICmp(args[i], "can_pickup_again") == 0)
        can_pickup_again = 1;
    }
    PlayerItems_DropCurrentItem(obj, can_pickup_again, &vel);
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x0046ba60
i32 Action_ReleaseLocator(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                          AIPACKET_s *packet, char **args, int argc, int flags,
                          f32 time) {
  GameObject_s *obj = 0;
  if (flags != 0) {
    if (packet != 0 && packet->pd0 != 0 && packet->pd0->obj != 0)
      obj = packet->pd0->obj;
    for (i32 i = 0; i < argc; i++) {
      char *s = NuStrIStr(args[i], "character=");
      if (s != 0)
        obj = GetNamedGameObjectInl(sys, s + 10);
    }
    if (obj != 0)
      *(void **)(obj->process290 + 0xa8) = 0;
  }
  return 1;
}

struct sGizFlockAntinode;
sGizFlockAntinode *GizFlock_FindAntinodeByName(const char *name);
void GizFlock_ActivateAntinode(sGizFlockAntinode *antinode, u32 active);

// FUNCTION: LEGOBATMAN 0x00472580
i32 Action_SetFlockAntinodeActive(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                  AIPACKET_s *packet, char **args, int argc,
                                  int flags, f32 time) {
  if (argc != 0) {
    sGizFlockAntinode *antinode = 0;
    u32 active = 1;
    for (i32 i = 0; i < argc; i++) {
      char *s = NuStrIStr(args[i], "name=");
      if (s != 0) {
        s += NuStrLen("name=");
        antinode = GizFlock_FindAntinodeByName(s);
      } else if (NuStrICmp(args[i], "FALSE") == 0)
        active = 0;
    }
    if (antinode != 0)
      GizFlock_ActivateAntinode(antinode, active);
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x004729b0
i32 Action_SetDontDrawNumFrames(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                AIPACKET_s *packet, char **args, int argc,
                                int flags, f32 time) {
  i32 frames = 1;
  if (packet == 0 || packet->pd0 == 0)
    return 1;
  GameObject_s *obj = packet->pd0->obj;
  for (i32 i = 0; i < argc; i++) {
    char *s = NuStrIStr(args[i], "num_frames");
    if (s != 0) {
      s = s + NuStrLen("num_frames") + 1;
      frames = (i32)AIParamToFloatEx(packet, process, s);
    }
  }
  obj->dont_draw_frames = frames;
  return 1;
}

void Player_ResetContexts(GameObject_s *obj);

// FUNCTION: LEGOBATMAN 0x00463780
i32 Action_ResetContext(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                        AIPACKET_s *packet, char **args, int argc, int flags,
                        f32 time) {
  GameObject_s *obj = 0;
  if (flags != 0) {
    if (packet != 0 && packet->pd0 != 0)
      obj = packet->pd0->obj;
    if (argc != 0) {
      for (i32 i = 0; i < argc; i++) {
        char *s = NuStrIStr(args[i], "character=");
        if (s != 0)
          obj = GetNamedGameObjectInl(sys, s + 10);
      }
    }
    if (obj != 0) {
      Player_ClearContext(obj, 1);
      Player_ResetContexts(obj);
    }
  }
  return 1;
}

struct Unk_WorldInfo5220Entry;
Unk_WorldInfo5220Entry *GizmoPickup_FindByName(WORLDINFO_s *world, char *name);
void GizmoPickup_TurnOnPickup(Unk_WorldInfo5220Entry *pickup, i32 on);

// FUNCTION: LEGOBATMAN 0x00470070
i32 Action_TurnOnPickup(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                        AIPACKET_s *packet, char **args, int argc, int flags,
                        f32 time) {
  Unk_WorldInfo5220Entry *pickup = 0;
  i32 on = 1;
  if (flags != 0) {
    for (i32 i = 0; i < argc; i++) {
      char *s = NuStrIStr(args[i], "name=");
      if (s != 0) {
        s += NuStrLen("name=");
        pickup = GizmoPickup_FindByName(g_unk00960894, s);
      } else if (NuStrICmp(args[i], "OFF") == 0) {
        on = 0;
      }
    }
    if (pickup != 0)
      GizmoPickup_TurnOnPickup(pickup, on);
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x00470a10
i32 Action_SetCanBeMindControlled(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                  AIPACKET_s *packet, char **args, int argc,
                                  int flags, f32 time) {
  i32 on = 1;
  if (flags != 0) {
    GameObject_s *obj = packet != 0 && packet->pd0 != 0 && packet->pd0->obj != 0
                            ? packet->pd0->obj
                            : 0;
    if (obj == 0)
      return 1;
    for (i32 i = 0; i < argc; i++) {
      if (NuStrICmp("True", args[i]) == 0)
        on = 1;
      else if (NuStrIStr(args[i], "false") != 0)
        on = 0;
    }
    obj->can_be_mind_controlled = on;
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x00454000
i32 Action_Respawnable(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                       AIPACKET_s *packet, char **args, int argc, int flags,
                       f32 time) {
  if (packet == 0 || packet->pd0 == 0 || packet->pd0->obj == 0)
    return 1;
  GameObject_s *obj = packet->pd0->obj;
  if (flags != 0) {
    obj->respawnable = 1;
    obj->respawn_at_origin = 0;
    for (i32 i = 0; i < argc; i++) {
      if (NuStrICmp(args[i], "origin") == 0)
        obj->respawn_at_origin = 1;
      else if (NuStrICmp(args[i], "false") == 0)
        obj->respawnable = 0;
    }
  }
  return 1;
}

void GrabVictim(GameObject_s *obj, GameObject_s *victim);

// FUNCTION: LEGOBATMAN 0x00457d70
i32 Action_SnapToOrigin(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                        AIPACKET_s *packet, char **args, int argc, int flags,
                        f32 time) {
  GameObject_s *obj = NULL;
  i32 check_terrain = 1;
  AICREATURE_s *creature;
  if (flags == 0)
    return 1;
  if (packet != NULL && packet->pd0 != NULL && packet->pd0->obj != NULL)
    obj = packet->pd0->obj;
  for (i32 i = 0; i < argc; i++) {
    char *value = NuStrIStr(args[i], "character");
    if (value != NULL)
      obj = GetNamedGameObjectInl(sys, value + 10);
    else if (NuStrIStr(args[i], "dont_check_terrain") != NULL)
      check_terrain = 0;
  }
  if (obj == NULL || (obj->flags1f8 & 0x400) == 0 || obj->b3c8 == 0xff)
    return 1;
  creature = &(*(AICREATURE_s **)((u8 *)sys + 0x224))[obj->b3c8];
  obj->position = creature->pos;
  obj->u246 = creature->y_rot;
  obj->yaw58 = creature->y_rot;
  obj->facing_angle = creature->y_rot;
  obj->path_flags3f6 &= ~1;
  obj->v80 = obj->position;
  obj->v98 = obj->position;
  plr_lastpos = obj->v80;
  obj->v8c = obj->v80;
  ((Unk_AIPacketObj *)obj)->respawn_pos = obj->position;
  ((Unk_AIPacketObj *)obj)->last_safe_pos = obj->position;
  obj->saved_position = obj->position;
  obj->velocity = v000;
  InitSurfaceInfo(obj);
  if (check_terrain)
    SetObjOnSurface(obj, 0);
  return 1;
}

// FUNCTION: LEGOBATMAN 0x0045a830
i32 Action_GrabVictim(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                      AIPACKET_s *packet, char **args, int argc, int flags,
                      f32 time) {
  GameObject_s *victim = 0;
  if (packet == 0 || packet->pd0 == 0 || packet->pd0->obj == 0)
    return 1;
  GameObject_s *obj = packet->pd0->obj;
  if (flags != 0) {
    Unk_AIPacketObj *opponent = *(Unk_AIPacketObj **)(obj->process290 + 0xe4);
    if (opponent != 0)
      victim = opponent->obj;
    for (i32 i = 0; i < argc; i++) {
      char *s = NuStrIStr(args[i], "victim=");
      if (s != 0)
        victim = GetNamedGameObjectInl(sys, s + 7);
    }
    if (victim != 0)
      GrabVictim(obj, victim);
  }
  return 1;
}

void GameCam_NewShake(GAMECAMERA_s *camera, f32 amount, f32 duration,
                      f32 speed);

// FUNCTION: LEGOBATMAN 0x00460de0
i32 Action_CameraShake(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                       AIPACKET_s *packet, char **args, int argc, int flags,
                       f32 time) {
  if (flags != 0) {
    f32 amount = 1.0f;
    f32 duration = 1.0f;
    f32 speed = 1.0f;
    for (i32 i = 0; i < argc; i++) {
      char *s = NuStrIStr(args[i], "mul=");
      if (s != 0)
        amount = AIParamToFloat(process, s + 4);
      else if ((s = NuStrIStr(args[i], "time=")) != 0)
        duration = AIParamToFloat(process, s + 5);
      else if ((s = NuStrIStr(args[i], "speed=")) != 0)
        speed = AIParamToFloat(process, s + 6);
    }
    // the parsed values are dead: the shake always uses 1, 1, 1
    GameCam_NewShake(g_unk0095f624, 1.0f, 1.0f, 1.0f);
  }
  return 1;
}

void GizObstacle_StopLooping(GIZOBSTACLE_s *obstacle, i32 on);

// FUNCTION: LEGOBATMAN 0x00462db0
i32 Action_StopObstacleLooping(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                               AIPACKET_s *packet, char **args, int argc,
                               int flags, f32 time) {
  GIZOBSTACLE_s *obstacle = 0;
  i32 on = 1;
  if (flags != 0 && argc != 0) {
    for (i32 i = 0; i < argc; i++) {
      char *s = NuStrIStr(args[i], "name=");
      if (s != 0) {
        GIZMO_s *gizmo = GizmoFindByName(g_unk00960894->gizmoSys2b0c,
                                         obstacle_gizmotype_id, s + 5);
        if (gizmo != 0)
          obstacle = (GIZOBSTACLE_s *)gizmo->object;
      } else if (NuStrICmp(args[i], "FALSE") == 0) {
        on = 0;
      }
    }
    if (obstacle != 0)
      GizObstacle_StopLooping(obstacle, on);
  }
  return 1;
}

// GLOBAL: LEGOBATMAN 0x00acb118
extern f32 ObstacleCamEnd;
// GLOBAL: LEGOBATMAN 0x00acb11c
extern f32 ObstacleCamBlendOutTime;
// GLOBAL: LEGOBATMAN 0x00acb690
extern f32 ObstacleCamTime;
extern i32 MiniCutCam;

// FUNCTION: LEGOBATMAN 0x004677c0
i32 Action_EndCameraCut(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                        AIPACKET_s *packet, char **args, int argc, int flags,
                        f32 time) {
  if (flags != 0 && MiniCutCam != 0) {
    for (i32 i = 0; i < argc; i++) {
      char *s = NuStrIStr(args[i], "end_time=");
      if (s != 0) {
        ObstacleCamEnd = AIParamToFloat(process, s + 9);
        ObstacleCamEnd += ObstacleCamTime;
      } else {
        s = NuStrIStr(args[i], "blend_out_time=");
        if (s != 0)
          ObstacleCamBlendOutTime = AIParamToFloat(process, s + 15);
      }
    }
  }
  return 1;
}

struct GIZMOBLOWUP_s;
GIZMOBLOWUP_s *GizmoBlowUp_FindByName(WORLDINFO_s *world, char *name);
void SuperCarry_BlowUp(GIZMOBLOWUP_s *blowup, i32 flags);

// FUNCTION: LEGOBATMAN 0x00471ca0
i32 Action_BlowupSuperCarryItem(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                AIPACKET_s *packet, char **args, int argc,
                                int flags, f32 time) {
  GIZMOBLOWUP_s *blowup = 0;
  i32 blowup_flags = 0;
  if (argc != 0) {
    for (i32 i = 0; i < argc; i++) {
      char *s = NuStrIStr(args[i], "blowup_name=");
      if (s != 0) {
        s += NuStrLen("blowup_name=");
        blowup = GizmoBlowUp_FindByName(g_unk00960894, s);
      } else if (NuStrIStr(args[i], "debris") != 0) {
        blowup_flags |= 1;
      } else if (NuStrIStr(args[i], "parts") != 0) {
        blowup_flags |= 2;
      }
    }
    if (blowup != 0)
      SuperCarry_BlowUp(blowup, blowup_flags);
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x00470640
i32 Action_RestockTorpedos(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                           AIPACKET_s *packet, char **args, int argc, int flags,
                           f32 time) {
  GameObject_s *obj = 0;
  i32 count = 0;
  if (packet && packet->pd0 && packet->pd0->obj)
    obj = packet->pd0->obj;
  if (flags) {
    count = (i32)AIParamToFloat(process, args[0]);
    if (count < 0)
      count = 0;
    else if (count > getMaxTorpedos(0))
      count = getMaxTorpedos(0);
  }
  if (obj && obj->torpedo && count) {
    if (obj->torpedo->count == 0) {
      obj->torpedo->field_84 = 0;
      obj->torpedo->field_04 = 0;
      obj->torpedo->field_03 = 0;
    }
    if (count > obj->torpedo->count)
      obj->torpedo->count = count;
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x00455990
i32 Action_SetAIOverrideControl(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                AIPACKET_s *packet, char **args, int argc,
                                int flags, f32 time) {
  Unk_AIPacketObj *object = NULL;
  i32 enabled = 1;
  if (flags != 0) {
    if (packet != NULL && packet->pd0 != NULL)
      object = packet->pd0;
    for (i32 i = 0; i < argc; i++) {
      char *value = NuStrIStr(args[i], "character");
      if (value != NULL) {
        if (GetNamedAPIObjectFn != NULL)
          object = GetNamedAPIObjectFn(sys, value + 10);
      } else if (NuStrICmp("FALSE", args[i]) == 0) {
        enabled = 0;
      }
    }
    process->action_data_3 = object;
  }
  object = (Unk_AIPacketObj *)process->action_data_3;
  if (object != NULL) {
    if (enabled) {
      GameObject_s *obj = object->obj;
      if (obj != NULL) {
        obj->f1544 = 0.0f;
        obj->f153c = 0.0f;
        if (obj->b257) {
          obj->b257 = 0;
          obj->current_hp = obj->hitpoints;
          obj->shield_hitpoints = obj->p54->p24->shield_hitpoints;
          obj->shield_state = 4;
        }
        if (obj->p1158 != NULL) {
          obj->p1158->f1544 = 0.0f;
          obj->p1158->f153c = 0.0f;
          if (obj->p1158->b257) {
            obj->p1158->b257 = 0;
            obj->p1158->current_hp = obj->p1158->hitpoints;
            obj->p1158->shield_hitpoints =
                obj->p1158->p54->p24->shield_hitpoints;
            obj->p1158->shield_state = 4;
          }
        }
      }
    }
    ((GameObject_s *)object)->ai_override_control = enabled;
  }
  return 1;
}

// STUB: LEGOBATMAN 0x00455b90
// original keeps a separate epilogue after the locator branch and puts
// object in ebx / args in edi; ours merges and swaps them (3 tries)
i32 Action_SetLastSafePathPos(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                              AIPACKET_s *packet, char **args, int argc,
                              int flags, f32 time) {
  u8 *locator = NULL;
  Unk_AIPacketObj *object = NULL;
  if (flags != 0) {
    if (packet != NULL && packet->pd0 != NULL)
      object = packet->pd0;
    for (i32 i = 0; i < argc; i++) {
      char *value = NuStrIStr(args[i], "character");
      if (value != NULL) {
        if (GetNamedAPIObjectFn != NULL)
          object = GetNamedAPIObjectFn(sys, value + 10);
      } else if (NuStrICmp("FALSE", args[i]) != 0) {
        if ((value = NuStrIStr(args[i], "locator=")) != NULL) {
          locator = (u8 *)AIPathFindLocator(sys, value + 8);
          if (locator == NULL)
            return 0;
        } else if ((value = NuStrIStr(args[i], "player=")) != NULL) {
          i32 n = (i32)AIParamToFloat(process, value + 7) - 1;
          if ((u32)n > 1 || Player[n] == NULL)
            return 0;
          object = (Unk_AIPacketObj *)Player[n];
        }
      }
    }
    if (locator != NULL) {
      object->respawn_pos = *(nuvec_s *)(locator + 0x10);
      object->last_safe_pos = *(nuvec_s *)(locator + 0x10);
      return 1;
    }
    if (object != NULL) {
      object->respawn_pos = object->pos5c;
      object->last_safe_pos = object->pos5c;
    }
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x00455f70
i32 Action_DontSetStoppedFlag(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                              AIPACKET_s *packet, char **args, int argc,
                              int flags, f32 time) {
  GameObject_s *obj = 0;
  i32 on = 1;
  if (flags != 0) {
    if (packet != 0 && packet->pd0 != 0 && packet->pd0->obj != 0)
      obj = packet->pd0->obj;
    for (i32 i = 0; i < argc; i++) {
      char *s = NuStrIStr(args[i], "character");
      if (s != 0)
        obj = GetNamedGameObjectInl(sys, s + 10);
      else if (NuStrICmp("FALSE", args[0]) == 0)
        on = 0;
    }
    if (obj != 0)
      obj->dont_set_stopped = on;
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x00470ee0
i32 Action_SetDeflectBolts(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                           AIPACKET_s *packet, char **args, int argc, int flags,
                           f32 time) {
  GameObject_s *obj = 0;
  i32 on = 1;
  if (flags != 0) {
    if (packet != 0 && packet->pd0 != 0 && packet->pd0->obj != 0)
      obj = packet->pd0->obj;
    for (i32 i = 0; i < argc; i++) {
      char *s = NuStrIStr(args[i], "character");
      if (s != 0)
        obj = GetNamedGameObjectInl(sys, s + 10);
      else if (NuStrICmp("FALSE", args[0]) == 0)
        on = 0;
    }
    if (obj != 0)
      obj->deflect_bolts = on;
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x004645b0
i32 Action_CanAttack(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                     AIPACKET_s *packet, char **args, int argc, int flags,
                     f32 time) {
  GameObject_s *obj = 0;
  i32 on = 1;
  if (flags != 0) {
    if (packet != 0 && packet->pd0 != 0 && packet->pd0->obj != 0)
      obj = packet->pd0->obj;
    for (i32 i = 0; i < argc; i++) {
      char *s = NuStrIStr(args[i], "character=");
      if (s != 0)
        obj = GetNamedGameObjectInl(sys, s + 10);
      else if (NuStrICmp(args[i], "FALSE") == 0)
        on = 0;
    }
    if (obj != 0)
      obj->can_attack = on;
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x0045db90
i32 Action_SetMaxYRotSeek(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                          AIPACKET_s *packet, char **args, int argc, int flags,
                          f32 time) {
  f32 seek = 0.0f;
  if (packet == 0 || packet->pd0 == 0 || packet->pd0->obj == 0)
    return 1;
  GameObject_s *obj = packet->pd0->obj;
  if (flags != 0) {
    seek = 1000000000.0f;
    if (argc != 0) {
      for (i32 i = 0; i < argc; i++) {
        if (NuStrICmp(args[i], "clear") != 0)
          seek = AIParamToFloat(process, args[i]);
      }
      if (seek < 0.0f)
        seek = 0.0f;
    }
  }
  obj->max_y_rot_seek = seek;
  return 1;
}

// GLOBAL: LEGOBATMAN 0x0095f730
extern u32 g_unk0095f730; // special button bit

void GameObjectSetCanUse(GameObject_s *obj, void *user, u8 a, u8 b, f32 time);

// FUNCTION: LEGOBATMAN 0x0045ad50
i32 Action_PressSpecialButton(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                              AIPACKET_s *packet, char **args, int argc,
                              int flags, f32 time) {
  if (packet == 0 || packet->pd0 == 0 || packet->pd0->obj == 0)
    return 1;
  GameObject_s *obj = packet->pd0->obj;
  if (obj == 0)
    return 1;
  if (flags != 0) {
    for (i32 i = 0; i < argc; i++) {
      if (NuStrICmp(args[i], "hold_button") == 0)
        process->hold_special_button = 1;
    }
  }
  GameObjectSetCanUse(obj, 0, 9, 2, 0.0f);
  *(u32 *)((u8 *)obj->p112c + 8) |= g_unk0095f730;
  if (process->hold_special_button != 0) {
    *(u32 *)((u8 *)obj->p112c + 4) |= g_unk0095f730;
    return 0;
  }
  return 1;
}

struct AIGROUP_s {
  u8 pad0[7];
  u8 member_count; // 0x07
  u8 count_across; // 0x08
};

// FUNCTION: LEGOBATMAN 0x0045bfe0
i32 Action_SetFormationCommander(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                 AIPACKET_s *packet, char **args, int argc,
                                 int flags, f32 time) {
  if (packet == 0 || packet->pd0 == 0 || packet->pd0->obj == 0 ||
      packet->group == 0)
    return 1;
  AIGROUP_s *group = packet->group;
  if (packet->group_member_index != group->member_count - 1)
    return 1;
  if (group->count_across != 1 &&
      group->member_count % group->count_across != 1)
    return 1;
  packet->movement_event_flags |= 0x1000000;
  return 1;
}

i32 NuSpecialExistsFn(nuhspecial_s *special);
void NuSpecialSetVisibility(nuhspecial_s *special, i32 visible);

// FUNCTION: LEGOBATMAN 0x004605f0
i32 Action_SetVisibility(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                         AIPACKET_s *packet, char **args, int argc, int flags,
                         f32 time) {
  i32 visible = 1;
  if (flags != 0) {
    nuhspecial_s special;
    memset(&special, 0, sizeof(special));
    for (i32 i = 0; i < argc; i++) {
      char *s = NuStrIStr(args[i], "name");
      if (s != 0)
        NuSpecialFind(g_unk00960894->scn140, &special, s + 5, 1);
      else if (NuStrIStr(args[i], "FALSE") != 0)
        visible = 0;
    }
    if (NuSpecialExistsFn(&special) != 0)
      NuSpecialSetVisibility(&special, visible);
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x00463b60
i32 Action_ProcessScriptWhenDeactivated(AISYS_s *sys,
                                        AISCRIPTPROCESS_s *process,
                                        AIPACKET_s *packet, char **args,
                                        int argc, int flags, f32 time) {
  GameObject_s *obj = 0;
  i32 on = 1;
  if (packet != 0 && packet->pd0 != 0 && packet->pd0->obj != 0)
    obj = packet->pd0->obj;
  if (argc != 0) {
    for (i32 i = 0; i < argc; i++) {
      char *s = NuStrIStr(args[i], "character=");
      if (s != 0)
        obj = GetNamedGameObjectInl(sys, s + 10);
      else if (NuStrICmp(args[i], "FALSE") == 0)
        on = 0;
    }
  }
  if (obj != 0)
    obj->process_when_deactivated = on;
  return 1;
}

// FUNCTION: LEGOBATMAN 0x004718f0
i32 Action_SetWoozy(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                    AIPACKET_s *packet, char **args, int argc, int flags,
                    f32 time) {
  GameObject_s *obj = 0;
  i32 woozy;
  if (argc != 0) {
    for (i32 i = 0; i < argc; i++) {
      char *s = NuStrIStr(args[i], "character");
      if (s != 0)
        obj = GetNamedGameObjectInl(sys, s + 10);
      if (NuStrIStr(args[i], "true") != 0)
        woozy = 1;
      else if (NuStrIStr(args[i], "false") != 0)
        woozy = 0;
    }
  }
  if (obj == 0)
    return 0;
  obj->woozy = woozy;
  return 1;
}

struct RIDEOBJECT_s {
  struct TECHNO_s *techno; // 0x00
  u8 pad4[8 - 4];
  GameObject_s *rider; // 0x08
  u8 padc[0x10 - 0xc];
  f32 radius_sqr;        // 0x10
  nuhspecial_s *special; // 0x14
  u8 pad18[0x2c - 0x18];
  nuvec_s offset; // 0x2c
};

RIDEOBJECT_s *RideObject_FindAvailable(GameObject_s *obj, i32 a);
numtx_s *NuSpecialGetDrawMtx(nuhspecial_s *special);
f32 NuVecDistSqr(nuvec_s *a, nuvec_s *b, nuvec_s *d);

// FUNCTION: LEGOBATMAN 0x00473050
i32 Action_GetInRideObject(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                           AIPACKET_s *packet, char **args, int argc, int flags,
                           f32 time) {
  nuvec_s pos;
  nuvec_s d;
  GameObject_s *obj = packet != 0 && packet->pd0 != 0 && packet->pd0->obj != 0
                          ? packet->pd0->obj
                          : 0;
  if (obj == 0)
    return 0;
  RIDEOBJECT_s *ride = RideObject_FindAvailable(obj, 0);
  if (ride != 0) {
    numtx_s *mtx = NuSpecialGetDrawMtx(ride->special);
    pos.x = mtx->m30 + ride->offset.x;
    pos.y = mtx->m31 + ride->offset.y;
    pos.z = mtx->m32 + ride->offset.z;
    f32 dist = NuVecDistSqr(&pos, &obj->v80, &d);
    if (dist < ride->radius_sqr) {
      obj->b9db = g_unk00960594;
      obj->techno = ride->techno;
      obj->f98c = 0.0f;
      obj->b9d9 = 0;
      ride->rider = obj;
    }
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x004640d0
i32 Action_CatchUpForbidden(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                            AIPACKET_s *packet, char **args, int argc,
                            int flags, f32 time) {
  GameObject_s *obj = 0;
  i32 on = 1;
  if (flags != 0) {
    if (packet != 0 && packet->pd0 != 0 && packet->pd0->obj != 0)
      obj = packet->pd0->obj;
    if (argc != 0) {
      for (i32 i = 0; i < argc; i++) {
        char *s = NuStrIStr(args[i], "character=");
        if (s != 0)
          obj = GetNamedGameObjectInl(sys, s + 10);
        else if (NuStrICmp(args[i], "FALSE") == 0)
          on = 0;
      }
    }
    if (obj != 0)
      obj->catch_up_forbidden = on;
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x0046d5f0
i32 Action_SplineFollowTerrain(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                               AIPACKET_s *packet, char **args, int argc,
                               int flags, f32 time) {
  GameObject_s *obj = 0;
  i32 on = 1;
  if (flags != 0) {
    if (packet != 0 && packet->pd0 != 0)
      obj = packet->pd0->obj;
    if (argc != 0) {
      for (i32 i = 0; i < argc; i++) {
        if (NuStrICmp(args[i], "FALSE") == 0) {
          on = 0;
        } else {
          char *s = NuStrIStr(args[i], "character=");
          if (s != 0)
            obj = GetNamedGameObjectInl(sys, s + 10);
        }
      }
    }
    if (obj != 0)
      obj->spline_follow_terrain = on;
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x0045e090
i32 Action_SetShieldHitPoints(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                              AIPACKET_s *packet, char **args, int argc,
                              int flags, f32 time) {
  GameObject_s *obj = 0;
  i32 hitpoints = -1;
  if (flags != 0) {
    if (packet != 0 && packet->pd0 != 0)
      obj = packet->pd0->obj;
    for (i32 i = 0; i < argc; i++) {
      char *s = NuStrIStr(args[i], "character=");
      if (s != 0)
        obj = GetNamedGameObjectInl(sys, s + 10);
      else
        hitpoints = (i32)AIParamToFloat(process, args[i]);
    }
    if (obj != 0) {
      if (hitpoints == -1)
        hitpoints = obj->p54->p24->shield_hitpoints;
      obj->shield_hitpoints = hitpoints;
    }
  }
  return 1;
}

extern i32 g_unk0093b104;
extern i32 g_unk0093b108;

// FUNCTION: LEGOBATMAN 0x00471b00
i32 Action_SetGenericGoon(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                          AIPACKET_s *packet, char **args, int argc, int flags,
                          f32 time) {
  i32 type = -1;
  i32 no_gun = 0;
  if (argc != 0) {
    for (i32 i = 0; i < argc; i++) {
      char *s = NuStrIStr(args[i], "type=");
      if (s != 0) {
        s += NuStrLen("type=");
        if (g_unk00ad68ec != 0 && g_unk00ad68fc != 0) {
          type = g_unk00ad68ec(s);
          if (type != 0xff)
            type = g_unk00ad68fc(type);
        }
      } else if (NuStrICmp(args[i], "no_gun") == 0) {
        no_gun = 1;
      }
    }
    if (no_gun != 0)
      g_unk0093b108 = type;
    else
      g_unk0093b104 = type;
  }
  return 1;
}

void DrawBossHitPoints(GameObject_s *obj, GameObject_s *obj2);

// FUNCTION: LEGOBATMAN 0x0046f710
i32 Action_DrawBossHitPoints(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                             AIPACKET_s *packet, char **args, int argc,
                             int flags, f32 time) {
  GameObject_s *obj2 = 0;
  GameObject_s *obj = 0;
  if (flags != 0) {
    if (packet != 0 && packet->pd0 != 0 && packet->pd0->obj != 0)
      obj = packet->pd0->obj;
    for (i32 i = 0; i < argc; i++) {
      char *s = NuStrIStr(args[i], "character=");
      if (s != 0) {
        s += 10;
        if (obj != 0)
          obj2 = GetNamedGameObjectInl(sys, s);
        else
          obj = GetNamedGameObjectInl(sys, s);
      } else if (NuStrICmp(args[i], "reset") == 0) {
        obj = 0;
      }
    }
    DrawBossHitPoints(obj, obj2);
  }
  return 1;
}

struct GAMESAVE_s;
struct SUPERCOUNTER;
struct SUPERCOUNTERSAVE;

struct GIZMOPICKUPTYPE_s {
  u8 pad0[0x12];
  u16 score; // 0x12
  u8 pad14[0x3c - 0x14];
};

struct GIZMOPICKUPSYS_s {
  GIZMOPICKUPTYPE_s *types; // 0x00
};

extern GIZMOPICKUPSYS_s *GizmoPickupSys;
extern GAMESAVE_s *g_unk009c59cc;
// GLOBAL: LEGOBATMAN 0x00ab096c
extern SUPERCOUNTER *g_unk00ab096c;
// GLOBAL: LEGOBATMAN 0x009652d4
extern i32 g_unk009652d4; // GizmoPickupSys type index
// GLOBAL: LEGOBATMAN 0x0095fd20
extern nuvec_s g_unk0095fd20;

i32 SuperCounter_AreaCheck(nuvec_s *pos, i32 area, SUPERCOUNTER *counter,
                           SUPERCOUNTERSAVE *save, i32 a);
void AddPickups(i32 type, i32 a, i32 b, i32 c, nuvec_s *pos, nuvec_s *vel,
                f32 f, i32 d, f32 g, f32 h, GameObject_s *obj, i32 e, i32 k);

// FUNCTION: LEGOBATMAN 0x004740b0
i32 Action_SetSuperCounter(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                           AIPACKET_s *packet, char **args, int argc, int flags,
                           f32 time) {
  if (flags != 0 && packet != 0 && packet->pd0 != 0 && packet->pd0->obj != 0) {
    GameObject_s *obj = packet->pd0->obj;
    if (obj != 0) {
      nuvec_s *pos = &obj->v80;
      if (SuperCounter_AreaCheck(
              pos, g_unk00960894->i124, g_unk00ab096c,
              (SUPERCOUNTERSAVE *)((u8 *)g_unk009c59cc + 0x7e10), 1) == 1 &&
          g_unk009652d4 != -1)
        AddPickups(GizmoPickupSys->types[g_unk009652d4].score, 0, 0, 0, pos,
                   &g_unk0095fd20, 0.0f, -1, 2.0f, 2000000.0f, 0, 0, 0);
    }
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x00460450
i32 Action_NoIdleSpeed(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                       AIPACKET_s *packet, char **args, int argc, int flags,
                       f32 time) {
  i32 on = 1;
  if (packet == 0 || packet->pd0 == 0 || packet->pd0->obj == 0)
    return 1;
  GameObject_s *obj = packet->pd0->obj;
  if (flags != 0) {
    for (i32 i = 0; i < argc; i++) {
      char *s = NuStrIStr(args[i], "character=");
      if (s != 0)
        obj = GetNamedGameObjectInl(sys, s + 10);
      else if (NuStrICmp(args[i], "FALSE") == 0)
        on = 0;
    }
    if (obj != 0)
      obj->no_idle_speed = on;
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x004683d0
i32 Action_FaceCharacter(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                         AIPACKET_s *packet, char **args, int argc, int flags,
                         f32 time) {
  if (flags != 0) {
    for (i32 i = 0; i < argc; i++) {
      char *s = NuStrIStr(args[i], "character=");
      if (s != 0)
        process->action_data_3 = GetNamedGameObjectInl(sys, s + 10);
      else
        process->action_timer = AIParamToFloat(process, args[i]);
    }
  }
  if (packet != 0 && process->action_data_3 != 0)
    packet->look_target = &((GameObject_s *)process->action_data_3)->position;
  if (process->action_timer > 0.0f) {
    process->action_timer -= time;
    if (process->action_timer <= 0.0f) {
      process->action_timer = 0.0f;
      return 1;
    }
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x0045be70
i32 Action_SetZeroAcceleration(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                               AIPACKET_s *packet, char **args, int argc,
                               int flags, f32 time) {
  i32 on = 1;
  GameObject_s *obj = 0;
  if (flags != 0) {
    if (packet != 0 && packet->pd0 != 0 && packet->pd0->obj != 0)
      obj = packet->pd0->obj;
    for (i32 i = 0; i < argc; i++) {
      if (NuStrIStr(args[i], "player1") != 0 ||
          NuStrIStr(args[i], "player") != 0)
        obj = player;
      else if (NuStrIStr(args[i], "player2") != 0)
        obj = player2;
      else if (NuStrIStr(args[i], "FALSE") != 0)
        on = 0;
    }
    if (obj != 0)
      obj->zero_acceleration = on;
  }
  return 1;
}

// STUB: LEGOBATMAN 0x004701d0
// original keeps sys in ebp and reloads process, recomputes set - 1 for the
// shift and has one epilogue; 3 tries all keep process in ebp
i32 Action_IgnoreTriggerSet(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                            AIPACKET_s *packet, char **args, int argc,
                            int flags, f32 time) {
  GameObject_s *obj = 0;
  u32 mask = 0;
  if (flags == 0)
    return 1;
  if (packet != 0 && packet->pd0 != 0)
    obj = packet->pd0->obj;
  {
    for (i32 i = 0; i < argc; i++) {
      char *s = NuStrIStr(args[i], "character=");
      if (s != 0) {
        obj = GetNamedGameObjectInl(sys, s + 10);
      } else {
        s = NuStrIStr(args[i], "set=");
        if (s != 0) {
          i32 set = (i32)AIParamToFloat(process, s + 4);
          if (set > 0 && set < 32)
            mask |= 1 << (set - 1);
        }
      }
    }
    if (obj != 0)
      obj->ignore_trigger_sets |= mask;
  }
  return 1;
}

i32 GizmoBlowupTypeGetIndexFromName(WORLDINFO_s *world, char *name);
void GizmoBlowUpTypeBlowUp(WORLDINFO_s *world, i32 type, nuvec_s *pos);

// FUNCTION: LEGOBATMAN 0x0045edf0
i32 Action_AddExplosion(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                        AIPACKET_s *packet, char **args, int argc, int flags,
                        f32 time) {
  i32 type = -1;
  GameObject_s *obj = 0;
  if (flags != 0) {
    if (packet != 0 && packet->pd0 != 0 && packet->pd0->obj != 0)
      obj = packet->pd0->obj;
    for (i32 i = 0; i < argc; i++) {
      char *s = NuStrIStr(args[i], "name");
      if (s != 0) {
        type = GizmoBlowupTypeGetIndexFromName(g_unk00960894, s + 5);
      } else if ((s = NuStrIStr(args[i], "character=")) != 0) {
        obj = GetNamedGameObjectInl(sys, s + 10);
      }
    }
    if (type != -1 && obj != 0)
      GizmoBlowUpTypeBlowUp(g_unk00960894, type, &obj->v80);
  }
  return 1;
}

i32 TagCharacter(GameObject_s *obj, GameObject_s *target, i32 a);
void SetPlayer(void);

// FUNCTION: LEGOBATMAN 0x00464be0
i32 Action_TagCharacter(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                        AIPACKET_s *packet, char **args, int argc, int flags,
                        f32 time) {
  GameObject_s *target = 0;
  GameObject_s *obj = 0;
  if (flags == 0)
    return 1;
  if (packet != 0 && packet->pd0 != 0 && packet->pd0->obj != 0)
    obj = packet->pd0->obj;
  if (argc != 0) {
    for (i32 i = 0; i < argc; i++) {
      char *s = NuStrIStr(args[i], "character=");
      if (s != 0) {
        obj = GetNamedGameObjectInl(sys, s + 10);
      } else {
        s = NuStrIStr(args[i], "tag_to=");
        if (s != 0)
          target = GetNamedGameObjectInl(sys, s + 7);
      }
    }
    if (obj != 0) {
      TagCharacter(obj, target, 0);
      SetPlayer();
    }
  }
  return 1;
}

void SetFlicker(GameObject_s *obj, f32 duration);

// FUNCTION: LEGOBATMAN 0x00473c60
i32 Action_SetFlickerTime(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                          AIPACKET_s *packet, char **args, int argc, int flags,
                          f32 time) {
  f32 duration = 0.0f;
  GameObject_s *obj = 0;
  if (flags != 0) {
    if (packet != 0 && packet->pd0 != 0 && packet->pd0->obj != 0)
      obj = packet->pd0->obj;
    if (argc != 0) {
      for (i32 i = 0; i < argc; i++) {
        char *s = NuStrIStr(args[i], "character=");
        if (s != 0) {
          s += NuStrLen("character=");
          obj = GetNamedGameObjectInl(sys, s);
        } else {
          s = NuStrIStr(args[i], "time=");
          if (s != 0) {
            s = s + NuStrLen("time") + 1;
            duration = AIParamToFloat(process, s);
          }
        }
      }
    }
    if (obj != 0)
      SetFlicker(obj, duration);
  }
  return 1;
}

void Unk00448b40(f32 dist);

// FUNCTION: LEGOBATMAN 0x0046f3e0
i32 Action_SetAO_RowDist(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                         AIPACKET_s *packet, char **args, int argc, int flags,
                         f32 time) {
  if (flags != 0) {
    for (i32 i = 0; i < argc; i++) {
      char *s = NuStrIStr(args[i], "Dist");
      if (s != 0)
        Unk00448b40(AIParamToFloat(process, s + 5));
    }
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x004623d0
i32 Action_MoveAwayFromLastAttacker(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                    AIPACKET_s *packet, char **args, int argc,
                                    int flags, f32 time) {
  if (packet == 0 || packet->pd0 == 0 || packet->pd0->obj == 0)
    return 1;
  GameObject_s *obj = packet->pd0->obj;
  if (flags != 0) {
    for (i32 i = 0; i < argc; i++) {
      if (AIActionParseSpeedFn != 0 &&
          AIActionParseSpeedFn(args[i], &packet->goal_speed_mode) != 0)
        continue;
      if (NuStrICmp(args[i], "face") == 0)
        process->action_data_1 = 1;
      else
        packet->movement_param = AIParamToFloat(process, args[i]);
    }
  }
  if (obj != 0 && obj->last_attacker != 0) {
    AIMoveInstruction(packet,
                      (nuvec_s *)(obj->last_attacker->process290 + 0x174),
                      *(f32 *)(obj->last_attacker->process290 + 0x120),
                      (AIPATHINFO *)(obj->last_attacker->process290 + 0x158), 2,
                      packet->movement_param);
    if (process->action_data_1 != 0)
      packet->look_target = &obj->last_attacker->position;
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x00472210
i32 Action_CanUseSupercarry(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                            AIPACKET_s *packet, char **args, int argc,
                            int flags, f32 time) {
  f32 duration = 1000000000.0f;
  GameObject_s *obj = 0;
  if (packet != 0 && packet->pd0 != 0 && packet->pd0->obj != 0 && flags != 0) {
    for (i32 i = 0; i < argc; i++) {
      char *s = NuStrIStr(args[i], "time=");
      if (s != 0) {
        s += NuStrLen("time=");
        duration = AIParamToFloatEx(packet, process, s);
      } else if (NuStrICmp(args[i], "false") == 0) {
        duration = 0.0f;
      }
    }
    if (packet->pd0 != 0 && packet->pd0->obj != 0)
      obj = packet->pd0->obj;
    if (duration == 0.0f)
      GameObjectSetCanUse(obj, 0, 0, 0, 0.0f);
    else
      GameObjectSetCanUse(obj, 0, 8, 0, duration);
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x00456410
i32 Action_SetLocatorSet(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                         AIPACKET_s *packet, char **args, int argc, int flags,
                         f32 time) {
  if (flags != 0) {
    for (i32 i = 0; i < argc; i++) {
      char *s = NuStrIStr(args[i], "name=");
      if (s != 0) {
        process->locator_set =
            AIPathFindLocatorSet(g_unk00960894->aiSys2bf8, s + 5);
      } else if (NuStrICmp(args[i], "from_current_loc") == 0) {
        AILOCATOR_s *locator = process->locator;
        if (locator != 0) {
          AssignLocatorAISys_s *ai = (AssignLocatorAISys_s *)sys;
          i32 index = locator - ai->locators;
          for (i32 j = 0; j < ai->locator_set_count; j++) {
            AILOCATORSET_s *set = &ai->locator_sets[j];
            for (i32 k = 0; k < set->locator_count; k++) {
              if (index == set->locator_entries[k])
                process->locator_set = set;
            }
          }
        }
      }
    }
  }
  return 1;
}

void StartBallooning(GameObject_s *obj, i32 a);
void SetBallooningHeight(GameObject_s *obj, f32 height);

// FUNCTION: LEGOBATMAN 0x004736a0
i32 Action_SetBallooning(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                         AIPACKET_s *packet, char **args, int argc, int flags,
                         f32 time) {
  AILOCATORSET_s *set = 0;
  GameObject_s *obj = 0;
  if (packet != 0 && packet->pd0 != 0 && packet->pd0->obj != 0)
    obj = packet->pd0->obj;
  if (obj == 0)
    return 0;
  if (argc <= 0)
    return 1;
  for (i32 i = 0; i < argc; i++) {
    char *s = NuStrIStr(args[i], "locator_set");
    if (s != 0) {
      s = s + NuStrLen("locator_set") + 1;
      set = AIPathFindLocatorSet(g_unk00960894->aiSys2bf8, s);
    } else if (NuStrIStr(args[i], "FALSE") != 0)
      obj->b9db = -1;
  }
  if (set != 0) {
    AILOCATOR_s *locator = 0;
    if (set->locator_count != 0)
      locator =
          &((AssignLocatorAISys_s *)sys)->locators[set->locator_entries[0]];
    process->locator_set = set;
    if (locator != 0) {
      process->locator = locator;
      obj->position = *(nuvec_s *)((u8 *)locator + 0x10);
      StartBallooning(obj, 0);
      SetBallooningHeight(obj, ((nuvec_s *)((u8 *)locator + 0x10))->y);
    }
  }
  return 1;
}

struct SOCK_s {
  u8 pad0[0x184];
};

struct SOCKSYS_s {
  SOCK_s *sock; // 0x00
};

SOCK_s *FindSock(SOCKSYS_s *sys, char *name);
void SockOn(SOCKSYS_s *sys, i32 index);
void SockOff(SOCKSYS_s *sys, i32 index);

// FUNCTION: LEGOBATMAN 0x004608c0
i32 Action_EnableSock(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                      AIPACKET_s *packet, char **args, int argc, int flags,
                      f32 time) {
  i32 on = 1;
  i32 index = -1;
  if (flags != 0) {
    for (i32 i = 0; i < argc; i++) {
      char *s = NuStrIStr(args[i], "ix=");
      if (s != 0) {
        index = (i32)AIParamToFloat(process, s + 3);
      } else if ((s = NuStrIStr(args[i], "name=")) != 0) {
        SOCK_s *sock = FindSock(g_unk00960894->sock_sys, s + 5);
        if (sock != 0)
          index = sock - g_unk00960894->sock_sys->sock;
      } else if (NuStrIStr(args[i], "FALSE") != 0) {
        on = 0;
      }
    }
    if (on != 0)
      SockOn(g_unk00960894->sock_sys, index);
    else
      SockOff(g_unk00960894->sock_sys, index);
  }
  return 1;
}

void DeactivateGameObject(GameObject_s *obj);

// FUNCTION: LEGOBATMAN 0x00453c50
i32 Action_DeActivate(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                      AIPACKET_s *packet, char **args, int argc, int flags,
                      f32 time) {
  GameObject_s *obj = 0;
  i32 set = 0;
  if (flags == 0)
    return 1;
  if (packet != 0 && packet->pd0 != 0 && packet->pd0->obj != 0)
    obj = packet->pd0->obj;
  for (i32 i = 0; i < argc; i++) {
    char *s = NuStrIStr(args[i], "character=");
    if (s != 0) {
      obj = GetNamedGameObjectInl(sys, s + 10);
    } else if ((s = NuStrIStr(args[i], "set=")) != 0) {
      set = (i32)AIParamToFloat(process, s + 4);
      if (set < 0 || set > 16)
        set = 0;
    }
  }
  if (set != 0) {
    GameObject_s *o = Obj;
    for (i32 i = 0; i < HIGHGAMEOBJECT; i++, o++) {
      if ((o->flags1fc & 1) != 0 && (o->flags1fc & 0x1000) != 0 &&
          o->process290[0x344 - 0x290] == set)
        DeactivateGameObject(o);
    }
  } else if (obj != 0) {
    DeactivateGameObject(obj);
  }
  return 1;
}

void Hint_SetHintFromId(i32 id, i32 a, i32 ignore_done_flag);

// FUNCTION: LEGOBATMAN 0x00461080
i32 Action_SetHint(AISYS_s *sys, AISCRIPTPROCESS_s *process, AIPACKET_s *packet,
                   char **args, int argc, int flags, f32 time) {
  i32 ignore_done_flag = 0;
  i32 id = -1;
  if (flags != 0) {
    for (i32 i = 0; i < argc; i++) {
      char *s = NuStrIStr(args[i], "id");
      if (s != 0)
        id = (i32)AIParamToFloat(process, s + 3);
      else if (NuStrIStr(args[i], "ignore_done_flag") != 0)
        ignore_done_flag = 1;
    }
    Hint_SetHintFromId(id, 0, ignore_done_flag);
  }
  return 1;
}

void Uncouple_Objects(GameObject_s *body, GameObject_s *trailer);

// FUNCTION: LEGOBATMAN 0x00473510
i32 Action_UncoupleVehicles(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                            AIPACKET_s *packet, char **args, int argc,
                            int flags, f32 time) {
  GameObject_s *body = 0;
  GameObject_s *trailer = 0;
  GameObject_s *obj = 0;
  if (packet != 0 && packet->pd0 != 0 && packet->pd0->obj != 0)
    obj = packet->pd0->obj;
  if (argc > 0) {
    for (i32 i = 0; i < argc; i++) {
      char *s = NuStrIStr(args[i], "body");
      if (s != 0) {
        s = s + NuStrLen("body") + 1;
        body = GetNamedGameObjectInl(sys, s);
      } else {
        s = NuStrIStr(args[i], "trailer");
        if (s != 0) {
          s = s + NuStrLen("trailer") + 1;
          trailer = GetNamedGameObjectInl(sys, s);
        }
      }
    }
  }
  if (obj != 0) {
    body = obj;
    trailer = obj->coupled_trailer;
  }
  if (body != 0 && trailer != 0)
    Uncouple_Objects(body, trailer);
  return 1;
}

unsigned __int64 PlayerItems_GetAllCarriedItemFlags(GameObject_s *obj);
void PlayerItems_RemoveItems(GameObject_s *obj, i32 a, i32 b, i32 c,
                             unsigned __int64 items);

// FUNCTION: LEGOBATMAN 0x00472e60
i32 Action_DisableWhip(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                       AIPACKET_s *packet, char **args, int argc, int flags,
                       f32 time) {
  GameObject_s *obj = 0;
  i32 disabled;
  if (packet != 0 && packet->pd0 != 0 && packet->pd0->obj != 0)
    obj = packet->pd0->obj;
  if (argc != 0) {
    for (i32 i = 0; i < argc; i++) {
      char *s = NuStrIStr(args[i], "character");
      if (s != 0)
        obj = GetNamedGameObjectInl(sys, s + 10);
      if (NuStrIStr(args[i], "true") != 0) {
        disabled = 1;
      } else if (NuStrIStr(args[i], "false") != 0) {
        disabled = 0;
        if ((PlayerItems_GetAllCarriedItemFlags(obj) & 0x200000) != 0)
          PlayerItems_RemoveItems(obj, 1, 1, 0, 0x200000);
        if ((PlayerItems_GetAllCarriedItemFlags(obj) & 0x80) != 0)
          PlayerItems_RemoveItems(obj, 1, 1, 0, 0x80);
      }
    }
  }
  if (obj == 0)
    return 0;
  obj->whip_disabled = disabled;
  return 1;
}

// FUNCTION: LEGOBATMAN 0x004632f0
i32 Action_CanOpenDoors(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                        AIPACKET_s *packet, char **args, int argc, int flags,
                        f32 time) {
  if (packet == 0 || packet->pd0 == 0 || packet->pd0->obj == 0)
    return 1;
  GameObject_s *obj = packet->pd0->obj;
  if (flags != 0) {
    obj->flags1580 |= 1;
    for (i32 i = 0; i < argc; i++) {
      if (NuStrICmp(args[i], "TRUE") == 0)
        obj->flags1580 |= 1;
      else if (NuStrICmp(args[i], "FALSE") == 0)
        obj->flags1580 &= ~1;
    }
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x0046fa60
i32 Action_GoToNewLevel(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                        AIPACKET_s *packet, char **args, int argc, int flags,
                        f32 time) {
  LEVELDATA_s *level = 0;
  LEVELDATA_s *freeplay_level = 0;
  char *cutscene = 0;
  if (flags != 0) {
    for (i32 i = 0; i < argc; i++) {
      char *s = NuStrIStr(args[i], "freeplay_level=");
      if (s != 0) {
        if (FreePlay != 0) {
          s += NuStrLen("freeplay_level=");
          freeplay_level = Level_FindByName(s, 0);
        }
      } else if ((s = NuStrIStr(args[i], "level=")) != 0) {
        s += NuStrLen("level=");
        level = Level_FindByName(s, 0);
      } else if ((s = NuStrIStr(args[i], "cutscene=")) != 0) {
        cutscene = s + NuStrLen("cutscene=");
      }
    }
  }
  if (FreePlay == 0 && cutscene != 0 &&
      NewCutScene(0, g_unk00960894->cutscene_sys, cutscene, 0) != 0)
    return 1;
  if (freeplay_level != 0) {
    GoToNewLevel(freeplay_level->idx);
    return 1;
  }
  if (level != 0)
    GoToNewLevel(level->idx);
  return 1;
}

// FUNCTION: LEGOBATMAN 0x0045db00
i32 Action_SetWalkSpeed(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                        AIPACKET_s *packet, char **args, int argc, int flags,
                        f32 time) {
  if (packet == 0 || packet->pd0 == 0 || packet->pd0->obj == 0)
    return 1;
  GameObject_s *obj = packet->pd0->obj;
  if (flags != 0) {
    obj->walk_speed_override = 1000000000.0f;
    if (argc != 0 && NuStrICmp(args[0], "default") != 0)
      obj->walk_speed_override = AIParamToFloat(process, args[0]);
  }
  return 1;
}

f32 NuRandFloat(void);

// STUB: LEGOBATMAN 0x0045fc50
// one swap off: the shared tail is `pop ebx; mov eax, 1` in the original,
// `mov eax, 1; pop ebx` here (wrapping, early returns, nesting all tried)
i32 Action_SetGravityHeight(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                            AIPACKET_s *packet, char **args, int argc,
                            int flags, f32 time) {
  f32 minimum = 1000000000.0f;
  f32 maximum = 1000000000.0f;
  if (packet == 0 || packet->pd0 == 0 || packet->pd0->obj == 0)
    return 1;
  GameObject_s *obj = packet->pd0->obj;
  if (flags == 0)
    return 1;
  obj->flags1410_lo &= ~8;
  obj->hover_height_override = 1000000000.0f;
  for (i32 i = 0; i < argc; i++) {
    if (NuStrICmp(args[i], "reset") == 0)
      continue;
    char *s = NuStrIStr(args[i], "min=");
    if (s != 0) {
      minimum = AIParamToFloat(process, s + 4);
    } else if ((s = NuStrIStr(args[i], "max=")) != 0) {
      maximum = AIParamToFloat(process, s + 4);
    } else {
      obj->hover_height_override = AIParamToFloat(process, args[i]);
    }
  }
  if (minimum != 1000000000.0 && maximum != 1000000000.0) {
    f32 r = NuRandFloat();
    obj->hover_height_override = (1.0f - r) * minimum + maximum * r;
  }
  return 1;
}

void ReleaseTakeOver(GameObject_s *obj, i32 a);

// FUNCTION: LEGOBATMAN 0x004641d0
i32 Action_SetTaggable(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                       AIPACKET_s *packet, char **args, int argc, int flags,
                       f32 time) {
  GameObject_s *tag_to = 0;
  i32 disabled = 0;
  GameObject_s *obj = 0;
  if (flags == 0)
    return 1;
  if (packet != 0 && packet->pd0 != 0 && packet->pd0->obj != 0)
    obj = packet->pd0->obj;
  if (argc > 0) {
    for (i32 i = 0; i < argc; i++) {
      char *s = NuStrIStr(args[i], "character=");
      if (s != 0)
        obj = GetNamedGameObjectInl(sys, s + 10);
      else if ((s = NuStrIStr(args[i], "tag_to=")) != 0)
        tag_to = GetNamedGameObjectInl(sys, s + 7);
      else if (NuStrICmp(args[i], "FALSE") == 0)
        disabled = 1;
    }
  }
  if (obj != 0) {
    if (disabled != 0 && (obj->flags1fc & 0x80) != 0) {
      if (obj->p1158 != 0)
        ReleaseTakeOver(obj, 0);
      else if (TagCharacter(obj, tag_to, 0) == 0)
        disabled = 0;
      SetPlayer();
    }
    obj->tag_disabled = disabled;
  }
  return 1;
}

void ComplexSockPosition(SOCKSYS_s *sys, nuvec_s *pos, i32 sock, i32 segment,
                         void *out);
void ComplexSockAngles(void *angles);
void CurrentStart(GameObject_s *obj, i32 a, i32 b);

// FUNCTION: LEGOBATMAN 0x0045f830
i32 Action_UseCurrentSpeed(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                           AIPACKET_s *packet, char **args, int argc, int flags,
                           f32 time) {
  i32 snap = 0;
  if (packet == 0 || packet->pd0 == 0 || packet->pd0->obj == 0)
    return 1;
  GameObject_s *obj = packet->pd0->obj;
  if (flags != 0) {
    obj->flags1414 |= 0x800000;
    obj->current_speed_mul = 1.0f;
    for (i32 i = 0; i < argc; i++) {
      char *s;
      if (NuStrICmp(args[i], "FALSE") == 0)
        obj->flags1414 &= ~0x800000;
      else if ((s = NuStrIStr(args[i], "multiplier")) != 0)
        obj->current_speed_mul = AIParamToFloat(process, s + 11);
      else if (NuStrICmp(args[i], "snaptospeed") == 0)
        snap = 1;
    }
    if ((obj->flags1414 & 0x800000) != 0 && (obj->flags140c & 0x40000) == 0) {
      obj->flags140c |= 0x40000;
      if (snap != 0) {
        ComplexSockPosition(g_unk00960894->sock_sys, &obj->position,
                            obj->sock_id, obj->sock_segment, &obj->sock_pos870);
        ComplexSockAngles(obj->sock_angles);
      }
    }
    if (snap != 0)
      CurrentStart(obj, 1, 1);
  }
  return 1;
}

GameObject_s *Unk0044c930(AISYS_s *aisys, char *name);
i32 SpecialMove_Check(GameObject_s *obj, GameObject_s *opponent, i32 flags,
                      i32 a);
void SpecialMove_Start(GameObject_s *obj, GameObject_s *opponent, i32 move,
                       i32 a);
// GLOBAL: LEGOBATMAN 0x0093b110
extern f32 g_unk0093b110;
// GLOBAL: LEGOBATMAN 0x0093b114
extern f32 g_unk0093b114;
extern i32 g_unk0096052c;

f32 NuRandFloat(void);

// FUNCTION: LEGOBATMAN 0x00474e00
i32 Action_EngageBlowup(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                        AIPACKET_s *packet, char **args, int argc, int flags,
                        f32 time) {
  char *name = NULL;
  GameObject_s *obj;
  if (packet == NULL || packet->pd0 == NULL || packet->pd0->obj == NULL)
    return 1;
  obj = packet->pd0->obj;
  if (flags != 0) {
    packet->movement_param = g_unk0093b110;
    process->action_data_5 = g_unk0093b114;
    for (i32 i = 0; i < argc; i++) {
      char *s = NuStrIStr(args[i], "name");
      if (s != NULL)
        name = s + 5;
      else if ((s = NuStrIStr(args[i], "fireinterval")) != NULL)
        process->action_data_5 = AIParamToFloat(process, s + 13);
      else if (NuStrIStr(args[i], "predictive") != NULL)
        process->action_data_1 = 1;
    }
    process->action_timer = NuRandFloat() * process->action_data_5;
    if (name != NULL) {
      GIZMO_s *gizmo = GizmoFindByName(g_unk00960894->gizmoSys2b0c,
                                       blowup_gizmotype_id, name);
      if (gizmo != NULL && gizmo->object != NULL)
        process->action_data_3 = (u8 *)gizmo->object + 0x58;
    }
    if (process->action_data_3 != NULL) {
      obj->flags140c |= 0x100000;
      obj->f1204 = NuRandFloat() * process->action_data_5;
    }
  }
  if (process->action_data_3 != NULL) {
    packet->look_target = (nuvec_s *)process->action_data_3;
    process->action_timer -= time;
    obj->aim_target = *(nuvec_s *)process->action_data_3;
    obj->flags140c |= 0x80200;
    if (process->action_timer <= 0.0f) {
      obj->predictive_aim = process->action_data_1;
      process->action_timer = NuRandFloat() * process->action_data_5 +
                              process->action_data_5 * 0.5f;
      obj->p112c->buttons8 |= g_unk0095f72c;
    }
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x00475050
i32 Action_EngageLocator(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                         AIPACKET_s *packet, char **args, int argc, int flags,
                         f32 time) {
  char *name = NULL;
  GameObject_s *obj;
  if (packet == NULL || packet->pd0 == NULL || packet->pd0->obj == NULL)
    return 1;
  obj = packet->pd0->obj;
  if (flags != 0) {
    packet->movement_param = g_unk0093b110;
    process->action_data_5 = g_unk0093b114;
    if (packet->locator_name != NULL)
      name = packet->locator_name;
    if (argc != 0) {
      for (i32 i = 0; i < argc; i++) {
        char *s = NuStrIStr(args[i], "name");
        if (s != NULL)
          name = s + 5;
        else if ((s = NuStrIStr(args[i], "fireinterval")) != NULL)
          process->action_data_5 = AIParamToFloat(process, s + 13);
        else if (NuStrIStr(args[i], "predictive") != NULL)
          process->action_data_1 = 1;
      }
      process->action_timer = NuRandFloat() * process->action_data_5;
    }
    if (name != NULL) {
      u8 *locator = (u8 *)AIPathFindLocator(sys, name);
      if (locator != NULL)
        process->action_data_3 = locator + 0x10;
    }
    if (process->action_data_3 != NULL) {
      obj->flags140c |= 0x100000;
      obj->f1204 = NuRandFloat() * process->action_data_5;
    }
  }
  if (process->action_data_3 != NULL) {
    packet->look_target = (nuvec_s *)process->action_data_3;
    process->action_timer -= time;
    obj->aim_target = *(nuvec_s *)process->action_data_3;
    obj->flags140c |= 0x80200;
    if (process->action_timer <= 0.0f) {
      obj->predictive_aim = process->action_data_1;
      process->action_timer = NuRandFloat() * process->action_data_5 +
                              process->action_data_5 * 0.5f;
      obj->p112c->buttons8 |= g_unk0095f72c;
    }
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x00475350
i32 Action_StartSpecialMove(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                            AIPACKET_s *packet, char **args, int argc,
                            int flags, f32 time) {
  i32 move_flags = 0;
  GameObject_s *opponent = 0;
  if (packet == 0 || packet->pd0 == 0 || packet->pd0->obj == 0)
    return 1;
  GameObject_s *obj = packet->pd0->obj;
  if (flags != 0) {
    packet->movement_param = g_unk0093b110;
    *(f32 *)((u8 *)process + 0x74) = g_unk0093b114;
    for (i32 i = 0; i < argc; i++) {
      char *s;
      if (NuStrICmp(args[i], "opponent=myopponent") == 0) {
        Unk_AIPacketObj *opp = *(Unk_AIPacketObj **)(obj->process290 + 0xe4);
        if (opp != 0)
          opponent = opp->obj;
      } else if ((s = NuStrIStr(args[i], "opponent")) != 0) {
        opponent = Unk0044c930(sys, s + 9);
      } else if (NuStrICmp(args[i], "button=ACTION") == 0) {
      } else if (NuStrICmp(args[i], "button=SPECIAL") == 0) {
        move_flags |= 0x200;
      } else if (NuStrICmp(args[i], "AIFORCEOVERRIDE") == 0) {
        move_flags |= 0x100;
      }
    }
    if (opponent != 0 && obj->b9db != g_unk0096052c) {
      i32 move = SpecialMove_Check(obj, opponent, move_flags, -1);
      if (move != -1)
        SpecialMove_Start(obj, opponent, move, 1);
    }
  }
  return 1;
}

// GLOBAL: LEGOBATMAN 0x00967a14
extern i32 turret_gizmotype_id;

struct GIZTURRET_s {
  u8 pad0[0xe8];
  GameObject_s *controller; // 0xe8
};

// FUNCTION: LEGOBATMAN 0x0046c700
i32 Action_SetLastAttacker(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                           AIPACKET_s *packet, char **args, int argc, int flags,
                           f32 time) {
  GameObject_s *attacker = NULL;
  GameObject_s *victim = NULL;
  i32 i = 0;
  if (flags != 0) {
    if (packet != NULL && packet->pd0 != NULL && packet->pd0->obj != NULL)
      victim = packet->pd0->obj;
    for (; i < argc; i++) {
      char *value = NuStrIStr(args[i], "victim=");
      if (value != NULL) {
        victim = GetNamedGameObjectInl(sys, value + 7);
      } else if (NuStrICmp(args[i], "attacker=opponent") == 0) {
        if (packet != NULL && packet->pe4 != NULL)
          attacker = packet->pe4->obj;
      } else if ((value = NuStrIStr(args[i], "attacker=")) != NULL) {
        attacker = GetNamedGameObjectInl(sys, value + 9);
      } else if (NuStrICmp(args[i], "attacker=player") == 0) {
        attacker = (*(Unk_AIPacketObj **)((u8 *)sys + 0x1698))->obj;
      }
    }
    if (victim != NULL && attacker != NULL)
      victim->last_attacker = attacker;
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x0046c880
i32 Action_LinkTurretToController(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                  AIPACKET_s *packet, char **args, int argc,
                                  int flags, f32 time) {
  GIZTURRET_s *turret = NULL;
  GameObject_s *controller = NULL;
  if (flags != 0) {
    for (i32 i = 0; i < argc; i++) {
      char *value = NuStrIStr(args[i], "turret=");
      if (value != NULL) {
        GIZMO_s *gizmo = GizmoFindByName(g_unk00960894->gizmoSys2b0c,
                                         turret_gizmotype_id, value + 7);
        if (gizmo != NULL && gizmo->object != NULL)
          turret = (GIZTURRET_s *)gizmo->object;
      } else if ((value = NuStrIStr(args[i], "controller=")) != NULL) {
        controller = GetNamedGameObjectInl(sys, value + 11);
      }
    }
    if (turret != NULL)
      turret->controller = controller;
  }
  return 1;
}

void TakeOverGameObject(GameObject_s *obj, GameObject_s *target, i32 a, i32 b);

// FUNCTION: LEGOBATMAN 0x0046c990
i32 Action_TakeOver(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                    AIPACKET_s *packet, char **args, int argc, int flags,
                    f32 time) {
  i32 release = 0;
  GameObject_s *obj = NULL;
  GameObject_s *target = NULL;
  if (flags == 0)
    return 1;
  if (packet != NULL && packet->pd0 != NULL && packet->pd0->obj != NULL) {
    obj = packet->pd0->obj;
    target = obj->takeover_target;
  }
  for (i32 i = 0; i < argc; i++) {
    char *value = NuStrIStr(args[i], "character=");
    if (value != NULL) {
      obj = GetNamedGameObjectInl(sys, value + 10);
    } else if ((value = NuStrIStr(args[i], "player=")) != NULL) {
      i32 n = (i32)AIParamToFloat(process, value + 7) - 1;
      if ((u32)n <= 1)
        obj = Player[n];
    } else if ((value = NuStrIStr(args[i], "target=")) != NULL) {
      target = GetNamedGameObjectInl(sys, value + 7);
    } else if (NuStrIStr(args[i], "last") != NULL) {
      target = obj->last_takeover;
    } else if (NuStrIStr(args[i], "Opponent") != NULL) {
      if (packet->pe4 != NULL)
        target = packet->pe4->obj;
    } else if (NuStrIStr(args[i], "release") != NULL) {
      release = 1;
    }
  }
  if (obj != NULL) {
    if (target != NULL && !release) {
      if ((target->p54->model_flags & 0x40000000) &&
          (target->b9db == 0x17 || target->b9db == 0x3e))
        return 0;
      if (target->p1158 == NULL && (target->p54->p24->flags13c & 0x40) &&
          !(obj->p54->model_flags & 0x10)) {
        if (!(target->flags1fc & 0x1000))
          return 0;
        TakeOverGameObject(obj, target, 1, 0);
        return 1;
      }
    } else if (release) {
      ReleaseTakeOver(obj, 0);
    }
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x0046cc10
i32 Action_ReleaseTakeOver(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                           AIPACKET_s *packet, char **args, int argc, int flags,
                           f32 time) {
  GameObject_s *obj = 0;
  if (flags != 0) {
    if (packet != 0 && packet->pd0 != 0 && packet->pd0->obj != 0)
      obj = packet->pd0->obj;
    for (i32 i = 0; i < argc; i++) {
      char *s = NuStrIStr(args[i], "character=");
      if (s != 0)
        obj = GetNamedGameObjectInl(sys, s + 10);
    }
    if (obj != 0 && obj->p1158 != 0)
      ReleaseTakeOver(obj, 0);
  }
  return 1;
}

void FollowAPIObject(Unk_AIPacketObj *api, void *target, i32 flags, f32 param);

// FUNCTION: LEGOBATMAN 0x00468ce0
i32 Action_FollowCharacter(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                           AIPACKET_s *packet, char **args, int argc, int flags,
                           f32 time) {
  if (packet == 0)
    return 1;
  if (flags != 0) {
    for (i32 i = 0; i < argc; i++) {
      if (AIActionParseSpeedFn != 0 &&
          AIActionParseSpeedFn(args[i], &packet->goal_speed_mode) != 0)
        continue;
      char *s = NuStrIStr(args[i], "character=");
      if (s != 0)
        process->action_data_3 = GetNamedGameObjectInl(sys, s + 10);
      else if (NuStrICmp(args[i], "ignore_radius") == 0)
        process->action_data_1 |= 2;
      else if (NuStrICmp(args[i], "can_go_off_path") == 0)
        process->action_data_1 |= 1;
      else if (NuStrICmp(args[i], "Opponent") == 0)
        process->action_data_3 = packet->pe4;
      else if (NuStrICmp(args[i], "TakeOverTarget") == 0)
        process->action_data_3 = packet->pd0->obj->takeover_target;
      else
        packet->movement_param = AIParamToFloat(process, args[i]);
    }
  }
  void *target = process->action_data_3;
  if (target != 0)
    FollowAPIObject(packet->pd0, target, process->action_data_1,
                    packet->movement_param);
  return 0;
}

struct AIPATHNODE_s {
  char *name;  // 0x00
  nuvec_s pos; // 0x04
  u8 pad10[0x2a - 0x10];
  u8 on_platform; // 0x2a
  u8 pad2b[0x40 - 0x2b];
  nuhspecial_s platform; // 0x40
  nuvec_s platform_pos;  // 0x4c, pos in the platform's space
  u8 pad58[0x5c - 0x58];
};

AIPATHNODE_s *AIPathFindNode(AISYS_s *sys, AIPATH_s *path, char *name);
void AIPathNodeBeenMoved(AISYS_s *sys, AIPATH_s *path, AIPATHNODE_s *node);

// FUNCTION: LEGOBATMAN 0x00470840
i32 Action_MoveNode(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                    AIPACKET_s *packet, char **args, int argc, int flags,
                    f32 time) {
  f32 x = 1000000000.0f;
  f32 y = 1000000000.0f;
  f32 z = 1000000000.0f;
  AIPATHNODE_s *node = 0;
  if (flags != 0) {
    AssignLocatorAISys_s *ai = (AssignLocatorAISys_s *)sys;
    for (i32 i = 0; i < argc; i++) {
      char *s = NuStrIStr(args[i], "node=");
      if (s != 0)
        node = AIPathFindNode(sys, ai->path_sys->active_path, s + 5);
      else if ((s = NuStrIStr(args[i], "x=")) != 0)
        x = AIParamToFloat(process, s + 2);
      else if ((s = NuStrIStr(args[i], "y=")) != 0)
        y = AIParamToFloat(process, s + 2);
      else if ((s = NuStrIStr(args[i], "z=")) != 0)
        z = AIParamToFloat(process, s + 2);
    }
    if (node != 0) {
      if (x != 1000000000.0)
        node->pos.x = x;
      if (y != 1000000000.0)
        node->pos.y = y;
      if (z != 1000000000.0)
        node->pos.z = z;
      AIPathNodeBeenMoved(sys, ai->path_sys->active_path, node);
    }
  }
  return 1;
}

void NuVecRotateY(nuvec_s *v, nuvec_s *v0, i32 a);

struct nugspline_s *NuSplineFind(nugscn_s *scene, char *name);
int NuSplineFindAllBeg(nugscn_s *scene, char *name,
                       struct nugspline_s **results, int max);

// GLOBAL: LEGOBATMAN 0x009c61c8
extern struct nugspline_s *script_spline_selected;

// spline length word: bit 15 marks "in use" while picking.
struct SPLINEHDR_s {
  u16 len : 15;
  u16 in_use : 1;
};
#define SPLINE_USED(s) (((SPLINEHDR_s *)(s))->in_use)

// STUB: LEGOBATMAN 0x0045c570
// only the unused-spline collect loop differs: original counts in ecx and
// holds the spline in eax, ours swaps them (5 tries)
i32 Action_SelectRandomSpline(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                              AIPACKET_s *packet, char **args, int argc,
                              int flags, f32 time) {
  struct nugspline_s *splines[32];
  struct nugspline_s *unused_splines[32];
  i32 count = 0;
  i32 unused = 0;
  i32 unused_count = 0;
  i32 i;
  if (flags) {
    script_spline_selected = NULL;
    for (i = 0; i < argc; i++) {
      char *value = NuStrIStr(args[i], "splines=");
      if (value != NULL) {
        count += NuSplineFindAllBeg(g_unk00960894->scn140, value + 8,
                                    &splines[count], 32 - count);
      } else if ((value = NuStrIStr(args[i], "spline=")) != NULL) {
        struct nugspline_s *spline =
            NuSplineFind(g_unk00960894->scn140, value + 7);
        if (spline != NULL && count < 32)
          splines[count++] = spline;
      } else if (NuStrIStr(args[i], "unused") != NULL) {
        unused = 1;
      }
    }
    if (count != 0) {
      if (unused) {
        GameObject_s *object = Obj;
        for (i = 0; i < HIGHGAMEOBJECT; i++, object++) {
          if ((object->flags1fc & 1) && (object->flags1fc & 0x1000) &&
              object->b257 == 0 && object->movement_spline != NULL)
            SPLINE_USED(object->movement_spline) = 1;
        }
        for (i = 0; i < count; i++) {
          struct nugspline_s *spline = splines[i];
          if (!SPLINE_USED(spline))
            unused_splines[unused_count++] = spline;
        }
        object = Obj;
        for (i = 0; i < HIGHGAMEOBJECT; i++, object++) {
          if ((object->flags1fc & 1) && (object->flags1fc & 0x1000) &&
              object->b257 == 0 && object->movement_spline != NULL)
            SPLINE_USED(object->movement_spline) = 0;
        }
        if (unused_count != 0)
          script_spline_selected =
              unused_splines[qrand() / (0xffff / unused_count + 1)];
      } else {
        script_spline_selected = splines[qrand() / (0xffff / count + 1)];
      }
    }
  }
  return 1;
}
#undef SPLINE_USED

// FUNCTION: LEGOBATMAN 0x0045d330
i32 Action_SetCurrentSpeed(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                           AIPACKET_s *packet, char **args, int argc, int flags,
                           f32 time) {
  f32 speed = 0.0f;
  i32 mode = -1;
  if (packet == 0 || packet->pd0 == 0 || packet->pd0->obj == 0)
    return 1;
  GameObject_s *obj = packet->pd0->obj;
  if (flags == 0)
    return 1;
  for (i32 i = 0; i < argc; i++) {
    char *s = NuStrIStr(args[i], "character=");
    if (s != 0)
      obj = GetNamedGameObjectInl(sys, s + 10);
    else if (NuStrICmp("speed=TIPTOE", args[i]) == 0)
      mode = 2;
    else if (NuStrICmp("speed=WALK", args[i]) == 0)
      mode = 1;
    else if (NuStrICmp("speed=RUN", args[i]) == 0)
      mode = 0;
    else
      speed = AIParamToFloat(process, args[i]);
  }
  if (obj != 0) {
    if (mode == 2) {
      speed = obj->p54->p24->tiptoe_speed;
    } else if (mode == 1) {
      speed = obj->p54->p24->walk_speed;
    } else if (mode == 0) {
      speed = obj->p54->p24->run_speed;
      obj->f1254 = 1.0f;
    }
    obj->velocity.x = 0.0f;
    obj->velocity.y = 0.0f;
    obj->velocity.z = speed;
    NuVecRotateY(&obj->velocity, &obj->velocity, obj->yaw58);
  }
  return 1;
}

nuvec_s *NuSpecialGetDrawPos(nuhspecial_s *special);
void NuSpecialSetDrawPos(nuhspecial_s *special, nuvec_s *pos);

// FUNCTION: LEGOBATMAN 0x004606e0
i32 Action_SpecialObjSetPos(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                            AIPACKET_s *packet, char **args, int argc,
                            int flags, f32 time) {
  nuvec_s pos;
  pos.x = 1000000000.0f;
  pos.y = 1000000000.0f;
  pos.z = 1000000000.0f;
  if (flags != 0) {
    nuhspecial_s special;
    memset(&special, 0, sizeof(special));
    for (i32 i = 0; i < argc; i++) {
      char *s = NuStrIStr(args[i], "name");
      if (s != 0)
        NuSpecialFind(g_unk00960894->scn140, &special, s + 5, 1);
      else if ((s = NuStrIStr(args[i], "x=")) != 0)
        pos.x = AIParamToFloat(process, s + 2);
      else if ((s = NuStrIStr(args[i], "y=")) != 0)
        pos.y = AIParamToFloat(process, s + 2);
      else if ((s = NuStrIStr(args[i], "z=")) != 0)
        pos.z = AIParamToFloat(process, s + 2);
    }
    if (NuSpecialExistsFn(&special) != 0) {
      nuvec_s *cur = NuSpecialGetDrawPos(&special);
      if (cur != 0) {
        if (pos.x == 1000000000.0)
          pos.x = cur->x;
        if (pos.y == 1000000000.0)
          pos.y = cur->y;
        if (pos.z == 1000000000.0)
          pos.z = cur->z;
        NuSpecialSetDrawPos(&special, &pos);
      }
    }
  }
  return 1;
}

AIPATH_s *AISysFindPath(AISYS_s *sys, char *name);
numtx_s *NuSpecialGetMtx(nuhspecial_s *special);
void NuVecInvMtxTransform(nuvec_s *out, nuvec_s *v, numtx_s *m);

// FUNCTION: LEGOBATMAN 0x00472640
i32 Action_AttachNodeToPlatform(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                AIPACKET_s *packet, char **args, int argc,
                                int flags, f32 time) {
  char *node_name = 0;
  char *path_name = 0;
  char *platform_name = 0;
  AIPATH_s *path = 0;
  WORLDINFO_s *world = WorldInfo_CurrentlyActive();
  nuhspecial_s special;
  if (flags != 0) {
    for (i32 i = 0; i < argc; i++) {
      char *s = NuStrIStr(args[i], "node");
      if (s != 0) {
        node_name = s + NuStrLen("node") + 1;
      } else if ((s = NuStrIStr(args[i], "path")) != 0) {
        path_name = s + NuStrLen("path") + 1;
      } else if ((s = NuStrIStr(args[i], "platform")) != 0) {
        platform_name = s + NuStrLen("platform") + 1;
      }
    }
    if (path_name != 0)
      path = AISysFindPath(sys, path_name);
    if (node_name != 0) {
      AIPATHNODE_s *node = AIPathFindNode(sys, path, node_name);
      if (node != 0) {
        NuSpecialFind(world->scn140, &special, platform_name, 0);
        if (NuSpecialExistsFn(&special) != 0) {
          node->platform = special;
          node->on_platform = 1;
          NuVecInvMtxTransform(&node->platform_pos, &node->pos,
                               NuSpecialGetMtx(&special));
        } else {
          memset(&node->platform, 0, sizeof(node->platform));
          node->on_platform = 0;
        }
      }
    }
  }
  return 1;
}

void GizmoSetVisibility(GIZMOSYS_s *sys, GIZMO_s *gizmo, i32 visible, i32 a);

// FUNCTION: LEGOBATMAN 0x0046ff80
i32 Action_GizmoSetVisibility(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                              AIPACKET_s *packet, char **args, int argc,
                              int flags, f32 time) {
  GIZMO_s *gizmo = 0;
  i32 visible = 1;
  if (flags != 0) {
    for (i32 i = 0; i < argc; i++) {
      char *s = NuStrIStr(args[i], "name=");
      if (s != 0) {
        s += NuStrLen("name=");
        gizmo = GizmoFindByName(g_unk00960894->gizmoSys2b0c, -1, s);
      } else if (NuStrICmp(args[i], "FALSE") == 0) {
        visible = 0;
      }
    }
    if (gizmo != 0)
      GizmoSetVisibility(g_unk00960894->gizmoSys2b0c, gizmo, visible, 1);
  }
  return 1;
}

struct PLAYERITEMTYPE_s {
  u8 pad0[0x40];
  f32 f40; // 0x40
};

struct PLAYERITEM_s {
  NULISTLNK link;         // 0x00
  PLAYERITEMTYPE_s *type; // 0x08
  u8 padc[0x25 - 0xc];
  u8 flags25; // 0x25
};

i32 PlayerItemType_FindIXFromName(char *name);
PLAYERITEMTYPE_s *PlayerItemType_FindFromIx(i32 ix);
PLAYERITEM_s *PlayerItems_AddItem(GameObject_s *obj, PLAYERITEMTYPE_s *type,
                                  GIZMOBLOWUP_s *blowup, i32 held, i32 current,
                                  i32 def, f32 f);

// FUNCTION: LEGOBATMAN 0x00470fd0
i32 Action_AddItem(AISYS_s *sys, AISCRIPTPROCESS_s *process, AIPACKET_s *packet,
                   char **args, int argc, int flags, f32 time) {
  i32 set_current = 1;
  i32 set_default = 1;
  i32 set_held = 1;
  GameObject_s *obj = 0;
  PLAYERITEMTYPE_s *type = 0;
  if (flags != 0) {
    if (packet != 0 && packet->pd0 != 0 && packet->pd0->obj != 0)
      obj = packet->pd0->obj;
    if (argc != 0) {
      for (i32 i = 0; i < argc; i++) {
        char *s = NuStrIStr(args[i], "character=");
        if (s != 0)
          obj = GetNamedGameObjectInl(sys, s + 10);
        else if (NuStrICmp(args[i], "set_as_current_item=FALSE") == 0)
          set_current = 0;
        else if (NuStrICmp(args[i], "set_as_default_item=FALSE") == 0)
          set_default = 0;
        else if (NuStrICmp(args[i], "set_item_as_held=FALSE") == 0)
          set_held = 0;
        else if ((s = NuStrIStr(args[i], "item=")) != 0)
          type = PlayerItemType_FindFromIx(
              (i16)PlayerItemType_FindIXFromName(s + 5));
      }
    }
    if (obj != 0 && type != 0) {
      NULISTHDR *items = (NULISTHDR *)((u8 *)obj + 0xb18);
      PLAYERITEM_s *item;
      for (item = (PLAYERITEM_s *)NuListGetHead(items); item != 0;
           item = (PLAYERITEM_s *)NuListGetNext(items, &item->link)) {
        if (item->type == type) {
          if (set_default != 0)
            item->flags25 |= 1;
          return 1;
        }
      }
      item = PlayerItems_AddItem(obj, type, 0, set_held, set_current,
                                 set_default, type->f40);
      if (item != 0)
        item->flags25 |= 2;
    }
  }
  return 1;
}

struct GoToLevelPathCnx_s {
  u8 pad0[0x28];
};

struct GoToLevelPathPath_s {
  u8 pad0[0x80];
  GoToLevelPathCnx_s *connections; // 0x80
};

struct GoToLevelPathNode_s {
  char *name;  // 0x00
  nuvec_s pos; // 0x04
  u8 pad10[0x14 - 0x10];
  f32 radius_sqr; // 0x14
  u8 pad18[0x2b - 0x18];
  u8 runtime_flags; // 0x2b
  u16 connection;   // 0x2c
};

f32 NuVecXZDistSqr(nuvec_s *a, nuvec_s *b, nuvec_s *d);
void AISysCharacterSetPathCnx(AIPACKET_s *packet, nuvec_s *pos,
                              GoToLevelPathCnx_s *cnx, i32 a);

// FUNCTION: LEGOBATMAN 0x004621e0
i32 Action_GoToLevelPath(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                         AIPACKET_s *packet, char **args, int argc, int flags,
                         f32 time) {
  if (packet == 0 || packet->pd0 == 0)
    return 1;
  if (flags != 0) {
    if (sys != 0 && packet->path_set != 0 && packet->path_node != 0) {
      SetPathAIPath_s *active = ((SetPathAISys_s *)sys)->path_sys->active_path;
      if ((SetPathAIPath_s *)packet->path_set != active) {
        AISysCharacterSetPath(packet, active);
        AISysGetCharacterPathPos(g_unk00960894->aiSys2bf8,
                                 (GameObject_s *)packet->pd0, packet, 0xff, 1);
      }
      return 1;
    }
  } else {
    GoToLevelPathNode_s *node = (GoToLevelPathNode_s *)process->action_data_3;
    if (node != 0) {
      nuvec_s d;
      f32 dist = NuVecXZDistSqr(&packet->pd0->pos5c, &node->pos, &d);
      if (dist < node->radius_sqr) {
        memset(&packet->path_set, 0, 0x1c);
        AISysCharacterSetPath(packet,
                              ((SetPathAISys_s *)sys)->path_sys->active_path);
        if ((node->runtime_flags & 1) != 0)
          AISysCharacterSetPathCnx(packet, &packet->pd0->pos5c,
                                   &((GoToLevelPathPath_s *)packet->path_set)
                                        ->connections[node->connection],
                                   0);
        return 1;
      }
      AIMoveInstruction(packet, &node->pos, 0.0f, &process->path_info, 1,
                        packet->movement_param);
    }
  }
  return 0;
}

void Unk00448b20(i32 count);

// FUNCTION: LEGOBATMAN 0x0046f340
i32 Action_SetAO_AttackersPerRow(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                 AIPACKET_s *packet, char **args, int argc,
                                 int flags, f32 time) {
  if (flags != 0) {
    if (argc != 0) {
      for (i32 i = 0; i < argc; i++) {
        char *s = NuStrIStr(args[i], "num");
        if (s != 0)
          Unk00448b20((i32)AIParamToFloat(process, s + 4));
      }
    }
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x00472370
i32 Action_IgnorePhobia(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                        AIPACKET_s *packet, char **args, int argc, int flags,
                        f32 time) {
  GameObject_s *obj = 0;
  i32 on = 1;
  i32 type = 0xff;
  if (flags != 0) {
    if (packet != 0 && packet->pd0 != 0 && packet->pd0->obj != 0)
      obj = packet->pd0->obj;
    for (i32 i = 0; i < argc; i++) {
      char *s = NuStrIStr(args[i], "character=");
      if (s != 0) {
        obj = GetNamedGameObjectInl(sys, s + 10);
      } else if (NuStrICmp(args[i], "FALSE") == 0) {
        on = 0;
      } else if ((s = NuStrIStr(args[i], "type=")) != 0) {
        if (g_unk00ad68ec != 0 && g_unk00ad68fc != 0) {
          type = g_unk00ad68ec(s + 5);
          if (type != 0xff)
            type = g_unk00ad68fc(type);
        }
      }
    }
    if (type != 0xff) {
      GameObject_s *o = Obj;
      for (i32 j = 0; j < HIGHGAMEOBJECT; j++, o++) {
        if ((o->flags1fc & 1) != 0 && (o->flags1fc & 0x1000) != 0 &&
            o->type15b0 == type)
          o->ignore_phobia = on;
      }
    } else if (obj != 0) {
      obj->ignore_phobia = on;
    }
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x004644c0
i32 Action_NotWithParty(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                        AIPACKET_s *packet, char **args, int argc, int flags,
                        f32 time) {
  GameObject_s *obj = 0;
  i32 on = 1;
  if (flags != 0) {
    if (packet != 0 && packet->pd0 != 0 && packet->pd0->obj != 0)
      obj = packet->pd0->obj;
    for (i32 i = 0; i < argc; i++) {
      char *s = NuStrIStr(args[i], "character=");
      if (s != 0)
        obj = GetNamedGameObjectInl(sys, s + 10);
      else if (NuStrICmp(args[i], "FALSE") == 0)
        on = 0;
    }
    if (obj != 0)
      obj->not_with_party = on;
  }
  return 1;
}

void Unk00448af0(i32 count);

// FUNCTION: LEGOBATMAN 0x0046f2a0
i32 Action_SetAO_MaxAttackers(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                              AIPACKET_s *packet, char **args, int argc,
                              int flags, f32 time) {
  if (flags != 0) {
    if (argc != 0) {
      for (i32 i = 0; i < argc; i++) {
        char *s = NuStrIStr(args[i], "max");
        if (s != 0)
          Unk00448af0((i32)AIParamToFloat(process, s + 4));
      }
    }
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x0045b990
i32 Action_DontPush(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                    AIPACKET_s *packet, char **args, int argc, int flags,
                    f32 time) {
  i32 on = 1;
  i32 count = 0;
  GameObject_s *obj = 0;
  i32 types[10];
  if (flags == 0)
    return 1;
  if (packet != 0 && packet->pd0 != 0 && packet->pd0->obj != 0)
    obj = packet->pd0->obj;
  for (i32 i = 0; i < argc; i++) {
    char *s = NuStrIStr(args[i], "type");
    if (s != 0) {
      if (g_unk00ad68ec != 0 && g_unk00ad68fc != 0) {
        i32 type = g_unk00ad68ec(s + 5);
        if (type != 0xff) {
          type = g_unk00ad68fc(type);
          if (type != -1 && count < 10)
            types[count++] = type;
        }
      }
    } else if ((s = NuStrIStr(args[i], "character=")) != 0) {
      obj = GetNamedGameObjectInl(sys, s + 10);
    } else if (NuStrICmp(args[i], "FALSE") == 0) {
      on = 0;
    }
  }
  if (count != 0) {
    GameObject_s *o = Obj;
    for (i32 j = 0; j < HIGHGAMEOBJECT; j++, o++) {
      if ((o->flags1fc & 1) != 0 && (o->flags1fc & 0x1000) != 0 &&
          (o->flags1f8 & 0x400) != 0) {
        for (i32 k = 0; k < count; k++) {
          if (o->type15b0 == types[k])
            o->dont_push = on;
        }
      }
    }
  } else if (obj != 0) {
    obj->dont_push = on;
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x00462cb0
i32 Action_CanTriggerObstacle(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                              AIPACKET_s *packet, char **args, int argc,
                              int flags, f32 time) {
  i32 blocked = 0;
  GIZOBSTACLE_s *obstacle = 0;
  if (flags != 0) {
    if (argc != 0) {
      for (i32 i = 0; i < argc; i++) {
        char *s = NuStrIStr(args[i], "name=");
        if (s != 0) {
          GIZMO_s *gizmo = GizmoFindByName(g_unk00960894->gizmoSys2b0c,
                                           obstacle_gizmotype_id, s + 5);
          if (gizmo != 0)
            obstacle = (GIZOBSTACLE_s *)gizmo->object;
        } else if (NuStrICmp(args[i], "FALSE") == 0) {
          blocked = 1;
        }
      }
      if (obstacle != 0)
        obstacle->stay_shut = blocked;
    }
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x0046d4f0
i32 Action_IgnoreSlideTerrain(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                              AIPACKET_s *packet, char **args, int argc,
                              int flags, f32 time) {
  GameObject_s *obj = 0;
  i32 on = 1;
  if (flags != 0) {
    if (packet != 0 && packet->pd0 != 0)
      obj = packet->pd0->obj;
    if (argc != 0) {
      for (i32 i = 0; i < argc; i++) {
        if (NuStrICmp(args[i], "FALSE") == 0) {
          on = 0;
        } else {
          char *s = NuStrIStr(args[i], "character=");
          if (s != 0)
            obj = GetNamedGameObjectInl(sys, s + 10);
        }
      }
    }
    if (obj != 0)
      obj->ignore_slide_terrain = on;
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x00455e60
i32 Action_DontTargetOthersOpponent(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                    AIPACKET_s *packet, char **args, int argc,
                                    int flags, f32 time) {
  i32 on = 1;
  if (packet == 0 || packet->pd0 == 0 || packet->pd0->obj == 0)
    return 1;
  GameObject_s *obj = packet->pd0->obj;
  if (flags != 0) {
    for (i32 i = 0; i < argc; i++) {
      char *s = NuStrIStr(args[i], "character");
      if (s != 0)
        obj = GetNamedGameObjectInl(sys, s + 10);
      else if (NuStrICmp("FALSE", args[i]) == 0)
        on = 0;
    }
    if (obj != 0)
      obj->dont_target_others_opponent = on;
  }
  return 1;
}

numtx_s *NuCameraGetMtx(void);
void SetHeadTarget(GameObject_s *obj, nuvec_s *target, i32 a, f32 b, f32 c,
                   f32 d);

// FUNCTION: LEGOBATMAN 0x00468190
i32 Action_FaceCamera(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                      AIPACKET_s *packet, char **args, int argc, int flags,
                      f32 time) {
  f32 min_time = 0.0f;
  f32 max_time = 0.0f;
  if (packet == 0 || packet->pd0 == 0 || packet->pd0->obj == 0)
    return 1;
  GameObject_s *obj = packet->pd0->obj;
  if (flags != 0) {
    for (i32 i = 0; i < argc; i++) {
      char *s;
      if (NuStrICmp("look_at_camera", args[i]) == 0)
        process->hold_special_button = 1;
      else if ((s = NuStrIStr(args[i], "mintime")) != 0)
        min_time = AIParamToFloatEx(packet, process, s + 8);
      else if ((s = NuStrIStr(args[i], "maxtime")) != 0)
        max_time = AIParamToFloatEx(packet, process, s + 8);
      else
        process->action_timer = AIParamToFloat(process, args[i]);
    }
    if (process->action_timer == 0.0f && max_time > min_time)
      process->action_timer = NuRandFloat() * (max_time - min_time) + min_time;
  }
  numtx_s *cam = NuCameraGetMtx();
  if (cam != 0) {
    packet->look_target = (nuvec_s *)&cam->m30;
    if (process->hold_special_button != 0)
      SetHeadTarget(obj, obj->look_target, 7, 1.0f, 0.0f, 0.0f);
  }
  if (process->action_timer > 0.0f) {
    process->action_timer -= time;
    if (process->action_timer <= 0.0f) {
      process->action_timer = 0.0f;
      return 1;
    }
  }
  return 0;
}

GIZOBSTACLE_s *GizObstacle_FindByName(struct GIZOBSTACLESYS_s *sys, char *name);

// FUNCTION: LEGOBATMAN 0x00463260
i32 Action_SetObstacleToEnd(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                            AIPACKET_s *packet, char **args, int argc,
                            int flags, f32 time) {
  GIZOBSTACLE_s *obstacle = 0;
  if (flags != 0) {
    if (argc != 0) {
      for (i32 i = 0; i < argc; i++) {
        char *s = NuStrIStr(args[i], "name=");
        if (s != 0)
          obstacle =
              GizObstacle_FindByName(g_unk00960894->giz_obstacle_sys, s + 5);
      }
      if (obstacle != 0)
        GizObstacle_JumpToEnd(obstacle);
    }
  }
  return 1;
}

// GLOBAL: LEGOBATMAN 0x00ab082c
extern u32 LEGO_AIPATHCNX_JUMP_NOW;
// GLOBAL: LEGOBATMAN 0x00ab0830
extern u32 LEGO_AIPATHCNX_DONT_JUMP_NOW;

u32 Unk00461220(char *str); // path connection flag from its name
u32 *AIPAthFindPathCnx(AISYS_s *sys, SetPathAIPath_s *path, char *from,
                       char *to, i32 *direction);

// FUNCTION: LEGOBATMAN 0x004657c0
i32 Action_SetPathCnxFlag(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                          AIPACKET_s *packet, char **args, int argc, int flags,
                          f32 time) {
  i32 direction;
  i32 both_ways = 0;
  i32 set = 1;
  char *from = 0;
  char *to = 0;
  u32 add = 0;
  u32 remove = 0;
  SetPathAISys_s *ai = (SetPathAISys_s *)sys;
  if (sys == 0 || ai->path_sys == 0 || ai->path_sys->path_count == 0 ||
      flags == 0)
    return 1;
  for (i32 i = 0; i < argc; i++) {
    char *s = NuStrIStr(args[i], "from");
    if (s != 0) {
      from = s + 5;
      continue;
    }
    s = NuStrIStr(args[i], "to");
    if (s != 0) {
      to = s + 3;
      continue;
    }
    u32 flag = Unk00461220(args[i]);
    if (flag != 0) {
      add |= flag;
      if (flag == LEGO_AIPATHCNX_JUMP_NOW)
        remove |= LEGO_AIPATHCNX_DONT_JUMP_NOW;
      else if (flag == LEGO_AIPATHCNX_DONT_JUMP_NOW)
        remove |= LEGO_AIPATHCNX_JUMP_NOW;
      else if (flag == 0x20000000)
        both_ways = 1;
      continue;
    }
    if (NuStrICmp(args[i], "bothways") == 0)
      both_ways = 1;
    else if (NuStrICmp(args[i], "FALSE") == 0)
      set = 0;
  }
  if (from != 0 && to != 0) {
    u32 *cnx =
        AIPAthFindPathCnx(sys, ai->path_sys->active_path, from, to, &direction);
    if (cnx != 0) {
      if (set != 0) {
        cnx[direction] |= add;
        cnx[direction] &= ~remove;
      } else {
        cnx[direction] &= ~add;
      }
      if (both_ways != 0) {
        if (set != 0) {
          cnx[!direction] |= add;
          cnx[!direction] &= ~remove;
        } else {
          cnx[!direction] &= ~add;
        }
      }
    }
  }
  return 1;
}

i32 qrand(void);
void GameCam_Judder(GAMECAMERA_s *camera, f32 amount, i32 axis,
                    nuvec_s *source);

// FUNCTION: LEGOBATMAN 0x00460ca0
i32 Action_JudderGameCamera(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                            AIPACKET_s *packet, char **args, int argc,
                            int flags, f32 time) {
  f32 amount = 0.1f;
  i32 axis = 0;
  if (flags != 0) {
    for (i32 i = 0; i < argc; i++) {
      char *s;
      if (NuStrIStr(args[i], "axis=x") != 0)
        axis = 0;
      else if (NuStrIStr(args[i], "axis=y") != 0)
        axis = 1;
      else if (NuStrIStr(args[i], "axis=z") != 0)
        axis = 2;
      else if ((s = NuStrIStr(args[i], "time")) != 0)
        amount = AIParamToFloat(process, s + 5);
    }
    if (axis == 2 && qrand() < 0x8000)
      amount = -amount;
    GameCam_Judder(g_unk0095f624, amount, axis, 0);
  }
  return 1;
}

struct Unk_GameObject4 {
  u8 pad0[0x13a];
  u16 route_mask; // 0x13a, bit per special route index
  u8 route;       // 0x13c, 0xff = none
};

struct SPECIALROUTE_s {
  u8 pad0[0x18];
  unsigned __int64 users;       // 0x18
  unsigned __int64 saved_users; // 0x20
};

SPECIALROUTE_s *AISysFindRouteByName(AISYS_s *sys, char *name, i32 *index);

// STUB: LEGOBATMAN 0x00473e40
// prologue pushes all four registers before the locals are zeroed and the
// object loops keep `index` in ecx; ours differs (3 tries)
i32 Action_ChangeSpecialRoute(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                              AIPACKET_s *packet, char **args, int argc,
                              int flags, f32 time) {
  i32 index = 0;
  i32 clear = 0;
  i32 restore = 0;
  SPECIALROUTE_s *route = 0;
  if (argc > 0) {
    for (i32 i = 0; i < argc; i++) {
      char *s = NuStrIStr(args[i], "route_name=");
      if (s != 0) {
        s += NuStrLen("route_name=");
        route = AISysFindRouteByName(sys, s, &index);
      } else if (NuStrIStr(args[i], "clear_users") != 0) {
        clear = 1;
      } else if (NuStrIStr(args[i], "restore_users") != 0) {
        restore = 1;
      }
    }
    if (route != 0) {
      if (clear != 0) {
        route->users = 0;
        for (i32 j = 0; j < HIGHGAMEOBJECT; j++) {
          if ((Obj[j].flags1fc & 1) != 0 && (Obj[j].flags1fc & 0x1000) != 0) {
            Unk_GameObject4 **p = &Obj[j].p4;
            (*p)->route_mask &= ~(1 << index);
            if ((*p)->route != 0xff && (*p)->route == index)
              (*p)->route = 0xff;
          }
        }
      } else if (restore != 0) {
        if ((route->saved_users & 0x8000000000000000) != 0)
          route->users = (unsigned __int64)-1;
        else
          route->users = route->saved_users;
        for (i32 j = 0; j < HIGHGAMEOBJECT; j++) {
          if ((Obj[j].flags1fc & 1) != 0 && (Obj[j].flags1fc & 0x1000) != 0)
            Obj[j].p4->route_mask |= 1 << index;
        }
      }
    }
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x004719e0
i32 Action_SetCapability(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                         AIPACKET_s *packet, char **args, int argc, int flags,
                         f32 time) {
  GameObject_s *obj = 0;
  i32 on = 1;
  if (argc != 0) {
    if (packet != 0 && packet->pd0 != 0 && packet->pd0->obj != 0)
      obj = packet->pd0->obj;
    u32 caps = 0;
    for (i32 i = 0; i < argc; i++) {
      char *s = NuStrIStr(args[i], "character");
      if (s != 0) {
        obj = GetNamedGameObjectInl(sys, s + 10);
      } else {
        u32 cap = Unk00461220(args[i]);
        if (cap != 0)
          caps |= cap;
        else if (NuStrICmp(args[i], "FALSE") == 0)
          on = 0;
      }
    }
    if (obj != 0 && caps != 0) {
      if (on != 0)
        obj->capabilities |= caps;
      else
        obj->capabilities &= ~caps;
    }
  }
  return 1;
}

struct APIDEBRISENTRY_s {
  i32 effect;      // 0x00
  char name[0x10]; // 0x04
};

struct APIDEBRISSYS_s {
  i32 named_count;           // 0x00
  i32 capacity;              // 0x04
  APIDEBRISENTRY_s *entries; // 0x08
};

// GLOBAL: LEGOBATMAN 0x00ab057c
extern APIDEBRISSYS_s *perm_debrissys;

i32 FindGameDebris(APIDEBRISSYS_s *debris_sys, char *name);
void AddFiniteShotDebrisEffect(i32 *handle, i32 effect, nuvec_s *position,
                               i32 count);

// FUNCTION: LEGOBATMAN 0x00460a30
i32 Action_AddDebris(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                     AIPACKET_s *packet, char **args, int argc, int flags,
                     f32 time) {
  i32 handle = -1;
  i32 type = 0x10;
  nuvec_s pos;
  pos.x = 1000000000.0f;
  pos.y = 1000000000.0f;
  pos.z = 1000000000.0f;
  GameObject_s *obj = 0;
  if (packet != 0 && packet->pd0 != 0 && packet->pd0->obj != 0)
    obj = packet->pd0->obj;
  if (flags != 0) {
    for (i32 i = 0; i < argc; i++) {
      char *s = NuStrIStr(args[i], "character=");
      if (s != 0)
        obj = GetNamedGameObjectInl(sys, s + 10);
      else if ((s = NuStrIStr(args[i], "x=")) != 0)
        pos.x = AIParamToFloat(process, s + 2);
      else if ((s = NuStrIStr(args[i], "y=")) != 0)
        pos.y = AIParamToFloat(process, s + 2);
      else if ((s = NuStrIStr(args[i], "z=")) != 0)
        pos.z = AIParamToFloat(process, s + 2);
      else if ((s = NuStrIStr(args[i], "type")) != 0)
        type = FindGameDebris(perm_debrissys, s + 5);
    }
    if (pos.x != 1000000000.0 && pos.y != 1000000000.0 &&
        pos.z != 1000000000.0) {
      AddFiniteShotDebrisEffect(
          &handle,
          ((APIDEBRISSYS_s *)g_unk00960894->p138)->entries[type].effect, &pos,
          1);
      return 1;
    }
    if (obj != 0)
      AddFiniteShotDebrisEffect(
          &handle,
          ((APIDEBRISSYS_s *)g_unk00960894->p138)->entries[type].effect,
          &obj->v80, 1);
  }
  return 1;
}

void Unk00504cf0(GameObject_s *obj, i32 type);

// FUNCTION: LEGOBATMAN 0x004752b0
i32 Action_FireSeed(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                    AIPACKET_s *packet, char **args, int argc, int flags,
                    f32 time) {
  i32 type = 0;
  if (packet == 0 || packet->pd0 == 0 || packet->pd0->obj == 0)
    return 1;
  GameObject_s *obj = packet->pd0->obj;
  for (i32 i = 0; i < argc; i++) {
    if (NuStrICmp(args[i], "type=character") == 0)
      type = 0;
    else if (NuStrICmp(args[i], "type=buildit") == 0)
      type = 1;
  }
  Unk00504cf0(obj, type);
  return 1;
}

i32 ActionFromName(const char *name);
i16 FindAnimIX(struct Unk_GameObject54 *character, char *name);
void ResetAnimPacket(void *packet, i32 animation);
f32 AnimDuration(i32 id, i32 anim, f32 a, f32 b, i32 c);

// FUNCTION: LEGOBATMAN 0x004628c0
i32 Action_ContextSetAnimation(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                               AIPACKET_s *packet, char **args, int argc,
                               int flags, f32 time) {
  i32 ncycles = 0;
  i32 infinite = 0;
  i16 anim = -1;
  if (packet == 0 || packet->pd0 == 0 || packet->pd0->obj == 0)
    return 1;
  GameObject_s *obj = packet->pd0->obj;
  if (flags != 0) {
    for (i32 i = 0; i < argc; i++) {
      char *s = NuStrIStr(args[i], "action=");
      if (s != 0)
        anim = ActionFromName(s + 7);
      else if ((s = NuStrIStr(args[i], "anim=")) != 0)
        anim = FindAnimIX(packet->pd0->character, s + 5);
      else if ((s = NuStrIStr(args[i], "ncycles=")) != 0)
        ncycles = (i32)AIParamToFloat(process, s + 8);
      else if (NuStrIStr(args[i], "infinite") != 0)
        infinite = 1;
      else if (NuStrIStr(args[i], "wait_until_finished") != 0)
        process->action_data_1 = 1;
    }
    if (anim != -1) {
      ResetAnimPacket((u8 *)obj + 8, anim);
      obj->b9db = 0x73;
      obj->s9d0 = anim;
      process->action_data_6 = anim;
      if (infinite != 0)
        obj->f98c = 1000000000.0f;
      else if (ncycles != 0)
        obj->f98c = AnimDuration(obj->type15b0, anim, 0.0f, 0.0f, 1) * ncycles;
      else
        obj->f98c = AnimDuration(obj->type15b0, anim, 0.0f, 0.0f, 1);
    }
  }
  if (process->action_data_1 != 0 && obj->b9db == 0x73 &&
      obj->s9d0 == process->action_data_6)
    return 0;
  return 1;
}

struct ADDPART_s {
  numtx_s *matrix; // 0x00
  u32 pad4;
  nuvec_s *velocity; // 0x08
  u32 padc[2];
  f32 f14;     // 0x14
  f32 f18;     // 0x18
  f32 gravity; // 0x1c
  u32 pad20;
  nuhspecial_s *special; // 0x24
  u32 pad28;
  u32 flags; // 0x2c
  u32 pad30[4];
  void (*collide)(PART_s *part); // 0x40
  u32 pad44[(0x9c - 0x44) / 4];
  f32 time_step; // 0x9c
  u32 pada0[(0xd8 - 0xa0) / 4];
};

// GLOBAL: LEGOBATMAN 0x0095e070
extern ADDPART_s Default_ADDPART;
// GLOBAL: LEGOBATMAN 0x00941728
extern f32 ForceThrowGravity;
// GLOBAL: LEGOBATMAN 0x0094172c
extern f32 ForceThrowSpeed;
extern f32 FRAMETIME;

void MakeThrowVector(nuvec_s *out, nuvec_s *from, nuvec_s *to, nuvec_s *vel,
                     f32 speed, f32 gravity);
void NuMtxSetTranslation(numtx_s *m, nuvec_s *v);
void PartCollide_3D(PART_s *part);
PART_s *AddPart(ADDPART_s *part);
void NewRumble(nupad_s *pad, f32 strength, i32 frames);

// FUNCTION: LEGOBATMAN 0x0045e830
i32 Action_AddPart(AISYS_s *sys, AISCRIPTPROCESS_s *process, AIPACKET_s *packet,
                   char **args, int argc, int flags, f32 time) {
  if (flags != 0) {
    nuhspecial_s special;
    nuvec_s pos;
    memset(&special, 0, sizeof(special));
    for (i32 i = 0; i < argc; i++) {
      char *s = NuStrIStr(args[i], "name");
      if (s != 0)
        NuSpecialFind(g_unk00960894->scn140, &special, s + 5, 1);
      else if ((s = NuStrIStr(args[i], "x")) != 0)
        pos.x = AIParamToFloat(process, s + 2);
      else if ((s = NuStrIStr(args[i], "y")) != 0)
        pos.y = AIParamToFloat(process, s + 2);
      else if ((s = NuStrIStr(args[i], "z")) != 0)
        pos.z = AIParamToFloat(process, s + 2);
    }
    if (NuSpecialExistsFn(&special) != 0) {
      nuvec_s vel;
      numtx_s mtx;
      MakeThrowVector(&vel, &pos, &player->v80, &player->velocity,
                      ForceThrowSpeed, ForceThrowGravity);
      NuMtxSetTranslation(&mtx, &pos);
      ADDPART_s part = Default_ADDPART;
      part.f14 = 0.1f;
      part.f18 = 0.1f;
      part.gravity = ForceThrowGravity;
      part.time_step = FRAMETIME;
      part.matrix = &mtx;
      part.velocity = &vel;
      part.special = &special;
      part.flags = 0x29b;
      part.collide = PartCollide_3D;
      AddPart(&part);
      NewRumble(player->p112c->pad0, 0.5f, 0);
    }
  }
  return 1;
}

struct nugspline_s *NuSplineFind(nugscn_s *scene, char *name);
void InitSplinePosition(void *pos, struct nugspline_s *spline, f32 t,
                        i32 looping);

// STUB: LEGOBATMAN 0x0045bd40
// spline/argc swap ebx and ebp (decl order, locals, pos temp all tried)
i32 Action_SetSpline(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                     AIPACKET_s *packet, char **args, int argc, int flags,
                     f32 time) {
  struct nugspline_s *spline = 0;
  i32 looping = 0;
  if (packet == 0 || packet->pd0 == 0 || packet->pd0->obj == 0)
    return 1;
  GameObject_s *obj = packet->pd0->obj;
  if (flags == 0)
    return 1;
  for (i32 i = 0; i < argc; i++) {
    char *s = NuStrIStr(args[i], "spline=");
    if (s != 0)
      spline = NuSplineFind(g_unk00960894->scn140, s + 7);
    else if (NuStrICmp(args[i], "looping") == 0)
      looping = 1;
  }
  memset(&obj->movement_spline, 0, 0x20);
  if (spline != 0)
    InitSplinePosition(&obj->movement_spline, spline, 0.0f, looping);
  return 1;
}

void PlayRepeatSfx(char *name, i32 sfx_id, f32 initial_delay, char play_count,
                   f32 repeat_delay, nuvec_s *pos);

// FUNCTION: LEGOBATMAN 0x00465ab0
i32 Action_PlaySfx(AISYS_s *sys, AISCRIPTPROCESS_s *process, AIPACKET_s *packet,
                   char **args, int argc, int flags, f32 time) {
  f32 start_delay = 0.0f;
  f32 repeat_delay = 0.0f;
  nuvec_s pos;
  pos.x = 1000000000.0f;
  pos.y = 1000000000.0f;
  pos.z = 1000000000.0f;
  char play_count = 1;
  char *name = 0;
  nuvec_s *pos_ptr = 0;
  if (flags == 0)
    return 1;
  for (i32 i = 0; i < argc; i++) {
    char *s = NuStrIStr(args[i], "name=");
    if (s != 0)
      name = s + 5;
    else if ((s = NuStrIStr(args[i], "playcount=")) != 0)
      play_count = (char)AIParamToFloat(process, s + 10);
    else if ((s = NuStrIStr(args[i], "repdelay=")) != 0)
      repeat_delay = AIParamToFloat(process, s + 9);
    else if ((s = NuStrIStr(args[i], "startdelay=")) != 0)
      start_delay = AIParamToFloat(process, s + 11);
    else if ((s = NuStrIStr(args[i], "x=")) != 0)
      pos.x = AIParamToFloat(process, s + 2);
    else if ((s = NuStrIStr(args[i], "y=")) != 0)
      pos.y = AIParamToFloat(process, s + 2);
    else if ((s = NuStrIStr(args[i], "z=")) != 0)
      pos.z = AIParamToFloat(process, s + 2);
    else if ((s = NuStrIStr(args[i], "character_pos=")) != 0)
      pos_ptr = &Unk0044c930(sys, s + 14)->v80;
  }
  if (pos_ptr == 0 && pos.x != 1000000000.0 && pos.y != 1000000000.0 &&
      pos.z != 1000000000.0)
    pos_ptr = &pos;
  if (name != 0)
    PlayRepeatSfx(name, -1, start_delay, play_count, repeat_delay, pos_ptr);
  return 1;
}

void NuVecAdd(nuvec_s *out, nuvec_s *a, nuvec_s *b);

// FUNCTION: LEGOBATMAN 0x004688a0
i32 Action_SpinOnSpot(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                      AIPACKET_s *packet, char **args, int argc, int flags,
                      f32 time) {
  f32 min_time = 0.0f;
  f32 max_time = 0.0f;
  if (packet == 0 || packet->pd0 == 0 || packet->pd0->obj == 0)
    return 1;
  GameObject_s *obj = packet->pd0->obj;
  if (flags != 0) {
    for (i32 i = 0; i < argc; i++) {
      char *s = NuStrIStr(args[i], "mintime=");
      if (s != 0)
        min_time = AIParamToFloatEx(packet, process, s + 8);
      else if ((s = NuStrIStr(args[i], "maxtime=")) != 0)
        max_time = AIParamToFloatEx(packet, process, s + 8);
      else if ((s = NuStrIStr(args[i], "time=")) != 0)
        process->action_timer = AIParamToFloatEx(packet, process, s + 5);
      else if ((s = NuStrIStr(args[i], "rot_rate=")) != 0)
        process->action_data_4 =
            (f32)(i32)(AIParamToFloatEx(packet, process, s + 9) *
                       182.04444885253906);
    }
    if (min_time != max_time)
      process->action_timer = NuRandFloat() * (max_time - min_time) + min_time;
    process->action_data_6 = obj->u246;
  }
  process->action_data_6 += (i16)(process->action_data_4 * time);
  process->action_pos.x = 0.0f;
  process->action_pos.y = 0.0f;
  process->action_pos.z = 1.0f;
  NuVecRotateY(&process->action_pos, &process->action_pos,
               process->action_data_6);
  NuVecAdd(&process->action_pos, &process->action_pos, &obj->v80);
  packet->look_target = &process->action_pos;
  if (process->action_timer > 0.0f) {
    process->action_timer -= time;
    if (process->action_timer <= 0.0f) {
      process->action_timer = 0.0f;
      return 1;
    }
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x0045f740
i32 Action_SetHoverPhase(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                         AIPACKET_s *packet, char **args, int argc, int flags,
                         f32 time) {
  if (packet == 0 || packet->pd0 == 0 || packet->pd0->obj == 0)
    return 1;
  GameObject_s *obj = packet->pd0->obj;
  if (flags != 0) {
    process->action_data_1 = 1;
    for (i32 i = 0; i < argc; i++) {
      if (NuStrICmp(args[i], "FALSE") == 0)
        process->action_data_1 = 0;
    }
  }
  if (process->action_data_1 != 0) {
    if (obj->b131c != 1) {
      *(u32 *)((u8 *)obj->p112c + 8) |= g_unk0095f728;
      return 0;
    }
  } else if (obj->b131c == 1) {
    *(u32 *)((u8 *)obj->p112c + 8) |= g_unk0095f728;
    return 0;
  }
  return 1;
}

i32 AISysSetLevelPath(AISYS_s *sys, char *name);

// FUNCTION: LEGOBATMAN 0x00460f70
i32 Action_SetLevelPath(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                        AIPACKET_s *packet, char **args, int argc, int flags,
                        f32 time) {
  char *name = 0;
  if (flags != 0 && ((SetPathAISys_s *)sys)->path_sys != 0) {
    for (i32 i = 0; i < argc; i++) {
      char *s = NuStrIStr(args[i], "name");
      if (s != 0)
        name = s + 5;
    }
    if (AISysSetLevelPath(sys, name) != 0) {
      for (i32 i = 0; i < 8; i++) {
        GameObject_s *obj = Player[i];
        if (obj != 0 && (obj->flags1fc & 1) != 0 &&
            (obj->flags1fc & 0x1000) != 0) {
          AISysCharacterSetPath(obj->process290,
                                ((SetPathAISys_s *)sys)->path_sys->active_path);
          AISysGetCharacterPathPos(g_unk00960894->aiSys2bf8, obj,
                                   obj->process290, 0xff, obj->b24d);
        }
      }
    }
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x004635e0
i32 Action_SnapWeaponOut(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                         AIPACKET_s *packet, char **args, int argc, int flags,
                         f32 time) {
  GameObject_s *obj = 0;
  i32 keep_out = -1;
  i32 on = 1;
  if (flags != 0) {
    if (packet != 0 && packet->pd0 != 0)
      obj = packet->pd0->obj;
    if (argc != 0) {
      for (i32 i = 0; i < argc; i++) {
        char *s = NuStrIStr(args[i], "character=");
        if (s != 0)
          obj = GetNamedGameObjectInl(sys, s + 10);
        else if ((s = NuStrIStr(args[i], "keep_out=")) != 0)
          keep_out = NuStrICmp(s + 9, "TRUE") == 0;
        else if (NuStrICmp(args[i], "FALSE") == 0)
          on = 0;
      }
    }
    if (obj != 0) {
      if (on != 0) {
        obj->flags130c |= 0x80000;
        obj->weapon_scale = 1.0f;
        if (keep_out != -1)
          obj->keep_weapon_out = keep_out;
      } else {
        obj->weapon_scale = 0.0f;
        obj->flags130c &= ~0x80000;
        obj->keep_weapon_out = 0;
      }
    }
  }
  return 1;
}

extern i32 g_unk0095fb5c; // techno gizmo type id
extern f32 g_unk009ca98c; // GameShadow probe height
// GLOBAL: LEGOBATMAN 0x0099e294
extern f32 ai_moveradius;
// GLOBAL: LEGOBATMAN 0x0095f73c
extern u32 g_unk0095f73c; // toggle-right pad bit

f32 GameShadow(GameObject_s *object, nuvec_s *position, f32 probe_height,
               i32 terrain_mask);
void AISysGetPathPos2(AISYS_s *sys, nuvec_s *pos, AIPATHINFO *info,
                      nuvec_s *out, struct AIPATHSET_s *path, i32 a);
i32 GizTechno_CanUseTechno(GameObject_s *obj, TECHNO_s *techno);

// 0x254-byte trigger sets at WORLDINFO_s::ai_trigger_set_sys.
struct AITRIGGERSET_s {
  u8 pad0[0x250];
  u16 flags; // 0x250
  u8 pad252[0x254 - 0x252];
};

i32 Action_HelpWithTriggers(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                            AIPACKET_s *packet, char **args, int argc,
                            int flags, f32 time);

// FUNCTION: LEGOBATMAN 0x00469f60
i32 Action_UseTriggerSet(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                         AIPACKET_s *packet, char **args, int argc, int flags,
                         f32 time) {
  GameObject_s *obj;
  AITRIGGERSET_s *set;
  if (packet == NULL || packet->pd0 == NULL || packet->pd0->obj == NULL ||
      sys == NULL || *(GameObject_s **)((u8 *)sys + 0x1698) == NULL)
    return 1;
  obj = packet->pd0->obj;
  if (flags != 0 && g_unk00960894->ai_trigger_set_sys != NULL) {
    for (i32 i = 0; i < argc; i++) {
      char *value = NuStrIStr(args[i], "set=");
      if (value != NULL) {
        i32 index = (i32)AIParamToFloat(process, value + 4);
        if ((u32)(index - 1) <= 31)
          process->action_data_3 =
              &((AITRIGGERSET_s *)g_unk00960894->ai_trigger_set_sys)[index - 1];
      }
    }
  }
  set = (AITRIGGERSET_s *)process->action_data_3;
  if (set != NULL) {
    obj->active_trigger_set = set;
    set->flags |= 2;
    Action_HelpWithTriggers(sys, process, packet, args, argc, flags, time);
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x0046a670
i32 Action_UseTechno(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                     AIPACKET_s *packet, char **args, int argc, int flags,
                     f32 time) {
  if (packet == 0 || packet->pd0 == 0 || packet->pd0->obj == 0)
    return 1;
  GameObject_s *obj = packet->pd0->obj;
  if (flags != 0) {
    for (i32 i = 0; i < argc; i++) {
      char *s = NuStrIStr(args[i], "name=");
      if (s == 0)
        continue;
      GIZMO_s *gizmo =
          GizmoFindByName(g_unk00960894->gizmoSys2b0c, g_unk0095fb5c, s + 5);
      if (gizmo == 0 || gizmo->object == 0)
        continue;
      TECHNO_s *techno = (TECHNO_s *)gizmo->object;
      process->action_data_3 = techno;
      process->action_pos = techno->ground_position;
      f32 height = GameShadow(0, &process->action_pos, g_unk009ca98c, -1);
      if (height != 2000000.0)
        process->action_pos.y = height;
      AISysGetPathPos2(sys, &process->action_pos, &process->path_info,
                       &process->action_pos, packet->path_set, 0xff);
    }
  }
  TECHNO_s *techno = (TECHNO_s *)process->action_data_3;
  if (techno == 0 || !techno->active || !techno->visible || techno->complete)
    return 1;
  AIMoveInstruction(packet, &process->action_pos, 0.0f, &process->path_info, 1,
                    0.0f);
  if (GizTechno_CanUseTechno(obj, techno) == 0) {
    if (FreePlay != 0) {
      process->action_timer -= time;
      if (process->action_timer < 0.0f) {
        process->action_timer = 0.5f;
        *(u32 *)((u8 *)obj->p112c + 8) |= g_unk0095f73c;
      }
    }
  } else {
    f32 dist = NuVecDistSqr((nuvec_s *)((u8 *)packet + 0x14c),
                            &process->action_pos, 0);
    if (dist < ai_moveradius * ai_moveradius) {
      packet->look_target = &techno->position;
      GameObjectSetCanUse(obj, techno, 2, 1, 0.0f);
    }
  }
  return 0;
}

void GizmoActivate(GIZMOSYS_s *sys, GIZMO_s *gizmo, i32 active, i32 a);

// FUNCTION: LEGOBATMAN 0x0046fe90
i32 Action_GizmoActivate(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                         AIPACKET_s *packet, char **args, int argc, int flags,
                         f32 time) {
  GIZMO_s *gizmo = 0;
  i32 active = 1;
  if (flags != 0) {
    for (i32 i = 0; i < argc; i++) {
      char *s = NuStrIStr(args[i], "name=");
      if (s != 0) {
        s += NuStrLen("name=");
        gizmo = GizmoFindByName(g_unk00960894->gizmoSys2b0c, -1, s);
      } else if (NuStrICmp(args[i], "FALSE") == 0) {
        active = 0;
      }
    }
    if (gizmo != 0)
      GizmoActivate(g_unk00960894->gizmoSys2b0c, gizmo, active, 1);
  }
  return 1;
}

void AddGameMsgCount(nuvec_s *pos, i32 count, i32 total, u8 red, u8 green,
                     u8 blue, f32 scale);

// FUNCTION: LEGOBATMAN 0x0046cd90
i32 Action_AddGameMsgCount(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                           AIPACKET_s *packet, char **args, int argc, int flags,
                           f32 time) {
  i32 count = 0;
  i32 total = 0;
  u8 red = 0;
  u8 green = 0;
  u8 blue = 0;
  nuvec_s *pos = 0;
  GameObject_s *obj = 0;
  if (flags != 0) {
    if (packet != 0 && packet->pd0 != 0 && packet->pd0->obj != 0)
      obj = packet->pd0->obj;
    for (i32 i = 0; i < argc; i++) {
      char *s;
      if (NuStrICmp("mypos", args[i]) == 0 && obj != 0) {
        pos = &obj->v80;
      } else if ((s = NuStrIStr(args[i], "total=")) != 0) {
        total = (i32)AIParamToFloat(process, s + 6);
      } else if ((s = NuStrIStr(args[i], "count=")) != 0) {
        count = (i32)AIParamToFloat(process, s + 6);
      } else if ((s = NuStrIStr(args[i], "messageval=")) != 0) {
        GIZAIMESSAGE_s *msg = CheckGizAIMessage(gizaimessagesys, s + 11, 0);
        if (msg != 0)
          count = (i32)msg->value;
      } else if ((s = NuStrIStr(args[i], "Red=")) != 0) {
        red = (u8)AIParamToFloat(process, s + 4);
      } else if ((s = NuStrIStr(args[i], "Green=")) != 0) {
        green = (u8)AIParamToFloat(process, s + 6);
      } else if ((s = NuStrIStr(args[i], "Blue=")) != 0) {
        blue = (u8)AIParamToFloat(process, s + 5);
      }
    }
    if (count != 0 && count <= total)
      AddGameMsgCount(pos, count, total, red, green, blue, 0.75f);
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x0045bcb0
i32 Action_SetControlSystem(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                            AIPACKET_s *packet, char **args, int argc,
                            int flags, f32 time) {
  if (packet == 0 || packet->pd0 == 0 || packet->pd0->obj == 0)
    return 1;
  GameObject_s *obj = packet->pd0->obj;
  if (flags != 0) {
    obj->control_rotational = 0;
    for (i32 i = 0; i < argc; i++) {
      if (NuStrICmp(args[i], "rotational") == 0)
        obj->control_rotational = 1;
    }
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x00462bc0
i32 Action_AlwaysTriggerObstacle(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                 AIPACKET_s *packet, char **args, int argc,
                                 int flags, f32 time) {
  i32 on = 1;
  GIZOBSTACLE_s *obstacle = 0;
  if (flags != 0) {
    if (argc != 0) {
      for (i32 i = 0; i < argc; i++) {
        char *s = NuStrIStr(args[i], "name=");
        if (s != 0) {
          GIZMO_s *gizmo = GizmoFindByName(g_unk00960894->gizmoSys2b0c,
                                           obstacle_gizmotype_id, s + 5);
          if (gizmo != 0)
            obstacle = (GIZOBSTACLE_s *)gizmo->object;
        } else if (NuStrICmp(args[i], "FALSE") == 0) {
          on = 0;
        }
      }
      if (obstacle != 0)
        obstacle->stay_open = on;
    }
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x00455d70
i32 Action_SetDontMove(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                       AIPACKET_s *packet, char **args, int argc, int flags,
                       f32 time) {
  GameObject_s *obj = 0;
  i32 on = 1;
  if (flags != 0) {
    if (packet != 0 && packet->pd0 != 0 && packet->pd0->obj != 0)
      obj = packet->pd0->obj;
    for (i32 i = 0; i < argc; i++) {
      char *s = NuStrIStr(args[i], "character");
      if (s != 0)
        obj = GetNamedGameObjectInl(sys, s + 10);
      else if (NuStrICmp("FALSE", args[i]) == 0)
        on = 0;
    }
    if (obj != 0)
      obj->dont_move = on;
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x00460ef0
i32 Action_PlayCutScene(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                        AIPACKET_s *packet, char **args, int argc, int flags,
                        f32 time) {
  char *name = 0;
  if (flags != 0) {
    for (i32 i = 0; i < argc; i++) {
      char *s = NuStrIStr(args[i], "name");
      if (s != 0)
        name = s + 5;
    }
    if (name != 0)
      NewCutScene(0, g_unk00960894->cutscene_sys, name, 0);
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x004634e0
i32 Action_KeepWeaponOut(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                         AIPACKET_s *packet, char **args, int argc, int flags,
                         f32 time) {
  GameObject_s *obj = 0;
  i32 on = 1;
  if (flags != 0) {
    if (packet != 0 && packet->pd0 != 0)
      obj = packet->pd0->obj;
    if (argc != 0) {
      for (i32 i = 0; i < argc; i++) {
        if (NuStrICmp(args[i], "FALSE") == 0) {
          on = 0;
        } else {
          char *s = NuStrIStr(args[i], "character=");
          if (s != 0)
            obj = GetNamedGameObjectInl(sys, s + 10);
        }
      }
    }
    if (obj != 0)
      obj->keep_weapon_out = on;
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x00464380
i32 Action_CannotDropIn(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                        AIPACKET_s *packet, char **args, int argc, int flags,
                        f32 time) {
  GameObject_s *obj = 0;
  i32 on = 1;
  if (flags != 0) {
    if (packet != 0 && packet->pd0 != 0 && packet->pd0->obj != 0)
      obj = packet->pd0->obj;
    for (i32 i = 0; i < argc; i++) {
      char *s = NuStrIStr(args[i], "character=");
      if (s != 0)
        obj = GetNamedGameObjectInl(sys, s + 10);
      else if ((s = NuStrIStr(args[i], "tag_to=")) != 0)
        GetNamedGameObjectInl(sys, s + 7);
      else if (NuStrICmp(args[i], "FALSE") == 0)
        on = 0;
    }
    if (obj != 0)
      obj->cannot_drop_in = on;
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x0046d3d0
i32 Action_AwkwardShapeOverride(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                AIPACKET_s *packet, char **args, int argc,
                                int flags, f32 time) {
  GameObject_s *obj = 0;
  i32 on = 1;
  if (flags != 0) {
    if (packet != 0 && packet->pd0 != 0)
      obj = packet->pd0->obj;
    if (argc != 0) {
      for (i32 i = 0; i < argc; i++) {
        if (NuStrICmp(args[i], "FALSE") == 0) {
          on = 0;
        } else {
          char *s = NuStrIStr(args[i], "character=");
          if (s != 0)
            obj = GetNamedGameObjectInl(sys, s + 10);
        }
      }
    }
    if (obj != 0) {
      if (obj->b9db == 0x3c || obj->p1158 != 0)
        Player_ClearContext(obj, 1);
      obj->awkward_shape_override = on;
    }
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x0045e740
i32 Action_CopyMessage(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                       AIPACKET_s *packet, char **args, int argc, int flags,
                       f32 time) {
  char *from = 0;
  char *to = 0;
  if (flags != 0 && gizaimessagesys != 0 && argc > 0) {
    for (i32 i = 0; i < argc; i++) {
      char *s = NuStrIStr(args[i], "from=");
      if (s != 0)
        from = s + 5;
      else if ((s = NuStrIStr(args[i], "to=")) != 0)
        to = s + 3;
    }
    if (from != 0 && to != 0) {
      GIZAIMESSAGE_s *src = CheckGizAIMessage(gizaimessagesys, from, 0);
      GIZAIMESSAGE_s *dst = CheckGizAIMessage(gizaimessagesys, to, 0);
      if (src != 0 && dst != 0)
        dst->value = src->value;
    }
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x00468b60
i32 Action_FaceLocator(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                       AIPACKET_s *packet, char **args, int argc, int flags,
                       f32 time) {
  if (packet == 0 || packet->pd0 == 0 || packet->pd0->obj == 0)
    return 1;
  GameObject_s *obj = packet->pd0->obj;
  if (flags != 0) {
    process->action_data_3 = process->locator;
    for (i32 i = 0; i < argc; i++) {
      char *s = NuStrIStr(args[i], "locator");
      if (s != 0)
        process->action_data_3 = AIPathFindLocator(sys, s + 8);
      else if ((s = NuStrIStr(args[i], "time")) != 0)
        process->action_timer = AIParamToFloat(process, s + 5);
    }
  }
  AILOCATOR_s *locator = (AILOCATOR_s *)process->action_data_3;
  if (locator != 0) {
    packet->look_target = (nuvec_s *)((u8 *)locator + 0x10);
    SetHeadTarget(obj, obj->look_target, 7, 1.0f, 0.0f, 0.0f);
  }
  if (process->action_timer > 0.0f) {
    process->action_timer -= time;
    if (process->action_timer <= 0.0f) {
      process->action_timer = 0.0f;
      return 1;
    }
  }
  return 0;
}

struct DOOR_s {
  u8 pad0[0xfb];
  u8 locked; // 0xfb, Action_ActivateDoor: set from "FALSE"
};

DOOR_s *Door_FindByName(WORLDINFO_s *wi, char *name);
void Unk006158f0(WORLDINFO_s *wi, DOOR_s *door, i32 a, i32 b);

// FUNCTION: LEGOBATMAN 0x00475700
i32 Action_MakeTargetableByTorps(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                 AIPACKET_s *packet, char **args, int argc,
                                 int flags, f32 time) {
  if (packet != NULL && packet->pd0 != NULL && packet->pd0->obj != NULL) {
    if (flags) {
      GameObject_s *target = NULL;
      i32 on = 1;
      i32 i;
      for (i = 0; i < argc; i++) {
        char *s = NuStrIStr(args[i], "name=");
        if (s != NULL) {
          s += NuStrLen("name=");
          target = GetNamedGameObjectInl(sys, s);
        } else if (NuStrIStr(args[i], "FALSE") != NULL)
          on = 0;
      }
    }
    return 0;
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x004757c0
i32 Action_ActivateDoor(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                        AIPACKET_s *packet, char **args, int argc, int flags,
                        f32 time) {
  i32 i;
  DOOR_s *door = NULL;
  i32 activate = 1;
  for (i = 0; i < argc; i++) {
    char *s = NuStrIStr(args[i], "name=");
    if (s != NULL) {
      s += NuStrLen("name=");
      door = Door_FindByName(g_unk00960894, s);
    }
    if (NuStrIStr(args[i], "FALSE") != NULL)
      activate = 0;
  }
  if (door != NULL)
    door->locked = (activate == 0);
  return 1;
}

// FUNCTION: LEGOBATMAN 0x00475c50
i32 Action_GoThroughDoor(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                         AIPACKET_s *packet, char **args, int argc, int flags,
                         f32 time) {
  char *name = NULL;
  i32 i;
  for (i = 0; i < argc; i++) {
    char *s = NuStrIStr(args[i], "Name=");
    if (s != NULL)
      name = s + NuStrLen("Name=");
  }
  if (name != NULL) {
    DOOR_s *door = Door_FindByName(g_unk00960894, name);
    if (door != NULL)
      Unk006158f0(g_unk00960894, door, 0, 1);
  }
  return 1;
}

void Unk005bf1c0(GameObject_s *obj);
void Unk006b6c40(AISYS_s *sys, GameObject_s *obj, AIPACKET_s *packet, i32 a,
                 i32 b);

// FUNCTION: LEGOBATMAN 0x00475cf0
i32 Action_UpdatePathInfo(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                          AIPACKET_s *packet, char **args, int argc, int flags,
                          f32 time) {
  i32 reset = 0;
  GameObject_s *obj;
  if (packet == NULL || packet->pd0 == NULL || packet->pd0->obj == NULL)
    return 1;
  obj = packet->pd0->obj;
  for (i32 i = 0; i < argc; i++) {
    char *value = NuStrIStr(args[i], "character=");
    if (value != NULL)
      obj = GetNamedGameObjectInl(sys, value + 10);
    else if (NuStrICmp("reset_route", args[i]) == 0)
      reset = 1;
  }
  if (obj != NULL) {
    if (reset) {
      obj->b3cc = 0xff;
      obj->b3cd = 0;
    }
    Unk005bf1c0(obj);
    Unk006b6c40(g_unk00960894->aiSys2bf8, obj, (AIPACKET_s *)obj->process290,
                0xff, 1);
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x00477ab0
i32 Action_CycleCharacter(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                          AIPACKET_s *packet, char **args, int argc, int flags,
                          f32 time) {
  i32 single_switch = 0;
  if (packet == NULL || packet->pd0 == NULL || packet->pd0->obj == NULL)
    return 1;
  GameObject_s *obj = packet->pd0->obj;
  if (FreePlay == 0 || obj->b24c == -1)
    return 1;
  if (flags != 0) {
    process->action_data_1 = 0xff;
    for (i32 i = 0; i < argc; i++) {
      char *value = NuStrIStr(args[i], "time");
      if (value != NULL) {
        value += NuStrLen("time") + 1;
        process->action_data_4 = AIParamToFloat(process, value);
        continue;
      }
      value = NuStrIStr(args[i], "ability");
      if (value != NULL) {
        value += NuStrLen("Ability") + 1;
        u32 ability = Unk00461220(value);
        if (ability != 0)
          process->action_data_1 = (u8)ability;
        continue;
      }
      value = NuStrIStr(args[i], "min_speed=");
      if (value != NULL) {
        process->action_data_5 = AIParamToFloat(process, value + 10);
        continue;
      }
      if (NuStrICmp(args[i], "SingleSwitch") == 0)
        single_switch = 1;
    }
  }
  if (obj->b9db == 0xb)
    return 0;
  i32 ability = process->action_data_1;
  if (ability != -1) {
    if (process->action_data_5 == 0.0f ||
        packet->pd0->character->p24->run_speed >= process->action_data_5) {
      if (obj->capabilities & ability)
        return 1;
    }
    process->action_timer += time;
    if (process->action_timer > process->action_data_4) {
      process->action_timer = 0.0f;
      obj->p112c->buttons8 |= g_unk0095f73c;
    }
    return 0;
  }
  if (process->action_data_5 != 0.0f) {
    if (packet->pd0->character->p24->run_speed >= process->action_data_5)
      return 1;
    process->action_timer += time;
    if (process->action_timer > process->action_data_4) {
      process->action_timer = 0.0f;
      obj->p112c->buttons8 |= g_unk0095f73c;
    }
  } else if (single_switch != 0) {
    obj->p112c->buttons8 |= g_unk0095f73c;
    return 1;
  }
  return 1;
}
