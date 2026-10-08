// gameapi/cutscene/gcutscn.cpp: certain range 0x006c9ca0..0x006ceef0.

typedef void (*GCutsceneCallback)(void *);

// GLOBAL: LEGOBATMAN 0x00ad7590
GCutsceneCallback g_cutsceneCallback_ad7590;
// GLOBAL: LEGOBATMAN 0x00ad7598
GCutsceneCallback g_cutsceneCallback_ad7598;
// GLOBAL: LEGOBATMAN 0x00ad759c
GCutsceneCallback g_cutsceneCallback_ad759c;
// GLOBAL: LEGOBATMAN 0x00ad75a0
GCutsceneCallback g_cutsceneCallback_ad75a0;
// GLOBAL: LEGOBATMAN 0x00ad75a4
GCutsceneCallback g_cutsceneCallback_ad75a4;
// GLOBAL: LEGOBATMAN 0x00ad75a8
GCutsceneCallback g_cutsceneCallback_ad75a8;
// GLOBAL: LEGOBATMAN 0x00ad75b4
GCutsceneCallback g_cutsceneCallback_ad75b4;

// GLOBAL: LEGOBATMAN 0x029f400c
unsigned char g_unk_029f400c;
// GLOBAL: LEGOBATMAN 0x029f400d
unsigned char g_unk_029f400d;

// Only +0x18 is known: the callback at 0x00ad75a0 fires when it is set.
struct GCutscene {
  unsigned char pad[0x18];
  int field_18;
};

// Element of the +0x64 array in Unk006ca3c0, stride 0x20.
struct Unk006ca3c0Entry {
  unsigned char pad[0x10];
  int id;
  unsigned char pad2[0x20 - 0x14];
};

struct Unk006ca3c0 {
  unsigned char pad[0x60];
  int entryCount;
  Unk006ca3c0Entry *entries;
};

struct Unk006ca380Item {
  unsigned char pad[0x9a];
  unsigned char type;
};

struct Unk006ca380 {
  unsigned char pad[0xc];
  Unk006ca380Item **items;
  int itemCount;
};

// FUNCTION: LEGOBATMAN 0x006cc620
void GCutsceneSetCallback_ad7590(GCutsceneCallback fn) {
  g_cutsceneCallback_ad7590 = fn;
}

// FUNCTION: LEGOBATMAN 0x006cc630
void GCutsceneSetCallback_ad7598(GCutsceneCallback fn) {
  g_cutsceneCallback_ad7598 = fn;
}

// FUNCTION: LEGOBATMAN 0x006cc650
void GCutsceneSetCallback_ad759c(GCutsceneCallback fn) {
  g_cutsceneCallback_ad759c = fn;
}

// FUNCTION: LEGOBATMAN 0x006cc670
void GCutsceneSetCallback_ad75a4(GCutsceneCallback fn) {
  g_cutsceneCallback_ad75a4 = fn;
}

// FUNCTION: LEGOBATMAN 0x006cc680
void GCutsceneSetCallback_ad75a8(GCutsceneCallback fn) {
  g_cutsceneCallback_ad75a8 = fn;
}

// FUNCTION: LEGOBATMAN 0x006cc6e0
void GCutsceneSetCallback_ad75b4(GCutsceneCallback fn) {
  g_cutsceneCallback_ad75b4 = fn;
}

// FUNCTION: LEGOBATMAN 0x006cd190
void SetUnk029f400c(unsigned char a, unsigned char b) {
  g_unk_029f400c = a;
  g_unk_029f400d = b;
}

// Tail-calls the callback (jmp) with the argument written back to the slot.
// FUNCTION: LEGOBATMAN 0x006cecf0
void GCutsceneNotify_ad75a0(GCutscene *cs) {
  if (cs->field_18 != 0 && g_cutsceneCallback_ad75a0 != 0)
    g_cutsceneCallback_ad75a0(cs);
}

// Returns 1-based index of the entry with the given id, 0 if none.
// FUNCTION: LEGOBATMAN 0x006ca3c0
int FindUnk006ca3c0Entry(Unk006ca3c0 *o, int id) {
  int i = 0;
  if (o->entryCount > 0) {
    Unk006ca3c0Entry *e = o->entries;
    do {
      i++;
      if (e->id == id)
        return i;
      e++;
    } while (i < o->entryCount);
  }
  return 0;
}

// Returns 1-based index of the item with the given type byte, 0 if none.
// FUNCTION: LEGOBATMAN 0x006ca380
int FindUnk006ca380Item(Unk006ca380 *o, int type) {
  int i = 0;
  if (o->itemCount > 0) {
    Unk006ca380Item **p = o->items;
    do {
      i++;
      if ((*p)->type == type)
        return i;
      p++;
    } while (i < o->itemCount);
  }
  return 0;
}
