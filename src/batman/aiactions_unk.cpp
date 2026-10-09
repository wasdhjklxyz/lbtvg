// batman/, file unknown: AI script Action_* handlers
// (0x004556b0..0x004717b0).

#include "../gameapi/ai/aisys_unk.h"
#include "../nu2api/nucore/nulist.h"
#include "../nu2api/nucore/nustring.h"
#include "worldinfo_unk.h"
#include <string.h>

void Detonate(nuvec_s *pos, i32 type, f32 scale);
void AddMiscPickups(nuvec_s *pos, i32 player_id, i32 coins, i32 torpedoes,
                    i32 a);
void StunGameObject(GameObject_s *target, GameObject_s *by, f32 time,
                    i32 flags);

static inline GameObject_s *GetNamedGameObject(AISYS_s *sys, char *name) {
  Unk_AIPacketObj *api;
  if (GetNamedAPIObjectFn && (api = GetNamedAPIObjectFn(sys, name)))
    return api->obj;
  return 0;
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
      obj = GetNamedGameObject(sys, value + 10);
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
        obj = GetNamedGameObject(sys, s + 10);
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

struct GIZMO_s {
  void *object; // 0x00
};

// GLOBAL: LEGOBATMAN 0x0095ff24
extern i32 obstacle_gizmotype_id;

GIZMO_s *GizmoFindByName(GIZMOSYS_s *gizmo_sys, i32 type_id, char *name);
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
        obj = GetNamedGameObject(sys, s + 10);
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
        obj = GetNamedGameObject(sys, s);
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
        obj = GetNamedGameObject(sys, s + 10);
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
  u8 pad0[0x8b];
  u8 flags8b_lo : 3;
  u8 complete : 1; // 0x8b bit 3
  u8 flags8b_hi : 4;
};

i32 GizmoGetTypeIDByName(GIZMOSYS_s *gizmo_sys, char *name);
TECHNO_s *Technos_FindControllingTechno(GameObject_s *object);

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
        obj = GetNamedGameObject(g_unk00960894->aiSys2bf8, s);
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
// GLOBAL: LEGOBATMAN 0x0095f72c
extern u32 g_unk0095f72c;
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

struct Unk_WorldApiObjSys {
  u8 pad0[0x210];
  u32 flags210; // 0x210
};

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
      obj = GetNamedGameObject(sys, s + 10);
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
        obj = GetNamedGameObject(sys, s + 10);
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

void Player_ClearContext(GameObject_s *obj, i32 mode);
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
          obj = GetNamedGameObject(sys, s + 10);
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
        victim = GetNamedGameObject(sys, s + 7);
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
        obj = GetNamedGameObject(sys, s + 10);
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
        obj = GetNamedGameObject(sys, s + 10);
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
        obj = GetNamedGameObject(sys, s + 10);
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
        obj = GetNamedGameObject(sys, s + 10);
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
        obj = GetNamedGameObject(sys, s + 10);
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
          obj = GetNamedGameObject(sys, s + 10);
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
            obj = GetNamedGameObject(sys, s + 10);
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
        obj = GetNamedGameObject(sys, s + 10);
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

// GLOBAL: LEGOBATMAN 0x00ad68ec
extern u8 (*g_unk00ad68ec)(char *name);
// GLOBAL: LEGOBATMAN 0x00ad68fc
extern i32 (*g_unk00ad68fc)(i32 id);
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
          obj2 = GetNamedGameObject(sys, s);
        else
          obj = GetNamedGameObject(sys, s);
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
        obj = GetNamedGameObject(sys, s + 10);
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
        process->action_data_3 = GetNamedGameObject(sys, s + 10);
      else
        process->face_timer = AIParamToFloat(process, args[i]);
    }
  }
  if (packet != 0 && process->action_data_3 != 0)
    packet->look_target = &((GameObject_s *)process->action_data_3)->position;
  if (process->face_timer > 0.0f) {
    process->face_timer -= time;
    if (process->face_timer <= 0.0f) {
      process->face_timer = 0.0f;
      return 1;
    }
  }
  return 0;
}

