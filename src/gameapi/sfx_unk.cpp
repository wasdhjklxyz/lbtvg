// gameapi/sfx_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nu2api/nucore/common.h"
#include <stdio.h>

typedef struct WORLDINFO_s {
  unsigned char pad0[0x80];
  char config_file[0x84]; // 0x80
} WORLDINFO;

i32 NuFileExists(char *name);
void SpecialSfxLoad(char *path, WORLDINFO *world);

// FUNCTION: LEGOBATMAN 0x00675430
void LoadSpecialSfxFile(WORLDINFO *world) {
  char path[256];
  sprintf(path, "%s.sfx", world->config_file);
  if (NuFileExists(path)) {
    SpecialSfxLoad(path, world);
  }
}
