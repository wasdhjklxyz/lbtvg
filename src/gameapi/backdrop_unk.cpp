// gameapi/, file unknown: backdrop keyword parsers (0x00657f60..0x006581e0),
// reached through the Backdrop_ keyword table.

#include "../nu2api/nucore/common.h"
#include "../nu2api/numath/nuinline_unk.h"

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x00657e40
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

typedef struct nufpar_s NUFPAR;

i32 NuFParGetInt(NUFPAR *parser);
f32 NuFParGetFloat(NUFPAR *parser);

struct nuhspecial_s {
  void *scene;
  void *special;
  void *display_special;
};

struct nufpar_s {
  u8 pad0[0x910];
  char *word_buf; // 0x910
};

i32 NuFParGetWord(NUFPAR *parser);
char *NuSpecialGetName(nuhspecial_s *sp);
i32 NuStrICmp(const char *a, const char *b);

struct BDPIECE_s {
  nuhspecial_s special;        // 0x00
  BDPIECE_s *followed_by[10];  // 0x0c
  BDPIECE_s *preceeded_by[10]; // 0x34
  i32 followed_count;          // 0x5c
  i32 preceeded_count;         // 0x60
};

struct BACKDROP_s {
  BDPIECE_s pieces[12]; // 0x000
  u8 pad4b0[0x4b8 - 0x4b0];
  f32 velx;       // 0x4b8
  f32 vely;       // 0x4bc
  f32 velz;       // 0x4c0
  f32 posx;       // 0x4c4
  f32 posy;       // 0x4c8
  f32 posz;       // 0x4cc
  f32 sizex;      // 0x4d0
  f32 sizey;      // 0x4d4
  f32 sizez;      // 0x4d8
  i32 use_bb;     // 0x4dc
  u8 piece_count; // 0x4e0
  u8 pad4e1[0x4e8 - 0x4e1];
  f32 delete_dist; // 0x4e8
};

// The backdrop piece being configured.
// GLOBAL: LEGOBATMAN 0x00ad1bfc
extern BACKDROP_s *g_unk00ad1bfc;
// The piece being configured.
// GLOBAL: LEGOBATMAN 0x00ad1c00
extern BDPIECE_s *g_unk00ad1c00;

struct nugscn_s;
i32 NuSpecialFind(nugscn_s *scene, nuhspecial_s *dest, char *name, i32 flags);

struct BDWORLD_s {
  u8 pad0[0x140];
  nugscn_s *current_gscn; // 0x140
};

// GLOBAL: LEGOBATMAN 0x00ad1c04
extern BDWORLD_s *g_unk00ad1c04;

// STUB: LEGOBATMAN 0x00657e60
// only the g_unk00ad1c00 store is scheduled earlier than the original
void Backdrop_piece(NUFPAR *parser) {
  if (NuFParGetWord(parser) && g_unk00ad1bfc != 0) {
    BDPIECE_s *piece = &g_unk00ad1bfc->pieces[g_unk00ad1bfc->piece_count++];
    g_unk00ad1c00 = piece;
    if (NuSpecialFind(g_unk00ad1c04->current_gscn, &piece->special,
                      parser->word_buf, 1) == 0)
      g_unk00ad1c00 = 0;
  }
}

// STUB: LEGOBATMAN 0x00657ed0
// only the g_unk00ad1c00 = 0 store is scheduled before the count compare
void Backdrop_piece2(NUFPAR *parser) {
  if (NuFParGetWord(parser) && g_unk00ad1bfc != 0) {
    g_unk00ad1c00 = 0;
    for (i32 i = 0; i < g_unk00ad1bfc->piece_count; i++) {
      char *name = NuSpecialGetName(&g_unk00ad1bfc->pieces[i].special);
      if (name != 0 && NuStrICmp(parser->word_buf, name) == 0) {
        g_unk00ad1c00 = &g_unk00ad1bfc->pieces[i];
        return;
      }
    }
  }
}

