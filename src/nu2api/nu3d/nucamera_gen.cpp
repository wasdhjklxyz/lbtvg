// nu2api/nu3d/nucamera_gen.cpp: __FILE__ anchor at 0x007151e0
// (NuCameraDestroy).

#include "../numath/numtx.h"
#include "../numath/nuvec.h"

typedef struct nucamera_s {
  NUMTX mtx;      // 0x00
  f32 fov;        // 0x40
  f32 unknown_44; // 0x44
  f32 aspect;     // 0x48
  f32 near_clip;  // 0x4c
  f32 far_clip;   // 0x50
  f32 unknown_54; // 0x54
  f32 unknown_58; // 0x58
  f32 unknown_5c; // 0x5c
  NUVEC scale;    // 0x60
} NUCAMERA;

void NuMtxSetIdentity(NUMTX *m);
extern "C" void *NuMemAllocFn(int size, const char *file, int line);

extern "C" void NuMemFreeFn(void *ptr, const char *file, int line);

// FUNCTION: LEGOBATMAN 0x00715180
NUCAMERA *NuCameraCreate() {
  NUCAMERA *cam = (NUCAMERA *)NuMemAllocFn(sizeof(NUCAMERA), __FILE__, 0x12a);

  NuMtxSetIdentity(&cam->mtx);

  cam->fov = 0.75f;
  cam->unknown_44 = 0.0f;
  cam->near_clip = 0.15f;
  cam->unknown_5c = 0.0f;
  cam->far_clip = 10000.0f;
  cam->aspect = 0.75f;
  cam->scale.z = 1.0f;
  cam->scale.y = 1.0f;
  cam->scale.x = 1.0f;
  cam->unknown_58 = 0.0f;
  cam->unknown_54 = 0.0f;

  return cam;
}

// FUNCTION: LEGOBATMAN 0x007151e0
void NuCameraDestroy(NUCAMERA *camera) {
  if (camera != 0) {
    NuMemFreeFn(camera, __FILE__, 0x147);
  }
}
