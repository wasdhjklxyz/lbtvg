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
