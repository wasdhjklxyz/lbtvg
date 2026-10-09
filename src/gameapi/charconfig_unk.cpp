// gameapi/charconfig_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nu2api/nucore/common.h"
#include "../nu2api/nucore/nustring.h"

typedef struct nufpar_s {
  unsigned char pad0[0x910];
  char *word_buf; // 0x910
} NUFPAR;

typedef struct CHARCONFIG_RUNTIME_s {
  unsigned char pad0[0xa0];
  f32 stop_speed;       // 0xa0
  f32 idle_speed;       // 0xa4
  f32 tiptoe_speed;     // 0xa8
  f32 walk_speed;       // 0xac
  f32 run_speed;        // 0xb0
  f32 backwards_factor; // 0xb4
  f32 air_gravity;      // 0xb8
  f32 hover_height;     // 0xbc
  f32 jump_speed;       // 0xc0
  unsigned char padc4[0xcc - 0xc4];
  f32 jump_2_speed; // 0xcc
  unsigned char padd0[0xd8 - 0xd0];
  f32 acceleration; // 0xd8
  unsigned char paddc[0xe4 - 0xdc];
  f32 maxheadturn;           // 0xe4
  f32 maxheadtilt;           // 0xe8
  f32 headrotrate;           // 0xec
  f32 cloak_up_angle;        // 0xf0
  f32 cloak_down_angle;      // 0xf4
  f32 viewrange;             // 0xf8
  f32 heardistance;          // 0xfc
  f32 max_viewheight;        // 0x100
  f32 min_viewheight;        // 0x104
  f32 turn_rate_1;           // 0x108
  f32 turn_rate_2;           // 0x10c
  f32 speed_up_time;         // 0x110
  f32 slow_down_time;        // 0x114
  f32 jump_move_speed_scale; // 0x118
  f32 loop_height;           // 0x11c
  unsigned char pad120[0x12c - 0x120];
  f32 die_air_punch_chance; // 0x12c
  f32 banking;              // 0x130
  f32 banking2;             // 0x134
  f32 thrust_draw_scale;    // 0x138
  u32 gcdata_flags;         // 0x13c
  unsigned char pad140[0x144 - 0x140];
  u32 flags144; // 0x144
  u32 flags148; // 0x148
  u32 flags;    // 0x14c
  u32 flags150; // 0x150
  unsigned char pad154[0x16c - 0x154];
  f32 mediumres_swapdist; // 0x16c
  f32 lowres_swapdist;    // 0x170
  u32 shadow_locators;    // 0x174, bit per locator
  u32 thrust_locators;    // 0x178
  unsigned char pad17c[0x180 - 0x17c];
  f32 ai_update_distance[4]; // 0x180
  u8 ai_update_interval[4];  // 0x190
  i16 sfx_misc[6];           // 0x194
  i16 sfx_die;               // 0x1a0
  i16 sfx_hurt;              // 0x1a2
  i16 sfx_doomed;            // 0x1a4
  i16 sfx_grunt;             // 0x1a6
  i16 sfx_engine;            // 0x1a8
  i16 sfx_shoot;             // 0x1aa
  i16 sfx_footstep;          // 0x1ac
  i16 sfx_chatter;           // 0x1ae
  i16 sfx_sabre;             // 0x1b0
  i16 sfx_punch;             // 0x1b2
  i16 sfx_punch_hit;         // 0x1b4
  i16 sfx_beepbeep;          // 0x1b6
  i16 sfx_phobia;            // 0x1b8
  i16 sfx_land_normal;       // 0x1ba
  i16 sfx_land_lunge;        // 0x1bc
  i16 sfx_land_slam;         // 0x1be
  i16 sfx_land_combatroll;   // 0x1c0
  i16 sfx_siren;             // 0x1c2
  i16 bolt_type;             // 0x1c4
  i16 bolt_type_2;           // 0x1c6
  unsigned char pad1c8[0x1cc - 0x1c8];
  i16 default_items[5];       // 0x1cc
  i16 rider_action;           // 0x1d6
  i16 coin_value;             // 0x1d8
  u8 blobshadow_alpha;        // 0x1da
  i8 grapple_gun_locator;     // 0x1db
  i8 defined_locators[0x10];  // 0x1dc
  i8 weapon_locator[4];       // 0x1ec
  i8 weapon_shoot_locator[4]; // 0x1f0
  unsigned char pad1f4[0x204 - 0x1f4];
  i8 hose_locators[4];        // 0x204
  i8 hand_locators[2];        // 0x208
  i8 grapple_locator[2];      // 0x20a
  i8 pivot_locator_1;         // 0x20c
  i8 pivot_locator_2;         // 0x20d
  i8 backpack_locators[2];    // 0x20e
  i8 debris_locators[2];      // 0x210
  i8 rocket_locator;          // 0x212
  i8 shield_locator;          // 0x213
  i8 head_locator;            // 0x214
  i8 collision_locator;       // 0x215
  i8 thingy_locator;          // 0x216
  i8 helmet_locator;          // 0x217
  i8 hat_locator;             // 0x218
  i8 throw_locator;           // 0x219
  i8 ride_locator;            // 0x21a
  i8 poo_locator;             // 0x21b
  i8 boost_locator;           // 0x21c
  i8 head_joint;              // 0x21d
  i8 cloak_joint;             // 0x21e
  i8 cloak_joint_2;           // 0x21f
  i8 place_locator;           // 0x220
  i8 extra_character_locator; // 0x221
  i8 charplatform_locator;    // 0x222
  i8 cable_locator;           // 0x223
  i8 attracto_count_locator;  // 0x224
  i8 attracto_suck_locator;   // 0x225
  u8 hit_points;              // 0x226
  u8 shield_hit_points;       // 0x227
  unsigned char pad228[0x232 - 0x228];
  u8 phobias;       // 0x232
  u8 chatter_delay; // 0x233
  unsigned char pad234[0x236 - 0x234];
  u8 detonator_type; // 0x236
  unsigned char pad237[0x238 - 0x237];
  i16 dance_action; // 0x238
} CHARCONFIG_RUNTIME_s;

typedef struct CHARCONFIG_s {
  CHARCONFIG_RUNTIME_s *runtime;
  unsigned char pad4[0x10 - 4];
  u32 flags10; // 0x10, 8/0x10: turn rate 1/2 set
} CHARCONFIG_s;

i32 NuFParGetWord(NUFPAR *parser);
i32 GetSfxId(char *name);

// GLOBAL: LEGOBATMAN 0x00acb864
extern CHARCONFIG_s charconfig;

i32 NuFParGetInt(NUFPAR *parser);
int NuAToI(const char *s);

