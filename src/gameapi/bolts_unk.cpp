// gameapi/bolts_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nu2api/nucore/common.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

// STUB: LEGOBATMAN 0x005e9790
// two-pass bolttype parser with default fn pointers to same-TU statics; not
// attempted
#if 0
#include "gameobject_unk.h"

#include "../nu2api/numath/numtx.h"

#include "../nu2api/nu3d/nuspecial.h"

struct BOLTSYS {
    BOLTTYPE_s *types;
    i32 count;
    void (*stop_targeting)(GameObject_s *, NUVEC *);
    void (*debris)(BOLT_s *, NUVEC *, i32, NUVEC *, i32);
    void (*shoot_origin)(GameObject_s *, NUVEC *);
    i32 (*shoot_direction)(GameObject_s *, NUVEC *);
    i32 (*hit_part)(BOLT_s *, PART_s *);
    i32 (*alternate_fire)(GameObject_s *, i32);
};

struct BOLT_s {
    BOLTTYPE_s *type;
    GameObject_s *owner;      // 0x04
    NUMTX orientation;        // 0x08, decoded shot matrix
    NUMTX effect_orientation; // 0x48, quarter-turn X composed with shot rotation
    NUVEC position;           // 0x88
    NUVEC previous_position;  // 0x94
    NUVEC velocity;           // 0xa0
    NUVEC field_0xac;
    NUVEC ray_end; // 0xb8, unclipped ray endpoint
    f32 scale;     // 0xc4
    f32 time;      // 0xc8
    f32 speed;
    f32 lifetime;         // 0xd0
    f32 radius;           // 0xd4
    f32 collision_radius; // 0xd8
    f32 ray_time;         // 0xdc
    f32 ray_radius;       // 0xe0
    f32 field_0xe4;
    f32 field_0xe8;
    f32 acceleration_y; // 0xec
    u32 flags;          // 0xf0
    u16 surface_x_rotation;
    u16 surface_z_rotation;
    u16 hit_flags;       // 0xf8, passed to ObjHitObj
    i16 hit_platform;    // 0xfa
    i16 hit_debris;      // 0xfc
    i16 hit_part_debris; // 0xfe
    u8 active;           // 0x100
    u8 type_id;          // 0x101
    u8 field_0x102;
    u8 index; // 0x103
    i8 field_0x104;
    i8 field_0x105;
    u8 pad_0x106[2];
    NUVEC bounds_min;            // 0x108, position minus collision radius
    NUVEC bounds_max;            // 0x114, position plus collision radius
    NUVEC hit_normal;            // 0x120, platform deflection normal
    NUVEC dogfight_hit_position; // 0x12c
};

struct BOLTTYPE_s {
    char name[16];
    f32 field_10;
    f32 field_14;
    f32 field_18;
    f32 field_1c;
    f32 field_20;
    f32 field_24;
    union {
        i32 field_28;
        i16 object_ids[2];
    };
    union {
        i32 field_2c;
        struct {
            i16 shadow_object_id;
            i16 debris_id;
        };
    };
    union {
        i32 field_30;
        i16 moving_debris[2];
    };
    union {
        i32 field_34;
        i16 moving_debris_counts[2];
    };
    i16 field_38;
    i16 field_3a;
    u8 field_3c;
    u8 field_3d;
    u16 field_3e;
    u32 field_40;
    void (*init_callback)(BOLT_s *);              // 0x44
    void (*update_callback)(BOLT_s *);            // 0x48
    void (*end_callback)(BOLT_s *);               // 0x4c
    void (*ricochet_callback)(BOLT_s *, NUVEC *); // 0x50
    f32 (*scale_callback)(BOLT_s *);              // 0x54
    char *shoot_sfx;
    char *hit_sfx;
    u32 field_60;
    i16 shoot_sfx_id;
    i16 hit_sfx_id;
    union {
        u8 pad_68[0x3c];
        struct {
            nuhspecial_s object_special;
            nuhspecial_s glow_special;
            nuhspecial_s reference_object_special;
            nuhspecial_s reference_glow_special;
            nuhspecial_s shadow_special;
        } specials;
    };
};

