// batman/, file unknown: AI script Condition_* parsers
// (0x0044da30..0x00452120).

#include "../gameapi/ai/aisys_unk.h"
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
