// gameapi/unk_00584780.cpp: particle kill/lookup (saga parts.cpp); a separate
// TU from parts_unk.cpp: KillPart calls AddScaledFiniteShotDebrisEffect
// instead of inlining it. File name unproven.

#include "../nu2api/nucore/common.h"
#include "../nu2api/numath/nuvec.h"
#include "gameobject_unk.h"
#include <stddef.h>
#include <string.h>

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

struct PARTTYPE_s {
  char name[0x10]; // 0x00
  u8 pad10[0xb2 - 0x10];
  i8 page; // 0xb2
  u8 padb3[0xec - 0xb3];
  f32 emission_period;        // 0xec
  f32 emission_period_random; // 0xf0
  f32 emission_pause;         // 0xf4
  f32 emission_pause_random;  // 0xf8
  u8 padfc[0x198 - 0xfc];
};

// GLOBAL: LEGOBATMAN 0x00a295b0
extern PARTTYPE_s part_types[128];

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

// GLOBAL: LEGOBATMAN 0x00a37408
void *PartRTL;

// FUNCTION: LEGOBATMAN 0x00584930
void SetPartRTLSet(u32 rtl_set) { PartRTL = (void *)rtl_set; }

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

struct PARTEMIT_s {
  i32 effect_id; // 0x00
  u8 pad04[0x6c - 0x04];
};

// GLOBAL: LEGOBATMAN 0x00a3628c
extern PARTEMIT_s part_emits[40];

i32 NuStrNCmp(const char *a, const char *b, i32 n);

// FUNCTION: LEGOBATMAN 0x00584b10
i32 PARTLookupTypePageOnly(char *name, i32 page) {
  if (name == NULL || name[0] == 0)
    return -1;
  if ((u32)(page - 1) <= 6) {
    for (i32 i = 0; i < 128; ++i) {
      if (part_types[i].page == page &&
          NuStrNICmp(name, part_types[i].name, 16) == 0)
        return i;
    }
  }
  for (i32 i = 0; i < 128; ++i) {
    if (part_types[i].page == 0 && NuStrNCmp(name, part_types[i].name, 16) == 0)
      return i;
  }
  return -1;
}

// FUNCTION: LEGOBATMAN 0x00584bc0
i32 GetPartCount(void) {
  i32 count = 0;
  for (i32 i = 0; i < 40; ++i) {
    if (part_emits[i].effect_id != -1)
      ++count;
  }
  return count;
}

// FUNCTION: LEGOBATMAN 0x00584c10
i32 GetMaxPartTypes(void) { return 0x80; }

// FUNCTION: LEGOBATMAN 0x00584c20
char *GetPartName(i32 index) {
  if (part_types[index].name[0] == 0)
    return NULL;
  return part_types[index].name;
}

// FUNCTION: LEGOBATMAN 0x00584c50
f32 PARTGetTotalOnTime(i32 index) {
  if (index >= 0)
    return part_types[index].emission_period +
           part_types[index].emission_period_random;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x00584c80
f32 PARTGetTotalOffTime(i32 index) {
  if (index >= 0)
    return part_types[index].emission_pause +
           part_types[index].emission_pause_random;
  return 0.0f;
}

// STUB: LEGOBATMAN 0x00584f00
// close: orig bases its pointer walk at &part->source_special (0x14c) and
// zeroes ecx inside the hit block; ours bases at 0x148 and hoists the zero.
void KillPartsByScene(void *scene) {
  PART_s *part = Part;
  for (i32 index = 0; index < MAXPARTS; ++index, ++part) {
    if ((part->flags148 & 1) != 0 && part->source_special != NULL &&
        part->special.scene == scene) {
      memset(&part->special, 0, sizeof(part->special));
      part->flags148 &= ~1;
      part->source_special = NULL;
    }
  }
}

// GLOBAL: LEGOBATMAN 0x0095e164
extern i32 edpart_load_particle_page;

// FUNCTION: LEGOBATMAN 0x00585040
void edpartSetParticlePage(i32 page) { edpart_load_particle_page = page; }