i32 GetSfxId(const char *name);

BOLTSYS *BoltSys = &BoltSys_Default;

static __used__ void Bolt_Debris_Default(BOLT_s *bolt, nuvec_s *points, int point, nuvec_s *, int);

static __used__ void Bolt_GetShootOrigin_Default(GameObject_s *object, nuvec_s *position);

static __used__ i32 Bolt_GetShootDirection_Default(GameObject_s *object, nuvec_s *direction);

static BOLTSYS BoltSys_Default = {
    &GlobalBoltType_Default,        1,    NULL, Bolt_Debris_Default, Bolt_GetShootOrigin_Default,
    Bolt_GetShootDirection_Default, NULL, NULL};

BOLTTYPE_s GlobalBoltType_Default = {"null", 4.0f, 2.0f, 0.0f, 0.125f, 1.0f, 0.1f, -1,   -1,   -1,   0, -1, 0,  1,
                                     255,    0,    0,    NULL, NULL,   NULL, NULL, NULL, NULL, NULL, 0, -1, -1, {}};

// from saga legoapi/items/collect/bolts.cpp
void BoltSys_Init(BOLTSYS *system) {
    for (i32 i = 0; i < system->count; ++i) {
        BOLTTYPE_s *type = &system->types[i];
        type->shoot_sfx_id = -1;
        if (type->shoot_sfx != NULL) {
            type->shoot_sfx_id = GetSfxId(type->shoot_sfx);
            type = &system->types[i];
        }
        type->hit_sfx_id = -1;
        if (type->hit_sfx != NULL)
            type->hit_sfx_id = GetSfxId(type->hit_sfx);
    }
    BoltSys = system;
    if (system->debris == NULL)
        system->debris = Bolt_Debris_Default;
    if (system->shoot_origin == NULL)
        system->shoot_origin = Bolt_GetShootOrigin_Default;
    if (system->shoot_direction == NULL)
        system->shoot_direction = Bolt_GetShootDirection_Default;
}
#endif

typedef struct nufpar_s NUFPAR;
f32 NuFParGetFloat(NUFPAR *parser);
i32 NuFParGetInt(NUFPAR *parser);

struct nufpar_s {
  unsigned char pad0[0x910];
  char *word_buf; // 0x910
};
struct nugscn_s;
struct nuhspecial_s {
  void *scene;
  void *special;
  void *display_special;
};

// The keyword parsers' view of a bolt type (PC layout).
struct BoltTypeKw_s {
  char name[16];              // 0x00
  f32 speed;                  // 0x10
  f32 duration;               // 0x14
  f32 gravity;                // 0x18
  f32 radius;                 // 0x1c
  f32 scale;                  // 0x20
  f32 scaletime;              // 0x24
  i16 debris_shoot;           // 0x28
  i16 debris_hit[3];          // 0x2a
  i16 debris_moving[2];       // 0x30
  i16 debris_moving_count[2]; // 0x34
  i16 part_hit;               // 0x38
  u8 damage;                  // 0x3a
  u8 pad3b[0x3c - 0x3b];
  i32 rand_angle; // 0x3c
  u8 pad40[0x58 - 0x40];
  unsigned __int64 flags; // 0x58
  i16 sfx_shoot;          // 0x60
  i16 sfx_hit;            // 0x62
  f32 target_dist_near;   // 0x64, squared
  f32 target_dist_mid;    // 0x68, squared
  u16 target_deg_near;    // 0x6c
  u16 target_deg_mid;     // 0x6e
  u16 target_deg_far;     // 0x70
  u8 pad72[0x74 - 0x72];
  nuhspecial_s obj;          // 0x74
  nuhspecial_s glow_obj;     // 0x80
  nuhspecial_s ref_obj;      // 0x8c
  nuhspecial_s ref_glow_obj; // 0x98
  nuhspecial_s shadow_obj;   // 0xa4
};

// GLOBAL: LEGOBATMAN 0x00ac7598
static BoltTypeKw_s *BT_bolttype;

