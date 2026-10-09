// batman/, file unknown: AI script Condition_* parsers
// (0x0044da30..0x00452120).

#include "../gameapi/ai/aisys_unk.h"
#include "../nu2api/nucore/nulist.h"
#include "../nu2api/nucore/nustring.h"
#include "worldinfo_unk.h"
#include <stddef.h>

// FUNCTION: LEGOBATMAN 0x00447370
i32 GameAIActionParseSpeed(char *str, u8 *out) {
  if (!NuStrICmp(str, "RUN")) {
    *out = 0;
    return 1;
  }
  if (!NuStrICmp(str, "WALK")) {
    *out = 1;
    return 1;
  }
  if (!NuStrICmp(str, "TIPTOE")) {
    *out = 2;
    return 1;
  }
  return 0;
}

struct GetNamedAICreature_s {
  char name[0xa8];
};

struct GetNamedAISys_s {
  u8 pad0[0x224];
  GetNamedAICreature_s *creatures; // 0x224
};

// GLOBAL: LEGOBATMAN 0x00ab3980
extern GameObject_s *player;
// GLOBAL: LEGOBATMAN 0x00ab3960
extern GameObject_s *Player[8];

int sprintf(char *buf, const char *fmt, ...);

// from saga gameapi/ai/aisys/aisys.cpp
// STUB: LEGOBATMAN 0x00447560
// logic right; orig funnels every found path into one /GS epilogue (with a
// degenerate "test eax; jne +0"), ours duplicates epilogues; params land in
// swapped registers.
GameObject_s *GetNamedAPIObject(AISYS_s *sys, char *name) {
  GetNamedAISys_s *system = (GetNamedAISys_s *)sys;
  if (system != NULL) {
    for (i32 index = 0; index < HIGHGAMEOBJECT; ++index) {
      if ((Obj[index].flags1fc & 1) == 0)
        continue;

      if ((Obj[index].flags1f8 & 0x400) != 0 && Obj[index].b3c8 != 0xff) {
        GetNamedAICreature_s *creature = &system->creatures[Obj[index].b3c8];
        if (creature != NULL && NuStrICmp(creature->name, name) == 0) {
          GameObject_s *result = &Obj[index];
          if (result == NULL)
            break;
          return result;
        }
      }
      if (NuStrICmp(Obj[index].p54->file, name) == 0) {
        GameObject_s *result = &Obj[index];
        if (result == NULL)
          break;
        return result;
      }
    }
  }

  GameObject_s *obj;
  if (NuStrICmp(name, "player") == 0) {
    obj = player;
  } else if (NuStrICmp(name, "player_2") == 0) {
    obj = Player[0] == player ? Player[1] : Player[0];
  } else {
    for (i32 index = 0; index < 8; ++index) {
      if (Player[index] == NULL)
        continue;
      char player_name[64];
      sprintf(player_name, "Player%d", index);
      if (NuStrICmp(player_name, name) == 0) {
        obj = Player[index];
        goto found;
      }
    }
    return NULL;
  }
found:
  return obj != NULL ? (GameObject_s *)obj->pad0 : NULL;
}

struct LEVELSCRIPTPROCESS_s {
  char name[0x10];           // 0x00
  u8 processor[0xdc - 0x10]; // 0x10, AISCRIPTPROCESS
};

void AISysSetLevelPath(AISYS_s *sys, void *path);
AISCRIPT *AIScriptFind(AISYS_s *sys, char *name, i32 can_use_default, i32 a,
                       i32 b);
void AIScriptProcessorInit(AISYS_s *sys, void *ai, void *process, void *a,
                           char *name, void *c, i32 d, void *script,
                           void *state);
void LevelScriptReStoreProgress(WORLDINFO_s *world,
                                LEVELSCRIPTPROCESS_s *process);
void AISysResetPathCnxs(AISYS_s *sys);
void AIPathCnxControlSysReset(void *sys);
void AIPathCnxHelperSysReset(WORLDINFO_s *world, void *sys);
void InitAICreatures(AISYS_s *sys);
void ResetAICreatures(AISYS_s *sys);
void GizmoSysUnk005bd480(GIZMOSYS_s *gizmo_sys, void *p, WORLDINFO_s *world);

// GLOBAL: LEGOBATMAN 0x0093b104
extern i32 g_unk0093b104;
// GLOBAL: LEGOBATMAN 0x0093b108
extern i32 g_unk0093b108;

// from saga legoapi/items/objects/gameobjects.cpp
// FUNCTION: LEGOBATMAN 0x0044bf50
void GameAISysReset(AISYS_s *system) {
  if (system == NULL)
    return;

  AISysSetLevelPath(system, NULL);

  g_unk00960894->processor_count = 0;
  for (i32 script_index = 0; script_index < 32; ++script_index) {
    char script_name[32];
    if (script_index != 0)
      sprintf(script_name, "Level%d", script_index);
    else
      sprintf(script_name, "Level");

    if (AIScriptFind(g_unk00960894->aiSys2bf8, script_name, 0, 1, 0) == NULL)
      continue;

    AIScriptProcessorInit(system, NULL,
                          ((LEVELSCRIPTPROCESS_s *)g_unk00960894
                               ->processors)[g_unk00960894->processor_count]
                              .processor,
                          NULL, script_name, NULL, 0, NULL, NULL);
    NuStrCpy(((LEVELSCRIPTPROCESS_s *)
                  g_unk00960894->processors)[g_unk00960894->processor_count]
                 .name,
             script_name);
    LevelScriptReStoreProgress(
        g_unk00960894, &((LEVELSCRIPTPROCESS_s *)g_unk00960894
                             ->processors)[g_unk00960894->processor_count]);
    ++g_unk00960894->processor_count;
  }

  AISysResetPathCnxs(system);
  AIPathCnxControlSysReset(g_unk00960894->ai_path_cnx_control_sys);
  AIPathCnxHelperSysReset(g_unk00960894, g_unk00960894->ai_path_cnx_helper_sys);
  InitAICreatures(system);
  ResetAICreatures(system);
  if (g_unk00960894->processor_count != 0)
    GizmoSysUnk005bd480(g_unk00960894->gizmoSys2b0c, g_unk00960894->p2b10,
                        g_unk00960894);
  g_unk0093b104 = g_unk0093b108 = -1;
}

f32 NuVecDistSqr(nuvec_s *v0, nuvec_s *v1, nuvec_s *d);
f32 NuFsqrt(f32 f);
void NuVecScale(nuvec_s *v, nuvec_s *v0, f32 k);
i32 LineIntersectSphere(nuvec_s *origin, nuvec_s *direction, nuvec_s *center,
                        f32 radius_squared, f32 *distance_squared);

// STUB: LEGOBATMAN 0x0044c960
// close: opponent load is hoisted above the flags early-out in orig, and the
// target-distance/length locals land in swapped stack slots
i32 PartyMemberInWay(GameObject_s *object, GameObject_s *opponent) {
  nuvec_s difference, direction;
  f32 length;
  f32 target_distance;
  if (object->flags1f8 & 5)
    return 0;
  target_distance = NuVecDistSqr(&opponent->v80, &object->v80, &difference);
  length = NuFsqrt(target_distance);
  NuVecScale(&direction, &difference, length != 0.0f ? 1.0f / length : 0.0f);
  for (i32 index = 0; index < 8; index++) {
    GameObject_s *member = Player[index];
    if (member == 0 || !(member->flags1fc & 1) ||
        !(member->flags1fc & 0x1000) || member == object || member == opponent)
      continue;
    if (NuVecDistSqr(&object->v80, &member->v80, &difference) <
        target_distance) {
      member = Player[index];
      f32 radius = 0.125f + member->radius;
      if (LineIntersectSphere(&object->v80, &direction, &member->v80,
                              radius * radius, 0))
        return 1;
    }
  }
  return 0;
}

// STUB: LEGOBATMAN 0x0044da30
// original tests str/sys before zeroing the result register; this form hoists
// the zero
i32 Condition_BeenHitByInit(AISYS_s *sys, char *str, AISCRIPT_s *script) {
  i32 result = 0;
  if (!str || !sys)
    return 0;
  if (!NuStrICmp(str, "PLAYER"))
    result = 4;
  else if (!NuStrICmp(str, "AI"))
    result = 2;
  else if (!NuStrICmp(str, "BADDY"))
    result = 0x10;
  else if (!NuStrICmp(str, "GOODY"))
    result = 8;
  else if (!NuStrICmp(str, "NEUTRAL"))
    result = 0x20;
  return result;
}

// AISYS_s is opaque here; only player_1 is evidenced.
struct AISysPlayer1_s {
  u8 pad0[0x1698];
  GameObject_s *player_1; // 0x1698
  GameObject_s *player_2; // 0x169c
};

i32 ObjHitObj_Flags(GameObject_s *object);

