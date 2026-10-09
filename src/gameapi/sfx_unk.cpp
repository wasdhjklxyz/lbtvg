// gameapi/sfx_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nu2api/nucore/common.h"
#include <stdio.h>

typedef struct WORLDINFO_s {
  unsigned char pad0[0x80];
  char config_file[0x84]; // 0x80
} WORLDINFO;

i32 NuFileExists(char *name);
void SpecialSfxLoad(char *path, WORLDINFO *world);

// FUNCTION: LEGOBATMAN 0x00675430
void LoadSpecialSfxFile(WORLDINFO *world) {
  char path[256];
  sprintf(path, "%s.sfx", world->config_file);
  if (NuFileExists(path)) {
    SpecialSfxLoad(path, world);
  }
}

typedef struct nufpar_s {
  u32 pad0[0x910 / 4];
  char *word_buf; // 0x910
} NUFPAR;

NUFPAR *NuFParCreate(char *file);
i32 NuFParGetLine(NUFPAR *parser);
i32 NuFParGetWord(NUFPAR *parser);
i32 NuFParGetInt(NUFPAR *parser);
f32 NuFParGetFloat(NUFPAR *parser);
void NuFParDestroy(NUFPAR *parser);
i32 NuStrICmp(const char *a, const char *b);
i32 CharIDFromName(char *name);
i32 ActionFromName(char *name);

typedef struct CHARPIVOT_s {
  i16 action;       // 0x00
  i16 locator;      // 0x02
  f32 start_frame;  // 0x04
  f32 end_frame;    // 0x08
  i16 character_id; // 0x0c
  u16 pad;
} CHARPIVOT;

// GLOBAL: LEGOBATMAN 0x00ad2a54
static CHARPIVOT *CharPivot;

// FUNCTION: LEGOBATMAN 0x006754c0
void CharPivot_Init(char *file, VARIPTR *buf) {
  i32 count = 0;
  CharPivot = 0;
  if (file == 0)
    return;
  NUFPAR *parser = NuFParCreate(file);
  if (parser == 0)
    return;
  buf->addr = (buf->addr + 3) & ~3;
  CharPivot = (CHARPIVOT *)buf->addr;
  while (NuFParGetLine(parser)) {
    CharPivot[count].start_frame = 0.0f;
    CharPivot[count].end_frame = 0.0f;
    CharPivot[count].character_id = -1;
    CharPivot[count].action = -1;
    CharPivot[count].locator = -1;
    while (NuFParGetWord(parser)) {
      if (NuStrICmp(parser->word_buf, "character") == 0) {
        if (NuFParGetWord(parser))
          CharPivot[count].character_id = CharIDFromName(parser->word_buf);
      } else if (NuStrICmp(parser->word_buf, "action") == 0) {
        if (NuFParGetWord(parser))
          CharPivot[count].action = ActionFromName(parser->word_buf);
      } else if (NuStrICmp(parser->word_buf, "locator") == 0) {
        i32 locator = NuFParGetInt(parser);
        if (locator >= 0 && locator < 20)
          CharPivot[count].locator = locator;
      } else if (NuStrICmp(parser->word_buf, "start_frame") == 0) {
        CharPivot[count].start_frame = NuFParGetFloat(parser);
      } else if (NuStrICmp(parser->word_buf, "end_frame") == 0) {
        CharPivot[count].end_frame = NuFParGetFloat(parser);
      }
    }
    if (CharPivot[count].character_id != -1 && CharPivot[count].action != -1)
      count++;
  }
  NuFParDestroy(parser);
  if (count > 0) {
    CharPivot[count].character_id = -1;
    buf->addr += (count + 1) * sizeof(CHARPIVOT);
  } else {
    CharPivot = 0;
  }
}
