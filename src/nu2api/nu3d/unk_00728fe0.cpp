// nu2api/nu3d, file unknown: NuMotionFilterGen::destroyResources (RTTI
// vtable 0x89842c slot 2; Mac releases four shader handles).

#include "../nucore/common.h"

struct NuFramebuffer;
struct NuProxyAttachment;

// 12-byte port entry; only the reference count is evidenced.
struct NuDataPortEntry {
  u32 u00;
  u32 u04;
  i32 refs; // 0x08
};

// Entries first.
class NuDataPortManager {
public:
  i32 registerPort(char const *name, void *data);

  NuDataPortEntry ports[1];
};

// Mac NuDataPort<T>: {index, manager}; the Mac assert in unregister is
// compiled out here.
template <class T> class NuDataPort {
public:
  ~NuDataPort() {
    if (index >= 0)
      unregister();
  }
  NuDataPortEntry *unregister() {
    NuDataPortEntry *e = &manager->ports[index];
    e->refs += -1;
    return e;
  }
  void registerData(char const *name, T data, NuDataPortManager &mgr) {
    if (index >= 0)
      unregister();
    manager = &mgr;
    index = mgr.registerPort(name, data);
  }

  i32 index;                  // 0x00
  NuDataPortManager *manager; // 0x04
};

struct NuMotionFilterGen {
  virtual ~NuMotionFilterGen();
  virtual void initResources();
  virtual void destroyResources();

  u8 pad04[8 - 4];
  NuDataPort<NuFramebuffer *> out_framebuffer;     // 0x08
  NuDataPort<NuProxyAttachment *> color_buffer;    // 0x10
  NuDataPort<NuProxyAttachment *> velocity_buffer; // 0x18
  NuDataPort<NuProxyAttachment *> depth_buffer;    // 0x20
};

// RTTI vtable 0x8982bc; fields as in src/nu2api/nu3d/numainfilter_gen_unk.cpp.
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

struct NuMainFilterGen : NuPostFilterGen {
  NuMainFilterGen();
  virtual void initResources();
  virtual void destroyResources();

  u8 pad0c[0x54 - 0xc];
  u16 u54;                  // 0x54, saga's dof_enabled/bloom_enabled
  bool motion_blur_enabled; // 0x56
  u8 u57;                   // 0x57
  i32 active_filter_count;  // 0x58
  u8 pad5c[0x70 - 0x5c];
  f32 f70; // 0x70
  i32 i74; // 0x74
  f32 f78; // 0x78
  i32 i7c; // 0x7c
  f32 f80; // 0x80
  f32 f84; // 0x84
  f32 f88; // 0x88
  f32 f8c; // 0x8c
  i32 i90; // 0x90
  i32 i94; // 0x94
  u8 pad98[0x118 - 0x98];
  f32 f118; // 0x118
  f32 f11c; // 0x11c
  f32 f120; // 0x120
};

// FUNCTION: LEGOBATMAN 0x00728920
NuMainFilterGen::NuMainFilterGen() {
  f80 = 1.0f;
  f84 = 1.0f;
  f88 = 1.0f;
  i94 = 0;
  f8c = 3.0f;
  i90 = 3;
  i7c = 0;
  f78 = 5.0f;
  i74 = 0;
  f70 = 2.1f;
  f118 = 0.0f;
  f11c = 0.0f;
  f120 = 1.0f;
}

// SYNTHETIC: LEGOBATMAN 0x00728990 NuMainFilterGen::`scalar deleting destructor'

// FUNCTION: LEGOBATMAN 0x007289b0
void NuMainFilterGen::initResources() {
  u54 = 0;
  motion_blur_enabled = false;
  active_filter_count = 0;
}

// FUNCTION: LEGOBATMAN 0x007289c0
void NuMainFilterGen::destroyResources() {}

// FUNCTION: LEGOBATMAN 0x00728fe0
void NuMotionFilterGen::destroyResources() {
  out_framebuffer.unregister();
  color_buffer.unregister();
  velocity_buffer.unregister();
  depth_buffer.unregister();
}

// The platform filters derived from the *Gen classes, constructed here with
// their Gen constructors inlined.

struct nueffecttex_s;

struct NuMotionAccumFilterGen : NuPostFilterGen {
  NuMotionAccumFilterGen() {
    f10 = 0;
    blend = 1.0f;
    f18 = 0.0f;
    frames = 1;
    mode = 0;
    current_frame = 0;
    b28 = false;
    b29 = false;
  }
  virtual void destroyResources();
  virtual void destroyTextureResources();

  nueffecttex_s *accumulation_texture; // 0x0c
  i32 f10;                             // 0x10
  f32 blend;                           // 0x14
  f32 f18;                             // 0x18
  i32 frames;                          // 0x1c
  i32 mode;                            // 0x20
  i32 current_frame;                   // 0x24
  bool b28;                            // 0x28
  bool b29;                            // 0x29
};

struct NuMotionAccumFilter : NuMotionAccumFilterGen {
  NuMotionAccumFilter();
  ~NuMotionAccumFilter();
  virtual void initResources();
  virtual void initTextureResources(i32 width, i32 height);
  virtual void render();
};

// FUNCTION: LEGOBATMAN 0x00729330
NuMotionAccumFilter::NuMotionAccumFilter() {}

// FUNCTION: LEGOBATMAN 0x00729360
NuMotionAccumFilter::~NuMotionAccumFilter() {}

struct NuSpeedBlurFilterGen : NuPostFilterGen {
  virtual void initResources();
  virtual void destroyResources();
  virtual void initTextureResources(i32 width, i32 height);
  virtual void destroyTextureResources();
};

struct NuSpeedBlurFilter : NuSpeedBlurFilterGen {
  NuSpeedBlurFilter();
  ~NuSpeedBlurFilter();
  virtual void initResources();
  virtual void initTextureResources(i32 width, i32 height);
  virtual void destroyTextureResources();
  virtual void render();
};

// FUNCTION: LEGOBATMAN 0x00729370
NuSpeedBlurFilter::NuSpeedBlurFilter() {}

// FUNCTION: LEGOBATMAN 0x00729380
NuSpeedBlurFilter::~NuSpeedBlurFilter() {}

struct NuDeferredFilterGen : NuPostFilterGen {
  NuDeferredFilterGen() {
    enabled = false;
    i38 = 0;
    f3c = 0.0f;
  }
  virtual void destroyResources();
  virtual void destroyTextureResources();
  virtual void render();
  virtual void resetAll();
  virtual bool isEnabled();

  u8 pad0c[0x38 - 0xc];
  i32 i38; // 0x38
  f32 f3c; // 0x3c
};

struct NuDeferredFilter : NuDeferredFilterGen {
  NuDeferredFilter();
  ~NuDeferredFilter();
  virtual void initResources();
  virtual void initTextureResources(i32 width, i32 height);
  virtual void template_blurLuminanceMap();
};

// FUNCTION: LEGOBATMAN 0x00729440
NuDeferredFilter::NuDeferredFilter() {}

// FUNCTION: LEGOBATMAN 0x00729460
NuDeferredFilter::~NuDeferredFilter() {}

// SYNTHETIC: LEGOBATMAN 0x00729470 NuMotionAccumFilter::`scalar deleting destructor'
// SYNTHETIC: LEGOBATMAN 0x00729490 NuSpeedBlurFilter::`scalar deleting destructor'
// SYNTHETIC: LEGOBATMAN 0x007294d0 NuDeferredFilter::`scalar deleting destructor'
