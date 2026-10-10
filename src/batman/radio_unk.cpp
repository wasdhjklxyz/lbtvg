// batman/radio_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nu2api/nucore/common.h"
#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"
#include <stddef.h>
#include <string.h>

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x004e3390
static f32 NuSinApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x004e3450
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

// STUB: LEGOBATMAN 0x004e34b0
// needs same-TU static FUN_004e3390 (caller keeps edx live across it); not
// attempted
#if 0
#include "../nu2api/numath/numtx.h"

#include "../nu2api/numath/nuvec.h"

#include "../nu2api/nu3d/nuspecial.h"

struct GIZMOBLOWUP_s {
    union {
        NUMTX transform; // 0x00, instance transform refreshed by the early-update pass
        struct {
            undefined field0_0x0[0x30];
            NUVEC position; // 0x30, translation exposed through GIZMOFNS
            undefined field_0x3c[4];
        };
    };
    union {
        undefined field_0x40[4];
        MechObjectInterface *mech_object_interface;
    };
    NUVEC screen_position; // 0x44, projected by the draw pass
    union {
        char field_0x50[0x50]; // compatibility view used by level-specific links
        struct {
            NUVEC mid_position; // 0x50, center used by effects and collision
            NUVEC bounds_min;   // 0x5c
            NUVEC bounds_max;   // 0x68
            f32 field_0x74;
            f32 field_0x78;
            f32 field_0x7c;
            f32 field_0x80;
            f32 field_0x84;
            f32 field_0x88;
            f32 field_0x8c;
            f32 field_0x90;
            f32 field_0x94;
            f32 flicker_timer; // 0x98
            union {
                u32 status_flags; // 0x9c, aggregate tested by the draw pass
                struct {
                    u8 output_flags;     // 0x9c, GIZMOBLOWUP_OUTPUT_FLAGS
                    u8 visibility_flags; // 0x9d, GIZMOBLOWUP_VISIBILITY_FLAGS
                    u8 state_flags;      // 0x9e, GIZMOBLOWUP_STATE_FLAGS
                    u8 field_0x9f;       // 0x9f, secondary output/runtime flags
                };
            };
        };
    };
    union {
        u32 draw_flags; // 0xa0, GIZMOBLOWUP_DRAW_FLAGS
        u32 field_0xa0; // compatibility name for level-specific setup code
    };
    u32 secondary_flags;     // 0xa4
    u32 field_0xa8;          // 0xa8
    GIZMOBLOWUPTYPE_s *type; // 0xac, shared type/animation data
    f32 target_scale;        // 0xb0, scales the transform-target marker
    f32 field_0xb4;
    f32 field_0xb8;
    f32 activation_delay; // 0xbc
    f32 field_0xc0;
    f32 animation_time; // 0xc4, explicitly selected animation frame
    f32 field_0xc8;
    f32 field_0xcc;
    f32 animation_offset; // 0xd0, offset from the type's base frame
    f32 respawn_timer;    // 0xd4
    f32 field_0xd8;
    f32 reflection_height;     // 0xdc
    GAMEANTINODE_s *anti_node; // 0xe0, registered while the blowup is visible
    i16 field_0xe4;
    i16 field_0xe6;
    i16 field_0xe8;
    i16 field_0xea;
    i16 field_0xec;
    i16 field_0xee;
    u16 field_0xf0;
    u16 field_0xf2;
    u16 field_0xf4;
    i16 field_0xf6;
    i16 field_0xf8;
    char name[0x10]; // 0xfa
    i16 platform_id; // 0x10a, terrain platform toggled with visibility
    i16 field_0x10c;
    undefined field_0x10e[6];
    u8 field_0x114;
    u8 field_0x115;
    u8 saved_state_0;   // 0x116
    u8 initial_state_0; // 0x117
    u8 saved_state_1;   // 0x118
    u8 initial_state_1; // 0x119
    undefined field_0x11a[2];
    nuhspecial_s *override_special; // 0x11c, optional per-instance special
    NUVEC *field_0x120;             // 0x120
    u8 field_0x124;                 // 0x124
    undefined field_0x125[3];
    float field_0x128; // 0x128
    void ClearMechObjectInterface();
    MechObjectInterface *GetMechObjectInterface();
};

typedef uint8_t u8;
typedef u8 undefined;

i32 radios_playing;

static RadioEntry radios[8];

f32 FRAMETIME;

f32 NuFmod(f32 a, f32 b);

void GizmoBlowupUpdateMatrix(GIZMOBLOWUP_s *blowup);

f32 NuTrigTable[NUTRIGTABLE_COUNT];

void NuMtxPreScale(NUMTX *m, NUVEC *s);

i32 NuSpecialExistsFn(void *special);

NUMTX *NuSpecialGetDrawMtx(void *special);

NUMTX *NuSpecialGetMtx(void *special);

void NuSpecialUpdate(nuhspecial_s *special);

void PlaySfx(char *name, nuvec_s *pos);

