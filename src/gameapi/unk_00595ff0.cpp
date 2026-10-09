// gameapi/unk_00595ff0.cpp: placed by tools/new.py; file name unproven.

#include "../nu2api/nucore/common.h"
#include "../nu2api/numath/numtx.h"
#include <stddef.h>
#include <string.h>

struct CHARACTERANIM_s {
  u32 pad00;
  u32 flags; // 0x04
};

struct CHARACTERMODEL_s {
  u32 pad00[2];
  void **model_data_a; // 0x08
  void **model_data_b; // 0x0c
};

// FUNCTION: LEGOBATMAN 0x00595ff0
u32 AnimFlags(CHARACTERMODEL_s *model, i32 animation) {
  if (animation != -1 && model->model_data_b[animation] != 0)
    return ((CHARACTERANIM_s *)model->model_data_a[animation])->flags;
  return 0;
}

u32 NuRandInt(void);

// FUNCTION: LEGOBATMAN 0x00597d60
i32 ParticlesPerFrame(f32 particles_per_frame, f32 frame_time) {
  i32 scaled_count =
      (i32)(frame_time * 60.0f * (particles_per_frame * 65536.0f));
  i32 count = 0;
  while (scaled_count >= 0x10000) {
    scaled_count -= 0x10000;
    ++count;
  }
  if ((i32)(NuRandInt() >> 16) < scaled_count)
    ++count;
  return count;
}

// FUNCTION: LEGOBATMAN 0x00597db0
i32 ParticlesPerSecond(f32 particles_per_second, f32 frame_time) {
  return ParticlesPerFrame(particles_per_second / 60.0f, frame_time);
}

#include "../nu2api/nucore/nustring.h"

struct APIDEBRISENTRY_s {
  i32 effect;      // 0x00
  char name[0x10]; // 0x04
};

struct APIDEBRISSYS_s {
  i32 named_count;           // 0x00
  i32 capacity;              // 0x04
  APIDEBRISENTRY_s *entries; // 0x08
};

// FUNCTION: LEGOBATMAN 0x00597e10
i32 FindGameDebris(APIDEBRISSYS_s *debris_sys, char *name) {
  for (i32 index = debris_sys->named_count; index < debris_sys->capacity;
       ++index) {
    if (NuStrICmp(name, debris_sys->entries[index].name) == 0)
      return index;
  }
  return -1;
}

struct DEBRISTYPE_s {
  char name[16]; // 0x00
};

// GLOBAL: LEGOBATMAN 0x00a28cb4
extern i32 EDPP_MAX_TYPES;

// GLOBAL: LEGOBATMAN 0x00a28cb8
extern DEBRISTYPE_s **debtab;

i32 LookupDebrisEffectPage(char *name, i32 page);
i32 LookupDebrisEffectPageOnly(char *name, i32 page);

// FUNCTION: LEGOBATMAN 0x00597e60
APIDEBRISSYS_s *InitGameDebris(variptr_u *cursor, variptr_u end, i32 count,
                               i32 flags, char **names, i32 page) {
  APIDEBRISSYS_s *sys = (APIDEBRISSYS_s *)((cursor->addr + 0xf) & ~0xf);
  cursor->addr = (cursor->addr + 0xf) & ~0xf;
  cursor->addr += sizeof(*sys);
  if (sys != 0) {
    memset(sys, 0, sizeof(*sys));
    sys->capacity = count;
    sys->named_count = flags;
    sys->entries = (APIDEBRISENTRY_s *)((cursor->addr + 0xf) & ~0xf);
    cursor->addr = (cursor->addr + 0xf) & ~0xf;
    cursor->addr += count * sizeof(*sys->entries);
    if (sys->entries != 0) {
      memset(sys->entries, -1, count * sizeof(*sys->entries));

      i32 i;
      for (i = 0; i < sys->named_count; i++) {
        NuStrCpy(sys->entries[i].name, names[i]);
        sys->entries[i].effect = -1;
        sys->entries[i].effect =
            LookupDebrisEffectPage(sys->entries[i].name, page);
      }
      for (i32 j = 1; i < sys->capacity && j < EDPP_MAX_TYPES; j++) {
        sys->entries[i].effect = -1;
        if (debtab[j] != 0) {
          NuStrCpy(sys->entries[i].name, debtab[j]->name);
          sys->entries[i].effect =
              LookupDebrisEffectPageOnly(sys->entries[i].name, page);
          i++;
        }
      }
      for (; i < sys->capacity; i++)
        sys->entries[i].effect = -1;
      return sys;
    }
  }
  return 0;
}

