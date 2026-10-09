// batman/, file unknown (between LoadPerm2 and Text_LoadStrings).

#include "../nu2api/nucore/nustring.h"
#include <string.h>

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

// name is a Mac pairing hint (order): verify
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

// name is a Mac pairing hint (gapfill): verify
// from saga legoapi/core/input/timer.cpp
// FUNCTION: LEGOBATMAN 0x005a1060
void ResetTimer(TIMER *timer, f32 reset_time) {
  timer->last_time_elapsed = reset_time;
  timer->time_elapsed = reset_time;
  timer->time_elapsed_mod_seconds = NuFmod(reset_time, 1.0f);
  timer->update_count = 0;
}
