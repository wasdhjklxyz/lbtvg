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
static DWORD WINAPI bgThreadMain(LPVOID param);

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

struct BGPROCREQ {
  void *fn;     // 0x000
  void *param;  // 0x004
  int done : 1; // 0x008
  int abort : 1;
  unsigned int pad00c;
  char data[0x200];     // 0x010
  struct bgvar_s *vars; // 0x210
  int var_count;        // 0x214
};

// saga BGVAR / BGPROCARGTYPE (1 i32, 2 float, 3 string, 4 u32, 5 pointer,
// 6 bool, 7 end).
struct bgvar_s {
  int type;
  union {
    int i32_value;
    unsigned int u32_value;
    float float_value;
    void *pointer_value;
    char *string_value;
  } value;
};

nulstnode_s *NuLstAllocTail(nulst_s *list);

int NuStrLen(const char *s);
int NuStrCpy(char *dst, const char *src);

// FUNCTION: LEGOBATMAN 0x006e2b10
BGPROCREQ *bgPostRequestV(void *fn, void *param, int first_type, ...) {
  BGPROCREQ *req;

  EnterCriticalSection(&g_bgCritSec);
  req = (BGPROCREQ *)NuLstAllocTail(bgProcList);
  if (req != 0) {
    req->fn = fn;
    req->param = param;
    req->done = 0;
    req->abort = 0;
    req->vars = (bgvar_s *)req->data;
    req->var_count = 0;

    bgvar_s *var = req->vars;
    char *string_end = (char *)&req->vars;
    va_list args;
    va_start(args, first_type);

    while (first_type != 7) {
      var->type = first_type;
      switch (first_type) {
      case 1:
        var->value.i32_value = va_arg(args, int);
        break;
      case 2:
        var->value.float_value = va_arg(args, float);
        break;
      case 3: {
        char *string = va_arg(args, char *);
        string_end -= NuStrLen(string) + 1;
        var->value.string_value = string_end;
        NuStrCpy(string_end, string);
        break;
      }
      case 4:
        var->value.u32_value = va_arg(args, unsigned int);
        break;
      case 5:
        var->value.pointer_value = va_arg(args, void *);
        break;
      case 6:
        var->value.i32_value = va_arg(args, int);
        break;
      }
      req->var_count++;
      var++;
      first_type = va_arg(args, int);
    }
    va_end(args);
  }
  LeaveCriticalSection(&g_bgCritSec);
  if (req != 0)
    SetEvent(g_bgWorkToDoEvent);
  return req;
}

// FUNCTION: LEGOBATMAN 0x006e2c30
BGPROCREQ *bgPostRequest(void *fn, void *param, void *data, int size) {
  BGPROCREQ *req;

  EnterCriticalSection(&g_bgCritSec);
  req = (BGPROCREQ *)NuLstAllocTail(bgProcList);
  if (req != 0) {
    req->fn = fn;
    req->param = param;
    req->done = 0;
    req->abort = 0;
    req->vars = 0;
    req->var_count = 0;
    if (data != 0 && size != 0)
      memcpy(req->data, data, size);
  }
  LeaveCriticalSection(&g_bgCritSec);
  if (req != 0)
    SetEvent(g_bgWorkToDoEvent);
  return req;
}

void NuLstFree(nulstnode_s *node);
void NuFmvCriticalSection(int unk, int enter);

#define BGACTIVE ((BGPROCREQ *)bgProcActive)
typedef void bgprocfn(BGPROCREQ *req);

// GLOBAL: LEGOBATMAN 0x00b058cc
extern volatile int g_bgFrozen;