extern GameObject_s *player;
extern GameObject_s *player2;

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
        obj = GetNamedGameObject(sys, s + 10);
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
        obj = GetNamedGameObject(sys, s + 10);
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
        obj = GetNamedGameObject(sys, s + 10);
      } else {
        s = NuStrIStr(args[i], "tag_to=");
        if (s != 0)
          target = GetNamedGameObject(sys, s + 7);
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
          obj = GetNamedGameObject(sys, s);
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

// GLOBAL: LEGOBATMAN 0x00ad6918
extern i32 (*AIActionParseSpeedFn)(char *str, u8 *out);

void AIMoveInstruction(AIPACKET_s *packet, nuvec_s *pos, f32 height,
                       AIPATHINFO *path_info, i32 type, f32 param);

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
        *(AILOCATORSET_s **)((u8 *)process + 0xac) =
            AIPathFindLocatorSet(g_unk00960894->aiSys2bf8, s + 5);
      } else if (NuStrICmp(args[i], "from_current_loc") == 0) {
        AILOCATOR_s *locator = *(AILOCATOR_s **)((u8 *)process + 0xa8);
        if (locator != 0) {
          AssignLocatorAISys_s *ai = (AssignLocatorAISys_s *)sys;
          i32 index = locator - ai->locators;
          for (i32 j = 0; j < ai->locator_set_count; j++) {
            AILOCATORSET_s *set = &ai->locator_sets[j];
            for (i32 k = 0; k < set->locator_count; k++) {
              if (index == set->locator_entries[k])
                *(AILOCATORSET_s **)((u8 *)process + 0xac) = set;
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
    *(AILOCATORSET_s **)((u8 *)process + 0xac) = set;
    if (locator != 0) {
      *(AILOCATOR_s **)((u8 *)process + 0xa8) = locator;
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
      obj = GetNamedGameObject(sys, s + 10);
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
        body = GetNamedGameObject(sys, s);
      } else {
        s = NuStrIStr(args[i], "trailer");
        if (s != 0) {
          s = s + NuStrLen("trailer") + 1;
          trailer = GetNamedGameObject(sys, s);
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
        obj = GetNamedGameObject(sys, s + 10);
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
        obj = GetNamedGameObject(sys, s + 10);
      else if ((s = NuStrIStr(args[i], "tag_to=")) != 0)
        tag_to = GetNamedGameObject(sys, s + 7);
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
        obj = GetNamedGameObject(sys, s + 10);
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
        process->action_data_3 = GetNamedGameObject(sys, s + 10);
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
      obj = GetNamedGameObject(sys, s + 10);
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
          obj = GetNamedGameObject(sys, s + 10);
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
      AIMoveInstruction(packet, &node->pos, 0.0f,
                        (AIPATHINFO *)((u8 *)process + 0x84), 1,
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
        obj = GetNamedGameObject(sys, s + 10);
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
        obj = GetNamedGameObject(sys, s + 10);
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
      obj = GetNamedGameObject(sys, s + 10);
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
            obj = GetNamedGameObject(sys, s + 10);
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
        obj = GetNamedGameObject(sys, s + 10);
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
        process->face_timer = AIParamToFloat(process, args[i]);
    }
    if (process->face_timer == 0.0f && max_time > min_time)
      process->face_timer = NuRandFloat() * (max_time - min_time) + min_time;
  }
  numtx_s *cam = NuCameraGetMtx();
  if (cam != 0) {
    packet->look_target = (nuvec_s *)&cam->m30;
    if (process->hold_special_button != 0)
      SetHeadTarget(obj, obj->look_target, 7, 1.0f, 0.0f, 0.0f);
  }
  if (process->face_timer > 0.0f) {
    process->face_timer -= time;
    if (process->face_timer <= 0.0f) {
      process->face_timer = 0.0f;
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
        obj = GetNamedGameObject(sys, s + 10);
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
        obj = GetNamedGameObject(sys, s + 10);
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
        process->face_timer = AIParamToFloatEx(packet, process, s + 5);
      else if ((s = NuStrIStr(args[i], "rot_rate=")) != 0)
        process->action_data_4 =
            (f32)(i32)(AIParamToFloatEx(packet, process, s + 9) *
                       182.04444885253906);
    }
    if (min_time != max_time)
      process->face_timer = NuRandFloat() * (max_time - min_time) + min_time;
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
  if (process->face_timer > 0.0f) {
    process->face_timer -= time;
    if (process->face_timer <= 0.0f) {
      process->face_timer = 0.0f;
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
          obj = GetNamedGameObject(sys, s + 10);
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
