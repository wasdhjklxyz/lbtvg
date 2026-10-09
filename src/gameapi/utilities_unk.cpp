// gameapi/utilities_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nu2api/nucore/common.h"
#include <stddef.h>

// from saga legoapi/misc/utilities.cpp
// FUNCTION: LEGOBATMAN 0x005ae740
i32 RotDiff(u16 current, u16 target) {
  i32 difference = static_cast<u32>(target) - static_cast<u32>(current);
  if (difference > 0x8000) {
    difference -= 0x10000;
  } else if (difference < -0x8000) {
    difference += 0x10000;
  }
  return difference;
}
