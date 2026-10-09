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
