// batman/, file unknown (between LoadPerm2 and Text_LoadStrings).

#include "../nu2api/nucore/nustring.h"
#include <stdio.h>
#include <string.h>

extern char **TTab;
char *getbutton(i32 button, char *a, char *b);

// FUNCTION: LEGOBATMAN 0x00408e60
i32 Text_ExpandButtonString(char *input, char *output) {
  if (NuStrICmp(input, "[TAG]") == 0 || NuStrICmp(input, "[TRIANGLE]") == 0 ||
      NuStrICmp(input, "[T]") == 0) {
    NuStrCpy(output, getbutton(0xc, TTab[0x330], TTab[0x331]));
    return 1;
  }
  if (NuStrICmp(input, "[ACTION]") == 0 || NuStrICmp(input, "[SQUARE]") == 0 ||
      NuStrICmp(input, "[S]") == 0) {
    NuStrCpy(output, getbutton(0xf, TTab[0x330], TTab[0x331]));
    return 1;
  }
  if (NuStrICmp(input, "[SPECIAL]") == 0 || NuStrICmp(input, "[CIRCLE]") == 0 ||
      NuStrICmp(input, "[O]") == 0) {
    NuStrCpy(output, getbutton(0xd, TTab[0x330], TTab[0x331]));
    return 1;
  }
  if (NuStrICmp(input, "[JUMP]") == 0 || NuStrICmp(input, "[CROSS]") == 0 ||
      NuStrICmp(input, "[X]") == 0) {
    NuStrCpy(output, getbutton(0xe, TTab[0x330], TTab[0x331]));
    return 1;
  }
  if (NuStrICmp(input, "[TOGGLELEFT]") == 0) {
    NuStrCpy(output, getbutton(0x10, TTab[0x330], TTab[0x331]));
    return 1;
  }
  if (NuStrICmp(input, "[TOGGLERIGHT]") == 0) {
    NuStrCpy(output, getbutton(0x12, TTab[0x330], TTab[0x331]));
    return 1;
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x004f9a60
void Text_DecodeButtons(char *in, char *out) {
  if (!NuStrICmp(in, "wibble"))
    NuStrCpy(out, "cross");
}

// GLOBAL: LEGOBATMAN 0x00a957f4
extern char **TTab_Original;
// GLOBAL: LEGOBATMAN 0x00a95810
extern i32 Text_MaxStrings_Overall;
// GLOBAL: LEGOBATMAN 0x00a957f8
extern char **TTab;
// GLOBAL: LEGOBATMAN 0x00a95814
extern u32 *Text_StringBits;
// GLOBAL: LEGOBATMAN 0x0095eb64
extern char *Text_ErrString;

typedef struct vufnt_s VUFNT;

// GLOBAL: LEGOBATMAN 0x00ad745c
extern i32 create_qfont3dz;
// GLOBAL: LEGOBATMAN 0x00a957f0
extern VUFNT *app_fnt;

VUFNT *LoadGameFont(char *path, char *name, variptr_u *buf, variptr_u *buf_end,
                    i32 render_plane);
VUFNT *LoadButtonFont(char *path, char *name, variptr_u *buf,
                      variptr_u *buf_end, i32 render_plane);

void TextRegisterButtonMapFn(void (*fn)(char *, char *));
extern i32 Text_Language;
char *Text_GetLanguagePath(i32 language);
void Text_LoadAndFixUpStrings(unsigned char *filename, unsigned char **buffer,
                              char **table, i32 count);
int NuPadUnk006d6850(void);
void MenuLoadTechnicalStrings(char *filepath, char *language, variptr_u *buf,
                              variptr_u buf_end);

// from saga legoapi/menus/core/text.cpp
// FUNCTION: LEGOBATMAN 0x004f9ab0
void Text_LoadStrings(variptr_u *buf, variptr_u *buf_end) {
  unsigned char *string_buffer;
  char language[32];
  char path[256];

  TextRegisterButtonMapFn(Text_DecodeButtons);
  NuStrCpy(language, Text_GetLanguagePath(Text_Language));
  NuStrCpy(path, "stuff\\text\\");
  NuStrCat(path, language);
  NuStrCat(path, ".txt");
  string_buffer = buf->u8_ptr;
  Text_LoadAndFixUpStrings((unsigned char *)path, &string_buffer, TTab, 1000);
  buf->u8_ptr = string_buffer;
  if (Text_Language == 3 && NuPadUnk006d6850() == 2) {
    string_buffer = buf->u8_ptr;
    Text_LoadAndFixUpStrings((unsigned char *)"stuff\\text\\american.txt",
                             &string_buffer, TTab, 1000);
    buf->u8_ptr = string_buffer;
    NuStrCpy(language, "american");
  }
  buf->addr = ((u32)string_buffer + 3) & ~3;
  MenuLoadTechnicalStrings("stuff\\text\\trc.csv", language, buf, *buf_end);
}

// FUNCTION: LEGOBATMAN 0x0059d770
void Text_LoadFont(char *path, variptr_u *buf, variptr_u *buf_end) {
  create_qfont3dz = 1;
  app_fnt = LoadGameFont(path, path, buf, buf_end, 1);
  LoadButtonFont("stuff\\text\\buttons_PC", 0, buf, buf_end, 0);
}

// STUB: LEGOBATMAN 0x0059d7c0
// close: register allocation only; orig aligns into eax and copies to esi
// for the table pointer, this aligns straight into esi.
void Text_InitStringTable(i32 count, variptr_u *buf, variptr_u *) {
  u32 addr = (buf->addr + 3) & ~3;
  u32 table_size = (count + 1) * sizeof(char *);
  char **tab = (char **)addr;
  buf->addr = (addr + table_size + 3) & ~3;
  TTab_Original = tab;
  memset(tab, 0, table_size);
  tab[0] = Text_ErrString;
  TTab = tab + 1;

  u32 *bits = buf->u32_ptr;
  i32 flags_size = (count + 31) / 32 * (i32)sizeof(u32);
  Text_MaxStrings_Overall = count;
  Text_StringBits = bits;
  memset(bits, 0, flags_size);
  buf->addr += flags_size;
}

#include "../nu2api/numath/nuvec.h"

struct TIMER_s {
  union {
    struct {
      f32 time_elapsed;
      f32 last_time_elapsed;
      f32 time_elapsed_mod_seconds;
    };
    NUVEC elapsed_components;
  };
  i32 update_count;
};
typedef struct TIMER_s TIMER;

f32 NuFmod(f32 a, f32 b);

void Text_InsertCommasIntoNumber(char *number, char *text, i32 length);

// GLOBAL: LEGOBATMAN 0x0095eb8c
extern i32 Text_Language;

struct TEXTENTRY {
  i16 *text_id;
  i16 value;
  i16 pad;
};

// FUNCTION: LEGOBATMAN 0x0059db20
void Text_InitTable(TEXTENTRY *entry, i32 first, i32 last) {
  i32 index = 0;
  if (entry == 0)
    return;

  u32 *bits = Text_StringBits;
  do {
    if (entry->text_id != 0 && entry->value != -1) {
      index = entry->value;
      if (index >= first && index <= last) {
        entry->value = index;
        *entry->text_id = index;
      }
      index++;
      entry++;
    }
    while (entry->text_id != 0 && entry->value == -1) {
      if (index >= first && index <= last) {
        entry->value = index;
        *entry->text_id = index;
        bits[index / 32] |= 1 << (index & 0x1f);
      } else {
        entry->value = 0;
        *entry->text_id = 0;
      }
      entry++;
      index++;
    }
  } while (entry->text_id != 0);
}

// FUNCTION: LEGOBATMAN 0x0059dbd0
void Text_InsertCommasIntoNumber(char *number, char *text, i32 length) {
  char separator = ',';
  if (Text_Language == 5 || (Text_Language != 2 && Text_Language != 3)) {
    separator = '.';
  }

  i32 count = length < 0 ? NuStrLen(number) : length;

  i32 output = 0;
  for (i32 digit = 0; digit < count; ++digit) {
    text[output++] = number[digit];
    const i32 remaining = count - digit - 1;
    if (remaining != 0 && remaining % 3 == 0) {
      text[output++] = separator;
    }
  }
  text[output] = '\0';
}

// from saga legoapi/menus/core/text.cpp
// FUNCTION: LEGOBATMAN 0x0059dc80
void Text_MakeScore(u32 score, char *text) {
  char digits[64];
  char *end = &digits[63];
  char *first = end - 1;
  *end = '\0';
  *first = static_cast<char>('0' + score % 10);
  score /= 10;
  while (score != 0) {
    *--first = static_cast<char>('0' + score % 10);
    score /= 10;
  }

  Text_InsertCommasIntoNumber(first, text, static_cast<i32>(end - first));
}

f32 NuFmod(f32 a, f32 b);

// FUNCTION: LEGOBATMAN 0x0059dd10
void Text_MakeTime(float time, i32 show_hours, i32 show_minutes,
                   i32 show_centiseconds, char *text) {
  if (time < 0.0f)
    time = 0.0f;

  i32 hours;
  i32 minutes;
  if (show_hours != 0) {
    hours = static_cast<i32>(time / 3600.0f);
    minutes = static_cast<i32>(NuFmod(time / 60.0f, 60.0f));
  } else {
    hours = 0;
    minutes = static_cast<i32>(time / 60.0f);
  }

  i32 seconds;
  if (show_hours != 0 || show_minutes != 0) {
    seconds = static_cast<i32>(NuFmod(time, 60.0f));
  } else {
    seconds = static_cast<i32>(time);
  }
  const i32 centiseconds = static_cast<i32>(NuFmod(time, 1.0f) * 100.0f);

  if (text == 0) {
    return;
  }

  if (show_hours != 0) {
    if (show_centiseconds != 0) {
      sprintf(text, "%i:%.2i:%.2i.%.2i", hours, minutes, seconds, centiseconds);
    } else {
      sprintf(text, "%i:%.2i:%.2i", hours, minutes, seconds);
    }
  } else if (show_minutes != 0) {
    if (show_centiseconds != 0) {
      sprintf(text, "%i:%.2i.%.2i", minutes, seconds, centiseconds);
    } else {
      sprintf(text, "%i:%.2i", minutes, seconds);
    }
  } else if (show_centiseconds != 0) {
    sprintf(text, "%i.%.2i", seconds, centiseconds);
  } else {
    sprintf(text, "%i", seconds);
  }
}

// from saga legoapi/core/input/timer.cpp
// FUNCTION: LEGOBATMAN 0x005a1060
void ResetTimer(TIMER *timer, f32 reset_time) {
  timer->last_time_elapsed = reset_time;
  timer->time_elapsed = reset_time;
  timer->time_elapsed_mod_seconds = NuFmod(reset_time, 1.0f);
  timer->update_count = 0;
}
