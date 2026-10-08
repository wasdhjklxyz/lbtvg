// gameapi/cutscene/gcutscn.cpp: certain range 0x006c9ca0..0x006ceef0.

struct GCutscene;

typedef void (*GCutsceneCallback)(void *);
typedef void (*GCutsceneItemCallback)(int index, GCutscene *cs, float t,
                                      void *user);

// Case-insensitive string compare (lives in the nupad_gen.cpp range).
extern int StrCaseCmp_006dc3a0(const char *a, const char *b);

// GLOBAL: LEGOBATMAN 0x00ad7590
GCutsceneItemCallback g_cutsceneCallback_ad7590;
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

// Sequence header: items pointer, then a 16-bit count.
struct GCutsceneSeq {
  void *items;
  unsigned short count;
};

struct GCutsceneData {
  unsigned char pad[0x18];
  GCutsceneSeq *seq18;
};

struct GCutscene {
  unsigned char pad[0x18];
  int field_18;
  unsigned char pad2[0x58 - 0x1c];
  GCutsceneData *data;
};

// Named table entries, stride 0x34: name pointer first, a byte at +0xa.
struct Unk006ce710Item {
  const char *name;
  unsigned char pad[6];
  unsigned char value_a;
  unsigned char pad2[0x34 - 0xb];
};

struct Unk006ce710Table {
  unsigned char pad[8];
  int count;
  Unk006ce710Item *items;
};

// Per-item 4-byte state, flags in bytes 2 and 3.
struct Unk006ce710Entry {
  unsigned char pad[2];
  unsigned char flags2;
  unsigned char flags3;
};

struct Unk006ce710 {
  unsigned char pad[8];
  Unk006ce710Table *table;
  Unk006ce710Entry *entries;
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
void GCutsceneSetCallback_ad7590(GCutsceneItemCallback fn) {
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

// Original hoists the ebx/ebp arg loads above the g_cb test; every shape
// tried sinks them below it. Static twin with cs in ebx: 0x006cbcd0 (its
// callers past 0x006d1000 prove gcutscn.cpp extends at least that far).
// STUB: LEGOBATMAN 0x006cbd20
void GCutsceneFireItemCallback(GCutscene *cs, float t, void *user) {
  if (g_cutsceneCallback_ad7590) {
    GCutsceneSeq *seq = cs->data->seq18;
    for (int i = 0; i < seq->count; i++)
      g_cutsceneCallback_ad7590(i, cs, t, user);
  }
}

// Sets or clears bit (bit - 1) of flags2 on the entry whose item is named key.
// FUNCTION: LEGOBATMAN 0x006ce710
void SetUnk006ce710Flag(Unk006ce710 *o, const char *key, int bit, int set) {
  Unk006ce710Table *t = o->table;
  for (int i = 0; i < t->count; i++) {
    if (StrCaseCmp_006dc3a0(t->items[i].name, key) == 0) {
      unsigned char m = (unsigned char)(1 << (bit - 1));
      if (set)
        o->entries[i].flags2 |= m;
      else
        o->entries[i].flags2 &= ~m;
      return;
    }
  }
}

// Returns a pointer to flags3 of the matched entry; falls off the end if none.
// FUNCTION: LEGOBATMAN 0x006ce890
unsigned char *ResetUnk006ce710Entry(Unk006ce710 *o, const char *key) {
  if (o) {
    Unk006ce710Table *t = o->table;
    for (int i = 0; i < t->count; i++) {
      if (StrCaseCmp_006dc3a0(t->items[i].name, key) == 0) {
        o->entries[i].flags2 = t->items[i].value_a;
        unsigned char *p = &o->entries[i].flags3;
        *p &= ~1;
        return p;
      }
    }
  }
}