static inline f32 NuFabs(f32 f) {
  u32 bits = *(u32 *)&f & 0x7fffffff;
  return *(f32 *)&bits;
}

// FUNCTION: LEGOBATMAN 0x005e8790
void BT_speed(NUFPAR *parser) { BT_bolttype->speed = NuFParGetFloat(parser); }

// FUNCTION: LEGOBATMAN 0x005e87b0
void BT_duration(NUFPAR *parser) {
  BT_bolttype->duration = NuFParGetFloat(parser);
}

// FUNCTION: LEGOBATMAN 0x005e87d0
void BT_radius(NUFPAR *parser) { BT_bolttype->radius = NuFParGetFloat(parser); }

// FUNCTION: LEGOBATMAN 0x005e87f0
void BT_scale(NUFPAR *parser) { BT_bolttype->scale = NuFParGetFloat(parser); }

// FUNCTION: LEGOBATMAN 0x005e8810
void BT_scaletime(NUFPAR *parser) {
  BT_bolttype->scaletime = NuFabs(NuFParGetFloat(parser));
}

// FUNCTION: LEGOBATMAN 0x005e8840
void BT_gravity(NUFPAR *parser) {
  BT_bolttype->gravity = NuFParGetFloat(parser);
}

// FUNCTION: LEGOBATMAN 0x005e8860
void BT_rand_angle(NUFPAR *parser) {
  BT_bolttype->rand_angle = (i32)(NuFParGetFloat(parser) * 182.04445f);
}

// FUNCTION: LEGOBATMAN 0x005e8ed0
void BT_damage(NUFPAR *parser) { BT_bolttype->damage = NuFParGetInt(parser); }

// FUNCTION: LEGOBATMAN 0x005e8ef0
void BT_nodeflect(NUFPAR *parser) {
  BoltTypeKw_s *bolt_type = BT_bolttype;
  bolt_type->flags |= 0x100ull;
}

// FUNCTION: LEGOBATMAN 0x005e8f10
void BT_converge(NUFPAR *parser) {
  BoltTypeKw_s *bolt_type = BT_bolttype;
  bolt_type->flags |= 0x400ull;
}

// FUNCTION: LEGOBATMAN 0x005e8f30
void BT_canonlyhitplayers(NUFPAR *parser) {
  BoltTypeKw_s *bolt_type = BT_bolttype;
  bolt_type->flags |= 0x80ull;
}

// FUNCTION: LEGOBATMAN 0x005e8f50
void BT_no_terrain(NUFPAR *parser) {
  BoltTypeKw_s *bolt_type = BT_bolttype;
  bolt_type->flags |= 4ull;
}

// FUNCTION: LEGOBATMAN 0x005e8f60
void BT_no_collide(NUFPAR *parser) {
  BoltTypeKw_s *bolt_type = BT_bolttype;
  bolt_type->flags |= 0x10000ull;
}

// FUNCTION: LEGOBATMAN 0x005e8f80
void BT_trooper_bolt(NUFPAR *parser) {
  BoltTypeKw_s *bolt_type = BT_bolttype;
  bolt_type->flags |= 0x80000ull;
}

// FUNCTION: LEGOBATMAN 0x005e8fa0
void BT_single_debris(NUFPAR *parser) {
  BoltTypeKw_s *bolt_type = BT_bolttype;
  bolt_type->flags |= 0x20000000ull;
}

// FUNCTION: LEGOBATMAN 0x005e8fc0
void BT_bolt_water(NUFPAR *parser) {
  BoltTypeKw_s *bolt_type = BT_bolttype;
  bolt_type->flags |= 0x200000000ull;
}

// FUNCTION: LEGOBATMAN 0x005e8fd0
void BT_torpedo(NUFPAR *parser) {
  BoltTypeKw_s *bolt_type = BT_bolttype;
  bolt_type->flags |= 0x400000000ull;
}

