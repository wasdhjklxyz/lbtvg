// gameapi/gizmoblowups_unk.cpp: placed by tools/new.py; file name unproven.

#include "../batman/worldinfo_unk.h"
#include "../nu2api/nucore/nustring.h"

struct GIZMOBLOWUP_s {
  u16 pad0[0xfe / 2];
  char name[0x2e]; // 0xfe
};

// FUNCTION: LEGOBATMAN 0x005dda30
GIZMOBLOWUP_s *GizmoBlowUp_FindByName(WORLDINFO_s *world, char *name) {
  GIZMOBLOWUP_s *blowup = world->gizmo_blowups;
  if (blowup != 0) {
    for (i32 i = 0; i < world->gizmo_blowup_count; ++i, ++blowup) {
      if (NuStrICmp(blowup->name, name) == 0)
        return blowup;
    }
  }
  return 0;
}
