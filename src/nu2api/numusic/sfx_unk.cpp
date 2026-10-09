// nu2api/numusic/sfx_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nucore/common.h"

typedef struct nusoundinfo_s {
  char *sfx_name; // 0x00
  u32 pad4[3];
  i16 next; // 0x10
  u16 pad12;
  u32 pad14[(0x40 - 0x14) / 4];
} NUSOUNDINFO;

u32 CRC_ProcessStringIgnoreCase(const char *str);
i32 NuStrNICmp(const char *a, const char *b, i32 n);

// GLOBAL: LEGOBATMAN 0x009f7a30
extern i16 *g_soundMap;
// GLOBAL: LEGOBATMAN 0x00a0f7e4
extern NUSOUNDINFO *g_soundInfo;
// GLOBAL: LEGOBATMAN 0x00a0f9c0
extern i32 g_unk00a0f9c0;

// STUB: LEGOBATMAN 0x00558d90
// close: block layout only; orig puts the map/flag "return -1" inline and
// does not align the loop head, this moves it to the end and pads the loop.
i32 GetSfxId(const char *name) {
  if (name == 0)
    return -1;
  NUSOUNDINFO *info = g_soundInfo;
  if (info == 0)
    return -1;
  i16 *map = g_soundMap;
  if (map == 0 || g_unk00a0f9c0 == 0)
    return -1;
  i32 index = map[CRC_ProcessStringIgnoreCase(name) & 0xff];
  if (index != -1) {
    do {
      if (NuStrNICmp(name, info[index].sfx_name, 32) == 0) {
        return index;
      }
      info = g_soundInfo;
      index = info[index].next;
    } while (index != -1);
  }

  return -1;
}

#include "../numath/nuvec.h"

void PlaySfxByIdEx(i32 sfx_id, nuvec_s *position, f32 volume, f32 pitch);

// from saga legoapi/audio/sfx.cpp
// FUNCTION: LEGOBATMAN 0x00559480
void PlaySfxAndSetVolume(char *name, nuvec_s *position, f32 volume) {
  i32 id = GetSfxId(name);
  if (id != -1) {
    PlaySfxByIdEx(id, position, volume, 1.0f);
  }
}
