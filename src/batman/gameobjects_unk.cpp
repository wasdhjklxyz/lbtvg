// batman/gameobjects_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nu2api/nucore/common.h"
#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"
#include <stddef.h>

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x004a33f0
static f32 NuSinApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x004a34b0
static f32 NuCosApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x004a34d0
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

// STUB: LEGOBATMAN 0x004a3630
// skipped: esi/edi register arguments (static helper of a same-TU caller);
// match with its caller.
#if 0
#include "../gameapi/gameobject_unk.h"

struct GAMECHARACTERDATA_s {
    MAKELAYERLISTFN make_layer_list; // 0x00
    GAMECHARACTERLAYER_s *layers;    // 0x04
    union {
        u32 field_0x08;
        i8 *layer_lookup;
    };
    f32 field_0x0c;
    f32 field_0x10;
    union {
        f32 field_0x14;
        f32 tiptoe_speed;
    };
    union {
        f32 field_0x18;
        f32 walk_speed;
    };
    union {
        f32 movement_speed;
        f32 run_speed; // 0x1c, ordinary directional top speed
    };
    union {
        f32 field_0x20;
        f32 backwards_speed_multiplier;
    };
    union {
        f32 field_0x24;
        f32 gravity;
    };
    f32 field_0x28;
    union {
        f32 field_0x2c;
        f32 jump_speed;
    };
    union {
        f32 field_0x30;
        f32 jump_duration;
    };
    union {
        f32 field_0x34;
        f32 jump_height;
    };
    union {
        f32 field_0x38;
        f32 second_jump_speed;
    };
    union {
        f32 field_0x3c;
        f32 second_jump_duration;
    };
    union {
        f32 field_0x40;
        f32 second_jump_height;
    };
    union {
        f32 field_0x44;
        f32 velocity_seek_rate;
    };
    f32 field_0x48;
    f32 field_0x4c;
    f32 field_0x50;
    f32 field_0x54;
    f32 field_0x58;
    f32 field_0x5c;
    f32 field_0x60;
    f32 viewdistance;  // 0x64
    f32 heardistance;  // 0x68
    f32 maxviewheight; // 0x6c
    f32 minviewheight; // 0x70
    union {
        f32 field_0x74;
        f32 turn_rate;
    };
    f32 field_0x78;
    f32 field_0x7c;
    f32 field_0x80;
    f32 field_0x84;
    f32 field_0x88;
    f32 field_0x8c;
    u32 flags_090;
    union {
        u32 field_0x94;
        u8 flags_094[4];
    };
    union {
        u32 field_0x98;
        u8 flags_098[4];
    };
    u32 layer_mask_special; // 0x9c
    u32 layer_mask;         // 0xa0, high-detail hierarchy render-part visibility mask
    u32 layer_mask_medium;  // 0xa4
    u32 layer_mask_low;     // 0xa8
    u32 layer_mask_dead;    // 0xac
    u32 ride_layers_off;    // 0xb0
    f32 field_0xb4;
    f32 field_0xb8;
    union {
        f32 field_0xbc;
        f32 ai_update_distance_0;
    };
    union {
        f32 field_0xc0;
        f32 ai_update_distance_1;
    };
    union {
        u32 field_0xc4;
        f32 ai_update_distance_2;
    };
    union {
        u32 field_0xc8;
        f32 ai_update_distance_3;
    };
    union {
        u32 field_0xcc;
        struct {
            u8 ai_update_interval_0;
            u8 ai_update_interval_1;
            u8 ai_update_interval_2;
            u8 ai_update_interval_3;
        };
    };
    union {
        struct {
            u32 field_0xd0;
            u32 field_0xd4;
            u32 field_0xd8;
        };
        i16 sfx_misc[6];
    };
    union {
        u32 field_0xdc;
        struct {
            i16 sfx_die;
            i16 sfx_hurt;
        };
    };
    union {
        u32 field_0xe0;
        struct {
            i16 sfx_grunt;
            i16 sfx_engine;
        };
    };
    union {
        u32 field_0xe4;
        struct {
            i16 sfx_shoot;
            i16 sfx_footstep;
        };
    };
    union {
        u32 field_0xe8;
        struct {
            i16 sfx_chatter;
            i16 sfx_sabre;
        };
    };
    union {
        u32 field_0xec;
        struct {
            i16 weapon_model;
            union {
                i16 field_0xee;
                u16 score;
            };
        };
    };
    union {
        u32 field_0xf0;
        struct {
            u16 shadow_locators;
            u16 thrust_locators;
        };
    };
    u8 hitpoints; // 0xf4
    u8 field_0xf5;
    u8 field_0xf6;
    u8 field_0xf7;
    union {
        u32 field_0xf8;
        i8 weapon_joints[4];
    };
    union {
        u32 field_0xfc;
        i8 weapon_shoot_joints[4];
    };
    union {
        struct {
            u32 field_0x100;
            u32 field_0x104;
        };
        i8 streak_joints[4][2];
    };
    union {
        u32 field_0x108;
        struct {
            i8 hand_locators[2];
            i8 grapple_locators[2];
        };
    };
    union {
        u32 field_0x10c;
        struct {
            i8 rocket_locator;
            i8 shield_locator;
            i8 head_locator;
            i8 collision_locator;
        };
    };
    union {
        u32 field_0x110;
        struct {
            i8 thingy_locator;
            i8 helmet_locator;
            i8 throw_locator;
            i8 ride_locator;
        };
    };
    union {
        u16 field_0x114;
        struct {
            i8 poo_locator;
            i8 hair_layer;
        };
    };
    union {
        u8 field275_0x116;
        u8 uses_weapon_action;
    };
    u8 field_0x117;
    union {
        u32 field_0x118;
        struct {
            i8 head_joint;
            i8 cloak_joint;
            i8 cloak_joint_2;
            i8 place_locator;
        };
    };
    union {
        u16 field_0x11c;
        struct {
            i8 cape_layer;
            i8 extra_character_locator;
        };
    };
    u8 field_0x11e;
    u8 layer_count; // 0x11f
};
typedef struct GAMECHARACTERDATA_s GAMECHARACTERDATA;

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
    u8 pad_0x18[0x20 - 0x18];
    union {
        f32 field_0x20;
        f32 time_secondary;
    }; // 0x20
    u8 pad_0x24[0x30 - 0x24];
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
    u8 pad_0x3f[0x42 - 0x3f];
    union {
        i16 overlay_animation; // -1 when no overlay is active
        u16 frame;
    }; // 0x42
    f32 field_0x44; // 0x44
};

