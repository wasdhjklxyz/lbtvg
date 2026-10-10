// nu2api/nu3d: NuMainFilterGen (RTTI vtable 0x8982bc; saga
// nupostfilter_generic.cpp). File name by analogy with the
// nuprocesscolourfilter_gen.cpp anchor at 0x701e80; unproven.

#include "../nucore/common.h"
#include "../numath/nuinline_unk.h"
#include "../numath/numtx_inline_unk.h"
#include "../numath/nutrig_unk.h"

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x006fd700
static f32 NuVecMagInline(f32 *v);

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x006fd810
static void NuMtxCopyInline(f32 *dst, f32 *src);

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x006fcf30
static f32 NuFabs(f32 f);
// FUNCTION: LEGOBATMAN 0x006fcf50
static f32 NuFdiv(f32 a, f32 b);
// FUNCTION: LEGOBATMAN 0x006fcf90
static f32 NuFsign(f32 f);
// FUNCTION: LEGOBATMAN 0x006fd120
static f32 NuSinApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x006fd1c0
static f32 NuCosApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x006fd400
static void NuVec4Copy(f32 *dst, f32 *src);
// FUNCTION: LEGOBATMAN 0x006fd420
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);
// FUNCTION: LEGOBATMAN 0x006fd490
static void NuVecScaleInline(f32 *dst, f32 *src, f32 s);

struct nueffecttex_s;

extern "C" nueffecttex_s *NuEffectTexCreate2D(i32 width, i32 height, i32 levels,
                                              i32 format, i32 usage);
u32 Unk006e4950(nueffecttex_s *tex);  // NuEffectTexGetLevels
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

struct NuMainFilterGen : NuPostFilterGen {
  virtual void initTextureResources(i32 width, i32 height);
  virtual void destroyTextureResources();
  virtual void reset();

  u8 pad0c[0x54 - 0xc];
  u16 u54; // 0x54, saga's dof_enabled/bloom_enabled (one word store here)
  bool motion_blur_enabled;          // 0x56
  u8 u57;                            // 0x57
  i32 active_filter_count;           // 0x58
  nueffecttex_s *blur_texture;       // 0x5c
  nueffecttex_s *downsample_texture; // 0x60
  nueffecttex_s *texture64;          // 0x64
  i32 texture64_size;                // 0x68
  u32 texture64_levels;              // 0x6c
  f32 blur_gain;                     // 0x70
  u32 downsample_lod;                // 0x74
  f32 blur_radius;                   // 0x78
  u8 pad7c[0x8c - 0x7c];
  f32 dof_blur; // 0x8c
};

// FUNCTION: LEGOBATMAN 0x006fe600
void NuMainFilterGen::initTextureResources(i32 width, i32 height) {
  texture64_size = 0x200;
  texture64_levels = 4;
  blur_texture = NuEffectTexCreate2D(0x200, 0x200, 2, 0x21, 2);
  downsample_lod = 0;
  i32 w = width, h = height;
  for (i32 i = 0; i < 3; ++i) {
    if (w < 128 || h < 128)
      break;
    ++downsample_lod;
    w /= 2;
    h /= 2;
  }
  downsample_texture = NuEffectTexCreate2D(w, h, 1, 0x21, 2);
  texture64 = NuEffectTexCreate2D(texture64_size, texture64_size,
                                  texture64_levels, 0x21, 2);
  texture64_levels = Unk006e4950(texture64);
  if (downsample_lod >= texture64_levels)
    downsample_lod = texture64_levels - 1;
  if (height < 704) {
    dof_blur -= 1.0f;
    blur_radius -= 0.85f;
    blur_gain -= 0.5f;
  }
}

// FUNCTION: LEGOBATMAN 0x006fe6f0
void NuMainFilterGen::destroyTextureResources() {
  Unk006e47f0(blur_texture);
  Unk006e47f0(downsample_texture);
  Unk006e47f0(texture64);
}

// FUNCTION: LEGOBATMAN 0x006fe720
void NuMainFilterGen::reset() {
  enabled = false;
  u54 = 0;
  motion_blur_enabled = false;
  active_filter_count = 0;
}

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_numainfilter_gen_unk(f32 *v, f32 a, i32 i) {
  v[2] = NuFabs(a);
  v[3] = NuFdiv(a, v[4]);
  v[5] = NuFsign(a);
  v[0] = NuSinApprox(i);
  v[1] = NuCosApprox(i);
  NuVec4Copy(v + 4, v);
  NuVec4Set(v, a, a, a, a);
  NuVecScaleInline(v + 8, v, a);
}

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_2_numainfilter_gen_unk(f32 *v, f32 a, i32 i) {
  NuMtxCopyInline(v + 32, v + 16);
}

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_3_numainfilter_gen_unk(f32 *v, f32 a, i32 i) {
  v[48] = NuVecMagInline(v);
}
