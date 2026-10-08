// batman/pc/nusound.cpp (anchors 0x00535110 and 0x00535970).

// Sound object whose methods live past this file; only the ones called from
// here are declared.
struct UnkNuSoundObj {
  int Unk005398d0();
  int Unk005398f0();
  int Unk00539910();
  int Unk005399c0();
};

// GLOBAL: LEGOBATMAN 0x009e6de0
int g_nuSoundUnk009e6de0;
// GLOBAL: LEGOBATMAN 0x009e6de8
UnkNuSoundObj *g_nuSoundObjs[2];
// GLOBAL: LEGOBATMAN 0x009e6e18
int g_nuSoundUnk009e6e18;
// GLOBAL: LEGOBATMAN 0x0094eabc
extern int g_unk0094eabc;

// FUNCTION: LEGOBATMAN 0x00535660
int NuSoundUnk00535660(int which) {
  if (which)
    which = 1;
  return g_nuSoundObjs[which]->Unk005398d0();
}

// FUNCTION: LEGOBATMAN 0x00535680
int NuSoundUnk00535680(int which) {
  if (which)
    which = 1;
  return g_nuSoundObjs[which]->Unk00539910();
}

// FUNCTION: LEGOBATMAN 0x005356a0
int NuSoundUnk005356a0(int which) {
  if (which)
    which = 1;
  return g_nuSoundObjs[which]->Unk005398f0();
}

// FUNCTION: LEGOBATMAN 0x00535720
int NuSoundGetStateUnk(int which) {
  if (which)
    which = 1;
  return g_nuSoundObjs[which]->Unk005399c0();
}

// Both calls are NuSoundGetStateUnk inlined.
// FUNCTION: LEGOBATMAN 0x00535740
int NuSoundUnk00535740(int flag) {
  if (flag == 0)
    return NuSoundGetStateUnk(g_nuSoundUnk009e6de0);
  return NuSoundGetStateUnk(g_unk0094eabc);
}

// FUNCTION: LEGOBATMAN 0x00535930
void NuSoundSetUnk009e6e18(int value) { g_nuSoundUnk009e6e18 = value; }
