// gameapi/gamepads_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nu2api/nucore/common.h"
#include "../nu2api/numath/nuvec.h"

typedef struct nupad_s nupad_s;

typedef struct GAMEPAD_s {
  nupad_s *pad;
} GAMEPAD_s;

typedef struct GameObject_s {
  unsigned char pad0[0x1fc];
  u8 flags_low; // 0x1fc
  unsigned char pad1fd[0x112c - 0x1fd];
  GAMEPAD_s *pad_gamepad; // 0x112c
} GameObject_s;

void NuSoundAddRumble(nupad_s *pad, f32 duration, i32 amount, f32 unk,
                      f32 strength);

// GLOBAL: LEGOBATMAN 0x00a95fec
extern f32 DEFAULTFPS;

// GLOBAL: LEGOBATMAN 0x00ab3960
extern GameObject_s *Player[8];

void PlaySfxById(i32 sfx_id, nuvec_s *position);

// GLOBAL: LEGOBATMAN 0x009e7bd8
extern nuvec_s nusound_special_positions[3];

// FUNCTION: LEGOBATMAN 0x0059f1d0
void GameAudio_PlaySfxById(i32 sfx_id, nuvec_s *position, i32 flags, i32) {
  if (flags != 0) {
    if (flags == 1 || flags == 3) {
      nusound_special_positions[1] = *position;
      PlaySfxById(sfx_id, &nusound_special_positions[1]);
    }
    if (flags == 2 || flags == 3) {
      nusound_special_positions[2] = *position;
      PlaySfxById(sfx_id, &nusound_special_positions[2]);
    }
  } else {
    PlaySfxById(sfx_id, position);
  }
}

typedef struct GAMECAMERA_s {
  u32 pad0[0x1d4 / 4];
  f32 shake_target_amount; // 0x1d4
  f32 shake_time;          // 0x1d8
  f32 shake_speed;         // 0x1dc
} GAMECAMERA_s;

// GLOBAL: LEGOBATMAN 0x0095f624
extern GAMECAMERA_s *GameCam;

// FUNCTION: LEGOBATMAN 0x005a1fb0
void GameCam_NewShake(GAMECAMERA_s *camera, float amount, float duration,
                      float speed) {
  if (camera == 0) {
    camera = GameCam;
  }
  camera->shake_target_amount = amount;
  camera->shake_time = duration;
  camera->shake_speed = speed;
}

// FUNCTION: LEGOBATMAN 0x005a2d10
void NewRumble(nupad_s *pad, float strength, i32) {
  if (pad != 0) {
    i32 amount = (i32)(strength * 255.0f);
    if (amount > 255)
      amount = 255;
    NuSoundAddRumble(pad, 0.0f, amount, 0.0f, strength);
  }
}

// FUNCTION: LEGOBATMAN 0x005a2dd0
void NewRumbleAllPlayers(float strength, float duration, i32 frames, i32) {
  if (frames > 0) {
    f32 frame_duration = (f32)frames / DEFAULTFPS;
    if (frame_duration > duration)
      duration = frame_duration;
  }
  for (i32 i = 0; i < 8; i++) {
    GameObject_s *object = Player[i];
    if (object != 0 && (object->flags_low & 0x80)) {
      if (object->pad_gamepad->pad != 0) {
        NuSoundAddRumble(object->pad_gamepad->pad, duration,
                         (i32)(strength * 255.0f), 0.0f, strength);
      }
    }
  }
}
