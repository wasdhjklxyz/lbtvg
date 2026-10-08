// batman/, file unknown (between pcbatman.cpp and pcapi.cpp by link order).

// +0 and +8 are zeroed via fldz/fst (float), +4 and +0xc via eax (int).
struct Unk0051e910 {
  float f0;
  int i4;
  float f8;
  int ic;

  void Reset();
};

// FUNCTION: LEGOBATMAN 0x0051e910
void Unk0051e910::Reset() {
  f0 = 0.0f;
  i4 = 0;
  f8 = 0.0f;
  ic = 0;
}