static void CC_set_locator(NUFPAR *parser, i8 *locator) {
  if (NuFParGetWord(parser) != 0) {
    i32 value = NuAToI(parser->word_buf);
    if (value >= -1 && value < 20)
      *locator = (i8)value;
  }
}

// FUNCTION: LEGOBATMAN 0x00622f60
void CC_collision_locator(NUFPAR *parser) {
  CC_set_locator(parser, &charconfig.runtime->collision_locator);
}

// FUNCTION: LEGOBATMAN 0x00622fa0
void CC_weapon_locator_1(NUFPAR *parser) {
  CC_set_locator(parser, &charconfig.runtime->weapon_locator[0]);
}

// FUNCTION: LEGOBATMAN 0x00622fe0
void CC_weapon_locator_2(NUFPAR *parser) {
  CC_set_locator(parser, &charconfig.runtime->weapon_locator[1]);
}

// FUNCTION: LEGOBATMAN 0x00623020
void CC_weapon_locator_3(NUFPAR *parser) {
  CC_set_locator(parser, &charconfig.runtime->weapon_locator[2]);
}

// FUNCTION: LEGOBATMAN 0x00623060
void CC_weapon_locator_4(NUFPAR *parser) {
  CC_set_locator(parser, &charconfig.runtime->weapon_locator[3]);
}

// FUNCTION: LEGOBATMAN 0x006230a0
void CC_weapon_shoot_locator_1(NUFPAR *parser) {
  CC_set_locator(parser, &charconfig.runtime->weapon_shoot_locator[0]);
}

// FUNCTION: LEGOBATMAN 0x006230e0
void CC_weapon_shoot_locator_2(NUFPAR *parser) {
  CC_set_locator(parser, &charconfig.runtime->weapon_shoot_locator[1]);
}

// FUNCTION: LEGOBATMAN 0x00623120
void CC_weapon_shoot_locator_3(NUFPAR *parser) {
  CC_set_locator(parser, &charconfig.runtime->weapon_shoot_locator[2]);
}

// FUNCTION: LEGOBATMAN 0x00623160
void CC_weapon_shoot_locator_4(NUFPAR *parser) {
  CC_set_locator(parser, &charconfig.runtime->weapon_shoot_locator[3]);
}

// FUNCTION: LEGOBATMAN 0x006231a0
void CC_shield_locator(NUFPAR *parser) {
  CC_set_locator(parser, &charconfig.runtime->shield_locator);
}

// FUNCTION: LEGOBATMAN 0x006231e0
void CC_throw_locator(NUFPAR *parser) {
  CC_set_locator(parser, &charconfig.runtime->throw_locator);
}

// FUNCTION: LEGOBATMAN 0x00623220
void CC_place_locator(NUFPAR *parser) {
  CC_set_locator(parser, &charconfig.runtime->place_locator);
}

// FUNCTION: LEGOBATMAN 0x00623260
void CC_grapple_gun_locator(NUFPAR *parser) {
  CC_set_locator(parser, &charconfig.runtime->grapple_gun_locator);
}

// FUNCTION: LEGOBATMAN 0x006232a0
void CC_grapple_locator(NUFPAR *parser) {
  CC_set_locator(parser, &charconfig.runtime->grapple_locator[0]);
  CC_set_locator(parser, &charconfig.runtime->grapple_locator[1]);
}

// FUNCTION: LEGOBATMAN 0x00623310
void CC_extra_character_locator(NUFPAR *parser) {
  CC_set_locator(parser, &charconfig.runtime->extra_character_locator);
}

// FUNCTION: LEGOBATMAN 0x00623350
void CC_charplatform_locator(NUFPAR *parser) {
  CC_set_locator(parser, &charconfig.runtime->charplatform_locator);
}

// FUNCTION: LEGOBATMAN 0x00623390
void CC_cable_locator(NUFPAR *parser) {
  CC_set_locator(parser, &charconfig.runtime->cable_locator);
}

// FUNCTION: LEGOBATMAN 0x006233d0
void CC_attracto_count_locator(NUFPAR *parser) {
  CC_set_locator(parser, &charconfig.runtime->attracto_count_locator);
}

// FUNCTION: LEGOBATMAN 0x00623410
void CC_attracto_suck_locator(NUFPAR *parser) {
  CC_set_locator(parser, &charconfig.runtime->attracto_suck_locator);
}

// FUNCTION: LEGOBATMAN 0x00623450
void CC_pivot_locator_1(NUFPAR *parser) {
  CC_set_locator(parser, &charconfig.runtime->pivot_locator_1);
}

// FUNCTION: LEGOBATMAN 0x00623490
void CC_pivot_locator_2(NUFPAR *parser) {
  CC_set_locator(parser, &charconfig.runtime->pivot_locator_2);
}

// FUNCTION: LEGOBATMAN 0x006234d0
void CC_ride_locator(NUFPAR *parser) {
  CC_set_locator(parser, &charconfig.runtime->ride_locator);
}

// FUNCTION: LEGOBATMAN 0x00623510
void CC_poo_locator(NUFPAR *parser) {
  CC_set_locator(parser, &charconfig.runtime->poo_locator);
}

// FUNCTION: LEGOBATMAN 0x00623550
void CC_head_locator(NUFPAR *parser) {
  CC_set_locator(parser, &charconfig.runtime->head_locator);
}

// FUNCTION: LEGOBATMAN 0x00623590
void CC_rocket_locator(NUFPAR *parser) {
  CC_set_locator(parser, &charconfig.runtime->rocket_locator);
}

// FUNCTION: LEGOBATMAN 0x006235d0
void CC_thingy_locator(NUFPAR *parser) {
  CC_set_locator(parser, &charconfig.runtime->thingy_locator);
}

// FUNCTION: LEGOBATMAN 0x00623610
void CC_helmet_locator(NUFPAR *parser) {
  CC_set_locator(parser, &charconfig.runtime->helmet_locator);
}

// FUNCTION: LEGOBATMAN 0x00623650
void CC_hat_locator(NUFPAR *parser) {
  CC_set_locator(parser, &charconfig.runtime->hat_locator);
}

// STUB: LEGOBATMAN 0x00623690
// close: orig keeps the GetWord loop unrotated (single call at the top);
// ours duplicates the call at the bottom whatever the loop spelling.
void CC_shadow_locators(NUFPAR *parser) {
  charconfig.runtime->shadow_locators = 0;
  u32 locator;
next:
  if (NuFParGetWord(parser) == 0)
    return;
  locator = NuAToI(parser->word_buf);
  if (locator <= 19)
    charconfig.runtime->shadow_locators |= 1 << locator;
  goto next;
}

