// batman/unk_0041d1c0.cpp: TU of unknown name, found by its header-static
// copies (the functions after them are not matched yet).

#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/numtx_inline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"
#include <stddef.h>

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x0041d3c0
static void NuMtxSetRotationYInline(f32 *m, i32 a);

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x0041d340
static void NuMtxCopyInline(f32 *dst, f32 *src);
// FUNCTION: LEGOBATMAN 0x0041d580
static void NuMtxRotateYInline(f32 *m, i32 a);

// FUNCTION: LEGOBATMAN 0x0041d1c0
static f32 NuFabs(f32 f);
// FUNCTION: LEGOBATMAN 0x0041d1e0
static f32 NuSinApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x0041d2a0
static f32 NuCosApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x0041d2f0
static void NuVec4Copy(f32 *dst, f32 *src);
// FUNCTION: LEGOBATMAN 0x0041d310
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_0041d1c0(f32 *v, f32 a, i32 i) {
  v[2] = NuFabs(a);
  v[0] = NuSinApprox(i);
  v[1] = NuCosApprox(i);
  NuVec4Copy(v + 4, v);
  NuVec4Set(v, a, a, a, a);
}

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_unk_0041d1c0(f32 *v, f32 a, i32 i) {
  NuMtxCopyInline(v + 32, v + 16);
  NuMtxRotateYInline(v + 16, i);
}

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_2_unk_0041d1c0(f32 *v, f32 a, i32 i) {
  NuMtxSetRotationYInline(v + 64, i);
}

// Mac order: Collection_CheckDrivingVehicleFn, ..Floating.., ..Flying..,
// VehicleArea_GetCollection, FixUpCharacters (after InitDefaultGame).

typedef struct GCDATA_s {
  u8 pad0[0x14c];
  u32 flags_lo : 13; // 0x14c
  u32 floating : 1;
  u32 flying : 1;
  u32 flags_hi : 17;
  u8 pad150[0x240 - 0x150];
} GCDATA;

extern GCDATA *g_unk00acb82c;

// GLOBAL: LEGOBATMAN 0x00a958c0
extern i32 g_unk00a958c0;
// GLOBAL: LEGOBATMAN 0x00a958b8
extern i32 g_unk00a958b8;

// FUNCTION: LEGOBATMAN 0x0041d780
i32 Unk0041d780(void) { return g_unk00a958c0 - g_unk00a958b8; }

// Collection check callback (address taken), no Mac counterpart.
// FUNCTION: LEGOBATMAN 0x0041d790
i32 CollectionCheckUnk0041d790(i32 id) { return id == 2; }

// FUNCTION: LEGOBATMAN 0x0041d7a0
i32 Collection_CheckDrivingVehicleFn(i32 id) {
  if (g_unk00acb82c[id].floating || g_unk00acb82c[id].flying)
    return 0;
  return 1;
}

// FUNCTION: LEGOBATMAN 0x0041d7d0
i32 Collection_CheckFloatingVehicleFn(i32 id) {
  return g_unk00acb82c[id].floating;
}

// FUNCTION: LEGOBATMAN 0x0041d800
i32 Collection_CheckFlyingVehicleFn(i32 id) { return g_unk00acb82c[id].flying; }

typedef struct AREADATA_s {
  u8 pad0[0x7c];
  u32 flags; // 0x7c
} AREADATA;

typedef struct COLLECTION_s COLLECTION;

// GLOBAL: LEGOBATMAN 0x009c591c
extern COLLECTION g_unk009c591c;
// GLOBAL: LEGOBATMAN 0x009c5904
extern COLLECTION g_unk009c5904;
// GLOBAL: LEGOBATMAN 0x009c5934
extern COLLECTION g_unk009c5934;
// GLOBAL: LEGOBATMAN 0x009c58a0
extern COLLECTION g_unk009c58a0;

// FUNCTION: LEGOBATMAN 0x0041d830
COLLECTION *VehicleArea_GetCollection(AREADATA *area) {
  if (area->flags & 4)
    return &g_unk009c591c;
  if ((area->flags & 0x180001) == 0x80001)
    return &g_unk009c5904;
  if ((area->flags & 0x180001) == 0x100001)
    return &g_unk009c5934;
  return &g_unk009c58a0;
}

typedef struct CHARACTERDATA_s {
  i32 type; // 0x00
  u8 pad04[0x14];
  void *move_fn;    // 0x18
  void *animate_fn; // 0x1c
  u8 pad20[0x48 - 0x20];
} CHARACTERDATA;

typedef struct CHARFIXUP_s {
  char *name;
  i16 *id;
} CHARFIXUP;

