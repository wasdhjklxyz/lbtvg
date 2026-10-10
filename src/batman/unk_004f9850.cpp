// batman/unk_004f9850.cpp: TU of unknown name, found by its header-static
// copies (the functions after them are not matched yet).

#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"

// FUNCTION: LEGOBATMAN 0x004f9850
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_004f9850(f32 *v, f32 a, i32 i) { NuVec4Set(v, a, a, a, a); }

// Batman's text.cpp game half (saga legoapi/menus/core/text.cpp):
// Text_SetLanguage_Game, Text_InitDefaultStrings, then Text_DecodeButtons.

// GLOBAL: LEGOBATMAN 0x0095eba0
extern f32 INTROTEXT_Y;
// GLOBAL: LEGOBATMAN 0x0095eb9c
extern f32 INTROTEXT_SCALE;

// saga's table with Batman's language numbers; case order from the jump
// targets.
// FUNCTION: LEGOBATMAN 0x004f9880
void Text_SetLanguage_Game(i32 language) {
  switch (language) {
  case 4:
    INTROTEXT_Y = 0.13f;
    INTROTEXT_SCALE = 0.575f;
    break;
  case 5:
    INTROTEXT_Y = 0.115f;
    INTROTEXT_SCALE = 0.5f;
    break;
  case 6:
    INTROTEXT_Y = 0.14f;
    INTROTEXT_SCALE = 0.61f;
    break;
  case 10:
    INTROTEXT_Y = 0.15f;
    INTROTEXT_SCALE = 0.67f;
    break;
  case 7:
    INTROTEXT_Y = 0.17f;
    INTROTEXT_SCALE = 0.76f;
    break;
  default:
    INTROTEXT_Y = 0.175f;
    INTROTEXT_SCALE = 0.79f;
    break;
  }
}

// GLOBAL: LEGOBATMAN 0x00a957f8
extern char **TTab;
// GLOBAL: LEGOBATMAN 0x009cd6c4
extern i16 tNULL;
// GLOBAL: LEGOBATMAN 0x009cd6c8
extern i16 tUNKNOWN;
// GLOBAL: LEGOBATMAN 0x009cd6cc
extern i16 g_unk009cd6cc;
// GLOBAL: LEGOBATMAN 0x009cd6d0
extern i16 g_unk009cd6d0;
// GLOBAL: LEGOBATMAN 0x009cd6d4
extern i16 g_unk009cd6d4;
// GLOBAL: LEGOBATMAN 0x009cd6d8
extern i16 g_unk009cd6d8;
// GLOBAL: LEGOBATMAN 0x009cd6dc
extern i16 g_unk009cd6dc;
// GLOBAL: LEGOBATMAN 0x009cd6e0
extern i16 g_unk009cd6e0;
// GLOBAL: LEGOBATMAN 0x0095eb68
extern char *txt_NULL;
// GLOBAL: LEGOBATMAN 0x0095eb6c
extern char *txt_UNKNOWN;
// GLOBAL: LEGOBATMAN 0x0099e3e8
extern char *g_unk0099e3e8;
// GLOBAL: LEGOBATMAN 0x0099e3ec
extern char *g_unk0099e3ec;
// GLOBAL: LEGOBATMAN 0x0099e3f0
extern char *g_unk0099e3f0;
// GLOBAL: LEGOBATMAN 0x0099e3f4
extern char *g_unk0099e3f4;
// GLOBAL: LEGOBATMAN 0x0099e3f8
extern char *g_unk0099e3f8;
// GLOBAL: LEGOBATMAN 0x0099e3fc
extern char *g_unk0099e3fc;

// from saga legoapi/menus/core/text.cpp (the language names are the six
// after tUNKNOWN)
// FUNCTION: LEGOBATMAN 0x004f9980
void Text_InitDefaultStrings(void) {
  TTab[tNULL] = txt_NULL;
  TTab[tUNKNOWN] = txt_UNKNOWN;
  TTab[g_unk009cd6cc] = g_unk0099e3e8;
  TTab[g_unk009cd6d0] = g_unk0099e3ec;
  TTab[g_unk009cd6d4] = g_unk0099e3f0;
  TTab[g_unk009cd6d8] = g_unk0099e3f4;
  TTab[g_unk009cd6dc] = g_unk0099e3f8;
  TTab[g_unk009cd6e0] = g_unk0099e3fc;
}
