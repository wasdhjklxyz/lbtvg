// gameapi/gizturret_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nu2api/nucore/common.h"
#include "../nu2api/nucore/nustring.h"

typedef struct GIZTURRET_s {
  u32 pad0[2];
  char name[0x13c - 8]; // 0x08
} GIZTURRET_s;

typedef struct GIZTURRETSYS_s {
  GIZTURRET_s *turrets; // 0x00
  u32 pad4;
  u16 count; // 0x08
} GIZTURRETSYS_s;

// FUNCTION: LEGOBATMAN 0x00661c30
GIZTURRET_s *GizTurret_FindByName(GIZTURRETSYS_s *system, char *name) {
  GIZTURRET_s *turret = 0;
  if (system != 0 && name != 0) {
    turret = system->turrets;
    for (i32 i = 0; i < system->count; ++i, ++turret) {
      if (NuStrICmp(turret->name, name) == 0) {
        return turret;
      }
    }
  }
  return turret;
}
