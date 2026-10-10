// nu2api/nu3d: NuSpeedBlurFilterGen (RTTI vtable 0x897cd4; saga
// nupostfilter_generic.cpp). File name by analogy with the
// nuprocesscolourfilter_gen.cpp anchor at 0x701e80; unproven.

#include "../nucore/common.h"

struct nueffecttex_s;

nueffecttex_s *NuEffectTexCreate2D(i32 width, i32 height, i32 levels,
                                   i32 format, i32 usage);
void Unk006e49c0(nueffecttex_s *tex, i32 a, i32 b, i32 c); // sampler state
void Unk006e4b90(nueffecttex_s *tex, i32 a, i32 b);        // address mode
void Unk006e47f0(nueffecttex_s *tex); // releases the effect texture

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

struct NuSpeedBlurFilterGen : NuPostFilterGen {
  NuSpeedBlurFilterGen();
  virtual void initResources();
  virtual void destroyResources();
  virtual void initTextureResources(i32 width, i32 height);
  virtual void destroyTextureResources();

  u8 pad0c[0x14 - 0xc];
  nueffecttex_s *texture; // 0x14
};

// FUNCTION: LEGOBATMAN 0x00701bf0
NuSpeedBlurFilterGen::NuSpeedBlurFilterGen() {}

// SYNTHETIC: LEGOBATMAN 0x00701c00 NuSpeedBlurFilterGen::`scalar deleting destructor'

// FUNCTION: LEGOBATMAN 0x00701c20
void NuSpeedBlurFilterGen::initResources() {}

// FUNCTION: LEGOBATMAN 0x00701c30
void NuSpeedBlurFilterGen::destroyResources() {}

// FUNCTION: LEGOBATMAN 0x00701c40
void NuSpeedBlurFilterGen::initTextureResources(i32 width, i32 height) {
  texture = NuEffectTexCreate2D(width / 2, height / 2, 1, 0x21, 2);
  Unk006e49c0(texture, 2, 2, 0);
  Unk006e4b90(texture, 2, 2);
}

// FUNCTION: LEGOBATMAN 0x00701c90
void NuSpeedBlurFilterGen::destroyTextureResources() { Unk006e47f0(texture); }
