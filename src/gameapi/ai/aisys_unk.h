#pragma once
// AI script system types; only fields with evidence.

#include "../gameobject_unk.h"

struct AISYS_s;
struct AISCRIPT_s;
struct AISCRIPTPROCESS_s;

// Objects hung off AIPACKET_s at +0xd0/+0xd4/+0xe4: a GameObject_s* first.
struct Unk_AIPacketObj {
  GameObject_s *obj; // 0x00
  u8 pad0[0x5c - 4];
  u8 unk5c[1]; // 0x5c
};

struct AIPACKET_s {
  u8 pad0[0xd0];
  Unk_AIPacketObj *pd0; // 0xd0
  Unk_AIPacketObj *pd4; // 0xd4
  u8 pad1[0xe4 - 0xd8];
  Unk_AIPacketObj *pe4; // 0xe4
};

f32 AIParamToFloatEx(AIPACKET_s *packet, AISCRIPTPROCESS_s *process, char *str);
