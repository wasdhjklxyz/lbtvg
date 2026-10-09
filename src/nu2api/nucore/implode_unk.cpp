// nu2api/nucore/implode_unk.cpp: placed by tools/new.py; file name unproven.

#include "common.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

// FUNCTION: LEGOBATMAN 0x006d6910
// from saga nu2api/nucore/implode.cpp
void ImplodeError(char *msg, ...) {
  va_list args;
  va_start(args, msg);

  putc('\n', stderr);
  vfprintf(stderr, msg, args);
  putc('\n', stderr);

  exit(1);
}
