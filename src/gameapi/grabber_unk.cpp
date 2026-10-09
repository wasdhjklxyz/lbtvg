// gameapi/grabber_unk.cpp: Grabber_* (saga gizmos_grabber.cpp); file name
// unproven.

#include "../batman/worldinfo_unk.h"
#include "../nu2api/nucore/common.h"

struct GRABBERSYS_s {
  u8 pad000[0x692];
  u8 active; // 0x692
};

// FUNCTION: LEGOBATMAN 0x00601280
i32 Grabber_IsActiveGrabber(GameObject_s *object) {
  GRABBERSYS_s *sys = WorldInfo_CurrentlyActive()->grabber_sys;
  if (sys == 0)
    return -1;
  if ((object->p54->p24->flags148 & 0x8000000) == 0)
    return -1;
  return sys->active != 0;
}

#include "../nu2api/nucore/nustring.h"

typedef struct nufpar_s {
  u8 pad0[0x910];
  char *word_buf; // 0x910
} NUFPAR;

i32 NuFParGetWord(NUFPAR *parser);
i32 NuFParGetInt(NUFPAR *parser);
f32 NuFParGetFloat(NUFPAR *parser);

// The grabber being configured by the Grab_ keyword parsers.
struct GRABBER_s {
  u8 pad0[0x614];
  char grab_sound_effect[0x3c]; // 0x614
  f32 scale;                    // 0x650
  f32 speed;                    // 0x654
  f32 radius;                   // 0x658
  u8 pad65c[0x674 - 0x65c];
  i32 grab_effect; // 0x674
  u8 pad678[0x67c - 0x678];
  i16 character; // 0x67c
  u8 pad67e[0x680 - 0x67e];
  i16 min_rot;     // 0x680
  i16 max_rot;     // 0x682
  i16 rot_min_cur; // 0x684
  i16 rot_max_cur; // 0x686
  u8 pad688[0x68c - 0x688];
  i16 start_ang; // 0x68c
  u8 pad68e[0x68f - 0x68e];
  u8 move;     // 0x68f, 1 = zy, 2 = x
  u8 invert_x; // 0x690
  u8 pad691[0x692 - 0x691];
  u8 active;       // 0x692
  u8 show_grabber; // 0x693
  union {
    u32 flags; // 0x694
    struct {
      u32 flags_lo : 4;
      u32 rotate : 1; // 0x10
    } bits;
  };
  i16 sfx[7]; // 0x698: idle, move, down, grab, drop, up, magnetattach
};

// GLOBAL: LEGOBATMAN 0x00aca69c
extern GRABBER_s *Grab_grabber;

// FUNCTION: LEGOBATMAN 0x006009e0
void Grab_radius(NUFPAR *parser) {
  Grab_grabber->radius = NuFParGetFloat(parser);
}

// FUNCTION: LEGOBATMAN 0x00600a00
void Grab_speed(NUFPAR *parser) {
  Grab_grabber->speed = NuFParGetFloat(parser);
}

// FUNCTION: LEGOBATMAN 0x00600a20
void Grab_rotate(NUFPAR *parser) {
  Grab_grabber->bits.rotate = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x00600a50
void Grab_scale(NUFPAR *parser) {
  Grab_grabber->scale = NuFParGetFloat(parser);
}

// FUNCTION: LEGOBATMAN 0x00600a70
void Grab_move_zy(NUFPAR *parser) { Grab_grabber->move = 1; }

// FUNCTION: LEGOBATMAN 0x00600a80
void Grab_move_x(NUFPAR *parser) { Grab_grabber->move = 2; }

// FUNCTION: LEGOBATMAN 0x00600a90
void Grab_invert_x(NUFPAR *parser) { Grab_grabber->invert_x = 1; }

// FUNCTION: LEGOBATMAN 0x00600ae0
void Grab_shadow(NUFPAR *parser) {
  Grab_grabber->flags |= 0x20;
  if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0)
    Grab_grabber->flags &= ~0x20;
}

// FUNCTION: LEGOBATMAN 0x00600c40
void Grab_hide_grabber(NUFPAR *parser) { Grab_grabber->show_grabber = 0; }

// FUNCTION: LEGOBATMAN 0x00600c60
void Grab_can_bounce_heavy(NUFPAR *parser) { Grab_grabber->flags |= 0x100; }

// FUNCTION: LEGOBATMAN 0x00600c70
void Grab_seek_to_plug(NUFPAR *parser) { Grab_grabber->flags |= 0x80; }

