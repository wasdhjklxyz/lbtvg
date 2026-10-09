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
  u8 pad0[0x650];
  f32 scale;  // 0x650
  f32 speed;  // 0x654
  f32 radius; // 0x658
  u8 pad65c[0x68f - 0x65c];
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
