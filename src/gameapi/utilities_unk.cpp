// gameapi/utilities_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nu2api/nucore/common.h"
#include <stddef.h>

#include "../nu2api/numath/nuvec.h"

i16 temp_yrot;

i32 NuAtan2D(f32 dx, f32 dy);

i32 i_temp_xrot;

f32 NuFsqrt(f32 f);

i16 temp_xrot;

void NuVecRotateX(NUVEC *v, NUVEC *v0, i32 a);

// GLOBAL: LEGOBATMAN 0x00aa055c
i16 temp_zrot;

// FUNCTION: LEGOBATMAN 0x005ae5c0
void FindAnglesZX(nuvec_s *normal, u16 *x_rotation, u16 *z_rotation) {
  u16 x_angle = NuAtan2D(normal->z, normal->y);
  if (x_rotation != 0) {
    *x_rotation = x_angle;
  }
  temp_xrot = x_angle;

  NUVEC rotated;
  NuVecRotateX(&rotated, normal, -(i32)x_angle);
  u16 z_angle = NuAtan2D(rotated.x, rotated.y);
  if (z_rotation != 0) {
    *z_rotation = -z_angle;
  }
  temp_zrot = -z_angle;
}

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

void NuVecRotateY(NUVEC *v, NUVEC *v0, i32 a);

// FUNCTION: LEGOBATMAN 0x005ae6c0
void GetRotationAngles(nuvec_s *direction, u16 *x_rotation, u16 *y_rotation) {
  NUVEC rotated;
  NUVEC copy = *direction;
  u16 y_angle = -NuAtan2D(copy.z, copy.x);
  NuVecRotateY(&rotated, &copy, -static_cast<i32>(static_cast<u16>(y_angle)));
  *x_rotation = -NuAtan2D(rotated.x, rotated.y);
  *y_rotation = y_angle;
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

// name is a Mac pairing hint (order): verify
// from saga legoapi/misc/utilities.cpp
// STUB: LEGOBATMAN 0x005b00d0
// skipped: x87 scheduling differs (orig keeps deltas on the stack, spills dot
// products); not attempted.
bool LineIntersectCircle(NUVEC *origin, NUVEC *direction, NUVEC *center,
                         f32 radius_squared) {
  f32 x = center->x - origin->x;
  f32 z = center->z - origin->z;
  f32 projection = direction->x * x + direction->z * z;
  if (projection >= 0.0f)
    return x * x + z * z - projection * projection <= radius_squared;
  return false;
}
