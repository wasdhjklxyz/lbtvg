// nu2api/nu3d, file unknown (between nugscn_dlist.cpp and numtl_dlist.cpp):
// NuMainFilter's texture resources (RTTI vtable 0x8979e4, slots 3 and 4).

#include "../nucore/common.h"

struct nueffecttex_s;

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