// FUNCTION: LEGOBATMAN 0x0044db10
f32 Condition_BeenHitBy(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                        AIPACKET_s *packet, char *str, void *data) {
  GameObject_s *obj = packet->pd0->obj;
  if (obj != NULL && (obj->flags140c & 1) && obj->last_attacker != NULL &&
      ((i32)data & ObjHitObj_Flags(obj->last_attacker)) != 0)
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044db60
void *Condition_BeenHitByNamedObjInit(AISYS_s *sys, char *name,
                                      AISCRIPT_s *script) {
  return name != NULL && GetNamedAPIObjectFn != NULL
             ? GetNamedAPIObjectFn(sys, name)
             : NULL;
}

// FUNCTION: LEGOBATMAN 0x0044db90
f32 Condition_BeenHitByNamedObj(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                AIPACKET_s *packet, char *str, void *data) {
  GameObject_s *obj = packet->pd0->obj;
  if (obj != NULL && data != NULL && (obj->flags140c & 1) &&
      obj->last_attacker != NULL && obj->last_attacker == data)
    return 1.0f;
  return 0.0f;
}

// The packet's owner is the object itself (GameObject_s view of pd0).
#define OWNER(packet) ((GameObject_s *)(packet)->pd0)

// FUNCTION: LEGOBATMAN 0x0044dbe0
f32 Condition_IAmAGoody(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                        AIPACKET_s *packet, char *str, void *data) {
  if (packet == NULL || packet->pd0 == NULL)
    return 0.0f;
  if (OWNER(packet)->flags1f8 & 5)
    return 0.0f;
  return 1.0f;
}

// FUNCTION: LEGOBATMAN 0x0044dc10
f32 Condition_IAmABaddy(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                        AIPACKET_s *packet, char *str, void *data) {
  if (packet != NULL && packet->pd0 != NULL &&
      (OWNER(packet)->flags1f8 & 0x10001) != 0)
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044dc40
f32 Condition_IAmANeutral(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                          AIPACKET_s *packet, char *str, void *data) {
  if (packet != NULL && packet->pd0 != NULL &&
      (OWNER(packet)->flags1f8 & 4) != 0)
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044dc70
f32 Condition_IAmAGoodieBaddie(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                               AIPACKET_s *packet, char *str, void *data) {
  if (packet != NULL && packet->pd0 != NULL &&
      (OWNER(packet)->flags1f8 & 0x10000) != 0)
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044dca0
void *Condition_EitherPlayerIsInit(AISYS_s *sys, char *arg,
                                   AISCRIPT_s *script) {
  return arg != NULL && GetNamedAPIObjectFn != NULL
             ? GetNamedAPIObjectFn(sys, arg)
             : NULL;
}

// FUNCTION: LEGOBATMAN 0x0044dcd0
f32 Condition_EitherPlayerIs(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                             AIPACKET_s *packet, char *str, void *argument) {
  if (sys != NULL && argument != NULL &&
      (((AISysPlayer1_s *)sys)->player_1 == argument ||
       ((AISysPlayer1_s *)sys)->player_2 == argument))
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044dd00
f32 Condition_Player1Is(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                        AIPACKET_s *packet, char *str, void *argument) {
  if (sys != NULL && argument != NULL &&
      ((AISysPlayer1_s *)sys)->player_1 == argument)
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044dd30
f32 Condition_Player2Is(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                        AIPACKET_s *packet, char *str, void *argument) {
  if (sys != NULL && argument != NULL &&
      ((AISysPlayer1_s *)sys)->player_2 == argument)
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044dd60
f32 Condition_IAmPlayer2(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                         AIPACKET_s *packet, char *str, void *data) {
  if (packet != NULL && packet->pd0 != NULL) {
    if (player == Player[0]) {
      if (packet->pd0->obj == Player[1])
        return 1.0f;
    } else if (player == Player[1]) {
      if (packet->pd0->obj == Player[0])
        return 1.0f;
    }
  }
  return 0.0f;
}

// GLOBAL: LEGOBATMAN 0x0093e144
extern i32 force_gizmotype_id;
// GLOBAL: LEGOBATMAN 0x00ab3984
extern GameObject_s *player2;

struct GIZFORCE_s {
  u8 pad0[0xa0];
  u32 runtime_flags; // 0xa0
};

i32 GizForce_GameObjUsingForce(GameObject_s *object, GIZFORCE_s *force);

// FUNCTION: LEGOBATMAN 0x0044ddc0
void *Condition_UsingForceInit(AISYS_s *sys, char *name, AISCRIPT_s *script) {
  GIZMO_s *gizmo =
      GizmoFindByName(g_unk00960894->gizmoSys2b0c, force_gizmotype_id, name);
  return gizmo != NULL ? *(void **)gizmo : NULL;
}

// FUNCTION: LEGOBATMAN 0x0044de00
f32 Condition_UsingForce(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                         AIPACKET_s *packet, char *str, void *argument) {
  if (GizForce_GameObjUsingForce(packet->pd0->obj, (GIZFORCE_s *)argument) != 0)
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044de30
f32 Condition_PlayerUsingForce(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                               AIPACKET_s *packet, char *str, void *argument) {
  if (GizForce_GameObjUsingForce(player, (GIZFORCE_s *)argument) != 0)
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044de60
f32 Condition_EitherPlayerUsingForce(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                     AIPACKET_s *packet, char *str,
                                     void *argument) {
  GIZFORCE_s *force = (GIZFORCE_s *)argument;
  if (GizForce_GameObjUsingForce(player, force) != 0)
    return 1.0f;
  if (GizForce_GameObjUsingForce(player2, force) != 0)
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044deb0
void *Condition_OnForcePlatformInit(AISYS_s *sys, char *name,
                                    AISCRIPT_s *script) {
  GIZMO_s *gizmo =
      GizmoFindByName(g_unk00960894->gizmoSys2b0c, force_gizmotype_id, name);
  if (gizmo != NULL) {
    GIZFORCE_s *force = *(GIZFORCE_s **)gizmo;
    if (force != NULL && (force->runtime_flags & 0x100) != 0)
      return force;
  }
  return NULL;
}

// FUNCTION: LEGOBATMAN 0x0044e190
f32 Condition_ForceBeingUsed(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                             AIPACKET_s *packet, char *str, void *argument) {
  GIZFORCE_s *force = (GIZFORCE_s *)argument;
  // 0x3c: current user.
  if (force != NULL && ((force->runtime_flags & 0x200000) != 0 ||
                        ((void **)force)[0x3c / 4] != 0))
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044e1c0
f32 Condition_PlayerDeflectingPart(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                   AIPACKET_s *packet, char *str, void *data) {
  // 0x1140: the part being deflected.
  if (player != NULL && ((void **)player)[0x1140 / 4] != NULL)
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044f390
f32 Condition_Colliding(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                        AIPACKET_s *packet, char *str, void *data) {
  // 0x1f0: 64-bit mask of objects being collided with.
  if (packet != NULL && packet->pd0 != NULL &&
      *(unsigned __int64 *)((char *)packet->pd0 + 0x1f0) != 0)
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044f400
f32 Condition_XPos(AISYS_s *sys, AISCRIPTPROCESS_s *process, AIPACKET_s *packet,
                   char *str, void *argument) {
  GameObject_s *object = (GameObject_s *)argument;
  if (object == NULL)
    object = packet != NULL && packet->pd0 != NULL ? packet->pd0->obj : NULL;
  if (object != NULL)
    return object->v80.x;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044f440
f32 Condition_YPos(AISYS_s *sys, AISCRIPTPROCESS_s *process, AIPACKET_s *packet,
                   char *str, void *argument) {
  GameObject_s *object = (GameObject_s *)argument;
  if (object == NULL)
    object = packet != NULL && packet->pd0 != NULL ? packet->pd0->obj : NULL;
  if (object != NULL)
    return object->v80.y;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044f480
f32 Condition_ZPos(AISYS_s *sys, AISCRIPTPROCESS_s *process, AIPACKET_s *packet,
                   char *str, void *argument) {
  GameObject_s *object = (GameObject_s *)argument;
  if (object == NULL)
    object = packet != NULL && packet->pd0 != NULL ? packet->pd0->obj : NULL;
  if (object != NULL)
    return object->v80.z;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044f4c0
f32 Condition_PlayerXPos(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                         AIPACKET_s *packet, char *str, void *data) {
  if (player != NULL)
    return player->v80.x;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044f4e0
f32 Condition_PlayerYPos(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                         AIPACKET_s *packet, char *str, void *data) {
  if (player != NULL)
    return player->v80.y;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044f500
f32 Condition_PlayerZPos(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                         AIPACKET_s *packet, char *str, void *data) {
  if (player != NULL)
    return player->v80.z;
  return 0.0f;
}

int NuAToI(const char *s);

// FUNCTION: LEGOBATMAN 0x0044f520
void *Condition_IsSetAliveInit(AISYS_s *sys, char *arg, AISCRIPT_s *script) {
  if (arg != NULL) {
    if (NuStrICmp(arg, "myset") == 0)
      return (void *)-1;
    i32 set = NuAToI(arg);
    if (set >= 1 && set <= 16)
      return (void *)set;
  }
  return NULL;
}

// GLOBAL: LEGOBATMAN 0x009c5b58
extern u8 aicreature_sets_alive[16];

// FUNCTION: LEGOBATMAN 0x0044f570
f32 Condition_IsSetAlive(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                         AIPACKET_s *packet, char *str, void *argument) {
  i32 set = (i32)argument;
  if (set == -1)
    set = ((u8 *)process)[0xb4];
  if (set != 0 && aicreature_sets_alive[set - 1] != 0)
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044f5a0
f32 Condition_NumInSetAlive(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                            AIPACKET_s *packet, char *str, void *argument) {
  i32 set = (i32)argument;
  if (set == -1)
    set = ((u8 *)process)[0xb4];
  if (set != 0)
    return aicreature_sets_alive[set - 1];
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044f5e0
f32 Condition_Context(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                      AIPACKET_s *packet, char *str, void *data) {
  if (packet != NULL && packet->pd0 != NULL && packet->pd0->obj != NULL)
    return packet->pd0->obj->b9db;
  return -1.0f;
}

// FUNCTION: LEGOBATMAN 0x0044f8c0
f32 Condition_InContext(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                        AIPACKET_s *packet, char *str, void *argument) {
  if (packet != NULL && packet->pd0 != NULL && packet->pd0->obj != NULL &&
      packet->pd0->obj->b9db == (i32)argument)
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044f900
f32 Condition_OpponentContext(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                              AIPACKET_s *packet, char *str, void *argument) {
  if (packet != NULL && packet->pe4 != NULL && packet->pe4->obj != NULL &&
      packet->pe4->obj->b9db == (i32)argument)
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044f940
f32 Condition_EitherPlayerInContext(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                    AIPACKET_s *packet, char *str,
                                    void *argument) {
  if (player != NULL && player->b9db == (i32)argument)
    return 1.0f;
  if (player2 != NULL && player2->b9db == (i32)argument)
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044f9e0
f32 Condition_InLayer(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                      AIPACKET_s *packet, char *str, void *argument) {
  if (packet != NULL && packet->pd0 != NULL &&
      (i8)OWNER(packet)->b24f == (i32)argument)
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044fa10
f32 Condition_OpponentInLayer(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                              AIPACKET_s *packet, char *str, void *argument) {
  if (packet != NULL && packet->pe4 != NULL &&
      (i8)((GameObject_s *)packet->pe4)->b24f == (i32)argument)
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044fa80
f32 Condition_OnDynamicGrapple(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                               AIPACKET_s *packet, char *str, void *argument) {
  GameObject_s *object = (GameObject_s *)argument;
  if (object == NULL) {
    if (packet == NULL || packet->pd0 == NULL ||
        (object = packet->pd0->obj) == NULL)
      return 0.0f;
  }
  if (object->b9db == 0x46 || object->b9db == 0x6b || object->b9db == 0x6c) {
    u8 *grapple = (u8 *)object->techno;
    if (grapple != NULL && grapple[0x1c] != 0)
      return 1.0f;
  }
  return 0.0f;
}

// GLOBAL: LEGOBATMAN 0x0095fc30
extern i32 g_unk0095fc30;
// GLOBAL: LEGOBATMAN 0x00961394
extern i32 g_unk00961394;
// GLOBAL: LEGOBATMAN 0x0096048c
extern i32 g_unk0096048c;

// FUNCTION: LEGOBATMAN 0x0044fae0
void *Condition_OnGrappleInit(AISYS_s *sys, char *name, AISCRIPT_s *script) {
  if (name != NULL && sys != NULL) {
    GIZMO_s *gizmo =
        GizmoFindByName(g_unk00960894->gizmoSys2b0c, g_unk0095fc30, name);
    if (gizmo != NULL)
      return *(void **)gizmo;
  }
  return NULL;
}

// FUNCTION: LEGOBATMAN 0x0044fb30
f32 Condition_OnGrapple(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                        AIPACKET_s *packet, char *str, void *argument) {
  if (packet != NULL && packet->pd0 != NULL) {
    GameObject_s *object = packet->pd0->obj;
    if (object->b9db == 0x46 || object->b9db == 0x6b || object->b9db == 0x6c) {
      void *grapple = object->techno;
      if (grapple != NULL && (argument == NULL || argument == grapple))
        return 1.0f;
    }
  }
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044fb90
void *Condition_EitherPlayerOnLedgeInit(AISYS_s *sys, char *name,
                                        AISCRIPT_s *script) {
  if (name != NULL && sys != NULL) {
    GIZMO_s *gizmo =
        GizmoFindByName(g_unk00960894->gizmoSys2b0c, g_unk00961394, name);
    if (gizmo != NULL)
      return *(void **)gizmo;
  }
  return NULL;
}

// FUNCTION: LEGOBATMAN 0x0044fbe0
f32 Condition_EitherPlayerOnLedge(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                  AIPACKET_s *packet, char *str,
                                  void *argument) {
  if (packet != NULL && packet->pd0 != NULL) {
    for (i32 i = 0; i < 8; i++) {
      GameObject_s *object = Player[i];
      if (object != NULL && (object->flags1fc & 0x1000) && object->b257 == 0 &&
          object->b9db == g_unk0096048c) {
        void *ledge = object->techno;
        if (ledge != NULL && (argument == NULL || argument == ledge))
          return 1.0f;
      }
    }
  }
  return 0.0f;
}
i32 GizmoGetOutput(GIZMOSYS_s *sys, GIZMO_s *gizmo, i32 output, i32 a);
void *AIPathFindLocatorSet(AISYS_s *sys, char *name);

// FUNCTION: LEGOBATMAN 0x0044d1d0
f32 Condition_GlynTest(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                       AIPACKET_s *packet, char *str, void *data) {
  return 1.0f;
}

// FUNCTION: LEGOBATMAN 0x0044d1e0
f32 Condition_Debug(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                    AIPACKET_s *packet, char *str, void *data) {
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044d1f0
f32 Condition_Active(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                     AIPACKET_s *packet, char *str, void *data) {
  // 0x13e: the packet's reset mode.
  if (packet != NULL && ((u8 *)packet)[0x13e] == 2)
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044d2d0
f32 Condition_PrefersBrawling(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                              AIPACKET_s *packet, char *str, void *data) {
  if (packet != NULL && packet->pd0 != NULL) {
    u32 flags = OWNER(packet)->p54->model_flags;
    if (flags & 0x80000000)
      return 1.0f;
    if (!(flags & 0x80))
      return 1.0f;
  }
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044d300
f32 Condition_LocatorExist(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                           AIPACKET_s *packet, char *str, void *data) {
  if (g_unk00960894->aiSys2bf8 != NULL && str != NULL &&
      AIPathFindLocator(g_unk00960894->aiSys2bf8, str) != NULL)
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044d340
f32 Condition_LocatorSetExist(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                              AIPACKET_s *packet, char *str, void *data) {
  if (g_unk00960894->aiSys2bf8 != NULL && str != NULL &&
      AIPathFindLocatorSet(g_unk00960894->aiSys2bf8, str) != NULL)
    return 1.0f;
  return 0.0f;
}

// GLOBAL: LEGOBATMAN 0x0095ff24
extern i32 obstacle_gizmotype_id;
// GLOBAL: LEGOBATMAN 0x00967aec
extern i32 gizspecial_gizmotype_id;

// FUNCTION: LEGOBATMAN 0x0044d380
void *Condition_ObstacleInit(AISYS_s *sys, char *name, AISCRIPT_s *script) {
  return GizmoFindByName(g_unk00960894->gizmoSys2b0c, obstacle_gizmotype_id,
                         name);
}

// FUNCTION: LEGOBATMAN 0x0044d3b0
void *Condition_GizSpecialInit(AISYS_s *sys, char *name, AISCRIPT_s *script) {
  return GizmoFindByName(g_unk00960894->gizmoSys2b0c, gizspecial_gizmotype_id,
                         name);
}

// FUNCTION: LEGOBATMAN 0x0044d3e0
f32 Condition_ObstacleAtStart(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                              AIPACKET_s *packet, char *str, void *argument) {
  if (argument != NULL && GizmoGetOutput(g_unk00960894->gizmoSys2b0c,
                                         (GIZMO_s *)argument, 1, 1) == 0)
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044d420
f32 Condition_ObstacleAtEnd(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                            AIPACKET_s *packet, char *str, void *argument) {
  if (argument != NULL && GizmoGetOutput(g_unk00960894->gizmoSys2b0c,
                                         (GIZMO_s *)argument, 0, 1) != 0)
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044d460
void *Condition_ObstaclePlayingInit(AISYS_s *sys, char *name,
                                    AISCRIPT_s *script) {
  GIZMO_s *gizmo =
      GizmoFindByName(g_unk00960894->gizmoSys2b0c, obstacle_gizmotype_id, name);
  return gizmo != NULL ? *(void **)gizmo : NULL;
}

struct GIZOBSTACLE_s {
  u8 pad0[0x34];
  u8 *anim;                        // 0x34, byte 0x0a bit 0: playing
  GameObject_s *triggering_object; // 0x38
  u8 pad3c[0xc8 - 0x3c];
  u32 runtime_flags; // 0xc8
};

// FUNCTION: LEGOBATMAN 0x0044d4a0
f32 Condition_ObstaclePlaying(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                              AIPACKET_s *packet, char *str, void *argument) {
  GIZOBSTACLE_s *obstacle = (GIZOBSTACLE_s *)argument;
  if (obstacle != NULL && (obstacle->runtime_flags & 1) &&
      (obstacle->runtime_flags & 2) && (obstacle->anim[0xa] & 1))
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044d4d0
f32 Condition_ObstacleLockedShut(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                 AIPACKET_s *packet, char *str,
                                 void *argument) {
  if ((((GIZOBSTACLE_s *)argument)->runtime_flags & 0x4000) != 0)
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044d4f0
f32 Condition_ObstacleLockedOpen(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                 AIPACKET_s *packet, char *str,
                                 void *argument) {
  if ((((GIZOBSTACLE_s *)argument)->runtime_flags & 0x2000) != 0)
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044d510
void *Condition_ObstacleOpenedByPlayerInit(AISYS_s *sys, char *name,
                                           AISCRIPT_s *script) {
  GIZMO_s *gizmo =
      GizmoFindByName(g_unk00960894->gizmoSys2b0c, obstacle_gizmotype_id, name);
  return gizmo != NULL ? *(void **)gizmo : NULL;
}

// FUNCTION: LEGOBATMAN 0x0044d550
f32 Condition_ObstacleOpenedByPlayer(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                     AIPACKET_s *packet, char *str,
                                     void *argument) {
  GIZOBSTACLE_s *obstacle = (GIZOBSTACLE_s *)argument;
  if (obstacle != NULL && player != NULL &&
      obstacle->triggering_object == player)
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044d580
f32 Condition_ObstacleOpenedByEitherPlayer(AISYS_s *sys,
                                           AISCRIPTPROCESS_s *process,
                                           AIPACKET_s *packet, char *str,
                                           void *argument) {
  GIZOBSTACLE_s *obstacle = (GIZOBSTACLE_s *)argument;
  if (obstacle != NULL) {
    if (player != NULL && obstacle->triggering_object == player)
      return 1.0f;
    if (player2 != NULL && obstacle->triggering_object == player2)
      return 1.0f;
  }
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044d5c0
void *Condition_ForceInit(AISYS_s *sys, char *name, AISCRIPT_s *script) {
  return GizmoFindByName(g_unk00960894->gizmoSys2b0c, force_gizmotype_id, name);
}

// FUNCTION: LEGOBATMAN 0x0044d5f0
f32 Condition_ForceAtStart(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                           AIPACKET_s *packet, char *str, void *argument) {
  if (argument != NULL && GizmoGetOutput(g_unk00960894->gizmoSys2b0c,
                                         (GIZMO_s *)argument, 1, 1) == 0)
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044d630
f32 Condition_ForceAtEnd(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                         AIPACKET_s *packet, char *str, void *argument) {
  if (argument != NULL && GizmoGetOutput(g_unk00960894->gizmoSys2b0c,
                                         (GIZMO_s *)argument, 0, 1) != 0)
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044d670
void *Condition_IsVisibleInit(AISYS_s *sys, char *name, AISCRIPT_s *script) {
  return name;
}

i32 NuSpecialGetVisibilityFn(nuhspecial_s *special);
i32 NuSpecialExistsFn(nuhspecial_s *special);

// FUNCTION: LEGOBATMAN 0x0044d680
f32 Condition_IsVisible(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                        AIPACKET_s *packet, char *str, void *argument) {
  nuhspecial_s special = {0};
  NuSpecialFind(g_unk00960894->scn140, &special, (char *)argument, 1);
  if (NuSpecialExistsFn(&special) != 0)
    return (f32)NuSpecialGetVisibilityFn(&special);
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044d700
f32 Condition_MySet(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                    AIPACKET_s *packet, char *str, void *data) {
  return (f32)((u8 *)process)[0xb4];
}

// FUNCTION: LEGOBATMAN 0x0044d720
void *Condition_ForcePushingInit(AISYS_s *system, char *name,
                                 AISCRIPT_s *script) {
  return name != NULL && GetNamedAPIObjectFn != NULL
             ? GetNamedAPIObjectFn(system, name)
             : NULL;
}

// FUNCTION: LEGOBATMAN 0x0044d750
f32 Condition_ForcePushing(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                           AIPACKET_s *packet, char *str, void *argument) {
  GameObject_s *object = (GameObject_s *)argument;
  if (object == NULL) {
    if (packet == NULL || packet->pd0 == NULL || packet->pd0->obj == NULL ||
        (object = packet->pd0->obj) == NULL)
      return 0.0f;
  }
  if (object->b9db == 0x1b)
    return 1.0f;
  return 0.0f;
}

// GLOBAL: LEGOBATMAN 0x00967a14
extern i32 turret_gizmotype_id;

// FUNCTION: LEGOBATMAN 0x0044d7a0
void *Condition_TurretAliveInit(AISYS_s *sys, char *name, AISCRIPT_s *script) {
  GIZMO_s *gizmo =
      GizmoFindByName(g_unk00960894->gizmoSys2b0c, turret_gizmotype_id, name);
  if (gizmo != NULL) {
    void *turret = *(void **)gizmo;
    if (turret != NULL)
      return turret;
  }
  return NULL;
}

struct GIZTURRET_s {
  u8 pad0[0x13a];
  u16 flags_lo : 4; // 0x13a
  u16 dead : 2;     // bits 4-5
  u16 flags_hi : 10;
};

// STUB: LEGOBATMAN 0x0044d7e0
// close: orig loads the u16 with movzx and tests al; ours tests the byte.
f32 Condition_TurretAlive(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                          AIPACKET_s *packet, char *str, void *argument) {
  // 0x13a: turret flags.
  GIZTURRET_s *turret = (GIZTURRET_s *)argument;
  if (turret != NULL && turret->dead == 0)
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044d800
f32 Condition_IAmAPartyCharacter(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                 AIPACKET_s *packet, char *str, void *data) {
  if (packet != NULL && packet->pd0 != NULL && OWNER(packet)->b24c != -1)
    return 1.0f;
  return 0.0f;
}

// GLOBAL: LEGOBATMAN 0x0096052c
extern i32 g_unk0096052c;
// GLOBAL: LEGOBATMAN 0x00960528
extern i32 g_unk00960528;

// FUNCTION: LEGOBATMAN 0x0044f620
i32 Condition_InContextInit(AISYS_s *sys, char *name, AISCRIPT_s *script) {
  if (NuStrICmp(name, "DEACTIVATED") == 0)
    return 0x17;
  if (NuStrICmp(name, "FORCEDBACK") == 0)
    return 0x22;
  if (NuStrICmp(name, "GRAB") == 0)
    return 0x38;
  if (NuStrICmp(name, "EAT") == 0)
    return 0x3f;
  if (NuStrICmp(name, "FORCEPUSHED") == 0)
    return 0x1c;
  if (NuStrICmp(name, "FORCEPUSH") == 0)
    return 0x1b;
  if (NuStrICmp(name, "GETIN") == 0)
    return 0x3c;
  if (NuStrICmp(name, "BALLOONING") == 0)
    return 0x5d;
  if (NuStrICmp(name, "STUNNED") == 0)
    return 0x5a;
  if (NuStrICmp(name, "FLOAT") == 0)
    return 0x4b;
  if (NuStrICmp(name, "GRAPPLE") == 0)
    return 0x46;
  if (NuStrICmp(name, "GRAPPLEBATARANG") == 0)
    return 0x6b;
  if (NuStrICmp(name, "GRAPPLECATCH") == 0)
    return 0x6c;
  if (NuStrICmp(name, "BIGJUMP") == 0)
    return 0x1f;
  if (NuStrICmp(name, "FLOAT") == 0)
    return 0x4b;
  if (NuStrICmp(name, "SHOOT") == 0)
    return 0xa;
  if (NuStrICmp(name, "LEDGE") == 0)
    return 0x5b;
  if (NuStrICmp(name, "ZAP") == 0)
    return 0x16;
  if (NuStrICmp(name, "ElectricZap") == 0)
    return 0x7b;
  if (NuStrICmp(name, "SpecialMove_Victim") == 0)
    return g_unk0096052c;
  if (NuStrICmp(name, "SpecialMove_Attacker") == 0)
    return g_unk00960528;
  return 0x7c;
}
// FUNCTION: LEGOBATMAN 0x0044f980
i32 Condition_InLayerInit(AISYS_s *sys, char *str, AISCRIPT_s *script) {
  if (!NuStrICmp(str, "WATER"))
    return 1;
  if (!NuStrICmp(str, "MUDKILL") || !NuStrICmp(str, "HAZARD"))
    return 6;
  return 0;
}

// STUB: LEGOBATMAN 0x00451980
// return-block layout differs (original orders the shared epilogues 2, 1, -1,
// 0)
i32 Condition_SideInit(AISYS_s *sys, char *str, AISCRIPT_s *script) {
  if (!str)
    return 0;
  if (!NuStrICmp(str, "baddy") || !NuStrICmp(str, "baddie"))
    return -1;
  if (!NuStrICmp(str, "goody") || !NuStrICmp(str, "goodie"))
    return 1;
  if (!NuStrICmp(str, "goodybaddy") || !NuStrICmp(str, "goodiebaddie"))
    return 2;
  return 0;
}

f32 NuVecDist(nuvec_s *v0, nuvec_s *v1, nuvec_s *d);

// FUNCTION: LEGOBATMAN 0x00451da0
f32 Condition_OpponentToPlayerRange(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                    AIPACKET_s *packet, char *str, void *data) {
  if (packet != NULL && packet->pd0 != NULL && packet->pe4 != NULL &&
      sys != NULL && ((AISysPlayer1_s *)sys)->player_1 != NULL) {
    nuvec_s difference;
    return NuVecDist(&((AISysPlayer1_s *)sys)->player_1->position,
                     &packet->pe4->pos5c, &difference);
  }
  return 1.0e9f;
}

// FUNCTION: LEGOBATMAN 0x00451e10
f32 Condition_OpponentPathPosRange(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                   AIPACKET_s *packet, char *str, void *data) {
  AISCRIPTPROCESS_s *ai;
  if (packet != NULL && packet->pd0 != NULL && packet->pe4 != NULL &&
      (ai = packet->pe4->ai) != NULL) {
    nuvec_s difference;
    return NuVecDist(&packet->pd0->pos5c, (nuvec_s *)((char *)ai + 0x174),
                     &difference);
  }
  return 1.0e9f;
}

static inline f32 NuFabs(f32 f) {
  u32 bits = *(u32 *)&f & 0x7fffffff;
  return *(f32 *)&bits;
}

// FUNCTION: LEGOBATMAN 0x00451e70
f32 Condition_OpponentRangeY(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                             AIPACKET_s *packet, char *str, void *data) {
  if (packet != NULL && packet->pe4 != NULL)
    return NuFabs(packet->pe4->pos5c.y - packet->pd0->pos5c.y);
  return 1.0e9f;
}

i32 GizmoGetOutput(GIZMOSYS_s *sys, GIZMO_s *gizmo, i32 output, i32 a);
i32 GizmoGetVisibility(GIZMOSYS_s *sys, GIZMO_s *gizmo);
void *FlowBoxFindByName(void *flow, char *name);

// FUNCTION: LEGOBATMAN 0x00451ec0
void *Condition_GizmoOutputInit(AISYS_s *sys, char *name, AISCRIPT_s *script) {
  return name != NULL ? GizmoFindByName(g_unk00960894->gizmoSys2b0c, -1, name)
                      : NULL;
}

// FUNCTION: LEGOBATMAN 0x00451f00
f32 Condition_GizmoOutput0(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                           AIPACKET_s *packet, char *str, void *argument) {
  if (argument != NULL)
    return (f32)(u32)GizmoGetOutput(g_unk00960894->gizmoSys2b0c,
                                    (GIZMO_s *)argument, 0, 1);
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x00451f50
f32 Condition_GizmoOutput1(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                           AIPACKET_s *packet, char *str, void *argument) {
  if (argument != NULL)
    return (f32)(u32)GizmoGetOutput(g_unk00960894->gizmoSys2b0c,
                                    (GIZMO_s *)argument, 1, 1);
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x00451fa0
f32 Condition_GizmoOutput2(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                           AIPACKET_s *packet, char *str, void *argument) {
  if (argument != NULL)
    return (f32)(u32)GizmoGetOutput(g_unk00960894->gizmoSys2b0c,
                                    (GIZMO_s *)argument, 2, 1);
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x00451ff0
f32 Condition_GizmoOutput3(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                           AIPACKET_s *packet, char *str, void *argument) {
  if (argument != NULL)
    return (f32)(u32)GizmoGetOutput(g_unk00960894->gizmoSys2b0c,
                                    (GIZMO_s *)argument, 3, 1);
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x00452040
void *Condition_GizmoVisibilityInit(AISYS_s *sys, char *name,
                                    AISCRIPT_s *script) {
  return name != NULL ? GizmoFindByName(g_unk00960894->gizmoSys2b0c, -1, name)
                      : NULL;
}

// FUNCTION: LEGOBATMAN 0x00452080
f32 Condition_GizmoVisibility(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                              AIPACKET_s *packet, char *str, void *argument) {
  if (argument != NULL)
    return (f32)GizmoGetVisibility(g_unk00960894->gizmoSys2b0c,
                                   (GIZMO_s *)argument);
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x004520c0
void *Condition_FlowBoxCompleteInit(AISYS_s *sys, char *name,
                                    AISCRIPT_s *script) {
  return name != NULL ? FlowBoxFindByName(g_unk00960894->p2b10, name) : NULL;
}

struct FLOWBOX_s {
  u8 pad0[0xa];
  u8 state_flags_low; // 0x0a
};

// FUNCTION: LEGOBATMAN 0x004520f0
f32 Condition_FlowBoxComplete(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                              AIPACKET_s *packet, char *str, void *argument) {
  FLOWBOX_s *box = (FLOWBOX_s *)argument;
  if (box != NULL)
    return (box->state_flags_low & 2) != 0 ? 1.0 : 0.0;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x00452120
i32 Condition_AngleAboutMyLocatorToPlayerInit(AISYS_s *sys, char *str,
                                              AISCRIPT_s *script) {
  if (str) {
    if (!NuStrICmp(str, "player1"))
      return 0;
    if (!NuStrICmp(str, "player2"))
      return 1;
    if (!NuStrICmp(str, "nearest"))
      return 2;
  }
  return -1;
}

// FUNCTION: LEGOBATMAN 0x00452340
f32 Condition_AnimSpeedMul(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                           AIPACKET_s *packet, char *str, void *data) {
  if (packet != NULL && packet->pd0 != NULL && packet->pd0->obj != NULL)
    return packet->pd0->obj->anim_speed_mul;
  return 1.0f;
}

void *GizmoPickup_FindByName(WORLDINFO_s *world, char *name);
i32 GizmoPickup_BeenTurnedOn(void *pickup);

// FUNCTION: LEGOBATMAN 0x00452370
void *Condition_PickupBeenTurnedOnInit(AISYS_s *sys, char *name,
                                       AISCRIPT_s *script) {
  return name != NULL ? GizmoPickup_FindByName(g_unk00960894, name) : NULL;
}

// FUNCTION: LEGOBATMAN 0x004523a0
f32 Condition_PickupBeenTurnedOn(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                 AIPACKET_s *packet, char *str,
                                 void *argument) {
  if (argument != NULL)
    return (f32)GizmoPickup_BeenTurnedOn(argument);
  return 0.0f;
}

// GLOBAL: LEGOBATMAN 0x009ccb00
extern i32 radios_playing;

// FUNCTION: LEGOBATMAN 0x004523d0
f32 Condition_CanHearRadio(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                           AIPACKET_s *packet, char *str, void *data) {
  if (radios_playing != 0)
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x004523f0
f32 Condition_BeingTowed(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                         AIPACKET_s *packet, char *str, void *data) {
  if (packet != NULL && (packet->pd0->obj->flags1414 & 0x1000) != 0)
    return 1.0f;
  return 0.0f;
}

// Only the fields the conditions read are evidenced.
struct GAMESAVE_s {
  u8 pad0[0xb];
  u8 music_enabled; // 0x0b
  u8 pad0c[0x77d0 - 0xc];
  struct {
    u8 pad0[0xb];
    u8 area_complete; // 0x0b
  } area_save[1];     // 0x77d0, 12 bytes each
};

// GLOBAL: LEGOBATMAN 0x009c59cc
extern GAMESAVE_s *g_unk009c59cc;

// FUNCTION: LEGOBATMAN 0x00452420
f32 Condition_MusicOn(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                      AIPACKET_s *packet, char *str, void *data) {
  if (g_unk009c59cc->music_enabled != 0)
    return 1.0f;
  return 0.0f;
}

void *CharIDFromName(char *name);
void *APICharacterLoaded(i32 id);

// FUNCTION: LEGOBATMAN 0x00452440
void *Condition_CharacterLoadedInit(AISYS_s *sys, char *argument,
                                    AISCRIPT_s *script) {
  return argument != NULL ? CharIDFromName(argument) : NULL;
}

// FUNCTION: LEGOBATMAN 0x00452460
f32 Condition_CharacterLoaded(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                              AIPACKET_s *packet, char *str, void *argument) {
  if ((i32)argument > -1 && APICharacterLoaded((i32)argument) != NULL)
    return 1.0f;
  return 0.0f;
}

void *Area_FindByName(char *name, void *unk);

// FUNCTION: LEGOBATMAN 0x00452530
void *Condition_AreaCompleteInit(AISYS_s *sys, char *name, AISCRIPT_s *script) {
  return name != NULL ? Area_FindByName(name, NULL) : NULL;
}

struct AREADATA_s {
  u8 pad0[0x60];
  i16 levels[0x12]; // 0x60
  u8 index;         // 0x84
  u8 level_count;   // 0x85
};

// FUNCTION: LEGOBATMAN 0x00452550
f32 Condition_AreaComplete(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                           AIPACKET_s *packet, char *str, void *argument) {
  AREADATA_s *area = (AREADATA_s *)argument;
  if (area != NULL && g_unk009c59cc->area_save[area->index].area_complete == 1)
    return 1.0f;
  return 0.0f;
}

// Fields of the AI process embedded at GameObject_s 0x290 (+0xe4 opponent,
// +0xe8 opponent metric) and the 0x480 flag word.
struct GameObjectAIView_s {
  u8 pad0[0x374];
  Unk_AIPacketObj *opponent; // 0x374
  f32 opponent_metric;       // 0x378
  u8 pad37c[0x480 - 0x37c];
  u32 flags480; // 0x480
};

// GLOBAL: LEGOBATMAN 0x00936468
extern i16 id_BAT;

// FUNCTION: LEGOBATMAN 0x00452590
f32 Condition_ShouldAttackOpponent(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                   AIPACKET_s *packet, char *str, void *data) {
  if (packet != NULL && packet->pd0 != NULL && packet->pd0->obj != NULL) {
    GameObjectAIView_s *object = (GameObjectAIView_s *)packet->pd0->obj;
    if (object->opponent != NULL && object->opponent->obj != NULL &&
        (object->flags480 & 0x800) != 0 && object->opponent_metric < 1.0f &&
        object->opponent->obj->type15b0 == id_BAT)
      return 1.0f;
  }
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x00452600
f32 Condition_InSwamp(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                      AIPACKET_s *packet, char *str, void *data) {
  if (packet != NULL && packet->pd0 != NULL && packet->pd0->obj != NULL &&
      packet->pd0->obj->b24f == 9)
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x00452730
f32 Condition_NetworkGameOnGoing(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                 AIPACKET_s *packet, char *str, void *data) {
  return 0;
}

char *NuStrIStr(char *str, const char *sub);
WORLDINFO_s *WorldInfo_CurrentlyActive(void);
void *GizmoBlowUpTypeFind005dbcb0(char *name, WORLDINFO_s *world);

// FUNCTION: LEGOBATMAN 0x00452740
void *Condition_EitherPlayerSuperCarryingInit(AISYS_s *sys, char *name,
                                              AISCRIPT_s *script) {
  char *type;
  if (name != NULL && (type = NuStrIStr(name, "type=")) != NULL)
    return GizmoBlowUpTypeFind005dbcb0(type + 5, WorldInfo_CurrentlyActive());
  return NULL;
}

void *SuperCarry_CarryingType(GameObject_s *object, void *type);
i32 SuperCarry_Carrying(GameObject_s *object);

// GLOBAL: LEGOBATMAN 0x00960500
extern i32 g_unk00960500;

// FUNCTION: LEGOBATMAN 0x00452790
f32 Condition_EitherPlayerSuperCarrying(AISYS_s *sys,
                                        AISCRIPTPROCESS_s *process,
                                        AIPACKET_s *packet, char *str,
                                        void *data) {
  for (i32 i = 0; i < 2; i++) {
    if (Player[i] != NULL && (Player[i]->flags1fc & 0x80) &&
        (data != NULL ? (i32)SuperCarry_CarryingType(Player[i], data)
                      : SuperCarry_Carrying(Player[i])) != 0 &&
        g_unk00960500 != -1 && Player[i]->b9d9 != g_unk00960500)
      return 1.0f;
  }
  return 0.0f;
}

// STUB: LEGOBATMAN 0x00452820
// close: orig keeps a 0.0 result on the x87 stack across the checks and
// fcom-s it against 0x98c; ours spills or reloads.
f32 Condition_IsContextAnimationFinished(AISYS_s *sys,
                                         AISCRIPTPROCESS_s *process,
                                         AIPACKET_s *packet, char *str,
                                         void *data) {
  if (packet != NULL && packet->pd0 != NULL && packet->pd0->obj != NULL)
    return packet->pd0->obj->f98c <= 0.0f;
  return 0.0f;
}

f32 *ContextAnimFrame0059b3c0(void *anim, i32 context, i32 a, i32 b);

// FUNCTION: LEGOBATMAN 0x00452860
f32 Condition_ContextAnimationFrame(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                    AIPACKET_s *packet, char *str, void *data) {
  if (packet != NULL && packet->pd0 != NULL && packet->pd0->obj != NULL) {
    GameObject_s *object = packet->pd0->obj;
    f32 *frame =
        ContextAnimFrame0059b3c0((char *)object + 8, object->s9d0, 1, 0);
    if (frame != NULL)
      return *frame;
  }
  return 0.0f;
}

i32 RideObject_On00663df0(GameObject_s *object);

// FUNCTION: LEGOBATMAN 0x004528b0
f32 Condition_OnRideObject(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                           AIPACKET_s *packet, char *str, void *data) {
  if (packet != NULL && packet->pd0 != NULL && packet->pd0->obj != NULL &&
      RideObject_On00663df0(packet->pd0->obj) != 0)
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x004528f0
f32 Condition_AnyPartyOnRideObject(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                   AIPACKET_s *packet, char *str, void *data) {
  for (i32 i = 0; i < 8; i++) {
    if (Player[i] != NULL && RideObject_On00663df0(Player[i]) != 0)
      return 1.0f;
  }
  return 0.0f;
}

// GLOBAL: LEGOBATMAN 0x009ccafd
extern u8 g_unk009ccafd;

// FUNCTION: LEGOBATMAN 0x00452930
f32 Condition_NumberOfActiveSwords(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                   AIPACKET_s *packet, char *str, void *data) {
  return g_unk009ccafd;
}

i16 PlayerItemType_FindIndex005ee0f0(char *name);
void *PlayerItemType_Get005ee1a0(i32 index);

// FUNCTION: LEGOBATMAN 0x00452950
void *Condition_IsCarryingPlayerItemInit(AISYS_s *sys, char *name,
                                         AISCRIPT_s *script) {
  if (name != NULL) {
    i32 index = PlayerItemType_FindIndex005ee0f0(name);
    if (index != -1)
      return PlayerItemType_Get005ee1a0(index);
  }
  return NULL;
}

// STUB: LEGOBATMAN 0x00452980
// close: register choice only (obj in eax, lea esi vs mov esi; add).
f32 Condition_IsCarryingPlayerItem(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                   AIPACKET_s *packet, char *str, void *data) {
  if (packet != NULL && packet->pd0 != NULL && packet->pd0->obj != NULL) {
    // 0xb18: list of carried player items, item type at node + 8.
    NULISTHDR *items;
    NULISTLNK *node =
        NuListGetHead(items = (NULISTHDR *)((char *)packet->pd0->obj + 0xb18));
    while (node != NULL) {
      if (data == NULL || data == ((void **)node)[2])
        return 1.0f;
      node = NuListGetNext(items, node);
    }
  }
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x004529f0
f32 Condition_IAmWoozy(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                       AIPACKET_s *packet, char *str, void *data) {
  if (packet != NULL && packet->pd0 != NULL && packet->pd0->obj != NULL &&
      (packet->pd0->obj->flags1418 & 0x20000) != 0)
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x00452a30
f32 Condition_ImAGirl(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                      AIPACKET_s *packet, char *str, void *data) {
  if (packet != NULL && packet->pd0 != NULL && packet->pd0->obj != NULL)
    return (f32)((packet->pd0->obj->flags1414 >> 14) & 1);
  return 0.0f;
}

struct AIPATHNODE_s {
  u8 pad0[0x10];
  u8 index; // 0x10
  u8 pad11[0x5c - 0x11];
};

struct AIPATHSET_s {
  u8 pad0[0x7c];
  AIPATHNODE_s *nodes; // 0x7c
  u8 pad80[4];
  i8 **reachable; // 0x84, [from][to] != -1 when a route exists
};

struct AIPATHFIND_s {
  u8 pad0[8];
  AIPATHSET_s *set; // 0x08
};

// AISYS_s is opaque here; only the path finder is evidenced.
struct AISysPathFind_s {
  u8 pad0[0x21c];
  AIPATHFIND_s *pathfind; // 0x21c
};

AIPATHNODE_s *AIPathFindNode(AISYS_s *sys, AIPATHSET_s *set, char *name);

// FUNCTION: LEGOBATMAN 0x00452a80
void *Condition_CanGetToNodeInit(AISYS_s *sys, char *name, AISCRIPT_s *script) {
  AIPATHNODE_s *node;
  if (sys != NULL && ((AISysPathFind_s *)sys)->pathfind != NULL &&
      ((AISysPathFind_s *)sys)->pathfind->set != NULL &&
      (node = AIPathFindNode(sys, ((AISysPathFind_s *)sys)->pathfind->set,
                             name)) != NULL)
    return (void *)(node - ((AISysPathFind_s *)sys)->pathfind->set->nodes);
  return (void *)-1;
}

// FUNCTION: LEGOBATMAN 0x00452af0
f32 Condition_CanGetToNode(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                           AIPACKET_s *packet, char *str, void *data) {
  i32 target = (i32)data;
  if (target >= 0 && packet != NULL && packet->path_node != NULL &&
      packet->path_set == ((AISysPathFind_s *)sys)->pathfind->set) {
    i32 from = packet->path_node->index;
    if (target == from)
      return 1.0f;
    if (packet->path_set->reachable[from][target] != -1)
      return 1.0f;
  }
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x00452b60
f32 Condition_PlayerCanGetToNode(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                 AIPACKET_s *packet, char *str, void *data) {
  // The object's AI packet sits at 0x290.
#define PLAYER_PACKET ((AIPACKET_s *)((char *)player + 0x290))
  i32 target = (i32)data;
  if (target >= 0 && player != NULL && PLAYER_PACKET->path_node != NULL &&
      PLAYER_PACKET->path_set == ((AISysPathFind_s *)sys)->pathfind->set) {
    if (target == PLAYER_PACKET->path_node->index)
      return 1.0f;
    if (PLAYER_PACKET->path_set
            ->reachable[PLAYER_PACKET->path_node->index][target] != -1)
      return 1.0f;
  }
  return 0.0f;
#undef PLAYER_PACKET
}

// FUNCTION: LEGOBATMAN 0x00452bd0
f32 Condition_GotCnxCapability(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                               AIPACKET_s *packet, char *str, void *data) {
  if (packet != NULL && data != NULL &&
      (packet->cnx_capabilities & (u32)data) != 0)
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x00452c70
f32 Condition_HasAbility(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                         AIPACKET_s *packet, char *str, void *data) {
  if (packet != NULL && packet->pd0 != NULL && packet->pd0->obj != NULL) {
    GameObject_s *obj = packet->pd0->obj;
    if (obj != NULL) {
      if (!NuStrICmp(str, "Hazard") &&
          (obj->p54->p24->flags13c & 0x4000000) != 0)
        return 1.0f;
      if (!NuStrICmp(str, "Security") &&
          (obj->p54->p24->flags13c & 0x2000000) != 0)
        return 1.0f;
      if (!NuStrICmp(str, "HighJump") &&
          (obj->p54->p24->flags13c & 0x400000) != 0)
        return 1.0f;
      if (!NuStrICmp(str, "Glide") && (obj->p54->p24->flags144 & 0x40000) != 0)
        return 1.0f;
    }
  }
  return 0.0f;
}

// GLOBAL: LEGOBATMAN 0x009c5fc8
extern nuvec_s g_unk009c5fc8; // alert position
// GLOBAL: LEGOBATMAN 0x009c5ffc
extern f32 g_unk009c5ffc; // alert timer

// FUNCTION: LEGOBATMAN 0x00452d70
f32 Condition_AlertRange(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                         AIPACKET_s *packet, char *str, void *data) {
  f32 range = 1.0e9f;
  if (packet != NULL && packet->pd0 != NULL && sys != NULL &&
      g_unk009c5ffc > 0.0f)
    range = NuVecDist(&g_unk009c5fc8, &packet->pd0->pos5c, NULL);
  return range;
}

void *AIPathFindLocatorSet(AISYS_s *sys, char *name);

// FUNCTION: LEGOBATMAN 0x00452de0
void *Condition_LocatorSetIsInit(AISYS_s *sys, char *name, AISCRIPT_s *script) {
  return name != NULL ? AIPathFindLocatorSet(g_unk00960894->aiSys2bf8, name)
                      : NULL;
}

// FUNCTION: LEGOBATMAN 0x00452e10
f32 Condition_LocatorSetIs(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                           AIPACKET_s *packet, char *str, void *data) {
  // 0xac: the process's current locator set.
  if (data != NULL && ((void **)process)[0xac / 4] == data)
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x00452e80
f32 Condition_OpponentInSpecialMove(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                    AIPACKET_s *packet, char *str, void *data) {
  if (packet != NULL && packet->pe4 != NULL && packet->pe4->obj->b9db == 0x30)
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x00452eb0
f32 Condition_IAmInSpecialMove(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                               AIPACKET_s *packet, char *str, void *data) {
  if (packet != NULL) {
    GameObject_s *obj = packet->pd0->obj;
    if (obj->b9db == 0x30 || obj->b9db == 0x74)
      return 1.0f;
  }
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x00452f90
f32 Condition_OpponentIsAVehicle(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                 AIPACKET_s *packet, char *str, void *data) {
  if (packet != NULL && packet->pe4 != NULL && packet->pe4->obj != NULL &&
      (packet->pe4->character->model_flags & 0x2000) != 0)
    return 1.0f;
  return 0.0f;
}

void *AISysFindArea(AISYS_s *sys, char *name);

// FUNCTION: LEGOBATMAN 0x00452fd0
void *Condition_NeutralInTriggerAreaInit(AISYS_s *sys, char *name,
                                         AISCRIPT_s *script) {
  return name != NULL ? AISysFindArea(sys, name) : NULL;
}

struct AIAREA_s {
  u8 pad0[0x38];
  struct AIAreaOwner_s *owner; // 0x38
  u8 pad3c[0x40 - 0x3c];
};

struct AIAreaOwner_s {
  u8 pad0[0x244];
  AIAREA_s *areas; // 0x244
};

// STUB: LEGOBATMAN 0x00452ff0
// close: orig copies data through eax into ebp and keeps the object count in
// ebx; ours allocates edi/ebp the other way round.
f32 Condition_NeutralInTriggerArea(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                   AIPACKET_s *packet, char *str, void *data) {
  // 0xa4: the process's current trigger area.
  AIAREA_s *area;
  if (data == NULL)
    area = ((AIAREA_s **)process)[0xa4 / 4];
  else
    area = (AIAREA_s *)data;
  if (area != NULL) {
    for (i32 index = 0; index < HIGHGAMEOBJECT; index++) {
      GameObject_s *obj = &Obj[index];
      if ((obj->flags1fc & 1) && (obj->flags1fc & 0x1000) && obj->b257 == 0 &&
          !(obj->flags1fc & 0x80) && (obj->flags1f8 & 4) &&
          (obj->area_mask & (1 << (area - area->owner->areas))) != 0)
        return 1.0f;
    }
  }
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044fc70
f32 Condition_Player2Active(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                            AIPACKET_s *packet, char *str, void *data) {
  if (player2 != NULL)
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044fc90
void *Condition_NumBaddiesInit(AISYS_s *sys, char *name, AISCRIPT_s *script) {
  return NULL;
}

// FUNCTION: LEGOBATMAN 0x0044fca0
f32 Condition_NumBaddies(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                         AIPACKET_s *packet, char *str, void *data) {
  i32 count = 0;
  GameObject_s *object = Obj;
  for (i32 i = 0; i < HIGHGAMEOBJECT; ++i, ++object) {
    if ((object->flags1fc & 1) && (object->flags1fc & 0x1000) &&
        (object->flags1f8 & 1))
      ++count;
  }
  return (f32)count;
}

// FUNCTION: LEGOBATMAN 0x0044fd00
void *Condition_NumForceObjectsInit(AISYS_s *system, char *name,
                                    AISCRIPT_s *script) {
  i32 flags = 0;
  if (name != NULL && system != NULL) {
    if (NuStrIStr(name, "throwable") != NULL)
      flags = 1;
    if (NuStrIStr(name, "inrange") != NULL)
      flags |= 2;
  }
  return (void *)flags;
}

struct LEVELDATA_s {
  u8 pad0[0x40];
  char name[0x22]; // 0x40
  i16 idx;         // 0x62
  u8 pad64[0x150 - 0x64];
};

// GLOBAL: LEGOBATMAN 0x00aca894
extern LEVELDATA_s *LDataList;
// GLOBAL: LEGOBATMAN 0x00aca8a4
extern i32 LEVELCOUNT;
// GLOBAL: LEGOBATMAN 0x00aca898
extern LEVELDATA_s *LastLData;

// FUNCTION: LEGOBATMAN 0x0044fd70
void *Condition_BeenToLevelInit(AISYS_s *system, char *arg,
                                AISCRIPT_s *script) {
  if (arg != NULL && system != NULL && g_unk00960894->area != NULL) {
    for (i32 area_level = 0; area_level < g_unk00960894->area->level_count;
         ++area_level) {
      if (NuStrICmp(arg,
                    LDataList[g_unk00960894->area->levels[area_level]].name) ==
          0)
        return (void *)area_level;
    }
  }
  return (void *)-1;
}

// GLOBAL: LEGOBATMAN 0x009ca958
extern u8 *LevelProgressData;

// FUNCTION: LEGOBATMAN 0x0044fe20
f32 Condition_BeenToLevel(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                          AIPACKET_s *packet, char *str, void *void_arg) {
  i32 area_level = (i32)void_arg;
  if (area_level != -1)
    return (f32)(*(u32 *)(LevelProgressData + area_level * 0x2e90 + 0x2800) &
                 1);
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044fe70
void *Condition_LastLevelInit(AISYS_s *system, char *name, AISCRIPT_s *script) {
  if (name != NULL && system != NULL && g_unk00960894->area != NULL) {
    for (i32 index = 0; index < LEVELCOUNT; ++index) {
      if (NuStrICmp(name, LDataList[index].name) == 0)
        return (void *)index;
    }
  }
  return (void *)-1;
}

// FUNCTION: LEGOBATMAN 0x0044fef0
f32 Condition_LastLevel(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                        AIPACKET_s *packet, char *str, void *argument) {
  if (LastLData != NULL && LastLData->idx == (i32)argument)
    return 1.0f;
  return 0.0f;
}

// GLOBAL: LEGOBATMAN 0x00ad210c
extern GIZAIMESSAGESYS_s *gizaimessagesys;

// STUB: LEGOBATMAN 0x0044ff10
// close: orig keeps a zero in eax for the checks and the NULL argument.
void *Condition_MessageInit(AISYS_s *system, char *name, AISCRIPT_s *script) {
  GIZAIMESSAGE_s *message = NULL;
  if (name != NULL && system != NULL && gizaimessagesys != NULL)
    return CheckGizAIMessage(gizaimessagesys, name, message);
  return message;
}

// FUNCTION: LEGOBATMAN 0x0044ff50
f32 Condition_Message(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                      AIPACKET_s *packet, char *str, void *argument) {
  if (argument != NULL)
    return ((f32 *)argument)[0x28 / 4];
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044ff70
void *Condition_ScriptParamInit(AISYS_s *sys, char *name, AISCRIPT_s *script) {
  if (name != NULL) {
    for (i32 index = 0; index < 4; ++index) {
      if (script->params[index].name != NULL &&
          NuStrICmp(script->params[index].name, name) == 0)
        return (void *)index;
    }
    return (void *)NuAToI(name);
  }
  return (void *)-1;
}

// FUNCTION: LEGOBATMAN 0x0044ffd0
f32 Condition_ScriptParam(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                          AIPACKET_s *packet, char *str, void *argument) {
  i32 index = (i32)argument;
  if (index >= 0)
    return process->params[index];
  return 0.0f;
}

i32 NuSpecialExistsFn(nuhspecial_s *special);
nuinstanim_s *NuSpecialGetInstAnim(nuhspecial_s *special);

// FUNCTION: LEGOBATMAN 0x0044fff0
void *Condition_AnimationFinishedInit(AISYS_s *sys, char *name,
                                      AISCRIPT_s *script) {
  nuhspecial_s special;
  NuSpecialFind(g_unk00960894->scn140, &special, name, 1);
  return NuSpecialExistsFn(&special) != 0 ? NuSpecialGetInstAnim(&special)
                                          : NULL;
}

float NuAnimEndFrameOld(void *data);

// FUNCTION: LEGOBATMAN 0x00450050
f32 Condition_AnimationFinished(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                AIPACKET_s *packet, char *str, void *argument) {
  nuinstanim_s *animation = (nuinstanim_s *)argument;
  if (animation != NULL && !(animation->flags50 & 1)) {
    void *data =
        g_unk00960894->scn140->instance_animation_data[animation->anim_ix];
    if (data != NULL && animation->ltime >= NuAnimEndFrameOld(data))
      return 1.0f;
  }
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x004500c0
void *Condition_RigidAnimFrameInit(AISYS_s *sys, char *name,
                                   AISCRIPT_s *script) {
  nuhspecial_s special;
  NuSpecialFind(g_unk00960894->scn140, &special, name, 1);
  return NuSpecialExistsFn(&special) != 0 ? NuSpecialGetInstAnim(&special)
                                          : NULL;
}

// FUNCTION: LEGOBATMAN 0x00450120
f32 Condition_RigidAnimFrame(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                             AIPACKET_s *packet, char *str, void *argument) {
  nuinstanim_s *animation = (nuinstanim_s *)argument;
  if (animation != NULL)
    return animation->ltime;
  return 1.0f;
}

void *CutScene_Find(void *cutscene_sys, char *name);
i32 instNuGCutSceneIsFinished(void *instance);

struct CUTINFO_s {
  u8 pad0[4];
  u8 *instance; // 0x04, byte 0x88 bit 1: started
};

// FUNCTION: LEGOBATMAN 0x00450140
void *Condition_CutSceneStartedInit(AISYS_s *sys, char *name,
                                    AISCRIPT_s *script) {
  return CutScene_Find(g_unk00960894->cutscene_sys, name);
}

// FUNCTION: LEGOBATMAN 0x00450170
f32 Condition_CutSceneStarted(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                              AIPACKET_s *packet, char *str, void *argument) {
  CUTINFO_s *cut = (CUTINFO_s *)argument;
  if (cut != NULL && cut->instance != NULL && (cut->instance[0x88] & 2) != 0)
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x004501a0
void *Condition_CutSceneFinishedInit(AISYS_s *sys, char *name,
                                     AISCRIPT_s *script) {
  return CutScene_Find(g_unk00960894->cutscene_sys, name);
}

// FUNCTION: LEGOBATMAN 0x004501d0
f32 Condition_CutSceneFinished(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                               AIPACKET_s *packet, char *str, void *argument) {
  CUTINFO_s *cut = (CUTINFO_s *)argument;
  if (cut != NULL && cut->instance != NULL &&
      instNuGCutSceneIsFinished(cut->instance) != 0)
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x00450200
void *Condition_CutSceneExistsInit(AISYS_s *sys, char *name,
                                   AISCRIPT_s *script) {
  return CutScene_Find(g_unk00960894->cutscene_sys, name);
}

// FUNCTION: LEGOBATMAN 0x00450230
f32 Condition_CutSceneExists(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                             AIPACKET_s *packet, char *str, void *argument) {
  if (argument != NULL)
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x00450240
void *Condition_CutScenePlayingInit(AISYS_s *sys, char *name,
                                    AISCRIPT_s *script) {
  return CutScene_Find(g_unk00960894->cutscene_sys, name);
}

// FUNCTION: LEGOBATMAN 0x00450270
f32 Condition_CutScenePlaying(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                              AIPACKET_s *packet, char *str, void *argument) {
  return 0.0f;
}

struct SOCK184_s {
  u8 pad0[0x184];
};

struct SOCKSYS_s {
  SOCK184_s *sock; // 0x00
};

void *FindSock(SOCKSYS_s *sys, char *name);

// FUNCTION: LEGOBATMAN 0x00450280
void *Condition_PlayerInSockInit(AISYS_s *sys, char *name, AISCRIPT_s *script) {
  return FindSock(g_unk00960894->sock_sys, name);
}

// FUNCTION: LEGOBATMAN 0x004502b0
f32 Condition_PlayerInSock(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                           AIPACKET_s *packet, char *str, void *argument) {
  if (argument != NULL && g_unk00960894->sock_sys != NULL &&
      &g_unk00960894->sock_sys->sock[player->sock_id] == argument)
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x00450560
f32 Condition_PlayerDistanceAlongSock(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                      AIPACKET_s *packet, char *str,
                                      void *data) {
  if (player != NULL)
    return player->sock_distance;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x00450580
f32 Condition_FurthestPlayerDistanceAlongSock(AISYS_s *sys,
                                              AISCRIPTPROCESS_s *process,
                                              AIPACKET_s *packet, char *str,
                                              void *data) {
  if (player != NULL) {
    if (player2 != NULL && player2->sock_distance > player->sock_distance)
      return player2->sock_distance;
    return player->sock_distance;
  }
  return 0.0f;
}

// STUB: LEGOBATMAN 0x004505d0
// close: block order of the 1.0/0.0 returns differs (4 spellings tried).
f32 Condition_FinishedSpline(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                             AIPACKET_s *packet, char *str, void *data) {
  if (packet != NULL && packet->pd0 != NULL && packet->pd0->obj != NULL) {
    GameObject_s *object = packet->pd0->obj;
    if (object->movement_spline == NULL)
      return 1.0f;
    if (object->movement_spline_finished == 0)
      return 0.0f;
    return 1.0f;
  }
  return -1.0f;
}

i32 Hint_CurrentId(void);
i32 Hint_Available(i32 id);
i32 Hint_Complete(i32 id);

// FUNCTION: LEGOBATMAN 0x00450620
f32 Condition_CurrentHintId(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                            AIPACKET_s *packet, char *str, void *data) {
  return Hint_CurrentId();
}

// FUNCTION: LEGOBATMAN 0x00450640
void *Condition_HintAvailableInit(AISYS_s *sys, char *argument,
                                  AISCRIPT_s *script) {
  return argument != NULL ? (void *)NuAToI(argument) : NULL;
}

// FUNCTION: LEGOBATMAN 0x00450660
f32 Condition_HintAvailable(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                            AIPACKET_s *packet, char *str, void *argument) {
  if (argument != NULL && Hint_Available((i32)argument) != 0)
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x00450690
f32 Condition_HintComplete(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                           AIPACKET_s *packet, char *str, void *argument) {
  if (argument != NULL && Hint_Complete((i32)argument) != 0)
    return 1.0f;
  return 0.0f;
}

// GLOBAL: LEGOBATMAN 0x00ab0894
extern i32 FreePlay;

// FUNCTION: LEGOBATMAN 0x004506c0
f32 Condition_Freeplay(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                       AIPACKET_s *packet, char *str, void *data) {
  return (f32)FreePlay;
}

void *Mission_Active(void *mission);

// FUNCTION: LEGOBATMAN 0x004506d0
f32 Condition_MissionMode(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                          AIPACKET_s *packet, char *str, void *data) {
  if (Mission_Active(NULL) != NULL)
    return 1.0f;
  return 0.0f;
}

// GLOBAL: LEGOBATMAN 0x00acd7f8
extern u8 *MissionSys;

// FUNCTION: LEGOBATMAN 0x004506f0
f32 Condition_MissionWon(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                         AIPACKET_s *packet, char *str, void *data) {
  if (MissionSys != NULL && MissionSys[0x1d] == 2)
    return 1.0f;
  return 0.0f;
}

// GLOBAL: LEGOBATMAN 0x00ab084c
extern i32 ChallengeMode;

// FUNCTION: LEGOBATMAN 0x00450710
f32 Condition_ChallengeMode(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                            AIPACKET_s *packet, char *str, void *data) {
  if (ChallengeMode != 0)
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x00450730
f32 Condition_PSP(AISYS_s *sys, AISCRIPTPROCESS_s *process, AIPACKET_s *packet,
                  char *str, void *data) {
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x00450740
f32 Condition_PS2(AISYS_s *sys, AISCRIPTPROCESS_s *process, AIPACKET_s *packet,
                  char *str, void *data) {
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x00450750
f32 Condition_BoltsDontGetDeflectedBack(AISYS_s *sys,
                                        AISCRIPTPROCESS_s *process,
                                        AIPACKET_s *packet, char *str,
                                        void *data) {
  if (packet != NULL && packet->pd0 != NULL &&
      (((u8 *)packet->pd0->obj)[0x1410] & 0x80) != 0)
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x00450780
f32 Condition_CheatProgress(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                            AIPACKET_s *packet, char *str, void *data) {
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x00450790
f32 Condition_BigJumpComplete(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                              AIPACKET_s *packet, char *str, void *data) {
  if (packet != NULL && packet->pd0 != NULL && packet->pd0->obj->b9db == 0x1f)
    return 0.0f;
  return 1.0f;
}

// FUNCTION: LEGOBATMAN 0x004507c0
void *Condition_RespawnLocatorIsInit(AISYS_s *sys, char *name,
                                     AISCRIPT_s *script) {
  return name != NULL ? AIPathFindLocator(sys, name) : NULL;
}

// FUNCTION: LEGOBATMAN 0x004507e0
f32 Condition_RespawnLocatorIs(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                               AIPACKET_s *packet, char *str, void *argument) {
  // 0x1b4: the packet's respawn locator.
  if (argument != NULL && packet != NULL &&
      ((void **)packet)[0x1b4 / 4] == argument)
    return 1.0f;
  return 0.0f;
}

// GLOBAL: LEGOBATMAN 0x009c5828
extern i32 MiniCutCam;
// GLOBAL: LEGOBATMAN 0x00acb114
extern void *ObstacleCamSpl;

// FUNCTION: LEGOBATMAN 0x00450810
f32 Condition_InMiniCut(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                        AIPACKET_s *packet, char *arg, void *data) {
  if (data != NULL) {
    if (((u8 *)data)[0x18] >= 3)
      return 1.0f;
  } else if (MiniCutCam != 0 || (arg != NULL && ObstacleCamSpl != NULL)) {
    return 1.0f;
  }
  return 0.0f;
}

// GLOBAL: LEGOBATMAN 0x009c600c
extern f32 drop_back_in_timer;
// GLOBAL: LEGOBATMAN 0x009c5fe4
extern i32 party_under_cover;
// GLOBAL: LEGOBATMAN 0x009c5b48
extern i32 nbaddies_can_see_players;

// FUNCTION: LEGOBATMAN 0x004509a0
f32 Condition_DropBackInTimer(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                              AIPACKET_s *packet, char *str, void *data) {
  return drop_back_in_timer;
}

// FUNCTION: LEGOBATMAN 0x004509b0
f32 Condition_PartyUnderCover(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                              AIPACKET_s *packet, char *str, void *data) {
  if (party_under_cover != 0)
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x004509d0
f32 Condition_NumBaddiesThatCanSeePlayers(AISYS_s *sys,
                                          AISCRIPTPROCESS_s *process,
                                          AIPACKET_s *packet, char *str,
                                          void *data) {
  return (f32)nbaddies_can_see_players;
}

// FUNCTION: LEGOBATMAN 0x00450ab0
f32 Condition_PartyContainsDroids(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                  AIPACKET_s *packet, char *str, void *data) {
  for (i32 index = 0; index < 8; ++index) {
    GameObject_s *object = Player[index];
    if (object != NULL && (object->flags1fc & 1) &&
        (object->flags1fc & 0x1000) &&
        (*(u32 *)((u8 *)object + 0x1410) & 0x10000000) == 0 &&
        (object->p54->model_flags & 0x10) != 0)
      return 1.0f;
  }
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x00450b10
f32 Condition_CannotReachDestination(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                     AIPACKET_s *packet, char *str,
                                     void *data) {
  // 0x1f0: packet runtime flags.
  if (packet != NULL && (((u32 *)packet)[0x1f0 / 4] & 0x400000) != 0)
    return 1.0f;
  return 0.0f;
}

// GLOBAL: LEGOBATMAN 0x0095fe74
extern i32 spinner_gizmotype_id;

// FUNCTION: LEGOBATMAN 0x00450b30
void *Condition_EitherPlayerPushingSpinnerInit(AISYS_s *sys, char *name,
                                               AISCRIPT_s *script) {
  return GizmoFindByName(g_unk00960894->gizmoSys2b0c, spinner_gizmotype_id,
                         name);
}

// FUNCTION: LEGOBATMAN 0x00450b60
f32 Condition_EitherPlayerPushingSpinner(AISYS_s *sys,
                                         AISCRIPTPROCESS_s *process,
                                         AIPACKET_s *packet, char *str,
                                         void *argument) {
  void *spinner = *(void **)argument;
  if (spinner != NULL) {
    if (player != NULL && player->b9db == 0x28 &&
        (void *)player->techno == spinner)
      return 1.0f;
    if (player2 != NULL && player2->b9db == 0x28 &&
        (void *)player2->techno == spinner)
      return 1.0f;
  }
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x00450c00
f32 Condition_CharacterRange(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                             AIPACKET_s *packet, char *str, void *argument) {
  GameObject_s *object = (GameObject_s *)argument;
  if (packet != NULL && packet->pd0 != NULL && object != NULL) {
    nuvec_s difference;
    return NuVecDist(&object->position, &packet->pd0->pos5c, &difference);
  }
  return 1.0e9f;
}

// FUNCTION: LEGOBATMAN 0x00450c50
f32 Condition_BeenSpawned(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                          AIPACKET_s *packet, char *str, void *data) {
  // 0x138: the packet's spawn slot, -1 when not spawned.
  if (packet != NULL && packet->pd0 != NULL && OWNER(packet)->b24c == -1 &&
      ((u8 *)packet)[0x138] == 0xff)
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x00450c90
void *Condition_TakeOverTargetInTriggerAreaInit(AISYS_s *sys, char *name,
                                                AISCRIPT_s *script) {
  return name != NULL ? AISysFindArea(sys, name) : NULL;
}

// FUNCTION: LEGOBATMAN 0x00450e20
void *Condition_AnyPartyInTriggerAreaInit(AISYS_s *sys, char *name,
                                          AISCRIPT_s *script) {
  return name != NULL ? AISysFindArea(sys, name) : NULL;
}

// FUNCTION: LEGOBATMAN 0x00450ed0
void *Condition_AllPartyInTriggerAreaInit(AISYS_s *sys, char *name,
                                          AISCRIPT_s *script) {
  return name != NULL ? AISysFindArea(sys, name) : NULL;
}

// FUNCTION: LEGOBATMAN 0x004512b0
f32 Condition_BeenTakenOver(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                            AIPACKET_s *packet, char *str, void *argument) {
  GameObject_s *object = (GameObject_s *)argument;
  if (object == NULL && packet->pd0 != NULL)
    object = packet->pd0->obj;
  if (object != NULL && object->p1158 != NULL && object->b9db == 0x3b)
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x00451340
f32 Condition_OnSpeederBike(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                            AIPACKET_s *packet, char *str, void *data) {
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x00451390
f32 Condition_UnderPlayerControl(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                 AIPACKET_s *packet, char *str,
                                 void *argument) {
  GameObject_s *object = (GameObject_s *)argument;
  if (object != NULL && (object->flags1fc & 0x80) != 0)
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x004513f0
f32 Condition_CharacterExists(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                              AIPACKET_s *packet, char *str, void *argument) {
  if (argument != NULL)
    return 1.0f;
  return 0.0f;
}

struct CDATA48_s {
  u8 pad0[0xc];
  char *file; // 0x0c
  u8 pad10[0x48 - 0x10];
};

// GLOBAL: LEGOBATMAN 0x00acb81c
extern CDATA48_s *CDataList;
// GLOBAL: LEGOBATMAN 0x00acb820
extern i32 CHARCOUNT;

// FUNCTION: LEGOBATMAN 0x00451400
void *Condition_CharacterTypeExistsInit(AISYS_s *system, char *name,
                                        AISCRIPT_s *script) {
  if (name != NULL && system != NULL) {
    for (i32 index = 0; index < CHARCOUNT; ++index) {
      if (NuStrICmp(CDataList[index].file, name) == 0)
        return (void *)index;
    }
  }
  return (void *)-1;
}

i32 Hub_GetRandomCharType(void);

// FUNCTION: LEGOBATMAN 0x00451470
f32 Condition_CharacterTypeExists(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                  AIPACKET_s *packet, char *str, void *data) {
  i32 i;
  if (!NuStrICmp(str, "RandomMap"))
    return Hub_GetRandomCharType() > -1 ? 0.0f : 1.0f;
  if ((i32)data < 0)
    return 0.0f;
  for (i = 0; i < HIGHGAMEOBJECT; i++)
    if (Obj[i].type15b0 == (i32)data)
      return 1.0f;
  return 0.0f;
}

// GLOBAL: LEGOBATMAN 0x00960048
extern i32 g_unk00960048;
// GLOBAL: LEGOBATMAN 0x00960a3c
extern i32 g_unk00960a3c;
// GLOBAL: LEGOBATMAN 0x009656d8
extern i32 g_unk009656d8;
extern i32 blowup_gizmotype_id;

i32 GizForce_Complete(GIZFORCE_s *force);
i32 GizForce_AnimComplete(GIZFORCE_s *force);

// FUNCTION: LEGOBATMAN 0x0044e480
void *Condition_LeverActiveInit(AISYS_s *sys, char *name, AISCRIPT_s *script) {
  void *lever = NULL;
  if (name != NULL) {
    GIZMO_s *gizmo =
        GizmoFindByName(g_unk00960894->gizmoSys2b0c, g_unk00960048, name);
    if (gizmo != NULL && *(void **)gizmo != NULL)
      lever = *(void **)gizmo;
  }
  return lever;
}

// FUNCTION: LEGOBATMAN 0x0044e4d0
f32 Condition_LeverActive(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                          AIPACKET_s *packet, char *str, void *argument) {
  if (argument != NULL && (((u8 *)argument)[0x94] & 0x80))
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044e4f0
f32 Condition_EitherPlayerWearingHelmet(AISYS_s *sys,
                                        AISCRIPTPROCESS_s *process,
                                        AIPACKET_s *packet, char *str,
                                        void *argument) {
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044e500
void *Condition_ForceCompleteInit(AISYS_s *sys, char *name,
                                  AISCRIPT_s *script) {
  GIZMO_s *gizmo =
      GizmoFindByName(g_unk00960894->gizmoSys2b0c, force_gizmotype_id, name);
  return gizmo != NULL ? *(void **)gizmo : NULL;
}

// FUNCTION: LEGOBATMAN 0x0044e540
f32 Condition_ForceComplete(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                            AIPACKET_s *packet, char *str, void *argument) {
  if (argument != NULL && GizForce_Complete((GIZFORCE_s *)argument) != 0)
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044e570
f32 Condition_ForceFinished(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                            AIPACKET_s *packet, char *str, void *argument) {
  if (argument != NULL && GizForce_AnimComplete((GIZFORCE_s *)argument) != 0)
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044e5a0
void *Condition_BuildItCompleteInit(AISYS_s *sys, char *name,
                                    AISCRIPT_s *script) {
  return GizmoFindByName(g_unk00960894->gizmoSys2b0c, g_unk00960a3c, name);
}

// FUNCTION: LEGOBATMAN 0x0044e5d0
f32 Condition_BuildItComplete(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                              AIPACKET_s *packet, char *str, void *argument) {
  if (argument != NULL) {
    u8 *build = *(u8 **)argument;
    if (build != NULL && build[0x71] == 2)
      return 1.0f;
  }
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044e5f0
void *Condition_DigCompleteInit(AISYS_s *sys, char *name, AISCRIPT_s *script) {
  return GizmoFindByName(g_unk00960894->gizmoSys2b0c, g_unk009656d8, name);
}

// FUNCTION: LEGOBATMAN 0x0044e620
f32 Condition_DigComplete(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                          AIPACKET_s *packet, char *str, void *argument) {
  if (argument != NULL) {
    u8 *dig = *(u8 **)argument;
    if (dig != NULL && *(i16 *)(dig + 0x70) == 1)
      return 1.0f;
  }
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044e650
void *Condition_BlowupInit(AISYS_s *sys, char *name, AISCRIPT_s *script) {
  return GizmoFindByName(g_unk00960894->gizmoSys2b0c, blowup_gizmotype_id,
                         name);
}

// FUNCTION: LEGOBATMAN 0x0044e680
f32 Condition_BlowupBlownup(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                            AIPACKET_s *packet, char *str, void *argument) {
  if (argument != NULL && GizmoGetOutput(g_unk00960894->gizmoSys2b0c,
                                         (GIZMO_s *)argument, 0, 1) != 0)
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044e6c0
void *Condition_BlowupBeenPickedUpInit(AISYS_s *sys, char *name,
                                       AISCRIPT_s *script) {
  GIZMO_s *gizmo =
      GizmoFindByName(g_unk00960894->gizmoSys2b0c, blowup_gizmotype_id, name);
  return gizmo != NULL ? *(void **)gizmo : NULL;
}

// FUNCTION: LEGOBATMAN 0x0044e700
f32 Condition_BlowupBeenPickedUp(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                 AIPACKET_s *packet, char *str,
                                 void *argument) {
  if (argument != NULL && (((u8 *)argument)[0x122] & 1))
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044e720
void *Condition_IsAliveInit(AISYS_s *sys, char *arg, AISCRIPT_s *script) {
  return arg != NULL && GetNamedAPIObjectFn != NULL
             ? GetNamedAPIObjectFn(sys, arg)
             : NULL;
}

struct AIGroupUnk_s {
  u8 pad0[7];
  u8 count; // 0x07
  u8 pad8[0xc - 8];
  GameObject_s *members[1]; // 0x0c
};

// STUB: LEGOBATMAN 0x0044e750
// close: orig tests members[i] in memory then reloads it, and allocates
// esi/ecx/edx where ours picks edx/esi/eax
f32 Condition_IsAlive(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                      AIPACKET_s *packet, char *str, void *argument) {
  GameObject_s *object = (GameObject_s *)argument;
  if (object != NULL) {
    // +4: the AI, whose +0x144 is its group.
    AIGroupUnk_s *group =
        *(AIGroupUnk_s **)(*(u8 **)((u8 *)object + 4) + 0x144);
    if (group != NULL) {
      for (i32 i = 0; i < group->count; i++) {
        if (group->members[i] != NULL) {
          GameObject_s *member = group->members[i];
          if ((member->flags1fc & 1) && (member->flags1fc & 0x1000) &&
              member->b257 == 0)
            return 1.0f;
        }
      }
    } else if ((object->flags1fc & 0x1000) && object->b257 == 0) {
      return 1.0f;
    }
  }
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044e7e0
void *Condition_AIOverrideControlInit(AISYS_s *sys, char *arg,
                                      AISCRIPT_s *script) {
  return arg != NULL && GetNamedAPIObjectFn != NULL
             ? GetNamedAPIObjectFn(sys, arg)
             : NULL;
}

// FUNCTION: LEGOBATMAN 0x0044e810
f32 Condition_AIOverrideControl(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                AIPACKET_s *packet, char *str, void *argument) {
  GameObject_s *object = (GameObject_s *)argument;
  if (object == NULL) {
    if (packet == NULL || (object = (GameObject_s *)packet->pd0) == NULL)
      return 0.0f;
  }
  if (object->flags1fc & 0x100)
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044e850
void *Condition_IsOnScreenInit(AISYS_s *sys, char *arg, AISCRIPT_s *script) {
  return arg != NULL && GetNamedAPIObjectFn != NULL
             ? GetNamedAPIObjectFn(sys, arg)
             : NULL;
}

// FUNCTION: LEGOBATMAN 0x0044e880
f32 Condition_IsOnScreen(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                         AIPACKET_s *packet, char *str, void *argument) {
  u8 *object = (u8 *)argument;
  if (object == NULL) {
    if (packet == NULL || (object = (u8 *)packet->pd0) == NULL)
      return 0.0f;
  }
  // 0x254: the model's draw result.
  if (object[0x254] != 0)
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044e8f0
f32 Condition_OffScreenTimer(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                             AIPACKET_s *packet, char *str, void *argument) {
  GameObject_s *object = (GameObject_s *)argument;
  if (object == NULL) {
    object = packet != NULL && packet->pd0 != NULL ? packet->pd0->obj : NULL;
  }
  if (object != NULL)
    return *(f32 *)((u8 *)object + 0x1438);
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044e930
f32 Condition_PlayerOnGround(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                             AIPACKET_s *packet, char *str, void *argument) {
  if (player != NULL &&
      (((u8 *)player)[0x24d] != 0 || ((u8 *)player)[0x24e] != 0))
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044e960
f32 Condition_OnGround(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                       AIPACKET_s *packet, char *str, void *argument) {
  if (packet != NULL && packet->pd0 != NULL) {
    u8 *object = (u8 *)packet->pd0->obj;
    if (object != NULL && (object[0x24d] != 0 || object[0x24e] != 0))
      return 1.0f;
  }
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044e9a0
f32 Condition_InMud(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                    AIPACKET_s *packet, char *str, void *argument) {
  if (packet != NULL && packet->pd0 != NULL) {
    GameObject_s *object = packet->pd0->obj;
    if (object != NULL && object->b24f == 6)
      return 1.0f;
  }
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044ea70
f32 Condition_BeenAlerted(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                          AIPACKET_s *packet, char *str, void *argument) {
  if (packet != NULL && packet->pd0 != NULL) {
    GameObject_s *object = packet->pd0->obj;
    // 0x13d8: alert target.
    if (object != NULL && *(void **)((u8 *)object + 0x13d8) != NULL)
      return 1.0f;
  }
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044ec00
void *Condition_LocatorRangeInit(AISYS_s *sys, char *arg, AISCRIPT_s *script) {
  return arg != NULL ? AIPathFindLocator(sys, arg) : NULL;
}

// FUNCTION: LEGOBATMAN 0x0044ea10
f32 Condition_PlayerOnElephantInWater(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                      AIPACKET_s *packet, char *str,
                                      void *argument) {
  GameObject_s *object = (GameObject_s *)argument;
  if (object == NULL) {
    if (packet->pd0 == NULL || (object = packet->pd0->obj) == NULL)
      return 0.0f;
  }
  GameObject_s *rider = object->p1158;
  if (rider != NULL && object->b9db == 0x3b &&
      *(i16 *)((u8 *)rider + 0x15b0) == 0x2c && rider->b24f == 1)
    return 1.0f;
  return 0.0f;
}

// GLOBAL: LEGOBATMAN 0x009f78f4
extern void *CurTerr;

i32 NuSpecialGetInstanceix(nuhspecial_s *special);
i32 FindPlatInst(i32 instance);

// FUNCTION: LEGOBATMAN 0x0044eaa0
void *Condition_OnObjectInit(AISYS_s *sys, char *name, AISCRIPT_s *script) {
  nuhspecial_s special;
  if (CurTerr != NULL &&
      NuSpecialFind(g_unk00960894->scn140, &special, name, 1) != 0)
    return (void *)FindPlatInst(NuSpecialGetInstanceix(&special));
  return (void *)-1;
}

// FUNCTION: LEGOBATMAN 0x0044eb10
f32 Condition_PlayerOnObject(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                             AIPACKET_s *packet, char *str, void *argument) {
  i32 platform = (i32)argument;
  if (player != NULL && platform != -1 &&
      (((u8 *)player)[0x24d] != 0 || ((u8 *)player)[0x24e] != 0) &&
      *(i16 *)((u8 *)player + 0x24a) == platform &&
      *(i16 *)((u8 *)player + 0x15b8) == platform)
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044eb60
f32 Condition_EitherPlayerOnObject(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                   AIPACKET_s *packet, char *str,
                                   void *argument) {
  i32 platform = (i32)argument;
  if (player != NULL && platform != -1 &&
      (((u8 *)player)[0x24d] != 0 || ((u8 *)player)[0x24e] != 0) &&
      *(i16 *)((u8 *)player + 0x24a) == platform &&
      *(i16 *)((u8 *)player + 0x15b8) == platform)
    return 1.0f;
  if (player2 != NULL && platform != -1 &&
      (((u8 *)player2)[0x24d] != 0 || ((u8 *)player2)[0x24e] != 0) &&
      *(i16 *)((u8 *)player2 + 0x24a) == platform &&
      *(i16 *)((u8 *)player2 + 0x15b8) == platform)
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044ed00
f32 Condition_OnObject(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                       AIPACKET_s *packet, char *str, void *argument) {
  if (packet != NULL && packet->pd0 != NULL) {
    u8 *object = (u8 *)packet->pd0->obj;
    i32 platform = (i32)argument;
    if (object != NULL && platform != -1 &&
        (object[0x24d] != 0 || object[0x24e] != 0) &&
        *(i16 *)(object + 0x24a) == platform &&
        *(i16 *)(object + 0x15b8) == platform)
      return 1.0f;
  }
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044ed70
f32 Condition_OnSameObjectAsPlayer(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                   AIPACKET_s *packet, char *str,
                                   void *argument) {
  if (packet != NULL && packet->pd0 != NULL && player != NULL) {
    u8 *object = (u8 *)packet->pd0->obj;
    u8 *other = (u8 *)player;
    if ((object[0x24d] != 0 || object[0x24e] != 0) &&
        (other[0x24d] != 0 || other[0x24e] != 0) &&
        *(i16 *)(object + 0x24a) != -1 &&
        *(i16 *)(object + 0x24a) == *(i16 *)(other + 0x24a) &&
        *(i16 *)(object + 0x15b8) == *(i16 *)(other + 0x15b8))
      return 1.0f;
  }
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044ee00
f32 Condition_SpawnCount(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                         AIPACKET_s *packet, char *str, void *argument) {
  // 0x204: respawn count.
  if (packet != NULL)
    return (f32) * (u32 *)((u8 *)packet + 0x204);
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044eed0
void *Condition_LocatorOnScreenInit(AISYS_s *sys, char *name,
                                    AISCRIPT_s *script) {
  return AIPathFindLocator(sys, name);
}

i32 NuCameraClipTestSphere(nuvec_s *pos, f32 radius, numtx_s *mtx);

// FUNCTION: LEGOBATMAN 0x0044eef0
f32 Condition_LocatorOnScreen(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                              AIPACKET_s *packet, char *str, void *argument) {
  u8 *locator = (u8 *)argument;
  if (locator == NULL)
    locator = *(u8 **)((u8 *)process + 0xa8);
  if (locator != NULL && NuCameraClipTestSphere((nuvec_s *)(locator + 0x10),
                                                0.0f, &numtx_identity) == 0)
    return 1.0f;
  return 0.0f;
}

i32 ObjBlocking(GameObject_s *object);

// FUNCTION: LEGOBATMAN 0x0044ef40
f32 Condition_Blocking(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                       AIPACKET_s *packet, char *str, void *argument) {
  if (packet != NULL && packet->pd0 != NULL) {
    GameObject_s *object = packet->pd0->obj;
    if (object != NULL && ObjBlocking(object) != 0)
      return 1.0f;
  }
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044efb0
f32 Condition_BeenHit(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                      AIPACKET_s *packet, char *str, void *argument) {
  GameObject_s *object = (GameObject_s *)argument;
  if (object == NULL)
    object = packet != NULL && packet->pd0 != NULL ? packet->pd0->obj : NULL;
  if (object != NULL &&
      (object->flicker_time > 0.0f || (object->flags140c & 1)))
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044f030
f32 Condition_BeenHitByBatarang(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                AIPACKET_s *packet, char *str, void *argument) {
  GameObject_s *object = (GameObject_s *)argument;
  if (object == NULL) {
    if (packet == NULL || packet->pd0 == NULL ||
        (object = packet->pd0->obj) == NULL)
      return 0.0f;
  }
  if (object->flags140c & 2)
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044f0a0
f32 Condition_BeenHitByWhip(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                            AIPACKET_s *packet, char *str, void *argument) {
  GameObject_s *object = (GameObject_s *)argument;
  if (object == NULL) {
    if (packet == NULL || packet->pd0 == NULL ||
        (object = packet->pd0->obj) == NULL)
      return 0.0f;
  }
  if (object->flags140c & 4)
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044f0e0
f32 Condition_HoverPhase(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                         AIPACKET_s *packet, char *str, void *argument) {
  if (packet != NULL && packet->pd0 != NULL)
    return (f32)((u8 *)packet->pd0->obj)[0x131c];
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044f150
f32 Condition_HitPoints(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                        AIPACKET_s *packet, char *str, void *argument) {
  GameObject_s *object = (GameObject_s *)argument;
  if (object == NULL) {
    object = packet != NULL && packet->pd0 != NULL ? packet->pd0->obj : NULL;
  }
  if (object != NULL)
    return (f32)((i8 *)object)[0x15c7];
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044f190
f32 Condition_CollidingWithOpponent(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                    AIPACKET_s *packet, char *str,
                                    void *argument) {
  // 0x1f0: colliding-objects mask; 0x1e8: collision identity mask.
  if (packet != NULL && packet->pe4 != NULL && packet->pd0 != NULL &&
      (*(unsigned __int64 *)((u8 *)packet->pd0 + 0x1f0) &
       *(unsigned __int64 *)((u8 *)packet->pe4 + 0x1e8)) != 0)
    return 1.0f;
  return 0.0f;
}

struct ForcePlatAnimData_s {
  u8 pad0[2];
  i16 platform_id; // 0x02
};

struct ForcePlatAnimObj_s {
  ForcePlatAnimObj_s *next; // 0x00
  u8 pad4[0x24 - 4];
  ForcePlatAnimData_s *data; // 0x24
};

struct ForcePlatAnimSet_s {
  u8 pad0[0x24];
  ForcePlatAnimObj_s *objects; // 0x24
};

struct ForcePlatGizmo_s {
  u8 pad0[0x28];
  ForcePlatAnimSet_s *anim_set; // 0x28
};

struct TerrPlatform_s {
  u8 pad0[0x40];
  numtx_s *mtx; // 0x40
  u8 pad44[0x6c - 0x44];
};

struct TerrPlatforms_s {
  u8 pad0[0x68];
  TerrPlatform_s *platforms; // 0x68
};

// FUNCTION: LEGOBATMAN 0x0044df00
f32 Condition_OnForcePlatform(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                              AIPACKET_s *packet, char *str, void *argument) {
  ForcePlatGizmo_s *force = (ForcePlatGizmo_s *)argument;
  if (force != NULL && packet != NULL && packet->pd0 != NULL) {
    GameObject_s *object = packet->pd0->obj;
    if (object != NULL &&
        (((u8 *)object)[0x24d] != 0 || ((u8 *)object)[0x24e] != 0)) {
      i16 platform = *(i16 *)((u8 *)object + 0x24a);
      if (platform == -1)
        return 0.0f;
      numtx_s *mtx = ((TerrPlatforms_s *)CurTerr)->platforms[platform].mtx;
      if (object->position.y >= mtx->m31) {
        for (ForcePlatAnimObj_s *anim = force->anim_set->objects; anim != NULL;
             anim = anim->next) {
          if (platform == anim->data->platform_id)
            return 1.0f;
        }
      }
    }
  }
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044dfb0
f32 Condition_PlayerOnForcePlatform(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                    AIPACKET_s *packet, char *str,
                                    void *argument) {
  ForcePlatGizmo_s *force = (ForcePlatGizmo_s *)argument;
  if (force != NULL && player != NULL) {
    GameObject_s *object = player;
    if ((((u8 *)object)[0x24d] != 0 || ((u8 *)object)[0x24e] != 0)) {
      i16 platform = *(i16 *)((u8 *)object + 0x24a);
      if (platform == -1)
        return 0.0f;
      numtx_s *mtx = ((TerrPlatforms_s *)CurTerr)->platforms[platform].mtx;
      if (object->position.y >= mtx->m31) {
        for (ForcePlatAnimObj_s *anim = force->anim_set->objects; anim != NULL;
             anim = anim->next) {
          if (platform == anim->data->platform_id)
            return 1.0f;
        }
      }
    }
  }
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044e050
f32 Condition_EitherPlayerOnForcePlatform(AISYS_s *sys,
                                          AISCRIPTPROCESS_s *process,
                                          AIPACKET_s *packet, char *str,
                                          void *argument) {
  ForcePlatGizmo_s *force = (ForcePlatGizmo_s *)argument;
  if (force != NULL && player != NULL) {
    i32 players = 0;
    if ((((u8 *)player)[0x24d] != 0 || ((u8 *)player)[0x24e] != 0) &&
        *(i16 *)((u8 *)player + 0x24a) != -1) {
      numtx_s *mtx = ((TerrPlatforms_s *)CurTerr)
                         ->platforms[*(i16 *)((u8 *)player + 0x24a)]
                         .mtx;
      if (player->position.y >= mtx->m31)
        players |= 1;
    }
    if (player2 != NULL &&
        (((u8 *)player2)[0x24d] != 0 || ((u8 *)player2)[0x24e] != 0) &&
        *(i16 *)((u8 *)player2 + 0x24a) != -1) {
      numtx_s *mtx = ((TerrPlatforms_s *)CurTerr)
                         ->platforms[*(i16 *)((u8 *)player2 + 0x24a)]
                         .mtx;
      if (player2->position.y >= mtx->m31)
        players |= 2;
    }
    if (players != 0) {
      for (ForcePlatAnimObj_s *anim = force->anim_set->objects; anim != NULL;
           anim = anim->next) {
        if ((players & 1) &&
            *(i16 *)((u8 *)player + 0x24a) == anim->data->platform_id)
          return 1.0f;
        if ((players & 2) &&
            *(i16 *)((u8 *)player2 + 0x24a) == anim->data->platform_id)
          return 1.0f;
      }
    }
  }
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044d830
void *Condition_IAmAInit(AISYS_s *system, char *name, AISCRIPT_s *script) {
  if (name != NULL && system != NULL) {
    for (i32 index = 0; index < CHARCOUNT; ++index) {
      if (NuStrICmp(CDataList[index].file, name) == 0)
        return (void *)index;
    }
  }
  return (void *)-1;
}

// FUNCTION: LEGOBATMAN 0x0044d8a0
f32 Condition_IAmA(AISYS_s *sys, AISCRIPTPROCESS_s *process, AIPACKET_s *packet,
                   char *str, void *argument) {
  // 0x15b0: character type.
  if (packet != NULL && packet->pd0 != NULL && packet->pd0->obj != NULL &&
      *(i16 *)((u8 *)packet->pd0->obj + 0x15b0) == (i32)argument)
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044d8e0
void *Condition_OpponentIsAInit(AISYS_s *system, char *name,
                                AISCRIPT_s *script) {
  if (name != NULL && system != NULL) {
    for (i32 index = 0; index < CHARCOUNT; ++index) {
      if (NuStrICmp(CDataList[index].file, name) == 0)
        return (void *)index;
    }
  }
  return (void *)-1;
}

// FUNCTION: LEGOBATMAN 0x0044d950
f32 Condition_OpponentIsA(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                          AIPACKET_s *packet, char *str, void *argument) {
  if (packet != NULL && packet->pe4 != NULL && packet->pe4->obj != NULL &&
      *(i16 *)((u8 *)packet->pe4->obj + 0x15b0) == (i32)argument)
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044d990
f32 Condition_OpponentIsAThreat(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                AIPACKET_s *packet, char *str, void *argument) {
  if (packet != NULL) {
    if (packet->pd0 != NULL && packet->pd0->obj != NULL &&
        *(f32 *)((u8 *)packet->pd0->obj + 0x135c) > 0.0f)
      return 1.0f;
    // 0x1f0 bit 11: opponent is a threat.
    return (f32)((*(u32 *)((u8 *)packet + 0x1f0) >> 11) & 1);
  }
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044d9f0
f32 Condition_OpponentJustPickedUpWeapon(AISYS_s *sys,
                                         AISCRIPTPROCESS_s *process,
                                         AIPACKET_s *packet, char *str,
                                         void *argument) {
  if (packet != NULL && packet->pe4 != NULL && packet->pe4->obj != NULL &&
      *(f32 *)((u8 *)packet->pe4->obj + 0xb84) > 0.0f)
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044e2d0
f32 Condition_EitherPlayerPullingLever(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                       AIPACKET_s *packet, char *str,
                                       void *argument) {
  if (player != NULL && player->b9db == 0x4a) {
    char *techno = (char *)player->techno;
    if (techno != NULL && (str == NULL || NuStrICmp(techno + 0x58, str) == 0))
      return 1.0f;
  }
  if (player2 != NULL && player2->b9db == 0x4a) {
    char *techno = (char *)player2->techno;
    if (techno != NULL && (str == NULL || NuStrICmp(techno + 0x58, str) == 0))
      return 1.0f;
  }
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044e360
f32 Condition_EitherPlayerUsingPanel(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                     AIPACKET_s *packet, char *str,
                                     void *argument) {
  if (player != NULL && player->b9db == 0xb) {
    char *techno = (char *)player->techno;
    if (techno != NULL && (str == NULL || NuStrICmp(techno + 0x40, str) == 0))
      return 1.0f;
  }
  if (player2 != NULL && player2->b9db == 0xb) {
    char *techno = (char *)player2->techno;
    if (techno != NULL && (str == NULL || NuStrICmp(techno + 0x40, str) == 0))
      return 1.0f;
  }
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0044e3f0
f32 Condition_EitherPlayerUsingTechno(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                      AIPACKET_s *packet, char *str,
                                      void *argument) {
  if (player != NULL && player->b9db == 0x51) {
    char *techno = (char *)player->techno;
    if (techno != NULL && (str == NULL || NuStrICmp(techno + 0x40, str) == 0))
      return 1.0f;
  }
  if (player2 != NULL && player2->b9db == 0x51) {
    char *techno = (char *)player2->techno;
    if (techno != NULL && (str == NULL || NuStrICmp(techno + 0x40, str) == 0))
      return 1.0f;
  }
  return 0.0f;
}

i32 GetMenuID();

// FUNCTION: LEGOBATMAN 0x00451960
f32 Condition_ShopActive(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                         AIPACKET_s *packet, char *str, void *argument) {
  if (GetMenuID() == 0xd)
    return 1.0f;
  return 0.0f;
}

// GLOBAL: LEGOBATMAN 0x00a97d70
extern f32 g_unk00a97d70;

// FUNCTION: LEGOBATMAN 0x004518f0
f32 Condition_ScreenWipe(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                         AIPACKET_s *packet, char *str, void *argument) {
  if (g_unk00a97d70 > 0.0f)
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x00452510
f32 Condition_NotTaggableSet(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                             AIPACKET_s *packet, char *str, void *argument) {
  if (argument != NULL && (((u8 *)argument)[0x9ed] & 2))
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x004510a0
f32 Condition_PlayerTakenOver(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                              AIPACKET_s *packet, char *str, void *argument) {
  if (player != NULL && player->p1158 != NULL && player->b9db != 0x3b)
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x00451120
f32 Condition_Player2TakenOver(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                               AIPACKET_s *packet, char *str, void *argument) {
  if (player2 != NULL && player2->p1158 != NULL && player2->b9db != 0x3b)
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x004518c0
f32 Condition_GotVictim(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                        AIPACKET_s *packet, char *str, void *argument) {
  if (packet != NULL && packet->pd0 != NULL &&
      (((u8 *)packet->pd0)[0x1310] & 4))
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x00451570
f32 Condition_LastAttackerIsActivePlayer(AISYS_s *sys,
                                         AISCRIPTPROCESS_s *process,
                                         AIPACKET_s *packet, char *str,
                                         void *argument) {
  if (packet != NULL && packet->pd0 != NULL) {
    // 0x15ec: last attacker.
    u8 *attacker = *(u8 **)((u8 *)packet->pd0->obj + 0x15ec);
    if (attacker != NULL && (attacker[0x1fc] & 0x80))
      return 1.0f;
  }
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x00452e30
f32 Condition_MindControlled(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                             AIPACKET_s *packet, char *str, void *argument) {
  if (str != NULL && GetNamedAPIObjectFn != NULL) {
    Unk_AIPacketObj *api = GetNamedAPIObjectFn(sys, str);
    if (api != NULL && api->obj != NULL &&
        (*(u32 *)((u8 *)api->obj + 0x1418) & 0x800))
      return 1.0f;
  }
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x00452490
f32 Condition_GoopVehicle(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                          AIPACKET_s *packet, char *str, void *argument) {
  if (player != NULL && (player->p54->p24->flags13c & 0x4000000))
    return 1.0f;
  if (player2 != NULL && (player2->p54->p24->flags13c & 0x4000000))
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x00451910
f32 Condition_HeadTurnRestricted(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                 AIPACKET_s *packet, char *str,
                                 void *argument) {
  if (packet != NULL && packet->pd0 != NULL && packet->pd0->obj != NULL)
    return (f32)((*(u32 *)((u8 *)packet->pd0->obj + 0x1410) >> 20) & 1);
  return 0.0f;
}

f32 NuVecDist(nuvec_s *a, nuvec_s *b, nuvec_s *diff);

// FUNCTION: LEGOBATMAN 0x00451870
f32 Condition_TakeOverRange(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                            AIPACKET_s *packet, char *str, void *argument) {
  if (packet != NULL && packet->pd0 != NULL && packet->pd0->obj != NULL) {
    // 0x13bc: the take-over target.
    GameObject_s *target = *(GameObject_s **)((u8 *)packet->pd0->obj + 0x13bc);
    if (target != NULL)
      return NuVecDist(&target->position, &packet->pd0->pos5c, NULL);
  }
  return 1e9f;
}

struct AILocatorSetIds_s {
  u8 pad0[0x10];
  i8 count; // 0x10
  u8 pad11[0x14 - 0x11];
  u8 *ids; // 0x14
};

struct AILocator44_s {
  u8 pad0[0x44];
};

// FUNCTION: LEGOBATMAN 0x00451640
f32 Condition_LocatorIsFirstInSet(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                  AIPACKET_s *packet, char *str,
                                  void *argument) {
  // process +0xa8/+0xac: current locator and set; sys +0x234: locators.
#define PROC_LOCATOR(p) (*(AILocator44_s **)((u8 *)(p) + 0xa8))
#define PROC_SET(p) (*(AILocatorSetIds_s **)((u8 *)(p) + 0xac))
  if (PROC_LOCATOR(process) != NULL && PROC_SET(process) != NULL &&
      PROC_LOCATOR(process) ==
          &(*(AILocator44_s **)((u8 *)sys + 0x234))[PROC_SET(process)->ids[0]])
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x004515d0
f32 Condition_GotLocatorInSet(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                              AIPACKET_s *packet, char *str, void *argument) {
  AILocatorSetIds_s *set = (AILocatorSetIds_s *)argument;
  if (set != NULL) {
    AILocator44_s *locator = *(AILocator44_s **)((u8 *)process + 0xa8);
    if (locator != NULL) {
      u8 index = locator - *(AILocator44_s **)((u8 *)sys + 0x234);
      for (i32 i = 0; i < set->count; i++) {
        if (index == set->ids[i])
          return 1.0f;
      }
    }
  }
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x00452c00
f32 Condition_CheckCoupled(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                           AIPACKET_s *packet, char *str, void *argument) {
  GameObject_s *object = NULL;
  if (packet != NULL && packet->pd0 != NULL && packet->pd0->obj != NULL)
    object = packet->pd0->obj;
  if (str != NULL) {
    Unk_AIPacketObj *api;
    if (GetNamedAPIObjectFn == NULL ||
        (api = GetNamedAPIObjectFn(sys, str)) == NULL)
      return 0.0f;
    object = api->obj;
  }
  // 0x1160: coupled partner.
  if (object != NULL && *(void **)((u8 *)object + 0x1160) != NULL)
    return 1.0f;
  return 0.0f;
}
