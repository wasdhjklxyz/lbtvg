// nu2api/nu3d, file unknown: NuMotionFilterGen::initResources (RTTI vtable
// 0x89842c slot 1; Mac binds four shader parameters by name).

#include "../nucore/common.h"

// 12-byte shader table entry; only the reference count is evidenced.
struct NuShaderEntry {
  u32 u00;
  u32 u04;
  i32 refs; // 0x08
};

// The shader parameter table: entries first.
struct NuShaderTable {
  NuShaderEntry entries[1];

  i32 Find(const char *name, i32 flags);
};

// {index, table} handle into a shader parameter table.
struct NuShaderHandle {
  i32 index;            // 0x00
  NuShaderTable *table; // 0x04

  NuShaderEntry *Release() {
    NuShaderEntry *e = &table->entries[index];
    e->refs += -1;
    return e;
  }
  void Bind(const char *name, i32 flags, NuShaderTable *t) {
    if (index >= 0)
      Release();
    table = t;
    index = t->Find(name, flags);
  }
};

// GLOBAL: LEGOBATMAN 0x029f82f0
extern NuShaderTable g_unk029f82f0; // shader parameter table

struct NuMotionFilterGen {
  virtual ~NuMotionFilterGen();
  virtual void initResources();
  virtual void destroyResources();

  u8 pad04[8 - 4];
  NuShaderHandle out_framebuffer; // 0x08
  NuShaderHandle color_buffer;    // 0x10
  NuShaderHandle velocity_buffer; // 0x18
  NuShaderHandle depth_buffer;    // 0x20
};

// FUNCTION: LEGOBATMAN 0x00734e80
void NuMotionFilterGen::initResources() {
  out_framebuffer.Bind("postEffect.outFramebuffer", 0, &g_unk029f82f0);
  color_buffer.Bind("postEffect.colorBuffer", 0, &g_unk029f82f0);
  velocity_buffer.Bind("postEffect.velocityBuffer", 0, &g_unk029f82f0);
  depth_buffer.Bind("postEffect.depthBuffer", 0, &g_unk029f82f0);
}
