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

// GLOBAL: LEGOBATMAN 0x00acb000
char Door_ExitName[64];

// GLOBAL: LEGOBATMAN 0x00acb060
i32 Door_Start;

// GLOBAL: LEGOBATMAN 0x00963654
i32 Door_NextSock = -1;

// name is a Mac pairing hint (order): verify
// from saga legoapi/props/doors/doors.cpp
// FUNCTION: LEGOBATMAN 0x00614f20
void Door_Reset() {
  Door_ExitName[0] = '\0';
  Door_Start = 0;
  Door_NextSock = -1;
}

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
