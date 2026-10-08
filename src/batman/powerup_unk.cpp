// batman/, file unknown: power-up pickups (0x0048d650).

#include "../gameapi/gameobject_unk.h"
#include "../gameapi/sfx_unk.h"

// GLOBAL: LEGOBATMAN 0x009652cc
extern f32 AreaPickupScale;

// STUB: LEGOBATMAN 0x0048d650
// x87 compare spelling differs (original tests hi<=mag / lo<=mag with
// test ah,0x41; jp and keeps hi/lo on the stack)
void PowerUp_ImpactPart(PART_s *part) {
  f32 lo = AreaPickupScale * 0.1f;
  f32 hi = AreaPickupScale * 2.5f;
  f32 mag = NuVecMag(&part->v80);
  f32 vol;
  if (mag >= hi) {
    vol = 1.0f;
  } else {
    if (mag < lo)
      return;
    vol = (mag - lo) / (hi - lo);
    if (vol <= 0.0f)
      return;
  }
  PlaySfxAndSetVolume("Gungan_BlueOrbBounce", &part->pos, vol);
}
