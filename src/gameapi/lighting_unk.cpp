// gameapi/lighting_unk.cpp: placed by tools/new.py; file name unproven.

#include "../batman/worldinfo_unk.h"
#include "../nu2api/nu3d/nuspecial.h"
#include "../nu2api/nucore/common.h"
#include <string.h>

struct NuDynamicLight {
  static void destroy(NuDynamicLight *light);
};

void rtlDynamicFree(i32 id);

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

// FUNCTION: LEGOBATMAN 0x006419d0
void InitSnakes(WORLDINFO_s *world) {
  memset(snakebodies, 0, sizeof(snakebodies));
  NuSpecialFind(world->scn140, &snake_hspecials[0], "Snake_bit_1", 1);
  NuSpecialFind(world->scn140, &snake_hspecials[1], "Snake_bit_2", 1);
  NuSpecialFind(world->scn140, &snake_hspecials[2], "Snake_bit_3", 1);
}
