// gameapi/area_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nu2api/nucore/common.h"

typedef struct AREADATA_s {
  u16 pad0[0x60 / 2];
  i16 levels[0x12]; // 0x60
  u8 pad84;
  u8 level_count; // 0x85
  u16 pad86[(0x92 - 0x86) / 2];
  i16 minikit_id; // 0x92
  u16 pad94[(0xbc - 0x94) / 2];
} AREADATA;

// GLOBAL: LEGOBATMAN 0x00aca560
extern i32 AREACOUNT;

// GLOBAL: LEGOBATMAN 0x00aca554
extern AREADATA *ADataList;

typedef struct nugscn_s NUGSCN;

void NuGScnRemove(NUGSCN *scene);
i32 Unk0060fef0(void);
void Unk00611070(i32 a);
void Unk00612c20(i32 a);
void Unk0061f890(void);
void Unk0061f5e0(void);
void Unk0059a080(i32 mode);
void Unk00642490(void);

// GLOBAL: LEGOBATMAN 0x00a958cc
extern NUGSCN *big_icon_scene;
// GLOBAL: LEGOBATMAN 0x00a958c8
extern NUGSCN *area_scene;
// GLOBAL: LEGOBATMAN 0x00a958c4
extern NUGSCN *vehicle_scene;

typedef struct LEVELDATA_s {
  u32 pad0[0x64 / 4];
  u32 flags; // 0x64
  u8 pad68[0xab - 0x68];
  i8 area_index; // 0xab
  u8 padac[0xd8 - 0xac];
  i8 area_level_index; // 0xd8
  u8 padd9[0x150 - 0xd9];
} LEVELDATA;

// GLOBAL: LEGOBATMAN 0x00aca894
extern LEVELDATA *LDataList;

// FUNCTION: LEGOBATMAN 0x005fb1e0
LEVELDATA *Area_FindStatusLevel(AREADATA *area, i32 *indexDest) {
  if (indexDest != 0) {
    *indexDest = -1;
  }

  if (area != 0) {
    for (i32 i = 0; i < area->level_count; i++) {
      if (LDataList[area->levels[i]].flags & 0x400) {
        if (indexDest != 0) {
          *indexDest = area->levels[i];
        }
        return &LDataList[area->levels[i]];
      }
    }
  }

  return 0;
}

// STUB: LEGOBATMAN 0x005fb260
// close: orig keeps `level` in ebx (spilled) and the 0xe0 mask in bl;
// this recomputes level from levelIdx*0x150 and tests with an immediate.
LEVELDATA *Area_FindNextPlayLevel(i32 levelIdx) {
  LEVELDATA *level = &LDataList[levelIdx];
  i32 areaIdx = level->area_index;
  i32 i = level->area_level_index;

  if (areaIdx != -1) {
    for (; i < ADataList[areaIdx].level_count - 1; i++) {
      i32 idx = ADataList[areaIdx].levels[i];
      if ((LDataList[idx].flags & 0xe0) == 0) {
        return &LDataList[idx];
      }
    }
  }
  return level;
}

// FUNCTION: LEGOBATMAN 0x005fc2b0
void DumpAreaData(i32 mode, i32) {
  if (big_icon_scene != 0) {
    NuGScnRemove(big_icon_scene);
  }
  big_icon_scene = 0;
  if (area_scene != 0) {
    NuGScnRemove(area_scene);
  }
  area_scene = 0;
  if (vehicle_scene != 0) {
    NuGScnRemove(vehicle_scene);
  }
  vehicle_scene = 0;
  if (Unk0060fef0() & 1) {
    Unk00611070(0);
  }
  if (Unk0060fef0() & 2) {
    Unk00612c20(0);
  }
  Unk0061f890();
  Unk0061f5e0();
  Unk0059a080(mode);
  Unk00642490();
}

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
