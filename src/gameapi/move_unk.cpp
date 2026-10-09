// gameapi/move_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nu2api/nucore/common.h"
#include <stddef.h>

// name is a Mac pairing hint (gapfill): verify
// from saga legoapi/characters/motion/move.cpp
// FUNCTION: LEGOBATMAN 0x005ae1d0
float SeekLinearF(float current, float target, float step) {
  if (current > target) {
    current -= step;
    if (current < target) {
      current = target;
    }
  } else if (current < target) {
    current += step;
    if (current > target) {
      current = target;
    }
  }
  return current;
}
