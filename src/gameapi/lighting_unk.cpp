// gameapi/lighting_unk.cpp: placed by tools/new.py; file name unproven.

#include "../batman/worldinfo_unk.h"
#include "../nu2api/nu3d/nuspecial.h"
#include "../nu2api/nucore/common.h"
#include <stdio.h>
#include <string.h>

struct NuDynamicLight {
  static void destroy(NuDynamicLight *light);
};

void rtlDynamicFree(i32 id);

i32 NuFileExists(char *name);
void *rtlLoadSet(char *file, variptr_u *buffer, variptr_u buffer_end);
void *edrtlBurnoutLoad(char *file, variptr_u *buffer, variptr_u buffer_end);

// FUNCTION: LEGOBATMAN 0x0063e3b0
void LoadLights(WORLDINFO_s *world, char *path) {
  char filename[256];
  sprintf(filename, "%s.rtl", path);
  if (NuFileExists(filename)) {
    world->rtl_set = rtlLoadSet(filename, &world->buf104, world->bufEnd108);
  } else {
    world->rtl_set =
        rtlLoadSet("levels\\default.rtl", &world->buf104, world->bufEnd108);
  }
  sprintf(filename, "%s.bur", path);
  world->burnset = edrtlBurnoutLoad(filename, &world->buf104, world->bufEnd108);
}

i32 rtlDynamicAlloc(void);
void rtlDynamicSetType(i32 id, i32 type);
void rtlDynamicEnable(i32 id, i32 enable);

// FUNCTION: LEGOBATMAN 0x0063e470
void InitGameObjectLights(void) {
  for (i32 i = 0; i < 64; ++i) {
    Obj[i].dynamic_light_id = -1;
    for (i32 j = 0; j < 2; j++) {
      if (Obj[i].lights[j].light != 0) {
        NuDynamicLight::destroy(Obj[i].lights[j].light);
        Obj[i].lights[j].light = 0;
      }
    }
  }
  GameObject_s *object = Obj;
  for (i32 i = 0; i < HIGHGAMEOBJECT; ++i, ++object) {
    if ((object->flags1fc & 1) && (object->flags1fc & 0x1000)) {
      object->dynamic_light_id = rtlDynamicAlloc();
      if (object->dynamic_light_id != -1) {
        rtlDynamicSetType(object->dynamic_light_id, 2);
        rtlDynamicEnable(object->dynamic_light_id, 0);
      }
    }
  }
}

// FUNCTION: LEGOBATMAN 0x0063e540
void FreeGameObjectLights() {
  i32 i = 0;
  GameObject_s *object = Obj;
  for (; i < HIGHGAMEOBJECT; ++i, ++object) {
    if ((object->flags1fc & 1) && (object->flags1fc & 0x1000)) {
      if (object->dynamic_light_id != -1) {
        rtlDynamicFree(object->dynamic_light_id);
        object->dynamic_light_id = -1;
      }
      for (i32 j = 0; j < 2; j++) {
        if (object->lights[j].light != 0) {
          NuDynamicLight::destroy(object->lights[j].light);
          object->lights[j].light = 0;
        }
      }
    }
  }
}

// GLOBAL: LEGOBATMAN 0x00ad0c18
extern nuhspecial_s snake_hspecials[3];
// GLOBAL: LEGOBATMAN 0x00ad0c40
extern u32 snakebodies[0x440 / 4];

struct CHARACTERDATA_s {
  u32 pad0;
  u32 flags; // 0x04
  u32 pad8[(0x48 - 8) / 4];
};

struct AREASAVE_s {
  u8 pad0[6];
  u8 minikit_complete; // 0x06
  u8 pad7[0xc - 7];
};

struct Unk00acb82c {
  u8 pad0[0x1c8];
  i16 s1c8; // 0x1c8
  u8 pad1ca[0x240 - 0x1ca];
};

i32 AreaFromMiniKitID(i32 minikitId);

// GLOBAL: LEGOBATMAN 0x00acb81c
extern CHARACTERDATA_s *CDataList;
// GLOBAL: LEGOBATMAN 0x00aca594
extern AREASAVE_s *Game_AreaSave;
// GLOBAL: LEGOBATMAN 0x00acb830
extern u8 *Game_CharacterSave;
// GLOBAL: LEGOBATMAN 0x00ad0bf4
extern i32 g_unk00ad0bf4;
// GLOBAL: LEGOBATMAN 0x00acb82c
extern Unk00acb82c *g_unk00acb82c;

static inline i32 Unk_InList(i32 id) {
  if (id == -1 || g_unk00ad0bf4 == 0)
    return -1;
  return g_unk00acb82c[id].s1c8;
}

// FUNCTION: LEGOBATMAN 0x006408d0
i32 Collection_Got(i32 id) {
  i32 result;
  i32 area;
  if ((CDataList[id].flags & 0x4000000) &&
      (area = AreaFromMiniKitID(id)) != -1) {
    if (Game_AreaSave == 0 || Game_AreaSave[area].minikit_complete == 0)
      return 0;
    result = 2;
  } else {
    if (Game_CharacterSave != 0 && (Game_CharacterSave[id] & 1) == 0)
      return 0;
    result = 1;
  }
  if (Unk_InList(id) == -1)
    return 0;
  return result;
}

// FUNCTION: LEGOBATMAN 0x006419d0
void InitSnakes(WORLDINFO_s *world) {
  memset(snakebodies, 0, sizeof(snakebodies));
  NuSpecialFind(world->scn140, &snake_hspecials[0], "Snake_bit_1", 1);
  NuSpecialFind(world->scn140, &snake_hspecials[1], "Snake_bit_2", 1);
  NuSpecialFind(world->scn140, &snake_hspecials[2], "Snake_bit_3", 1);
}
