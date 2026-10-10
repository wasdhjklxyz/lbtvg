// gameapi/unk_00639330.cpp: TU of unknown name, found by its header-static
// copies (the functions after them are not matched yet).

#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"

// FUNCTION: LEGOBATMAN 0x00639330
static f32 NuSinApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x006393d0
static f32 NuCosApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x006393e0
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_00639330(f32 *v, f32 a, i32 i) {
  v[0] = NuSinApprox(i);
  v[1] = NuCosApprox(i);
  NuVec4Set(v, a, a, a, a);
}

// The rest of this TU is saga legoapi/characters/core/playeritems.cpp
// (FaceOpponent onwards are in playeritems_unk.cpp for now).
// (FaceOpponent onwards are in playeritems_unk.cpp for now).

#include "gameobject_unk.h"
#include <string.h>

typedef struct CONTEXTOPPONENT_s {
  GameObject_s *object;   // 0x00
  BlowupTarget_s *blowup; // 0x04
  u32 pad08[2];
} CONTEXTOPPONENT;

GameObject_s *ObjOpponent(GameObject_s *obj, f32 range, f32 extra_radius, i32 a,
                          i32 mode, i32 pass);
BlowupTarget_s *GizmoBlowUpOpponent(GameObject_s *obj, f32 range,
                                    f32 extra_radius, f32 c, i32 type, i32 e,
                                    i32 f, i32 g, i32 h, i32 i, i32 j);

// Batman's form of saga move.cpp's SelectOpponent, results in a context
// struct; flags 1 = characters, 2 = blowups.
// FUNCTION: LEGOBATMAN 0x00639400
void SelectOpponent(GameObject_s *obj, f32 range, f32 extra_radius, i32 mode,
                    i32 gizmo_first, CONTEXTOPPONENT *ctx, u32 flags) {
  memset(ctx, 0, sizeof(*ctx));
  if (gizmo_first == 0)
    ctx->object = ObjOpponent(obj, range, extra_radius, 1, mode, 1);
  if (ctx->object != NULL)
    return;
  if ((flags & 2) && ((obj->b1fc & 0x80) || gizmo_first != 0))
    ctx->blowup = GizmoBlowUpOpponent(obj, range, extra_radius, 0.0f,
                                      mode == 2 ? 3 : 4, 0, 0, 0, 0, 1, 0);
  if ((flags & 1) && ctx->blowup == NULL && gizmo_first == 0 &&
      !(mode == 2 && (obj->p54->p24->flags148 & 0x200)))
    ctx->object = ObjOpponent(obj, range, extra_radius, 1, mode, 2);
}
