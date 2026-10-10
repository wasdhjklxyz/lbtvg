// batman/, file unknown: DynamicMaterialManager and HiresTextureManager
// (Mac: StringToHash, IsDynamicMaterial, GetMaterial_Internal, IsLoaded,
// GetTexture_Internal, DumpLevel), 0x67a810..0x67abc8.

#include "../nu2api/nucore/common.h"
#include "../nu2api/nucore/nustring.h"
#include <new>

struct WORLDINFO_s;
struct numtl_s;

// Raw view of the WORLDINFO_s level buffer (0x104).
struct HTWORLD_s {
  u8 pad0[0x104];
  variptr_u buf;     // 0x104
  variptr_u buf_end; // 0x108
};

i32 NuTexRead(char *name, variptr_u *buf, variptr_u buf_end);
void NuTexDestroy(i32 tid);

class DynamicMaterialManager {
public:
  static u32 StringToHash(char const *str);
  static i32 IsDynamicMaterial(numtl_s *mtl);

  // GLOBAL: LEGOBATMAN 0x00ad2af8
  static DynamicMaterialManager m_oSingleton;

  numtl_s *mtls[0x20]; // 0x000
  void *b[0x40];       // 0x080
  void *c[0x40];       // 0x180
};

struct HiresTexture {
  HiresTexture() : tid(0), name(0) {}

  WORLDINFO_s *world; // 0x0
  i32 tid;            // 0x4
  char *name;         // 0x8
};

class HiresTextureManager {
public:
  i32 IsLoaded(char *name, WORLDINFO_s *world);
  i32 GetTexture_Internal(char *name, WORLDINFO_s *world);
  static void DumpLevel(WORLDINFO_s *world);

  // GLOBAL: LEGOBATMAN 0x00ad2d78
  static HiresTextureManager m_oSingleton;

  HiresTexture *textures[0x10]; // 0x00
};

// FUNCTION: LEGOBATMAN 0x0067a810
u32 DynamicMaterialManager::StringToHash(char const *str) {
  u32 hash = 0x811c9dc5;
  while (*str != 0) {
    hash = (hash ^ *str) * 0x199933;
    str++;
  }
  return hash;
}

// FUNCTION: LEGOBATMAN 0x0067a840
i32 DynamicMaterialManager::IsDynamicMaterial(numtl_s *mtl) {
  if (mtl != 0) {
    for (i32 i = 0; i < 0x20; i++) {
      if (m_oSingleton.mtls[i] == mtl)
        return 1;
    }
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x0067aa40
i32 HiresTextureManager::IsLoaded(char *name, WORLDINFO_s *world) {
  for (i32 i = 0; i < 0x10; i++) {
    HiresTexture *tex = textures[i];
    if (tex != 0 && tex->world == world && tex->tid > 0 &&
        NuStrICmp(tex->name, name) == 0)
      return tex->tid;
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x0067aaa0
void HiresTextureManager::DumpLevel(WORLDINFO_s *world) {
  for (i32 i = 0; i < 0x10; i++) {
    HiresTexture *tex = m_oSingleton.textures[i];
    if (tex != 0 && tex->world == world) {
      if (tex->tid != 0)
        NuTexDestroy(tex->tid);
      m_oSingleton.textures[i] = 0;
    }
  }
}

void NuSevereWarning(const char *fmt, ...);

// STUB: LEGOBATMAN 0x0067aae0
// ebx/edi swapped (orig keeps &world->buf in ebx, the slot index in edi);
// everything else matches.
i32 HiresTextureManager::GetTexture_Internal(char *name, WORLDINFO_s *world) {
  if (name == 0 || NuStrLen(name) == 0)
    return 0;
  i32 tid = IsLoaded(name, world);
  if (tid != 0)
    return tid;
  HTWORLD_s *w = (HTWORLD_s *)world;
  void *saved = w->buf.void_ptr;
  for (i32 i = 0; i < 0x10; i++) {
    if (textures[i] == 0) {
      w->buf.addr = (w->buf.addr + 0xf) & ~0xf;
      HiresTexture *tex = new (w->buf.void_ptr) HiresTexture;
      textures[i] = tex;
      w->buf.addr += sizeof(HiresTexture);
      tex->world = 0;
      tex->tid = 0;
      tex->name = "";
      tex->tid = NuTexRead(name, &w->buf, w->buf_end);
      if (tex->tid != 0) {
        tex->name = name;
        tex->world = world;
        return tex->tid;
      }
      w->buf.void_ptr = saved;
      textures[i] = 0;
      return 0;
    }
  }
  NuSevereWarning(name);
  return 0;
}
