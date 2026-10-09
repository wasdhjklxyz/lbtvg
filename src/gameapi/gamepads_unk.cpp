// gameapi/gamepads_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nu2api/nucore/common.h"
#include "../nu2api/numath/nuvec.h"
#include <string.h>

typedef struct nupad_s nupad_s;

typedef struct GAMEPAD_s {
  nupad_s *pad;
  u8 pad4[0x26 - 4];
  u16 input_angle; // 0x26
} GAMEPAD_s;

typedef struct GameObject_s {
  unsigned char pad0[0x1fc];
  u8 flags_low; // 0x1fc
  unsigned char pad1fd[0x871 - 0x1fd];
  char sock_id; // 0x871
  unsigned char pad872[0x896 - 0x872];
  u16 yrot; // 0x896
  unsigned char pad898[0x112c - 0x898];
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

struct GAMEAUDIO {
  i32 (*override_footstep_fn)(GameObject_s *, i32);
  i32 (*check_reverb_fn)(void);
  i16 sfx_ids[201];
};

// GLOBAL: LEGOBATMAN 0x00a95908
extern GAMEAUDIO GameAudio_Default;
// GLOBAL: LEGOBATMAN 0x0095ed94
extern GAMEAUDIO *GameAudio;

typedef struct APIOBJECT_s {
  u8 pad0[0x1fc];
  u8 flags_low; // 0x1fc
  u8 pad1fd[0x24c - 0x1fd];
  i8 player_index; // 0x24c
} APIOBJECT;

// FUNCTION: LEGOBATMAN 0x0059f160
i32 GameAudio_GetPlrSfxBits(void *object_ptr) {
  APIOBJECT *object = (APIOBJECT *)object_ptr;
  if (object != 0 && (object->flags_low & 0x80)) {
    return 1 << object->player_index;
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x0059f190
void GameAudio_Reset() {
  GameAudio = &GameAudio_Default;
  memset(&GameAudio_Default, 0, sizeof(GameAudio_Default));
  for (i32 i = 0; i < 201; ++i) {
    GameAudio_Default.sfx_ids[i] = -1;
  }
}

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
  u32 pad1e0[(0x1fc - 0x1e0) / 4];
  i32 input_yaw; // 0x1fc
} GAMECAMERA_s;

// GLOBAL: LEGOBATMAN 0x0095f624
extern GAMECAMERA_s *GameCam;

// name is a Mac pairing hint (gapfill): verify
// from saga legoapi/audio/sfx.cpp
// FUNCTION: LEGOBATMAN 0x0059f260
void GameAudio_PlaySfx(i32 sfx, nuvec_s *position, i32 flags, i32 volume) {
  if ((u32)sfx <= 200) {
    GameAudio_PlaySfxById(GameAudio->sfx_ids[sfx], position, flags, volume);
  }
}

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

// FUNCTION: LEGOBATMAN 0x005a2d60
void NewBuzz(nupad_s *pad, float duration, i32) {
  if (pad != 0)
    NuSoundAddRumble(pad, duration, 0, 0.0f, 0.0f);
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

typedef struct TECHNO_s {
  u8 pad0[0x83];
  u8 target_mode; // 0x83
  u8 pad84[0xb8 - 0x84];
  GameObject_s *controlled_object; // 0xb8
  u8 padbc[0xcc - 0xbc];
} TECHNO;

typedef struct SOCK_s {
  u8 pad0[0x68];
  u8 flags; // 0x68
  u8 pad69[0x6e - 0x69];
  u16 input_yaw; // 0x6e
  u8 pad70[0x184 - 0x70];
} SOCK;

typedef struct SOCKSYS_s {
  SOCK *sock;
} SOCKSYS;

typedef struct WORLDINFO_s {
  u8 pad0[0x29cc];
  SOCKSYS *sock_sys; // 0x29cc
  u8 pad29d0[0x51ec - 0x29d0];
  TECHNO *technos; // 0x51ec
  i32 ntechnos;    // 0x51f0
} WORLDINFO;

// GLOBAL: LEGOBATMAN 0x00960894
extern WORLDINFO *WORLD;

// FUNCTION: LEGOBATMAN 0x005a3940
u16 GamePad_InputAngle(GameObject_s *object, GAMEPAD_s *pad) {
  if ((object->flags_low & 0x80) == 0 || object->sock_id == -1) {
    goto camera_relative;
  }
  if ((WORLD->sock_sys->sock[object->sock_id].flags & 0x40) != 0) {
    goto socket_relative;
  }

camera_relative:
  return pad->input_angle + GameCam->input_yaw;

socket_relative:
  return WORLD->sock_sys->sock[object->sock_id].input_yaw + object->yrot +
         pad->input_angle;
}

// FUNCTION: LEGOBATMAN 0x005a6550
TECHNO *Technos_FindControllingTechno(GameObject_s *object) {
  if (object != 0) {
    for (i32 index = 0; index < WORLD->ntechnos; ++index) {
      TECHNO *techno = &WORLD->technos[index];
      if (techno->target_mode == 1 && techno->controlled_object == object)
        return techno;
    }
  }
  return 0;
}
