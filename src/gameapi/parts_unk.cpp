// gameapi/parts_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nu2api/nucore/common.h"
#include <stddef.h>

#include "../nu2api/numath/nuvec.h"

void AddFiniteShotDebrisEffect2(i32 *handle, i32 effect, NUVEC *position,
                                NUVEC *emitter_momentum,
                                NUVEC *particle_momentum, i32 count);

// from saga legoapi/render/fx/parts.cpp
// FUNCTION: LEGOBATMAN 0x005834f0
void AddFiniteShotDebrisEffect(i32 *handle, i32 effect, NUVEC *position,
                               i32 count) {
  AddFiniteShotDebrisEffect2(handle, effect, position, NULL, NULL, count);
}

i32 CreateScaledEffect(i32 effect_index, f32 requested_scale);

// from saga legoapi/render/fx/parts.cpp
// FUNCTION: LEGOBATMAN 0x00583520
void AddScaledFiniteShotDebrisEffect(i32 *key, i32 effect, NUVEC *position,
                                     NUVEC *orientation, NUVEC *momentum,
                                     i32 count, f32 scale) {
  i32 scaled = CreateScaledEffect(effect, scale);
  if (scaled != -1) {
    AddFiniteShotDebrisEffect2(key, scaled, position, orientation, momentum,
                               count);
  }
}