// STUB: LEGOBATMAN 0x006236e0
// close: same loop rotation difference as CC_shadow_locators.
void CC_thrust_locators(NUFPAR *parser) {
  charconfig.runtime->thrust_locators = 0;
  u32 locator;
next:
  if (NuFParGetWord(parser) == 0)
    return;
  locator = NuAToI(parser->word_buf);
  if (locator <= 19)
    charconfig.runtime->thrust_locators |= 1 << locator;
  goto next;
}

// FUNCTION: LEGOBATMAN 0x00623730
void CC_hand_locators(NUFPAR *parser) {
  for (i32 i = 0; i < 2; i++) {
    charconfig.runtime->hand_locators[i] = -1;
    if (NuFParGetWord(parser) != 0) {
      u32 locator = NuAToI(parser->word_buf);
      if (locator <= 19)
        charconfig.runtime->hand_locators[i] = (i8)locator;
    }
  }
}

// FUNCTION: LEGOBATMAN 0x00623790
void CC_debris_locators(NUFPAR *parser) {
  for (i32 i = 0; i < 2; i++) {
    charconfig.runtime->debris_locators[i] = -1;
    if (NuFParGetWord(parser) != 0) {
      u32 locator = NuAToI(parser->word_buf);
      if (locator <= 19)
        charconfig.runtime->debris_locators[i] = (i8)locator;
    }
  }
}

// FUNCTION: LEGOBATMAN 0x006237f0
void CC_backpack_locators(NUFPAR *parser) {
  for (i32 i = 0; i < 2; i++) {
    charconfig.runtime->backpack_locators[i] = -1;
    if (NuFParGetWord(parser) != 0) {
      u32 locator = NuAToI(parser->word_buf);
      if (locator <= 19)
        charconfig.runtime->backpack_locators[i] = (i8)locator;
    }
  }
}

i16 DefinedLocators_FindIX(char *name);

// FUNCTION: LEGOBATMAN 0x00623de0
void CC_define_locator(NUFPAR *parser) {
  if (NuFParGetWord(parser) != 0) {
    i32 index = DefinedLocators_FindIX(parser->word_buf);
    if (index >= 0 && index < 0x10)
      charconfig.runtime->defined_locators[index] = (i8)NuFParGetInt(parser);
  }
}

// FUNCTION: LEGOBATMAN 0x00623e30
void CC_boost_locator(NUFPAR *parser) {
  CC_set_locator(parser, &charconfig.runtime->boost_locator);
}

// Stand-alone copy at 0x00623e70 takes the parser in esi.
static void CC_set_joint(NUFPAR *parser, i8 *joint) {
  if (NuFParGetWord(parser) != 0) {
    i32 value = NuAToI(parser->word_buf);
    if (value >= 0)
      *joint = (i8)value;
  }
}

// FUNCTION: LEGOBATMAN 0x00623ea0
void CC_head_joint(NUFPAR *parser) {
  CC_set_joint(parser, &charconfig.runtime->head_joint);
}

// FUNCTION: LEGOBATMAN 0x00623ee0
void CC_cloak_joint(NUFPAR *parser) {
  CC_set_joint(parser, &charconfig.runtime->cloak_joint);
}

// FUNCTION: LEGOBATMAN 0x00623f20
void CC_cloak_joint2(NUFPAR *parser) {
  CC_set_joint(parser, &charconfig.runtime->cloak_joint_2);
}

// Stand-alone copy at 0x00623f60 takes the parser in esi.
static void CC_set_sfx(NUFPAR *parser, i16 *sfx) {
  if (NuFParGetWord(parser) != 0)
    *sfx = (i16)GetSfxId(parser->word_buf);
}

// FUNCTION: LEGOBATMAN 0x00623f90
void CC_sfx_die(NUFPAR *parser) {
  CC_set_sfx(parser, &charconfig.runtime->sfx_die);
}

// FUNCTION: LEGOBATMAN 0x00623fd0
void CC_sfx_hurt(NUFPAR *parser) {
  CC_set_sfx(parser, &charconfig.runtime->sfx_hurt);
}

// FUNCTION: LEGOBATMAN 0x00624010
void CC_sfx_doomed(NUFPAR *parser) {
  CC_set_sfx(parser, &charconfig.runtime->sfx_doomed);
}

// FUNCTION: LEGOBATMAN 0x00624050
void CC_sfx_grunt(NUFPAR *parser) {
  CC_set_sfx(parser, &charconfig.runtime->sfx_grunt);
}

// FUNCTION: LEGOBATMAN 0x00624090
void CC_sfx_engine(NUFPAR *parser) {
  i16 *engine = &charconfig.runtime->sfx_engine;
  if (NuFParGetWord(parser) != 0) {
    *engine = (i16)GetSfxId(parser->word_buf);
  }
  charconfig.runtime->flags &= ~0x20;
  if (NuFParGetWord(parser) != 0 &&
      NuStrICmp(parser->word_buf, "move_only") == 0) {
    charconfig.runtime->flags |= 0x20;
  }
}

// FUNCTION: LEGOBATMAN 0x00624110
void CC_sfx_shoot(NUFPAR *parser) {
  CC_set_sfx(parser, &charconfig.runtime->sfx_shoot);
}

// FUNCTION: LEGOBATMAN 0x00624150
void CC_sfx_footstep(NUFPAR *parser) {
  CC_set_sfx(parser, &charconfig.runtime->sfx_footstep);
}

// FUNCTION: LEGOBATMAN 0x00624190
void CC_sfx_chatter(NUFPAR *parser) {
  CC_set_sfx(parser, &charconfig.runtime->sfx_chatter);
}

