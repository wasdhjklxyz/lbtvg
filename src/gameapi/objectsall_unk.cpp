// gameapi/objectsall_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nu2api/nucore/common.h"
#include <stddef.h>

// STUB: LEGOBATMAN 0x0060ea60
// switch on object kind compiles to a jump table; not attempted
#if 0
#include "../batman/worldinfo_unk.h"

typedef size_t usize;

typedef struct LEVEL_OBJECT_RUNTIME_s {
    nuhspecial_s special; // 0x00
    i16 platform_id;      // 0x0c
    u8 active;            // 0x0e
    u8 pad_0f;
} LEVEL_OBJECT_RUNTIME;

typedef struct LEVELOBJECT {
    u8 kind;
    u8 pad_01;
    u16 reflection;
    char *name;
} LEVELOBJECT;

typedef struct nugscn_s {
    i32 *texture_ids;
    i32 ntextures;
    struct nunativetex_s **textures;
    struct numtl_s **mtls;
    i32 nummtl;
    undefined field8_0x14;
    undefined field9_0x15;
    undefined field10_0x16;
    undefined field11_0x17;
    undefined field12_0x18;
    undefined field13_0x19;
    undefined field14_0x1a;
    undefined field15_0x1b;
    i32 num_instances; // 0x1c
    u8 *instances;     // 0x20, 0x50-byte legacy instance records
    i32 numspecial;
    struct nuspecial_s *specials;
    undefined field26_0x2c;
    undefined field27_0x2d;
    undefined field28_0x2e;
    undefined field29_0x2f;
    i32 numsplines;
    struct nugspline_s *splines;
    undefined pad_38[8];
    struct nugscn_s **additional_scenes;         // 0x40
    i32 rendered_additional_scene_count;         // 0x44
    i16 num_instance_animations;                 // 0x48
    i16 num_instance_animation_data;             // 0x4a
    struct nuinstanim_s *instance_animations;    // 0x4c, 0x60-byte entries
    struct numtx_s *instance_animation_matrices; // 0x50, 0x40-byte entries
    nuanimdata_s **instance_animation_data;      // 0x54
    i32 num_instance_ids;                        // 0x58
    void *instance_ids;                          // 0x5c, borrowed file data; record format unresolved
    i32 num_texture_anims;
    void *texture_anims;
    u16 *texture_anim_ids;
    u32 max_portals;   // 0x6c
    NUPORTAL *portals; // 0x70, 0x20-byte entries
    i32 num_rooms;     // 0x74
    NUROOM *rooms;     // 0x78, 0x18-byte entries
    undefined pad_7c[0x20];
    struct NUFRUSTRUM *portal_frusta[16]; // 0x9c, traversal work list
    i32 num_portal_frusta;                // 0xdc
    i32 camera_room;                      // 0xe0
    i32 portal_depth;                     // 0xe4, maximum recursive depth
    undefined pad_e8[0x0c];
    i32 portal_instance_count;      // 0xf4, nonzero when portal visibility data is present
    NUPORTALSPHERE *portal_spheres; // 0xf8, one 0x10-byte sphere per instance
    NUPORTALBOX *portal_boxes;      // 0xfc, one 0x20-byte box per instance
    undefined pad_100[0x10];
    struct nudisplayscene_s *display_list;
    undefined pad_114[0x0c];
    i32 visibility_result_instance_count; // 0x120
    undefined pad_124[4];
    struct nugscn_s *visibility_source_scene; // 0x128
    void *occlusion_data;                     // 0x12c
    undefined pad_130[4];
    void *instance_visibility_tree;     // 0x134
    void *portal_visibility_marker;     // 0x138, enables the portal result at 0x13c
    u8 *instance_visibility_flags;      // 0x13c, shared portal-visibility result buffer
    void *visibility_context;           // 0x140
    u8 *instance_tree_visibility_flags; // 0x144
    u8 visibility_state;                // 0x148
    undefined field302_0x149;
    undefined field303_0x14a;
    undefined field304_0x14b;
    undefined field305_0x14c;
    undefined field306_0x14d;
    undefined field307_0x14e;
    undefined field308_0x14f;
    undefined field309_0x150;
    undefined field310_0x151;
    undefined field311_0x152;
    undefined field312_0x153;
    undefined field313_0x154;
    undefined field314_0x155;
    undefined field315_0x156;
    undefined field316_0x157;
    i32 shader_texture_animation_enabled; // 0x158
    undefined field321_0x15c;
    undefined field322_0x15d;
    undefined field323_0x15e;
    undefined field324_0x15f;
    undefined field325_0x160;
    undefined field326_0x161;
    undefined field327_0x162;
    undefined field328_0x163;
    undefined field329_0x164;
    undefined field330_0x165;
    undefined field331_0x166;
    undefined field332_0x167;
    undefined field333_0x168;
    undefined field334_0x169;
    undefined field335_0x16a;
    undefined field336_0x16b;
    undefined field337_0x16c;
    undefined field338_0x16d;
    undefined field339_0x16e;
    undefined field340_0x16f;
    undefined field341_0x170;
    undefined field342_0x171;
    undefined field343_0x172;
    undefined field344_0x173;
    undefined field345_0x174;
    undefined field346_0x175;
    undefined field347_0x176;
    undefined field348_0x177;
    undefined field349_0x178;
    undefined field350_0x179;
    undefined field351_0x17a;
    undefined field352_0x17b;
    undefined field353_0x17c;
    undefined field354_0x17d;
    undefined field355_0x17e;
    undefined field356_0x17f;
    undefined field357_0x180;
    undefined field358_0x181;
    undefined field359_0x182;
    undefined field360_0x183;
    undefined field361_0x184;
    undefined field362_0x185;
    undefined field363_0x186;
    undefined field364_0x187;
    undefined field365_0x188;
    undefined field366_0x189;
    undefined field367_0x18a;
    undefined field368_0x18b;
    undefined field369_0x18c;
    undefined field370_0x18d;
    undefined field371_0x18e;
    undefined field372_0x18f;
    undefined field373_0x190;
    undefined field374_0x191;
    undefined field375_0x192;
    undefined field376_0x193;
    undefined field377_0x194;
    undefined field378_0x195;
    undefined field379_0x196;
    undefined field380_0x197;
    undefined field381_0x198;
    undefined field382_0x199;
    undefined field383_0x19a;
    undefined field384_0x19b;
    undefined field385_0x19c;
    undefined field386_0x19d;
    undefined field387_0x19e;
    undefined field388_0x19f;
    undefined field389_0x1a0;
    undefined field390_0x1a1;
    undefined field391_0x1a2;
    undefined field392_0x1a3;
    undefined field393_0x1a4;
    undefined field394_0x1a5;
    undefined field395_0x1a6;
    undefined field396_0x1a7;
    undefined field397_0x1a8;
    undefined field398_0x1a9;
    undefined field399_0x1aa;
    undefined field400_0x1ab;
    undefined field401_0x1ac;
    undefined field402_0x1ad;
    undefined field403_0x1ae;
    undefined field404_0x1af;
    undefined field405_0x1b0;
    undefined field406_0x1b1;
    undefined field407_0x1b2;
    undefined field408_0x1b3;
    undefined field409_0x1b4;
    undefined field410_0x1b5;
    undefined field411_0x1b6;
    undefined field412_0x1b7;
    undefined field413_0x1b8;
    undefined field414_0x1b9;
    undefined field415_0x1ba;
    undefined field416_0x1bb;
    undefined field417_0x1bc;
    undefined field418_0x1bd;
    undefined field419_0x1be;
    undefined field420_0x1bf;
    undefined field421_0x1c0;
    undefined field422_0x1c1;
    undefined field423_0x1c2;
    undefined field424_0x1c3;
    undefined field425_0x1c4;
    undefined field426_0x1c5;
    undefined field427_0x1c6;
    undefined field428_0x1c7;
    undefined field429_0x1c8;
    undefined field430_0x1c9;
    undefined field431_0x1ca;
    undefined field432_0x1cb;
    undefined field433_0x1cc;
    undefined field434_0x1cd;
    undefined field435_0x1ce;
    undefined field436_0x1cf;
    struct nunativegscene_s *field437_0x1d0;
    undefined field438_0x1d4;
    undefined field439_0x1d5;
    undefined field440_0x1d6;
    undefined field441_0x1d7;
    undefined field442_0x1d8;
    undefined field443_0x1d9;
    undefined field444_0x1da;
    undefined field445_0x1db;
    undefined field446_0x1dc;
    undefined field447_0x1dd;
    undefined field448_0x1de;
    undefined field449_0x1df;
    nuanimendlookup_s *animation_end_frames; // 0x1e0
    undefined field454_0x1e4;
    undefined field455_0x1e5;
    undefined field456_0x1e6;
    undefined field457_0x1e7;
    undefined field458_0x1e8;
    undefined field459_0x1e9;
    undefined field460_0x1ea;
    undefined field461_0x1eb;
    undefined field462_0x1ec;
    undefined field463_0x1ed;
    undefined field464_0x1ee;
    undefined field465_0x1ef;
    undefined field466_0x1f0;
    undefined field467_0x1f1;
    undefined field468_0x1f2;
    undefined field469_0x1f3;
    undefined field470_0x1f4;
    undefined field471_0x1f5;
    undefined field472_0x1f6;
    undefined field473_0x1f7;
} NUGSCN;
typedef nugscn_s NUGSCN;