i32 CurrentAnim(ANIMPACKET_s *packet);

i16 id_BUGGY, id_GYROCOPTER, id_BANTHA, id_BOMARRMONK, id_DEWBACK;

i16 id_ATAT, id_SPEEDERBIKE;

i16 id_HEAVYREPEATINGCANNON, id_BIGGUN, id_TROOPERCANNON, id_STAP2, id_CLONEWALKER;

// from saga legoapi/items/objects/gameobjects.cpp
static void TakeOver_SetAction(GameObject_s *rider, GameObject_s *vehicle) {
    i32 action;
    if (vehicle->id == id_BUGGY)
        action = 0x70;
    else if (vehicle->id == id_GYROCOPTER)
        action = 0x83;
    else if (static_cast<GAMECHARACTERDATA *>(vehicle->apiobj.character_data->field11_0x24)->field275_0x116 == 12) {
        action = CurrentAnim(&vehicle->apiobj.anim_packet) == 3 || CurrentAnim(&vehicle->apiobj.anim_packet) == 0x17
                     ? 0xc4
                     : 0xc3;
    } else if (vehicle->id == id_BANTHA || vehicle->id == id_BOMARRMONK)
        action = 0x70;
    else if (vehicle->id == id_DEWBACK)
        action = 0x83;
    else if (vehicle->id == id_LANDSPEEDER || vehicle->id == id_FLASHSPEEDER)
        action = 0x84;
    else if (vehicle->id == id_TAUNTAUN)
        action = 0x86;
    else if (vehicle->id == id_SPEEDERBIKE)
        action = 0xc0;
    else if (vehicle->id == id_HEAVYREPEATINGCANNON || vehicle->id == id_BIGGUN)
        action = 0xc1;
    else if (vehicle->id == id_TROOPERCANNON) {
        action = rider->apiobj.character_model->model_data_b[0xc2] != NULL ? 0xc2 : 0xc1;
    } else if (vehicle->id == id_STAP2)
        action = 0xc1;
    else if (vehicle->id == id_CLONEWALKER)
        action = 0xc0;
    else {
        rider->context_animation = 0x6c;
        return;
    }
    rider->context_animation = rider->apiobj.character_model->model_data_b[action] != NULL ? action : 0x6c;
}
#endif

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_gameobjects_unk(f32 *v, f32 a, i32 i) {
  v[0] = NuSinApprox(i);
  v[1] = NuCosApprox(i);
  NuVec4Set(v, a, a, a, a);
}