// FUNCTION: LEGOBATMAN 0x005e8fe0
void BT_typea(NUFPAR *parser) {
  BoltTypeKw_s *bolt_type = BT_bolttype;
  bolt_type->flags |= 0x4000000000ull;
}

// FUNCTION: LEGOBATMAN 0x005e8ff0
void BT_typeb(NUFPAR *parser) {
  BoltTypeKw_s *bolt_type = BT_bolttype;
  bolt_type->flags |= 0x8000000000ull;
}

// FUNCTION: LEGOBATMAN 0x005e9010
void BT_typec(NUFPAR *parser) {
  BoltTypeKw_s *bolt_type = BT_bolttype;
  bolt_type->flags |= 0x10000000000ull;
}

// FUNCTION: LEGOBATMAN 0x005e9030
void BT_typed(NUFPAR *parser) {
  BoltTypeKw_s *bolt_type = BT_bolttype;
  bolt_type->flags |= 0x20000000000ull;
}

struct BoltFlagName_s {
  char *name;
  u32 pad4;
  unsigned __int64 flag;
};

i32 NuFParGetWord(NUFPAR *parser);
i32 NuStrLen(const char *s);
i32 NuStrCpy(char *dst, const char *src);
int NuStrICmp(const char *a, const char *b);
i32 NuSpecialFind(nugscn_s *scene, nuhspecial_s *dest, char *name, i32 flags);
i32 GetSfxId(char *name);
i32 PARTLookupType(char *name);
i32 Unk0055fe40(char *name);

struct BTWorld_s {
  u8 pad0[0x138];
  i32 i138; // 0x138
  u8 pad13c[0x140 - 0x13c];
  nugscn_s *current_gscn; // 0x140
  u8 pad144[0x4c28 - 0x144];
  BoltTypeKw_s bolt_types[8]; // 0x4c28
};

// GLOBAL: LEGOBATMAN 0x00ac7590
extern nugscn_s *BT_scene;
// GLOBAL: LEGOBATMAN 0x00ac75a8
extern BTWorld_s *BT_worldinfo;
// GLOBAL: LEGOBATMAN 0x00ac75ac
extern i32 g_unk00ac75ac;
// GLOBAL: LEGOBATMAN 0x00ac75a4
static i32 BT_gdeb_moving_count;
// GLOBAL: LEGOBATMAN 0x00a958ac
extern nugscn_s *things_scene;
extern nugscn_s *area_scene;
extern nugscn_s *vehicle_scene;
// GLOBAL: LEGOBATMAN 0x00961d10
extern BoltFlagName_s g_unk00961d10[];

// GLOBAL: LEGOBATMAN 0x00961c60
extern BoltTypeKw_s GlobalBoltType_Default;
// GLOBAL: LEGOBATMAN 0x00961f10
extern u8 BoltType_ConfigKeywords[];

NUFPAR *NuFParCreateMem(char *name, char *buffer, i32 size);
i32 NuFParPushCom(NUFPAR *parser, void *commands);
i32 NuFParGetLine(NUFPAR *parser);
i32 NuFParInterpretWord(NUFPAR *parser);
void NuFParDestroy(NUFPAR *parser);

// FUNCTION: LEGOBATMAN 0x005e8700
// Adds a bolt type to the first free world slot (inlined on the Mac); type in
// eax, the world on the stack.
static void Unk005e8700(BoltTypeKw_s *type, BTWorld_s *world) {
  if (type == 0)
    return;
  for (i32 i = 0; i < 8; i++) {
    if (NuStrLen(world->bolt_types[i].name) == 0) {
      world->bolt_types[i] = *type;
      return;
    }
  }
}

// FUNCTION: LEGOBATMAN 0x005e8750
void BT_name(NUFPAR *parser) {
  if (NuFParGetWord(parser) != 0 && NuStrLen(parser->word_buf) <= 15)
    NuStrCpy(BT_bolttype->name, parser->word_buf);
}

