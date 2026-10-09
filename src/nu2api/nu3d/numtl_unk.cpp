// nu2api/nu3d/numtl_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nucore/common.h"

typedef struct numtl_s {
  unsigned char pad0[0x2c0];
  u16 version; // 0x2c0, bumped by NuMtlUpdate
  u16 filler5;
} NUMTL;

void NuMtlUpdatePS(NUMTL *mtl);

// FUNCTION: LEGOBATMAN 0x00727b40
void NuMtlUpdate(NUMTL *mtl) {
  NuMtlUpdatePS(mtl);
  mtl->version++;
}
