// gameapi/, file unknown: saga legoapi/render/core/screen.cpp
// (NeedScreenGrab, DrawStillScreen), 0x5a5990..

#include "../nu2api/nucore/common.h"

struct numtl_s;

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
