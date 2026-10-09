// nu2api/nucore/nutime.cpp (saga), between the gcutscn.cpp and nupad_gen.cpp
// anchors; identified by saga's bodies (no Mac symbols).

#include "common.h"

typedef struct nutime_s {
  u32 low;
  i32 high;
} NUTIME;

void NuTimeGetTicksPS(u32 *low, u32 *high);
void NuTimeGetTicksPerSecondPS(u32 *low, u32 *high);

// nuapi.frametime
// GLOBAL: LEGOBATMAN 0x00adf688
extern f32 g_nuapiFrameTime;

// nuapi.forced_frame_time
// GLOBAL: LEGOBATMAN 0x00adf690
extern f32 g_nuapiForcedFrameTime;

// GLOBAL: LEGOBATMAN 0x00b038cc
static u32 g_nuTimeStartFrame;

extern "C" unsigned int NuTimeGetTime(void);

// FUNCTION: LEGOBATMAN 0x006d4fe0
extern "C" void NuTimeStartFrame(void) { g_nuTimeStartFrame = NuTimeGetTime(); }

// FUNCTION: LEGOBATMAN 0x006d4ff0
extern "C" u32 NuTimeGetStartFrame(void) { return g_nuTimeStartFrame; }

// FUNCTION: LEGOBATMAN 0x006d5000
extern "C" u32 NuTimeGetSinceStartFrame(void) {
  return NuTimeGetTime() - g_nuTimeStartFrame;
}

// FUNCTION: LEGOBATMAN 0x006d5010
f32 NuTimeGetFrameTime(void) { return g_nuapiFrameTime; }

// FUNCTION: LEGOBATMAN 0x006d5020
void NuTimeForceFrameTime(f32 frame_time) {
  g_nuapiForcedFrameTime = frame_time;
}

// FUNCTION: LEGOBATMAN 0x006d5030
void NuTimeGet(NUTIME *t) { NuTimeGetTicksPS(&t->low, (u32 *)&t->high); }

// FUNCTION: LEGOBATMAN 0x006d5050
void NuTimeSub(NUTIME *t, NUTIME *a, NUTIME *b) {
  if (a->low >= b->low) {
    t->low = a->low - b->low;
    t->high = a->high - b->high;
  } else {
    t->low = a->low - b->low;
    t->high = a->high - b->high - 1;
  }
}

// FUNCTION: LEGOBATMAN 0x006d5090
f32 NuTimeSeconds(NUTIME *t) {
  u32 low;
  u32 high;
  f64 ticks;
  f64 ticks_per_second;
  f64 seconds;

  NuTimeGetTicksPerSecondPS(&low, &high);

  ticks = t->low + t->high * 4.294967295e+09;
  ticks_per_second = low + high * 4.294967295e+09;

  seconds = ticks / ticks_per_second;

  return seconds;
}

// FUNCTION: LEGOBATMAN 0x006d5100
f32 NuTimeMilliSeconds(NUTIME *t) {
  u32 low;
  u32 high;
  f64 ticks;
  f64 ticks_per_second;
  f64 millis;

  NuTimeGetTicksPerSecondPS(&low, &high);

  ticks = t->low + t->high * 4.294967295e+09;
  ticks_per_second = low + high * 4.294967295e+09;

  millis = ticks / ticks_per_second * 1000.0;

  return millis;
}

// FUNCTION: LEGOBATMAN 0x006d5170
f32 NuTimeMicroSeconds(NUTIME *t) {
  u32 low;
  u32 high;
  f64 ticks;
  f64 ticks_per_second;
  f64 micros;

  NuTimeGetTicksPerSecondPS(&low, &high);

  ticks = t->low + t->high * 4.294967295e+09;
  ticks_per_second = low + high * 4.294967295e+09;

  micros = ticks / ticks_per_second * 1000000.0;

  return micros;
}

// FUNCTION: LEGOBATMAN 0x006d51e0
f32 NuTimeScanlines(NUTIME *t) { return NuTimeSeconds(t) * 272.0f * 60.0f; }
