// gameapi/scoremultiplier_unk.cpp: ScoreMultiplier_* (Mac); file name
// unproven.

#include "../batman/worldinfo_unk.h"
#include "../nu2api/nucore/common.h"

struct SCOREMULTIPLIER_s {
  u8 pad00[0xa];
  u8 level; // 0x0a
  u8 pad0b;
  f32 timer; // 0x0c
};

// STUB: LEGOBATMAN 0x00635b10
// one register: orig reloads the multiplier into edx for the final timer
// store, ours into eax.
void ScoreMultiplier_IncrementMultiplier(GameObject_s *object, i32 amount,
                                         i32 max, f32 time) {
  if (object != 0 && object->score_multiplier != 0) {
    if (object->score_multiplier->level <= max) {
      if (object->score_multiplier->level + amount < max) {
        object->score_multiplier->level += amount;
        object->score_multiplier->timer = time;
        return;
      }
      object->score_multiplier->level = max;
    }
    object->score_multiplier->timer = time;
  }
}
