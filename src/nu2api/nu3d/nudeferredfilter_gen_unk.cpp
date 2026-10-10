// nu2api/nu3d: NuDeferredFilterGen (RTTI vtable 0x897e34; saga
// NuDeferredFilterGen.h, nupostfilter_generic.cpp). PC layout; file name by
// analogy with the nuprocesscolourfilter_gen.cpp anchor; unproven.

#include "../nucore/common.h"

struct nueffecttex_s;

void Unk006e47f0(nueffecttex_s *tex); // releases the effect texture
nueffecttex_s *NuEffectTexCreate2D(i32 width, i32 height, i32 levels,
                                   i32 format, i32 usage);
void Unk006e4980(nueffecttex_s *tex, i32 a, i32 b); // NuEffectTexSetLODs
void Unk006e4b90(nueffecttex_s *tex, i32 a,
                 i32 b);                          // NuEffectTexSetAddressing2D
void Unk006e49b0(nueffecttex_s *tex, i32 colour); // NuEffectTexSetBorderColour
void Unk006e49c0(nueffecttex_s *tex, i32 a, i32 b,
                 i32 c);             // NuEffectTexSetFilters
i32 Unk006e4900(nueffecttex_s *tex); // NuEffectTexGetWidth
i32 Unk006e4910(nueffecttex_s *tex); // NuEffectTexGetHeight
i32 Unk006e4950(nueffecttex_s *tex); // NuEffectTexGetLevels

struct nushaderprogram_s;
void Unk00530ca0(i32 state, i32 value); // render state

struct NuPostFilter {
  static void copy(nueffecttex_s *src, i32 src_level, nueffecttex_s *dst,
                   i32 dst_level, nushaderprogram_s *program,
                   nueffecttex_s *extra);
  static void blur7x7(nueffecttex_s *src, i32 src_level, nueffecttex_s *dst,
                      i32 dst_level, i32 a, i32 b, bool c, f32 scale,
                      nushaderprogram_s *program);
};

// GLOBAL: LEGOBATMAN 0x029f3e2c
extern nushaderprogram_s *g_unk029f3e2c; // copy program
// GLOBAL: LEGOBATMAN 0x029f3e48
extern nushaderprogram_s *g_unk029f3e48; // blur program

// GLOBAL: LEGOBATMAN 0x029dcc08
extern nueffecttex_s *g_unk029dcc08; // default effect texture
// GLOBAL: LEGOBATMAN 0x00adf714
extern i32 g_unk00adf714;

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
  virtual void initTextureResources(i32 width, i32 height);
  virtual void destroyTextureResources();
  virtual void resetAll();
  virtual bool isEnabled();
  virtual void template_blurLuminanceMap(); // 9

  nueffecttex_s *tex0c;        // 0x0c
  nueffecttex_s *i10;          // 0x10
  nueffecttex_s *i14;          // 0x14
  nueffecttex_s *tex18;        // 0x18
  bool b1c;                    // 0x1c
  nueffecttex_s *tex20[2];     // 0x20
  nueffecttex_s *tex28[2];     // 0x28
  nueffecttex_s *tex30[2];     // 0x30
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

// STUB: LEGOBATMAN 0x00708160
// b1c = (width / 2 == width): original materialises it via xor ecx/sete cl
// with width in esi; 10 spellings tried
void NuDeferredFilterGen::initTextureResources(i32 width, i32 height) {
  for (i32 i = 0; i < 2; i++) {
    tex30[i] = tex20[i];
    Unk006e4980(tex20[i], 0, 1);
    Unk006e4b90(tex20[i], 3, 3);
    Unk006e49b0(tex20[i], -1);
    Unk006e49c0(tex20[i], 1, 1, 0);
  }
  b1c = width / 2 == width;
  if (!b1c)
    tex0c = g_unk029dcc08;
  else
    tex0c = NuEffectTexCreate2D(width, height, 1, 0x21, 2);
  if (g_unk00adf714 != 0) {
    i32 h = Unk006e4910(tex0c);
    i32 w = Unk006e4900(tex0c);
    i10 = NuEffectTexCreate2D(w, h, 1, 0x21, 2);
  } else
    i10 = 0;
  i14 = tex0c;
  if (!b1c)
    tex18 = g_unk029dcc08;
  else
    tex18 = NuEffectTexCreate2D(0x200, 0x200, 4, 0x21, 2);
  program = Unk006e4950(tex18) > 4 ? 4 : Unk006e4950(tex18);
  if (program == 0)
    program = 4;
}

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

// FUNCTION: LEGOBATMAN 0x007089c0
void NuDeferredFilterGen::template_blurLuminanceMap() {
  Unk00530ca0(0x16, 1);
  Unk00530ca0(0xf, 0);
  Unk00530ca0(0x1b, 0);
  Unk00530ca0(7, 0);
  Unk00530ca0(0x34, 0);
  NuPostFilter::copy(tex18, 0, i14, 0, g_unk029f3e2c, 0);
  NuPostFilter::blur7x7(tex18, 0, tex18, 1, 2, program - 1, true, 1.0f,
                        g_unk029f3e48);
}
