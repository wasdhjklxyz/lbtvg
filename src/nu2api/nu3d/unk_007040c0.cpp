// nu2api/nu3d: NuSSAOFilterGen (RTTI vtable 0x897dc4; saga
// nupostfilter_generic.cpp). File boundary unproven.

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

struct NuSSAOFilterGen : NuPostFilterGen {
  NuSSAOFilterGen();
  virtual void initResources();
  virtual void destroyResources();
  virtual void initTextureResources(i32 width, i32 height);
  virtual void destroyTextureResources();
  virtual void render();
};

// FUNCTION: LEGOBATMAN 0x007040c0
NuSSAOFilterGen::NuSSAOFilterGen() {}

// FUNCTION: LEGOBATMAN 0x007040f0
void NuSSAOFilterGen::initResources() {}

// FUNCTION: LEGOBATMAN 0x00704100
void NuSSAOFilterGen::destroyResources() {}

// FUNCTION: LEGOBATMAN 0x00704110
void NuSSAOFilterGen::initTextureResources(i32 width, i32 height) {}

// FUNCTION: LEGOBATMAN 0x00704120
void NuSSAOFilterGen::destroyTextureResources() {}

// FUNCTION: LEGOBATMAN 0x00704130
void NuSSAOFilterGen::render() {}