// FUNCTION: LEGOBATMAN 0x006241d0
void CC_chatter_delay(NUFPAR *parser) {
  charconfig.runtime->chatter_delay = (u8)NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x006241f0
void CC_sfx_sabre(NUFPAR *parser) {
  CC_set_sfx(parser, &charconfig.runtime->sfx_sabre);
}

// FUNCTION: LEGOBATMAN 0x00624230
void CC_sfx_punch(NUFPAR *parser) {
  CC_set_sfx(parser, &charconfig.runtime->sfx_punch);
}

// FUNCTION: LEGOBATMAN 0x00624270
void CC_sfx_punch_hit(NUFPAR *parser) {
  CC_set_sfx(parser, &charconfig.runtime->sfx_punch_hit);
}

// FUNCTION: LEGOBATMAN 0x006242b0
void CC_sfx_beepbeep(NUFPAR *parser) {
  CC_set_sfx(parser, &charconfig.runtime->sfx_beepbeep);
}

// FUNCTION: LEGOBATMAN 0x006242f0
void CC_sfx_phobia(NUFPAR *parser) {
  CC_set_sfx(parser, &charconfig.runtime->sfx_phobia);
}

// FUNCTION: LEGOBATMAN 0x00624330
void CC_sfx_misc(NUFPAR *parser) {
  i32 index;
  for (index = 0; index < 6; index++) {
    if (charconfig.runtime->sfx_misc[index] == -1)
      break;
  }
  if (index < 6 && NuFParGetWord(parser) != 0)
    charconfig.runtime->sfx_misc[index] = (i16)GetSfxId(parser->word_buf);
  index++;
  if (index < 6)
    charconfig.runtime->sfx_misc[index] = -1;
}

// FUNCTION: LEGOBATMAN 0x006243c0
void CC_sfx_land_normal(NUFPAR *parser) {
  CC_set_sfx(parser, &charconfig.runtime->sfx_land_normal);
}

// FUNCTION: LEGOBATMAN 0x00624400
void CC_sfx_land_lunge(NUFPAR *parser) {
  CC_set_sfx(parser, &charconfig.runtime->sfx_land_lunge);
}

// FUNCTION: LEGOBATMAN 0x00624440
void CC_sfx_land_slam(NUFPAR *parser) {
  CC_set_sfx(parser, &charconfig.runtime->sfx_land_slam);
}

// FUNCTION: LEGOBATMAN 0x00624480
void CC_sfx_land_combatroll(NUFPAR *parser) {
  CC_set_sfx(parser, &charconfig.runtime->sfx_land_combatroll);
}

// FUNCTION: LEGOBATMAN 0x006244c0
void CC_sfx_siren(NUFPAR *parser) {
  CC_set_sfx(parser, &charconfig.runtime->sfx_siren);
}

// One flag word of CHARCONFIG_RUNTIME_s seen as bits (CC_hero and friends
// assign single bits from an on/off keyword).
struct CCBits {
  u32 b0 : 1, b1 : 1, b2 : 1, b3 : 1, b4 : 1, b5 : 1, b6 : 1, b7 : 1;
  u32 b8 : 1, b9 : 1, b10 : 1, b11 : 1, b12 : 1, b13 : 1, b14 : 1, b15 : 1;
  u32 b16 : 1, b17 : 1, b18 : 1, b19 : 1, b20 : 1, b21 : 1, b22 : 1, b23 : 1;
  u32 b24 : 1, b25 : 1, b26 : 1, b27 : 1, b28 : 1, b29 : 1, b30 : 1, b31 : 1;
};

// FUNCTION: LEGOBATMAN 0x00624500
void CC_hero(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags148)->b20 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x00624550
void CC_villain(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags148)->b21 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x006245a0
void CC_no_collision(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags)->b27 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x006245f0
void CC_no_weapon_check(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags)->b28 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x00624640
void CC_smash_vehicle_proximities(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags)->b29 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x00624690
void CC_use_awkward_shapes(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags)->b30 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x006246e0
void CC_throw_kill_parts_up(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags)->b31 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

f32 NuFParGetFloat(NUFPAR *parser);

// FUNCTION: LEGOBATMAN 0x00624730
void CC_cloak_up_angle(NUFPAR *parser) {
  charconfig.runtime->cloak_up_angle =
      NuFParGetFloat(parser) * 3.1415927f / 180.0f;
}

// FUNCTION: LEGOBATMAN 0x00624760
void CC_cloak_down_angle(NUFPAR *parser) {
  charconfig.runtime->cloak_down_angle =
      NuFParGetFloat(parser) * 3.1415927f / 180.0f;
}

// FUNCTION: LEGOBATMAN 0x00624790
void CC_viewrange(NUFPAR *parser) {
  charconfig.runtime->viewrange = NuFParGetFloat(parser);
}

// FUNCTION: LEGOBATMAN 0x006247b0
void CC_heardistance(NUFPAR *parser) {
  charconfig.runtime->heardistance = NuFParGetFloat(parser);
}

// FUNCTION: LEGOBATMAN 0x006247d0
void CC_max_viewheight(NUFPAR *parser) {
  charconfig.runtime->max_viewheight = NuFParGetFloat(parser);
}

// FUNCTION: LEGOBATMAN 0x006247f0
void CC_min_viewheight(NUFPAR *parser) {
  charconfig.runtime->min_viewheight = NuFParGetFloat(parser);
}

// FUNCTION: LEGOBATMAN 0x00624810
void CC_mediumres_swapdist(NUFPAR *parser) {
  charconfig.runtime->mediumres_swapdist = NuFParGetFloat(parser);
}

// FUNCTION: LEGOBATMAN 0x00624830
void CC_lowres_swapdist(NUFPAR *parser) {
  charconfig.runtime->lowres_swapdist = NuFParGetFloat(parser);
}

// FUNCTION: LEGOBATMAN 0x00624850
void CC_turn_rate_1(NUFPAR *parser) {
  f32 rate = NuFParGetFloat(parser);
  if (rate > 0.0f) {
    charconfig.runtime->turn_rate_1 = rate;
    charconfig.flags10 |= 0x8;
  }
}

// FUNCTION: LEGOBATMAN 0x00624890
void CC_turn_rate_2(NUFPAR *parser) {
  f32 rate = NuFParGetFloat(parser);
  if (rate > 0.0f) {
    charconfig.runtime->turn_rate_2 = rate;
    charconfig.flags10 |= 0x10;
  }
}

// FUNCTION: LEGOBATMAN 0x006248d0
void CC_banking(NUFPAR *parser) {
  charconfig.runtime->banking = NuFParGetFloat(parser);
}

// FUNCTION: LEGOBATMAN 0x006248f0
void CC_banking2(NUFPAR *parser) {
  charconfig.runtime->banking2 = NuFParGetFloat(parser);
}

// FUNCTION: LEGOBATMAN 0x00624910
void CC_thrust_draw_scale(NUFPAR *parser) {
  charconfig.runtime->thrust_draw_scale = NuFParGetFloat(parser);
}

// FUNCTION: LEGOBATMAN 0x00624930
void CC_can_be_towed(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags144)->b0 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x00624980
void CC_use_special(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags144)->b1 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x006249d0
void CC_get_torpedos(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags144)->b2 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x00624a20
void CC_torpedo_target(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags144)->b5 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x00624a70
void CC_zap_first(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags144)->b6 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x00624ac0
void CC_vehicle_explosion(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags144)->b9 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x00624b10
void CC_dragbomb(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags144)->b3 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x00624b60
void CC_explode_on_impact(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags144)->b7 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x00624bb0
void CC_immune_to_dragbombs(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags144)->b8 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x00624c00
void CC_always_check_anims(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags)->b21 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x00624c50
static void CC_SetGCDataFlagsj(NUFPAR *parser, u32 flags) {
  charconfig.runtime->gcdata_flags |= flags;
  if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0)
    charconfig.runtime->gcdata_flags &= ~flags;
}

