// nu2api/nu3d: NuEdgeAAFilterGen (RTTI vtable 0x897d7c; saga
// nupostfilter_generic.cpp), after the nuprocesscolourfilter_gen.cpp anchor
// at 0x701e80. File boundary unproven.

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

struct NuEdgeAAFilterGen : NuPostFilterGen {
  NuEdgeAAFilterGen();
  virtual void initResources();
  virtual void destroyResources();
  virtual void initTextureResources(i32 width, i32 height);
  virtual void destroyTextureResources();
  virtual void render();
};

// FUNCTION: LEGOBATMAN 0x00702160
NuEdgeAAFilterGen::NuEdgeAAFilterGen() {}

// SYNTHETIC: LEGOBATMAN 0x00702170 NuEdgeAAFilterGen::`scalar deleting destructor'

// FUNCTION: LEGOBATMAN 0x00702190
void NuEdgeAAFilterGen::initResources() {}

// FUNCTION: LEGOBATMAN 0x007021a0
void NuEdgeAAFilterGen::destroyResources() {}

// FUNCTION: LEGOBATMAN 0x007021b0
void NuEdgeAAFilterGen::initTextureResources(i32 width, i32 height) {}

// FUNCTION: LEGOBATMAN 0x007021c0
void NuEdgeAAFilterGen::destroyTextureResources() {}

// FUNCTION: LEGOBATMAN 0x007021d0
void NuEdgeAAFilterGen::render() {}
