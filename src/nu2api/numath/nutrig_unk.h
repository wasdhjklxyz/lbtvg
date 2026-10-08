#pragma once
// NuSinApprox is a static function (Mac nm: 't') emitted once per including TU;
// each copy is annotated in its own .cpp. Real header name unproven.

#include "../nucore/common.h"

static f32 NuSinApprox(i32 angle) {
  f32 x;
  f64 x2;
  f64 x3;
  f64 x5;
  f64 x7;
  f64 x9;

  angle &= 0xffff;
  if (angle > 0xc000)
    angle -= 0xc000;
  else if (angle > 0x4000)
    angle = 0xc000 - angle;
  else
    angle += 0x4000;
  x = (f32)angle * (2.0 * 3.1415927f / 65536.0);
  x = x - 3.1415927f / 2.0;
  x2 = x * x;
  x3 = x * x2;
  x5 = x3 * x2;
  x7 = x5 * x2;
  x9 = x7 * x2;
  return x + (x3 * -0.16666657f) + (x5 * 0.0083330255f) +
         (x7 * -0.00019807414f) + (x9 * 2.601887e-06f);
}
