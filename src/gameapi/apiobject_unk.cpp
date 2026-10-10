// gameapi/apiobject_unk.cpp: saga keeps these in
// legoapi/items/base/apiobject.cpp; Batman file name unproven.

#include "../nu2api/nucore/common.h"
#include "../nu2api/numath/numtx.h"
#include <stddef.h>
#include <string.h>

struct ANIMPACKET_s {
  union {
    f32 current_time;
    f32 field_0x00;
  };
  union {
    f32 previous_time;
    f32 field_0x04;
  };
  f32 blend_elapsed;  // 0x08
  f32 blend_duration; // 0x0c
  union {
    f32 blend_source_time;
    f32 time;
  };
  union {
    f32 blend_target_time;
    f32 time2;
  };
  u32 pad_0x18[(0x20 - 0x18) / 4];
  union {
    f32 field_0x20;
    f32 time_secondary;
  }; // 0x20
  u32 pad_0x24[(0x30 - 0x24) / 4];
  union {
    u8 flags;
    u8 field_0x30;
  };
  u8 blending;           // 0x31
  i16 blend_animation_a; // 0x32
  i16 blend_animation_b; // 0x34
  i16 animation_index;   // 0x36
  union {
    i16 previous_animation;
    i16 field_0x38;
  };
  union {
    i16 requested_animation;
    i16 field_0x3a;
  };
  u8 blend_source_reversed; // 0x3c
  u8 blend_target_reversed; // 0x3d
  u8 current_reversed;      // 0x3e
  u8 pad_0x3f;
  u16 pad_0x40;
  union {
    i16 overlay_animation; // -1 when no overlay is active
    u16 frame;
  }; // 0x42
  f32 field_0x44; // 0x44
};

struct CHARACTERANIM_s {
  char *name;       // 0x00
  u32 flags;        // 0x04
  i16 animation_id; // 0x08
  u8 pad0a;
  u8 stop_frame; // 0x0b
  u32 pad0c[(0x1c - 0x0c) / 4];
  f32 speed_x;        // 0x1c
  f32 action_speed;   // 0x20
  u8 event_frames[4]; // 0x24
};

struct CHARACTERDATA {
  u32 pad00[4];
  CHARACTERANIM_s *animations; // 0x10
};

