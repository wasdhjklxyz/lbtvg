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
  unsigned char pad150[0x1a8 - 0x150];
  i16 sfx_engine; // 0x1a8
} CHARCONFIG_RUNTIME_s;

typedef struct CHARCONFIG_s {
  CHARCONFIG_RUNTIME_s *runtime;
} CHARCONFIG_s;

i32 NuFParGetWord(NUFPAR *parser);
i32 GetSfxId(char *name);

// GLOBAL: LEGOBATMAN 0x00acb864
extern CHARCONFIG_s charconfig;

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
