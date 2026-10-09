// gameapi/definedlocators_unk.cpp: DefinedLocators_* (Mac); file name
// unproven.

#include "../nu2api/nucore/common.h"
#include "../nu2api/nucore/nustring.h"

// GLOBAL: LEGOBATMAN 0x00acd768
extern char **DefinedLocatorNames;

// GLOBAL: LEGOBATMAN 0x00acd76c
extern i32 DefinedLocatorCount;

// FUNCTION: LEGOBATMAN 0x00620aa0
i16 DefinedLocators_FindIX(char *name) {
  if (DefinedLocatorNames != 0) {
    for (i32 i = 0; i < DefinedLocatorCount; i++) {
      char *locator = DefinedLocatorNames[i];
      if (locator != 0 && NuStrICmp(locator, name) == 0)
        return i;
    }
  }
  return -1;
}
