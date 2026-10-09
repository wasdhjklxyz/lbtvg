// gameapi/move_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nu2api/nucore/common.h"
#include <stddef.h>

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

// GLOBAL: LEGOBATMAN 0x00a95fe0
extern f32 FRAMETIME;

// FUNCTION: LEGOBATMAN 0x005ae240
f32 SeekValF(f32 current, f32 target, f32 rate) {
  rate = FRAMETIME * rate;
  if (rate > 1.0f)
    rate = 1.0f;
  rate = current + (target - current) * rate;
  return rate;
}
