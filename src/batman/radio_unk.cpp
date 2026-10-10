// batman/radio_unk.cpp: placed by tools/new.py; file name unproven.

#include "../gameapi/sfx_unk.h"
#include "../nu2api/nucore/common.h"
#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"
#include "../nu2api/numath/nuvec.h"
#include <stddef.h>
#include <string.h>

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x004e3390
static f32 NuSinApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x004e3450
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

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
  NUMTX transform; // 0x00
  u8 pad40[0xa0 - 0x40];
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

// GLOBAL: LEGOBATMAN 0x00a95fe0
extern f32 FRAMETIME;
// GLOBAL: LEGOBATMAN 0x00943b08
extern f32 g_unk00943b08; // radio pulse period
// GLOBAL: LEGOBATMAN 0x00943b0c
extern f32 g_unk00943b0c; // radio pulse scale

f32 NuFmod(f32 a, f32 b);
void NuMtxPreScale(NUMTX *m, nuvec_s *s);
NUMTX *NuSpecialGetDrawMtx(NUHSPECIAL *sp);
i32 NuSpecialExistsFn(NUHSPECIAL *sp);

// Batman's form of saga legoapi/audio/radio.cpp (NuSinApprox for the pulse)
// STUB: LEGOBATMAN 0x004e34b0
// one instruction off: the original sets ebx = radios before storing the
// loop counter 8; every form tried stores the counter first.
void UpdateRadios() {
  RADIO *radio = radios;
  i32 n = 8;

  if (radios_playing == 0)
    return;

  radios_playing = 0;
  do {
    if (radio->time > 0.0f) {
      radio->time -= FRAMETIME;
      if (radio->time < 0.0f)
        radio->time = 0.0f;
      else
        radios_playing = 1;

      f32 phase = NuFmod(radio->time, g_unk00943b08);
      i32 angle = (i32)(phase * 65536.0f);
      nuvec_s scale;
      if (radio->blowup != NULL) {
        GizmoBlowupUpdateMatrix(radio->blowup);
        scale.x = NuSinApprox(angle) * g_unk00943b0c + 1.0f;
        scale.y = scale.z = scale.x;
        NuMtxPreScale(&radio->blowup->transform, &scale);
        radio->blowup->flags |= 0x10000;
        PlaySfx("swdisco", (nuvec_s *)&radio->blowup->transform.m[12]);
      } else {
        if (NuSpecialExistsFn(&radio->special) != 0) {
          NUMTX *source = NuSpecialGetMtx(&radio->special);
          NUMTX *matrix = NuSpecialGetDrawMtx(&radio->special);
          *matrix = *source;
          if (angle != 0) {
            scale.x = NuSinApprox(angle) * g_unk00943b0c + 1.0f;
            scale.y = scale.z = scale.x;
            NuMtxPreScale(matrix, &scale);
            NuSpecialUpdate(&radio->special);
          }
          PlaySfx("swdisco", (nuvec_s *)&matrix->m[12]);
        }
      }
    }
    radio++;
  } while (--n);
}

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
