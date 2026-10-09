// gameapi/, file unknown: backdrop keyword parsers (0x00657f60..0x006581e0),
// reached through the Backdrop_ keyword table.

#include "../nu2api/nucore/common.h"

typedef struct nufpar_s NUFPAR;

i32 NuFParGetInt(NUFPAR *parser);
f32 NuFParGetFloat(NUFPAR *parser);

struct BACKDROP_s {
  u8 pad0[0x4b8];
  f32 velx; // 0x4b8
  f32 vely; // 0x4bc
  f32 velz; // 0x4c0
  u8 pad4c4[0x4d0 - 0x4c4];
  f32 sizex;  // 0x4d0
  f32 sizey;  // 0x4d4
  f32 sizez;  // 0x4d8
  i32 use_bb; // 0x4dc
  u8 pad4e0[0x4e8 - 0x4e0];
  f32 delete_dist; // 0x4e8
};

// The backdrop piece being configured.
// GLOBAL: LEGOBATMAN 0x00ad1bfc
extern BACKDROP_s *g_unk00ad1bfc;

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
