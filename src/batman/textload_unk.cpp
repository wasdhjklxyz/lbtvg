// batman/, file unknown: text string loader (0x0059d850..). Kept apart from
// text_unk.cpp: defining Text_LoadAndFixUpStrings next to its caller
// Text_LoadStrings changes the caller's register allocation, so they are
// separate TUs in the original.

#include "../nu2api/nucore/nustring.h"

typedef struct nufpar_s {
  u32 pad0[0x910 / 4];
  char *word_buf; // 0x910
  u32 pad914[(0x978 - 0x914) / 4];
  char is_utf16; // 0x978
  char is_utf8;  // 0x979
} NUFPAR;

NUFPAR *NuFParCreate(char *filename);
i32 NuFParGetLine(NUFPAR *parser);
i32 NuFParGetLineW(NUFPAR *parser);
i32 NuFParGetWord(NUFPAR *parser);
i32 NuFParGetWordW(NUFPAR *parser);
i32 NuFParGetInt(NUFPAR *parser);
void NuFParDestroy(NUFPAR *parser);
int NuStrLenW(const unsigned short *s);
unsigned char *NuPadUtf8Encode(unsigned char *dst, unsigned short c);

struct ANIMPACKET_s {
  union {
    f32 current_time;
    f32 field_0x00;
  };
  union {
    f32 previous_time;
    f32 field_0x04;
  };
  f32 blend_elapsed;  // 0x08
  f32 blend_duration; // 0x0c
  union {
    f32 blend_source_time;
    f32 time;
  };
  union {
    f32 blend_target_time;
    f32 time2;
  };
  u8 pad_0x18[0x20 - 0x18];
  union {
    f32 field_0x20;
    f32 time_secondary;
  }; // 0x20
  u8 pad_0x24[0x30 - 0x24];
  union {
    u8 flags;
    u8 field_0x30;
  };
  u8 blending;           // 0x31
  i16 blend_animation_a; // 0x32
  i16 blend_animation_b; // 0x34
  i16 animation_index;   // 0x36
  union {
    i16 previous_animation;
    i16 field_0x38;
  };
  union {
    i16 requested_animation;
    i16 field_0x3a;
  };
  u8 blend_source_reversed; // 0x3c
  u8 blend_target_reversed; // 0x3d
  u8 current_reversed;      // 0x3e
  u8 pad_0x3f[0x42 - 0x3f];
  union {
    i16 overlay_animation; // -1 when no overlay is active
    u16 frame;
  }; // 0x42
  f32 field_0x44; // 0x44
};

struct CHARACTERANIM_s {
  char *name; // 0x00
  u32 pad04;
  i16 animation_id; // 0x08
  u16 pad0a;
  u32 pad0c[(0x20 - 0x0c) / 4];
  f32 action_speed; // 0x20
  u32 pad24;
};

struct CHARACTERDATA {
  u32 pad00[4];
  CHARACTERANIM_s *animations; // 0x10
};

// FUNCTION: LEGOBATMAN 0x0059a0d0
i16 FindAnimIX(CHARACTERDATA *character, char *name) {
  if (character != 0) {
    CHARACTERANIM_s *animation = character->animations;
    while (animation != 0 && animation->name != 0) {
      if (NuStrICmp(name, animation->name) == 0)
        return animation->animation_id;
      ++animation;
    }
  }
  return -1;
}

// FUNCTION: LEGOBATMAN 0x0059a730
void ResetAnimPacket(ANIMPACKET_s *packet, i32 animation) {
  if (packet == 0)
    return;
  packet->requested_animation = animation;
  packet->previous_animation = packet->requested_animation;
  packet->animation_index = packet->previous_animation;
  packet->previous_time = 1.0f;
  packet->blend_target_time = packet->previous_time;
  packet->current_time = packet->blend_target_time;
  packet->blending = 0;
  packet->flags = 4;
  packet->overlay_animation = -1;
  packet->current_reversed = 0;
  packet->blend_source_reversed = 0;
  packet->blend_target_reversed = 0;
}

