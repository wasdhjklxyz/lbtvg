// gameapi/, file unknown (between terrain.c and rtleditor.cpp by link order).

// GLOBAL: LEGOBATMAN 0x0095e230
int g_unk_0095e230;

// Called with 0/1 around level loading; returns the previous value.
// FUNCTION: LEGOBATMAN 0x0058b6d0
int SetUnk0095e230(int value) {
  int old = g_unk_0095e230;
  g_unk_0095e230 = value;
  return old;
}
