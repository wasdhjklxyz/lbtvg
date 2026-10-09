// gameapi/minikits_unk.cpp: placed by tools/new.py; file name unproven.
// from saga legoapi/items/collect/minikits.cpp

#include "../nu2api/nucore/common.h"
#include <stddef.h>

#include "../nu2api/numath/nuvec.h"

#include "../nu2api/nu3d/nuspecial.h"

struct GAMEMESSAGE_s {
  char *text;
  char text_buffer[0x78];
  NUVEC position_a; // 0x7c
  NUVEC target_position;
  NUVEC position;
  NUVEC start_position;
  f32 field_0xac;
  f32 target_scale;
  f32 field_0xb4;
  f32 field_0xb8;
  f32 elapsed;
  f32 duration;
  f32 field_0xc4;
  f32 field_0xc8;
  f32 field_0xcc;
  f32 field_0xd0;
  f32 field_0xd4;
  u32 flags;
  u32 score;
  u16 field_0xe0;
  u16 rotation_y;
  u16 field_0xe4;
  u16 icon;
  union {
    NUVEC color;
    nuhspecial_s
        special; // model messages store a handle, not three float coordinates
    struct {
      u32 color1;
      u32 color2;
      u32 color3;
    };
  };
  u8 red;
  u8 green;
  u8 blue;
  u8 alpha;
  u8 active;
  u8 field_0xf9;
  u8 field_0xfa;
  u8 field_0xfb;
  u8 field_0xfc;
  i8 player_index;
  union {
    u8 field_0xfe;
    u8 target_type;
  };
  u8 field_0xff;
  void (*delay_fn)(GAMEMESSAGE_s *);
  void (*tick_fn)(GAMEMESSAGE_s *);
  void (*update_fn)(GAMEMESSAGE_s *);
  void (*draw_callback)(GAMEMESSAGE_s *, NUVEC *, f32);
  void (*end_fn)(GAMEMESSAGE_s *);
};

struct ADDGAMEMSG {
  char *text;               // 0x00
  nuvec_s *position;        // 0x04
  nuvec_s *target_position; // 0x08
  f32 scale;                // 0x0c
  f32 target_scale;         // 0x10
  u8 red;                   // 0x14
  u8 green;                 // 0x15
  u8 blue;                  // 0x16
  u8 alpha;                 // 0x17
  u32 flags;                // 0x18
  f32 duration;             // 0x1c
  f32 field_0x20;           // 0x20
  u16 field_0x24;           // 0x24
  i16 icon;                 // 0x26
  union {
    nuvec_s *extra_position;
    nuhspecial_s *special;
  }; // 0x28
  u32 score;                         // 0x2c
  f32 field_0x30;                    // 0x30
  f32 field_0x34;                    // 0x34
  void (*delay_fn)(GAMEMESSAGE_s *); // 0x38, called when the delay expires
  void (*tick_fn)(GAMEMESSAGE_s *);  // 0x3c, called by UpdateGameMessages
  void (*update_fn)(
      GAMEMESSAGE_s *);            // 0x40, position adjustment during drawing
  void *field_0x44;                // 0x44
  void (*end_fn)(GAMEMESSAGE_s *); // 0x48
  i8 player_index;                 // 0x4c
  u8 field_0x4d;
  u8 field_0x4e;
  u8 field_0x4f;
};

ADDGAMEMSG AddGameMsg_Default;

char *LEGOASCII_BIGARROW = NULL;

char *txt_UNKNOWN;

void GameMsg_Draw_MiniKitDetector(GAMEMESSAGE_s *message, nuvec_s *position,
                                  float scale);

GAMEMESSAGE_s *AddGameMsg(ADDGAMEMSG *message);

// FUNCTION: LEGOBATMAN 0x00635ab0
void MiniKitDetector(nuvec_s *position) {
  ADDGAMEMSG message = AddGameMsg_Default;
  message.text = LEGOASCII_BIGARROW != NULL ? LEGOASCII_BIGARROW : txt_UNKNOWN;
  message.position = position;
  message.flags = 0x40083;
  message.scale = 0.6f;
  message.field_0x4f = 4;
  message.field_0x44 = reinterpret_cast<void *>(GameMsg_Draw_MiniKitDetector);
  AddGameMsg(&message);
}