void AddFiniteShotDebrisEffect(i32 *handle, i32 effect, nuvec_s *position,
                               i32 count);

// FUNCTION: LEGOBATMAN 0x00597fc0
i32 AddGameDebris(APIDEBRISSYS_s *system, i32 type, nuvec_s *position) {
  if (type >= 0 && type < system->capacity &&
      system->entries[type].effect != -1) {
    i32 handle = -1;
    AddFiniteShotDebrisEffect(&handle, system->entries[type].effect, position,
                              1);
    return 1;
  }
  return 0;
}

void AddFiniteShotDebrisEffect2(i32 *handle, i32 effect, nuvec_s *position,
                                nuvec_s *emitter_momentum,
                                nuvec_s *particle_momentum, i32 count);

// FUNCTION: LEGOBATMAN 0x00598010
i32 AddGameDebrisMomentum(APIDEBRISSYS_s *system, i32 type, nuvec_s *position,
                          nuvec_s *emitter_momentum,
                          nuvec_s *particle_momentum) {
  if (type >= 0 && type < system->capacity &&
      system->entries[type].effect != -1) {
    i32 handle = -1;
    AddFiniteShotDebrisEffect2(&handle, system->entries[type].effect, position,
                               emitter_momentum, particle_momentum, 1);
    return 1;
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x00598070
i32 AddGameDebrisXYZ(APIDEBRISSYS_s *system, i32 type, f32 x, f32 y, f32 z) {
  if (type >= 0 && type < system->capacity &&
      system->entries[type].effect != -1) {
    i32 handle = -1;
    nuvec_s position = {x, y, z};
    AddFiniteShotDebrisEffect(&handle, system->entries[type].effect, &position,
                              1);
    return 1;
  }
  return 0;
}

void AddVariableShotDebrisEffect(i32 effect, nuvec_s *position, i32 count,
                                 i16 z_rotation, i16 y_rotation);

// FUNCTION: LEGOBATMAN 0x005980e0
i32 AddGameDebrisRot(APIDEBRISSYS_s *system, i32 type, nuvec_s *position,
                     i32 count, u16 z_rotation, u16 y_rotation) {
  if (type >= 0 && type < system->capacity &&
      system->entries[type].effect != -1 && count > 0) {
    AddVariableShotDebrisEffect(system->entries[type].effect, position, count,
                                z_rotation, y_rotation);
    return 1;
  }
  return 0;
}

void AddVariableShotDebrisEffectMtx3(i32 effect, nuvec_s *position,
                                     nuvec_s *momentum, i32 count,
                                     numtx_s *orientation, numtx_s *matrix);

// GLOBAL: LEGOBATMAN 0x00ad3b60
extern nuvec_s nuvec_zero;

void NuMtxSetRotationX(numtx_s *m, i32 angle);
void NuMtxMulR(numtx_s *out, numtx_s *a, numtx_s *b);

// FUNCTION: LEGOBATMAN 0x00598130
i32 AddGameDebrisMtx(APIDEBRISSYS_s *system, i32 type, nuvec_s *position,
                     i32 count, numtx_s *matrix) {
  if (type >= 0 && type < system->capacity &&
      system->entries[type].effect != -1 && count > 0) {
    numtx_s orientation;
    NuMtxSetRotationX(&orientation, 0x4000);
    NuMtxMulR(&orientation, &orientation, matrix);
    AddVariableShotDebrisEffectMtx3(system->entries[type].effect, position,
                                    &nuvec_zero, count, &orientation,
                                    &numtx_identity);
    return 1;
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x005981c0
i32 AddGameDebrisMom(APIDEBRISSYS_s *system, i32 type, nuvec_s *position,
                     i32 count, nuvec_s *momentum) {
  if (type >= 0 && type < system->capacity &&
      system->entries[type].effect != -1 && count > 0) {
    nuvec_s zero = {0.0f, 0.0f, 0.0f};
    if (momentum == 0)
      momentum = &zero;
    AddVariableShotDebrisEffectMtx3(system->entries[type].effect, position,
                                    momentum, count, 0, 0);
    return 1;
  }
  return 0;
}

// GLOBAL: LEGOBATMAN 0x02a14dc8
void (*APIObjPlaySfxByIdFn)(i32, nuvec_s *);

// FUNCTION: LEGOBATMAN 0x00598230
void SetAPIObjPlaySfxByIdFn(void (*play_sfx)(i32, nuvec_s *)) {
  APIObjPlaySfxByIdFn = play_sfx;
}
