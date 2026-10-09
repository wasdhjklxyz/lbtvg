// batman/pc/windows.cpp (leaked __FILE__ ".\windows.cpp", anchor 0x00527d60).

#include "../../nu2api/nucore/common.h"
#include <string.h>

extern "C" void *NuMemAllocFn(int size, const char *file, int line);

struct CURSORTEX_s {
  u8 pad0[0x74];
  void *bits; // 0x74
  i32 size;   // 0x78
  u8 pad7c[0x80 - 0x7c];
};

struct CURSORMTL_s {
  u8 pad0[0x40];
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
  u8 pad44[0x54 - 0x44];
  f32 f54; // 0x54
  f32 f58; // 0x58
  f32 f5c; // 0x5c
  u8 pad60[0x70 - 0x60];
  f32 f70; // 0x70
  u16 tid; // 0x74
};

// GLOBAL: LEGOBATMAN 0x0094d3c8
extern i32 g_mouseCursorTid; // -1 until created
// GLOBAL: LEGOBATMAN 0x0094c320
extern u8 g_mouseCursorBits[0x1080];
// GLOBAL: LEGOBATMAN 0x009d1368
extern CURSORMTL_s *g_mouseCursorMtl;

i32 Unk00728220(CURSORTEX_s *tex);
CURSORMTL_s *NuMtlCreate(i32 count);
void NuMtlUpdate(CURSORMTL_s *mtl);

// FUNCTION: LEGOBATMAN 0x00527d60
void PCCreateMouseCursorMaterial() {
  if (g_mouseCursorTid == -1) {
    CURSORTEX_s *tex = (CURSORTEX_s *)NuMemAllocFn(0x80, __FILE__, 0x679);
    memset(tex, 0, 0x80);
    tex->bits = g_mouseCursorBits;
    tex->size = 0x1080;
    g_mouseCursorTid = Unk00728220(tex);
  }
  g_mouseCursorMtl = NuMtlCreate(1);
  g_mouseCursorMtl->f54 = 1.0f;
  g_mouseCursorMtl->f58 = 1.0f;
  g_mouseCursorMtl->f5c = 1.0f;
  g_mouseCursorMtl->attrib12 = 2;
  g_mouseCursorMtl->z_mode = 3;
  g_mouseCursorMtl->f70 = 1.0f;
  g_mouseCursorMtl->filter_mode = 1;
  g_mouseCursorMtl->attrib18 = 1;
  g_mouseCursorMtl->tid = g_mouseCursorTid;
  NuMtlUpdate(g_mouseCursorMtl);
}
