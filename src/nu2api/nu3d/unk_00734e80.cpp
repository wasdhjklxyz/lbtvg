// nu2api/nu3d, file unknown: NuMotionFilterGen::initResources (RTTI vtable
// 0x89842c slot 1; Mac binds four shader parameters by name).

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

// GLOBAL: LEGOBATMAN 0x029f82f0
extern NuDataPortManager g_unk029f82f0; // the data port manager

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

// FUNCTION: LEGOBATMAN 0x00734e80
void NuMotionFilterGen::initResources() {
  out_framebuffer.registerData("postEffect.outFramebuffer", 0, g_unk029f82f0);
  color_buffer.registerData("postEffect.colorBuffer", 0, g_unk029f82f0);
  velocity_buffer.registerData("postEffect.velocityBuffer", 0, g_unk029f82f0);
  depth_buffer.registerData("postEffect.depthBuffer", 0, g_unk029f82f0);
}
