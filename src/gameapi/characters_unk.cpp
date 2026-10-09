// gameapi/characters_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nu2api/nucore/common.h"
#include "../nu2api/nucore/nustring.h"

typedef struct CHARACTERDATA_s {
  u32 pad0[3];
  char *file; // 0x0c
  u32 pad10[(0x48 - 0x10) / 4];
} CHARACTERDATA;

// GLOBAL: LEGOBATMAN 0x00acb820
extern i32 CHARCOUNT;

// GLOBAL: LEGOBATMAN 0x00acb81c
extern CHARACTERDATA *CDataList;

// FUNCTION: LEGOBATMAN 0x0061f190
i32 CharIDFromName(char *name) {
  for (i32 i = 0; i < CHARCOUNT; i++) {
    if (NuStrICmp(CDataList[i].file, name) == 0) {
      return i;
    }
  }

  return -1;
}
