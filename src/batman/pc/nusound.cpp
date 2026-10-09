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

// 0x40c-byte per-channel records; only the byte at +0x404 is known.
struct UnkNuSoundChannel {
  unsigned char pad[0x404];
  unsigned char active;
  unsigned char pad2[0x40c - 0x405];
};

// Methods live past this file; thiscall on the global object below.
struct UnkNuSoundMgr {
  int Unk005346b0(int chan, int zero);
  void Unk005347c0(int handle, int zero, float value, int zero2);
};
struct UnkNuSoundObjVol {
  void Unk00539930(int value);
};

// GLOBAL: LEGOBATMAN 0x009e7b3c
UnkNuSoundChannel *g_nuSoundChannels;
// GLOBAL: LEGOBATMAN 0x0094eae4
extern int g_nuSoundVolumeScale;
// GLOBAL: LEGOBATMAN 0x009d6d98
extern float g_nuSoundVolumeTable[0x4000];
// GLOBAL: LEGOBATMAN 0x009e6e30
UnkNuSoundMgr g_nuSoundMgr;
// GLOBAL: LEGOBATMAN 0x0094eacc
extern int g_unk0094eacc;

// Inlined wherever the table is indexed.
static float NuSoundVolume(int vol) {
  if (vol > 0x3fff)
    vol = 0;
  return g_nuSoundVolumeTable[vol];
}

// Only the first two parameters are used; the rest match the callers.
// FUNCTION: LEGOBATMAN 0x005352a0
void NuSoundUnk005352a0(int chan, int vol, int p3, int p4, float p5, int p6,
                        float p7, float p8, int p9) {
  if (g_nuSoundChannels && g_nuSoundChannels[chan].active) {
    if (g_nuSoundVolumeScale != 0x4000)
      vol = g_nuSoundVolumeScale * vol >> 15;
    int handle = g_nuSoundMgr.Unk005346b0(chan, 0);
    if (handle > -1)
      g_nuSoundMgr.Unk005347c0(handle, 0, NuSoundVolume(vol), 0);
  }
}

void NuSoundUnk00535360(int, int, int, int, int, float, int, float, float, int);

// FUNCTION: LEGOBATMAN 0x00535320
void NuSoundUnk00535320(int p1, int p2, int p3, int p4, float p5, int p6,
                        float p7, float p8) {
  NuSoundUnk005352a0(p1, p2, p3, p4, p5, p6, p7, p8, 0);
}

// FUNCTION: LEGOBATMAN 0x00535470
void NuSoundUnk00535470(int p1, int p2, int p3, int p4, int p5, float p6,
                        int p7, float p8, float p9) {
  NuSoundUnk00535360(p1, p2, p3, p4, p5, p6, p7, p8, p9, 0);
}

// FUNCTION: LEGOBATMAN 0x005356c0
void NuSoundUnk005356c0(int which, int vol) {
  if (which)
    which = 1;
  int value = (int)NuSoundVolume(vol) + g_unk0094eacc;
  if (value < -10000)
    value = -10000;
  ((UnkNuSoundObjVol *)g_nuSoundObjs[which])->Unk00539930(value);
}

void *Unk006e1d50(int size, const char *file, int line);
void Unk00533c40();
extern "C" void *memset(void *, int, unsigned int);

// GLOBAL: LEGOBATMAN 0x009e6dd0
int g_nuSoundUnk009e6dd0;
// GLOBAL: LEGOBATMAN 0x009e6dd8
int g_nuSoundUnk009e6dd8;
// GLOBAL: LEGOBATMAN 0x009e6ddc
void *g_nuSoundUnk009e6ddc;

// FUNCTION: LEGOBATMAN 0x00535970
void NuSoundUnk00535970(int value, int count) {
  if (g_nuSoundUnk009e6dd8 != count) {
    if (g_nuSoundUnk009e6ddc)
      Unk00533c40();
    g_nuSoundUnk009e6ddc = Unk006e1d50(count * 2, ".\\pc\\nusound.cpp", 0x8c4);
    memset(g_nuSoundUnk009e6ddc, 0, count * 2);
  }
  g_nuSoundUnk009e6dd8 = count;
  g_nuSoundUnk009e6dd0 = value;
}

struct UnkNuSoundObjPlay {
  int Unk005398d0();
  void Unk00539860(void *channelData, float value);
  void Unk005398b0(unsigned int flags);
};

struct UnkNuSoundRequest {
  unsigned char pad[8];
  int which;
  int chan;
  int unk10;
  unsigned char pad2[0x1c - 0x14];
  int unk1c;
  float unk20;
};

struct Unk009e6db0Record {
  unsigned char pad[8];
  int unk8;
  unsigned char pad2[0x1c - 0xc];
};

// GLOBAL: LEGOBATMAN 0x0094eab8
extern int g_unk0094eab8;
// GLOBAL: LEGOBATMAN 0x0094eac0
extern int g_unk0094eac0;
// GLOBAL: LEGOBATMAN 0x009e6db0
Unk009e6db0Record *g_nuSoundUnk009e6db0;

