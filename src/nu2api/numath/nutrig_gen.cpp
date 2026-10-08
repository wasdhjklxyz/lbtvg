// nu2api/numath/nutrig_gen.cpp: functions ahead of the nutrig_gen.cpp
// __FILE__ anchor (0x0068fed0).

#include "../nucore/common.h"

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