// STUB: LEGOBATMAN 0x006e2cc0
// The original reads bgProcActive twice for `->done = 1` (load/or/store)
// yet keeps it for the fn test; volatile pointee gets the first, not both.
static DWORD WINAPI bgThreadMain(LPVOID param) {
  WaitForSingleObject(g_bgWorkToDoEvent, INFINITE);
  EnterCriticalSection(&g_bgCritSec);
  bgProcActive = NuLstGetNext(bgProcList, 0);
  LeaveCriticalSection(&g_bgCritSec);
  for (;;) {
    if (WaitForSingleObject(g_bgFreezeEvent, 0) == WAIT_TIMEOUT) {
      g_bgFrozen = 1;
      WaitForSingleObject(g_bgFreezeEvent, INFINITE);
      g_bgFrozen = 0;
    }
    if (bgProcActive) {
      NuFmvCriticalSection(0, 1);
      ((volatile BGPROCREQ *)bgProcActive)->done = 1;
      if (BGACTIVE->fn)
        ((bgprocfn *)BGACTIVE->fn)(BGACTIVE);
      if (BGACTIVE->param)
        ((bgprocfn *)BGACTIVE->param)(BGACTIVE);
      NuFmvCriticalSection(0, 0);
    }
    EnterCriticalSection(&g_bgCritSec);
    if (bgProcActive)
      NuLstFree(bgProcActive);
    bgProcActive = NuLstGetNext(bgProcList, 0);
    LeaveCriticalSection(&g_bgCritSec);
    while (!bgProcActive) {
      if (WaitForSingleObject(g_bgFreezeEvent, 0) == WAIT_TIMEOUT) {
        g_bgFrozen = 1;
        WaitForSingleObject(g_bgFreezeEvent, INFINITE);
        g_bgFrozen = 0;
      }
      WaitForSingleObject(g_bgWorkToDoEvent, INFINITE);
      EnterCriticalSection(&g_bgCritSec);
      bgProcActive = NuLstGetNext(bgProcList, 0);
      LeaveCriticalSection(&g_bgCritSec);
    }
  }
}

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

// Mac order after bgThreadMain: bgProcAbortAll, bgprocFreeze, bgprocUnFreeze,
// bgprocIsFrozen, bgprocIsFreezing, bgProcIsBgThread, bgSuspendMain,
// bgProcClose (then NuFileInitEarly, in nufile_ps_unk.cpp).

// GLOBAL: LEGOBATMAN 0x00b058cc
volatile int g_bgFrozen;
// GLOBAL: LEGOBATMAN 0x00b058d0
volatile int g_bgFreezing;

// STUB: LEGOBATMAN 0x006e2ef0
// Only diff: the original pushes edi at entry, ours inside the loop.
void bgProcAbortAll(void) {
  BGPROCREQ *req;
  BGPROCREQ *next;

  req = (BGPROCREQ *)NuLstGetNext(bgProcList, bgProcActive);
  while (req) {
    next = (BGPROCREQ *)NuLstGetNext(bgProcList, (nulstnode_s *)req);
    req->abort = 1;
    if (req->param)
      ((bgprocfn *)req->param)(req);
    NuLstFree((nulstnode_s *)req);
    req = next;
  }
  if (bgProcActive)
    ((volatile BGPROCREQ *)bgProcActive)->abort = 1;
}

// FUNCTION: LEGOBATMAN 0x006e2f70
void bgprocFreeze(void) {
  g_bgFreezing = 1;
  ResetEvent(g_bgFreezeEvent);
  SetEvent(g_bgWorkToDoEvent);
}

// FUNCTION: LEGOBATMAN 0x006e2fa0
void bgprocUnFreeze(void) {
  g_bgFreezing = 0;
  SetEvent(g_bgFreezeEvent);
}

// FUNCTION: LEGOBATMAN 0x006e2fc0
int bgprocIsFrozen(void) { return g_bgFrozen != 0; }

// FUNCTION: LEGOBATMAN 0x006e2fd0
int bgprocIsFreezing(void) {
  if (g_bgFrozen || g_bgFreezing)
    return 1;
  return 0;
}

// FUNCTION: LEGOBATMAN 0x006e2ff0
int bgProcIsBgThread(void) {
  int result = 0;
  if (GetCurrentThreadId() == g_bgProcThreadId)
    result = 1;
  return result;
}

// FUNCTION: LEGOBATMAN 0x006e3010
void bgSuspendMain(void) {
  if (bgProcList && (bgProcActive || NuLstGetNext(bgProcList, 0)))
    Sleep(1);
}

// FUNCTION: LEGOBATMAN 0x006e3040
void bgProcClose(void) {}
