// batman/, file unknown (between LoadPerm2 and Text_LoadStrings).

#include "../nu2api/nucore/nustring.h"

// FUNCTION: LEGOBATMAN 0x004f9a60
void Text_DecodeButtons(char *in, char *out) {
  if (!NuStrICmp(in, "wibble"))
    NuStrCpy(out, "cross");
}
