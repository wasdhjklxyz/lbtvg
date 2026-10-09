// gameapi/faders_unk.cpp: placed by tools/new.py; file name unproven
// (saga keeps it in legoapi/render/light/faders.cpp).

#include "../batman/worldinfo_unk.h"
#include "../nu2api/nu3d/nuspecial.h"
#include "../nu2api/nucore/common.h"
#include <stddef.h>

typedef struct nufpar_s {
  u8 pad0[0x910];
  char *word_buf; // 0x910
} NUFPAR;

struct FADER_s {
  nuhspecial_s special;         // 0x00
  nuhspecial_s while_animating; // 0x0c
  nuhspecial_s draw_at;         // 0x18
  u8 flags;                     // 0x24
  u8 pad25[3];
};

NUFPAR *NuFParCreateMem(char *name, char *buffer, i32 bufferSize);
i32 NuFParGetLine(NUFPAR *parser);
i32 NuFParGetWord(NUFPAR *parser);
i32 NuStrICmp(const char *a, const char *b);
void NuFParDestroy(NUFPAR *parser);

// from saga legoapi/render/light/faders.cpp
// STUB: LEGOBATMAN 0x00655790
// logic matches; regalloc differs (orig keeps &fader->draw_at as a second
// induction variable in esi and spills fader).
void Faders_Configure(WORLDINFO_s *world, char *config) {
  world->faders = NULL;
  world->fader_count = 0;
  if (world->scn140 == NULL)
    return;
  NUFPAR *parser = NuFParCreateMem("faders", config, 0xffff);
  if (parser == NULL)
    return;
  world->buf104.addr = (world->buf104.addr + 3) & ~3;
  world->faders = (FADER_s *)world->buf104.addr;
  FADER_s *fader = world->faders;
  while (NuFParGetLine(parser) != 0) {
    NuFParGetWord(parser);
    if (NuStrICmp(parser->word_buf, "fader") != 0)
      continue;
    fader->special.scene = NULL;
    fader->special.special = NULL;
    fader->special.display_special = NULL;
    fader->while_animating.scene = NULL;
    fader->while_animating.special = NULL;
    fader->while_animating.display_special = NULL;
    fader->draw_at.scene = NULL;
    fader->draw_at.special = NULL;
    fader->draw_at.display_special = NULL;
    fader->flags &= ~1;
    if (NuFParGetWord(parser) == 0 ||
        NuSpecialFind(world->scn140, &fader->special, parser->word_buf, 1) == 0)
      continue;
    while (NuFParGetWord(parser) != 0) {
      if (NuStrICmp(parser->word_buf, "while_animating") == 0) {
        if (NuFParGetWord(parser) != 0)
          NuSpecialFind(world->scn140, &fader->while_animating,
                        parser->word_buf, 1);
      } else if (NuStrICmp(parser->word_buf, "draw_at") == 0) {
        if (NuFParGetWord(parser) != 0)
          NuSpecialFind(world->scn140, &fader->draw_at, parser->word_buf, 1);
      } else if (NuStrICmp(parser->word_buf, "fade_up") == 0) {
        fader->flags &= ~1;
      }
    }
    ++fader;
    ++world->fader_count;
  }
  NuFParDestroy(parser);
  if (world->fader_count > 0)
    world->buf104.addr = ((u32)fader + 3) & ~3;
  else
    world->faders = NULL;
}
