// nu2api/nucore/nuerror.cpp (saga): 0x006e2620..0x006e2aa0. The *Print
// functions are inlined into the *Function statics, so they share a TU. Mac
// order: NuErrorPrint, NuSevereWarningPrint, NuWarningPrint, NuDebugMsgPrint,
// (nuonline_pc), NuSet*MsgHandler, NuErrorProlog, NuSevereWarningProlog,
// NuWarningProlog, NuDebugMsgProlog, NuDebugMsgPrologTTY.

#include "common.h"
#include <stdarg.h>
#include <stdio.h>

class CNuConsole {
public:
  void SendError(char *fmt, ...);
  void SendDebugText(char *fmt, ...);
  void SendWarning(char *fmt, ...);
};

extern CNuConsole g_nuConsole;

void NuStrCat(char *dst, const char *src);

typedef void (*NuErrorFunctionPtr)(char *, ...);
typedef void (*NuDebugTTYFunctionPtr)(i32, char *, ...);

// GLOBAL: LEGOBATMAN 0x00b05470
static char *nufile;
// GLOBAL: LEGOBATMAN 0x00b05474
static i32 nuline;
// GLOBAL: LEGOBATMAN 0x00b05070
static char txt[1024];
// GLOBAL: LEGOBATMAN 0x00b054a0
static char captxt[1024];
// GLOBAL: LEGOBATMAN 0x00b05490
void (*nuerror_handler)(char *);
// GLOBAL: LEGOBATMAN 0x00b03c6c
void (*nuwarning_handler)(char *);
// GLOBAL: LEGOBATMAN 0x00b03c64
void (*nusevere_warning_handler)(char *);
// GLOBAL: LEGOBATMAN 0x00b03c60
void (*nudebug_handler)(char *);
// GLOBAL: LEGOBATMAN 0x00b058b8
extern i32 DisableDebugMsg;

// FUNCTION: LEGOBATMAN 0x006e2620
void NuErrorPrint(char *msg) { g_nuConsole.SendError("%s", msg); }

// FUNCTION: LEGOBATMAN 0x006e2640
void NuSevereWarningPrint(char *msg) { g_nuConsole.SendError("%s", msg); }

// FUNCTION: LEGOBATMAN 0x006e2660
void NuWarningPrint(char *msg) { g_nuConsole.SendWarning("%s", msg); }

// FUNCTION: LEGOBATMAN 0x006e2680
void NuDebugMsgPrint(char *msg) { g_nuConsole.SendDebugText("%s", msg); }

// FUNCTION: LEGOBATMAN 0x006e2700
void NuSetErrorMsgHandler(void (*handler)(char *)) {
  nuerror_handler = handler;
}

// FUNCTION: LEGOBATMAN 0x006e2710
void NuSetWarningMsgHandler(void (*handler)(char *)) {
  nuwarning_handler = handler;
}

// FUNCTION: LEGOBATMAN 0x006e2720
void NuSetSevereWarningMsgHandler(void (*handler)(char *)) {
  nusevere_warning_handler = handler;
}

// FUNCTION: LEGOBATMAN 0x006e2730
void NuSetDebugMsgHandler(void (*handler)(char *)) {
  nudebug_handler = handler;
}

// FUNCTION: LEGOBATMAN 0x006e2740
static void NuErrorFunction(char *format, ...) {
  va_list args;

  sprintf(captxt, "NuError - %s(%d) : ", nufile, nuline);
  va_start(args, format);
  vsprintf(txt, format, args);
  va_end(args);
  NuStrCat(captxt, txt);
  NuStrCat(captxt, "\n");
  if (nuerror_handler != 0) {
    nuerror_handler(captxt);
  }
  NuErrorPrint(captxt);
}

// FUNCTION: LEGOBATMAN 0x006e27c0
NuErrorFunctionPtr NuErrorProlog(char *file, i32 line) {
  nufile = file;
  nuline = line;
  return NuErrorFunction;
}

// FUNCTION: LEGOBATMAN 0x006e27e0
static void NuSevereWarningFunction(char *format, ...) {
  va_list args;

  sprintf(captxt, "NuSevereWarning - %s(%d) : ", nufile, nuline);
  va_start(args, format);
  vsprintf(txt, format, args);
  va_end(args);
  NuStrCat(captxt, txt);
  NuStrCat(captxt, "\n");
  if (nusevere_warning_handler != 0) {
    nusevere_warning_handler(captxt);
  }
  NuSevereWarningPrint(captxt);
}

// FUNCTION: LEGOBATMAN 0x006e2860
NuErrorFunctionPtr NuSevereWarningProlog(char *file, i32 line) {
  nufile = file;
  nuline = line;
  return NuSevereWarningFunction;
}

// FUNCTION: LEGOBATMAN 0x006e2880
static void NuWarningFunction(char *format, ...) {
  va_list args;

  if (!DisableDebugMsg) {
    sprintf(captxt, "NuWarning - %s(%d) : ", nufile, nuline);
    va_start(args, format);
    vsprintf(txt, format, args);
    va_end(args);
    NuStrCat(captxt, txt);
    NuStrCat(captxt, "\n");
    if (nuwarning_handler != 0) {
      nuwarning_handler(captxt);
    }
    NuWarningPrint(captxt);
  }
}

// FUNCTION: LEGOBATMAN 0x006e2910
NuErrorFunctionPtr NuWarningProlog(char *file, i32 line) {
  nufile = file;
  nuline = line;
  return NuWarningFunction;
}

// FUNCTION: LEGOBATMAN 0x006e2930
static void NuDebugMsgFunction(char *format, ...) {
  char buf[1024];
  va_list args;

  if (!DisableDebugMsg) {
    sprintf(buf, "NuDebugMsg - %s(%d) :", nufile, nuline);
    va_start(args, format);
    vsprintf(txt, format, args);
    va_end(args);
    NuStrCat(buf, txt);
    NuStrCat(buf, "\n");
    if (nudebug_handler != 0) {
      nudebug_handler(buf);
    }
    NuDebugMsgPrint(buf);
  }
}

// FUNCTION: LEGOBATMAN 0x006e29f0
static void NuDebugMsgFunctionTTY(i32 channel, char *format, ...) {
  va_list args;

  if (!DisableDebugMsg) {
    sprintf(captxt, "NuDebugMsg - %s(%d) :", nufile, nuline);
    va_start(args, format);
    vsprintf(txt, format, args);
    va_end(args);
    NuStrCat(txt, "\n");
    NuDebugMsgPrint(captxt);
    NuDebugMsgPrint(txt);
  }
}

// FUNCTION: LEGOBATMAN 0x006e2a70
NuErrorFunctionPtr NuDebugMsgProlog(char *file, i32 line) {
  nufile = file;
  nuline = line;
  return NuDebugMsgFunction;
}

// FUNCTION: LEGOBATMAN 0x006e2a90
NuDebugTTYFunctionPtr NuDebugMsgPrologTTY(char *file, i32 line) {
  nufile = file;
  nuline = line;
  return NuDebugMsgFunctionTTY;
}
