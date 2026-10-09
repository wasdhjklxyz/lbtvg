// batman/, file unknown: AI script Condition_* parsers
// (0x0044da30..0x00452120).

#include "../gameapi/ai/aisys_unk.h"
#include "../nu2api/nucore/nustring.h"
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
