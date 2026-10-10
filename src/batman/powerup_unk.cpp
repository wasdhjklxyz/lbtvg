// batman/, file unknown: power-up pickups (0x0048d650).

#include "../gameapi/gameobject_unk.h"
#include "../gameapi/sfx_unk.h"
#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x0048b650
static f32 NuFdiv(f32 a, f32 b);
// FUNCTION: LEGOBATMAN 0x0048b6a0
static f32 NuSinApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x0048b760
static f32 NuCosApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x0048b780
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

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

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_powerup_unk(f32 *v, f32 a, i32 i) {
  v[3] = NuFdiv(a, v[4]);
  v[0] = NuSinApprox(i);
  v[1] = NuCosApprox(i);
  NuVec4Set(v, a, a, a, a);
}
