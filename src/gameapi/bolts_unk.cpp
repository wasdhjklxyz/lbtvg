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