// FUNCTION: LEGOBATMAN 0x005e8890
void BT_sceneconfig(NUFPAR *parser) {
  nugscn_s *scene;
  BT_scene = things_scene;
  if (NuFParGetWord(parser) == 0)
    return;
  if (NuStrICmp(parser->word_buf, "level") == 0) {
    if (BT_worldinfo == 0)
      return;
    scene = BT_worldinfo->current_gscn;
  } else if (NuStrICmp(parser->word_buf, "area") == 0) {
    scene = area_scene;
  } else if (NuStrICmp(parser->word_buf, "vehicle") == 0) {
    scene = vehicle_scene;
  } else {
    return;
  }
  if (scene != 0)
    BT_scene = scene;
}

// FUNCTION: LEGOBATMAN 0x005e8920
void BT_obj(NUFPAR *parser) {
  if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "none") != 0 &&
      BT_scene != 0) {
    if (NuSpecialFind(BT_scene, &BT_bolttype->obj, parser->word_buf, 1) != 0)
      BT_bolttype->ref_obj = BT_bolttype->obj;
  }
}

// FUNCTION: LEGOBATMAN 0x005e89a0
void BT_ref_obj(NUFPAR *parser) {
  if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "none") != 0 &&
      BT_scene != 0)
    NuSpecialFind(BT_scene, &BT_bolttype->ref_obj, parser->word_buf, 1);
}

// FUNCTION: LEGOBATMAN 0x005e8a00
void BT_glow_obj(NUFPAR *parser) {
  if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "none") != 0 &&
      BT_scene != 0) {
    if (NuSpecialFind(BT_scene, &BT_bolttype->glow_obj, parser->word_buf, 1) !=
        0)
      BT_bolttype->ref_glow_obj = BT_bolttype->glow_obj;
  }
}

// FUNCTION: LEGOBATMAN 0x005e8a80
void BT_ref_glow_obj(NUFPAR *parser) {
  if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "none") != 0 &&
      BT_scene != 0)
    NuSpecialFind(BT_scene, &BT_bolttype->ref_glow_obj, parser->word_buf, 1);
}

// FUNCTION: LEGOBATMAN 0x005e8ae0
void BT_shadow_obj(NUFPAR *parser) {
  if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "none") != 0 &&
      BT_scene != 0)
    NuSpecialFind(BT_scene, &BT_bolttype->shadow_obj, parser->word_buf, 1);
}

// FUNCTION: LEGOBATMAN 0x005e8b40
void BT_debris_shoot(NUFPAR *parser) {
  if (g_unk00ac75ac != 0 && NuFParGetWord(parser) != 0 &&
      NuStrICmp(parser->word_buf, "none") != 0)
    BT_bolttype->debris_shoot = Unk0055fe40(parser->word_buf);
}

// FUNCTION: LEGOBATMAN 0x005e8b90
void BT_debris_hit(NUFPAR *parser) {
  i32 i;
  if (g_unk00ac75ac != 0 && NuFParGetWord(parser) != 0 &&
      NuStrICmp(parser->word_buf, "none") != 0) {
    for (i = 0; i < 3; i++) {
      if (BT_bolttype->debris_hit[i] == -1)
        break;
    }
    if (i < 3)
      BT_bolttype->debris_hit[i] = Unk0055fe40(parser->word_buf);
  }
}

// FUNCTION: LEGOBATMAN 0x005e8c10
void BT_debris_moving(NUFPAR *parser) {
  if (g_unk00ac75ac != 0 && BT_gdeb_moving_count < 2 &&
      NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "none") != 0) {
    BT_bolttype->debris_moving[BT_gdeb_moving_count] =
        Unk0055fe40(parser->word_buf);
    i32 count = (i32)NuFParGetFloat(parser);
    BT_bolttype->debris_moving_count[BT_gdeb_moving_count] = abs(count);
    BT_gdeb_moving_count++;
  }
}

// FUNCTION: LEGOBATMAN 0x005e8ca0
void BT_part_hit(NUFPAR *parser) {
  if (BT_worldinfo != 0 && NuFParGetWord(parser) != 0 &&
      NuStrICmp(parser->word_buf, "none") != 0)
    BT_bolttype->part_hit = PARTLookupType(parser->word_buf);
}

