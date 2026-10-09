// gameapi/saveload_unk.cpp: placed by tools/new.py. The leaked __FILE__ says
// gameapi/gui/gamemenu/apisave.c (C linkage below to match).

#include "../nu2api/nucore/common.h"
#include <stddef.h>

extern "C" void *NuMemAllocFn(int size, const char *file, int line);

// GLOBAL: LEGOBATMAN 0x0099efd0
extern i32 SAVESLOTS;
// GLOBAL: LEGOBATMAN 0x00ad7418
extern void *memcard_hashfn;
// GLOBAL: LEGOBATMAN 0x00ad7408
extern void *memcard_savedata;
// GLOBAL: LEGOBATMAN 0x00ad7410
extern i32 memcard_savedatasize;
// GLOBAL: LEGOBATMAN 0x00ad740c
extern void *memcard_savedatabuffer;
// GLOBAL: LEGOBATMAN 0x00ad7414
extern i32 g_saveUnk00ad7414;
// GLOBAL: LEGOBATMAN 0x00ad741c
extern i32 g_saveUnk00ad741c;

// Batman's version: no header data, at most 6 slots.
// FUNCTION: LEGOBATMAN 0x006c1590
extern "C" void SaveSystemInitialiseEx(i32 slots, void *make_save_hash,
                                       void *save, i32 save_size, i32 arg5,
                                       i32 arg6) {
  SAVESLOTS = slots;
  if (slots >= 6)
    SAVESLOTS = 6;
  memcard_hashfn = make_save_hash;
  memcard_savedata = save;
  memcard_savedatasize = save_size;
  memcard_savedatabuffer = NuMemAllocFn(save_size + 4, __FILE__, 0x6d);
  g_saveUnk00ad7414 = arg5;
  g_saveUnk00ad741c = arg6;
}

void Text3DEx(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale,
              f32 z_scale, u32 alignment, u8 red, u8 green, u8 blue, i32 alpha,
              i32 a13, i32 a14);

// Batman's Text3DEx takes two more arguments than saga's.
// FUNCTION: LEGOBATMAN 0x006c53b0
extern "C" void Text3D(char *text, f32 x, f32 y, f32 z, f32 x_scale,
                       f32 y_scale, f32 z_scale, u32 alignment, u8 red,
                       u8 green, u8 blue) {
  Text3DEx(text, x, y, z, x_scale, y_scale, z_scale, alignment, red, green,
           blue, 128, -1, 0);
}

extern "C" void SmartTextExLimit(char *text, f32 x, f32 y, f32 z, f32 x_scale,
                                 f32 y_scale, f32 z_scale, u32 alignment,
                                 u8 red, u8 green, u8 blue, f32 max_width,
                                 i32 max_lines, void *message_box,
                                 i32 suppress_draw, u32 alpha, i32 limit,
                                 i32 a18);

// FUNCTION: LEGOBATMAN 0x006c5f00
extern "C" void SmartTextEx(char *text, f32 x, f32 y, f32 z, f32 x_scale,
                            f32 y_scale, f32 z_scale, u32 alignment, u8 red,
                            u8 green, u8 blue, f32 max_width, i32 max_lines,
                            void *message_box, i32 suppress_draw, u32 alpha) {
  SmartTextExLimit(text, x, y, z, x_scale, y_scale, z_scale, alignment, red,
                   green, blue, max_width, max_lines, message_box,
                   suppress_draw, alpha, -1, 0);
}
