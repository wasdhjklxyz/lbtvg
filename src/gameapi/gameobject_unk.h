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
  u8 pad2[0x2c8 - 0x208];
  i32 i2c8; // 0x2c8
};

struct Unk_GameObject50 {
  u8 pad0[8];
  Unk_GameObject50_08 *p08; // 0x08
  Unk_GameObject50_0c *p0c; // 0x0c
};

struct Unk_GameObject54_24 {
  u8 pad0[0xbc];
  f32 fbc; // 0xbc, GameShadow: 0 = ignore hover layer
  u8 pad0c0[0x148 - 0xc0];
  u32 flags148; // 0x148
  u8 pad1[0x1d6 - 0x14c];
  i16 s1d6; // 0x1d6
  u8 pad2[4];
  u8 b1dc[1]; // 0x1dc
};

struct Unk_GameObject54 {
  u8 pad0[4];
  u32 model_flags; // 0x04
  u8 pad8[0xc - 8];
  char *file; // 0x0c
  u8 pad10[0x24 - 0x10];
  Unk_GameObject54_24 *p24; // 0x24
};

struct Unk_GameObject112c {
  nupad_s *pad0; // 0x00
  u8 pad1[0x28 - 4];
  f32 f28; // 0x28
  u8 pad2c[0x5a - 0x2c];
  u16 flags5a; // 0x5a
};

// FaceOpponent: aim point at +0x58.
struct BlowupTarget_s {
  u8 pad0[0x58];
  nuvec_s mid_position; // 0x58
};

struct NuDynamicLight;

// FreeGameObjectLights: two 0x2c-byte entries at +0x129c, light first.
struct GameObjectLight_s {
  NuDynamicLight *light;
  u8 pad4[0x2c - 4];
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
  u8 pad2[0x1dc - 0x8c];
  f32 radius; // 0x1dc, PartyMemberInWay: sphere radius - 0.125
  u8 pad1e0[0x1f8 - 0x1e0];
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
  u8 pad7[0x480 - 0x3cf];
  u32 flags480_lo : 26;
  u32 move_range_type : 2; // 0x480 bits 26-27
  u32 flags480_hi : 4;
  u8 pad7a[0x488 - 0x484];
  f32 move_range; // 0x488
  u8 pad7b[0x988 - 0x48c];
  f32 f988; // 0x988
  f32 f98c; // 0x98c
  u8 pad8[0x998 - 0x990];
  f32 f998; // 0x998
  u8 pad9[0x9a8 - 0x99c];
  GameObject_s *force_target;    // 0x9a8
  BlowupTarget_s *blowup_target; // 0x9ac
  u8 pad9b[0x9bc - 0x9b0];
  struct TECHNO_s *techno; // 0x9bc
  u8 pad9c[0x9d0 - 0x9c0];
  i16 s9d0; // 0x9d0
  i16 s9d2; // 0x9d2
  u8 pad10[0x9d8 - 0x9d4];
  u8 b9d8;   // 0x9d8
  u8 b9d9;   // 0x9d9
  u8 b9da;   // 0x9da
  char b9db; // 0x9db
  u8 pad11[0x9df - 0x9dc];
  u8 b9df; // 0x9df
  u8 b9e0; // 0x9e0
  u8 pad9e1[0x9e8 - 0x9e1];
  f32 f9e8;  // 0x9e8
  char b9ec; // 0x9ec
  u8 pad12[0x112c - 0x9ed];
  Unk_GameObject112c *p112c; // 0x112c
  u8 pad13[0x114c - 0x1130];
  struct TORPEDOPACKET_s *torpedo; // 0x114c
  u8 pad13a[0x1158 - 0x1150];
  GameObject_s *p1158; // 0x1158
  u8 pad13b[0x11b0 - 0x115c];
  i32 i11b0; // 0x11b0
  u8 pad11b4[0x11bc - 0x11b4];
  i32 i11bc; // 0x11bc
  u8 pad11c0[0x11c4 - 0x11c0];
  f32 f11c4; // 0x11c4
  u8 pad11c8[0x11cc - 0x11c8];
  f32 f11cc;             // 0x11cc
  f32 f11d0;             // 0x11d0
  f32 f11d4;             // 0x11d4
  f32 weapon_scale;      // 0x11d8
  f32 weapon_scale_rate; // 0x11dc
  u8 pad11e0[0x11f0 - 0x11e0];
  f32 f11f0; // 0x11f0
  u8 pad11f4[0x1204 - 0x11f4];
  f32 f1204; // 0x1204
  f32 f1208; // 0x1208
  u8 pad120c[0x1228 - 0x120c];
  f32 f1228; // 0x1228
  u8 pad122c[0x123c - 0x122c];
  f32 f123c; // 0x123c
  u8 pad1240[0x1250 - 0x1240];
  f32 f1250; // 0x1250
  u8 pad1254[0x1264 - 0x1254];
  f32 f1264; // 0x1264
  u8 pad1268[0x1270 - 0x1268];
  f32 f1270; // 0x1270
  f32 f1274; // 0x1274
  u8 pad14a[0x1298 - 0x1278];
  i32 dynamic_light_id;        // 0x1298
  GameObjectLight_s lights[2]; // 0x129c
  u8 pad14a2[0x12fc - 0x12f4];
  i16 s12fc; // 0x12fc
  u8 pad12fe[0x1300 - 0x12fe];
  i16 s1300; // 0x1300
  i16 s1302; // 0x1302
  u8 pad1304[0x130c - 0x1304];
  u32 flags130c; // 0x130c
  u32 flags1310; // 0x1310
  u8 pad14b[0x131c - 0x1314];
  u8 b131c;              // 0x131c
  u8 weapon_scale_state; // 0x131d
  u8 pad131e[0x132b - 0x131e];
  u8 b132b; // 0x132b
  u8 pad14c[0x135c - 0x132c];
  f32 f135c; // 0x135c
  u8 pad14d[0x140c - 0x1360];
  u32 flags140c; // 0x140c
  u32 flags1410_lo : 15;
  u32 doomed_take_damage : 1; // 0x1410 bit 15
  u32 flags1410_hi : 16;
  u32 flags1414; // 0x1414
  u32 flags1418; // 0x1418
  u8 pad15[0x1430 - 0x141c];
  u32 flags1430; // 0x1430
  u8 pad15b[0x143c - 0x1434];
  struct AILOCATOR_s *doomed_escape_locator; // 0x143c
  u8 pad16[0x1534 - 0x1440];
  f32 f1534; // 0x1534
  u8 pad17[0x154c - 0x1538];
  f32 flicker_time; // 0x154c
  u8 pad17b[0x157c - 0x1550];
  CABLE_s *cable157c; // 0x157c
  u8 pad18[0x15b0 - 0x1580];
  i16 type15b0; // 0x15b0
  u8 pad15b2[0x15bc - 0x15b2];
  i16 platform_id; // 0x15bc, -1 = none (PlatOnOff index)
  u8 pad19[0x162c - 0x15be];
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
