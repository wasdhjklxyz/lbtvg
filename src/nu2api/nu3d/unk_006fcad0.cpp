// nu2api/nu3d, file unknown (between nugscn_dlist.cpp and numtl_dlist.cpp):
// the PC post filters' initResources, which create their shader programs.
// Classes and slot order from RTTI (NuPostFilterGen vtable 0x89766c) and
// saga's NuPostFilterGen.h.

#include "../nucore/common.h"

#include <stdarg.h>

i32 Unk006fc180(i32 count, va_list shaders); // creates a program

// GLOBAL: LEGOBATMAN 0x009d10c4
extern i32 g_unk009d10c4; // pixel shader model

// Shader bytecode blobs.
// GLOBAL: LEGOBATMAN 0x008731b8
extern u8 g_unk008731b8[]; // shared fullscreen vertex shader
// GLOBAL: LEGOBATMAN 0x0088eee0
extern u8 g_unk0088eee0[];
// GLOBAL: LEGOBATMAN 0x0088f030
extern u8 g_unk0088f030[];
// GLOBAL: LEGOBATMAN 0x0088f2f0
extern u8 g_unk0088f2f0[];
// GLOBAL: LEGOBATMAN 0x0088f500
extern u8 g_unk0088f500[];
// GLOBAL: LEGOBATMAN 0x0088f7b8
extern u8 g_unk0088f7b8[];
// GLOBAL: LEGOBATMAN 0x0088f948
extern u8 g_unk0088f948[];
// GLOBAL: LEGOBATMAN 0x0088f9e0
extern u8 g_unk0088f9e0[];
// GLOBAL: LEGOBATMAN 0x0088fc60
extern u8 g_unk0088fc60[];
// GLOBAL: LEGOBATMAN 0x0088fd08
extern u8 g_unk0088fd08[];
// GLOBAL: LEGOBATMAN 0x008930b0
extern u8 g_unk008930b0[];

struct NuPostFilterGen {
  virtual ~NuPostFilterGen();
  virtual void initResources();
  virtual void destroyResources();
  virtual void initTextureResources(i32 width, i32 height);
  virtual void destroyTextureResources();
  virtual void render();
  virtual void reset();
  virtual void resetAll();
  virtual bool isEnabled();

  bool enabled; // 0x04
  i32 program;  // 0x08
};

struct NuMotionAccumFilterGen : NuPostFilterGen {
  virtual void initResources();
};

struct NuMotionAccumFilter : NuMotionAccumFilterGen {
  virtual void initResources();
};

struct NuSpeedBlurFilterGen : NuPostFilterGen {
  virtual void initResources();

  u8 pad0c[0x1c - 0xc];
  i32 i1c; // 0x1c
};

struct NuSpeedBlurFilter : NuSpeedBlurFilterGen {
  virtual void initResources();
};

struct NuProcessColourFilterGen : NuPostFilterGen {
  virtual void initResources();

  u8 pad0c[0x18 - 0xc];
  i32 i18; // 0x18
};

struct NuProcessColourFilter : NuProcessColourFilterGen {
  virtual void initResources();
};

struct NuSSAOFilterGen : NuPostFilterGen {
  virtual void initResources();

  i32 program2; // 0x0c
};

struct NuSSAOFilter : NuSSAOFilterGen {
  virtual void initResources();
};

// GLOBAL: LEGOBATMAN 0x0094c0b7
extern u8 g_unk0094c0b7; // SSAO available
// GLOBAL: LEGOBATMAN 0x0094c0ba
extern u8 g_unk0094c0ba;
// GLOBAL: LEGOBATMAN 0x0094c0bb
extern u8 g_unk0094c0bb;
// GLOBAL: LEGOBATMAN 0x009d0ff8
extern i32 g_unk009d0ff8;
// GLOBAL: LEGOBATMAN 0x009d1021
extern u8 g_unk009d1021;
// GLOBAL: LEGOBATMAN 0x0088fe58
extern u8 g_unk0088fe58[];
// GLOBAL: LEGOBATMAN 0x0088ff68
extern u8 g_unk0088ff68[];
// GLOBAL: LEGOBATMAN 0x00891028
extern u8 g_unk00891028[];
// GLOBAL: LEGOBATMAN 0x00891f08
extern u8 g_unk00891f08[];
// GLOBAL: LEGOBATMAN 0x00892fc8
extern u8 g_unk00892fc8[];

void Unk006e40d0(); // empty in the release build

struct NuEdgeAAFilterGen : NuPostFilterGen {
  virtual void initResources();
};

struct NuEdgeAAFilter : NuEdgeAAFilterGen {
  virtual void initResources();
};

// Both take a shader count and that many shader bytecode pointers.
// FUNCTION: LEGOBATMAN 0x006fc310
i32 Unk006fc310(i32 count, ...) {
  va_list args;
  va_start(args, count);
  return Unk006fc180(count, args);
}

// FUNCTION: LEGOBATMAN 0x006fc330
i32 Unk006fc330(i32 count, ...) {
  va_list args;
  va_start(args, count);
  return Unk006fc180(count, args);
}

// FUNCTION: LEGOBATMAN 0x006fcad0
void NuMotionAccumFilter::initResources() {
  NuMotionAccumFilterGen::initResources();
  program = Unk006fc310(2, g_unk008731b8, g_unk0088eee0);
}

// FUNCTION: LEGOBATMAN 0x006fcb00
void NuSpeedBlurFilter::initResources() {
  NuSpeedBlurFilterGen::initResources();
  if (g_unk009d10c4 > 1) {
    program = Unk006fc310(2, g_unk0088f030, g_unk0088f2f0);
    i1c = 0x80;
  } else {
    program = Unk006fc310(2, g_unk0088f500, g_unk0088f7b8);
    i1c = 0;
  }
}

// FUNCTION: LEGOBATMAN 0x006fcb60
void NuProcessColourFilter::initResources() {
  NuProcessColourFilterGen::initResources();
  if (g_unk009d10c4 > 1) {
    program = Unk006fc310(2, g_unk0088f948, g_unk0088f9e0);
    i18 = 0x1e;
  } else {
    program = Unk006fc310(2, g_unk0088fc60, g_unk0088fd08);
    i18 = 0;
  }
}

// FUNCTION: LEGOBATMAN 0x006fcbc0
void NuSSAOFilter::initResources() {
  NuSSAOFilterGen::initResources();
  if (g_unk0094c0bb) {
    if (g_unk009d0ff8) {
      if (!g_unk009d1021)
        program = Unk006fc310(2, g_unk0088fe58, g_unk0088ff68);
      else
        program = Unk006fc310(2, g_unk0088fe58, g_unk00891028);
    } else if (g_unk0094c0ba && g_unk009d1021) {
      Unk006e40d0();
    } else {
      program = Unk006fc310(2, g_unk0088fe58, g_unk00891f08);
    }
    program2 = Unk006fc310(2, g_unk008731b8, g_unk00892fc8);
  }
  if (program == 0)
    g_unk0094c0b7 = 0;
}

// FUNCTION: LEGOBATMAN 0x006fcc50
void NuEdgeAAFilter::initResources() {
  NuEdgeAAFilterGen::initResources();
  program = Unk006fc310(2, g_unk008731b8, g_unk008930b0);
}
