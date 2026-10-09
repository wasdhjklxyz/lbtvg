// batman/cnuconsole_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nu2api/nucore/common.h"
#include <signal.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

class CNuConsole {
public:
  void SendError(char *fmt, ...);
  void SendDebugText(char *fmt, ...);
  void ExitNow(char *fmt, ...);
  void SendWarning(char *fmt, ...);
  void SendD3DWarning(long hr, char *fmt, ...);

  u8 pad_000;
  u8 debug_enabled; // 0x001
  u8 pad_002[0x500 - 2];
  void *output;      // 0x500
  u8 output_enabled; // 0x504
};

// GLOBAL: LEGOBATMAN 0x00ad31c4
extern HWND g_hwnd_0ad31c4;
// GLOBAL: LEGOBATMAN 0x009d5970
static char g_error_msg[0x800];
// GLOBAL: LEGOBATMAN 0x009d6170
static char g_error_text[0x800];

void FUN_0052e500(char *msg);
void FUN_006d3520(void *output, char *msg);
extern "C" const char *WINAPI DXGetErrorString9A(HRESULT hr);
extern "C" const char *WINAPI DXGetErrorDescription9A(HRESULT hr);

class CD3DCore {
public:
  i32 FindNearestMode(unsigned int width, unsigned int height,
                      unsigned int depth) const;
  void SetNextDisplayMode(unsigned int width, unsigned int height,
                          unsigned int depth);

  u8 pad0[0x618];
  i32 mode_count; // 0x618
  u8 pad61c[4];
  i32 current_mode; // 0x620
  u8 pad624[0x668 - 0x624];
  i32 next_mode; // 0x668
};

// FUNCTION: LEGOBATMAN 0x0052c1f0
void CD3DCore::SetNextDisplayMode(unsigned int width, unsigned int height,
                                  unsigned int depth) {
  i32 mode = FindNearestMode(width, height, depth);
  if (mode >= mode_count)
    mode = current_mode;
  next_mode = mode;
}

// FUNCTION: LEGOBATMAN 0x0052e6f0
void CNuConsole::SendError(char *fmt, ...) {
  if (fmt != NULL) {
    va_list args;
    va_start(args, fmt);
    vsprintf(g_error_text, fmt, args);
  }

  sprintf(g_error_msg,
          "Important. Do not ignore!\n\n%s\n\n(Your application has generated "
          "an error message)\n\n",
          g_error_text);
  FUN_0052e500(g_error_msg);

  switch (MessageBoxA(g_hwnd_0ad31c4, g_error_msg, "Nu2Api Runtime Error!",
                      MB_ICONERROR | MB_ABORTRETRYIGNORE)) {
  case IDABORT:
    raise(SIGABRT);
    _exit(3);
  case IDRETRY:
    DebugBreak();
  }

  sprintf(g_error_msg, "NUERROR: %s\r\n", g_error_text);
  OutputDebugStringA(g_error_msg);

  if (output_enabled && output != NULL) {
    FUN_006d3520(output, g_error_msg);
  }
}

// FUNCTION: LEGOBATMAN 0x0052e5f0
void CNuConsole::SendDebugText(char *fmt, ...) {
  if (fmt != NULL) {
    va_list args;
    va_start(args, fmt);
    vsprintf(g_error_text, fmt, args);
  }
  sprintf(g_error_msg, "NUDEBUG: %s", g_error_text);
  if (debug_enabled)
    OutputDebugStringA(g_error_msg);
}

// FUNCTION: LEGOBATMAN 0x0052e640
void CNuConsole::ExitNow(char *fmt, ...) {
  if (fmt != NULL) {
    va_list args;
    va_start(args, fmt);
    vsprintf(g_error_text, fmt, args);
  }
  sprintf(g_error_msg, "\n%s\n", g_error_text);
  MessageBoxA(g_hwnd_0ad31c4, g_error_msg, "Fatal Error!", MB_ICONERROR);
  raise(SIGABRT);
  _exit(3);
}

// FUNCTION: LEGOBATMAN 0x0052e7b0
void CNuConsole::SendWarning(char *fmt, ...) {
  if (fmt != NULL) {
    va_list args;
    va_start(args, fmt);
    vsprintf(g_error_text, fmt, args);
  }
  sprintf(g_error_msg, "NUWARNING: %s\r\n", g_error_text);
  OutputDebugStringA(g_error_msg);
  if (output_enabled && output != NULL)
    FUN_006d3520(output, g_error_msg);
}

// FUNCTION: LEGOBATMAN 0x0052e860
void CNuConsole::SendD3DWarning(long hr, char *fmt, ...) {
  if (fmt != NULL) {
    va_list args;
    va_start(args, fmt);
    vsprintf(g_error_text, fmt, args);
  }
  sprintf(g_error_msg, "NUD3DWARNING: %s, DirectDescription: %s, %s\r\n",
          g_error_text, DXGetErrorString9A(hr), DXGetErrorDescription9A(hr));
  OutputDebugStringA(g_error_msg);
  if (output_enabled && output != NULL)
    FUN_006d3520(output, g_error_msg);
}
