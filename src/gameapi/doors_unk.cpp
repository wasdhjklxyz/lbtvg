// gameapi/doors_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nu2api/nucore/common.h"
#include "../nu2api/nucore/nustring.h"

typedef struct DOOR_s {
  char name[0x128]; // name first; rest unknown
} DOOR_s;

typedef struct WORLDINFO_s {
  u8 pad0[0x47a8];
  DOOR_s *doors;  // 0x47a8
  i32 door_count; // 0x47ac
} WORLDINFO_s;

// FUNCTION: LEGOBATMAN 0x00615190
DOOR_s *Door_FindByName(WORLDINFO_s *world, char *name) {
  DOOR_s *door = world->doors;
  if (door != 0) {
    for (i32 i = 0; i < world->door_count; i++, door++) {
      if (NuStrICmp(name, door->name) == 0) {
        return door;
      }
    }
  }
  return 0;
}
