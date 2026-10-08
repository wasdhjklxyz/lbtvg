#pragma once
// AI script system types; only fields with evidence.

#include "../gameobject_unk.h"

struct AISYS_s;
struct AISCRIPT_s;
struct AISCRIPTPROCESS_s;

// APIOBJECT_s in ref/saga/src/legoapi/items/base/apiobject.h: a GameObject_s*
// first, position at +0x5c.
struct Unk_AIPacketObj {
  GameObject_s *obj; // 0x00
  u8 pad0[0x5c - 4];
  nuvec_s pos5c; // 0x5c
};

struct AIPACKET_s {
  u8 pad0[0xd0];
  Unk_AIPacketObj *pd0; // 0xd0
  Unk_AIPacketObj *pd4; // 0xd4
  u8 pad1[0xe4 - 0xd8];
  Unk_AIPacketObj *pe4; // 0xe4
};

typedef Unk_AIPacketObj *AIGETNAMEDAPIOBJECT(AISYS_s *sys, char *name);

// GLOBAL: LEGOBATMAN 0x00ad695c
extern AIGETNAMEDAPIOBJECT *GetNamedAPIObjectFn;

f32 AIParamToFloat(AISCRIPTPROCESS_s *process, char *str);
f32 AIParamToFloatEx(AIPACKET_s *packet, AISCRIPTPROCESS_s *process, char *str);
i32 AIScriptSetBaseScriptStateByName(AISCRIPTPROCESS_s *process, char *name);
void AIScriptProcess(AISYS_s *sys, GameObject_s *obj, AISCRIPTPROCESS_s *packet,
                     AISCRIPTPROCESS_s *process, f32 elapsed);
