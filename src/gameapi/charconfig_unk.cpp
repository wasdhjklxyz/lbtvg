// gameapi/charconfig_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nu2api/nucore/common.h"
#include "../nu2api/nucore/nustring.h"

typedef struct nufpar_s {
  unsigned char pad0[0x910];
  char *word_buf; // 0x910
} NUFPAR;

typedef struct CHARCONFIG_RUNTIME_s {
  unsigned char pad0[0x148];
  u32 flags148; // 0x148
  u32 flags;    // 0x14c
  unsigned char pad150[0x174 - 0x150];
  u32 shadow_locators; // 0x174, bit per locator
  u32 thrust_locators; // 0x178
  unsigned char pad17c[0x194 - 0x17c];
  i16 sfx_misc[6];         // 0x194
  i16 sfx_die;             // 0x1a0
  i16 sfx_hurt;            // 0x1a2
  i16 sfx_doomed;          // 0x1a4
  i16 sfx_grunt;           // 0x1a6
  i16 sfx_engine;          // 0x1a8
  i16 sfx_shoot;           // 0x1aa
  i16 sfx_footstep;        // 0x1ac
  i16 sfx_chatter;         // 0x1ae
  i16 sfx_sabre;           // 0x1b0
  i16 sfx_punch;           // 0x1b2
  i16 sfx_punch_hit;       // 0x1b4
  i16 sfx_beepbeep;        // 0x1b6
  i16 sfx_phobia;          // 0x1b8
  i16 sfx_land_normal;     // 0x1ba
  i16 sfx_land_lunge;      // 0x1bc
  i16 sfx_land_slam;       // 0x1be
  i16 sfx_land_combatroll; // 0x1c0
  i16 sfx_siren;           // 0x1c2
  unsigned char pad1c4[0x1db - 0x1c4];
  i8 grapple_gun_locator;     // 0x1db
  i8 defined_locators[0x10];  // 0x1dc
  i8 weapon_locator[4];       // 0x1ec
  i8 weapon_shoot_locator[4]; // 0x1f0
  unsigned char pad1f4[0x208 - 0x1f4];
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
  unsigned char pad226[0x233 - 0x226];
  u8 chatter_delay; // 0x233
} CHARCONFIG_RUNTIME_s;

typedef struct CHARCONFIG_s {
  CHARCONFIG_RUNTIME_s *runtime;
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
