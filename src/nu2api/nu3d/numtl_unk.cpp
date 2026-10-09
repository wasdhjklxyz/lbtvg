// nu2api/nu3d/numtl_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nucore/common.h"

typedef struct numtl_s {
  unsigned char pad0[0x2c0];
  u16 version; // 0x2c0, bumped by NuMtlUpdate
  u16 filler5;
} NUMTL;

void NuMtlUpdatePS(NUMTL *mtl);

// FUNCTION: LEGOBATMAN 0x00727b40
void NuMtlUpdate(NUMTL *mtl) {
  NuMtlUpdatePS(mtl);
  mtl->version++;
}

struct NuDynamicLight {
  unsigned int data[0x12f0 / 4];

  static void destroy(NuDynamicLight *light);
};

// GLOBAL: LEGOBATMAN 0x029f8ef0
extern NuDynamicLight g_dynamicLights[20];
// GLOBAL: LEGOBATMAN 0x029df970
extern unsigned char g_dynamicLightUsed[20];

// FUNCTION: LEGOBATMAN 0x0072b040
void NuDynamicLight::destroy(NuDynamicLight *light) {
  for (int i = 0; i < 20; i++) {
    if (&g_dynamicLights[i] == light) {
      g_dynamicLightUsed[i] = 0;
    }
  }
}