struct nugscn_s *IconScene_FindById(i32 character_id);

typedef struct AREADATA_s {
    char dir[64];
    char file[32];
    i16 levels[12];

    i16 name_id;

    u16 flags;

    byte index;
    byte level_count;
    byte cheat;
    u8 super_counter_count;
    SUPERCOUNTER *super_counters;
    u16 challenge_trial_time;
    u8 episode_index;
    byte area_index;
    i16 area_music;
    i16 minikit_id;
    i32 true_hero_targets[2]; // story and free-play coin thresholds
    i16 text_id;
    byte text_id_value;
    byte field41_0x97;
    i16 *hub_player_ids; // 0x98, optional extra hub characters terminated by -1
} AREADATA;

typedef uint8_t u8;
typedef u8 undefined;

LEVELOBJECT *ObjTabList;

i32 LEVELOBJECTCOUNT;

i32 NuStrICmp(const char *a, const char *b);

i32 KNOBS;

NUGSCN *area_scene;

i32 CHARCOUNT;

NUGSCN *big_icon_scene;

NUGSCN *saveicon_scene;

NUGSCN *vehicle_scene;

NUGSCN *button_scene;

NUGSCN *things_scene;

i32 NuSpecialExistsFn(void *special);

void NuSpecialSetVisibility(void *special, i32 visible);