// FUNCTION: LEGOBATMAN 0x00624ca0
void CC_choke(NUFPAR *parser) { CC_SetGCDataFlagsj(parser, 0x2); }

// FUNCTION: LEGOBATMAN 0x00624cf0
void CC_lightning(NUFPAR *parser) { CC_SetGCDataFlagsj(parser, 0x4); }

// FUNCTION: LEGOBATMAN 0x00624d40
void CC_resist_zap(NUFPAR *parser) { CC_SetGCDataFlagsj(parser, 0x4000); }

// FUNCTION: LEGOBATMAN 0x00624d90
void CC_has_no_turn(NUFPAR *parser) { CC_SetGCDataFlagsj(parser, 0x10000); }

// FUNCTION: LEGOBATMAN 0x00624de0
void CC_ghost(NUFPAR *parser) { CC_SetGCDataFlagsj(parser, 0x8000); }

// FUNCTION: LEGOBATMAN 0x00624e30
void CC_speed_up_time(NUFPAR *parser) {
  charconfig.runtime->speed_up_time = NuFParGetFloat(parser);
}

// FUNCTION: LEGOBATMAN 0x00624e50
void CC_slow_down_time(NUFPAR *parser) {
  charconfig.runtime->slow_down_time = NuFParGetFloat(parser);
}

// FUNCTION: LEGOBATMAN 0x00624e70
void CC_jump_move_speed_scale(NUFPAR *parser) {
  charconfig.runtime->jump_move_speed_scale = NuFParGetFloat(parser);
}

// FUNCTION: LEGOBATMAN 0x00624e90
void CC_loop_height(NUFPAR *parser) {
  charconfig.runtime->loop_height = NuFParGetFloat(parser);
}

// FUNCTION: LEGOBATMAN 0x00624eb0
void CC_no_tiptoe(NUFPAR *parser) { CC_SetGCDataFlagsj(parser, 0x8); }

// FUNCTION: LEGOBATMAN 0x00624f00
void CC_already_got_hat(NUFPAR *parser) { CC_SetGCDataFlagsj(parser, 0x10); }

// FUNCTION: LEGOBATMAN 0x00624f50
void CC_not_got_hat(NUFPAR *parser) {
  charconfig.runtime->gcdata_flags &= ~0x10;
}

// FUNCTION: LEGOBATMAN 0x00624f60
void CC_punch_weapon_out(NUFPAR *parser) { CC_SetGCDataFlagsj(parser, 0x20); }

// FUNCTION: LEGOBATMAN 0x00624fb0
void CC_can_take_over(NUFPAR *parser) { CC_SetGCDataFlagsj(parser, 0x40); }

// FUNCTION: LEGOBATMAN 0x00625000
void CC_turning_circle(NUFPAR *parser) { CC_SetGCDataFlagsj(parser, 0x100); }

// FUNCTION: LEGOBATMAN 0x00625050
void CC_slide_orientation(NUFPAR *parser) { CC_SetGCDataFlagsj(parser, 0x200); }

// FUNCTION: LEGOBATMAN 0x006250a0
void CC_can_use_cable(NUFPAR *parser) { CC_SetGCDataFlagsj(parser, 0x400); }

// FUNCTION: LEGOBATMAN 0x006250f0
void CC_extra_toggle(NUFPAR *parser) { CC_SetGCDataFlagsj(parser, 0x80000000); }

// FUNCTION: LEGOBATMAN 0x00625140
void CC_can_flatten(NUFPAR *parser) { CC_SetGCDataFlagsj(parser, 0x1000); }

// FUNCTION: LEGOBATMAN 0x00625190
void CC_tightrope_walk(NUFPAR *parser) { CC_SetGCDataFlagsj(parser, 0x80000); }

// FUNCTION: LEGOBATMAN 0x006251e0
void CC_car_wheels(NUFPAR *parser) { CC_SetGCDataFlagsj(parser, 0x100000); }

// FUNCTION: LEGOBATMAN 0x006254c0
void CC_clear_default_items(NUFPAR *parser) {
  charconfig.flags10 |= 0x100;
  for (i32 i = 0; i < 5; i++)
    charconfig.runtime->default_items[i] = -1;
}

// FUNCTION: LEGOBATMAN 0x006254f0
void CC_shield_hit_points(NUFPAR *parser) {
  charconfig.runtime->shield_hit_points = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x00625510
void CC_stop_speed(NUFPAR *parser) {
  charconfig.runtime->stop_speed = NuFParGetFloat(parser);
}

// FUNCTION: LEGOBATMAN 0x00625530
void CC_idle_speed(NUFPAR *parser) {
  charconfig.runtime->idle_speed = NuFParGetFloat(parser);
}

// FUNCTION: LEGOBATMAN 0x00625550
void CC_tiptoe_speed(NUFPAR *parser) {
  charconfig.runtime->tiptoe_speed = NuFParGetFloat(parser);
}

// FUNCTION: LEGOBATMAN 0x00625570
void CC_walk_speed(NUFPAR *parser) {
  charconfig.runtime->walk_speed = NuFParGetFloat(parser);
}

// FUNCTION: LEGOBATMAN 0x00625590
void CC_run_speed(NUFPAR *parser) {
  charconfig.runtime->run_speed = NuFParGetFloat(parser);
}

// FUNCTION: LEGOBATMAN 0x006255b0
void CC_backwards_factor(NUFPAR *parser) {
  charconfig.runtime->backwards_factor = NuFParGetFloat(parser);
}

// FUNCTION: LEGOBATMAN 0x006255d0
void CC_air_gravity(NUFPAR *parser) {
  charconfig.runtime->air_gravity = NuFParGetFloat(parser);
}

// FUNCTION: LEGOBATMAN 0x006255f0
void CC_jump_speed(NUFPAR *parser) {
  charconfig.runtime->jump_speed = NuFParGetFloat(parser);
}

// FUNCTION: LEGOBATMAN 0x00625610
void CC_jump_2_speed(NUFPAR *parser) {
  charconfig.runtime->jump_2_speed = NuFParGetFloat(parser);
}

// FUNCTION: LEGOBATMAN 0x00625630
void CC_acceleration(NUFPAR *parser) {
  charconfig.runtime->acceleration = NuFParGetFloat(parser);
}

// FUNCTION: LEGOBATMAN 0x006256a0
void CC_hit_points(NUFPAR *parser) {
  charconfig.runtime->hit_points = NuFParGetInt(parser);
}

struct CCCharacter_s {
  u8 pad0[4];
  u32 model_flags; // 0x04
};

// saga's charconfig.character; charconfig.runtime follows it.
// GLOBAL: LEGOBATMAN 0x00acb860
extern CCCharacter_s *g_unk00acb860;

// FUNCTION: LEGOBATMAN 0x006256c0
static void CC_SetCDataFlagsj(NUFPAR *parser, u32 flags) {
  g_unk00acb860->model_flags |= flags;
  if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0)
    g_unk00acb860->model_flags &= ~flags;
}

