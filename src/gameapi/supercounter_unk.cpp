// gameapi/supercounter_unk.cpp: SuperCounter config helpers (Mac
// SuperCounterConfig_*); file name unproven.

#include "../nu2api/nucore/common.h"
#include <string.h>

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
