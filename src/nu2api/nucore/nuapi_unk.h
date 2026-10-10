#pragma once
// nuapi (saga NUAPI, ref/saga/src/nu2api/nucore/nuapi.h): one 0xf4-byte
// struct at 0x00adf648. Batman's layout differs from saga's; only fields with
// evidence in this tree are named, the rest is u32 padding.

#include "common.h"

typedef struct nutime_s {
  u32 low;
  i32 high;
} NUTIME;

// saga NUPADREC; 0x54 in nuapi.
typedef struct nupadrec_s {
  i32 mode;               // 0x0: 0 normal, 1 record, 2 play
  u8 *buf_start;          // 0x4
  u8 *buf_end;            // 0x8
  VARIPTR record;         // 0xc
  i32 buf_size;           // 0x10
  char *filepath;         // 0x14
  i32 should_end;         // 0x18
  i32 end_record_buttons; // 0x1c
  i32 end_play_buttons;   // 0x20
  i32 is_recording;       // 0x24
} NUPADREC;

typedef struct nuapi_s {
  i32 language;               // 0x00
  i32 unk04;                  // 0x04
  i32 video_mode;             // 0x08
  i32 video_swap_mode;        // 0x0c
  i32 video_aspect;           // 0x10
  i32 screen_width;           // 0x14
  i32 screen_height;          // 0x18
  f32 fps;                    // 0x1c
  i32 nuframe_begin_cnt;      // 0x20
  u32 pad24;                  // 0x24
  NUTIME time;                // 0x28: frame start
  u32 pad30[2];               // 0x30
  NUTIME time2;               // 0x38
  f32 frametime;              // 0x40
  i32 frame_count;            // 0x44
  f32 forced_frame_time;      // 0x48
  f32 unk4c;                  // 0x4c: frame time summed while 0x029f3ee4 == 0
  i32 unk50;                  // 0x50
  NUPADREC pad_record;        // 0x54
  u32 pad7c[2];               // 0x7c
  void *wind;                 // 0x84: NUWIND *, NuWindAnimate(nuapi.wind, ...)
  u32 pad88[3];               // 0x88
  i32 disable_os_menu_freeze; // 0x94
  f32 video_brightness;       // 0x98
  u32 pad9c[2];               // 0x9c
  i32 unka4;                  // 0xa4
  i32 unka8;                  // 0xa8
  i32 multi_thread_render;    // 0xac
  u32 padb0;                  // 0xb0
  i32 unkb4;                  // 0xb4
  f32 unkb8[4];               // 0xb8
  void *current_gscene;       // 0xc8
  i32 unkcc;                  // 0xcc
  u32 padd0[4];               // 0xd0
  i32 unke0[4];               // 0xe0
  f32 unkf0;                  // 0xf0
} NUAPI;

extern NUAPI nuapi;