// FUNCTION: LEGOBATMAN 0x00600c80
void Grab_magnet(NUFPAR *parser) { Grab_grabber->flags |= 0x800; }

// FUNCTION: LEGOBATMAN 0x00600c90
void Grab_crane(NUFPAR *parser) { Grab_grabber->flags |= 0x200; }

// FUNCTION: LEGOBATMAN 0x00600ca0
void Grab_not_active(NUFPAR *parser) { Grab_grabber->active = 0; }

// GLOBAL: LEGOBATMAN 0x00ad6900
extern i32 (*g_unk00ad6900)(char *name);

WORLDINFO_s *WorldInfo_CurrentlyLoading(void);
i32 FindGameDebris(void *debris_sys, char *name);
int NuAToI(const char *s);

// FUNCTION: LEGOBATMAN 0x00600aa0
void Grab_character(NUFPAR *parser) {
  if (NuFParGetWord(parser) != 0)
    Grab_grabber->character = g_unk00ad6900(parser->word_buf);
}

// FUNCTION: LEGOBATMAN 0x00600b30
void Grab_minrot(NUFPAR *parser) {
  if (NuFParGetWord(parser) != 0) {
    Grab_grabber->min_rot = (NuAToI(parser->word_buf) << 16) / 360;
    Grab_grabber->rot_min_cur = Grab_grabber->min_rot;
  }
}

// FUNCTION: LEGOBATMAN 0x00600b90
void Grab_maxrot(NUFPAR *parser) {
  if (NuFParGetWord(parser) != 0) {
    Grab_grabber->max_rot = (NuAToI(parser->word_buf) << 16) / 360;
    Grab_grabber->rot_max_cur = Grab_grabber->max_rot;
  }
}

// FUNCTION: LEGOBATMAN 0x00600bf0
void Grab_start_ang(NUFPAR *parser) {
  if (NuFParGetWord(parser) != 0)
    Grab_grabber->start_ang = (NuAToI(parser->word_buf) << 16) / 360;
}

// FUNCTION: LEGOBATMAN 0x00600c50
void Grab_can_bounce(NUFPAR *parser) { Grab_grabber->flags |= 0x40; }

// FUNCTION: LEGOBATMAN 0x00600cb0
void Grab_swapblowup(NUFPAR *parser) { Grab_grabber->flags |= 0x400; }

// FUNCTION: LEGOBATMAN 0x00600cc0
void Grab_grabeffect(NUFPAR *parser) {
  if (NuFParGetWord(parser) != 0)
    Grab_grabber->grab_effect =
        FindGameDebris(WorldInfo_CurrentlyLoading()->p138, parser->word_buf);
}

// FUNCTION: LEGOBATMAN 0x00600d00
void Grab_grabsoundeffect(NUFPAR *parser) {
  if (NuFParGetWord(parser) != 0)
    NuStrCpy(Grab_grabber->grab_sound_effect, parser->word_buf);
}

i32 GetSfxId(char *name);

// FUNCTION: LEGOBATMAN 0x00600d30
void Grab_sfx(NUFPAR *parser) {
  i16 *sfx = 0;
  if (NuFParGetWord(parser) != 0) {
    if (NuStrICmp(parser->word_buf, "idle") == 0)
      sfx = &Grab_grabber->sfx[0];
    else if (NuStrICmp(parser->word_buf, "move") == 0)
      sfx = &Grab_grabber->sfx[1];
    else if (NuStrICmp(parser->word_buf, "down") == 0)
      sfx = &Grab_grabber->sfx[2];
    else if (NuStrICmp(parser->word_buf, "grab") == 0)
      sfx = &Grab_grabber->sfx[3];
    else if (NuStrICmp(parser->word_buf, "drop") == 0)
      sfx = &Grab_grabber->sfx[4];
    else if (NuStrICmp(parser->word_buf, "up") == 0)
      sfx = &Grab_grabber->sfx[5];
    else if (NuStrICmp(parser->word_buf, "magnetattach") == 0)
      sfx = &Grab_grabber->sfx[6];
    if (sfx != 0 && NuFParGetWord(parser) != 0)
      *sfx = GetSfxId(parser->word_buf);
  }
}

// FUNCTION: LEGOBATMAN 0x00600e80
void Grab_drop_when_disabled(NUFPAR *parser) { Grab_grabber->flags |= 0x1000; }
