// batman/sockpar_unk.cpp: SockParBATMAN_* socket config parsers (Mac
// symbols); between pcbatman.cpp and pcapi.cpp by link order, file name
// unproven.

#include "../nu2api/nucore/common.h"
#include "../nu2api/numath/nuinline_unk.h"

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x00415940
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

typedef struct nufpar_s NUFPAR;

struct SOCKPARBATMAN_s {
  u8 pad0[0x6a];
  u16 flags; // 0x6a, 1 = one player only, 2 = two player only
};

// FUNCTION: LEGOBATMAN 0x00415970
void SockParBATMAN_one_player_only(NUFPAR *parser, void *ctx) {
  if (ctx != 0)
    ((SOCKPARBATMAN_s *)ctx)->flags |= 1;
}

// FUNCTION: LEGOBATMAN 0x00415990
void SockParBATMAN_two_player_only(NUFPAR *parser, void *ctx) {
  if (ctx != 0)
    ((SOCKPARBATMAN_s *)ctx)->flags |= 2;
}

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_sockpar_unk(f32 *v, f32 a, i32 i) {
  NuVec4Set(v, a, a, a, a);
}