// from saga legoapi/audio/radio.cpp
void UpdateRadios() {
    if (radios_playing == 0) {
        return;
    }

    radios_playing = 0;
    for (i32 i = 0; i < 8; i++) {
        RadioEntry *radio = &radios[i];
        if (radio->time <= 0.0f) {
            continue;
        }

        radio->time -= FRAMETIME;
        if (!(radio->time >= 0.0f)) {
            radio->time = 0.0f;
        } else {
            radios_playing = 1;
        }

        f32 phase = NuFmod(radio->time, 0.5f);
        i32 angle = static_cast<i32>(phase * 65536.0f) >> 1;
        NUMTX *matrix;
        if (radio->blowup != NULL) {
            GizmoBlowupUpdateMatrix(radio->blowup);
            f32 scale_value = NuTrigTable[angle & 0x7fff] * 0.1f + 1.0f;
            NUVEC scale = {scale_value, scale_value, scale_value};
            matrix = &radio->blowup->transform;
            NuMtxPreScale(matrix, &scale);
            radio->blowup->state_flags |= 1;
        } else {
            if (NuSpecialExistsFn(&radio->special) == 0) {
                continue;
            }
            *NuSpecialGetDrawMtx(&radio->special) = *NuSpecialGetMtx(&radio->special);
            matrix = NuSpecialGetDrawMtx(&radio->special);
            if (static_cast<i32>(phase * 65536.0f) != 0) {
                f32 scale_value = NuTrigTable[angle & 0x7fff] * 0.1f + 1.0f;
                NUVEC scale = {scale_value, scale_value, scale_value};
                NuMtxPreScale(matrix, &scale);
                NuSpecialUpdate(&radio->special);
            }
        }
        PlaySfx("swdisco", reinterpret_cast<NUVEC *>(&matrix->m30));
    }
}
#endif

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_radio_unk(f32 *v, f32 a, i32 i) {
  v[0] = NuSinApprox(i);
  NuVec4Set(v, a, a, a, a);
}

typedef struct nuhspecial_s {
  u32 pad[3];
} NUHSPECIAL;
typedef struct numtx_s {
  f32 m[16];
} NUMTX;
typedef struct GIZMOSYS_s GIZMOSYS;
typedef struct nugscn_s NUGSCN;

typedef struct RADIOBLOWUP_s {
  u8 pad0[0xa0];
  u32 flags; // 0xa0
  u8 pada4[0xfe - 0xa4];
  char name[1]; // 0xfe
} RADIOBLOWUP;

typedef struct GIZMO_s {
  void *object;
} GIZMO;

typedef struct RADIO_s {
  RADIOBLOWUP *blowup; // 0x00
  NUHSPECIAL special;  // 0x04
  f32 time;            // 0x10
} RADIO;

typedef struct RADIOWORLD_s {
  u8 pad0[0x140];
  NUGSCN *current_gscn; // 0x140
  u8 pad144[0x2b0c - 0x144];
  GIZMOSYS *gizmo_sys; // 0x2b0c
} RADIOWORLD;

// GLOBAL: LEGOBATMAN 0x009ccb08
extern RADIO radios[8];
// GLOBAL: LEGOBATMAN 0x009ccb00
extern i32 radios_playing;
// GLOBAL: LEGOBATMAN 0x00960894
extern RADIOWORLD *WORLD;
// GLOBAL: LEGOBATMAN 0x00961104
extern i32 blowup_gizmotype_id;
// GLOBAL: LEGOBATMAN 0x00943b04
extern f32 g_unk00943b04; // radio play time

GIZMO *GizmoFindByName(GIZMOSYS *gizmo_sys, i32 type_id, char *name);
i32 NuSpecialFind(NUGSCN *scene, NUHSPECIAL *out, char *name, i32 flags);
char *NuSpecialGetName(NUHSPECIAL *sp);
NUMTX *NuSpecialGetMtx(NUHSPECIAL *sp);
NUMTX *NuSpecialGetInstanceMtx(NUHSPECIAL *sp);
void NuSpecialUpdate(NUHSPECIAL *sp);
void GizmoBlowupUpdateMatrix(RADIOBLOWUP *blowup);
i32 NuStrICmp(const char *a, const char *b);

// Batman's form of saga legoapi/audio/radio.cpp (one search loop for stop)
// FUNCTION: LEGOBATMAN 0x004e36b0
void PlayRadio(char *special_name, char *blowup_name, i32 play) {
  RADIO *radio;
  i32 i;

  if (special_name == NULL && blowup_name == NULL)
    return;
  if (play != 0) {
    for (i = 0, radio = radios; i < 8; i++, radio++) {
      if (radio->time <= 0.0f)
        goto found;
    }
    return;
  found:
    if (blowup_name != NULL) {
      GIZMO *gizmo =
          GizmoFindByName(WORLD->gizmo_sys, blowup_gizmotype_id, blowup_name);
      if (gizmo == NULL)
        return;
      radio->blowup = (RADIOBLOWUP *)gizmo->object;
    } else if (special_name == NULL ||
               NuSpecialFind(WORLD->current_gscn, &radio->special, special_name,
                             1) == 0) {
      return;
    }
    radio->time = g_unk00943b04;
    radios_playing = 1;
    return;
  }

  for (i = 0, radio = radios; i < 8; i++, radio++) {
    if (blowup_name != NULL) {
      if (radio->blowup != NULL &&
          NuStrICmp(radio->blowup->name, blowup_name) == 0) {
        GizmoBlowupUpdateMatrix(radio->blowup);
        radio->blowup->flags |= 0x10000;
        memset(radio, 0, sizeof(*radio));
        return;
      }
      if (special_name != NULL) {
        char *name = NuSpecialGetName(&radio->special);
        if (name != NULL && NuStrICmp(name, special_name) == 0) {
          NUMTX *source = NuSpecialGetMtx(&radio->special);
          NUMTX *instance = NuSpecialGetInstanceMtx(&radio->special);
          *instance = *source;
          NuSpecialUpdate(&radio->special);
          memset(radio, 0, sizeof(*radio));
          return;
        }
      }
    }
  }
}
