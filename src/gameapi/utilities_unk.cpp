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

// name is a Mac pairing hint (gapfill): verify
// from saga legoapi/misc/utilities.cpp
// FUNCTION: LEGOBATMAN 0x005b0000
i32 LineIntersectSphere(NUVEC *origin, NUVEC *direction, NUVEC *center,
                        f32 radius_squared, f32 *distance_squared) {
  NUVEC v;
  v.x = center->x - origin->x;
  v.y = center->y - origin->y;
  v.z = center->z - origin->z;
  f32 projection = direction->x * v.x + direction->y * v.y + direction->z * v.z;
  if (projection < 0.0f)
    return 0;
  f32 distance = v.x * v.x + v.y * v.y + v.z * v.z;
  projection *= projection;
  distance -= projection;
  if (distance <= radius_squared) {
    if (distance_squared != NULL)
      *distance_squared = distance;
    return 1;
  }
  return 0;
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

// STUB: LEGOBATMAN 0x005b0e00
// heavy x87 (axis-angle rotation with double temporaries); not attempted
#if 0
f32 NuVecNorm(NUVEC *v, NUVEC *v0);

f32 NuTrigTable[NUTRIGTABLE_COUNT];

// from saga legoapi/misc/utilities.cpp
void VecRotateAxis(nuvec_s *vector, u16 angle, nuvec_s *axis) {
    NuVecNorm(axis, axis);
    const f32 cosine = NuTrigTable[((static_cast<u32>(angle) + 0x4000) >> 1) & 0x7fff];
    const f32 sine = NuTrigTable[angle >> 1];
    const f32 complement = 1.0f - cosine;
    const f32 x = axis->x;
    const f32 y = axis->y;
    const f32 z = axis->z;
    const NUVEC source = *vector;
    const f32 tx = complement * x;
    const f32 ty = complement * y;
    const f32 tz = complement * z;
    const f32 xy = tx * y;
    const f32 xz = tx * z;
    const f32 yz = ty * z;
    const f32 sx = sine * x;
    const f32 sy = sine * y;
    const f32 sz = z * sine;
    vector->x = ((tx * x + cosine) * source.x + 0.0f) + (xy - sz) * source.y + (xz + sy) * source.z;
    vector->y = ((xy + sz) * source.x + 0.0f) + (y * ty + cosine) * source.y + (yz - sx) * source.z;
    vector->z = ((xz - sy) * source.x + 0.0f) + (sx + yz) * source.y + (tz * z + cosine) * source.z;
}
#endif
