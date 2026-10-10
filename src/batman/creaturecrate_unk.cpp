// batman/, file unknown: CreatureCrate_Stop and its TU's NuSinApprox copy.

#include "../gameapi/sfx_unk.h"
#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"
#include "worldinfo_unk.h"

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x0043a820
static f32 NuFdiv(f32 a, f32 b);
// FUNCTION: LEGOBATMAN 0x0043a930
static f32 NuCosApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x0043a950
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

i16 FindGameDebris(void *page, char *name);
void AddGameDebris(void *page, i32 id, nuvec_s *pos);
i32 PARTLookupType(char *name);
void AddFiniteShotPART(i32 type, nuvec_s *pos, i32 count);
void GameCam_Judder(GAMECAMERA_s *cam, f32 amount, i32 a, nuvec_s *pos);
void NewRumbleAllPlayers(f32 a, f32 b, i32 c, i32 d);

// Body in nutrig_unk.h: a static that every user TU gets its own copy of.
// FUNCTION: LEGOBATMAN 0x0043a870
static f32 NuSinApprox(i32 angle);

// Keeps the static NuSinApprox copy alive until its real caller is matched.
f32 Unk_CreatureCrate_NuSinApproxUser(i32 angle) { return NuSinApprox(angle); }

// FUNCTION: LEGOBATMAN 0x0043a980
void CreatureCrate_Stop(PART_s *part) {
  GameObject_s *obj = part->objd4;
  if (obj) {
    i16 debris = FindGameDebris(g_unk00960894->p138, "CRATE_POP");
    if (debris != -1)
      AddGameDebris(g_unk00960894->p138, debris, &part->pos);
    i32 type = PARTLookupType("CRATE_PART");
    if (type != -1)
      AddFiniteShotPART(type, &part->pos, 1);
    obj->flags1fc |= 0x1000;
    obj->b3ce = 2;
    GameAudio_PlaySfx(0x83, &part->pos, 0, 0);
    GameCam_Judder(g_unk0095f624, 0.1f, 0, 0);
    NewRumbleAllPlayers(0.0f, 0.0f, 2, 0);
  }
}

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_creaturecrate_unk(f32 *v, f32 a, i32 i) {
  v[3] = NuFdiv(a, v[4]);
  v[1] = NuCosApprox(i);
  NuVec4Set(v, a, a, a, a);
}
