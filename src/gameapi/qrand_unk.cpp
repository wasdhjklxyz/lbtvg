// gameapi/qrand_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nu2api/nucore/common.h"
#include <stddef.h>

i32 qseed = 0x3039;

// from saga legoapi/core/input/qrand.cpp
// FUNCTION: LEGOBATMAN 0x005ae160
i32 qrand(void) {
  qseed = qseed * 0x24cd + 1 & 0xffff;

  return qseed;
}