// FUNCTION: LEGOBATMAN 0x00657f60
void Backdrop_followed_by(NUFPAR *parser) {
  if (NuFParGetWord(parser) && g_unk00ad1bfc != 0 && g_unk00ad1c00 != 0 &&
      g_unk00ad1c00->followed_count < 10) {
    for (i32 i = 0; i < g_unk00ad1bfc->piece_count; i++) {
      char *name = NuSpecialGetName(&g_unk00ad1bfc->pieces[i].special);
      if (name != 0 && NuStrICmp(parser->word_buf, name) == 0) {
        BDPIECE_s *piece = g_unk00ad1c00;
        piece->followed_by[piece->followed_count] = &g_unk00ad1bfc->pieces[i];
        piece->followed_count++;
        return;
      }
    }
  }
}

// FUNCTION: LEGOBATMAN 0x00658000
void Backdrop_preceeded_by(NUFPAR *parser) {
  if (NuFParGetWord(parser) && g_unk00ad1bfc != 0 && g_unk00ad1c00 != 0 &&
      g_unk00ad1c00->preceeded_count < 10) {
    for (i32 i = 0; i < g_unk00ad1bfc->piece_count; i++) {
      char *name = NuSpecialGetName(&g_unk00ad1bfc->pieces[i].special);
      if (name != 0 && NuStrICmp(parser->word_buf, name) == 0) {
        BDPIECE_s *piece = g_unk00ad1c00;
        piece->preceeded_by[piece->preceeded_count] = &g_unk00ad1bfc->pieces[i];
        piece->preceeded_count++;
        return;
      }
    }
  }
}

// FUNCTION: LEGOBATMAN 0x00658100
void Backdrop_posx(NUFPAR *parser) {
  g_unk00ad1bfc->posx = NuFParGetFloat(parser);
}

// FUNCTION: LEGOBATMAN 0x00658120
void Backdrop_posy(NUFPAR *parser) {
  g_unk00ad1bfc->posy = NuFParGetFloat(parser);
}

// FUNCTION: LEGOBATMAN 0x00658140
void Backdrop_posz(NUFPAR *parser) {
  g_unk00ad1bfc->posz = NuFParGetFloat(parser);
}

// FUNCTION: LEGOBATMAN 0x006580a0
void Backdrop_velx(NUFPAR *parser) {
  g_unk00ad1bfc->velx = NuFParGetFloat(parser);
}

// FUNCTION: LEGOBATMAN 0x006580c0
void Backdrop_vely(NUFPAR *parser) {
  g_unk00ad1bfc->vely = NuFParGetFloat(parser);
}

// FUNCTION: LEGOBATMAN 0x006580e0
void Backdrop_velz(NUFPAR *parser) {
  g_unk00ad1bfc->velz = NuFParGetFloat(parser);
}

// FUNCTION: LEGOBATMAN 0x00658160
void Backdrop_sizex(NUFPAR *parser) {
  g_unk00ad1bfc->sizex = NuFParGetFloat(parser);
}

// FUNCTION: LEGOBATMAN 0x00658180
void Backdrop_sizey(NUFPAR *parser) {
  g_unk00ad1bfc->sizey = NuFParGetFloat(parser);
}

// FUNCTION: LEGOBATMAN 0x006581a0
void Backdrop_sizez(NUFPAR *parser) {
  g_unk00ad1bfc->sizez = NuFParGetFloat(parser);
}

// FUNCTION: LEGOBATMAN 0x006581c0
void Backdrop_useBB(NUFPAR *parser) {
  g_unk00ad1bfc->use_bb = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x006581e0
void Backdrop_deletedist(NUFPAR *parser) {
  g_unk00ad1bfc->delete_dist = NuFParGetFloat(parser);
}

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_backdrop_unk(f32 *v, f32 a, i32 i) {
  NuVec4Set(v, a, a, a, a);
}
