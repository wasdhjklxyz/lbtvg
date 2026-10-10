// nu2api/nu3d: NuDeferredFilterGen (RTTI vtable 0x897e34; saga
// NuDeferredFilterGen.h, nupostfilter_generic.cpp). PC layout; file name by
// analogy with the nuprocesscolourfilter_gen.cpp anchor; unproven.

#include "../nucore/common.h"

struct nueffecttex_s;

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

struct NuDeferredFilterGen : NuPostFilterGen {
  virtual void initResources();
  virtual void destroyTextureResources();
  virtual void resetAll();
  virtual bool isEnabled();

  u8 pad0c[0x10 - 0xc];
  i32 i10; // 0x10
  i32 i14; // 0x14
  u8 pad18[0x20 - 0x18];
  nueffecttex_s *tex20[2]; // 0x20
  nueffecttex_s *tex28[2]; // 0x28
  u8 pad30[0x38 - 0x30];
  i32 i38;                     // 0x38
  i32 i3c;                     // 0x3c
  i32 i40;                     // 0x40
  i32 i44;                     // 0x44
  i32 dynamic_light_count;     // 0x48
  i32 i4c;                     // 0x4c
  i32 deferred_geometry_count; // 0x50
};

// FUNCTION: LEGOBATMAN 0x00708100
bool NuDeferredFilterGen::isEnabled() {
  return enabled && (dynamic_light_count != 0 || deferred_geometry_count > 0);
}

// FUNCTION: LEGOBATMAN 0x00708140
void NuDeferredFilterGen::initResources() { resetAll(); }

// FUNCTION: LEGOBATMAN 0x00708280
void NuDeferredFilterGen::destroyTextureResources() {
  for (i32 i = 0; i < 2; i++) {
    if (tex28[i] != 0) {
      Unk006e47f0(tex28[i]);
      tex28[i] = 0;
    }
    if (tex20[i] != 0) {
      Unk006e47f0(tex20[i]);
      tex20[i] = 0;
    }
  }
  i14 = 0;
  i10 = 0;
}

// FUNCTION: LEGOBATMAN 0x007082d0
void NuDeferredFilterGen::resetAll() {
  i38 = 0;
  i3c = 0;
  i40 = 0;
  i44 = 0;
  dynamic_light_count = 0;
  i4c = 0;
  deferred_geometry_count = 0;
}
