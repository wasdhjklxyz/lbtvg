#pragma once
// GameObject_s: only fields with evidence; size 0x1648 from the stride of the
// object array walked in Condition_CharacterTypeExists.

#include "../nu2api/numath/nuvec.h"

struct CABLE_s;
struct nupad_s;

struct Unk_GameObject50_08_204 {
  u8 pad0[4];
  u32 flags4; // 0x04
};

struct Unk_GameObject50_08 {
  u8 pad0[0x204];
  Unk_GameObject50_08_204 *p204; // 0x204
};

struct Unk_GameObject50_0c {
  u8 pad0[0x104];
  void *p104; // 0x104
  u8 pad1[0x204 - 0x108];
  void *p204; // 0x204
};

struct Unk_GameObject50 {
  u8 pad0[8];
  Unk_GameObject50_08 *p08; // 0x08
  Unk_GameObject50_0c *p0c; // 0x0c
};

struct Unk_GameObject54_24 {
  u8 pad0[0x148];
  u32 flags148; // 0x148
  u8 pad1[0x1d6 - 0x14c];
  i16 s1d6; // 0x1d6
  u8 pad2[4];
  u8 b1dc[1]; // 0x1dc
};

struct Unk_GameObject54 {
  u8 pad0[0x24];
  Unk_GameObject54_24 *p24; // 0x24
};

struct Unk_GameObject112c {
  nupad_s *pad0; // 0x00
  u8 pad1[0x28 - 4];
  f32 f28; // 0x28
};

// FaceOpponent: aim point at +0x58.
struct BlowupTarget_s {
  u8 pad0[0x58];
  nuvec_s mid_position; // 0x58
};

struct GameObject_s {
  u8 pad0[0x50];
  Unk_GameObject50 *p50; // 0x50
  Unk_GameObject54 *p54; // 0x54
  u8 pad1[0x5a - 0x58];
  i16 facing_angle; // 0x5a
  nuvec_s position; // 0x5c
  u8 pad1b[0x80 - 0x68];
  nuvec_s v80; // 0x80
  u8 pad2[0x1f8 - 0x8c];
  u32 flags1f8; // 0x1f8
  u32 flags1fc; // 0x1fc
  u8 pad3[0x24c - 0x200];
  char b24c; // 0x24c
  u8 pad4[0x257 - 0x24d];
  char b257; // 0x257
  u8 pad5[0x259 - 0x258];
  u8 b259; // 0x259
  u8 pad5b[0x290 - 0x25a];
  u8 process290[0x3c8 - 0x290]; // 0x290  AISCRIPTPROCESS_s
  u8 b3c8;                      // 0x3c8
  u8 pad6[0x3ce - 0x3c9];
  u8 b3ce; // 0x3ce
  u8 pad7[0x988 - 0x3cf];
  f32 f988; // 0x988
  f32 f98c; // 0x98c
  u8 pad8[0x998 - 0x990];
  f32 f998; // 0x998
  u8 pad9[0x9a8 - 0x99c];
  GameObject_s *force_target;    // 0x9a8
  BlowupTarget_s *blowup_target; // 0x9ac
  u8 pad9b[0x9d0 - 0x9b0];
  i16 s9d0; // 0x9d0
  i16 s9d2; // 0x9d2
  u8 pad10[0x9d9 - 0x9d4];
  u8 b9d9;   // 0x9d9
  u8 b9da;   // 0x9da
  char b9db; // 0x9db
  u8 pad11[0x9df - 0x9dc];
  u8 b9df; // 0x9df
  u8 pad12[0x112c - 0x9e0];
  Unk_GameObject112c *p112c; // 0x112c
  u8 pad13[0x11cc - 0x1130];
  f32 f11cc; // 0x11cc
  u8 pad14[0x11d8 - 0x11d0];
  f32 weapon_scale; // 0x11d8
  u8 pad14a[0x130c - 0x11dc];
  u32 flags130c; // 0x130c
  u8 pad14b[0x131d - 0x1310];
  u8 weapon_scale_state; // 0x131d
  u8 pad14c[0x135c - 0x131e];
  f32 f135c; // 0x135c
  u8 pad14d[0x1414 - 0x1360];
  u32 flags1414; // 0x1414
  u8 pad15[0x1430 - 0x1418];
  u32 flags1430; // 0x1430
  u8 pad16[0x1534 - 0x1434];
  f32 f1534; // 0x1534
  u8 pad17[0x157c - 0x1538];
  CABLE_s *cable157c; // 0x157c
  u8 pad18[0x15b0 - 0x1580];
  i16 type15b0; // 0x15b0
  u8 pad19[0x162c - 0x15b2];
  i16 s162c; // 0x162c
  u8 pad20[0x1648 - 0x162e];
};

// PART_s: +0x30 position, +0x80 vector passed to NuVecMag, +0xd4 owner.
struct PART_s {
  u8 pad0[0x30];
  nuvec_s pos; // 0x30
  u8 pad1[0x80 - 0x3c];
  nuvec_s v80; // 0x80
  u8 pad2[0xd4 - 0x8c];
  GameObject_s *objd4; // 0xd4
};

// GLOBAL: LEGOBATMAN 0x00ab364c
extern GameObject_s *Obj;
// GLOBAL: LEGOBATMAN 0x00ab3648
extern i32 HIGHGAMEOBJECT;
