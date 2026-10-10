// nu2api/nu3d/nuprocesscolourfilter_gen.cpp: leaked __FILE__ at 0x701e80.
// NuProcessColourFilterGen (RTTI vtable 0x897cfc; saga
// nupostfilter_generic.cpp).

#include "../nucore/common.h"

struct NuPostFilterGen {
  NuPostFilterGen() : enabled(false) {}
  virtual ~NuPostFilterGen() {}
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

struct NuProcessColourFilterGen : NuPostFilterGen {
  NuProcessColourFilterGen();
  virtual void initResources();
  virtual void destroyResources();
  virtual void initTextureResources(i32 width, i32 height);
  virtual void destroyTextureResources();
  virtual void render();

  u8 pad0c[0x10 - 0xc];
  i32 f10;  // 0x10
  bool b14; // 0x14
};

// FUNCTION: LEGOBATMAN 0x00701e40
NuProcessColourFilterGen::NuProcessColourFilterGen() {
  b14 = false;
  f10 = 0;
}

// FUNCTION: LEGOBATMAN 0x00701e60
void NuProcessColourFilterGen::initResources() {}

// FUNCTION: LEGOBATMAN 0x00701e70
void NuProcessColourFilterGen::destroyResources() {}

// FUNCTION: LEGOBATMAN 0x00701fe0
void NuProcessColourFilterGen::initTextureResources(i32 width, i32 height) {}

// FUNCTION: LEGOBATMAN 0x00701ff0
void NuProcessColourFilterGen::destroyTextureResources() {}
