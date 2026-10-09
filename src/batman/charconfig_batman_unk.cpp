// batman/charconfig_batman_unk.cpp: Batman-specific character config
// keyword parsers (keyword table 0x00936288); between pcbatman.cpp and
// pcapi.cpp by link order, file name unproven.

#include "../nu2api/nucore/common.h"
#include "../nu2api/nucore/nustring.h"

typedef struct nufpar_s {
  u8 pad0[0x910];
  char *word_buf; // 0x910
} NUFPAR;

i32 NuFParGetWord(NUFPAR *parser);
i32 Unk0061ff40(char *name);
i32 Unk00629850(char *name);

struct CHARCONFIG_RUNTIME_s {
  u8 pad0[0x22b];
  i8 variant; // 0x22b
  u8 pad22c[0x234 - 0x22c];
  u8 security_guard;  // 0x234
  u8 security_access; // 0x235
};

struct CHARCONFIG_s {
  CHARCONFIG_RUNTIME_s *runtime;
};

extern CHARCONFIG_s charconfig;

// FUNCTION: LEGOBATMAN 0x0041d020
void CC_variant(NUFPAR *parser) {
  if (NuFParGetWord(parser) != 0) {
    if (NuStrICmp(parser->word_buf, "off") == 0)
      charconfig.runtime->variant = -1;
    else
      charconfig.runtime->variant = Unk0061ff40(parser->word_buf);
  }
}

// keywords "security_guard" and "security_type"
// FUNCTION: LEGOBATMAN 0x0041d090
void CC_security_guard(NUFPAR *parser) {
  if (NuFParGetWord(parser) != 0) {
    charconfig.runtime->security_guard = Unk00629850(parser->word_buf);
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0)
      charconfig.runtime->security_guard = 0xff;
  }
}

// FUNCTION: LEGOBATMAN 0x0041d110
void CC_security_access(NUFPAR *parser) {
  if (NuFParGetWord(parser) != 0) {
    charconfig.runtime->security_access = Unk00629850(parser->word_buf);
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0)
      charconfig.runtime->security_access = 0xff;
  }
}
