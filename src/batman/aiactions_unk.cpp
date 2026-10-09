// batman/, file unknown: AI script Action_* handlers
// (0x004556b0..0x004717b0).

#include "../gameapi/ai/aisys_unk.h"
#include "../nu2api/nucore/nustring.h"
#include "worldinfo_unk.h"

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

struct AssignLocatorAISys_s {
  u8 pad0[0x234];
  AILOCATOR_s *locators; // 0x234
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
  u8 pad4[4];
  f32 field_08; // 0x08
  u8 padc[0x3c - 0xc];
  nuvec_s pickup_positions[1]; // 0x3c
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
