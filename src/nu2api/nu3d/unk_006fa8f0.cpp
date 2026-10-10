// nu2api/nu3d, file unknown (between nugscn_dlist.cpp and numtl_dlist.cpp,
// after NuPostFilter::copy/blur7x7): NuDeferredFilter (RTTI vtable 0x89835c),
// the PC NuDeferredFilterGen.

#include "../nucore/common.h"

struct nueffecttex_s;
struct nushaderprogram_s;
void Unk00530ca0(i32 state, i32 value); // render state

struct NuPostFilter {
  static void blurSM1(nueffecttex_s *src, i32 src_level, nueffecttex_s *dst,
                      i32 dst_level, i32 a, i32 b, bool c, f32 scale,
                      nushaderprogram_s *program);
  static void blur7x7(nueffecttex_s *src, i32 src_level, nueffecttex_s *dst,
                      i32 dst_level, i32 a, i32 b, bool c, f32 scale,
                      nushaderprogram_s *program);

  // GLOBAL: LEGOBATMAN 0x029dcbf8
  static nushaderprogram_s *blurSM1Program;
};

void Unk007002c0(nueffecttex_s *src, i32 src_level, nueffecttex_s *dst,
                 i32 dst_level, i32 a, i32 b, bool c, f32 scale,
                 nushaderprogram_s *program); // shader model 2 path

// GLOBAL: LEGOBATMAN 0x009d10c4
extern i32 g_unk009d10c4; // pixel shader model

// GLOBAL: LEGOBATMAN 0x029f3e48
extern nushaderprogram_s *g_unk029f3e48; // blur program

// GLOBAL: LEGOBATMAN 0x0094c0bb
extern u8 g_unk0094c0bb;

// RTTI vtable 0x8982bc.
struct NuMainFilterGen {
  virtual ~NuMainFilterGen();
  virtual void initResources();
  virtual void destroyResources();
  virtual void initTextureResources(i32 width, i32 height);
  virtual void destroyTextureResources();
  virtual void render();

  bool enabled; // 0x04
  u8 pad05[0x55 - 0x5];
  bool bloom_enabled; // 0x55
};

// RTTI vtable 0x8979e4.
struct NuMainFilter : NuMainFilterGen {
  virtual void render();

  void Unk006f7b30(); // the shader model 1 render path
};

// Slot order from the NuDeferredFilterGen vtable 0x897e34
// (src/nu2api/nu3d/nudeferredfilter_gen_unk.cpp).
struct NuDeferredFilter {
  virtual ~NuDeferredFilter();
  virtual void initResources();
  virtual void destroyResources();
  virtual void initTextureResources(i32 width, i32 height);
  virtual void destroyTextureResources();
  virtual void render();
  virtual void reset();
  virtual void resetAll();
  virtual bool isEnabled();
  virtual void template_blurLuminanceMap(); // 9

  bool enabled; // 0x04
  i32 program;  // 0x08, mip levels
  u8 pad0c[0x14 - 0xc];
  nueffecttex_s *tex14; // 0x14
  nueffecttex_s *tex18; // 0x18
};

// FUNCTION: LEGOBATMAN 0x006fa830
void NuPostFilter::blur7x7(nueffecttex_s *src, i32 src_level,
                           nueffecttex_s *dst, i32 dst_level, i32 a, i32 b,
                           bool c, f32 scale, nushaderprogram_s *program) {
  if (g_unk009d10c4 >= 2)
    Unk007002c0(src, src_level, dst, dst_level, a, b, c, scale, program);
  else
    blurSM1(src, src_level, dst, dst_level, a, b, c, scale, blurSM1Program);
}

// FUNCTION: LEGOBATMAN 0x006fa8d0
void NuMainFilter::render() {
  if (g_unk0094c0bb)
    NuMainFilterGen::render();
  else if (bloom_enabled)
    Unk006f7b30();
}

// FUNCTION: LEGOBATMAN 0x006fa8f0
void NuDeferredFilter::template_blurLuminanceMap() {
  Unk00530ca0(0x16, 1);
  Unk00530ca0(0xf, 0);
  Unk00530ca0(0x1b, 0);
  Unk00530ca0(7, 0);
  Unk00530ca0(0x34, 0);
  NuPostFilter::blur7x7(tex14, 0, tex18, 0, 1, program, true, 1.0f,
                        g_unk029f3e48);
}
