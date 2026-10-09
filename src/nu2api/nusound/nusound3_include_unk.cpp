// nu2api/nusound/nusound3_include_unk.cpp: placed by tools/new.py; file name
// unproven.

#include "../nucore/common.h"

typedef struct nusound_filename_info_s nusound_filename_info_s;

struct MusicManager {
  void Initialise(char *cfg, void *unk, VARIPTR *buffer_start,
                  VARIPTR buffer_end);
  void GetSoundFiles(nusound_filename_info_s **finfo, void *unk);
};

// GLOBAL: LEGOBATMAN 0x00a95dc0
extern MusicManager music_man;
// GLOBAL: LEGOBATMAN 0x0095d910
extern char *audio_ps2_music_ext;
// GLOBAL: LEGOBATMAN 0x00a958f0
extern i16 *ActionPairTab;
// GLOBAL: LEGOBATMAN 0x00a95d84
extern i16 *AmbientPairTab;

// FUNCTION: LEGOBATMAN 0x005a0830
nusound_filename_info_s *ConfigureMusic(char *file, VARIPTR *bufferStart,
                                        VARIPTR *bufferEnd) {
  nusound_filename_info_s *finfo;

  music_man.Initialise("audio\\music.cfg", 0, bufferStart, *bufferEnd);
  music_man.GetSoundFiles(&finfo, 0);

  audio_ps2_music_ext = ".mib";

  u8 *p = (u8 *)((bufferStart->addr + 3) & ~3);
  *(i32 *)p = 0;
  p += 0x1c;
  *(i16 *)p = -1;
  ActionPairTab = (i16 *)p;
  p += 0x1c;
  *(i16 *)p = -1;
  AmbientPairTab = (i16 *)p;
  p += 4;
  bufferStart->void_ptr = p;

  return finfo;
}

#include "../numath/nuvec.h"

struct GAMECAMERA_s {
  u8 pad0[0x11c];
  NUVEC pos;
  u8 pad128[0x1c8 - 0x128];
  f32 judder_time;
  f32 judder_duration;
  u8 pad1d0[0x210 - 0x1d0];
  u8 judder_reverse;
  u8 judder_axis;
};

struct LEVELDATA_s {
  u8 pad0[0xdd];
  u8 camera_judder_distance;
};

struct WORLDINFO_s {
  u8 pad0[0x12c];
  LEVELDATA_s *current_level;
};

// GLOBAL: LEGOBATMAN 0x00aca574
extern i32 VehicleArea;
// GLOBAL: LEGOBATMAN 0x0095f624
extern GAMECAMERA_s *GameCam;
// GLOBAL: LEGOBATMAN 0x00960894
extern WORLDINFO_s *WORLD;

i32 qrand(void);
f32 NuVecDist(NUVEC *v0, NUVEC *v1, NUVEC *d);

static inline f32 NuFabs(f32 f) {
  *(u32 *)&f &= 0x7fffffff;
  return f;
}

// FUNCTION: LEGOBATMAN 0x005a21e0
void GameCam_Judder(GAMECAMERA_s *camera, f32 amount, i32 axis, NUVEC *source) {
  if (camera == 0)
    camera = GameCam;
  f32 absolute_amount = NuFabs(amount);
  if (!(absolute_amount > camera->judder_time))
    return;
  if (source != 0) {
    f32 distance = NuVecDist(&camera->pos, source, 0);
    f32 maximum_distance =
        (f32)(u8)WORLD->current_level->camera_judder_distance;
    if (distance < maximum_distance) {
      f32 attenuated_amount =
          ((maximum_distance - distance) / maximum_distance) * absolute_amount;
      if (attenuated_amount > camera->judder_time) {
        camera->judder_duration = attenuated_amount;
        camera->judder_time = attenuated_amount;
        if (amount < 0.0f)
          camera->judder_reverse = 1;
        else
          camera->judder_reverse = 0;
      }
    }
  } else {
    camera->judder_duration = absolute_amount;
    camera->judder_time = absolute_amount;
    if (amount < 0.0f)
      camera->judder_reverse = 1;
    else
      camera->judder_reverse = 0;
  }
  camera->judder_axis = (u8)axis;
}

// FUNCTION: LEGOBATMAN 0x005a2310
void GameCam_HitJudder(void) {
  f32 amount = VehicleArea != 0 ? -0.3f : -0.2f;
  if (qrand() < 0x8000) {
    amount = -amount;
  }
  GameCam_Judder(GameCam, amount, qrand() / 0x5556, 0);
}
