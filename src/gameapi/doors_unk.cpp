// gameapi/doors_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nu2api/nucore/common.h"
#include "../nu2api/nucore/nustring.h"
#include "../nu2api/numath/nuvec.h"

typedef struct DOORSPLINE_s {
  i16 length; // 0x00
  u16 pad2;
  u32 pad4;
  nuvec_s *pts; // 0x08
} DOORSPLINE_s;

typedef struct DOOR_s {
  char name[0x80];               // 0x00
  char camera_spline_name[0x20]; // 0x80
  DOORSPLINE_s *spline;          // 0xa0
  u32 pada4[(0xf0 - 0xa4) / 4];
  i16 level; // 0xf0
  u8 padf2[0xf9 - 0xf2];
  u8 flags; // 0xf9
  u8 padfa[2];
  DOORSPLINE_s *camera_spline; // 0xfc
  f32 camera_wait;             // 0x100
  f32 camera_blend_time;       // 0x104
  u32 pad108[(0x128 - 0x108) / 4];
} DOOR_s;

typedef struct WORLDINFO_s {
  u8 pad0[0x120];
  i32 level_idx; // 0x120
  u8 pad124[0x47a8 - 0x124];
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

// GLOBAL: LEGOBATMAN 0x00acb068
extern i32 Door_UseCutCam;
// GLOBAL: LEGOBATMAN 0x00960894
extern WORLDINFO_s *WORLD;
// GLOBAL: LEGOBATMAN 0x00acb040
extern char Door_ExitCameraSplineName[];
// GLOBAL: LEGOBATMAN 0x00acafd4
extern nuvec_s Door_CutCamPos0;
// GLOBAL: LEGOBATMAN 0x00acafe8
extern nuvec_s Door_CutCamPos1;
// GLOBAL: LEGOBATMAN 0x00acafc8
extern f32 Door_CutCamWaitTime;
// GLOBAL: LEGOBATMAN 0x00acafe4
extern f32 Door_CutCamWait;
// GLOBAL: LEGOBATMAN 0x00acaff4
extern f32 Door_CutCamBlendTime;
// GLOBAL: LEGOBATMAN 0x00acb06c
extern i32 Door_CutLookAtPlayers;

// FUNCTION: LEGOBATMAN 0x006151e0
void Door_SetCutCam(DOOR_s *door) {
  Door_UseCutCam = 0;
  if (door->camera_spline != 0) {
    Door_UseCutCam = 1;
    Door_CutCamPos0 = door->camera_spline->pts[0];
    Door_CutCamPos1 = door->camera_spline->pts[1];
  } else {
    if (door->camera_spline_name[0] == '\0' ||
        door->level == WORLD->level_idx) {
      return;
    }
    Door_UseCutCam = 1;
    NuStrCpy(Door_ExitCameraSplineName, door->camera_spline_name);
  }
  Door_CutCamWait = Door_CutCamWaitTime = door->camera_wait;
  Door_CutCamBlendTime = door->camera_blend_time;
  Door_CutLookAtPlayers = door->flags & 2;
}

typedef struct PLAYERSTARTENTRY_s {
  nuvec_s *pos; // 0x00
  u32 pad4[2];
  i16 angle; // 0x0c
  u16 pade;
} PLAYERSTARTENTRY;

// GLOBAL: LEGOBATMAN 0x00ab3710
extern PLAYERSTARTENTRY PlayerStart[8];

void NuVecSub(nuvec_s *out, nuvec_s *a, nuvec_s *b);
i32 NuAtan2D(f32 dx, f32 dy);

// FUNCTION: LEGOBATMAN 0x00615390
i32 StartDoorPositions(void) {
  Door_Start = 0;
  if (Door_ExitName[0] != '\0') {
    DOOR_s *door = WORLD->doors;
    if (door != 0) {
      for (i32 i = 0; i < WORLD->door_count; i++, door++) {
        if (door->spline != 0 && NuStrICmp(door->name, Door_ExitName) == 0) {
          for (i32 i = 0; i < 8; i++) {
            i32 k = i * 2 + 4;
            if (door->spline->length > k + 1) {
              nuvec_s tmp;
              PlayerStart[i].pos = &door->spline->pts[k];
              NuVecSub(&tmp, &door->spline->pts[k + 1], PlayerStart[i].pos);
              PlayerStart[i].angle = NuAtan2D(tmp.x, tmp.z);
            } else {
              PlayerStart[i].pos = PlayerStart[i - 1].pos;
              PlayerStart[i].angle = PlayerStart[i - 1].angle;
            }
          }
          Door_Start = 1;
          return 1;
        }
      }
    }
    Door_ExitName[0] = '\0';
  }
  return 0;
}
