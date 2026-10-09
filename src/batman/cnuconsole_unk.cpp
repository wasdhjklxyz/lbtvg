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

  u8 pad_000[0x500];
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
