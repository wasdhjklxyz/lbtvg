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
  u8 pad10[4];
  void *locator_present[1]; // 0x14, indexed by locator
};

struct Unk_GameObject54_24 {
  u8 pad0[0xa8];
  f32 tiptoe_speed; // 0xa8
  f32 walk_speed;   // 0xac
  f32 run_speed;    // 0xb0
  u8 padb4[0xbc - 0xb4];
  f32 fbc; // 0xbc, GameShadow: 0 = ignore hover layer
  u8 pad0c0[0x13c - 0xc0];
  u32 flags13c; // 0x13c, ability bits (Condition_HasAbility)
  u8 pad140[0x144 - 0x140];
  u32 flags144; // 0x144
  u32 flags148; // 0x148
  u32 flags14c; // 0x14c, 0x10000000 = Condition_GotGun override
  u8 b150;      // 0x150, bit 1: Player_HasFastBuild
  u8 pad1[0x1d6 - 0x151];
  i16 s1d6; // 0x1d6
  u8 pad2[4];
  u8 b1dc[1]; // 0x1dc
  u8 pad1dd[0x217 - 0x1dd];
  i8 hat_locator2; // 0x217, LoseHat fallback locator
  i8 hat_locator;  // 0x218
  u8 pad219[0x227 - 0x219];
  u8 shield_hitpoints; // 0x227
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
  u8 pad1[8 - 4];
  u32 buttons8; // 0x08, EngageBlowup ORs the fire button in
  u8 pad0c[0x26 - 0xc];
  u16 input_angle; // 0x26
  f32 f28;         // 0x28, input magnitude
  u8 pad2c[0x54 - 0x2c];
  void *operator_data; // 0x54
  u8 pad58[0x5a - 0x58];
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
  u8 pad0[4];
  struct Unk_GameObject4 *p4; // 0x04
  u8 pad08[0x50 - 8];
  Unk_GameObject50 *p50; // 0x50
  Unk_GameObject54 *p54; // 0x54
  u16 yaw58;             // 0x58, Action_SetCurrentSpeed rotates velocity by it
  i16 facing_angle;      // 0x5a
  nuvec_s position;      // 0x5c
  nuvec_s velocity;      // 0x68
  u8 pad1b[0x80 - 0x74];
  nuvec_s v80; // 0x80
  nuvec_s v8c; // 0x8c, SnapToOrigin: copy of v80
  nuvec_s v98; // 0x98, SnapToOrigin: copy of position
  u8 pad2[0x1dc - 0xa4];
  f32 radius; // 0x1dc, PartyMemberInWay: sphere radius - 0.125
  u8 pad1e0[0x1e8 - 0x1e0];
  unsigned __int64 collide_mask; // 0x1e8, CollidingWithObject: what I am
  unsigned __int64 collide_with; // 0x1f0, CollidingWithObject: what I touch
  u32 flags1f8;                  // 0x1f8
  union {
    u32 flags1fc; // 0x1fc
    struct {
      u32 : 1;
      u32 dont_push : 1; // 0x1fc bit 1
    };
    struct {
      u32 : 8;
      u32 ai_override_control : 1; // 0x1fc bit 8
    };
    u8 b1fc; // 0x1fc low byte; bit 7 = active player (PlayerIdToGameObj)
  };
  u8 pad3[0x246 - 0x200];
  u16 u246; // 0x246, MovingBackwards: facing compared with pad input
  u8 pad248[0x24c - 0x248];
  char b24c; // 0x24c
  i8 b24d;   // 0x24d, AISysGetCharacterPathPos last argument
  u8 pad24e[0x24f - 0x24e];
  u8 b24f; // 0x24f, 9 = in swamp (Condition_InSwamp)
  u8 pad250[0x251 - 0x250];
  i8 surface; // 0x251, TerSurface index (Hub_CanStartMenu)
  u8 pad4[0x257 - 0x252];
  char b257; // 0x257
  u8 pad5[0x259 - 0x258];
  u8 b259; // 0x259
  u8 pad5b[0x278 - 0x25a];
  unsigned __int64 area_mask; // 0x278, bit per AI trigger area
  u8 pad280[0x290 - 0x280];
  union {
    u8 process290[0x3c8 - 0x290]; // 0x290  AISCRIPTPROCESS_s
    struct {
      u8 pad290[0x3b4 - 0x290];
      i16 s3b4; // 0x3b4, ResetPlayerAI: -1
      i16 s3b6; // 0x3b6, ResetPlayerAI: -1
      i16 s3b8; // 0x3b8, ResetPlayerAI: -1
    };
  };
  u8 b3c8; // 0x3c8
  u8 pad6[0x3cc - 0x3c9];
  u8 b3cc;               // 0x3cc, Action_UpdatePathInfo reset_route: 0xff
  u8 b3cd;               // 0x3cd, Action_UpdatePathInfo reset_route: 0
  u8 b3ce;               // 0x3ce
  u8 goal_speed_mode;    // 0x3cf, its packet's goal_speed_mode
  u8 control_rotational; // 0x3d0, Action_SetControlSystem
  u8 pad7[0x3f6 - 0x3d1];
  u16 path_flags3f6; // 0x3f6, bit 0: on path (SnapToOrigin clears)
  u8 pad3f8[0x418 - 0x3f8];
  u32 u418; // 0x418, ResetPlayerAI: 0
  u8 pad41c[0x424 - 0x41c];
  void *p424; // 0x424, ResetPlayerAI: NULL
  void *p428; // 0x428, ResetPlayerAI: NULL
  u8 pad42c[0x47c - 0x42c];
  nuvec_s *look_target; // 0x47c, AI look target (process290 + 0x1ec)
  union {
    struct {
      u32 flags480_lo : 26;
      u32 move_range_type : 2; // 0x480 bits 26-27
      u32 flags480_hi : 4;
    };
    struct {
      u32 : 18;
      u32 zero_acceleration : 1; // 0x480 bit 18
    };
    struct {
      u32 : 5;
      u32 b480_5 : 1; // 0x480 bits 5, 6, 9: ResetPlayerAI clears them
      u32 b480_6 : 1;
      u32 : 2;
      u32 b480_9 : 1;
    };
  };
  u8 pad7a[0x488 - 0x484];
  f32 move_range;   // 0x488
  u32 capabilities; // 0x48c, Action_SetCapability
  u32 u490;         // 0x490, ResetPlayerAI: 0
  u8 pad494[0x870 - 0x494];
  u8 sock_pos870;   // 0x870, ComplexSockPosition out
  char sock_id;     // 0x871, -1 = none
  i16 sock_segment; // 0x872
  u8 pad874[0x878 - 0x874];
  nuvec_s sock_mid; // 0x878, ComplexSockPosition midpoint
  u8 pad884[0x896 - 0x884];
  u16 sock_mid_rot_y; // 0x896
  u8 pad898[0x89c - 0x898];
  f32 sock_distance; // 0x89c, distance along the current sock
  u8 pad8a0[0x988 - 0x8a0];
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
  u8 pad11[0x9de - 0x9dc];
  char special_move; // 0x9de, -1 = none
  u8 b9df;           // 0x9df
  u8 b9e0;           // 0x9e0
  u8 pad9e1[0x9e4 - 0x9e1];
  union {
    u32 flags9e4; // 0x9e4
    struct {
      u32 : 7;
      u32 facing_reversed : 1; // 0x9e4 bit 7, ObjOpponentStillThere
    };
  };
  f32 f9e8;  // 0x9e8
  char b9ec; // 0x9ec
  u8 b9ed_lo : 1;
  u8 tag_disabled : 1; // 0x9ed bit 1, Action_SetTaggable
  u8 b9ed_hi : 6;
  u8 pad9ee[0x9f4 - 0x9ee];
  u8 sock_angles[4]; // 0x9f4, ComplexSockAngles out
  u8 pad12[0xb38 - 0x9f8];
  unsigned __int64
      carried_item_flags; // 0xb38, PlayerItems_GetAllCarriedItemFlags
  u8 pad12b[0xb48 - 0xb40];
  unsigned __int64 ub48; // 0xb48, Condition_GotGun
  u8 padb50[0xb88 - 0xb50];
  struct SCOREMULTIPLIER_s *score_multiplier; // 0xb88
  u8 pad12c[0xb98 - 0xb8c];
  struct {
    f32 m[16];
  } locator_mtx[22]; // 0xb98
  u8 pad1118[0x112c - 0x1118];
  Unk_GameObject112c *p112c; // 0x112c
  u8 pad13[0x1144 - 0x1130];
  struct Unk_GameObject1144
      *p1144; // 0x1144, byte 0x14 bit 2: Player_HasDeflectBolts
  u8 pad1148[0x114c - 0x1148];
  struct TORPEDOPACKET_s *torpedo; // 0x114c
  u8 pad13a[0x1158 - 0x1150];
  GameObject_s *p1158;           // 0x1158
  GameObject_s *last_takeover;   // 0x115c, Action_TakeOver "last"
  GameObject_s *coupled_trailer; // 0x1160, Action_UncoupleVehicles
  u8 pad13b[0x11b0 - 0x1164];
  i32 i11b0; // 0x11b0
  u8 pad11b4[0x11bc - 0x11b4];
  i32 i11bc; // 0x11bc
  f32 f11c0; // 0x11c0
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
  f32 f1254; // 0x1254, Action_SetCurrentSpeed sets 1 for RUN
  u8 pad1258[0x1264 - 0x1258];
  f32 f1264; // 0x1264
  u8 pad1268[0x1270 - 0x1268];
  f32 f1270; // 0x1270
  f32 f1274; // 0x1274
  f32 f1278; // 0x1278, > 0 grants every Player_Has* power
  u8 pad14a[0x1298 - 0x127c];
  i32 dynamic_light_id;        // 0x1298
  GameObjectLight_s lights[2]; // 0x129c
  u8 pad14a2[0x12fc - 0x12f4];
  i16 s12fc; // 0x12fc
  u8 pad12fe[0x1300 - 0x12fe];
  i16 s1300; // 0x1300
  i16 s1302; // 0x1302
  u8 pad1304[0x130a - 0x1304];
  u16 looping_sfx; // 0x130a
  u32 flags130c;   // 0x130c
  u32 flags1310;   // 0x1310
  u8 pad14b[0x131c - 0x1314];
  u8 b131c;              // 0x131c
  u8 weapon_scale_state; // 0x131d
  u8 pad131e[0x1321 - 0x131e];
  u8 shield_hitpoints; // 0x1321
  u8 shield_state;     // 0x1322, ResetAIOverrideCharacter: 4
  u8 pad1323[0x132b - 0x1323];
  u8 b132b; // 0x132b
  u8 pad14c[0x135c - 0x132c];
  f32 f135c; // 0x135c
  u8 pad1360[0x1364 - 0x1360];
  nuvec_s aim_target; // 0x1364, EngageBlowup
  u8 pad1370[0x137c - 0x1370];
  void *movement_spline; // 0x137c
  u8 pad1380[0x1383 - 0x1380];
  u8 movement_spline_finished; // 0x1383
  u8 pad14d[0x13bc - 0x1384];
  GameObject_s *takeover_target; // 0x13bc
  u8 pad13c0[0x13e0 - 0x13c0];
  f32 current_speed_mul; // 0x13e0
  u8 pad13e4[0x13f0 - 0x13e4];
  f32 walk_speed_override; // 0x13f0
  u8 pad13f4[0x1404 - 0x13f4];
  f32 max_y_rot_seek;      // 0x1404
  u32 ignore_trigger_sets; // 0x1408, bit per trigger set 1..31
  union {
    u32 flags140c; // 0x140c
    struct {
      u32 flags140c_lo : 24;
      u32 respawnable : 1;       // 0x140c bit 24
      u32 respawn_at_origin : 1; // 0x140c bit 25
    };
    struct {
      u32 : 21;
      u32 predictive_aim : 1; // 0x140c bit 21, EngageBlowup "predictive"
    };
    struct {
      u32 : 29;
      u32 catch_up_forbidden : 1; // 0x140c bit 29
    };
    struct {
      u32 : 30;
      u32 cannot_drop_in : 1; // 0x140c bit 30
    };
    struct {
      u32 : 8;
      u32 keep_weapon_out : 1; // 0x140c bit 8
    };
    struct {
      u32 : 13;
      u32 can_attack : 1; // 0x140c bit 13
    };
  };
  union {
    struct {
      u32 flags1410_lo : 15;
      u32 doomed_take_damage : 1; // 0x1410 bit 15
      u32 flags1410_hi : 16;
    };
    struct {
      u32 : 9;
      u32 dont_set_stopped : 1; // 0x1410 bit 9
    };
    struct {
      u32 : 28;
      u32 not_with_party : 1; // 0x1410 bit 28
    };
    struct {
      u32 : 25;
      u32 b1410_25 : 1; // 0x1410 bit 25, ResetPlayerAI clears it
    };
    struct {
      u32 : 8;
      u32 dont_move : 1; // 0x1410 bit 8
    };
  };
  union {
    u32 flags1414; // 0x1414
    struct {
      u32 flags1414_lo : 25;
      u32 item_ignore_los : 1; // 0x1414 bit 25
    };
    struct {
      u32 : 15;
      u32 spline_follow_terrain : 1; // 0x1414 bit 15
    };
    struct {
      u32 : 13;
      u32 ignore_slide_terrain : 1; // 0x1414 bit 13
    };
    struct {
      u32 : 10;
      u32 awkward_shape_override : 1; // 0x1414 bit 10
    };
    struct {
      u32 : 27;
      u32 no_idle_speed : 1; // 0x1414 bit 27
    };
    struct {
      u32 : 8;
      u32 no_time_based_update : 1; // 0x1414 bit 8
    };
    struct {
      u32 : 4;
      u32 can_be_carried : 1; // 0x1414 bit 4
    };
    struct {
      u32 : 26;
      u32 ignore_last_safe_path_pos : 1; // 0x1414 bit 26
    };
    struct {
      u32 : 16;
      u32 use_one_at_once : 1; // 0x1414 bit 16
    };
  };
  union {
    u32 flags1418; // 0x1418
    struct {
      u32 flags1418_lo : 13;
      u32 can_be_mind_controlled : 1; // 0x1418 bit 13
    };
    struct {
      u32 : 22;
      u32 whip_disabled : 1; // 0x1418 bit 22
    };
    struct {
      u32 : 8;
      u32 ignore_phobia : 1; // 0x1418 bit 8
    };
    struct {
      u32 : 7;
      u32 dont_target_others_opponent : 1; // 0x1418 bit 7
    };
    struct {
      u32 : 10;
      u32 deflect_bolts : 1; // 0x1418 bit 10
    };
    struct {
      u32 : 3;
      u32 process_when_deactivated : 1; // 0x1418 bit 3
    };
    struct {
      u32 : 17;
      u32 woozy : 1; // 0x1418 bit 17
    };
    struct {
      u32 : 9;
      u32 can_be_targetted_by_cable : 1; // 0x1418 bit 9
    };
  };
  u32 u141c; // 0x141c, ResetPlayerAI: 0
  u32 u1420; // 0x1420
  u32 u1424; // 0x1424
  u32 u1428; // 0x1428
  u8 pad142c[0x1430 - 0x142c];
  u32 flags1430; // 0x1430
  u8 pad15b[0x143c - 0x1434];
  struct AILOCATOR_s *doomed_escape_locator; // 0x143c
  u8 pad16[0x1534 - 0x1440];
  f32 f1534; // 0x1534
  u8 pad17[0x153c - 0x1538];
  f32 f153c; // 0x153c, ResetAIOverrideCharacter: 0
  u8 pad1540[0x1544 - 0x1540];
  f32 f1544; // 0x1544, ResetAIOverrideCharacter: 0
  u8 pad1548[0x154c - 0x1548];
  f32 flicker_time; // 0x154c
  u8 pad17b[0x155c - 0x1550];
  f32 hover_height_override; // 0x155c
  u8 pad1560[0x1568 - 0x1560];
  f32 anim_speed_mul; // 0x1568
  u8 pad156c[0x1574 - 0x156c];
  void *hat; // 0x1574, LoseHat clears it
  u8 pad1578[0x157c - 0x1578];
  CABLE_s *cable157c; // 0x157c
  u32 flags1580;      // 0x1580, 4 = can shoot off screen
  u8 pad18[0x15b0 - 0x1584];
  i16 type15b0; // 0x15b0
  u8 pad15b2[0x15bc - 0x15b2];
  i16 platform_id; // 0x15bc, -1 = none (PlatOnOff index)
  u8 pad15be[0x15bf - 0x15be];
  u8 dont_draw_frames; // 0x15bf
  u8 pad15c0[0x15c6 - 0x15c0];
  u8 hitpoints;  // 0x15c6, max
  i8 current_hp; // 0x15c7
  u8 pad15c8[0x15ca - 0x15c8];
  u8 b15ca; // 0x15ca, ResetPlayerAI: 0
  u8 b15cb; // 0x15cb
  u8 b15cc; // 0x15cc
  u8 pad15cd[0x15d0 - 0x15cd];
  u32 u15d0; // 0x15d0
  f32 f15d4; // 0x15d4
  u8 pad15d8[0x15ec - 0x15d8];
  GameObject_s *last_attacker; // 0x15ec, Condition_BeenHitBy
  u8 pad15f0[0x1608 - 0x15f0];
  nuvec_s saved_position; // 0x1608
  u8 pad1614[0x1618 - 0x1614];
  struct GIZFORCE_s *gizforce_target; // 0x1618
  u8 pad19[0x1628 - 0x161c];
  struct AITRIGGERSET_s *active_trigger_set; // 0x1628, Action_UseTriggerSet
  i16 s162c;                                 // 0x162c
  u8 pad20[0x1648 - 0x162e];
};

