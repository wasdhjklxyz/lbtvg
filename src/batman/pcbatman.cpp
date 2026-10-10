#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x00404330
static f32 NuSinApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x004043f0
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);
// pcbatman.cpp: first object linked; 0x00401000..0x00408900 by link order.

struct Unk00404420 {
  float f0;
  float f4;
  float f8;
  float fc;

  Unk00404420 *Set(float a, float b, float c, float d);
};

// `return this` is what produces the `mov eax, ecx` before the stores.
// FUNCTION: LEGOBATMAN 0x00404420
Unk00404420 *Unk00404420::Set(float a, float b, float c, float d) {
  f0 = a;
  f4 = b;
  f8 = c;
  fc = d;
  return this;
}

struct Unk00404450 {
  float f0;
  float f4;
  float f8;
  float fc;
  float f10;
  float f14;
  float f18;
  float f1c;
  float f20;
  float f24;
  float f28;
  float f2c;
  float f30;
  float f34;
  float f38;
  float f3c;

  Unk00404450 *Set(float a, float b, float c, float d, float e, float f,
                   float g, float h, float i, float j, float k, float l,
                   float m, float n, float o, float p);
};

// FUNCTION: LEGOBATMAN 0x00404450
Unk00404450 *Unk00404450::Set(float a, float b, float c, float d, float e,
                              float f, float g, float h, float i, float j,
                              float k, float l, float m, float n, float o,
                              float p) {
  f0 = a;
  f4 = b;
  f8 = c;
  fc = d;
  f10 = e;
  f14 = f;
  f18 = g;
  f1c = h;
  f20 = i;
  f24 = j;
  f28 = k;
  f2c = l;
  f30 = m;
  f34 = n;
  f38 = o;
  f3c = p;
  return this;
}

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_pcbatman(f32 *v, f32 a, i32 i) {
  v[0] = NuSinApprox(i);
  NuVec4Set(v, a, a, a, a);
}
