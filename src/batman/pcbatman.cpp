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