// FUNCTION: LEGOBATMAN 0x0059b3c0
f32 *AnimPlaying(ANIMPACKET_s *packet, i32 animation, i32 target, i32 source) {
  if (animation == -1)
    return 0;
  if (packet->blending != 0) {
    if (target != 0 && packet->blend_animation_b == animation)
      return &packet->blend_target_time;
    if (source != 0 && packet->blend_animation_a == animation)
      return &packet->blend_source_time;
  } else {
    if (packet->animation_index == animation)
      return &packet->current_time;
  }
  return 0;
}

// from saga legoapi/items/base/apiobject.cpp
// FUNCTION: LEGOBATMAN 0x0059b480
i32 CurrentAnim(ANIMPACKET_s *packet) {
  if (packet->blending != 0)
    return packet->blend_animation_b;
  return packet->animation_index;
}

struct CHARACTERMODEL_s {
  u32 pad00[2];
  void **model_data_a; // 0x08
  void **model_data_b; // 0x0c
};

// FUNCTION: LEGOBATMAN 0x0059b540
f32 AnimSpeedZ(CHARACTERMODEL_s *model, i32 animation) {
  if (animation != -1 && model->model_data_b[animation] != 0)
    return ((CHARACTERANIM_s *)model->model_data_a[animation])->action_speed;
  return 0.0f;
}

// STUB: LEGOBATMAN 0x0059d850
// close: register-arg static (parser in esi, matches); only the final "wii"
// test differs: orig branches to the shared `return 1`, ours if-converts it.
static i32 Text_PlatformSpecificIgnore(NUFPAR *parser) {
  if (NuStrICmp(parser->word_buf, "360") == 0)
    return 1;
  if (NuStrICmp(parser->word_buf, "ps2") == 0)
    return 1;
  if (NuStrICmp(parser->word_buf, "ps3") == 0)
    return 1;
  if (NuStrICmp(parser->word_buf, "psp") == 0)
    return 1;
  if (NuStrICmp(parser->word_buf, "pc") == 0) {
    NuFParGetWord(parser);
    return 0;
  }
  if (NuStrICmp(parser->word_buf, "wii") == 0)
    return 1;
  return 0;
}

// STUB: LEGOBATMAN 0x0059d900
// close: ecx/eax picks for the *buffer stores, NuStrLen result move, and the
// u8 -> u16 -> int double movzx before NuPadUtf8Encode.
void Text_LoadAndFixUpStrings(unsigned char *filename, unsigned char **buffer,
                              char **table, i32 count) {
  unsigned char *out = *buffer;
  NUFPAR *parser = NuFParCreate((char *)filename);
  if (parser != 0) {
    if (parser->is_utf16 != 0) {
      while (NuFParGetLineW(parser) != 0) {
        i32 index = NuFParGetInt(parser);
        if (index <= 0 || index >= count)
          continue;
        NuFParGetWordW(parser);
        unsigned short *wide = (unsigned short *)parser->word_buf;
        i32 length = NuStrLenW(wide);
        if (length <= 0)
          continue;
        table[index] = (char *)out;
        for (; length > 0; length--)
          out = NuPadUtf8Encode(out, *wide++);
        *out++ = 0;
      }
    } else if (parser->is_utf8 != 0) {
      while (NuFParGetLine(parser) != 0) {
        NuFParGetWord(parser);
        if (parser->word_buf[0] == 0)
          continue;
        i32 index = NuAToI(parser->word_buf);
        if (index <= 0 || index >= count)
          continue;
        NuFParGetWord(parser);
        if (Text_PlatformSpecificIgnore(parser))
          continue;
        i32 length = NuStrLen(parser->word_buf);
        table[index] = (char *)out;
        NuStrCpy((char *)out, parser->word_buf);
        out += length + 1;
      }
    } else {
      while (NuFParGetLine(parser) != 0) {
        i32 index = NuFParGetInt(parser);
        if (index <= 0 || index >= count)
          continue;
        NuFParGetWord(parser);
        if (Text_PlatformSpecificIgnore(parser))
          continue;
        char *word = parser->word_buf;
        i32 length = NuStrLen(word);
        if (length <= 0)
          continue;
        table[index] = (char *)out;
        for (; length > 0; length--, word++) {
          u8 character = (u8)*word;
          if (character < 0x80)
            *out++ = character;
          else
            out = NuPadUtf8Encode(out, character);
        }
        *out++ = 0;
      }
    }
    NuFParDestroy(parser);
  }
  *buffer = out;
}
