// gameapi/losehat_unk.cpp: LoseHat (Mac); file name unproven.

#include "../batman/worldinfo_unk.h"
#include "../nu2api/nucore/common.h"

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
