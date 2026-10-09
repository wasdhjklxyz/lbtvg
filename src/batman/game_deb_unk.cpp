// batman/game_deb_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nu2api/nucore/common.h"
#include <stddef.h>

i32 LookupDebrisEffectPageIgnore(char *name, i32 page, i32 ignore);

// from saga legoapi/render/fx/game_deb.cpp
// FUNCTION: LEGOBATMAN 0x0055fd40
i32 LookupDebrisEffectPage(char *name, i32 page) {
  return LookupDebrisEffectPageIgnore(name, page, 0);
}

struct debinftype {
  char name[0x11]; // 0x00
  u8 page;         // 0x11
};

// GLOBAL: LEGOBATMAN 0x00a28cb4
extern i32 EDPP_MAX_TYPES;
// GLOBAL: LEGOBATMAN 0x00a28cb8
extern debinftype **debtab;
// GLOBAL: LEGOBATMAN 0x009eaec4
extern i32 edpp_page_used[8];

i32 NuStrICmp(const char *a, const char *b);

// from saga legoapi/render/fx/game_deb.cpp
// FUNCTION: LEGOBATMAN 0x0055fd60
extern "C" i32 LookupDebrisEffectPageOnly(char *name, char page) {
  if (name == NULL)
    return -1;
  if ((u8)page <= 7 && edpp_page_used[page] != 0) {
    for (i32 i = 1; i < EDPP_MAX_TYPES; ++i) {
      if (debtab[i] != NULL && debtab[i]->page == (u8)page &&
          NuStrICmp(debtab[i]->name, name) == 0)
        return i;
    }
  }
  for (i32 i = 1; i < EDPP_MAX_TYPES; ++i) {
    if (debtab[i] != NULL &&
        ((debtab[i]->page == 0 && edpp_page_used[0] != 0) ||
         (debtab[i]->page == 1 && edpp_page_used[1] != 0)) &&
        NuStrICmp(debtab[i]->name, name) == 0)
      return i;
  }
  return -1;
}