// FUNCTION: LEGOBATMAN 0x00625700
void CC_baddie(NUFPAR *parser) { CC_SetCDataFlagsj(parser, 0x4); }

// FUNCTION: LEGOBATMAN 0x00625740
void CC_jedi(NUFPAR *parser) { CC_SetCDataFlagsj(parser, 0x8); }

// FUNCTION: LEGOBATMAN 0x00625780
void CC_droid(NUFPAR *parser) { CC_SetCDataFlagsj(parser, 0x10); }

// FUNCTION: LEGOBATMAN 0x006257c0
void CC_protocol(NUFPAR *parser) { CC_SetCDataFlagsj(parser, 0x30); }

// FUNCTION: LEGOBATMAN 0x00625800
void CC_astromech(NUFPAR *parser) { CC_SetCDataFlagsj(parser, 0x50); }

// FUNCTION: LEGOBATMAN 0x00625840
void CC_cannon(NUFPAR *parser) { CC_SetGCDataFlagsj(parser, 0x800); }

// FUNCTION: LEGOBATMAN 0x00625890
void CC_bounty_hunter(NUFPAR *parser) { CC_SetCDataFlagsj(parser, 0x1000000); }

// FUNCTION: LEGOBATMAN 0x006258e0
void CC_blaster(NUFPAR *parser) { CC_SetCDataFlagsj(parser, 0x10000080); }

// FUNCTION: LEGOBATMAN 0x00625930
void CC_teleport(NUFPAR *parser) { CC_SetCDataFlagsj(parser, 0x40000); }

// FUNCTION: LEGOBATMAN 0x00625980
void CC_zipup(NUFPAR *parser) { CC_SetCDataFlagsj(parser, 0x100000); }

// FUNCTION: LEGOBATMAN 0x006259d0
void CC_untargetable(NUFPAR *parser) { CC_SetCDataFlagsj(parser, 0x80000); }

// FUNCTION: LEGOBATMAN 0x00625a20
void CC_orientate(NUFPAR *parser) { CC_SetCDataFlagsj(parser, 0x100); }

// FUNCTION: LEGOBATMAN 0x00625a70
void CC_neutral(NUFPAR *parser) { CC_SetCDataFlagsj(parser, 0x200); }

// FUNCTION: LEGOBATMAN 0x00625ac0
void CC_complex_shadow(NUFPAR *parser) { CC_SetCDataFlagsj(parser, 0x10000); }

// FUNCTION: LEGOBATMAN 0x00625b10
void CC_shadow_orientation(NUFPAR *parser) {
  CC_SetGCDataFlagsj(parser, 0x200000);
}

// FUNCTION: LEGOBATMAN 0x00625b60
void CC_can_open_doors(NUFPAR *parser) { CC_SetCDataFlagsj(parser, 0x400); }

// FUNCTION: LEGOBATMAN 0x00625bb0
void CC_non_stick(NUFPAR *parser) { CC_SetCDataFlagsj(parser, 0x800); }

// FUNCTION: LEGOBATMAN 0x00625c00
void CC_can_shoot_offscreen(NUFPAR *parser) {
  CC_SetCDataFlagsj(parser, 0x1000);
}

// FUNCTION: LEGOBATMAN 0x00625c50
void CC_can_fire(NUFPAR *parser) { CC_SetCDataFlagsj(parser, 0x10000000); }

struct WORLDINFO_s;
i32 BoltType_FindIDByName(char *name, WORLDINFO_s *world);

// FUNCTION: LEGOBATMAN 0x00625ca0
void CC_bolt_type(NUFPAR *parser) {
  if (NuFParGetWord(parser) != 0)
    charconfig.runtime->bolt_type = BoltType_FindIDByName(parser->word_buf, 0);
  else
    charconfig.runtime->bolt_type = -1;
  charconfig.runtime->bolt_type_2 = charconfig.runtime->bolt_type;
}

// FUNCTION: LEGOBATMAN 0x00625d10
void CC_not_using_item_sys(NUFPAR *parser) {
  charconfig.runtime->flags148 |= 0x80000000;
}

// FUNCTION: LEGOBATMAN 0x00625d20
void CC_minikit(NUFPAR *parser) { CC_SetCDataFlagsj(parser, 0x4000000); }

// FUNCTION: LEGOBATMAN 0x00625d70
void CC_minikit_with_scene(NUFPAR *parser) {
  CC_SetCDataFlagsj(parser, 0x4000000);
  ((CCBits *)&charconfig.runtime->flags144)->b11 =
      g_unk00acb860->model_flags >> 26;
}

