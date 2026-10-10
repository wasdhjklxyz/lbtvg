// nu2api/nucore/pc/nufile_pc.cpp: __FILE__ anchor at 0x006e3430 (see
// docs/linkmap.md). Mac order: NuSysDirOpen, NuFileOpenDir, NuSysDirClose,
// NuFileCloseDir, NuSysDirRead, NuFileReadDir, NuFileCreateDir,
// NuFileCreatePath, NuFileGetInfo, NuFileDelete, then the NuThread layer.

#include "../common.h"
#include <string.h>
#include <windows.h>

// --- NuSysDir ---------------------------------------------------------------

extern "C" void *NuMemAllocFn(int size, const char *file, int line);
extern "C" void NuMemFreeFn(void *ptr, const char *file, int line);

// One NuSysDirRead result. Times are local, one byte per field.
struct NUSYSDIRTIME_s {
  u8 second; // 0x0
  u8 minute; // 0x1
  u8 hour;   // 0x2
  u8 day;    // 0x3
  u8 month;  // 0x4
  u8 year;   // 0x5, low byte of the full year
  u8 zero;   // 0x6
};

struct NUSYSDIRENTRY_s {
  u32 mode; // 0x00, 0x100 read, 0x180 read-only (sic), +0x2000 directory
  u8 pad4[8 - 4];
  u32 size; // 0x08
  u8 pad0c;
  NUSYSDIRTIME_s created; // 0x0d
  u8 pad14;
  NUSYSDIRTIME_s accessed; // 0x15
  u8 pad1c;
  NUSYSDIRTIME_s written; // 0x1d
  u32 u24;                // 0x24
  u8 pad28[0x40 - 0x28];
  char name[MAX_PATH]; // 0x40
};

// GLOBAL: LEGOBATMAN 0x00b058d8
i32 nusysdir_count;
// GLOBAL: LEGOBATMAN 0x00b058e0
char *nusysdir_names[16];
// GLOBAL: LEGOBATMAN 0x00b03970
HANDLE nusysdir_handles[16];
// GLOBAL: LEGOBATMAN 0x00b03c70
WIN32_FIND_DATAA nusysdir_finddata[16];

// FUNCTION: LEGOBATMAN 0x006e3430
i32 NuSysDirOpen(char *path) {
  char buf[200];
  i32 i;
  if (nusysdir_count == 16)
    return 0;
  for (i = 0; i < 16; i++) {
    if (nusysdir_names[i] == NULL)
      break;
  }
  if (i == 16)
    return 0;
  strcpy(buf, path);
  if (buf[strlen(buf) - 1] != '\\')
    strcat(buf, "\\");
  strcat(buf, "*.*");
  nusysdir_names[i] = (char *)NuMemAllocFn(strlen(buf) + 4, __FILE__, 0x22a);
  strcpy(nusysdir_names[i], buf);
  nusysdir_count++;
  nusysdir_handles[i] =
      FindFirstFileA(nusysdir_names[i], &nusysdir_finddata[i]);
  if (nusysdir_handles[i] == INVALID_HANDLE_VALUE) {
    NuMemFreeFn(nusysdir_names[i], __FILE__, 0x232);
    nusysdir_count--;
    nusysdir_names[i - 1] = NULL; // sic (Mac too)
    return -1;
  }
  return i;
}

// FUNCTION: LEGOBATMAN 0x006e3610
i32 NuSysDirClose(i32 dir) {
  NuMemFreeFn(nusysdir_names[dir], __FILE__, 0x23e);
  nusysdir_names[dir - 1] = NULL; // sic
  FindClose(nusysdir_handles[dir]);
  nusysdir_count--;
  return 0;
}

