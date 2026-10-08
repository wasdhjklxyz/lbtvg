#pragma once
// WORLDINFO_s: only fields with evidence.

#include "../gameapi/gameobject_unk.h"

struct nugscn_s;
struct nuhspecial_s;
struct GIZMOSYS_s;
struct GIZMO_s;
struct GIZMOBLOWUP_s;
struct GIZAIMESSAGESYS_s;
struct GIZAIMESSAGE_s;
struct AISYS_s;
struct AILOCATOR_s;
struct GAMECAMERA_s;

// 0x2c-byte entries of the list at WORLDINFO_s+0x5220.
struct Unk_WorldInfo5220Entry {
  u8 pad0[0x17];
  u8 b17; // 0x17
  u8 b18; // 0x18
  u8 pad1[0x24 - 0x19];
  u8 b24; // 0x24
  u8 pad2[0x2c - 0x25];
};

struct Unk_WorldInfo5220 {
  Unk_WorldInfo5220Entry *list; // 0x00
  u8 pad0[4];
  i32 count; // 0x08
};

struct Unk_WorldInfo2b04 {
  u8 pad0[0xd7e];
  u8 bd7e; // 0xd7e
  u8 pad1[0xf];
  u8 bd8e; // 0xd8e
  u8 pad2[0xf];
  u8 bd9e; // 0xd9e
};

struct WORLDINFO_s {
  u8 pad0[0x104];
  variptr_u buf104;    // 0x104
  variptr_u bufEnd108; // 0x108
  u8 pad1[0x138 - 0x10c];
  void *p138; // 0x138
  u8 pad2[0x140 - 0x13c];
  nugscn_s *scn140; // 0x140
  u8 pad3[0x148 - 0x144];
  nugscn_s *scn148; // 0x148
  u8 pad4[0x2974 - 0x14c];
  i32 i2974; // 0x2974
  u8 pad5[0x2b04 - 0x2978];
  Unk_WorldInfo2b04 *p2b04; // 0x2b04
  u8 pad6[0x2b0c - 0x2b08];
  GIZMOSYS_s *gizmoSys2b0c; // 0x2b0c
  u8 pad7[0x2bf8 - 0x2b10];
  AISYS_s *aiSys2bf8; // 0x2bf8
  u8 pad8[0x5220 - 0x2bfc];
  Unk_WorldInfo5220 *p5220; // 0x5220
};

// GLOBAL: LEGOBATMAN 0x00960894
extern WORLDINFO_s *g_unk00960894;
// GLOBAL: LEGOBATMAN 0x0095f624
extern GAMECAMERA_s *g_unk0095f624;
// GLOBAL: LEGOBATMAN 0x0095eb8c
extern i32 g_unk0095eb8c; // language
// GLOBAL: LEGOBATMAN 0x00a96070
extern i32 g_unk00a96070;

i32 NuSpecialFind(nugscn_s *scene, nuhspecial_s *dest, char *name, i32 flags);
nugscn_s *NuGScnRead(variptr_u *buf, variptr_u buf_end, char *path);
GIZMO_s *GizmoFindByName(GIZMOSYS_s *sys, i32 type_id, char *name);
GIZMOBLOWUP_s *GizmoBlowUp_FindByName(WORLDINFO_s *wi, char *name);
GIZAIMESSAGE_s *CheckGizAIMessage(GIZAIMESSAGESYS_s *sys, char const *name,
                                  GIZAIMESSAGE_s *out);
AILOCATOR_s *AIPathFindLocator(AISYS_s *aisys, char *name);
WORLDINFO_s *WorldInfo_CurrentlyActive(void);
