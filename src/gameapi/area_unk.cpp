// gameapi/area_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nu2api/nucore/common.h"
#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"
#include "../nu2api/numath/nuvec.h"
#include <stdio.h>

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x005f6d00
static f32 NuSinApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x005f6da0
static f32 NuCosApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x005f6db0
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

typedef struct AREADATA_s {
  u16 pad0[0x40 / 2];
  char file[0x20]; // 0x40
  i16 levels[0xe]; // 0x60
  u32 flags;       // 0x7c
  u32 pad80;
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

struct PART_s {
  u32 pad0[0x100 / 4];
  f32 f100; // 0x100
  u8 pad104[0x21a - 0x104];
  u8 b21a; // 0x21a
};

struct GameObject_s;

struct EXPLOSION {
  i32 field_0x00;
  i32 field_0x04;
  GameObject_s *object;
  nuvec_s position;
  f32 field_0x18;
  f32 field_0x1c;
  f32 field_0x20;
  i32 field_0x24;
  i32 field_0x28;
  u16 field_0x2c;
  u16 field_0x2e;
  u16 field_0x30;
  u8 field_0x32;
  u8 field_0x33;
  u8 field_0x34;
  u8 pad35[3];
};

// GLOBAL: LEGOBATMAN 0x00aca018
extern EXPLOSION Explosion[8];
// GLOBAL: LEGOBATMAN 0x00aca1d8
extern i32 i_explosion;

i32 qrand(void);
i32 ObjHitObj_Flags(GameObject_s *object);

// FUNCTION: LEGOBATMAN 0x005f6df0
EXPLOSION *AddExplosion(nuvec_s *position, float radius, float strength,
                        GameObject_s *object, i32 effect, i32 flags, i32 b) {
  EXPLOSION *explosion = &Explosion[i_explosion];
  explosion->position = *position;
  explosion->field_0x18 = radius;
  explosion->field_0x1c = 0;
  explosion->field_0x20 = strength;
  explosion->object = object;
  explosion->field_0x00 = 0;
  explosion->field_0x04 = 0;
  explosion->field_0x2c = qrand();
  explosion->field_0x2e = effect;
  explosion->field_0x24 = flags;
  explosion->field_0x28 = ObjHitObj_Flags(object);
  explosion->field_0x34 = b;
  explosion->field_0x30 = 0;
  explosion->field_0x32 = 1;
  explosion->field_0x33 = 0xff;
  if (++i_explosion == 8) {
    i_explosion = 0;
  }
  return explosion;
}

struct WORLDINFO_s {
  u8 pad0[0x80];
  char config_file[0x84]; // 0x80
  variptr_u giz_buffer;   // 0x104
  variptr_u giz_end;      // 0x108
  u8 pad10c[0x12c - 0x10c];
  LEVELDATA *current_level; // 0x12c
  u8 pad130[0x13c - 0x130];
  void *part_debris_sys; // 0x13c
  void *current_gscn;    // 0x140
  u8 pad144[0x2adc - 0x144];
  i32 page_pp;   // 0x2adc
  i32 page_part; // 0x2ae0
};

// GLOBAL: LEGOBATMAN 0x00aca1dc
extern i32 PDEBCOUNT;
// GLOBAL: LEGOBATMAN 0x00aca1e0
extern char **PDebNameList;

void edpartSetParticlePage(i32 page);
i32 NuFileExists(char *name);
i32 edpartLoadPage(char *path, i32 a, void *scene);
void *InitPartDebris(variptr_u *buffer, variptr_u *buffer_end, i32 max,
                     i32 count, char **names, i32 page);

// FUNCTION: LEGOBATMAN 0x005f7a60
void LoadPartFile(WORLDINFO_s *world) {
  char path[256];
  world->page_part = -1;
  edpartSetParticlePage(world->page_pp);

  if ((world->current_level->flags & 0xe0) == 0) {
    sprintf(path, "%s.par", world->config_file);
    if (NuFileExists(path) > 0) {
      world->page_part = edpartLoadPage(path, 1, world->current_gscn);
    }
    world->part_debris_sys =
        InitPartDebris(&world->giz_buffer, &world->giz_end, 0x40, PDEBCOUNT,
                       PDebNameList, world->page_part);
  }
}

// FUNCTION: LEGOBATMAN 0x005f7cc0
void PartStop_Flickerer(PART_s *part) {
  part->f100 = 5.0f;
  if ((part->b21a & 1) != 0)
    part->f100 = 2.5f;
  else if ((part->b21a & 2) != 0)
    part->f100 = 10.0f;
}

i32 NuStrICmp(const char *a, const char *b);

// FUNCTION: LEGOBATMAN 0x005fb060
AREADATA *Area_FindByName(char *name, i32 *indexDest) {
  if (name != 0) {
    for (i32 i = 0; i < AREACOUNT; i++) {
      if (NuStrICmp(ADataList[i].file, name) == 0) {
        if (indexDest != 0) {
          *indexDest = i;
        }
        return &ADataList[i];
      }
    }
  }

  if (indexDest != 0) {
    *indexDest = -1;
  }

  return 0;
}

typedef struct AREAFIXUP {
  char *name;
  AREADATA **area;
} AREAFIXUP;

// FUNCTION: LEGOBATMAN 0x005fb0e0
void Areas_FixUp(AREAFIXUP *fixup) {
  if (fixup != 0) {
    char *name = fixup->name;
    while (name != 0) {
      if (fixup->area != 0) {
        *fixup->area = Area_FindByName(name, 0);
      }
      fixup++;
      name = fixup->name;
    }
  }
  for (i32 i = 0; i < AREACOUNT; i++) {
    if (ADataList[i].flags & 0x40) {
      for (i32 j = 0; j < ADataList[i].level_count; j++) {
        LDataList[ADataList[i].levels[j]].flags |= 0x1000000;
      }
    }
  }
}

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

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_area_unk(f32 *v, f32 a, i32 i) {
  v[0] = NuSinApprox(i);
  v[1] = NuCosApprox(i);
  NuVec4Set(v, a, a, a, a);
}
