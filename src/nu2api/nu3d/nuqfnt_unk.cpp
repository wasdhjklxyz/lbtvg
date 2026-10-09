// nu2api/nu3d/nuqfnt_unk.cpp: saga keeps NuQFntDuplicate in
// nu2api/nu3d/nuqfnt.cpp; file name unproven.

#include "../nucore/common.h"
#include <stddef.h>

// An empty function (0x006fceb0 is a lone `ret`), see nutexanm_gen.cpp.
static void NuErrorUnk006fceb0(...) {}

typedef struct nuattrib_s {
  u32 a; // 0x40
  u32 b; // 0x44
} NUATTRIB;

typedef struct numtl_s {
  u8 pad0[0x40];
  NUATTRIB attribs; // 0x40
  u8 pad48[0x74 - 0x48];
  u16 tex_id;   // 0x74
  u16 sort_pri; // 0x76
  u8 pad78[0x2c0 - 0x78];
  u16 version; // 0x2c0
} NUMTL;

typedef struct vufnt_s {
  u8 pad0[6];
  u16 flags; // 0x06
  i32 size;  // 0x08
  u8 padc[0x34 - 0xc];
  u32 glyphs;      // 0x34
  u32 unicode_map; // 0x38
  u8 pad3c[0x40 - 0x3c];
  NUMTL *mtl; // 0x40
} VUFNT;

// GLOBAL: LEGOBATMAN 0x029f4640
extern i32 g_nuMtlRenderPlane;

void NuMemCpy(unsigned char *dst, unsigned char *src, int n);
NUMTL *NuMtlCreateUnk00727b60(i32 is3d);
void NuMtlUpdatePS(NUMTL *mtl);

static inline i32 NuMtlSetCurrentRenderPlane(i32 plane) {
  if (plane >= 0x18 || plane < 0)
    NuErrorUnk006fceb0();
  i32 old = g_nuMtlRenderPlane;
  g_nuMtlRenderPlane = plane;
  return old;
}

static inline void NuMtlUpdate(NUMTL *mtl) {
  NuMtlUpdatePS(mtl);
  mtl->version++;
}

// from saga nu2api/nu3d/nuqfnt.cpp
// STUB: LEGOBATMAN 0x0072e150
// the empty NuErrorUnk006fceb0 calls get optimised away here (orig keeps
// them and relies on eax surviving); rest of the body unverified.
VUFNT *NuQFntDuplicate(VUFNT *font, i32 flags, i32 render_plane, VARIPTR *buf,
                       VARIPTR *buf_end) {
  if (font == NULL)
    NuErrorUnk006fceb0();
  VUFNT *duplicate = (VUFNT *)((buf->addr + 0xf) & ~0xf);
  buf->void_ptr = duplicate;
  buf->addr += font->size;

  if (buf->addr < buf_end->addr) {
    NuMemCpy((unsigned char *)duplicate, (unsigned char *)font, font->size);
    duplicate->flags |= 1;
    duplicate->glyphs += (u32)duplicate - (u32)font;
    duplicate->unicode_map += (u32)duplicate - (u32)font;
  }

  i32 previous_render_plane = NuMtlSetCurrentRenderPlane(render_plane);
  NUMTL *material = NuMtlCreateUnk00727b60((flags & 8) ? 1 : 0);
  duplicate->mtl = material;
  material->attribs = font->mtl->attribs;
  duplicate->mtl->tex_id = font->mtl->tex_id;
  duplicate->mtl->sort_pri = font->mtl->sort_pri;
  duplicate->mtl->attribs.a = (duplicate->mtl->attribs.a & ~0x10000) | 0x20000;
  if (flags & 8) {
    duplicate->mtl->attribs.b &= ~0x800000;
  }
  if (flags & 0x40) {
    duplicate->mtl->attribs.a &= ~0xc000;
  }
  NuMtlUpdate(duplicate->mtl);
  NuMtlSetCurrentRenderPlane(previous_render_plane);
  return duplicate;
}