extern CHARACTERDATA *g_unk00acb81c;
extern i32 g_unk00acb820;
// GLOBAL: LEGOBATMAN 0x00963cd0
extern GCDATA g_unk00963cd0;
// GLOBAL: LEGOBATMAN 0x009cd6c8
extern i16 g_unk009cd6c8;

i32 CharIDFromName(char *name);
void Move_CHARACTER(void);
void Animate_CHARACTER(void);

// from saga legoapi/characters/core/characters.cpp
// FUNCTION: LEGOBATMAN 0x0041d880
void FixUpCharacters(CHARFIXUP *fixup) {
  i32 i;

  for (i = 0; i < g_unk00acb820; i++) {
    g_unk00acb82c[i] = g_unk00963cd0;
    if (g_unk00acb81c[i].type == -1)
      g_unk00acb81c[i].type = g_unk009cd6c8;
    if (g_unk00acb81c[i].move_fn == NULL)
      g_unk00acb81c[i].move_fn = (void *)Move_CHARACTER;
    if (g_unk00acb81c[i].animate_fn == NULL)
      g_unk00acb81c[i].animate_fn = (void *)Animate_CHARACTER;
  }

  if (fixup != NULL) {
    while (fixup->name != NULL) {
      if (fixup->id != NULL)
        *fixup->id = (i16)CharIDFromName(fixup->name);
      fixup++;
    }
  }
}

void SetMoveAndAnimateFunctions(u32 a, u32 b, u32 c, u32 d, i32 type,
                                void *move, void *animate, void *postanimate);
void Move_JEDI(void);
void Animate_JEDI(void);
void Move_CANNON(void);
void Animate_CANNON(void);
void Move_VEHICLE(void);
void Animate_VEHICLE(void);
void Move_BEAST(void);
void Animate_BEAST(void);
void Move_CRITTER(void);
void Animate_CRITTER(void);
void Move_DRAGBOMB(void);

// GLOBAL: LEGOBATMAN 0x009364a4
extern i16 id_DRAGBOMBGROUND;
// GLOBAL: LEGOBATMAN 0x009364a8
extern i16 id_DRAGBOMBAIR;
// GLOBAL: LEGOBATMAN 0x009364ac
extern i16 id_DRAGBOMBWATER;
// GLOBAL: LEGOBATMAN 0x009364b0
extern i16 id_DRAGBOMBBONUS;
// GLOBAL: LEGOBATMAN 0x009364b4
extern i16 id_DRAGBOMBBONUS2;

// Batman's version of saga legoapi/characters/core/charconfig.cpp; the
// function names come from the Mac build (same calls, same order).
// FUNCTION: LEGOBATMAN 0x0041d960
void ExtraCharacterFixUpAfterConfig(void) {
  SetMoveAndAnimateFunctions(8, 8, 0, 0, -1, (void *)Move_JEDI,
                             (void *)Animate_JEDI, NULL);
  SetMoveAndAnimateFunctions(0, 0, 0x800, 0x800, -1, (void *)Move_CANNON,
                             (void *)Animate_CANNON, NULL);
  SetMoveAndAnimateFunctions(0x2000, 0x2000, 0, 0, -1, (void *)Move_VEHICLE,
                             (void *)Animate_VEHICLE, NULL);
  SetMoveAndAnimateFunctions(0x40000000, 0x40000000, 0, 0, -1,
                             (void *)Move_BEAST, (void *)Animate_BEAST, NULL);
  SetMoveAndAnimateFunctions(0, 0, 0, 0, 7, (void *)Move_CRITTER,
                             (void *)Animate_CRITTER, NULL);
  SetMoveAndAnimateFunctions(0, 0, 0, 0, 7, (void *)Move_CRITTER,
                             (void *)Animate_CRITTER, NULL);
  SetMoveAndAnimateFunctions(0, 0, 0, 0, 13, (void *)Move_VEHICLE,
                             (void *)Animate_VEHICLE, NULL);
  if (id_DRAGBOMBGROUND != -1)
    g_unk00acb81c[id_DRAGBOMBGROUND].move_fn = (void *)Move_DRAGBOMB;
  if (id_DRAGBOMBAIR != -1)
    g_unk00acb81c[id_DRAGBOMBAIR].move_fn = (void *)Move_DRAGBOMB;
  if (id_DRAGBOMBWATER != -1)
    g_unk00acb81c[id_DRAGBOMBWATER].move_fn = (void *)Move_DRAGBOMB;
  if (id_DRAGBOMBBONUS != -1)
    g_unk00acb81c[id_DRAGBOMBBONUS].move_fn = (void *)Move_DRAGBOMB;
  if (id_DRAGBOMBBONUS2 != -1)
    g_unk00acb81c[id_DRAGBOMBBONUS2].move_fn = (void *)Move_DRAGBOMB;
}
