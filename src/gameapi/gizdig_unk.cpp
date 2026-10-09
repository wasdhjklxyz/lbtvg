// gameapi/gizdig_unk.cpp: dig gizmo sound configuration (GizDigSFX_*);
// file name unproven.

#include "../batman/worldinfo_unk.h"
#include "../nu2api/nucore/common.h"
#include "../nu2api/nucore/nustring.h"

typedef struct nufpar_s {
  u8 pad0[0x910];
  char *word_buf; // 0x910
} NUFPAR;

typedef struct nufpcomjmp_s {
  char *fn_name;
  void (*fn)(NUFPAR *parser);
} NUFPCOMJMP;

struct GAMEANIMOBJ_s {
  GAMEANIMOBJ_s *next; // 0x00
  u32 special[3];      // 0x04, nuhspecial_s
};

struct GAMEANIMSET_s {
  u8 pad0[0x24];
  GAMEANIMOBJ_s *objects; // 0x24
};

struct GIZDIG_s {
  u8 pad0[0x34];
  GAMEANIMSET_s *anim_set; // 0x34
  u8 pad38[0x7a - 0x38];
  i16 sfx_process; // 0x7a
  u8 pad7c[0xa0 - 0x7c];
};

struct GIZDIGSYS_s {
  GIZDIG_s *digs; // 0x00
  u8 pad4[0xe - 4];
  u16 count; // 0x0e
};

NUFPAR *NuFParCreateMem(char *name, char *buffer, i32 bufferSize);
i32 NuFParPushCom(NUFPAR *parser, NUFPCOMJMP *commands);
i32 NuFParGetLine(NUFPAR *parser);
i32 NuFParGetWord(NUFPAR *parser);
i32 NuFParInterpretWord(NUFPAR *parser);
void NuFParPopCom(NUFPAR *parser);
void NuFParDestroy(NUFPAR *parser);
char *NuSpecialGetName(nuhspecial_s *sp);
i32 GetSfxId(char *name);

// GLOBAL: LEGOBATMAN 0x009656e0
extern i32 GizDigSFX_load_version;
// GLOBAL: LEGOBATMAN 0x00aced24
extern WORLDINFO_s *GizDigSFX_worldinfo;
// GLOBAL: LEGOBATMAN 0x00aced20
extern GIZDIG_s *GizDigSFX_dig;
// GLOBAL: LEGOBATMAN 0x009656e4
extern NUFPCOMJMP GizDigSFX_ConfigKeywords[];

// FUNCTION: LEGOBATMAN 0x0063af70
static GIZDIG_s *GizDigFindByNameUnk0063af70(WORLDINFO_s *world, char *name) {
  GIZDIGSYS_s *system = world->giz_dig_sys;
  if (system != 0) {
    GIZDIG_s *dig = system->digs;
    for (i32 i = 0; i < system->count; i++, dig++) {
      if (dig->anim_set != 0) {
        for (GAMEANIMOBJ_s *object = dig->anim_set->objects; object != 0;
             object = object->next) {
          char *special_name =
              NuSpecialGetName((nuhspecial_s *)object->special);
          if (special_name != 0 && NuStrICmp(name, special_name) == 0)
            return dig;
        }
      }
    }
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x0063afe0
void GizDigSFX_digname(NUFPAR *parser) {
  GizDigSFX_dig = 0;
  if (NuFParGetWord(parser))
    GizDigSFX_dig =
        GizDigFindByNameUnk0063af70(GizDigSFX_worldinfo, parser->word_buf);
}

// FUNCTION: LEGOBATMAN 0x0063b020
void GizDigSFX_processsfx(NUFPAR *parser) {
  if (GizDigSFX_dig != 0 && NuFParGetWord(parser) &&
      GizDigSFX_dig->sfx_process == -1)
    GizDigSFX_dig->sfx_process = GetSfxId(parser->word_buf);
}

// FUNCTION: LEGOBATMAN 0x0063b070
void GizDigSFX_completesfx(NUFPAR *parser) {}

// FUNCTION: LEGOBATMAN 0x0063b080
void GizDigSFX_returnsfx(NUFPAR *parser) {}

// FUNCTION: LEGOBATMAN 0x0063b090
void GizDigSFX_Configure(WORLDINFO_s *world, char *config) {
  if (GizDigSFX_load_version >= 1 || world == 0 || world->giz_dig_sys == 0 ||
      world->giz_dig_sys->count == 0)
    return;
  GizDigSFX_dig = 0;
  GizDigSFX_worldinfo = world;
  NUFPAR *parser = NuFParCreateMem("digSFX", config, 0xffff);
  if (parser == 0)
    return;
  NuFParPushCom(parser, GizDigSFX_ConfigKeywords);
  i32 inside = 0;
  while (NuFParGetLine(parser)) {
    while (NuFParGetWord(parser)) {
      if (inside) {
        if (NuStrICmp(parser->word_buf, "digsfx_end") == 0) {
          inside = 0;
        } else {
          NuFParInterpretWord(parser);
        }
      } else if (NuStrICmp(parser->word_buf, "digsfx_start") == 0) {
        inside = 1;
      }
    }
  }
  NuFParPopCom(parser);
  NuFParDestroy(parser);
}
