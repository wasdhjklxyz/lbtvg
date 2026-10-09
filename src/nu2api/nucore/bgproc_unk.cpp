// nu2api/nucore/bgproc_unk.cpp: background process list, between
// nufile_gen.cpp (0x006e0830) and nufile_pc.cpp (0x006e3430).

struct nulst_s;
struct nulstnode_s;
nulstnode_s *NuLstGetNext(nulst_s *list, nulstnode_s *node);

// GLOBAL: LEGOBATMAN 0x00b03c68
nulst_s *bgProcList;
// GLOBAL: LEGOBATMAN 0x00b058c0
nulstnode_s *bgProcActive;

// FUNCTION: LEGOBATMAN 0x006e2af0
nulstnode_s *bgGetProcActive(void) {
  if (bgProcActive)
    return bgProcActive;
  return NuLstGetNext(bgProcList, bgProcActive);
}

#include <windows.h>

nulst_s *NuLstCreate(int element_count, int element_size);
DWORD WINAPI bgThreadMain(LPVOID param);

// GLOBAL: LEGOBATMAN 0x00b05930
extern int bgProcInitialised;
// GLOBAL: LEGOBATMAN 0x00b058a0
extern CRITICAL_SECTION g_bgCritSec;
// GLOBAL: LEGOBATMAN 0x00b03bd8
extern HANDLE g_bgWorkToDoEvent;
// GLOBAL: LEGOBATMAN 0x00b03bdc
extern HANDLE g_bgFreezeEvent;
// GLOBAL: LEGOBATMAN 0x00b058c4
extern HANDLE g_bgProcThread;
// GLOBAL: LEGOBATMAN 0x00b058c8
extern DWORD g_bgProcThreadId;

// FUNCTION: LEGOBATMAN 0x006e2e40
void bgProcInit(void) {
  if (!bgProcInitialised) {
    bgProcInitialised = 1;
    bgProcList = NuLstCreate(0x10, 0x218);
    InitializeCriticalSection(&g_bgCritSec);
    g_bgWorkToDoEvent = CreateEventA(0, 0, 0, "BGWorkToDo");
    g_bgFreezeEvent = CreateEventA(0, 1, 1, "BGFreeze");
    bgProcActive = 0;
    g_bgProcThread = CreateThread(0, 0x10000, bgThreadMain, 0, CREATE_SUSPENDED,
                                  &g_bgProcThreadId);
    SetThreadPriority(g_bgProcThread, THREAD_PRIORITY_BELOW_NORMAL);
    ResumeThread(g_bgProcThread);
  }
}
