// gameapi/carrying_unk.cpp: placed by tools/new.py; file name unproven.

#include "gameobject_unk.h"

// GLOBAL: LEGOBATMAN 0x009604fc
extern i32 LEGOCONTEXT_SUPERCARRY;

// FUNCTION: LEGOBATMAN 0x00645450
i32 SuperCarry_Carrying(GameObject_s *object) {
  if (LEGOCONTEXT_SUPERCARRY != -1 && object->b9db == LEGOCONTEXT_SUPERCARRY) {
    if (object->b9d9 == 2 || object->b9d9 == 3 || object->b9d9 == 6)
      return 1;
  }
  return 0;
}

#include "../nu2api/nu3d/nuspecial.h"
#include "../nu2api/numath/numtx.h"

struct HUBMINIKITPIECE_s {
  nuhspecial_s special; // 0x00
  numtx_s matrix;       // 0x0c
  u8 pad4c[0x64 - 0x4c];
  u8 direction; // 0x64
  u8 pad65[2];
  u8 direction_index; // 0x67
};

struct MINIKIT {
  nugscn_s *gscn;            // 0x00
  HUBMINIKITPIECE_s *pieces; // 0x04
  u8 piece_count;            // 0x08
  i8 field_0x9;              // 0x09
  i16 id;                    // 0x0a
  nuhspecial_s shadow;       // 0x0c
};

struct MiniKitCharData_s {
  u8 pad0[0xc];
  char *file; // 0x0c
  u8 pad10[0x48 - 0x10];
};

// GLOBAL: LEGOBATMAN 0x00acb81c
extern MiniKitCharData_s *CDataList;
// GLOBAL: LEGOBATMAN 0x00966804
extern char *minikit_direction_names[6];

int sprintf(char *buf, const char *fmt, ...);
i32 NuSpecialFind(nugscn_s *scene, nuhspecial_s *dest, char *name, i32 flags);
numtx_s *NuSpecialGetInstanceMtx(nuhspecial_s *special);

// from saga legoapi/gizmo/base/gizmo_sys.cpp
// STUB: LEGOBATMAN 0x00647150
// logic and layout right; orig pushes esi only after the gscn test (cmp
// [ebp], 0), ours pushes it first and compares against a zeroed esi.
void MiniKit_InitPieces(MINIKIT *minikit, i32 count, variptr_u *buf,
                        variptr_u *buf_end) {
  if (minikit->gscn != 0) {
    i32 direction_counts[6] = {0};
    char name[64];

    buf->addr = (buf->addr + 3) & ~3;
    minikit->pieces = (HUBMINIKITPIECE_s *)buf->addr;
    minikit->piece_count = 0;

    for (i32 index = 0; index < count; ++index) {
      for (i32 direction = 0; direction < 6; ++direction) {
        sprintf(name, "%s_%s_%i", CDataList[minikit->id].file,
                minikit_direction_names[direction], index);
        if (NuSpecialFind(minikit->gscn,
                          &minikit->pieces[minikit->piece_count].special, name,
                          1) == 0)
          continue;
        minikit->pieces[minikit->piece_count].matrix = *NuSpecialGetInstanceMtx(
            &minikit->pieces[minikit->piece_count].special);
        minikit->pieces[minikit->piece_count].direction = direction;
        minikit->pieces[minikit->piece_count].direction_index =
            direction_counts[direction]++;
        ++minikit->piece_count;
      }
    }

    sprintf(name, "%s_shadow", CDataList[minikit->id].file);
    NuSpecialFind(minikit->gscn, &minikit->shadow, name, 1);

    if (minikit->piece_count > 0)
      buf->addr += minikit->piece_count * sizeof(HUBMINIKITPIECE_s);
    else
      minikit->pieces = 0;
  } else {
    minikit->field_0x9 = -1;
  }
}
