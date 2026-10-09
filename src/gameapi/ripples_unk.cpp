// gameapi/ripples_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nu2api/nucore/common.h"
#include <stddef.h>

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
