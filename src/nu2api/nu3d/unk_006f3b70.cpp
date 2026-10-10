// nu2api/nu3d, file unknown (between nugscn_dlist.cpp and numtl_dlist.cpp):
// NuEffectTexCreate2D (inlined into NuMotionAccumFilter::initTextureResources,
// so one TU), NuMainFilter's texture resources (RTTI vtable 0x8979e4, slots 3
// and 4), NuMotionAccumFilter (RTTI vtable 0x8982e4).

#include "../nucore/common.h"

// 56-byte effect texture; layout unknown here.
struct nueffecttex_s {
  u8 pad[0x38];
};

void Unk006e40d0(); // error report
void Unk006e4eb0(nueffecttex_s *tex, i32 width, i32 height, i32 levels,
                 i32 format, i32 a, i32 usage); // initialises the texture

// GLOBAL: LEGOBATMAN 0x0299656c
extern u8 g_unk0299656c[32]; // effect texture pool: used flags
// GLOBAL: LEGOBATMAN 0x00b10ad8
extern nueffecttex_s g_unk00b10ad8[32]; // effect texture pool

static inline nueffecttex_s *NuEffectTexAlloc() {
  for (i32 i = 0; i != 32; i++) {
    if (!g_unk0299656c[i]) {
      g_unk0299656c[i] = 1;
      return &g_unk00b10ad8[i];
    }
  }
  Unk006e40d0();
  return 0;
}

// FUNCTION: LEGOBATMAN 0x006f37b0
extern "C" nueffecttex_s *NuEffectTexCreate2D(i32 width, i32 height, i32 levels,
                                              i32 format, i32 usage) {
  nueffecttex_s *tex = NuEffectTexAlloc();
  format &= ~0x30;
  switch (format) {
  case 0:
    break;
  case 1:
    break;
  default:
    Unk006e40d0();
    return 0;
  }
  Unk006e4eb0(tex, width, height, levels, format, 0, usage);
  return tex;
}

// GLOBAL: LEGOBATMAN 0x029dcc08
extern nueffecttex_s *g_unk029dcc08; // the back buffer's effect texture
// GLOBAL: LEGOBATMAN 0x0094c0bb
extern u8 g_unk0094c0bb;

// A render-target attachment (unknown class) embedded in the PC filters.
struct NUATTACH_s {
  void Unk006e85e0(nueffecttex_s *tex, i32 a, i32 b); // attach
  void Unk006e8580();                                 // release
};

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

  u8 pad0c[0x134 - 0xc];
};

struct NuMainFilter : NuMainFilterGen {
  virtual void initTextureResources(i32 width, i32 height);
  virtual void destroyTextureResources();

  NUATTACH_s attachment; // 0x134
};

// FUNCTION: LEGOBATMAN 0x006f3b70
void NuMainFilter::initTextureResources(i32 width, i32 height) {
  if (g_unk0094c0bb)
    NuMainFilterGen::initTextureResources(width, height);
  attachment.Unk006e85e0(g_unk029dcc08, 0, 0);
}

// FUNCTION: LEGOBATMAN 0x006f3bb0
void NuMainFilter::destroyTextureResources() {
  if (g_unk0094c0bb)
    NuMainFilterGen::destroyTextureResources();
  attachment.Unk006e8580();
}

struct NuMotionAccumFilterGen : NuPostFilterGen {
  virtual void initTextureResources(i32 width, i32 height);
  virtual void destroyTextureResources();

  nueffecttex_s *accumulation_texture; // 0x0c
};

struct NuMotionAccumFilter : NuMotionAccumFilterGen {
  virtual void initTextureResources(i32 width, i32 height);

  u8 pad10[0xac - 0x10];
  nueffecttex_s *texture; // 0xac
};

// FUNCTION: LEGOBATMAN 0x006f8210
void NuMotionAccumFilter::initTextureResources(i32 width, i32 height) {
  NuMotionAccumFilterGen::initTextureResources(width, height);
  texture = NuEffectTexCreate2D(width, height, 1, 1, 2);
}
