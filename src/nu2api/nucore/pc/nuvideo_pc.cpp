// nu2api/nucore/pc/nuvideo_pc.cpp: 0x006e2ab0..0x006e2ae0, between nuerror
// and bgproc. Mac order: NuVideoSetSwapModePS, NuVideoSetBrightnessPS,
// NuVideoGetRetraceRatePS.

#include "../nuapi_unk.h"

void Unk005299b0(f32 brightness);

// FUNCTION: LEGOBATMAN 0x006e2ab0
void NuVideoSetSwapModePS(i32 mode) {}

// FUNCTION: LEGOBATMAN 0x006e2ac0
void NuVideoSetBrightnessPS(void) { Unk005299b0(nuapi.video_brightness); }

// FUNCTION: LEGOBATMAN 0x006e2ae0
f32 NuVideoGetRetraceRatePS(void) { return 60.0f; }
