// nu2api/nu3d: NuMotionAccumFilterGen (RTTI vtable 0x897cac; saga
// nupostfilter_generic.cpp). File name by analogy with the
// nuprocesscolourfilter_gen.cpp anchor at 0x701e80; unproven.

#include "../nucore/common.h"

struct nueffecttex_s;

nueffecttex_s *NuEffectTexCreate2D(i32 width, i32 height, i32 levels,
                                   i32 format, i32 usage);
void Unk006e47f0(nueffecttex_s *tex); // releases the effect texture

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
  virtual void initTextureResources(i32 width, i32 height);
  virtual void destroyTextureResources();

  nueffecttex_s *accumulation_texture; // 0x0c
};

// FUNCTION: LEGOBATMAN 0x00701370
void NuMotionAccumFilterGen::initTextureResources(i32 width, i32 height) {
  accumulation_texture = NuEffectTexCreate2D(width, height, 1, 0x21, 2);
}

// FUNCTION: LEGOBATMAN 0x007013a0
void NuMotionAccumFilterGen::destroyTextureResources() {
  Unk006e47f0(accumulation_texture);
}
