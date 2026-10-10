// nu2api/nu3d, file unknown (just before NuMovieGrab_PC.cpp): NuEdgeAAFilter's
// texture resources (RTTI vtable 0x898334, slots 3 and 4).

#include "../nucore/common.h"

struct nueffecttex_s;

// GLOBAL: LEGOBATMAN 0x029dcc08
extern nueffecttex_s *g_unk029dcc08; // the back buffer's effect texture

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

struct NuEdgeAAFilterGen : NuPostFilterGen {
  virtual void initTextureResources(i32 width, i32 height);
  virtual void destroyTextureResources();
};

struct NuEdgeAAFilter : NuEdgeAAFilterGen {
  virtual void initTextureResources(i32 width, i32 height);
  virtual void destroyTextureResources();

  NUATTACH_s attachment; // 0x0c
};

// FUNCTION: LEGOBATMAN 0x006e8ec0
void NuEdgeAAFilter::initTextureResources(i32 width, i32 height) {
  NuEdgeAAFilterGen::initTextureResources(width, height);
  attachment.Unk006e85e0(g_unk029dcc08, 0, 0);
}

// FUNCTION: LEGOBATMAN 0x006e8ef0
void NuEdgeAAFilter::destroyTextureResources() {
  NuEdgeAAFilterGen::destroyTextureResources();
  attachment.Unk006e8580();
}
