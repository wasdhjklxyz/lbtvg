// gameapi/charconfig_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nu2api/nucore/common.h"
#include "../nu2api/nucore/nustring.h"

typedef struct nufpar_s {
  unsigned char pad0[0x910];
  char *word_buf; // 0x910
} NUFPAR;

typedef struct CHARCONFIG_RUNTIME_s {
  unsigned char pad0[0x14c];
  u32 flags; // 0x14c
  unsigned char pad150[0x1a0 - 0x150];
  i16 sfx_die;       // 0x1a0
  i16 sfx_hurt;      // 0x1a2
  i16 sfx_doomed;    // 0x1a4
  i16 sfx_grunt;     // 0x1a6
  i16 sfx_engine;    // 0x1a8
  i16 sfx_shoot;     // 0x1aa
  i16 sfx_footstep;  // 0x1ac
  i16 sfx_chatter;   // 0x1ae
  i16 sfx_sabre;     // 0x1b0
  i16 sfx_punch;     // 0x1b2
  i16 sfx_punch_hit; // 0x1b4
  i16 sfx_beepbeep;  // 0x1b6
  i16 sfx_phobia;    // 0x1b8
  unsigned char pad1ba[0x233 - 0x1ba];
  u8 chatter_delay; // 0x233
} CHARCONFIG_RUNTIME_s;

typedef struct CHARCONFIG_s {
  CHARCONFIG_RUNTIME_s *runtime;
} CHARCONFIG_s;

i32 NuFParGetWord(NUFPAR *parser);
i32 GetSfxId(char *name);

// GLOBAL: LEGOBATMAN 0x00acb864
extern CHARCONFIG_s charconfig;

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

i32 NuFParGetInt(NUFPAR *parser);

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
