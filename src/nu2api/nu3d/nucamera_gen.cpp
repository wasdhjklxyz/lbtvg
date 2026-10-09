// nu2api/nu3d/nucamera_gen.cpp: __FILE__ anchor at 0x007151e0
// (NuCameraDestroy).

typedef struct nucamera_s NUCAMERA;

extern "C" void NuMemFreeFn(void *ptr, const char *file, int line);

// FUNCTION: LEGOBATMAN 0x007151e0
void NuCameraDestroy(NUCAMERA *camera) {
  if (camera != 0) {
    NuMemFreeFn(camera, __FILE__, 0x147);
  }
}
