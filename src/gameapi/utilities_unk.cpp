// gameapi/utilities_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nu2api/nucore/common.h"
#include <stddef.h>

#include "../nu2api/numath/nuvec.h"

i16 temp_yrot;

i32 NuAtan2D(f32 dx, f32 dy);

i32 i_temp_xrot;

f32 NuFsqrt(f32 f);

i16 temp_xrot;

// name is a Mac pairing hint (order): verify
// from saga legoapi/misc/utilities.cpp
// FUNCTION: LEGOBATMAN 0x005ae640
void FindAnglesXY(nuvec_s *direction, u16 *x_rotation, u16 *y_rotation) {
  temp_yrot = NuAtan2D(direction->x, direction->z);
  if (y_rotation != NULL)
    *y_rotation = temp_yrot;
  i_temp_xrot = -NuAtan2D(direction->y, NuFsqrt(direction->x * direction->x +
                                                direction->z * direction->z));
  temp_xrot = i_temp_xrot;
  if (x_rotation != NULL)
    *x_rotation = temp_xrot;
}

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