// from saga legoapi/items/objects/objectsall.cpp
void LevelObjects_InitForLevel(WORLDINFO_s *world) {
    usize aligned = ALIGN(world->giz_buffer.addr, 4);
    world->lev_objs = reinterpret_cast<LEVEL_OBJECT_RUNTIME *>(aligned);
    world->giz_buffer.addr = aligned + static_cast<usize>(LEVELOBJECTCOUNT) * sizeof(LEVEL_OBJECT_RUNTIME);
    memset(world->lev_objs, 0, static_cast<usize>(LEVELOBJECTCOUNT) * sizeof(LEVEL_OBJECT_RUNTIME));

    if (ObjTabList == NULL || LEVELOBJECTCOUNT <= 0) {
        return;
    }

    for (i32 object_index = 0; object_index < LEVELOBJECTCOUNT; ++object_index) {
        LEVELOBJECT &object_type = ObjTabList[object_index];
        LEVEL_OBJECT_RUNTIME &object = world->lev_objs[object_index];

        if (NuStrICmp(object_type.name, "power_up") == 0) {
            KNOBS = object_index;
        }

        NUGSCN *scene = NULL;
        switch (object_type.kind) {
            case LEVEL_OBJECT_SCENE_LEVEL:
                scene = world->current_gscn;
                break;

            case LEVEL_OBJECT_SCENE_AREA:
                scene = area_scene != NULL ? area_scene : world->current_gscn;
                break;

            case LEVEL_OBJECT_SCENE_CHARACTER_ICON:
                for (i32 character_id = 0; character_id < CHARCOUNT; ++character_id) {
                    scene = IconScene_FindById(character_id);
                    if (scene != NULL && NuSpecialFind(scene, &object.special, object_type.name, 1) != 0) {
                        goto object_resolved;
                    }
                }
                scene = big_icon_scene != NULL ? big_icon_scene : world->icons_gscn;
                break;

            case LEVEL_OBJECT_SCENE_SAVE_ICON:
                scene = saveicon_scene;
                break;

            case LEVEL_OBJECT_SCENE_VEHICLE:
                scene = vehicle_scene;
                break;

            case LEVEL_OBJECT_SCENE_BUTTON:
                scene = button_scene;
                break;

            case LEVEL_OBJECT_SCENE_THINGS:
            default:
                if (area_scene != NULL && world->area != NULL &&
                    (world->area->flags & AREAFLAG_OVERRIDE_THINGS_SCENE) != 0) {
                    if (NuSpecialFind(area_scene, &object.special, object_type.name, 1) != 0) {
                        goto object_resolved;
                    }
                }
                scene = things_scene;
                break;
        }

        if (scene != NULL) {
            NuSpecialFind(scene, &object.special, object_type.name, 1);
        }

    object_resolved:
        object.active = static_cast<u8>(NuSpecialExistsFn(&object.special));
        if (object.active != 0) {
            NuSpecialSetVisibility(&object.special, 0);
        }
    }
}
#endif