// PART_s: +0x30 position, +0x80 vector passed to NuVecMag, +0xd4 owner.
struct PART_s {
  u8 pad0[0x30];
  nuvec_s pos; // 0x30
  u8 pad1[0x80 - 0x3c];
  nuvec_s v80; // 0x80
  u8 pad2[0xa4 - 0x8c];
  nuvec_s impact_position; // 0xa4
  nuvec_s impact_normal;   // 0xb0
  u8 pad2b[0xd4 - 0xbc];
  GameObject_s *objd4; // 0xd4
  u8 padd8[0xe0 - 0xd8];
  f32 radius; // 0xe0
  u8 pade4[0x100 - 0xe4];
  f32 f100; // 0x100, ThermalDetonator: beep while in (0, 1)
  u8 pad104[0x10c - 0x104];
  u32 flags10c; // 0x10c, KillPart: 0x20000 = owns debris
  u8 pad110[0x148 - 0x110];
  u32 flags148;         // 0x148
  void *source_special; // 0x14c
  struct {
    void *scene; // 0x150
    u32 pad[2];
  } special; // 0x150
  u8 pad15c[0x1c4 - 0x15c];
  void (*kill_callback)(struct PART_s *part, i32 reason); // 0x1c4
  u8 pad1c8[0x1d0 - 0x1c8];
  void (*stop_callback)(struct PART_s *part); // 0x1d0
  u8 pad1d4[0x1e0 - 0x1d4];
  void *debris_key;       // 0x1e0
  i32 *lighting_template; // 0x1e4
  u8 pad1e8[0x1fc - 0x1e8];
  i32 kill_effect; // 0x1fc
  u8 pad200[0x20c - 0x200];
  f32 kill_effect_scale; // 0x20c
  u8 pad210[0x216 - 0x210];
  i8 force_player_mask; // 0x216
  u8 pad217[0x219 - 0x217];
  u8 surface219; // 0x219, terrain surface hit (0x1c = kill)
  u8 pad21a[0x22c - 0x21a];
  f32 reflection_height; // 0x22c
  u8 pad230[0x250 - 0x230];
};

// GLOBAL: LEGOBATMAN 0x00ab364c
extern GameObject_s *Obj;
// GLOBAL: LEGOBATMAN 0x00ab3648
extern i32 HIGHGAMEOBJECT;