// FUNCTION: LEGOBATMAN 0x005e8cf0
void BT_sfx_shoot(NUFPAR *parser) {
  if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "none") != 0)
    BT_bolttype->sfx_shoot = GetSfxId(parser->word_buf);
}

// FUNCTION: LEGOBATMAN 0x005e8d40
void BT_sfx_hit(NUFPAR *parser) {
  if (NuFParGetWord(parser) != 0) {
    if (NuStrICmp(parser->word_buf, "none") != 0)
      BT_bolttype->sfx_hit = GetSfxId(parser->word_buf);
    else
      BT_bolttype->sfx_hit = -2;
  }
}

// FUNCTION: LEGOBATMAN 0x005e8da0
void BT_TargetDist_Near(NUFPAR *parser) {
  f64 d = NuFParGetFloat(parser);
  BT_bolttype->target_dist_near = d * d;
}

// FUNCTION: LEGOBATMAN 0x005e8dc0
void BT_TargetDist_Mid(NUFPAR *parser) {
  f64 d = NuFParGetFloat(parser);
  BT_bolttype->target_dist_mid = d * d;
}

// FUNCTION: LEGOBATMAN 0x005e8de0
void BT_TargetDeg_Near(NUFPAR *parser) {
  BT_bolttype->target_deg_near = (u16)(NuFParGetFloat(parser) * 182.04445f);
}

// FUNCTION: LEGOBATMAN 0x005e8e30
void BT_TargetDeg_Mid(NUFPAR *parser) {
  BT_bolttype->target_deg_mid = (u16)(NuFParGetFloat(parser) * 182.04445f);
}

// FUNCTION: LEGOBATMAN 0x005e8e80
void BT_TargetDeg_Far(NUFPAR *parser) {
  BT_bolttype->target_deg_far = (u16)(NuFParGetFloat(parser) * 182.04445f);
}

// FUNCTION: LEGOBATMAN 0x005e9050
void BT_NoOffscreenCull(NUFPAR *parser) {
  BoltTypeKw_s *bolt_type = BT_bolttype;
  bolt_type->flags |= 0x20ull;
}

// FUNCTION: LEGOBATMAN 0x005e9060
void BT_flags(NUFPAR *parser) {
  while (NuFParGetWord(parser) != 0) {
    for (BoltFlagName_s *f = g_unk00961d10; f->name != 0; f++) {
      if (NuStrICmp(parser->word_buf, f->name) == 0) {
        BoltTypeKw_s *bolt_type = BT_bolttype;
        bolt_type->flags |= f->flag;
        break;
      }
    }
  }
}

// STUB: LEGOBATMAN 0x005eabf0
// orig keeps reading_type and the parser in stack slots; ours uses edi
void BoltTypes_Configure(BTWorld_s *world, char *config) {
  memset(world->bolt_types, 0, sizeof(world->bolt_types));
  NUFPAR *parser = NuFParCreateMem("bolttypes", config, 0xffff);
  if (parser == 0)
    return;
  NuFParPushCom(parser, BoltType_ConfigKeywords);
  BoltTypeKw_s type;
  i32 reading_type = 0;
  while (NuFParGetLine(parser)) {
    if (NuFParGetWord(parser) == 0)
      continue;
    if (reading_type) {
      if (NuStrICmp(parser->word_buf, "bolttype_end") == 0) {
        reading_type = 0;
        if (type.name[0] != 0)
          Unk005e8700(&type, world);
      } else {
        NuFParInterpretWord(parser);
      }
    } else if (NuStrICmp(parser->word_buf, "bolttype_start") == 0) {
      BT_scene = things_scene;
      BT_bolttype = &type;
      type = GlobalBoltType_Default;
      reading_type = 1;
      BT_worldinfo = world;
      g_unk00ac75ac = world->i138;
      type.name[0] = 0;
      type.debris_moving[0] = -1;
      type.debris_moving[1] = -1;
      BT_gdeb_moving_count = 0;
    }
  }
  NuFParDestroy(parser);
}