// FUNCTION: LEGOBATMAN 0x00535780
void NuSoundUnk00535780(UnkNuSoundRequest *req) {
  int a, b;
  if (req->which) {
    a = g_unk0094eabc;
    b = g_unk0094eac0;
  } else {
    a = g_nuSoundUnk009e6de0;
    b = g_unk0094eab8;
  }
  if (req->unk10 == -1)
    req->unk10 = req->chan + 1;
  if (a != -1 && b != -1) {
    unsigned int flags = 0;
    if (req->unk1c == 0)
      flags = 2;
    if (g_nuSoundUnk009e6db0 && g_nuSoundUnk009e6db0[req->chan].unk8 == 0)
      flags |= 1;
    ((UnkNuSoundObjPlay *)g_nuSoundObjs[req->which])->Unk005398d0();
    ((UnkNuSoundObjPlay *)g_nuSoundObjs[req->which])
        ->Unk00539860(&g_nuSoundChannels[req->chan].pad[4], req->unk20);
    ((UnkNuSoundObjPlay *)g_nuSoundObjs[req->which])->Unk005398b0(flags);
  }
}

#include <windows.h>

void Unk006e2090(void *ptr, const char *file, int line);
struct UnkNuSoundObjDtor {
  ~UnkNuSoundObjDtor();
};

// GLOBAL: LEGOBATMAN 0x009d6d94
extern int g_nuSoundInitialised;
// GLOBAL: LEGOBATMAN 0x009e6e0c
HANDLE g_nuSoundThreadEvent;
// GLOBAL: LEGOBATMAN 0x009e6e10
int g_nuSoundThreadRunning;
// GLOBAL: LEGOBATMAN 0x009e6e14
unsigned char g_nuSoundThreadQuit;

// FUNCTION: LEGOBATMAN 0x00535110
void NuSoundShutdown() {
  if (g_nuSoundInitialised) {
    if (g_nuSoundObjs[1])
      delete (UnkNuSoundObjDtor *)g_nuSoundObjs[1];
    if (g_nuSoundObjs[0])
      delete (UnkNuSoundObjDtor *)g_nuSoundObjs[0];
    g_nuSoundObjs[1] = 0;
    g_nuSoundObjs[0] = 0;
    if (g_nuSoundThreadEvent && g_nuSoundThreadRunning) {
      g_nuSoundThreadQuit = 1;
      SetEvent(g_nuSoundThreadEvent);
      while (g_nuSoundThreadRunning)
        Sleep(10);
      DeleteObject(g_nuSoundThreadEvent);
      g_nuSoundThreadEvent = 0;
      g_nuSoundThreadRunning = 0;
    }
    if (g_nuSoundUnk009e6ddc)
      Unk006e2090(g_nuSoundUnk009e6ddc, ".\\pc\\nusound.cpp", 0x72f);
    g_nuSoundUnk009e6ddc = 0;
    g_nuSoundInitialised = 0;
  }
}

struct UnkNuSoundMgr2 {
  int Unk005346b0(int chan, int zero);
  void Unk005347c0(int handle, int zero, float value, int p);
  void Unk00534ac0(int handle, void *local, int flag);
};
int Unk005349f0(const float *pos, int vol);

// GLOBAL: LEGOBATMAN 0x009e7bd8
extern float g_nuSoundListenerPos[3];
// GLOBAL: LEGOBATMAN 0x009e7be4
extern float g_nuSoundUnk009e7be4[9];
// GLOBAL: LEGOBATMAN 0x009e6df4
extern int g_nuSoundUnk009e6df4;

// STUB: LEGOBATMAN 0x00535360
void NuSoundUnk00535360(const float *pos, int chan, int vol, int p4, int p5,
                        float p6, int p7, float p8, float p9, int p10) {
  struct {
    float x, y, z;
  } local;
  if (g_nuSoundChannels && g_nuSoundChannels[chan].active &&
      (pos < g_nuSoundUnk009e7be4 || pos > g_nuSoundUnk009e7be4 + 9 ||
       pos[0] != g_nuSoundListenerPos[0] || pos[1] != g_nuSoundListenerPos[1] ||
       pos[2] != g_nuSoundListenerPos[2])) {
    if (g_nuSoundVolumeScale != 0x4000)
      vol = g_nuSoundVolumeScale * vol >> 15;
    int v = Unk005349f0(pos, vol);
    if (v) {
      int handle = ((UnkNuSoundMgr2 *)&g_nuSoundMgr)->Unk005346b0(chan, 0);
      if (handle > -1) {
        ((UnkNuSoundMgr2 *)&g_nuSoundMgr)->Unk00534ac0(handle, &local, 1);
        ((UnkNuSoundMgr2 *)&g_nuSoundMgr)
            ->Unk005347c0(handle, 0, NuSoundVolume(v), p5);
        g_nuSoundUnk009e6df4++;
      }
    }
  }
}

// FUNCTION: LEGOBATMAN 0x00535b60
extern "C" void NuSoundAddRumble() {}

#include <stdio.h>

void Unk00533c60();

// FUNCTION: LEGOBATMAN 0x00535c10
extern "C" void NuSoundStopStereo(int id) {
  char text[0x40];
  sprintf(text, ">>> NuSoundStopStereo %d", id);
  Unk00533c60();
  (id == g_nuSoundUnk009e6de0 ? g_nuSoundObjs[0] : g_nuSoundObjs[1])
      ->Unk005398d0();
}
