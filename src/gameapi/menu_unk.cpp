// gameapi/menu_unk.cpp: menu system setup (saga legoapi/menus/screens/
// gamemenuall.cpp); after AIBugPit.cpp, before apisave.c. File name unproven.

#include "../nu2api/nucore/common.h"
#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"
#include <stdio.h>
#include <string.h>

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x006bec30
static f32 NuSinApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x006becd0
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

typedef struct MENUFNINFO_s {
  u32 data[7];
} MENUFNINFO;

typedef struct numtl_s {
  u8 pad0[0x40];
  u32 filter_mode : 4; // 0x40
  u32 alpha_mode : 2;
  u32 attrib6 : 2;
  u32 attrib8 : 2;
  u32 attrib10 : 2;
  u32 attrib12 : 2;
  u32 z_mode : 2;
  u32 attrib16 : 2;
  u32 attrib18 : 1;
  u32 attrib19 : 13;
} NUMTL;

// GLOBAL: LEGOBATMAN 0x0099e4e0
extern MENUFNINFO MenuInfo[0x64]; // first 0x1f reserved
// GLOBAL: LEGOBATMAN 0x0099e38c
extern i32 MenusUsed;
// GLOBAL: LEGOBATMAN 0x0099e388
extern i32 MenuLanguages;
// GLOBAL: LEGOBATMAN 0x00ad7352
extern u8 g_unk00ad7352;
// GLOBAL: LEGOBATMAN 0x00ad69cc
extern i32 g_unk00ad69cc;
// GLOBAL: LEGOBATMAN 0x00ad6988
extern char MenuHeader[];
// GLOBAL: LEGOBATMAN 0x0099e3d4
extern u8 MENUHEADERR;
// GLOBAL: LEGOBATMAN 0x0099e3d5
extern u8 MENUHEADERG;
// GLOBAL: LEGOBATMAN 0x00ad6978
extern i32 header_r;
// GLOBAL: LEGOBATMAN 0x00ad6984
extern i32 header_g;
// GLOBAL: LEGOBATMAN 0x0099e3a8
extern void (*drawslotinfofn)(f32, f32, i32, i32);
// GLOBAL: LEGOBATMAN 0x00ad7308
extern i32 MenuFadeEnabled;
// GLOBAL: LEGOBATMAN 0x00ad731c
extern i32 MenuDrawDropShadows;
// GLOBAL: LEGOBATMAN 0x00ad7330
extern NUMTL *MenuFadeMtl;

NUMTL *NuMtlCreate(i32 count);
void NuMtlUpdate(NUMTL *mtl);
void FUN_0051fd10(i32 unk);

// FUNCTION: LEGOBATMAN 0x006bf2a0
void MenuInitialiseEx(MENUFNINFO *menu_info, i32 menu_id_count,
                      i32 language_count,
                      void (*draw_save_slots_info_fn)(f32, f32, i32, i32),
                      i32 is_fade_enabled, i32 is_shadow_enabled) {
  char menus_used_str[64];

  if (menu_id_count + 0x1f > 0x64)
    menu_id_count = 0x45;
  for (i32 i = 0; i < menu_id_count; i++)
    MenuInfo[0x1f + i] = menu_info[i];

  MenusUsed = menu_id_count + 0x1f;
  sprintf(menus_used_str, "Menus used: %d", MenusUsed);

  MenuLanguages = language_count;
  g_unk00ad69cc = g_unk00ad7352;
  MenuHeader[0] = '\0';
  header_r = MENUHEADERR;
  header_g = MENUHEADERG;

  if (draw_save_slots_info_fn != 0)
    drawslotinfofn = draw_save_slots_info_fn;

  MenuFadeEnabled = is_fade_enabled;
  MenuDrawDropShadows = is_shadow_enabled;

  MenuFadeMtl = NuMtlCreate(1);
  MenuFadeMtl->z_mode = 3;
  MenuFadeMtl->alpha_mode = 1;
  MenuFadeMtl->attrib16 = 2;
  MenuFadeMtl->attrib8 = 1;
  MenuFadeMtl->attrib10 = 1;
  MenuFadeMtl->attrib18 = 1;
  MenuFadeMtl->filter_mode = 1;
  NuMtlUpdate(MenuFadeMtl);
  FUN_0051fd10(-1);
}

struct GAMEMENU_s {
  u8 pad0[0x14];
  i16 menu; // 0x14
  u8 pad16[0xe0 - 0x16];
};

// GLOBAL: LEGOBATMAN 0x00ad69d0
extern GAMEMENU_s GameMenu[10];
// GLOBAL: LEGOBATMAN 0x0099e380
extern i32 MenuSFX;
// GLOBAL: LEGOBATMAN 0x0099e384
extern i32 GameMenuLevel;

// from saga legoapi/menus/screens/gamemenuall.cpp
// FUNCTION: LEGOBATMAN 0x006bf550
void MenuReset(void) {
  memset(GameMenu, 0, sizeof(GameMenu));
  GameMenu[0].menu = -1;
  MenuSFX = -1;
  GameMenuLevel = 0;
  FUN_0051fd10(-1);
}

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_menu_unk(f32 *v, f32 a, i32 i) {
  v[0] = NuSinApprox(i);
  NuVec4Set(v, a, a, a, a);
}
