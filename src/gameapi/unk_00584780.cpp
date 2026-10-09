// gameapi/unk_00584780.cpp: particle kill/lookup (saga parts.cpp); a separate
// TU from parts_unk.cpp: KillPart calls AddScaledFiniteShotDebrisEffect
// instead of inlining it. File name unproven.

#include "../nu2api/nucore/common.h"
#include "../nu2api/numath/nuvec.h"
#include "gameobject_unk.h"
#include <stddef.h>

void AddScaledFiniteShotDebrisEffect(i32 *key, i32 effect, NUVEC *position,
                                     NUVEC *orientation, NUVEC *momentum,
                                     i32 count, f32 scale);
void DebFreeInstantly(void *key);
void rtlDynamicFree(i32 handle);

// FUNCTION: LEGOBATMAN 0x00584780
void KillPart(PART_s *part, i32 reason) {
  if ((part->flags148 & 1) == 0)
    return;
  part->flags148 &= ~1;
  if ((part->flags10c & 0x20000) != 0 && part->debris_key != NULL)
    DebFreeInstantly(part->debris_key);
  if (part->lighting_template != NULL) {
    rtlDynamicFree(*part->lighting_template);
    *part->lighting_template = -1;
  }
  if (part->kill_callback != NULL)
    part->kill_callback(part, reason);
  if (reason != 6 && part->kill_effect != -1) {
    i32 key = -1;
    AddScaledFiniteShotDebrisEffect(&key, part->kill_effect, &part->pos, 0, 0,
                                    1, part->kill_effect_scale);
  }
}

// GLOBAL: LEGOBATMAN 0x00a361b0
extern PART_s *Part;

// GLOBAL: LEGOBATMAN 0x0095e148
extern i32 MAXPARTS;

// FUNCTION: LEGOBATMAN 0x00584830
void KillAllParts(void) {
  PART_s *part = Part;
  if (part != NULL) {
    for (i32 i = 0; i < MAXPARTS; i++, part++) {
      if ((part->flags148 & 1) != 0)
        KillPart(part, 0);
    }
  }
}

f32 NuVecDistSqr(nuvec_s *a, nuvec_s *b, nuvec_s *d);

// FUNCTION: LEGOBATMAN 0x00584870
PART_s *FindPart(nuvec_s *position, i32 player, void *owner) {
  if (Part == NULL)
    return NULL;
  PART_s *result = NULL;
  f32 nearest = 1000000.0f;
  PART_s *part = Part;
  for (i32 i = 0; i < MAXPARTS; ++i, ++part) {
    if ((part->flags148 & 1) != 0 &&
        (player == -1 || part->force_player_mask == player) &&
        (owner == NULL || part->objd4 == owner)) {
      if (position == NULL)
        return part;
      f32 distance = NuVecDistSqr(position, &part->pos, NULL);
      if (distance < nearest) {
        nearest = distance;
        result = part;
      }
    }
  }
  return result;
}

i32 NuStrNICmp(const char *a, const char *b, i32 n);

struct PARTTYPE_s {
  char name[0x10]; // 0x00
  u8 pad10[0x198 - 0x10];
};

// GLOBAL: LEGOBATMAN 0x00a295b0
extern PARTTYPE_s part_types[128];

// FUNCTION: LEGOBATMAN 0x00584ac0
i32 PARTLookupType(char *name) {
  if (name == NULL || name[0] == 0)
    return -1;
  for (i32 i = 0; i < 128; ++i) {
    if (NuStrNICmp(name, part_types[i].name, 16) == 0)
      return i;
  }
  return -1;
}
