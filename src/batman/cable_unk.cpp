// batman/, file unknown: tow cable (0x00416460).

#include "../gameapi/sfx_unk.h"
#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"
#include "worldinfo_unk.h"

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x00415fe0
static f32 NuFdiv(f32 a, f32 b);
// FUNCTION: LEGOBATMAN 0x00416030
static f32 NuSinApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x004160f0
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

// CABLE_s after ref/saga/src/legoapi/legoapi_types.h; Batman's layout is
// wider (points at +0x14, velocities at +0xd8, byte flags at +0x1f9).
struct CABLE_s {
  GameObject_s *source; // 0x00
  GameObject_s *target; // 0x04
  u8 pad0[0x14 - 0x08];
  nuvec_s points[1]; // 0x14
  u8 pad1[0xd8 - 0x20];
  nuvec_s velocities[1]; // 0xd8
  u8 pad2[0x1f7 - 0xe4];
  u8 point_count; // 0x1f7
  u8 pad3[1];
  u8 flags1f9; // 0x1f9
  u8 flags1fa; // 0x1fa
};

// GLOBAL: LEGOBATMAN 0x00ab3980
extern GameObject_s *player;
// GLOBAL: LEGOBATMAN 0x00ab3984
extern GameObject_s *player2;

void GameCam_Blend(GAMECAMERA_s *cam, f32 time, f32 delay, i32 flags);

// FUNCTION: LEGOBATMAN 0x00416460
void ReleaseCable(CABLE_s *cable, int snap) {
  i32 i;
  if (cable->flags1f9 & 4)
    cable->flags1fa |= 8;
  else
    cable->flags1fa |= 4;
  if (cable->target) {
    cable->target->flags1414 &= ~0x100;
    cable->target->flags1414 &= ~0x1000;
  }
  if (snap)
    GameAudio_PlaySfxById(GetSfxId("TowCable_Snap"), &cable->points[0], 1, 0);
  else
    GameAudio_PlaySfxById(GetSfxId("TowCable_Detach"), &cable->points[0], 1, 0);
  if (cable->source && (cable->source == player || cable->source == player2))
    GameCam_Blend(g_unk0095f624, 1.0f, 0.0f, 1);
  if ((cable->flags1fa & 4) && cable->source &&
      cable->source->cable157c == cable) {
    cable->source->cable157c = 0;
    cable->source = 0;
  }
  cable->target = 0;
  for (i = 0; i < cable->point_count; i++)
    cable->velocities[i].x = 0.0f;
}

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_cable_unk(f32 *v, f32 a, i32 i) {
  v[3] = NuFdiv(a, v[4]);
  v[0] = NuSinApprox(i);
  NuVec4Set(v, a, a, a, a);
}
