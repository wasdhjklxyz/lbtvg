// batman/, file unknown (between pcbatman.cpp and pcapi.cpp by link order).

struct Unk0053ac00 {
  unsigned char pad0[0x18];
  unsigned char primary[0x40 - 0x18];
  unsigned char flag;
  unsigned char pad1[0x320 - 0x41];
  unsigned char alternate[1];

  void *Current();
};

// "default, then override" keeps the original's branch order; an early
// return swaps the two leas and does not match.
// FUNCTION: LEGOBATMAN 0x0053ac00
void *Unk0053ac00::Current() {
  void *result = primary;
  if (flag)
    result = alternate;
  return result;
}
