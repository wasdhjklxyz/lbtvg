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
  NUVEC pos;                // 0x11c
  NUVEC desired_position;   // 0x128
  NUVEC blend_start_target; // 0x134
  NUVEC target;             // 0x140
  NUVEC blend_end_target;   // 0x14c
  u8 pad158[0x164 - 0x158];
  NUVEC blend_start_position; // 0x164
  u8 pad170[0x17c - 0x170];
  NUVEC blend_end_position; // 0x17c
  u8 pad188[0x1bc - 0x188];
  f32 blend_time;      // 0x1bc
  f32 blend_duration;  // 0x1c0
  f32 blend_curve;     // 0x1c4
  f32 judder_time;     // 0x1c8
  f32 judder_duration; // 0x1cc
  u8 pad1d0[0x204 - 0x1d0];
  u16 blend_start_pitch;   // 0x204
  u16 desired_pitch;       // 0x206
  u16 blend_start_yaw;     // 0x208
  u16 desired_yaw;         // 0x20a
  u16 blend_start_roll;    // 0x20c
  u16 desired_roll;        // 0x20e
  u8 judder_reverse;       // 0x210
  u8 judder_axis;          // 0x211
  u8 reset_blend;          // 0x212
  u8 blend_mode;           // 0x213
  i8 mode;                 // 0x214
  u8 previous_mode;        // 0x215
  u8 previous_camera_mode; // 0x216
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

// FUNCTION: LEGOBATMAN 0x005a27c0
void GameCam_Blend(GAMECAMERA_s *camera, f32 duration, f32 curve, i32 mode) {
  if (camera == 0)
    camera = GameCam;
  if (!(duration > 0.0f) || camera->mode == -1)
    return;

  camera->blend_start_pitch = camera->desired_pitch;
  camera->blend_start_yaw = camera->desired_yaw;
  camera->blend_start_roll = camera->desired_roll;
  camera->blend_mode = mode == 0 ? 1 : 2;
  camera->previous_camera_mode = camera->previous_mode;
  camera->reset_blend = 1;
  camera->blend_curve = curve;
  camera->blend_time = 0.0f;
  camera->blend_duration = duration;
  camera->blend_start_target = camera->target;
  camera->blend_start_position = camera->desired_position;
  camera->blend_end_target = camera->target;
  camera->blend_end_position = camera->desired_position;
}
