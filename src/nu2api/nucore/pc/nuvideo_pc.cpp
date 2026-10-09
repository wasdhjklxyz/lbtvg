// nu2api/nucore/pc/nuvideo_pc.cpp: 0x006e2ab0..0x006e2ae0, between nuerror
// and bgproc. Mac order: NuVideoSetSwapModePS, NuVideoSetBrightnessPS,
// NuVideoGetRetraceRatePS.

#include "../common.h"

// nuapi.video_brightness (0x00adf6e0, defined in nupad_gen.cpp)
extern f32 g_nuPadUnk0adf6e0;

void Unk005299b0(f32 brightness);

// FUNCTION: LEGOBATMAN 0x006e2ab0
void NuVideoSetSwapModePS(i32 mode) {}

// FUNCTION: LEGOBATMAN 0x006e2ac0
void NuVideoSetBrightnessPS(void) { Unk005299b0(g_nuPadUnk0adf6e0); }

// FUNCTION: LEGOBATMAN 0x006e2ae0
f32 NuVideoGetRetraceRatePS(void) { return 60.0f; }
