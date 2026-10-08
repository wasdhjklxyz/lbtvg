// batman/, file unknown: gizmo flow actions (0x00483e20).

#include "../nu2api/nucore/nustring.h"

struct GIZFLOW_s;
struct FLOWBOX_s;

void PlayRadio(char *special, char *blowup, i32 loop);

// FUNCTION: LEGOBATMAN 0x00483e20
void GizActions_PlayRadio(GIZFLOW_s *flow, FLOWBOX_s *box, char **args,
                          int argc) {
  i32 loop = 1;
  char *special = 0;
  char *blowup = 0;
  char *s;
  i32 i;
  for (i = 0; i < argc; i++) {
    if ((s = NuStrIStr(args[i], "BlowUp=")))
      blowup = s + NuStrLen("BlowUp=");
    else if ((s = NuStrIStr(args[i], "Special=")))
      special = s + NuStrLen("Special=");
    else if (!NuStrICmp(args[i], "FALSE"))
      loop = 0;
  }
  if (special || blowup)
    PlayRadio(special, blowup, loop);
}
