// batman/, file unknown: gizmo flow actions (0x00483e20).

#include "../nu2api/nucore/nustring.h"

struct GIZFLOW_s;
struct FLOWBOX_s;

void PlayRadio(char *special, char *blowup, i32 loop);

// FUNCTION: LEGOBATMAN 0x00483e20
void GizActions_PlayRadio(GIZFLOW_s *flow, FLOWBOX_s *box, char **args,
                          int argc) {
  i32 loop = 1;
  char *special = 0;
  char *blowup = 0;
  char *s;
  i32 i;
  for (i = 0; i < argc; i++) {
    if ((s = NuStrIStr(args[i], "BlowUp=")))
      blowup = s + NuStrLen("BlowUp=");
    else if ((s = NuStrIStr(args[i], "Special=")))
      special = s + NuStrLen("Special=");
    else if (!NuStrICmp(args[i], "FALSE"))
      loop = 0;
  }
  if (special || blowup)
    PlayRadio(special, blowup, loop);
}

#include "worldinfo_unk.h"

struct GIZFORCESYS_s {
  u8 pad0[0xe];
  u16 count; // 0x0e
};

typedef struct nufpar_s {
  u8 pad0[0x910];
  char *word_buf; // 0x910
} NUFPAR;

typedef struct nufpcomjmp_s {
  char *fn_name;
  void (*fn)(NUFPAR *parser);
} NUFPCOMJMP;

// GLOBAL: LEGOBATMAN 0x0093e148
extern i32 GizForceSFX_load_version;
// GLOBAL: LEGOBATMAN 0x009c6f3c
extern WORLDINFO_s *GizForceSFX_worldinfo;
// GLOBAL: LEGOBATMAN 0x009c6f38
extern void *GizForceSFX_force;
// GLOBAL: LEGOBATMAN 0x0093e14c
extern NUFPCOMJMP GizForceSFX_ConfigKeywords[];

NUFPAR *NuFParCreateMem(char *name, char *buffer, i32 bufferSize);
i32 NuFParPushCom(NUFPAR *parser, NUFPCOMJMP *commands);
i32 NuFParGetLine(NUFPAR *parser);
i32 NuFParGetWord(NUFPAR *parser);
i32 NuFParInterpretWord(NUFPAR *parser);
void NuFParPopCom(NUFPAR *parser);
void NuFParDestroy(NUFPAR *parser);

// from saga legoapi/gizmos/traps/gizforce.cpp
// FUNCTION: LEGOBATMAN 0x00486350
void GizForceSFX_Configure(WORLDINFO_s *world, char *config) {
  if (GizForceSFX_load_version >= 16 || world == 0 ||
      world->giz_force_sys == 0 || world->giz_force_sys->count == 0)
    return;
  GizForceSFX_force = 0;
  GizForceSFX_worldinfo = world;
  NUFPAR *parser = NuFParCreateMem("ForceSFX", config, 0xffff);
  if (parser == 0)
    return;
  NuFParPushCom(parser, GizForceSFX_ConfigKeywords);
  i32 inside = 0;
  while (NuFParGetLine(parser)) {
    while (NuFParGetWord(parser)) {
      if (inside) {
        if (NuStrICmp(parser->word_buf, "forcesfx_end") == 0) {
          inside = 0;
        } else {
          NuFParInterpretWord(parser);
        }
      } else if (NuStrICmp(parser->word_buf, "forcesfx_start") == 0) {
        inside = 1;
      }
    }
  }
  NuFParPopCom(parser);
  NuFParDestroy(parser);
}
