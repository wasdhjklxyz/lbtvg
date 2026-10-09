// batman/game_deb_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nu2api/nucore/common.h"
#include <stddef.h>

i32 LookupDebrisEffectPageIgnore(char *name, i32 page, i32 ignore);

// from saga legoapi/render/fx/game_deb.cpp
// FUNCTION: LEGOBATMAN 0x0055fd40
i32 LookupDebrisEffectPage(char *name, i32 page) {
  return LookupDebrisEffectPageIgnore(name, page, 0);
}
