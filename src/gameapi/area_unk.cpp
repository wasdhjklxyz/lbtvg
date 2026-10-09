// gameapi/area_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nu2api/nucore/common.h"

typedef struct AREADATA_s {
  u16 pad0[0x92 / 2];
  i16 minikit_id; // 0x92
  u16 pad94[(0xbc - 0x94) / 2];
} AREADATA;

// GLOBAL: LEGOBATMAN 0x00aca560
extern i32 AREACOUNT;

// GLOBAL: LEGOBATMAN 0x00aca554
extern AREADATA *ADataList;

// FUNCTION: LEGOBATMAN 0x005fc350
i32 AreaFromMiniKitID(i32 minikitId) {
  i32 i;

  for (i = 0; i < AREACOUNT; i++) {
    if (ADataList[i].minikit_id == minikitId) {
      return i;
    }
  }

  return -1;
}
