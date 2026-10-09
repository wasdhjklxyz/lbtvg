// batman/thermaldetonators_unk.cpp: placed by tools/new.py; file name unproven.

#include "../gameapi/gameobject_unk.h"
#include "../gameapi/sfx_unk.h"
#include "../nu2api/nucore/common.h"
#include <stddef.h>

// Both tables are 0x1c-byte entries tested at +0x9623cc / +0x96274c, 0x380
// (32 entries) apart; the +4 flags offset (and so both bases) is assumed.
struct TERRSURFACE_s {
  f32 f0;
  u32 flags; // 0x04
  u32 pad8[5];
};

// GLOBAL: LEGOBATMAN 0x009623c8
extern TERRSURFACE_s TerSurface[32];
// GLOBAL: LEGOBATMAN 0x00962748
extern TERRSURFACE_s TerLayer[17];
// GLOBAL: LEGOBATMAN 0x009e81c4
extern f32 EShadY;
// GLOBAL: LEGOBATMAN 0x009ca98c
extern f32 g_unk009ca98c;

extern "C" void NewTerrPlatformsOff(void);
f32 GameShadow(GameObject_s *object, nuvec_s *position, f32 probe_height,
               i32 terrain_mask);
extern "C" i32 ShadowInfo(void);
extern "C" i32 EShadowInfo(void);

// FUNCTION: LEGOBATMAN 0x004f9f70
void PartUpdate_ThermalDetonator(PART_s *part) {
  if (!(part->flags148 & 0x4000) && (part->flags148 & 2)) {
    if (part->f100 > 0.0f && part->f100 < 1.0f) {
      PlaySfx("ThermalDet_Beep", &part->pos);
      part->flags148 |= 0x4000;
      return;
    }
  }
  if (part->flags148 & 2)
    return;

  part->flags148 &= ~0x20000;
  NewTerrPlatformsOff();
  f32 height = GameShadow(0, &part->pos, g_unk009ca98c, -1);
  if (height == 2000000.0)
    return;
  if (part->pos.y > height) {
    i32 surface = ShadowInfo();
    if (surface >= -1 && surface <= 16 && (TerSurface[surface].flags & 2)) {
      part->reflection_height = height;
      part->flags148 |= 0x20000;
    }
  }
  f32 water = EShadY;
  if (water == 2000000.0)
    return;
  i32 layer = EShadowInfo();
  if ((u32)layer > 16)
    return;
  if (!(part->flags148 & 0x8000) && part->radius + part->pos.y < water &&
      ((TerLayer[layer].flags & 9) || layer == 9))
    part->flags148 |= 0x8000;
}
