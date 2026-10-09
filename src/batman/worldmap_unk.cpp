// batman/worldmap_unk.cpp: WorldMap (Mac WorldMap::*); file name unproven.

#include "../nu2api/nucore/common.h"
#include "../nu2api/nucore/nustring.h"

struct WORLDINFO_s;

extern "C" int NuSPrintf(char *buf, char *fmt, ...);
char *NuStrIStr(char *str, const char *sub);

class WorldMap {
public:
  char *GetPointerSpecialName(WORLDINFO_s *world, int pointer) const;
};

// GLOBAL: LEGOBATMAN 0x009cf69c
extern char g_unk009cf69c[]; // shared name buffer

// FUNCTION: LEGOBATMAN 0x00519370
char *WorldMap::GetPointerSpecialName(WORLDINFO_s *world, int pointer) const {
  char *suffix = "A";
  if (pointer == 1)
    suffix = "B";
  else if (pointer == 2)
    suffix = "C";
  if (NuStrIStr((char *)world, "batcave_f") != 0)
    NuSPrintf(g_unk009cf69c, "villain_pointer_%s", suffix);
  else
    NuSPrintf(g_unk009cf69c, "hero_pointer_%s", suffix);
  return g_unk009cf69c;
}
