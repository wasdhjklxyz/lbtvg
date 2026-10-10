// batman/pc/d3dCore.cpp (leaked __FILE__ anchor 0x0052bbf0). CNuConsole
// lives in another TU: PreInitialize does not know ExitNow never returns.

#include "../../nu2api/nucore/common.h"
#include <math.h>
#include <stddef.h>
#include <windows.h>

#include "../../nu2api/numath/nuinline_unk.h"
#include "../../nu2api/numath/nutrig_unk.h"
#include <d3d9.h>

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x00529df0
static f32 NuSinApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x00529e90
static f32 NuCosApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x00529ea0
static void NuVec4Copy(f32 *dst, f32 *src);
// FUNCTION: LEGOBATMAN 0x00529ec0
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);
// FUNCTION: LEGOBATMAN 0x00529ee0
static void NuVecScaleInline(f32 *dst, f32 *src, f32 s);

class CNuConsole {
public:
  void ExitNow(char *fmt, ...);
};

class CD3DCore {
public:
  i32 FindNearestMode(unsigned int width, unsigned int height,
                      unsigned int depth) const;
  void SetNextDisplayMode(unsigned int width, unsigned int height,
                          unsigned int depth);
  i32 DetermineNominalAspectRatio(unsigned int width,
                                  unsigned int height) const;
  void PreInitialize();

  u8 pad0[4];
  IDirect3D9 *d3d; // 0x004
  u8 pad8[0x10 - 8];
  u8 caps[0x130]; // 0x010, D3DCAPS9
  u8 pad140[0x1ac - 0x140];
  u8 identifier[0x450]; // 0x1ac, D3DADAPTER_IDENTIFIER9
  u8 pad5fc[0x608 - 0x5fc];
  D3DDISPLAYMODE desktop_mode; // 0x608
  i32 mode_count;              // 0x618
  u8 pad61c[4];
  i32 current_mode;    // 0x620
  i32 nominal_aspect;  // 0x624
  f32 aspect;          // 0x628
  i32 nominal_aspect2; // 0x62c
  f32 aspect2;         // 0x630
  u8 pad634[0x668 - 0x634];
  i32 next_mode; // 0x668
};

// GLOBAL: LEGOBATMAN 0x009d0380
extern CNuConsole g_nuConsole;

// FUNCTION: LEGOBATMAN 0x0052c0a0
void CD3DCore::PreInitialize() {
  d3d = Direct3DCreate9(D3D_SDK_VERSION);
  if (d3d == NULL)
    g_nuConsole.ExitNow("Failed to initialize DirectX");
  d3d->GetDeviceCaps(0, D3DDEVTYPE_HAL, (D3DCAPS9 *)caps);
  d3d->GetAdapterIdentifier(0, 0, (D3DADAPTER_IDENTIFIER9 *)identifier);
  d3d->GetAdapterDisplayMode(0, &desktop_mode);
  u32 width = desktop_mode.Width;
  u32 height = desktop_mode.Height;
  nominal_aspect = DetermineNominalAspectRatio(width, height);
  f32 ratio = (f32)width / (f32)height;
  nominal_aspect2 = nominal_aspect;
  aspect = ratio;
  aspect2 = ratio;
}

// FUNCTION: LEGOBATMAN 0x0052c1f0
void CD3DCore::SetNextDisplayMode(unsigned int width, unsigned int height,
                                  unsigned int depth) {
  i32 mode = FindNearestMode(width, height, depth);
  if (mode >= mode_count)
    mode = current_mode;
  next_mode = mode;
}

// 0: 4:3, 1: 16:9, 2: 16:10, whichever the desktop is closest to.
// STUB: LEGOBATMAN 0x0052a7a0
// close: ours stores/reloads each fabsf result once more than the orig
// (fabs, fabsf, fabsf of the difference tried)
i32 CD3DCore::DetermineNominalAspectRatio(unsigned int width,
                                          unsigned int height) const {
  i32 result = 0;
  f32 ratio = (f32)width / (f32)height;
  f32 best = 1000.0f;
  f32 diff;
  diff = fabsf(ratio - 1.3333334f);
  if (diff < best) {
    best = diff;
    result = 0;
  }
  diff = fabsf(ratio - 1.7777778f);
  if (diff < best) {
    best = diff;
    result = 1;
  }
  diff = fabsf(ratio - 1.6f);
  if (diff < best)
    result = 2;
  return result;
}

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_d3dCore(f32 *v, f32 a, i32 i) {
  v[0] = NuSinApprox(i);
  v[1] = NuCosApprox(i);
  NuVec4Copy(v + 4, v);
  NuVec4Set(v, a, a, a, a);
  NuVecScaleInline(v + 8, v, a);
}
