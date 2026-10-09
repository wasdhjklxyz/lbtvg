// gameapi/traffic_unk.cpp: traffic animation config parsers (saga
// legoapi/items/objects/traffic.cpp); file name unproven.

#include "../nu2api/nucore/common.h"
#include "../nu2api/nucore/nustring.h"

typedef struct nufpar_s {
  u8 pad0[0x910];
  char *word_buf; // 0x910
} NUFPAR;

struct nugscn_s;
struct nuhspecial_s {
  void *scene;
  void *special;
  void *display_special;
};

i32 NuFParGetWord(NUFPAR *parser);
f32 NuFParGetFloat(NUFPAR *parser);
char *NuSpecialGetName(nuhspecial_s *sp);
i32 NuSpecialFind(nugscn_s *scene, nuhspecial_s *dest, char *name, i32 flags);

struct TRAFFICANIM_s {
  nuhspecial_s special; // 0x00
  void *animation;      // 0x0c
  u8 pad10[0x14 - 0x10];
  f32 tfactor;        // 0x14
  f32 frame_interval; // 0x18
  f32 rand_interval;  // 0x1c
  u8 pad20[0x24 - 0x20];
  f32 y_offset;             // 0x24
  i8 vehicle_indices[0x10]; // 0x28
  u8 pad38[0x138 - 0x38];
  i8 vehicle_count; // 0x138
  u8 pad139[0x140 - 0x139];
};

struct TRAFFICANIMSYS_s {
  TRAFFICANIM_s animations[64]; // 0x0000
  nuhspecial_s vehicles[0x10];  // 0x5000
  u8 pad50c0[0x77e0 - 0x50c0];
  i8 animation_count; // 0x77e0
  i8 vehicle_count;   // 0x77e1
};

struct TRAFFICWORLD_s {
  u8 pad0[0x140];
  nugscn_s *current_gscn; // 0x140
};

// GLOBAL: LEGOBATMAN 0x00ad1c08
extern TRAFFICANIMSYS_s *parse_trafficanimsys;
// GLOBAL: LEGOBATMAN 0x00ad1c0c
extern TRAFFICANIM_s *parse_trafficanim;
// GLOBAL: LEGOBATMAN 0x00ad1c10
extern TRAFFICANIM_s reference_trafficanim;
// GLOBAL: LEGOBATMAN 0x00ad1d50
extern TRAFFICWORLD_s *parse_worldinfo;

// FUNCTION: LEGOBATMAN 0x00658e60
void TrafficAnim_yoffset(NUFPAR *parser) {
  if (parse_trafficanim != 0)
    parse_trafficanim->y_offset = NuFParGetFloat(parser);
}

struct TRAFFICINSTANIM_s {
  u8 pad0[0x40];
  f32 tfactor; // 0x40
  u8 pad44[0x5c - 0x44];
  u16 anim_ix; // 0x5c
};

struct TRAFFICSCENE_s {
  u8 pad0[0x54];
  void **instance_animation_data; // 0x54
};

TRAFFICINSTANIM_s *NuSpecialGetInstAnim(nuhspecial_s *sp);
i32 NuFParPushCom(NUFPAR *parser, void *commands);
i32 NuFParInterpretWord(NUFPAR *parser);
void NuFParPopCom(NUFPAR *parser);

// GLOBAL: LEGOBATMAN 0x009674d8
extern u8 TrafficAnim_ConfigKeywords[];

// STUB: LEGOBATMAN 0x00658e80
// close: orig loads the count byte once (cmp cl, 0x40); ours compares memory
// then reloads; one store scheduled differently.
void Traffic_animobj(NUFPAR *parser) {
  if (NuFParGetWord(parser) == 0)
    return;
  nuhspecial_s special;
  if (NuSpecialFind(parse_worldinfo->current_gscn, &special, parser->word_buf,
                    1) == 0)
    return;
  TRAFFICINSTANIM_s *instance_animation = NuSpecialGetInstAnim(&special);
  if (!(instance_animation == 0 ||
        parse_trafficanimsys->animation_count >= 64)) {
    TRAFFICANIM_s *animation =
        &parse_trafficanimsys
             ->animations[parse_trafficanimsys->animation_count++];
    *animation = reference_trafficanim;
    animation->special = special;
    animation->tfactor = instance_animation->tfactor;
    animation->animation =
        ((TRAFFICSCENE_s *)animation->special.scene)
            ->instance_animation_data[instance_animation->anim_ix];
    parse_trafficanim = animation;
    NuFParPushCom(parser, TrafficAnim_ConfigKeywords);
    while (NuFParGetWord(parser) != 0)
      NuFParInterpretWord(parser);
    NuFParPopCom(parser);
  }
}

// FUNCTION: LEGOBATMAN 0x00658f90
void Traffic_frame_interval(NUFPAR *parser) {
  reference_trafficanim.frame_interval = NuFParGetFloat(parser);
}

// FUNCTION: LEGOBATMAN 0x00658fb0
void Traffic_rand_interval(NUFPAR *parser) {
  reference_trafficanim.rand_interval = NuFParGetFloat(parser);
}

// FUNCTION: LEGOBATMAN 0x00658fd0
void Traffic_tfactor(NUFPAR *parser) {
  reference_trafficanim.tfactor = NuFParGetFloat(parser);
}

// FUNCTION: LEGOBATMAN 0x00658ff0
i32 TrafficFindVehicleUnk00658ff0(char *name) {
  i32 i;
  if (name != 0 && parse_worldinfo != 0 && parse_trafficanimsys != 0) {
    for (i = 0; i < parse_trafficanimsys->vehicle_count; i++) {
      if (NuStrICmp(name,
                    NuSpecialGetName(&parse_trafficanimsys->vehicles[i])) == 0)
        return i;
    }
    if (parse_trafficanimsys->vehicle_count < 0x10) {
      if (NuSpecialFind(parse_worldinfo->current_gscn,
                        &parse_trafficanimsys
                             ->vehicles[parse_trafficanimsys->vehicle_count],
                        name, 1) != 0) {
        TRAFFICANIMSYS_s *sys = parse_trafficanimsys;
        sys->vehicle_count++;
        return sys->vehicle_count - 1;
      }
    }
  }
  return -1;
}

// FUNCTION: LEGOBATMAN 0x006590c0
void Traffic_vehicle(NUFPAR *parser) {
  if (NuFParGetWord(parser) && parse_trafficanimsys != 0 &&
      reference_trafficanim.vehicle_count < 0x10) {
    i32 index = TrafficFindVehicleUnk00658ff0(parser->word_buf);
    if (index >= 0)
      reference_trafficanim
          .vehicle_indices[reference_trafficanim.vehicle_count++] = index;
  }
}
