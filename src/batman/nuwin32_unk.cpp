// batman/nuwin32_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nu2api/nucore/common.h"
#include <stddef.h>
#include <windows.h>

// needs windows.h first
#include <shellapi.h>

// Swaps the global at 0x00b03884, returning the old one.
extern "C" void *SetUnk00b03884(void *value);

// GLOBAL: LEGOBATMAN 0x009d10cc
extern i32 g_nuWin32Unk009d10cc;
// GLOBAL: LEGOBATMAN 0x009d109c
extern void *g_nuWin32Unk009d109c[];

// FUNCTION: LEGOBATMAN 0x00526230
extern "C" void NuWin32SetDFS(i32 index) {
  if (g_nuWin32Unk009d10cc > 0 && g_nuWin32Unk009d109c[0] != NULL)
    SetUnk00b03884(g_nuWin32Unk009d109c[index]);
}

// GLOBAL: LEGOBATMAN 0x009d1354
extern i32 g_nuWin32Unk009d1354; // launched from Media Center

// GLOBAL: LEGOBATMAN 0x02a1a000
extern HWND g_nuPCWindow;
// GLOBAL: LEGOBATMAN 0x00ad31c4
extern HWND g_hwnd_0ad31c4;

// FUNCTION: LEGOBATMAN 0x00527010
int NuPCCreateWindowFromHWnd(HWND hwnd, int show) {
  HANDLE mutex = CreateMutexA(NULL, TRUE,
                              "TTales-{BB049982-BDFA-42a0-8584-3917255CBA0E} ");
  bool already_running = GetLastError() == ERROR_ALREADY_EXISTS;
  if (mutex)
    ReleaseMutex(mutex);
  if (already_running) {
    HWND other = g_nuPCWindow;
    if (other) {
      SetForegroundWindow(other);
      if (IsIconic(other))
        ShowWindow(other, SW_RESTORE);
    }
    CoUninitialize();
    return 0;
  }
  g_nuPCWindow = hwnd;
  ShowWindow(hwnd, show);
  UpdateWindow(hwnd);
  g_hwnd_0ad31c4 = hwnd;
  return 1;
}

// FUNCTION: LEGOBATMAN 0x005270a0
void RelaunchMediaCenterIfNecessary() {
  char path[MAX_PATH];
  if (GetSystemMetrics(SM_MEDIACENTER) && g_nuWin32Unk009d1354 &&
      ExpandEnvironmentStringsA("%SystemRoot%\\ehome\\ehshell.exe", path,
                                MAX_PATH) &&
      GetFileAttributesA(path) != INVALID_FILE_ATTRIBUTES)
    ShellExecuteA(NULL, "open", path, NULL, NULL, SW_SHOWNORMAL);
}
