// gameapi/specialmove_unk.cpp: SpecialMove_* (Mac); file name unproven.

#include "../batman/worldinfo_unk.h"
#include "../nu2api/nucore/common.h"
#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x0062ddd0
static f32 NuSinApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x0062de70
static f32 NuCosApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x0062de80
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

// 0x9c bytes per special move; only the phase byte is evidenced.
struct SPECIALMOVE_s {
  u8 pad00[0x77];
  u8 layer_bits; // 0x77
  u8 pad78[0x98 - 0x78];
  i8 next_phase; // 0x98, -1 = single phase
  u8 pad99[0x9c - 0x99];
};

// GLOBAL: LEGOBATMAN 0x00acd800
extern SPECIALMOVE_s *SpecialMoves;

// GLOBAL: LEGOBATMAN 0x00960528
extern i32 LEGOCONTEXT_SPECIALMOVE;

// GLOBAL: LEGOBATMAN 0x0096052c
extern i32 LEGOCONTEXT_SPECIALMOVE2;

// FUNCTION: LEGOBATMAN 0x0062dee0
i32 SpecialMove_IsInMultiPhase(GameObject_s *object) {
  if (object != 0 &&
      (object->b9db == LEGOCONTEXT_SPECIALMOVE ||
       object->b9db == LEGOCONTEXT_SPECIALMOVE2) &&
      object->special_move != -1 &&
      SpecialMoves[object->special_move].next_phase != -1)
    return 1;
  return 0;
}

// FUNCTION: LEGOBATMAN 0x0062df30
void SpecialMove_AdjustLayerBits(GameObject_s *object, u32 &bits) {
  if (object == 0)
    return;
  if ((LEGOCONTEXT_SPECIALMOVE != -1 &&
       object->b9db == LEGOCONTEXT_SPECIALMOVE) ||
      (LEGOCONTEXT_SPECIALMOVE2 != -1 &&
       object->b9db == LEGOCONTEXT_SPECIALMOVE2))
    bits |= SpecialMoves[object->special_move].layer_bits;
}

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_specialmove_unk(f32 *v, f32 a, i32 i) {
  v[0] = NuSinApprox(i);
  v[1] = NuCosApprox(i);
  NuVec4Set(v, a, a, a, a);
}
