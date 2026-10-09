// gameapi/bolts_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nu2api/nucore/common.h"
#include <stddef.h>

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

// The keyword parsers' view of a bolt type (PC layout).
struct BoltTypeKw_s {
  char name[16]; // 0x00
  f32 speed;     // 0x10
  f32 duration;  // 0x14
  f32 gravity;   // 0x18
  f32 radius;    // 0x1c
  f32 scale;     // 0x20
  f32 scaletime; // 0x24
  u8 pad28[0x3a - 0x28];
  u8 damage; // 0x3a
  u8 pad3b[0x3c - 0x3b];
  i32 rand_angle; // 0x3c
  u8 pad40[0x58 - 0x40];
  unsigned __int64 flags; // 0x58
};

// GLOBAL: LEGOBATMAN 0x00ac7598
extern BoltTypeKw_s *BT_bolttype;

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
