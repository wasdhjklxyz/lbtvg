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
