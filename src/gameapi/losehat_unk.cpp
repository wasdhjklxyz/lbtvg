// gameapi/losehat_unk.cpp: LoseHat (Mac); file name unproven.

#include "../batman/worldinfo_unk.h"
#include "../nu2api/nucore/common.h"
#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x0065b9e0
static f32 NuFabs(f32 f);
// FUNCTION: LEGOBATMAN 0x0065ba00
static f32 NuSinApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x0065baa0
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

// Hat drop at a locator matrix; name unknown.
void Unk0065c0b0(void *mtx, i32 a, GameObject_s *object, i32 b);

// FUNCTION: LEGOBATMAN 0x0065c320
void LoseHat(GameObject_s *object) {
  if (object->hat == 0)
    return;
  i32 locator = object->p54->p24->hat_locator;
  if (locator != -1 && object->p50->locator_present[locator] != 0) {
    Unk0065c0b0(&object->locator_mtx[locator], 0, object, 1);
    object->hat = 0;
    return;
  }
  locator = object->p54->p24->hat_locator2;
  if (locator != -1 && object->p50->locator_present[locator] != 0)
    Unk0065c0b0(&object->locator_mtx[locator], 0, object, 1);
  object->hat = 0;
}

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_losehat_unk(f32 *v, f32 a, i32 i) {
  v[2] = NuFabs(a);
  v[0] = NuSinApprox(i);
  NuVec4Set(v, a, a, a, a);
}
