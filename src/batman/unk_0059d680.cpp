// batman/unk_0059d680.cpp: TU of unknown name, found by its header-static
// copies (the functions after them are not matched yet).

#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"

// FUNCTION: LEGOBATMAN 0x0059d680
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_0059d680(f32 *v, f32 a, i32 i) { NuVec4Set(v, a, a, a, a); }

// The rest of this TU is saga legoapi/menus/core/text.cpp (Text_LoadFont
// onwards are in text_unk.cpp for now).

#include <stddef.h>

typedef struct LANGUAGEDATA_s {
  i32 language;
  char *name;
} LANGUAGEDATA;

// GLOBAL: LEGOBATMAN 0x0095eb90
extern LANGUAGEDATA *Text_LanguageList;
// GLOBAL: LEGOBATMAN 0x0095eb94
extern i32 LANGUAGECOUNT;

// from saga legoapi/menus/core/text.cpp
// FUNCTION: LEGOBATMAN 0x0059d6a0
void Text_InitLanguageList(LANGUAGEDATA *language_list) {
  if (language_list != NULL)
    Text_LanguageList = language_list;

  LANGUAGECOUNT = 0;
  while (Text_LanguageList[LANGUAGECOUNT].language != -1)
    ++LANGUAGECOUNT;
}

extern i32 Text_Language;
// GLOBAL: LEGOBATMAN 0x00a95884
extern void (*Text_GameSetLanguageFn)(i32 language);
i32 NuLanguageGet(void);

// Batman's form: the language comes from NuLanguageGet.
// FUNCTION: LEGOBATMAN 0x0059d6e0
void Text_SetLanguage(void) {
  Text_Language = NuLanguageGet();
  if (Text_GameSetLanguageFn != NULL)
    Text_GameSetLanguageFn(Text_Language);
}

// GLOBAL: LEGOBATMAN 0x0095eb70
extern char *txtpath_ENGLISH;
// GLOBAL: LEGOBATMAN 0x0095eb74
extern char *txtpath_eb74;
// GLOBAL: LEGOBATMAN 0x0095eb78
extern char *txtpath_eb78;
// GLOBAL: LEGOBATMAN 0x0095eb7c
extern char *txtpath_eb7c;
// GLOBAL: LEGOBATMAN 0x0095eb80
extern char *txtpath_eb80;
// GLOBAL: LEGOBATMAN 0x0095eb84
extern char *txtpath_eb84;
// GLOBAL: LEGOBATMAN 0x0095eb88
extern char *txtpath_eb88;

// saga legoapi/menus/core/text.cpp; case order from the jump targets
// FUNCTION: LEGOBATMAN 0x0059d700
char *Text_GetLanguagePath(i32 language) {
  switch (language) {
  case 5:
    return txtpath_eb74;
  case 4:
    return txtpath_eb78;
  case 7:
    return txtpath_eb7c;
  case 6:
    return txtpath_eb80;
  case 10:
    return txtpath_eb84;
  case 0:
    return txtpath_eb88;
  default:
    return txtpath_ENGLISH;
  }
}
