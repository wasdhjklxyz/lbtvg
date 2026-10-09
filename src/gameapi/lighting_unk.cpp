// gameapi/lighting_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nu2api/nucore/common.h"
#include "gameobject_unk.h"

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
