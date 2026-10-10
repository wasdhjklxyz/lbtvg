// nu2api/nu3d, file unknown: NuMotionFilterGen::destroyResources (RTTI
// vtable 0x89842c slot 2; Mac releases four shader handles).

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

// FUNCTION: LEGOBATMAN 0x00728fe0
void NuMotionFilterGen::destroyResources() {
  out_framebuffer.Release();
  color_buffer.Release();
  velocity_buffer.Release();
  depth_buffer.Release();
}
