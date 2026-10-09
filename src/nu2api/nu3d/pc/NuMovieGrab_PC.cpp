// nu2api/nu3d/pc/NuMovieGrab_PC.cpp: __FILE__ anchors at
// 0x006ea240..0x006ea2d0.

#include "../../nucore/common.h"

extern "C" void *NuMemAllocFn(int size, const char *file, int line);
extern "C" void NuMemFreeFn(void *ptr, const char *file, int line);

typedef struct numoviegrabinfo_s {
  i32 width;    // 0x00
  i32 height;   // 0x04
  i32 bpp;      // 0x08
  void *buffer; // 0x0c
} NUMOVIEGRABINFO;

typedef struct nudisplaymode_s {
  i32 width;
  i32 height;
  i32 f8;
  i32 fc;
} NUDISPLAYMODE;

// GLOBAL: LEGOBATMAN 0x009d0eec
extern NUDISPLAYMODE *g_unk009d0eec; // display mode table

// GLOBAL: LEGOBATMAN 0x009d0ef0
extern i32 g_unk009d0ef0; // current display mode index

void Unk006e47f0(void *shader);

// GLOBAL: LEGOBATMAN 0x029f1b38
extern void *g_nuInstSurfGeomUnk029f1b38;
// GLOBAL: LEGOBATMAN 0x029f1b3c
extern void *g_nuInstSurfGeomUnk029f1b3c;

struct NuInstSurfGeom {
  static i32 DestroyPS();
};

// FUNCTION: LEGOBATMAN 0x006e8f20
i32 NuInstSurfGeom::DestroyPS() {
  Unk006e47f0(g_nuInstSurfGeomUnk029f1b38);
  Unk006e47f0(g_nuInstSurfGeomUnk029f1b3c);
  return 1;
}

// FUNCTION: LEGOBATMAN 0x006ea240
i32 NuMovieGrabAttachFrameBuffer(NUMOVIEGRABINFO *info) {
  NUDISPLAYMODE mode = g_unk009d0eec[g_unk009d0ef0];
  info->width = mode.width;
  info->height = mode.height;
  info->buffer =
      NuMemAllocFn(info->bpp * mode.height * mode.width + 0x3c, __FILE__, 0x2e);
  return info->buffer != 0;
}

// FUNCTION: LEGOBATMAN 0x006ea2a0
void NuMovieGrabDetachFrameBuffer(NUMOVIEGRABINFO *info) {
  if (info->buffer != 0) {
    NuMemFreeFn(info->buffer, __FILE__, 0x3f);
    info->buffer = 0;
  }
}
