// nu2api/nucore/pc/nutime_pc.cpp: the QueryPerformanceCounter layer under
// nutime.cpp (saga's nutime_android.c names the *PS entry points).

#include "../common.h"
#include <windows.h>

typedef struct nutime_s {
  u32 low;
  i32 high;
} NUTIME;

void NuTimeGet(NUTIME *t);
float NuTimeSeconds(NUTIME *t);

// FUNCTION: LEGOBATMAN 0x006e3b30
extern "C" unsigned int NuTimeGetTime(void) {
  NUTIME t;
  NuTimeGet(&t);
  return (unsigned int)(NuTimeSeconds(&t) * 1000.0f);
}

// GLOBAL: LEGOBATMAN 0x00b05498
extern LARGE_INTEGER g_nuTimeStart;
// GLOBAL: LEGOBATMAN 0x00b039b0
extern LARGE_INTEGER g_nuTimeFrequency;

// Ticks since NuTimeInitPS, wrapping past the 63-bit limit.
// STUB: LEGOBATMAN 0x006e3b80
// close: ours folds MAX - start + now into now - start + MAX on the wrap
// path; tried a separate local and both operand orders
void NuTimeGetTicksPS(u32 *low, u32 *high) {
  LARGE_INTEGER now;
  QueryPerformanceCounter(&now);
  i64 ticks = now.QuadPart;
  if (ticks > g_nuTimeStart.QuadPart)
    ticks -= g_nuTimeStart.QuadPart;
  else {
    i64 wrap = 0x7fffffffffffffff - g_nuTimeStart.QuadPart;
    ticks += wrap;
  }
  *low = (u32)ticks;
  *high = (u32)(ticks >> 32);
}

// FUNCTION: LEGOBATMAN 0x006e3bf0
void NuTimeGetTicksPerSecondPS(u32 *low, u32 *high) {
  *low = (u32)g_nuTimeFrequency.QuadPart;
  *high = (u32)(g_nuTimeFrequency.QuadPart >> 32);
}

// Samples the counter pinned to CPU 0, then restores the affinity.
// FUNCTION: LEGOBATMAN 0x006e3c10
void NuTimeInitPS(void) {
  DWORD_PTR process_mask = 1;
  DWORD_PTR system_mask = 1;
  if (!GetProcessAffinityMask(GetCurrentProcess(), &process_mask, &system_mask))
    process_mask = 1;
  SetProcessAffinityMask(GetCurrentProcess(), 1);
  QueryPerformanceCounter(&g_nuTimeStart);
  QueryPerformanceFrequency(&g_nuTimeFrequency);
  SetProcessAffinityMask(GetCurrentProcess(), process_mask);
}

// Mac order after NuTimeInitPS: NuLanguageInitPS, NuLanguageSetPS,
// NuRegionSetPS, NuLanguageConsoleSelectable.

// GLOBAL: LEGOBATMAN 0x0094c044
extern i32 g_nuPCLanguageDefault;

i32 NuPCDetermineLanguage(i32 language);

// FUNCTION: LEGOBATMAN 0x006e3c80
void NuLanguageInitPS(void) { NuPCDetermineLanguage(g_nuPCLanguageDefault); }

// FUNCTION: LEGOBATMAN 0x006e3c90
void NuLanguageSetPS(void) {}

// FUNCTION: LEGOBATMAN 0x006e3ca0
void NuRegionSetPS(void) {}

// FUNCTION: LEGOBATMAN 0x006e3cb0
i32 NuLanguageConsoleSelectable(void) { return 1; }