// FUNCTION: LEGOBATMAN 0x00625de0
void CC_minikit_noscenewhenplayable(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags144)->b22 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x00625e30
void CC_lift_hover(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags144)->b17 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x00625e80
void CC_freeze_flash(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags)->b22 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x00625ed0
void CC_shoot_turning_circle(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags)->b23 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x00625f20
void CC_always_regenerate_hearts(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags)->b25 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x00625f70
void CC_always_fast_build(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags150)->b1 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x00625fc0
void CC_flatten_die(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags)->b4 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x00626010
void CC_can_blend_to_shoot(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags)->b6 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x00626060
void CC_can_blend_to_punch(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags)->b7 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x006260b0
void CC_supercounter_character(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags)->b9 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x00626100
void CC_critter(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags)->b10 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x00626150
void CC_can_poo(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags148)->b11 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x006261a0
void CC_can_super_carry(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags148)->b12 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x006261f0
void CC_can_zap(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags148)->b13 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x00626240
void CC_can_electrocute(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags150)->b0 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x00626290
void CC_put_weapon_away_on_shoot(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags148)->b14 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x006262e0
void CC_always_stop_to_shoot(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags148)->b15 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x00626330
void CC_second_shot_only(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags148)->b16 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x00626380
void CC_no_start_punch_sfx(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags148)->b17 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x006263d0
void CC_single_jump_slam(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags148)->b18 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x00626420
void CC_dont_draw_rider(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags148)->b19 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x00626470
void CC_shoot_backwards(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags148)->b22 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x006264c0
void CC_glide_anytime(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags144)->b18 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x00626510
void CC_can_communicate(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags144)->b19 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x00626560
void CC_immune_to_magnets(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags144)->b20 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x006265b0
void CC_immune_to_forcepush(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags144)->b21 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x00626600
void CC_cannot_kill(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags144)->b23 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x00626650
void CC_wait_for_weapon_in_out(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags144)->b24 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x006266a0
void CC_double_jump_hover(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags144)->b25 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x006266f0
void CC_no_category(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags144)->b26 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x00626740
void CC_no_force(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags144)->b27 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x00626790
void CC_white_flash(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags144)->b28 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x006267e0
void CC_buck_rider(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags144)->b29 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x00626830
void CC_die_usecurrentlayers(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags144)->b30 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x00626880
void CC_no_weapon_draw(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags148)->b1 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x006268d0
void CC_no_jump_fire(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags148)->b2 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x00626920
void CC_hover_over_mud(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags148)->b3 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x00626970
void CC_toggle_if_not_in_collection(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags148)->b4 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x006269c0
void CC_toggle_if_in_minikit_bonus(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags148)->b5 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x00626a10
void CC_deflect_bolts_in_minikit_bonus(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags148)->b6 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x00626a60
void CC_has_grapple(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags148)->b7 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x00626ab0
void CC_hover_wings(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags148)->b8 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x00626b00
void CC_has_whip(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags148)->b9 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x00626b50
void CC_can_scream(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags148)->b23 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x00626ba0
void CC_fight_sequential(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags)->b16 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x00626bf0
void CC_helicopter(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags)->b15 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x00626c40
void CC_super_turn(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags)->b26 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x00626c90
void CC_respawn(NUFPAR *parser) { CC_SetCDataFlagsj(parser, 0x8000000); }

// FUNCTION: LEGOBATMAN 0x00626ce0
void CC_vehicle(NUFPAR *parser) {
  CC_SetCDataFlagsj(parser, 0x2000);
  charconfig.runtime->flags148 |= 0x80000000;
}

// FUNCTION: LEGOBATMAN 0x00626d40
void CC_floating_vehicle(NUFPAR *parser) {
  CC_SetCDataFlagsj(parser, 0x2000);
  ((CCBits *)&charconfig.runtime->flags)->b13 =
      g_unk00acb860->model_flags >> 13;
}

// FUNCTION: LEGOBATMAN 0x00626db0
void CC_flying_vehicle(NUFPAR *parser) {
  CC_SetCDataFlagsj(parser, 0x2000);
  ((CCBits *)&charconfig.runtime->flags)->b14 =
      g_unk00acb860->model_flags >> 13;
}

// FUNCTION: LEGOBATMAN 0x00626e20
void CC_only_active_when_taken_over(NUFPAR *parser) {
  CC_SetCDataFlagsj(parser, 0x20000000);
}

// FUNCTION: LEGOBATMAN 0x00626e70
void CC_beast(NUFPAR *parser) { CC_SetCDataFlagsj(parser, 0x40000000); }

// FUNCTION: LEGOBATMAN 0x00626ec0
void CC_prefers_brawling(NUFPAR *parser) {
  CC_SetCDataFlagsj(parser, 0x80000000);
}

// FUNCTION: LEGOBATMAN 0x00626f10
void CC_dontpush(NUFPAR *parser) { CC_SetCDataFlagsj(parser, 0x4000); }

// FUNCTION: LEGOBATMAN 0x00626f60
void CC_dont_move_out_of_way(NUFPAR *parser) {
  CC_SetGCDataFlagsj(parser, 0x2000);
}

// FUNCTION: LEGOBATMAN 0x00626fb0
void CC_dontmove(NUFPAR *parser) { CC_SetGCDataFlagsj(parser, 0x80); }

// FUNCTION: LEGOBATMAN 0x00627000
void CC_jetpack(NUFPAR *parser) { CC_SetCDataFlagsj(parser, 0x8000); }

// FUNCTION: LEGOBATMAN 0x00627050
void CC_oldheadmovement(NUFPAR *parser) { CC_SetCDataFlagsj(parser, 0x20000); }

// FUNCTION: LEGOBATMAN 0x006270a0
void CC_atatheadmovement(NUFPAR *parser) {
  charconfig.runtime->flags148 |= 0x400;
}

// FUNCTION: LEGOBATMAN 0x006270b0
void CC_cannotbigjump(NUFPAR *parser) { CC_SetCDataFlagsj(parser, 0x200000); }

// FUNCTION: LEGOBATMAN 0x00627100
void CC_no_offpath_teleport(NUFPAR *parser) {
  CC_SetCDataFlagsj(parser, 0x400000);
}

// FUNCTION: LEGOBATMAN 0x00627150
void CC_no_kill_parts(NUFPAR *parser) { CC_SetCDataFlagsj(parser, 0x2000000); }

// FUNCTION: LEGOBATMAN 0x006271a0
void CC_high_jump(NUFPAR *parser) { CC_SetGCDataFlagsj(parser, 0x400000); }

// FUNCTION: LEGOBATMAN 0x006271f0
void CC_super_strength(NUFPAR *parser) { CC_SetGCDataFlagsj(parser, 0x800000); }

// FUNCTION: LEGOBATMAN 0x00627300
void CC_bypass_security(NUFPAR *parser) {
  CC_SetGCDataFlagsj(parser, 0x2000000);
}

// FUNCTION: LEGOBATMAN 0x00627350
void CC_techno(NUFPAR *parser) { CC_SetGCDataFlagsj(parser, 0x1); }

// FUNCTION: LEGOBATMAN 0x006273a0
void CC_hazard_protection(NUFPAR *parser) {
  CC_SetGCDataFlagsj(parser, 0x4000000);
}

// FUNCTION: LEGOBATMAN 0x006273f0
void CC_wall_jump(NUFPAR *parser) { CC_SetGCDataFlagsj(parser, 0x8000000); }

// FUNCTION: LEGOBATMAN 0x00627440
void CC_got_batarang(NUFPAR *parser) { CC_SetGCDataFlagsj(parser, 0x20000000); }

// FUNCTION: LEGOBATMAN 0x00627490
void CC_tightrope_tilt(NUFPAR *parser) {
  CC_SetGCDataFlagsj(parser, 0x10000000);
}

