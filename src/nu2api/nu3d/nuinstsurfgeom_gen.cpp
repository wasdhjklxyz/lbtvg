// nu2api/nu3d/nuinstsurfgeom_gen.cpp (leaked __FILE__ in Destroy).

#include "../nucore/common.h"
#include <stddef.h>

extern "C" void NuMemFreeFn(void *ptr, const char *file, int line);

struct NuInstSurfGeom {
  static i32 Destroy();
  static i32 DestroyPS();
};

// GLOBAL: LEGOBATMAN 0x029f3ebc
extern i32 g_nuInstSurfGeomUnk029f3ebc;
// GLOBAL: LEGOBATMAN 0x029f3ecc
extern i32 g_nuInstSurfGeomUnk029f3ecc;
// GLOBAL: LEGOBATMAN 0x029f3ed0
extern void *g_nuInstSurfGeomUnk029f3ed0;
// GLOBAL: LEGOBATMAN 0x029f3ed8
extern i32 g_nuInstSurfGeomUnk029f3ed8;
// GLOBAL: LEGOBATMAN 0x029f3edc
extern void *g_nuInstSurfGeomUnk029f3edc;

// FUNCTION: LEGOBATMAN 0x00709de0
i32 NuInstSurfGeom::Destroy() {
  if (g_nuInstSurfGeomUnk029f3ed0 != NULL)
    NuMemFreeFn(g_nuInstSurfGeomUnk029f3ed0, __FILE__, 0x84);
  if (g_nuInstSurfGeomUnk029f3edc != NULL)
    NuMemFreeFn(g_nuInstSurfGeomUnk029f3edc, __FILE__, 0x86);
  g_nuInstSurfGeomUnk029f3ecc = 0;
  g_nuInstSurfGeomUnk029f3ed0 = NULL;
  g_nuInstSurfGeomUnk029f3ed8 = 0;
  g_nuInstSurfGeomUnk029f3edc = NULL;
  DestroyPS();
  g_nuInstSurfGeomUnk029f3ebc = 0;
  return 1;
}
