// batman/, file unknown: hub functions near Hub_Reset.

#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"
#include "worldinfo_unk.h"

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x00492130
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);
// FUNCTION: LEGOBATMAN 0x00492160
static f32 NuFabs(f32 f);
// FUNCTION: LEGOBATMAN 0x00492180
static f32 NuFsign(f32 f);
// FUNCTION: LEGOBATMAN 0x00492380
static f32 NuSinApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x00492440
static f32 NuCosApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x00492460
static void NuVec4Copy(f32 *dst, f32 *src);

void *Door_FindByName(WORLDINFO_s *wi, char *name);
GameObject_s *Unk0044c930(AISYS_s *aisys, char *name);

enum HubSwitchMode {};

// GLOBAL: LEGOBATMAN 0x009c9728
void *g_unk009c9728;
// GLOBAL: LEGOBATMAN 0x009c9720
i32 g_unk009c9720;
// GLOBAL: LEGOBATMAN 0x009c9724
f32 g_unk009c9724;

// Three {GameObject_s*, special} pairs at 0x009c9134, 0x10 apart.
// GLOBAL: LEGOBATMAN 0x009c9134
GameObject_s *g_unk009c9134;
// GLOBAL: LEGOBATMAN 0x009c9138
extern nuhspecial_s g_unk009c9138;
// GLOBAL: LEGOBATMAN 0x009c9144
GameObject_s *g_unk009c9144;
// GLOBAL: LEGOBATMAN 0x009c9148
extern nuhspecial_s g_unk009c9148;
// GLOBAL: LEGOBATMAN 0x009c9154
GameObject_s *g_unk009c9154;
// GLOBAL: LEGOBATMAN 0x009c9158
extern nuhspecial_s g_unk009c9158;

// STUB: LEGOBATMAN 0x004931b0
// 8 bytes short: the conditional name select is laid out differently around the
// push of g_unk00960894
void Hub_SignalSwitchHeroVillain(HubSwitchMode mode) {
  g_unk009c9728 = Door_FindByName(g_unk00960894, mode == 1 ? "door_from_arkham"
                                                           : "door_to_arkham");
  if (g_unk009c9728) {
    g_unk009c9720 = mode;
    g_unk009c9724 = 0.0f;
  }
}

// FUNCTION: LEGOBATMAN 0x00497d70
void InitializeDummyChars(WORLDINFO_s *wi) {
  g_unk009c9134 = Unk0044c930(wi->aiSys2bf8, "dummychar_1");
  NuSpecialFind(wi->scn140, &g_unk009c9138, "batCar_door", 0);
  g_unk009c9144 = Unk0044c930(wi->aiSys2bf8, "dummychar_3");
  NuSpecialFind(wi->scn140, &g_unk009c9148, "batBoat_door", 0);
  g_unk009c9154 = Unk0044c930(wi->aiSys2bf8, "dummychar_5");
  NuSpecialFind(wi->scn140, &g_unk009c9158, "batWing_door", 0);
}

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_hub_unk(f32 *v, f32 a, i32 i) {
  NuVec4Set(v, a, a, a, a);
  v[2] = NuFabs(a);
  v[5] = NuFsign(a);
  v[0] = NuSinApprox(i);
  v[1] = NuCosApprox(i);
  NuVec4Copy(v + 4, v);
}
