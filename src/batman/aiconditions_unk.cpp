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

// AISYS_s is opaque here; only player_1 is evidenced.
struct AISysPlayer1_s {
  u8 pad0[0x1698];
  GameObject_s *player_1; // 0x1698
};

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
  u8 pad0[0x84];
  u8 index; // 0x84
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
