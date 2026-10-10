// gameapi/lighting_unk.cpp: placed by tools/new.py; file name unproven.

#include "../batman/worldinfo_unk.h"
#include "../nu2api/nu3d/nuspecial.h"
#include "../nu2api/nucore/common.h"
#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"
#include <stdio.h>
#include <string.h>

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x0063d930
static f32 NuSinApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x0063d9d0
static f32 NuCosApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x0063db60
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);
// FUNCTION: LEGOBATMAN 0x00641800
static f32 NuFabs(f32 f);
// FUNCTION: LEGOBATMAN 0x00641820
static f32 NuFsign(f32 f);

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

i32 edppLoadPage(char *path, i32 a, nugscn_s *scene);
void *InitGameDebris(variptr_u *buffer, variptr_u *buffer_end, i32 count,
                     i32 flags, char **debris_name, i32 page);

// FUNCTION: LEGOBATMAN 0x006423a0
void Particles_Load(WORLDINFO_s *world, char **debris_name, i32 count,
                    i32 flags) {
  char path[0x100];

  world->page_pp = -1;
  sprintf(path, "%s.ptl", world->config_file);
  if (NuFileExists(path) != 0) {
    world->page_pp = edppLoadPage(path, 1, world->scn140);
  }

  world->p138 = InitGameDebris(&world->buf104, &world->bufEnd108, count, flags,
                               debris_name, world->page_pp);
}

extern "C" void PlatOnOff(i32 index, i32 enabled);
void NewBuzzFrames(nupad_s *pad, i32 frames, i32 flags);
void GameAudio_PlaySfx(i32 sfx, nuvec_s *position, i32 flags, i32 volume);
void PlayGruntSfx(GameObject_s *object);

// The carried object's view of the +0x9bc pointer.
struct SUPERCARRYOBJ_s {
  u32 pad0[0x58 / 4];
  nuvec_s pos; // 0x58
  u32 pad64[(0xa0 - 0x64) / 4];
  u32 flags; // 0xa0
  u32 pada4[(0x10c - 0xa4) / 4];
  u16 pad10c;
  i16 platform; // 0x10e
  u32 pad110[(0x120 - 0x110) / 4];
  u16 pad120;
  u16 flags122; // 0x122
};

#define SUPERCARRY_OBJ(o) ((SUPERCARRYOBJ_s *)(o)->techno)

// FUNCTION: LEGOBATMAN 0x006445b0
void SuperCarry_PickUpBlowUp(GameObject_s *obj) {
  SUPERCARRY_OBJ(obj)->flags122 |= 1;
  SUPERCARRY_OBJ(obj)->flags |= 0x20000000;
  *(u32 *)((u8 *)obj + 0x9e4) &= ~0x20000000;
  SUPERCARRY_OBJ(obj)->flags &= ~0x800000;
  SUPERCARRY_OBJ(obj)->flags &= ~0x4000;
  PlatOnOff(SUPERCARRY_OBJ(obj)->platform, 0);
  NewBuzzFrames(obj->p112c->pad0, 1, 0);
  GameAudio_PlaySfx(0x92, &SUPERCARRY_OBJ(obj)->pos, 0, 0);
  PlayGruntSfx(obj);
}

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_lighting_unk(f32 *v, f32 a, i32 i) {
  v[0] = NuSinApprox(i);
  v[1] = NuCosApprox(i);
  NuVec4Set(v, a, a, a, a);
  v[2] = NuFabs(a);
  v[5] = NuFsign(a);
}