// FUNCTION: LEGOBATMAN 0x006e3660
i32 NuSysDirRead(i32 dir, NUSYSDIRENTRY_s *entry) {
  FILETIME local;
  SYSTEMTIME st;
  WIN32_FIND_DATAA *data = &nusysdir_finddata[dir];
  u32 mode = 0x100;
  if (data->dwFileAttributes & FILE_ATTRIBUTE_READONLY)
    mode = 0x180;
  if (data->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
    mode += 0x2000;
  entry->mode = mode;
  entry->size = nusysdir_finddata[dir].nFileSizeLow;
  entry->u24 = 0;
  strcpy(entry->name, nusysdir_finddata[dir].cFileName);
  FileTimeToLocalFileTime(&nusysdir_finddata[dir].ftCreationTime, &local);
  FileTimeToSystemTime(&local, &st);
  entry->created.second = (u8)st.wSecond;
  entry->created.minute = (u8)st.wMinute;
  entry->created.hour = (u8)st.wHour;
  entry->created.day = (u8)st.wDay;
  entry->created.month = (u8)st.wMonth;
  entry->created.year = (u8)st.wYear;
  entry->created.zero = 0;
  FileTimeToLocalFileTime(&nusysdir_finddata[dir].ftLastWriteTime, &local);
  FileTimeToSystemTime(&local, &st);
  entry->written.second = (u8)st.wSecond;
  entry->written.minute = (u8)st.wMinute;
  entry->written.hour = (u8)st.wHour;
  entry->written.day = (u8)st.wDay;
  entry->written.month = (u8)st.wMonth;
  entry->written.year = (u8)st.wYear;
  entry->written.zero = 0;
  FileTimeToLocalFileTime(&nusysdir_finddata[dir].ftLastAccessTime, &local);
  FileTimeToSystemTime(&local, &st);
  entry->accessed.second = (u8)st.wSecond;
  entry->accessed.minute = (u8)st.wMinute;
  entry->accessed.hour = (u8)st.wHour;
  entry->accessed.day = (u8)st.wDay;
  entry->accessed.month = (u8)st.wMonth;
  entry->accessed.year = (u8)st.wYear;
  entry->accessed.zero = 0;
  if (data->dwFileAttributes == 0xffffffff)
    return 0;
  if (FindNextFileA(nusysdir_handles[dir], data) != 1)
    data->dwFileAttributes = 0xffffffff;
  return strlen(entry->name);
}

// --- NuFile directory API over NuSysDir
// ---------------------------------------

// The fields of nupad_gen.cpp's NUFILEDEVICE these use.
struct NUFILEDEVICE {
  i32 id; // 0x0, 1 = no directories (NuFileOpenDir returns 0x1000), 2 = ";1"
  u8 pad4[0x22c - 4];
  i32 (*format_name_fn)(NUFILEDEVICE *device, char *dst, char *path,
                        i32 size); // 0x22c
};

NUFILEDEVICE *NuFileGetDeviceFromPath(char *path);
void Unk006e25b0(i32 error); // empty in the release build

// GLOBAL: LEGOBATMAN 0x0099f6d4
extern NUFILEDEVICE *nudir_default_device;

// What NuFileReadDir returns (saga saveload reads it as raw bytes).
struct NUFILEDIRTIME_s {
  u8 second; // 0x0
  u8 minute; // 0x1
  u8 hour;   // 0x2
  u8 day;    // 0x3
  u8 month;  // 0x4
  u8 pad5;
  u16 year; // 0x6
};

struct NUFILEDIRENTRY_s {
  u32 attrib;              // 0x00, 1 read, 2 write, 4 exec, 8 directory
  u32 size;                // 0x04
  NUFILEDIRTIME_s created; // 0x08
  NUFILEDIRTIME_s written; // 0x10
  char name[MAX_PATH];     // 0x18
};

// FUNCTION: LEGOBATMAN 0x006e3cc0
i32 NuFileOpenDir(char *path) {
  char buf[256];
  NUFILEDEVICE *device = NuFileGetDeviceFromPath(path);
  if (device == NULL)
    device = nudir_default_device;
  device->format_name_fn(device, buf, path, 0x100);
  if (device->id == 2)
    strcat(buf, ";1");
  if (device->id == 1)
    return 0x1000;
  i32 dir = NuSysDirOpen(buf);
  if (dir < 0)
    return 0;
  return dir + 1;
}

// FUNCTION: LEGOBATMAN 0x006e3da0
void NuFileCloseDir(i32 file) {
  if (file < 0x1000)
    NuSysDirClose(file - 1);
}

// FUNCTION: LEGOBATMAN 0x006e3df0
i32 NuFileReadDir(i32 file, NUFILEDIRENTRY_s *out) {
  NUSYSDIRENTRY_s entry;
  i32 len = 0;
  if (file >= 0x1000)
    return 0;
  i32 result = NuSysDirRead(file - 1, &entry);
  if (result > 0) {
    out->attrib = ((((((entry.mode >> 2) & 0x800) | (entry.mode & 0x100)) >> 2 |
                     (entry.mode & 0x80)) >>
                        2 |
                    (entry.mode & 0x40)) >>
                   4);
    out->size = entry.size;
    out->created.second = entry.created.second;
    out->created.minute = entry.created.minute;
    out->created.hour = entry.created.hour;
    out->created.day = entry.created.day;
    out->created.month = entry.created.month;
    out->created.year = entry.created.year + (entry.created.zero << 8);
    out->written.second = entry.written.second;
    out->written.minute = entry.written.minute;
    out->written.hour = entry.written.hour;
    out->written.day = entry.written.day;
    out->written.month = entry.written.month;
    out->written.year = entry.written.year + (entry.written.zero << 8);
    strcpy(out->name, entry.name);
    len = strlen(out->name);
  } else if (result < 0) {
    Unk006e25b0(result);
  }
  return len;
}

// FUNCTION: LEGOBATMAN 0x006e3820
i32 NuFileCreateDir(char *path) {
  SECURITY_ATTRIBUTES sa;

  sa.bInheritHandle = TRUE;
  sa.nLength = sizeof(sa);
  sa.lpSecurityDescriptor = NULL;
  return CreateDirectoryA(path, &sa);
}

// FUNCTION: LEGOBATMAN 0x006e3850
i32 NuFileCreatePath(char *path) { return 0; }

// FUNCTION: LEGOBATMAN 0x006e3940
i32 NuFileDelete(char *path) { return DeleteFileA(path); }

// --- NuThread ---------------------------------------------------------------

// GLOBAL: LEGOBATMAN 0x00b058dc
volatile i32 g_nuCritSecCount;
// GLOBAL: LEGOBATMAN 0x00b039a0
CRITICAL_SECTION g_nuCritSecs[13];
// LockCount of a freshly initialised section.
// GLOBAL: LEGOBATMAN 0x0099f774
extern LONG g_nuCritSecUnlocked;

// FUNCTION: LEGOBATMAN 0x006e3950
i32 NuThreadCreateCriticalSection(void) {
  if (g_nuCritSecCount <= 12) {
    g_nuCritSecCount++;
    InitializeCriticalSection(&g_nuCritSecs[g_nuCritSecCount]);
    g_nuCritSecUnlocked = g_nuCritSecs[g_nuCritSecCount].LockCount;
    return g_nuCritSecCount;
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x006e39a0
void NuThreadDestroyCriticalSection(i32 cs) {}

// FUNCTION: LEGOBATMAN 0x006e39b0
void NuThreadCriticalSectionBegin(i32 cs) {
  if (cs >= 1 && cs <= 12)
    EnterCriticalSection(&g_nuCritSecs[cs]);
}

// FUNCTION: LEGOBATMAN 0x006e39d0
void NuThreadCriticalSectionEnd(i32 cs) {
  if (cs >= 1 && cs <= 12)
    LeaveCriticalSection(&g_nuCritSecs[cs]);
}

// Original computes setne into cl, then mov al, cl; ours sets al directly.
// STUB: LEGOBATMAN 0x006e39f0
u8 NuIsCriticalSectionLocked(i32 cs) {
  if (cs >= 1 && cs <= 12) {
    LONG lock = g_nuCritSecs[cs].LockCount;
    return lock == g_nuCritSecUnlocked ? 0 : 1;
  }
  return 1;
}

struct nuthread_s {
  HANDLE thread;
  HANDLE signal;
};

// GLOBAL: LEGOBATMAN 0x00b03be0
extern nuthread_s g_nuThreads[];

// FUNCTION: LEGOBATMAN 0x006e3ad0
void NuThreadSignalSend(i32 thread) {
  ReleaseSemaphore(g_nuThreads[thread].signal, 1, NULL);
}

// FUNCTION: LEGOBATMAN 0x006e3af0
void NuThreadSignalRecieve(i32 thread) {
  WaitForSingleObject(g_nuThreads[thread].signal, INFINITE);
}

// FUNCTION: LEGOBATMAN 0x006e3b10
void NuThreadSignalRecieveTimeout(i32 thread, i32 timeout) {
  WaitForSingleObject(g_nuThreads[thread].signal, timeout);
}

// --- NuPSFile ----------------------------------------------------------------

extern HANDLE g_nuPSFileHandles[64]; // nufile_ps_unk.cpp
// GLOBAL: LEGOBATMAN 0x0099f820
extern volatile i32 g_nuPSFileCritSec; // -1 until the first open

// getpointer (nufile_ps_unk.cpp 0x006e3220), inlined here: this file and
// that one are one translation unit in the original.
static inline i32 NuPSFileFreeSlot(void) {
  i32 i;
  for (i = 0; i < 64; i++) {
    if (g_nuPSFileHandles[i] == INVALID_HANDLE_VALUE)
      return i;
  }
  return -1;
}

// FUNCTION: LEGOBATMAN 0x006e3f60
extern "C" i32 NuPSFileOpen(char *path, i32 mode) {
  DWORD access = 0;
  DWORD disposition = 0;
  i32 fh;
  switch (mode) {
  case 0:
    access = GENERIC_READ;
    disposition = OPEN_EXISTING;
    break;
  case 1:
    access = GENERIC_WRITE;
    disposition = CREATE_ALWAYS;
    break;
  case 2:
    access = GENERIC_WRITE;
    disposition = OPEN_ALWAYS;
    break;
  }
  if (g_nuPSFileCritSec == -1)
    g_nuPSFileCritSec = NuThreadCreateCriticalSection();
  NuThreadCriticalSectionBegin(g_nuPSFileCritSec);
  fh = NuPSFileFreeSlot();
  g_nuPSFileHandles[fh] =
      CreateFileA(path, access, FILE_SHARE_READ, NULL, disposition, 0, NULL);
  NuThreadCriticalSectionEnd(g_nuPSFileCritSec);
  if (g_nuPSFileHandles[fh] == INVALID_HANDLE_VALUE)
    return -1;
  return fh;
}
