// gameapi/supercounter_unk.cpp: SuperCounter config helpers (Mac
// SuperCounterConfig_*); file name unproven.

#include "../nu2api/nucore/common.h"
#include "../nu2api/numath/nuinline_unk.h"
#include <string.h>

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x00652240
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

typedef struct SUPERCOUNTER {
  u8 data[0x518];
  u8 field_518; // 0x518
  u8 field_519; // 0x519
  u8 field_51a; // 0x51a
  u8 pad51b;
} SUPERCOUNTER;

// FUNCTION: LEGOBATMAN 0x00652260
void SuperCounterConfig_Reset(SUPERCOUNTER *counter) {
  memset(counter, 0, sizeof(SUPERCOUNTER));
  counter->field_518 = 0xff;
  counter->field_519 = 0xff;
  counter->field_51a = 0xff;
}

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_supercounter_unk(f32 *v, f32 a, i32 i) {
  NuVec4Set(v, a, a, a, a);
}
