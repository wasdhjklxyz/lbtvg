// nu2api/numath/nutrig_gen.cpp: functions ahead of the nutrig_gen.cpp
// __FILE__ anchor (0x0068fed0).

#include "../nucore/common.h"
#include "./nutrig_unk.h"

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x0068f4c0
static f32 NuSinApprox(i32 angle);

// LUT helpers (static in saga nutrig.cpp); their own bodies are not matched
// yet.
u16 fxyd(f32 dx, f32 dy);
u16 fxyda(f32 dx, f32 dy);

// FUNCTION: LEGOBATMAN 0x0068fa00
i32 NuAtan2D(f32 dx, f32 dy) {
  if (dx == 0.0f)
    return dy < 0.0f ? 0x8000 : 0;
  if (dy == 0.0f)
    return dx < 0.0f ? 0xc000 : 0x4000;
  if (dx < 0.0f) {
    if (dy < 0.0f)
      return fxyd(-dx, -dy) + 0x8000;
    return -fxyd(-dx, dy);
  }
  if (dy < 0.0f)
    return 0x8000 - fxyd(dx, -dy);
  return fxyd(dx, dy);
}

// FUNCTION: LEGOBATMAN 0x0068fae0
i32 NuAtan2DA(f32 dx, f32 dy) {
  if (dx == 0.0f)
    return dy < 0.0f ? 0x8000 : 0;
  if (dy == 0.0f)
    return dx < 0.0f ? 0xc000 : 0x4000;
  if (dx < 0.0f) {
    if (dy < 0.0f)
      return fxyda(-dx, -dy) + 0x8000;
    return -fxyda(-dx, dy);
  }
  if (dy < 0.0f)
    return 0x8000 - fxyda(dx, -dy);
  return fxyda(dx, dy);
}

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_nutrig_gen(f32 *v, f32 a, i32 i) { v[0] = NuSinApprox(i); }