// FUNCTION: LEGOBATMAN 0x006277d0
void CC_blobshadow_alpha(NUFPAR *parser) {
  charconfig.runtime->blobshadow_alpha = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x00627870
void CC_hover_height(NUFPAR *parser) {
  charconfig.runtime->hover_height = NuFParGetFloat(parser);
}

// FUNCTION: LEGOBATMAN 0x006278d0
void CC_maxheadturn(NUFPAR *parser) {
  charconfig.runtime->maxheadturn =
      NuFParGetFloat(parser) * 3.1415927f / 180.0f;
}

// FUNCTION: LEGOBATMAN 0x00627900
void CC_maxheadtilt(NUFPAR *parser) {
  charconfig.runtime->maxheadtilt =
      NuFParGetFloat(parser) * 3.1415927f / 180.0f;
}

// FUNCTION: LEGOBATMAN 0x00627930
void CC_headrotrate(NUFPAR *parser) {
  charconfig.runtime->headrotrate =
      NuFParGetFloat(parser) * 3.1415927f / 180.0f;
}

// FUNCTION: LEGOBATMAN 0x006275b0
void CC_only_animate_when_taken_over(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags)->b19 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x00627600
void CC_has_scene_element(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags144)->b10 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x00627650
void CC_dont_turn(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags144)->b12 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x006276a0
void CC_wheelie(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags)->b3 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x006276f0
void CC_no_jump(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags144)->b13 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x00627740
void CC_no_shoot(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags144)->b14 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x00627960
void CC_coin_value(NUFPAR *parser) {
  i32 value = NuFParGetInt(parser);
  if (value > 10000)
    value = 10000;
  else if (value < 0)
    value = 0;
  charconfig.runtime->coin_value = (value / 10) * 10;
}

float NuAToF(const char *s);

// FUNCTION: LEGOBATMAN 0x006279b0
static void CC_set_timebaseupdatei(NUFPAR *parser, i32 index) {
  if (NuFParGetWord(parser) != 0) {
    f32 distance = NuAToF(parser->word_buf);
    if (NuFParGetWord(parser) != 0) {
      i32 frames = NuAToI(parser->word_buf);
      charconfig.runtime->ai_update_distance[index] = distance;
      charconfig.runtime->ai_update_interval[index] = frames;
      charconfig.flags10 |= 0x20;
    }
  }
}

// All four pass slot 0 in the shipped code.
// FUNCTION: LEGOBATMAN 0x00627a20
void CC_set_timebaseupdate1(NUFPAR *parser) {
  CC_set_timebaseupdatei(parser, 0);
}

// FUNCTION: LEGOBATMAN 0x00627a30
void CC_set_timebaseupdate2(NUFPAR *parser) {
  CC_set_timebaseupdatei(parser, 0);
}

// FUNCTION: LEGOBATMAN 0x00627a40
void CC_set_timebaseupdate3(NUFPAR *parser) {
  CC_set_timebaseupdatei(parser, 0);
}

// FUNCTION: LEGOBATMAN 0x00627a50
void CC_set_timebaseupdate4(NUFPAR *parser) {
  CC_set_timebaseupdatei(parser, 0);
}

i32 ActionFromName(char *name);

// FUNCTION: LEGOBATMAN 0x00627a60
void CC_rider_action(NUFPAR *parser) {
  if (NuFParGetWord(parser) != 0)
    charconfig.runtime->rider_action = ActionFromName(parser->word_buf);
}

// FUNCTION: LEGOBATMAN 0x00627a90
void CC_dance_action(NUFPAR *parser) {
  if (NuFParGetWord(parser) != 0)
    charconfig.runtime->dance_action = ActionFromName(parser->word_buf);
}

// GLOBAL: LEGOBATMAN 0x00acd770
extern u8 (*g_unk00acd770)(char *name);

// FUNCTION: LEGOBATMAN 0x00627ac0
void CC_phobia(NUFPAR *parser) {
  if (NuFParGetWord(parser) != 0 && g_unk00acd770 != 0) {
    u8 phobia = g_unk00acd770(parser->word_buf);
    if (phobia != 0) {
      if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0)
        charconfig.runtime->phobias &= ~phobia;
      else
        charconfig.runtime->phobias |= phobia;
    }
  }
}

// FUNCTION: LEGOBATMAN 0x00627b40
void CC_DeflectBolts(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags148)->b24 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x00627b90
void CC_HoseLocators(NUFPAR *parser) {
  i32 i;
  for (i = 0; i < 4; i++) {
    if (NuFParGetWord(parser) == 0)
      break;
    i32 locator = NuAToI(parser->word_buf);
    if ((u32)locator > 19)
      break;
    charconfig.runtime->hose_locators[i] = locator;
  }
  for (; i < 4; i++)
    charconfig.runtime->hose_locators[i] = -1;
}

// FUNCTION: LEGOBATMAN 0x00627c00
void CC_DieAir_PunchChance(NUFPAR *parser) {
  charconfig.runtime->die_air_punch_chance = NuFParGetFloat(parser);
}

// FUNCTION: LEGOBATMAN 0x00627c20
void CC_NeedsPaddling(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags148)->b25 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x00627c70
void CC_CanMindControl(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags148)->b26 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x00627cc0
void CC_grabber_control(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags148)->b27 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x00627d10
void CC_float_high(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags148)->b28 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x00627d60
void CC_use_turnmul(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags)->b0 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x00627db0
void CC_dont_trigger_vehicle_proximity(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags)->b1 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x00627e00
void CC_wall_shuffle_face_wall(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags)->b2 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x00628270
void CC_detonator_type(NUFPAR *parser) {
  charconfig.runtime->detonator_type = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x00628290
void CC_swimmer(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags148)->b30 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x006282e0
void CC_diver(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags144)->b4 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x00628330
void CC_move_on_rails(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags148)->b29 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x00628380
void CC_deactivate_backtonormal(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags)->b11 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x006283d0
void CC_ai_terrain_awkwardshape(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags)->b12 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x00628420
void CC_batarang_target(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags)->b17 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x00628470
void CC_can_scare(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags)->b18 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x006284c0
void CC_cannot_freeze(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags)->b20 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}

// FUNCTION: LEGOBATMAN 0x00628510
void CC_sfx_glide_is_brolly(NUFPAR *parser) {
  ((CCBits *)&charconfig.runtime->flags)->b24 =
      NuFParGetWord(parser) == 0 || NuStrICmp(parser->word_buf, "off") != 0;
}
