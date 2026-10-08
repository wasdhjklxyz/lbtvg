// batman/, file unknown: credits / titles scene (0x004a6c60..0x004aacb0).

#include "../nu2api/nu3d/nuspecial.h"
#include "../nu2api/nucore/nustring.h"
#include "worldinfo_unk.h"

// Episode data entries indexed by WORLDINFO_s::i2974; bit 0 of +0x1b
// selects hero vs villain.
struct EDATA_s {
  u8 pad0[0x1b];
  u8 flags1b; // 0x1b
  u8 pad1[0x20 - 0x1c];
};

struct Unk_LastAData {
  u8 pad0[0x8e];
  char b8e; // 0x8e
};

// GLOBAL: LEGOBATMAN 0x00ad11c0
extern EDATA_s *EDataList;
// GLOBAL: LEGOBATMAN 0x00ad11c4
extern i32 EPISODECOUNT;
// GLOBAL: LEGOBATMAN 0x00aca558
extern Unk_LastAData *LastAData;

i32 qrand(void);

// FUNCTION: LEGOBATMAN 0x004aacb0
void Credits_LoadImages(WORLDINFO_s *wi) {
  char name[64];
  wi->i2974 = -1;
  if (LastAData)
    wi->i2974 = LastAData->b8e;
  if (wi->i2974 == -1) {
    if (EPISODECOUNT > 0)
      wi->i2974 = qrand() / (0xffff / EPISODECOUNT + 1);
    if (wi->i2974 == -1)
      return;
  }
  NuStrCpy(name, "levels\\titles\\pictures");
  if (EDataList[wi->i2974].flags1b & 1)
    NuStrCat(name, "_1.gsc");
  else
    NuStrCat(name, "_2.gsc");
  wi->scn148 = NuGScnRead(&wi->buf104, wi->bufEnd108, name);
}
