// gameapi/, file unknown (between rtleditor.cpp and listman_gen.cpp by link
// order).

struct Unk0062bf50 {
  unsigned char pad[0xb68];
  int field_b68;
};

// FUNCTION: LEGOBATMAN 0x0062bf50
bool IsUnk0062bf50Clear(Unk0062bf50 *p) { return p->field_b68 == 0; }
