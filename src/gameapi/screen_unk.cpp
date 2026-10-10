// gameapi/, file unknown: saga legoapi/render/core/screen.cpp
// (NeedScreenGrab, DrawStillScreen), 0x5a5990..

#include "../nu2api/nucore/common.h"
#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x005a3aa0
static f32 NuSinApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x005a3b40
static f32 NuCosApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x005a3b50
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

#include "fade_unk.h"

struct numtl_s;

// Fade class inline members (fade_unk.h), emitted here in class order: ctor,
// dtor, GetFadeType, scalar deleting dtor (not annotatable by match.py yet).

// FUNCTION: LEGOBATMAN 0x005a3c10
FadeBase::FadeBase() : info(NULL) {}

// FUNCTION: LEGOBATMAN 0x005a3c20
FadeBase::~FadeBase() {}

// FUNCTION: LEGOBATMAN 0x005a3c50
Fade::Fade() {}

// FUNCTION: LEGOBATMAN 0x005a3c60
Fade::~Fade() {}

// FUNCTION: LEGOBATMAN 0x005a3c70
i32 Fade::GetFadeType() const { return 0; }

// FUNCTION: LEGOBATMAN 0x005a3ca0
BlackWipe::BlackWipe() {}

// FUNCTION: LEGOBATMAN 0x005a3cb0
BlackWipe::~BlackWipe() {}

// FUNCTION: LEGOBATMAN 0x005a3cc0
i32 BlackWipe::GetFadeType() const { return 1; }

// FUNCTION: LEGOBATMAN 0x005a3cf0
StillScreenWipe::StillScreenWipe() {}

// FUNCTION: LEGOBATMAN 0x005a3d00
StillScreenWipe::~StillScreenWipe() {}

// FUNCTION: LEGOBATMAN 0x005a3d10
i32 StillScreenWipe::GetFadeType() const { return 2; }

// FUNCTION: LEGOBATMAN 0x005a3d40
CrossFade::CrossFade() {}

// FUNCTION: LEGOBATMAN 0x005a3d50
CrossFade::~CrossFade() {}

// FUNCTION: LEGOBATMAN 0x005a3d60
i32 CrossFade::GetFadeType() const { return 5; }

// FUNCTION: LEGOBATMAN 0x005a3d90
BlackCrossFade::BlackCrossFade() {}

// FUNCTION: LEGOBATMAN 0x005a3da0
BlackCrossFade::~BlackCrossFade() {}

// FUNCTION: LEGOBATMAN 0x005a3db0
i32 BlackCrossFade::GetFadeType() const { return 6; }

// FUNCTION: LEGOBATMAN 0x005a3de0
SpinWipe::SpinWipe() {}

// FUNCTION: LEGOBATMAN 0x005a3df0
SpinWipe::~SpinWipe() {}

// FUNCTION: LEGOBATMAN 0x005a3e00
i32 SpinWipe::GetFadeType() const { return 4; }

// FUNCTION: LEGOBATMAN 0x005a3e30
StillScreen::StillScreen() {}

// FUNCTION: LEGOBATMAN 0x005a3e40
StillScreen::~StillScreen() {}

// FUNCTION: LEGOBATMAN 0x005a3e50
i32 StillScreen::GetFadeType() const { return 3; }

// GLOBAL: LEGOBATMAN 0x0095f8f0
Fade g_unk0095f8f0;
// GLOBAL: LEGOBATMAN 0x0095f8f8
BlackWipe g_unk0095f8f8;
// GLOBAL: LEGOBATMAN 0x0095f900
StillScreenWipe g_unk0095f900;
// GLOBAL: LEGOBATMAN 0x0095f908
StillScreen g_unk0095f908;
// GLOBAL: LEGOBATMAN 0x0095f910
SpinWipe g_unk0095f910;
// GLOBAL: LEGOBATMAN 0x0095f918
CrossFade g_unk0095f918;
// GLOBAL: LEGOBATMAN 0x0095f920
BlackCrossFade g_unk0095f920;

// GLOBAL: LEGOBATMAN 0x00a97d2c
u8 ScreenGrabNeeded;

// GLOBAL: LEGOBATMAN 0x00a97c54
extern f32 MainRenderTime;
// GLOBAL: LEGOBATMAN 0x00a97bfc
extern numtl_s *pause_rndr_mtl;
// GLOBAL: LEGOBATMAN 0x00a97db0
extern i32 g_unk00a97db0; // screen rect x
// GLOBAL: LEGOBATMAN 0x00a97db4
extern i32 g_unk00a97db4; // screen rect y
// GLOBAL: LEGOBATMAN 0x0095f9f8
extern i32 g_unk0095f9f8; // screen rect width
// GLOBAL: LEGOBATMAN 0x0095f9fc
extern i32 g_unk0095f9fc; // screen rect height

i32 NuRndrBeginScene(i32 flags);
void *NuVpGetCurrentViewport();
void NuRndrClear(i32 clear_flags, i32 background_colour, f32 alpha);
void NuRndrRectUV2di(i32 x, i32 y, i32 width, i32 height, f32 u0, f32 v0,
                     f32 u1, f32 v1, i32 colour, numtl_s *material);
void NuRndrGradRectUV2di(i32 x, i32 y, i32 width, i32 height, f32 u0, f32 v0,
                         f32 u1, f32 v1, i32 *colours, numtl_s *material);
void NuRndrEndScene();

// FUNCTION: LEGOBATMAN 0x005a5990
void NeedScreenGrab(i32 needed) { ScreenGrabNeeded = needed != 0; }

// FUNCTION: LEGOBATMAN 0x005a59b0
void DrawStillScreen(i32 clear) {
  NuRndrBeginScene(1);
  NuVpGetCurrentViewport();
  if (clear != 0)
    NuRndrClear(0x500, 0, 1.0f);
  if (MainRenderTime < 1.0f) {
    i32 colour = ((i32)(MainRenderTime * 128.0f) << 24) | 0x808080;
    i32 colours[4] = {colour, colour, colour, colour};
    NuRndrGradRectUV2di(g_unk00a97db0, g_unk00a97db4, g_unk0095f9f8,
                        g_unk0095f9fc, 0.0f, 0.0f, 1.0f, 1.0f, colours,
                        pause_rndr_mtl);
  } else {
    NuRndrRectUV2di(g_unk00a97db0, g_unk00a97db4, g_unk0095f9f8, g_unk0095f9fc,
                    0.0f, 0.0f, 1.0f, 1.0f, 0x80808080, pause_rndr_mtl);
  }
  NuRndrEndScene();
}

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_screen_unk(f32 *v, f32 a, i32 i) {
  v[0] = NuSinApprox(i);
  v[1] = NuCosApprox(i);
  NuVec4Set(v, a, a, a, a);
}
