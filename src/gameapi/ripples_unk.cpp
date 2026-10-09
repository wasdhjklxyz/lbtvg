// gameapi/ripples_unk.cpp: placed by tools/new.py; file name unproven.

#include "../batman/worldinfo_unk.h"
#include "../nu2api/nucore/common.h"
#include <stddef.h>

struct RIPPLEEFFECT_s {
  u8 start_color[4];       // 0x00
  u8 end_color[4];         // 0x04
  f32 lifetime;            // 0x08
  f32 initial_size;        // 0x0c
  f32 end_size;            // 0x10
  char texture_name[0x10]; // 0x14
  void *material;          // 0x24
};

typedef struct nufpar_s {
  u32 pad0[0x910 / 4];
  char *word_buf; // 0x910
} NUFPAR;

NUFPAR *NuFParCreateMem(char *name, char *buffer, i32 bufferSize);
void NuFParPushCom(NUFPAR *parser, void *commands);
i32 NuFParGetLine(NUFPAR *parser);
i32 NuFParGetWord(NUFPAR *parser);
i32 NuFParInterpretWord(NUFPAR *parser);
void NuFParDestroy(NUFPAR *parser);
i32 NuStrICmp(const char *a, const char *b);
void InitRippleMtl(char *name, void **material, VARIPTR *buf, VARIPTR *end);

// GLOBAL: LEGOBATMAN 0x00967130
extern u8 RippleEffect_ConfigKeywords[];
// GLOBAL: LEGOBATMAN 0x00ad1bec
extern WORLDINFO_s *RE_worldinfo;
// GLOBAL: LEGOBATMAN 0x00ad1bf0
extern RIPPLEEFFECT_s *RE_rippleeffect;

i32 NuFParGetInt(NUFPAR *parser);
int NuStrLen(const char *s);
int NuStrCpy(char *dst, const char *src);

// FUNCTION: LEGOBATMAN 0x00655b50
void RE_texture_name(NUFPAR *parser) {
  if (NuFParGetWord(parser) && NuStrLen(parser->word_buf) < 16)
    NuStrCpy(RE_rippleeffect->texture_name, parser->word_buf);
}

