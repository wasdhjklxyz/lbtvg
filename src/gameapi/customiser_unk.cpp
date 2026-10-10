// gameapi/, file unknown: Customiser (Mac Customiser::*; saga
// legoapi/characters/core/customiser.cpp).

#include "../nu2api/nucore/common.h"
#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"
#include <string.h>

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x0060f8e0
static f32 NuSinApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x0060f980
static f32 NuCosApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x0060f990
static void NuVec4Copy(f32 *dst, f32 *src);
// FUNCTION: LEGOBATMAN 0x0060f9b0
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);
// FUNCTION: LEGOBATMAN 0x0060fa10
static void NuVecScaleInline(f32 *dst, f32 *src, f32 s);

struct nuvec_s;

void PlaySfx(char *name, nuvec_s *position);
i32 Hub_ClearStats();
void Text_FillInExtendedSaveInfo();
extern "C" i32 TriggerAutoSave();
extern "C" void NewMenuBatte(i32 a, i32 b, i32 c);

struct TIMER_s;
struct GAMEMENU_s {
  u8 pad[0xe0];
};
extern "C" void MenuRememberCursor(GAMEMENU_s *menu);
void MenuReset(void);
void ResetTimer(TIMER_s *timer, float time);

// GLOBAL: LEGOBATMAN 0x00ad69d0
extern GAMEMENU_s GameMenu[10];
// GLOBAL: LEGOBATMAN 0x0099e384
extern i32 GameMenuLevel;
// GLOBAL: LEGOBATMAN 0x00a9600c
extern TIMER_s g_unk00a9600c; // customiser timer
// GLOBAL: LEGOBATMAN 0x00aca830
extern u8 g_unk00aca830;
// GLOBAL: LEGOBATMAN 0x00aca850
extern f32 g_unk00aca850;
// GLOBAL: LEGOBATMAN 0x00acad78
extern i32 g_unk00acad78;

// GLOBAL: LEGOBATMAN 0x009c59d0
extern u8 *g_unk009c59d0; // the current character settings (0x78 bytes)
// GLOBAL: LEGOBATMAN 0x00acad30
extern u8 *g_unk00acad30; // the settings as last saved

// GLOBAL: LEGOBATMAN 0x00ab0950
extern i32 g_unk00ab0950;
// GLOBAL: LEGOBATMAN 0x00acad64
extern i32 g_unk00acad64;

class Customiser {
public:
  static i32 HasChangedSinceSave();
  static void MenuEnd();
  static void MenuSaveAndExit();
};

// STUB: LEGOBATMAN 0x0060ffc0
// inlined memcmp gets different registers (count in esi, pointers ecx/edx)
i32 Customiser::HasChangedSinceSave() {
  u8 *cur = g_unk009c59d0;
  u8 *saved = g_unk00acad30;
  if (memcmp(cur, saved, 0x78) != 0 ||
      memcmp(cur + 0x18, saved + 0x18, 0x20) != 0 || cur[0x38] != saved[0x38] ||
      cur[0x74] != saved[0x74])
    return 1;
  return 0;
}

// FUNCTION: LEGOBATMAN 0x00610160
void Customiser::MenuEnd() {
  g_unk00aca830 = 1;
  g_unk00aca850 = 0.6f;
  MenuRememberCursor(&GameMenu[GameMenuLevel]);
  MenuReset();
  ResetTimer(&g_unk00a9600c, 0.0f);
  g_unk00acad78 = 0;
}

// FUNCTION: LEGOBATMAN 0x006122e0
void Customiser::MenuSaveAndExit() {
  PlaySfx("menuBack", 0);
  g_unk00acad64 = 1;
  if (g_unk00ab0950 == 0 && HasChangedSinceSave()) {
    Hub_ClearStats();
    Text_FillInExtendedSaveInfo();
    if (TriggerAutoSave() != 0) {
      MenuEnd();
      return;
    }
    MenuEnd();
    NewMenuBatte(1000, -1, -1);
    return;
  }
  MenuEnd();
}

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_customiser_unk(f32 *v, f32 a, i32 i) {
  v[0] = NuSinApprox(i);
  v[1] = NuCosApprox(i);
  NuVec4Copy(v + 4, v);
  NuVec4Set(v, a, a, a, a);
  NuVecScaleInline(v + 8, v, a);
}
