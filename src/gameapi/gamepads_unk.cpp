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

struct GruntCharacterData_s;

typedef struct GameObject_s {
  unsigned char pad0[0x54];
  GruntCharacterData_s *character; // 0x54
  unsigned char pad58[0x80 - 0x58];
  nuvec_s pos80; // 0x80
  unsigned char pad8c[0x19c - 0x8c];
  nuvec_s lower_position; // 0x19c
  unsigned char pad1a8[0x1fc - 0x1a8];
  u8 flags_low; // 0x1fc
  unsigned char pad1fd[0x252 - 0x1fd];
  u8 is_underwater;    // 0x252
  u8 intersects_water; // 0x253
  u8 b254;             // 0x254
  unsigned char pad255[0x871 - 0x255];
  char sock_id; // 0x871
  unsigned char pad872[0x896 - 0x872];
  u16 yrot; // 0x896
  unsigned char pad898[0x112c - 0x898];
  GAMEPAD_s *pad_gamepad; // 0x112c
  unsigned char pad1130[0x140c - 0x1130];
  u32 flags140c; // 0x140c
  unsigned char pad1410[0x1414 - 0x1410];
  u32 flags1414; // 0x1414
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

// from saga legoapi/audio/sfx.cpp
// FUNCTION: LEGOBATMAN 0x0059f260
void GameAudio_PlaySfx(i32 sfx, nuvec_s *position, i32 flags, i32 volume) {
  if ((u32)sfx <= 200) {
    GameAudio_PlaySfxById(GameAudio->sfx_ids[sfx], position, flags, volume);
  }
}

// STUB: LEGOBATMAN 0x0059f8d0
// skipped: switch on jump type compiles to a jump table; not attempted.
#if 0
void PlaySfxByIdAndSetVolume(i32 sfx_id, nuvec_s *position, f32 volume);

// from saga legoapi/audio/sfx.cpp
void PlayJumpSfx(GameObject_s *object, i32 type) {
    i32 sfx;
    const u32 flags = object->apiobj.character_data->model_flags;
    if ((flags & 0x40) != 0) {
        sfx = GameAudio->sfx_ids[1];
    } else if ((flags & 8) != 0) {
        switch (type) {
            case 0:
                sfx = GameAudio->sfx_ids[2];
                break;
            case 1:
                sfx = GameAudio->sfx_ids[3];
                break;
            case 2:
                sfx = GameAudio->sfx_ids[6];
                break;
            case 3:
                sfx = GameAudio->sfx_ids[4];
                break;
            case 4:
                sfx = GameAudio->sfx_ids[5];
                break;
            default:
                return;
        }
    } else {
        sfx = GameAudio->sfx_ids[0];
    }
    if (sfx != -1) {
        if (static_cast<i8>(object->apiobj.flags_low) >= 0 && (object->field_0xefb & 8) == 0) {
            PlaySfxByIdAndSetVolume(sfx, &object->apiobj.lower_position, 0.5f);
        } else {
            GameAudio_PlaySfxById(sfx, &object->apiobj.lower_position, 0, 1);
        }
    }
}
#endif

struct GruntGameCharacter_s {
  u8 pad0[0x13c];
  u32 flags13c; // 0x13c
  u8 pad140[0x14c - 0x140];
  u32 flags14c; // 0x14c
  u8 pad150[0x1a6 - 0x150];
  i16 sfx_grunt; // 0x1a6
  u8 pad1a8[0x1ba - 0x1a8];
  i16 s1ba; // 0x1ba, PlayLandSfx: per-character land sfx ids (-1 = none)
  i16 s1bc; // 0x1bc
  i16 s1be; // 0x1be
  i16 s1c0; // 0x1c0
};

struct GruntCharacterData_s {
  u8 pad0[4];
  u32 model_flags; // 0x04
  u8 pad8[0x24 - 8];
  GruntGameCharacter_s *game_character; // 0x24
};

struct LEVELDATA_s {
  u8 pad0[0x64];
  u32 flags; // 0x64
};

typedef struct WORLDINFO_s {
  u8 pad0[0x12c];
  LEVELDATA_s *current_level; // 0x12c
  u8 pad130[0x29cc - 0x130];
  struct SOCKSYS_s *sock_sys; // 0x29cc
  u8 pad29d0[0x51ec - 0x29d0];
  struct TECHNO_s *technos; // 0x51ec
  i32 ntechnos;             // 0x51f0
} WORLDINFO;
WORLDINFO_s *WorldInfo_CurrentlyActive(void);
void PlaySfxByIdAndSetVolume(i32 sfx_id, nuvec_s *position, f32 volume);

// FUNCTION: LEGOBATMAN 0x0059f9a0
void PlayLandSfx(GameObject_s *object, i32 type, i32 unk) {
  i32 sfx;
  if (type == -1)
    return;
  if (type == 1) {
    if (object->character->game_character->s1bc != -1) {
      if ((object->character->game_character->flags13c & 0x800000) &&
          object->character->game_character->s1be != -1)
        PlaySfxById(object->character->game_character->s1be,
                    &object->lower_position);
      sfx = object->character->game_character->s1bc;
    } else {
      if ((object->character->model_flags & 8) == 0)
        return;
      sfx = GameAudio->sfx_ids[0xb];
    }
  } else if (type == 2) {
    if (object->character->game_character->s1be == -1)
      return;
    sfx = object->character->game_character->s1be;
  } else if (type == 3) {
    sfx = GameAudio->sfx_ids[0xd];
  } else if (type == 4) {
    if (object->character->game_character->s1c0 != -1)
      sfx = object->character->game_character->s1bc;
    else
      sfx = GameAudio->sfx_ids[0xe];
  } else {
    if (object->is_underwater != 0 || object->intersects_water != 0)
      return;
    i32 alternate = WorldInfo_CurrentlyActive()->current_level->flags & 0x1000;
    if (object->character->model_flags & 0x10) {
      if (alternate != 0)
        sfx = GameAudio->sfx_ids[10];
      else
        sfx = GameAudio->sfx_ids[9];
    } else {
      if (type != 0)
        return;
      if (alternate != 0)
        sfx = GameAudio->sfx_ids[8];
      else if (object->character->game_character->s1ba != -1)
        sfx = object->character->game_character->s1ba;
      else
        sfx = GameAudio->sfx_ids[7];
    }
  }
  if (sfx != -1) {
    if ((object->flags_low & 0x80) || (object->flags140c & 0x80000000))
      PlaySfxById(sfx, &object->lower_position);
    else
      PlaySfxByIdAndSetVolume(sfx, &object->lower_position, 0.5f);
  }
}

// FUNCTION: LEGOBATMAN 0x0059fe80
void PlayGruntSfx(GameObject_s *object) {
  if ((object->flags_low & 0x80) || (object->flags140c & 0x80000000) ||
      object->b254) {
    GruntCharacterData_s *character = object->character;
    GruntGameCharacter_s *game_character = character->game_character;
    i16 configured_sfx = game_character->sfx_grunt;
    i32 sfx;
    if (configured_sfx != -1 &&
        ((game_character->flags13c & 0x20000) ||
         !(object->flags1414 & 0x4000)) &&
        (!(game_character->flags13c & 0x20000) ||
         (object->flags1414 & 0x4000))) {
      sfx = configured_sfx;
    } else if (character->model_flags & 0x40000000) {
      sfx = GameAudio->sfx_ids[0x1f];
    } else {
      if ((character->model_flags & 0x4002010) ||
          (game_character->flags14c & 0x400)) {
        return;
      }
      if (object->flags1414 & 0x4000) {
        sfx = GameAudio->sfx_ids[0x20];
      } else {
        sfx = GameAudio->sfx_ids[0x21];
      }
    }
    if (sfx != -1) {
      PlaySfxById(sfx, &object->pos80);
    }
  }
}

void PlaySfx(char *name, nuvec_s *pos);
i32 GetSfxId(const char *name);

struct RepeatSfx {
  i16 sfx_id;           // 0x0
  u8 state;             // 0x2
  char plays_remaining; // 0x3
  f32 timer;            // 0x4
  f32 interval;         // 0x8
  nuvec_s *position;    // 0xc
};

// GLOBAL: LEGOBATMAN 0x00a95aa4
extern i32 repsfxcount;
// GLOBAL: LEGOBATMAN 0x00a95ab8
extern RepeatSfx repsfxtab[32];

// FUNCTION: LEGOBATMAN 0x005a0590
void PlayRepeatSfx(char *name, i32 sfx_id, f32 initial_delay, char play_count,
                   f32 interval, nuvec_s *position) {
  if (play_count == 1 && initial_delay == 0.0f) {
    if (sfx_id != -1) {
      PlaySfxById(sfx_id, position);
    } else {
      PlaySfx(name, position);
    }
    return;
  }

  if (initial_delay > 0.0f) {
    repsfxtab[repsfxcount].state = 1;
  } else {
    repsfxtab[repsfxcount].state = 2;
  }

  if (sfx_id == -1)
    sfx_id = GetSfxId(name);

  repsfxtab[repsfxcount].sfx_id = (i16)sfx_id;
  repsfxtab[repsfxcount].timer = initial_delay;
  repsfxtab[repsfxcount].plays_remaining = play_count;
  repsfxtab[repsfxcount].interval = interval;
  repsfxtab[repsfxcount].position = position;
  repsfxcount = (repsfxcount + 1) & 31;
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

// FUNCTION: LEGOBATMAN 0x005a2d90
void NewBuzzFrames(nupad_s *pad, i32 frames, i32) {
  if (pad != 0)
    NuSoundAddRumble(pad, (f32)frames / DEFAULTFPS, 0, 0.0f, 0.0f);
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