// FUNCTION: LEGOBATMAN 0x00655ba0
void RE_start_colour(NUFPAR *parser) {
  RE_rippleeffect->start_color[0] = NuFParGetInt(parser);
  RE_rippleeffect->start_color[1] = NuFParGetInt(parser);
  RE_rippleeffect->start_color[2] = NuFParGetInt(parser);
  RE_rippleeffect->start_color[3] = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x00655bf0
void RE_end_colour(NUFPAR *parser) {
  RE_rippleeffect->end_color[0] = NuFParGetInt(parser);
  RE_rippleeffect->end_color[1] = NuFParGetInt(parser);
  RE_rippleeffect->end_color[2] = NuFParGetInt(parser);
  RE_rippleeffect->end_color[3] = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x00655ca0
void RippleEffects_Configure(WORLDINFO_s *world, char *config) {
  world->ripple_effects = 0;
  world->ripple_effect_count = 0;
  NUFPAR *parser = NuFParCreateMem("rippleeffects", config, 0xffff);
  if (parser == 0)
    return;
  world->buf104.addr = (world->buf104.addr + 3) & ~3;
  world->ripple_effects = (RIPPLEEFFECT_s *)world->buf104.addr;
  RIPPLEEFFECT_s *effect = world->ripple_effects;
  NuFParPushCom(parser, RippleEffect_ConfigKeywords);
  i32 active = 0;
  while (NuFParGetLine(parser)) {
    NuFParGetWord(parser);
    if (parser->word_buf[0] == 0)
      continue;
    if (active) {
      if (NuStrICmp(parser->word_buf, "rippleeffects_end") == 0) {
        active = 0;
        if (effect->texture_name[0] != 0) {
          effect++;
          world->ripple_effect_count++;
        }
      } else {
        NuFParInterpretWord(parser);
      }
    } else if (world->ripple_effect_count < 2 &&
               NuStrICmp(parser->word_buf, "rippleeffects_start") == 0) {
      active = 1;
      effect->lifetime = 2.0f;
      RE_worldinfo = world;
      RE_rippleeffect = effect;
      effect->initial_size = 0.0f;
      effect->texture_name[0] = 0;
      effect->end_size = 1.0f;
      effect->start_color[0] = 0x40;
      effect->start_color[1] = 0x40;
      effect->start_color[2] = 0x40;
      effect->start_color[3] = 0xff;
      effect->end_color[0] = 0;
      effect->end_color[1] = 0;
      effect->end_color[2] = 0;
      effect->end_color[3] = 0;
      effect->material = 0;
    }
  }
  NuFParDestroy(parser);
  if (world->ripple_effect_count > 0) {
    world->buf104.addr =
        ((u32)(world->ripple_effects + world->ripple_effect_count) + 15) & ~15;
    for (i32 i = 0; i < world->ripple_effect_count; i++)
      InitRippleMtl(world->ripple_effects[i].texture_name,
                    &world->ripple_effects[i].material, &world->buf104,
                    &world->bufEnd108);
  } else {
    world->ripple_effects = 0;
  }
}

// GLOBAL: LEGOBATMAN 0x00967124
extern char *RippleEffectNames[2];

// FUNCTION: LEGOBATMAN 0x00655e50
i32 LookupRippleEffectIndex(char *name) {
  for (i32 i = 0; i < 2; i++) {
    if (NuStrICmp(RippleEffectNames[i], name) == 0)
      return i;
  }
  return -1;
}

// STUB: LEGOBATMAN 0x00656130
// callee FUN_00656040 (list-node alloc) takes the set in ecx: register-arg
// static
#if 0
#include "../nu2api/numath/numtx.h"

#include "../nu2api/numath/nuvec.h"

struct ripple_set_s {
    union {
        u32 reset_state;
        struct {
            u16 count;
            union {
                u16 free_count;
                u16 active_count;
            };
        };
    };
    ripple_node_s *nodes;
    union {
        ripple_node_s *current;
        ripple_node_s *free_head;
    };
    union {
        void *field_0x0c;
        ripple_node_s *newest;
    };
    union {
        void *field_0x10;
        ripple_node_s *oldest;
    };
};

struct RGBA {
    union {
        u32 value;
        struct {
            u8 r, g, b, a;
        };
    };
};

struct ripple_node_s {
    union {
        u8 pad_0x00[0x78];
        struct {
            NUMTX matrix;
            NUVEC velocity;
            struct numtl_s *material;
            f32 size;
            f32 initial_size;
            f32 growth;
            RGBA color;
            RGBA start_color;
            RGBA end_color;
            f32 age;
            f32 lifetime;
            f32 delay;
            u16 flags;
            u16 reserved_76;
        };
    };
    ripple_node_s *next;
    ripple_node_s *previous;
};

// the attribs byte6 bit7 set (asm has no `orb $0x80,0x46` here). The material
// is queued for dynamic display-list creation and the platform pass runs with
// is_3d=1 (no has_no_transform bit). Byte6 clear keeps NuMtlUpdatePS on the
// RetrieveShaderVariant path instead of the vtx_desc bit2 poke path.
extern "C" NUMTL *NuMtlCreate3D(i32 count);

NUVEC v000;

// from saga legoapi/items/fx/ripples.cpp
void AddRipple(ripple_set_s *set, numtx_s *matrix, float size, float growth, float lifetime, float delay,
               RGBA start_color, RGBA end_color, i32 flags, numtl_s *material, nuvec_s *velocity) {
    if (material == NULL)
        return;
    if (set == NULL)
        return;
    u16 active_count = set->active_count;
    u16 capacity = set->count;
    ripple_node_s *newest = set->newest;
    ripple_node_s *node = set->free_head;
    if (!(active_count < capacity)) {
        node = set->oldest;
        set->newest = node;
        set->oldest = node->next;
    } else {
        ripple_node_s *next_free;
        if (node != node->next) {
            node->next->previous = node->previous;
            node->previous->next = node->next;
            next_free = node->next;
        } else {
            next_free = NULL;
        }
        if (newest == NULL) {
            node->next = node;
            node->previous = node;
        } else {
            ripple_node_s *next = newest->next;
            node->next = next;
            newest->next = node;
            next->previous = node;
            node->previous = newest;
        }
        set->free_head = next_free;
        ++active_count;
        set->active_count = active_count;
        set->newest = node;
        if (active_count == capacity)
            set->free_head = NULL;
        if (set->oldest == NULL)
            set->oldest = node;
    }
    node->matrix = *matrix;
    node->material = material;
    node->initial_size = size;
    node->size = size;
    node->flags = static_cast<u16>(flags);
    node->lifetime = lifetime;
    node->start_color = start_color;
    node->growth = growth;
    node->end_color = end_color;
    node->color = start_color;
    node->age = 0.0f;
    node->delay = delay;
    if (velocity != NULL)
        node->velocity = *velocity;
    else
        node->velocity = v000;
}
#endif

f32 NuFParGetFloat(NUFPAR *parser);

// FUNCTION: LEGOBATMAN 0x00655c40
void RE_life(NUFPAR *parser) {
  RE_rippleeffect->lifetime = NuFParGetFloat(parser);
}

// FUNCTION: LEGOBATMAN 0x00655c60
void RE_start_radius(NUFPAR *parser) {
  RE_rippleeffect->initial_size = NuFParGetFloat(parser);
}

// FUNCTION: LEGOBATMAN 0x00655c80
void RE_end_radius(NUFPAR *parser) {
  RE_rippleeffect->end_size = NuFParGetFloat(parser);
}
