// nu2api/nufile/pc/nufile_ps_unk.cpp: the PC device/file layer after
// bgproc, 0x006e3050..0x006e3430. Mac order: NuFileInitEarly, NuFileExists,
// DEV_FormatName, DEVCDDVDROM_Interrogate, DEVHOST_Interrogate,
// DEVMEMORYCARD_Interrogate, NuFileSetBadGameDisc,
// NuFileCheckBadGameDiscStatus, getpointer, (NuThread*), NuPSFileClose,
// NuPSFileRead, NuPSFileWrite, NuPSFileLSeek.

#include "../../nucore/common.h"
#include <windows.h>

// GLOBAL: LEGOBATMAN 0x00b058d4
static int g_nuFileEarlyInit;
// GLOBAL: LEGOBATMAN 0x00b05478
CRITICAL_SECTION g_nuFileCritSec;

// FUNCTION: LEGOBATMAN 0x006e3050
void NuFileInitEarly(void) {
  if (!g_nuFileEarlyInit) {
    InitializeCriticalSection(&g_nuFileCritSec);
    g_nuFileEarlyInit = 1;
  }
}

int NuFileUnk006dd990(char *name);

// FUNCTION: LEGOBATMAN 0x006e3070
int NuFileExists(char *name) { return NuFileUnk006dd990(name); }

struct nudev_s {
  u8 pad00[0xc];
  i32 present; // 0x0c
};

// FUNCTION: LEGOBATMAN 0x006e31d0
i32 DEVCDDVDROM_Interrogate(nudev_s *dev) {
  dev->present = 1;
  return 1;
}

// FUNCTION: LEGOBATMAN 0x006e31e0
i32 DEVHOST_Interrogate(nudev_s *dev) {
  dev->present = 1;
  return 1;
}

// FUNCTION: LEGOBATMAN 0x006e31f0
i32 DEVMEMORYCARD_Interrogate(nudev_s *dev) {
  dev->present = 1;
  return 1;
}

// FUNCTION: LEGOBATMAN 0x006e3200
void NuFileSetBadGameDisc(void) {}

// FUNCTION: LEGOBATMAN 0x006e3210
i32 NuFileCheckBadGameDiscStatus(void) { return 0; }

// GLOBAL: LEGOBATMAN 0x0099f6f0
extern HANDLE g_nuPSFileHandles[64];

// FUNCTION: LEGOBATMAN 0x006e3220
i32 getpointer(void) {
  i32 i;
  for (i = 0; i < 64; i++) {
    if (g_nuPSFileHandles[i] == INVALID_HANDLE_VALUE)
      return i;
  }
  return -1;
}

// FUNCTION: LEGOBATMAN 0x006e3240
i32 NuPSFileClose(i32 fh) {
  BOOL ok = CloseHandle(g_nuPSFileHandles[fh]);
  g_nuPSFileHandles[fh] = INVALID_HANDLE_VALUE;
  return ok == FALSE;
}

// FUNCTION: LEGOBATMAN 0x006e3270
i32 NuPSFileRead(i32 fh, void *buf, i32 size) {
  DWORD read = 0;
  ReadFile(g_nuPSFileHandles[fh], buf, size, &read, NULL);
  return read;
}

// FUNCTION: LEGOBATMAN 0x006e32b0
i32 NuPSFileWrite(i32 fh, void *buf, i32 size) {
  DWORD written = 0;
  if (WriteFile(g_nuPSFileHandles[fh], buf, size, &written, NULL))
    return written;
  return 0;
}

// FUNCTION: LEGOBATMAN 0x006e32f0
__int64 NuPSFileLSeek(i32 fh, __int64 offset, i32 whence) {
  LARGE_INTEGER dist;
  LARGE_INTEGER pos;
  DWORD method;

  switch (whence) {
  case 0:
    method = FILE_BEGIN;
    break;
  case 1:
    method = FILE_CURRENT;
    break;
  default:
    method = FILE_END;
    break;
  }
  pos.QuadPart = 0;
  dist.QuadPart = offset;
  if (SetFilePointerEx(g_nuPSFileHandles[fh], dist, &pos, method))
    return pos.QuadPart;
  return -1;
}

// Mac order after NuPSFileLSeek: NuPSFileInitDevices, NuFileSetAppDirectory,
// NuFileGetAppDirectory, NuFileRename.

// FUNCTION: LEGOBATMAN 0x006e3360
void NuPSFileInitDevices(void) {}

// GLOBAL: LEGOBATMAN 0x00b03ad8
static char g_nuFileAppDirectory[256];

extern "C" char *strcpy(char *dst, const char *src);
#pragma intrinsic(strcpy)

// FUNCTION: LEGOBATMAN 0x006e3370
void NuFileSetAppDirectory(char *dir) { strcpy(g_nuFileAppDirectory, dir); }

// FUNCTION: LEGOBATMAN 0x006e3390
void NuFileGetAppDirectory(char *dir) { strcpy(dir, g_nuFileAppDirectory); }

// Expands a file name through the device layer (dst, src, size).
void NuFileUnk006d2ec0(char *dst, char *src, i32 size);

// FUNCTION: LEGOBATMAN 0x006e33b0
void NuFileRename(char *to, char *from) {
  char new_name[256];
  char old_name[256];

  NuFileUnk006d2ec0(old_name, from, 0x100);
  NuFileUnk006d2ec0(new_name, to, 0x100);
  DeleteFileA(new_name);
  MoveFileA(old_name, new_name);
}
