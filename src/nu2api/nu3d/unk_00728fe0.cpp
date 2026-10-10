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

// FUNCTION: LEGOBATMAN 0x00728fe0
void NuMotionFilterGen::destroyResources() {
  out_framebuffer.unregister();
  color_buffer.unregister();
  velocity_buffer.unregister();
  depth_buffer.unregister();
}