struct CHARACTERMODEL_s {
  u32 pad00;
  struct NUHGOBJ *hierarchy; // 0x04
  void **model_data_a;       // 0x08
  void **model_data_b;       // 0x0c
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
#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x00595ea0
static f32 NuFabs(f32 f);
// FUNCTION: LEGOBATMAN 0x00595ec0
static f32 NuFdiv(f32 a, f32 b);
// FUNCTION: LEGOBATMAN 0x00595f00
static f32 NuSinApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x00595fa0
static f32 NuCosApprox(i32 angle);

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

// FUNCTION: LEGOBATMAN 0x0059a0d0
i16 FindAnimIX(CHARACTERDATA *character, char *name) {
  if (character != 0) {
    CHARACTERANIM_s *animation = character->animations;
    while (animation != 0 && animation->name != 0) {
      if (NuStrICmp(name, animation->name) == 0)
        return animation->animation_id;
      ++animation;
    }
  }
  return -1;
}

struct NUHGOBJ {
  u8 pad000[0x190];
  i32 render_count; // 0x190
};

// FUNCTION: LEGOBATMAN 0x0059a270
i32 MakeLayerList_Index(CHARACTERMODEL_s *model, i16 *layers, u32 mask) {
  if (model == 0)
    return 0;
  i32 count = 0;
  u32 layer_bit = 1;
  for (i32 layer = 0; layer < 32 && model->hierarchy->render_count > layer;
       ++layer) {
    if ((mask & layer_bit) != 0) {
      *layers++ = (i16)layer;
      ++count;
    }
    layer_bit <<= 1;
  }
  return count;
}

// FUNCTION: LEGOBATMAN 0x0059a730
void ResetAnimPacket(ANIMPACKET_s *packet, i32 animation) {
  if (packet == 0)
    return;
  packet->requested_animation = animation;
  packet->previous_animation = packet->requested_animation;
  packet->animation_index = packet->previous_animation;
  packet->previous_time = 1.0f;
  packet->blend_target_time = packet->previous_time;
  packet->current_time = packet->blend_target_time;
  packet->blending = 0;
  packet->flags = 4;
  packet->overlay_animation = -1;
  packet->current_reversed = 0;
  packet->blend_source_reversed = 0;
  packet->blend_target_reversed = 0;
}

struct MINIANIMPACKET_s {
  f32 current_time;           // 0x00
  f32 previous_time;          // 0x04
  f32 blend_elapsed;          // 0x08
  f32 blend_duration;         // 0x0c
  f32 blend_source_time;      // 0x10
  f32 blend_target_time;      // 0x14
  u8 flags;                   // 0x18
  u8 blending;                // 0x19
  i16 blend_animation_a;      // 0x1a
  i16 blend_animation_b;      // 0x1c
  i16 current_animation_id;   // 0x1e
  i16 previous_animation_id;  // 0x20
  i16 requested_animation_id; // 0x22
};

// FUNCTION: LEGOBATMAN 0x0059a770
void ResetMiniAnimPacket(MINIANIMPACKET_s *packet, i32 animation) {
  if (packet != 0) {
    packet->requested_animation_id = animation;
    packet->previous_animation_id = packet->requested_animation_id;
    packet->current_animation_id = packet->previous_animation_id;
    packet->previous_time = 1.0f;
    packet->blend_target_time = packet->previous_time;
    packet->current_time = packet->blend_target_time;
    packet->blending = 0;
    packet->flags = 4;
  }
}

f32 NuRandFloat(void);

// FUNCTION: LEGOBATMAN 0x0059a7a0
void SetMiniAnimTimeRandom(CHARACTERMODEL_s *model, MINIANIMPACKET_s *packet) {
  if (model == 0 || packet == 0)
    return;
  if (model->model_data_b[packet->requested_animation_id] != 0)
    packet->current_time =
        NuRandFloat() *
            (NuAnimEndFrame(
                 model->model_data_b[packet->requested_animation_id]) -
             1.0f) +
        1.0f;
}

// FUNCTION: LEGOBATMAN 0x0059a7f0
void SetAnimTimeRandom(CHARACTERMODEL_s *model, ANIMPACKET_s *packet) {
  if (model == 0 || packet == 0)
    return;
  if (model->model_data_b[packet->requested_animation] != 0)
    packet->current_time =
        NuRandFloat() *
            (NuAnimEndFrame(model->model_data_b[packet->requested_animation]) -
             1.0f) +
        1.0f;
}

// FUNCTION: LEGOBATMAN 0x0059a840
f32 GetAnimTimeRandom(CHARACTERMODEL_s *model, i32 animation) {
  if (model == 0 || model->model_data_b[animation] == 0)
    return 0.0f;
  return NuRandFloat() *
             (NuAnimEndFrame(model->model_data_b[animation]) - 1.0f) +
         1.0f;
}

// FUNCTION: LEGOBATMAN 0x0059b220
void AnimPacket_MiniToFull(MINIANIMPACKET_s *mini_packet,
                           ANIMPACKET_s *packet) {
  packet->current_time = mini_packet->current_time;
  packet->previous_time = mini_packet->previous_time;
  packet->blend_elapsed = mini_packet->blend_elapsed;
  packet->blend_duration = mini_packet->blend_duration;
  packet->blend_source_time = mini_packet->blend_source_time;
  packet->blend_target_time = mini_packet->blend_target_time;
  packet->flags = mini_packet->flags;
  packet->blending = mini_packet->blending;
  packet->blend_animation_a = mini_packet->blend_animation_a;
  packet->blend_animation_b = mini_packet->blend_animation_b;
  packet->animation_index = mini_packet->current_animation_id;
  packet->previous_animation = mini_packet->previous_animation_id;
  packet->requested_animation = mini_packet->requested_animation_id;
  packet->blend_source_reversed = 0;
  packet->blend_target_reversed = 0;
  packet->current_reversed = 0;
  packet->overlay_animation = -1;
}

// FUNCTION: LEGOBATMAN 0x0059b2a0
void AnimPacket_FullToMini(ANIMPACKET_s *packet,
                           MINIANIMPACKET_s *mini_packet) {
  mini_packet->current_time = packet->current_time;
  mini_packet->previous_time = packet->previous_time;
  mini_packet->blend_elapsed = packet->blend_elapsed;
  mini_packet->blend_duration = packet->blend_duration;
  mini_packet->blend_source_time = packet->blend_source_time;
  mini_packet->blend_target_time = packet->blend_target_time;
  mini_packet->flags = packet->flags;
  mini_packet->blending = packet->blending;
  mini_packet->blend_animation_a = packet->blend_animation_a;
  mini_packet->blend_animation_b = packet->blend_animation_b;
  mini_packet->current_animation_id = packet->animation_index;
  mini_packet->previous_animation_id = packet->previous_animation;
  mini_packet->requested_animation_id = packet->requested_animation;
}

void UpdateAnimPacket(CHARACTERMODEL_s *model, ANIMPACKET_s *packet,
                      f32 frame_step, f32 movement_speed, f32 blend_step,
                      f32 extra);

// FUNCTION: LEGOBATMAN 0x0059b310
void UpdateMiniAnimPacket(CHARACTERMODEL_s *model,
                          MINIANIMPACKET_s *mini_packet, f32 frame_step,
                          f32 movement_speed, f32 blend_step) {
  ANIMPACKET_s packet;
  AnimPacket_MiniToFull(mini_packet, &packet);
  UpdateAnimPacket(model, &packet, frame_step, movement_speed, blend_step,
                   0.0f);
  AnimPacket_FullToMini(&packet, mini_packet);
}

// FUNCTION: LEGOBATMAN 0x0059b370
i32 AnimBlendingFromTo(CHARACTERMODEL_s *model, ANIMPACKET_s *packet,
                       i32 source_animation, i32 target_animation) {
  if (packet->blending != 0 && source_animation != -1 &&
      packet->blend_animation_a == source_animation && target_animation != -1 &&
      packet->blend_animation_b == target_animation) {
    if (model != 0) {
      if (source_animation == -1 || model->model_data_b[source_animation] == 0)
        return 0;
      if (target_animation == -1 || model->model_data_b[target_animation] == 0)
        return 0;
    }
    return 1;
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x0059b3c0
f32 *AnimPlaying(ANIMPACKET_s *packet, i32 animation, i32 target, i32 source) {
  if (animation == -1)
    return 0;
  if (packet->blending != 0) {
    if (target != 0 && packet->blend_animation_b == animation)
      return &packet->blend_target_time;
    if (source != 0 && packet->blend_animation_a == animation)
      return &packet->blend_source_time;
  } else {
    if (packet->animation_index == animation)
      return &packet->current_time;
  }
  return 0;
}

// from saga legoapi/items/base/apiobject.cpp
// FUNCTION: LEGOBATMAN 0x0059b480
i32 CurrentAnim(ANIMPACKET_s *packet) {
  if (packet->blending != 0)
    return packet->blend_animation_b;
  return packet->animation_index;
}

// FUNCTION: LEGOBATMAN 0x0059b4a0
i32 AnimSpeedXZ(CHARACTERMODEL_s *model, i32 animation, f32 *x, f32 *z) {
  if (animation != -1 && model->model_data_b[animation] != 0) {
    CHARACTERANIM_s *info = (CHARACTERANIM_s *)model->model_data_a[animation];
    if (info->speed_x != 0.0f || info->action_speed != 0.0f) {
      if (x != 0)
        *x = info->speed_x;
      if (z != 0)
        *z = info->action_speed;
      return 1;
    }
  }
  if (x != 0)
    *x = 0.0f;
  if (z != 0)
    *z = 0.0f;
  return 0;
}

// FUNCTION: LEGOBATMAN 0x0059b540
f32 AnimSpeedZ(CHARACTERMODEL_s *model, i32 animation) {
  if (animation != -1 && model->model_data_b[animation] != 0)
    return ((CHARACTERANIM_s *)model->model_data_a[animation])->action_speed;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0059b570
f32 AnimListFrame(CHARACTERMODEL_s *model, i32 animation, i32 frame) {
  if (animation == -1 || model->model_data_b[animation] == 0 || frame < 0 ||
      frame > 3)
    return 0.0f;
  CHARACTERANIM_s *info = (CHARACTERANIM_s *)model->model_data_a[animation];
  return info->event_frames[frame];
}

// FUNCTION: LEGOBATMAN 0x0059b5b0
f32 AnimStopFrame(CHARACTERMODEL_s *model, i32 animation) {
  if (animation != -1 && model->model_data_b[animation] != 0)
    return ((CHARACTERANIM_s *)model->model_data_a[animation])->stop_frame;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x0059b5e0
void AnimListFrameArray(CHARACTERMODEL_s *model, i32 animation, f32 *frames) {
  if (animation != -1 && model->model_data_b[animation] != 0) {
    CHARACTERANIM_s *info = (CHARACTERANIM_s *)model->model_data_a[animation];
    frames[0] = info->event_frames[0];
    frames[1] = info->event_frames[1];
    frames[2] = info->event_frames[2];
    frames[3] = info->event_frames[3];
  }
}

struct APICHARACTERMODELLIST_s {
  i16 model_id; // 0x00
  i16 pad02;
};

// FUNCTION: LEGOBATMAN 0x0059c3d0
i32 InModelList(APICHARACTERMODELLIST_s *list, i32 id, i32 *out_index) {
  if (list != 0) {
    i32 i = 0;
    for (; list->model_id != -1; list++, i++) {
      if (list->model_id == id) {
        if (out_index != 0)
          *out_index = i;
        return 1;
      }
    }
  }
  if (out_index != 0)
    *out_index = -1;
  return 0;
}

struct ACTIONINFO_s {
  char *name; // 0x00
  u32 flags;  // 0x04
};

struct EXTRAACTIONDATA_s {
  char *name; // 0x00
  i32 action; // 0x04
};

// GLOBAL: LEGOBATMAN 0x00a94780
ACTIONINFO_s *APIActionInfo;

// GLOBAL: LEGOBATMAN 0x00a94784
EXTRAACTIONDATA_s *APIExtraActionData;

// FUNCTION: LEGOBATMAN 0x0059c430
void SetActionInfo(ACTIONINFO_s *action_info,
                   EXTRAACTIONDATA_s *extra_action_data) {
  APIActionInfo = action_info;
  APIExtraActionData = extra_action_data;
}

// FUNCTION: LEGOBATMAN 0x0059c450
u32 ActionInfoFlags(i32 action) {
  if (APIActionInfo != 0 && action >= 0 &&
      action < apicharsys->model_id_capacity)
    return APIActionInfo[action].flags;
  return 0;
}

// FUNCTION: LEGOBATMAN 0x0059c480
const char *ActionInfoName(i32 action) {
  if (APIActionInfo != 0 && action >= 0 &&
      action < apicharsys->model_id_capacity)
    return APIActionInfo[action].name;
  return "?";
}

// STUB: LEGOBATMAN 0x0059c4b0
// close: orig tests extra->name with cmp [esi],0 and reloads it for the
// NuStrICmp call (check-then-reload); ours carries it in eax.
i32 ActionFromName(const char *name) {
  if (apicharsys == 0)
    return -1;

  if (APIActionInfo != 0) {
    for (i32 action = 0; action < apicharsys->model_id_capacity; ++action) {
      if (NuStrICmp(APIActionInfo[action].name, name) == 0)
        return action;
    }
  }

  if (APIExtraActionData != 0) {
    EXTRAACTIONDATA_s *extra = APIExtraActionData;
    while (extra->name != 0) {
      if (NuStrICmp(extra->name, name) == 0)
        return extra->action;
      extra++;
    }
  }
  return -1;
}

// FUNCTION: LEGOBATMAN 0x0059c530
u32 ParseAnimStance(char *name, i32 alternate) {
  if (NuStrICmp(name, "left") == 0)
    return alternate != 0 ? 0x8000 : 0x40000;
  if (NuStrICmp(name, "right") == 0)
    return alternate != 0 ? 0x10000 : 0x80000;
  return alternate != 0 ? 0x4000 : 0x20000;
}

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_apiobject_unk(f32 *v, f32 a, i32 i) {
  v[2] = NuFabs(a);
  v[3] = NuFdiv(a, v[4]);
  v[0] = NuSinApprox(i);
  v[1] = NuCosApprox(i);
}
