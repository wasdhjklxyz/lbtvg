// nu2api/nucore/pc/nufile_pc.cpp: __FILE__ anchor at 0x006e3430 (see
// docs/linkmap.md). Mac order: NuSysDirOpen, NuFileOpenDir, NuSysDirClose,
// NuFileCloseDir, NuSysDirRead, NuFileReadDir, NuFileCreateDir,
// NuFileCreatePath, NuFileGetInfo, NuFileDelete, then the NuThread layer.

#include "../common.h"
#include <windows.h>

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
