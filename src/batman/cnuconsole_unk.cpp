// batman/cnuconsole_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nu2api/nucore/common.h"
#include <signal.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

class CNuConsole {
public:
  void SendError(char *fmt, ...);
  void SendDebugText(char *fmt, ...);
  void ExitNow(char *fmt, ...);
  void SendWarning(char *fmt, ...);
  void SendD3DWarning(long hr, char *fmt, ...);
  void ResetFontUnk0052e580();
  void Unk0052e8e0();
  void Initialize();

  u8 active;        // 0x000
  u8 debug_enabled; // 0x001
  u8 pad_002[0x10 - 2];
  i32 i10;         // 0x010
  i32 i14;         // 0x014
  i32 i18;         // 0x018
  i32 font_size;   // 0x01c
  char font[0x80]; // 0x020
  u32 colours[5];  // 0x0a0
  i32 *pb4;        // 0x0b4
  f32 fb8;         // 0x0b8
  f32 fbc;         // 0x0bc
  f32 fc0;         // 0x0c0
  f32 fc4;         // 0x0c4
  u8 pad_0c8[0x500 - 0xc8];
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

// FUNCTION: LEGOBATMAN 0x0052e580
void CNuConsole::ResetFontUnk0052e580() {
  i14 = -1;
  i10 = -1;
  i18 = -1;
  font_size = 12;
  strcat(font, "Arial");
  colours[0] = 0xff;
  colours[1] = 0xff4c5844;
  colours[2] = 0xff889180;
  colours[3] = 0xff2d3128;
  colours[4] = 0xff3e4637;
}

void *Unk006dd970(char *name, i32 mode);

// FUNCTION: LEGOBATMAN 0x0052fd10
void CNuConsole::Initialize() {
  char path[0x400];
  fb8 = 100.0f;
  active = 1;
  fbc = 100.0f;
  fc0 = 600.0f;
  fc4 = 500.0f;
  ResetFontUnk0052e580();
  pb4 = &i10;
  Unk0052e8e0();
  if (output_enabled) {
    sprintf(path, "Log.txt");
    output = Unk006dd970(path, 1);
    if (output != NULL)
      FUN_006d3520(output, "-- Log Started --\n\n");
  }
}
