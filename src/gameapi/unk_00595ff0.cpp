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

struct APICHARACTERMODEL {
  u16 model_id;                // 0x00
  u8 flags;                    // 0x02
  u8 field_0x3;                // 0x03
  void *hierarchy;             // 0x04
  void **model_data_a;         // 0x08
  void **model_data_b;         // 0x0c
  void **model_data_c;         // 0x10
  u8 points_of_interest[0x50]; // 0x14
  u8 pad64[0x6c - 0x64];
};

struct APICHARDATA {
  u32 pad00;
  u32 model_flags; // 0x04
  u8 pad08[0x48 - 0x08];
};

struct APICHARACTERSYS_s {
  i32 character_count; // 0x00
  u32 pad04;
  i32 model_id_capacity;     // 0x08
  i32 permanent_model_count; // 0x0c
  i32 loaded_model_count;    // 0x10
  u32 pad14;
  APICHARACTERMODEL *models; // 0x18
  i16 *playermodelids;       // 0x1c
  APICHARDATA *char_data;    // 0x20
};

// GLOBAL: LEGOBATMAN 0x00a94740
extern APICHARACTERSYS_s *apicharsys;

f32 NuAnimEndFrame(void *anim);

// FUNCTION: LEGOBATMAN 0x00595fb0
f32 AnimEndFrame(CHARACTERMODEL_s *model, i32 animation) {
  if (model != 0 && animation >= 0 &&
      animation < apicharsys->model_id_capacity &&
      model->model_data_b[animation] != 0)
    return NuAnimEndFrame(model->model_data_b[animation]);
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x00595ff0
u32 AnimFlags(CHARACTERMODEL_s *model, i32 animation) {
  if (animation != -1 && model->model_data_b[animation] != 0)
    return ((CHARACTERANIM_s *)model->model_data_a[animation])->flags;
  return 0;
}

// GLOBAL: LEGOBATMAN 0x0095ea5c
static i32 AnimBlendMode = 1;

// FUNCTION: LEGOBATMAN 0x00596020
void SetAnimBlendMode(i32 mode) { AnimBlendMode = mode; }

// FUNCTION: LEGOBATMAN 0x00596030
i32 GetAnimBlendMode(void) { return AnimBlendMode; }

struct APIOBJECT {
  u8 pad000[0x1fc];
  u32 flags; // 0x1fc, bit 0 = in use
  u8 pad200[0x259 - 0x200];
  u8 index; // 0x259
};

struct APIOBJECTSYS_s {
  u32 object_size;                    // 0x00
  APIOBJECT *objects;                 // 0x04
  unsigned __int64 line_of_sight[64]; // 0x08
  u8 pad208[0x218 - 0x208];
};

void *AISysBufferAlloc(variptr_u *buf, variptr_u *buf_end, u32 size);

// FUNCTION: LEGOBATMAN 0x00596040
APIOBJECTSYS_s *APIObjectSysInit(i32 size, variptr_u *buf, variptr_u *buf_end) {
  APIOBJECTSYS_s *system =
      (APIOBJECTSYS_s *)AISysBufferAlloc(buf, buf_end, sizeof(APIOBJECTSYS_s));
  if (system != 0) {
    memset(system, 0, sizeof(*system));
    if (size != 0) {
      system->objects = (APIOBJECT *)AISysBufferAlloc(buf, buf_end, size * 64);
      if (system->objects != 0) {
        system->object_size = size;
        memset(system->objects, 0, size * 64);
      }
    }
  }
  return system;
}

// FUNCTION: LEGOBATMAN 0x005960e0
APIOBJECT *APIObjectCreate(APIOBJECTSYS_s *system) {
  i32 index;
  APIOBJECT *object;
  if (system != 0 && system->object_size != 0) {
    object = system->objects;
    for (index = 0; index < 64;
         ++index, object = (APIOBJECT *)((u8 *)object + system->object_size)) {
      if ((object->flags & 1) == 0) {
        memset(object, 0, system->object_size);
        object->flags |= 1;
        object->index = index;
        return object;
      }
    }
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x00596130
void APIObjectRemoveFromLOSTable(APIOBJECTSYS_s *system, APIOBJECT *source,
                                 APIOBJECT *target) {
  if (source != 0) {
    system->line_of_sight[source->index] &=
        ~((unsigned __int64)1 << target->index);
  } else {
    system->line_of_sight[target->index] = 0;
  }
}

// FUNCTION: LEGOBATMAN 0x00596180
void APIObjectDestroy(APIOBJECTSYS_s *system, APIOBJECT *object) {
  if (system == 0 || object == 0 || system->object_size == 0)
    return;
  if (system->line_of_sight != 0) {
    APIOBJECT *other = system->objects;
    for (i32 i = 0; i < 64; i++) {
      if ((other->flags & 1) != 0)
        APIObjectRemoveFromLOSTable(system, other, object);
      other = (APIOBJECT *)((u8 *)other + system->object_size);
    }
    APIObjectRemoveFromLOSTable(system, 0, object);
  }
  memset(object, 0, system->object_size);
}

// FUNCTION: LEGOBATMAN 0x005961f0
void APIObjectDestroyAll(APIOBJECTSYS_s *system) {
  if (system != 0)
    memset(system->objects, 0, system->object_size * 64);
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

// FUNCTION: LEGOBATMAN 0x00598d80
void APICharacterModelReset(APICHARACTERMODEL *model) {
  model->flags &= 0xfe;
  model->model_id = 0;
  model->field_0x3 = 0;
  model->hierarchy = 0;
  if (apicharsys->model_id_capacity != 0) {
    memset(model->model_data_a, 0,
           apicharsys->model_id_capacity * sizeof(void *));
    memset(model->model_data_b, 0,
           apicharsys->model_id_capacity * sizeof(void *));
    memset(model->model_data_c, 0,
           apicharsys->model_id_capacity * sizeof(void *));
  }
  memset(model->points_of_interest, 0, sizeof(model->points_of_interest));
}

// FUNCTION: LEGOBATMAN 0x00599090
void APIResetCharacterRemap(void) {
  for (i32 i = 0; i < apicharsys->character_count; ++i) {
    if ((apicharsys->char_data[i].model_flags & 2) == 0)
      apicharsys->playermodelids[i] = -1;
  }
}

// FUNCTION: LEGOBATMAN 0x0059a050
APICHARACTERMODEL *APICharacterLoaded(i32 character_id) {
  if (character_id != -1) {
    i16 model_index = apicharsys->playermodelids[character_id];
    if (model_index != -1)
      return &apicharsys->models[model_index];
  }
  return 0;
}

void NuHGobjDestroy(void *hierarchy);

// FUNCTION: LEGOBATMAN 0x0059a080
void APIDumpCharacterModels(i32 mode) {
  for (i32 i = mode != 0 ? 0 : apicharsys->permanent_model_count;
       i < apicharsys->loaded_model_count; i++) {
    if (apicharsys->models[i].hierarchy != 0)
      NuHGobjDestroy(apicharsys->models[i].hierarchy);
  }
}
