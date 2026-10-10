// gameapi/techno_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nu2api/nu3d/nuspecial.h"
#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"
#include "gameobject_unk.h"
#include <stdio.h>

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x005a8c90
static f32 NuFabs(f32 f);
// FUNCTION: LEGOBATMAN 0x005a8cb0
static f32 NuSinApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x005a8d50
static f32 NuCosApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x005a8d60
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);
// FUNCTION: LEGOBATMAN 0x005a8dc0
static void NuVecScaleInline(f32 *dst, f32 *src, f32 s);

struct TECHNO_s {
  u8 pad0[0x83];
  u8 target_mode; // 0x83
  u8 pad84[0xb8 - 0x84];
  void *controlled_object; // 0xb8
};

i32 NuSpecialCompare(nuhspecial_s *a, nuhspecial_s *b);

// GLOBAL: LEGOBATMAN 0x00960558
extern i32 LEGOCONTEXT_TECHNO;
// GLOBAL: LEGOBATMAN 0x00ab3960
extern GameObject_s *Player[8];

// FUNCTION: LEGOBATMAN 0x005a7bd0
i32 Techno_FindOperator(void *target, Unk_GameObject112c **pad,
                        GameObject_s **operator_object) {
  if (LEGOCONTEXT_TECHNO == -1)
    return 0;
  for (i32 index = 0; index < 8; ++index) {
    if (Player[index] != 0 && Player[index]->b9db == LEGOCONTEXT_TECHNO &&
        Player[index]->techno != 0) {
      TECHNO_s *techno = Player[index]->techno;
      if ((techno->target_mode == 2 &&
           NuSpecialCompare((nuhspecial_s *)target,
                            (nuhspecial_s *)techno->controlled_object) != 0) ||
          Player[index]->techno->controlled_object == target) {
        if (pad != 0) {
          *pad = Player[index]->p112c;
        }
        if (operator_object != 0) {
          *operator_object = Player[index];
        }
        return 1;
      }
    }
  }
  return 0;
}

// Material view for the rope setup (bitfields as in menu_unk.cpp).
struct GRAPPLEMTL_s {
  u32 head; // 0x00
  u8 pad4[0x40 - 4];
  u32 filter_mode : 4; // 0x40
  u32 alpha_mode : 2;
  u32 attrib6 : 2;
  u32 attrib8 : 2;
  u32 attrib10 : 2;
  u32 attrib12 : 2;
  u32 z_mode : 2;
  u32 attrib16 : 2;
  u32 attrib18 : 1;
  u32 attrib19 : 13;
  u8 pad44[0x70 - 0x44];
  f32 f70; // 0x70
  u16 tid; // 0x74
};

// GLOBAL: LEGOBATMAN 0x00a984c0
extern GRAPPLEMTL_s *GrappleMtls[4];

GRAPPLEMTL_s *NuMtlCreate(i32 count);
void NuMtlUpdate(GRAPPLEMTL_s *mtl);
void Unk007280b0(GRAPPLEMTL_s *mtl);
i32 Unk006e6060(char *name, variptr_u *buffer, variptr_u buffer_end);

// FUNCTION: LEGOBATMAN 0x005aa1f0
void InitGrappleMtls(variptr_u *buffer, variptr_u *buffer_end) {
  char name[0x20];
  if (GrappleMtls[0] != 0)
    GrappleMtls[0]->head = 0x04040404;
  for (i32 i = 0; i < 4; i++) {
    GRAPPLEMTL_s *mtl = NuMtlCreate(1);
    if (mtl != 0) {
      mtl->f70 = 1.0f;
      mtl->filter_mode = 1;
      mtl->alpha_mode = 1;
      mtl->attrib8 = 0;
      mtl->attrib10 = 0;
      mtl->attrib12 = 2;
      mtl->z_mode = 0;
      mtl->attrib16 = 2;
      sprintf(name, "stuff\\Rope%i", i + 1);
      buffer->addr = (buffer->addr + 0xf) & ~0xf;
      i32 tid = Unk006e6060(name, buffer, *buffer_end);
      if (tid == 0) {
        Unk007280b0(mtl);
      } else {
        mtl->tid = tid;
        GrappleMtls[i] = mtl;
        NuMtlUpdate(mtl);
      }
    }
  }
}

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_techno_unk(f32 *v, f32 a, i32 i) {
  v[2] = NuFabs(a);
  v[0] = NuSinApprox(i);
  v[1] = NuCosApprox(i);
  NuVec4Set(v, a, a, a, a);
  NuVecScaleInline(v + 8, v, a);
}
