// nu2api/nufile/nufpar_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nucore/nustring.h"

typedef u16 NUWCHAR16;

typedef struct nufpar_s {
  u32 pad0[0x910 / 4];
  char *word_buf; // 0x910
  u32 pad914[(0x978 - 0x914) / 4];
  char is_utf16; // 0x978
} NUFPAR;

i32 NuFParGetWord(NUFPAR *parser);
void NuUnicodeToAscii(char *dst, NUWCHAR16 *src);

// FUNCTION: LEGOBATMAN 0x006daa40
f32 NuFParGetFloat(NUFPAR *parser) {
  char buf[64];

  NuFParGetWord(parser);
  if (parser->is_utf16) {
    NuUnicodeToAscii(buf, (NUWCHAR16 *)parser->word_buf);
  } else {
    char *dst = buf;
    char *src = parser->word_buf;
    if (src != 0) {
      while (*src != '\0') {
        *dst++ = *src++;
      }
    }
    *dst = '\0';
  }

  if (buf[0] != '\0') {
    return NuAToF(buf);
  } else {
    return 0.0f;
  }
}
