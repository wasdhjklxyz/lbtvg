// nu2api/nu3d/pc/NuMovieGrab_PC.cpp: __FILE__ anchors at
// 0x006ea240..0x006ea2d0.

#include "../../nucore/common.h"
#include <stdio.h>

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

// GLOBAL: LEGOBATMAN 0x029dcc11
extern char g_nuScreenDumpPending;
// GLOBAL: LEGOBATMAN 0x029d1048
extern char *g_nuScreenDumpName;
// GLOBAL: LEGOBATMAN 0x029d104c
extern i32 g_nuScreenDumpIndex;
// GLOBAL: LEGOBATMAN 0x029d1050
extern i32 g_nuScreenDumpUnk029d1050;
// GLOBAL: LEGOBATMAN 0x029d1054
extern i32 g_nuScreenDumpUnk029d1054;

void Unk00529d40(i32 a, i32 b);
void *Unk006dd970(char *name, i32 mode);
i32 Unk00529c10();
i32 Unk00529c20();
void Unk006d3260(void *file, i32 a, i32 b);
void Unk006dcb00(void *file);
void Unk00529c30();

// FUNCTION: LEGOBATMAN 0x006ea1a0
extern "C" void NuScreenDump(void) {
  char name[0x100];
  if (g_nuScreenDumpPending) {
    sprintf(name, "%s_%d.bmp", g_nuScreenDumpName, g_nuScreenDumpIndex);
    Unk00529d40(g_nuScreenDumpUnk029d1050, g_nuScreenDumpUnk029d1054);
    void *file = Unk006dd970(name, 1);
    if (file != NULL) {
      Unk006d3260(file, Unk00529c20(), Unk00529c10());
      Unk006dcb00(file);
    }
    Unk00529c30();
    g_nuScreenDumpPending = 0;
  }
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

class CD3DCoreGrab {
public:
  i32 Unk0052bbf0(i32 width, i32 height, u8 **bitmap);
};

// GLOBAL: LEGOBATMAN 0x009d08d0
extern CD3DCoreGrab g_d3dCoreGrab;

extern "C" void *memcpy(void *dst, const void *src, unsigned int size);

// Grabs the back buffer as a .bmp image and copies its pixels out.
// FUNCTION: LEGOBATMAN 0x006ea2d0
i32 NuMovieGrabGetFrame(NUMOVIEGRABINFO *info) {
  if (info->buffer == 0)
    return 0;
  i32 width = info->width;
  i32 height = info->height;
  u8 *bitmap = 0;
  i32 size = g_d3dCoreGrab.Unk0052bbf0(width, height, &bitmap);
  if (size == *(i32 *)(bitmap + 2)) {
    u8 *pixels = bitmap + 0x36;
    if (info->bpp == 4) {
      memcpy(info->buffer, pixels, width * height * 4);
    } else if (info->bpp == 3) {
      for (i32 y = 0; y < info->height; y++) {
        for (i32 x = 0; x < info->width; x++) {
          ((u8 *)info->buffer)[(info->width * y + x) * 3] =
              pixels[(info->width * y + x) * 4];
          ((u8 *)info->buffer)[(info->width * y + x) * 3 + 1] =
              pixels[(info->width * y + x) * 4 + 1];
          ((u8 *)info->buffer)[(info->width * y + x) * 3 + 2] =
              pixels[(info->width * y + x) * 4 + 2];
        }
      }
    }
    NuMemFreeFn(bitmap, __FILE__, 0x87);
    return 1;
  }
  NuMemFreeFn(bitmap, __FILE__, 0x8c);
  return 0;
}
