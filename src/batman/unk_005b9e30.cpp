// batman/unk_005b9e30.cpp: TU of unknown name, found by its header-static
// copies (the functions after them are not matched yet).

#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"

// FUNCTION: LEGOBATMAN 0x005b9e30
static f32 NuFabs(f32 f);
// FUNCTION: LEGOBATMAN 0x005b9e50
static f32 NuSinApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x005b9ef0
static f32 NuCosApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x005b9f00
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_005b9e30(f32 *v, f32 a, i32 i) {
  v[2] = NuFabs(a);
  v[0] = NuSinApprox(i);
  v[1] = NuCosApprox(i);
  NuVec4Set(v, a, a, a, a);
}

// The rest of this TU is the Mac's Lever file (.. Levers_RegisterGizmo
// 0x5bb4f0, Lever_MoveCode); callbacks named by their RegisterGizmo slot.

#include "../nu2api/nu3d/nuspecial.h"
#include "leveldata_unk.h"
#include "worldinfo_unk.h"
#include <stddef.h>
#include <string.h>

typedef struct LEVER_s {
  u8 pad0[0x40];
  nuvec_s v40;     // 0x40
  nuvec_s v4c;     // 0x4c
  char name[0x10]; // 0x58
  nuvec_s pos68;   // 0x68
  f32 f74;         // 0x74
  f32 pull;        // 0x78
  f32 f7c;         // 0x7c
  f32 f80;         // 0x80
  f32 f84;         // 0x84
  u16 rot_y;       // 0x88
  u16 colour;      // 0x8a
  i16 plat;        // 0x8c, -1 = none
  u16 s8e;         // 0x8e
  u16 s90;         // 0x90
  u16 s92;         // 0x92
  union {
    struct {
      u16 b0 : 1;
      u16 being_pulled : 1; // 0x94 bit 1
      u16 goodie : 1;       // 0x94 bit 2
      u16 baddie : 1;       // 0x94 bit 3
      u16 visible : 1;      // 0x94 bit 4
      u16 b5 : 1;
      u16 b6 : 1;
      u16 active : 1; // 0x94 bit 7
      u16 b8 : 1;
      u16 b9 : 1;
      u16 b10 : 1;
      u16 b11 : 1;
      u16 b12 : 1;
      u16 b13 : 1;
    };
    u16 state; // 0x94
  };
  i16 s96;          // 0x96
  char colour_code; // 0x98
  u8 pad99[0xa0 - 0x99];
  nuhspecial_s special; // 0xa0
  f32 fac;              // 0xac
} LEVER;

typedef struct LEVERPROGRESS_s {
  u32 down[1];    // 0x00
  u32 active[1];  // 0x04
  u32 visible[1]; // 0x08
} LEVERPROGRESS;

typedef struct GIZMO_s {
  void *object;
} GIZMO;

typedef struct ADDGIZMOTYPE_s {
  char *name;        // 0x00
  char *prefix;      // 0x04
  u16 progress_size; // 0x08
  void *fns[0x1c];   // 0x0c
} ADDGIZMOTYPE;

// GLOBAL: LEGOBATMAN 0x00960118
extern ADDGIZMOTYPE Default_ADDGIZMOTYPE;
// GLOBAL: LEGOBATMAN 0x00960050
extern char *Lever_OutputName;

i32 NuStrLen(const char *s);
void AddGizmo(GIZMOSYS_s *gizmo_sys, i32 type_id, void *a, void *object);
void *GameBufferAllocProgressUnk005bbaf0(VARIPTR *buf, VARIPTR *buf_end,
                                         i32 size);
extern "C" void PlatOnOff(i32 index, i32 enabled);

// GLOBAL: LEGOBATMAN 0x00963200
extern i32 g_unk00963200[9]; // lever colour ids: y o r b g p w u l

// FUNCTION: LEGOBATMAN 0x005b9f20
static void Lever_SetColour(LEVER *lever) {
  if (lever->colour_code == 'r')
    lever->colour = g_unk00963200[2];
  else if (lever->colour_code == 'o')
    lever->colour = g_unk00963200[1];
  else if (lever->colour_code == 'y')
    lever->colour = g_unk00963200[0];
  else if (lever->colour_code == 'b')
    lever->colour = g_unk00963200[3];
  else if (lever->colour_code == 'g')
    lever->colour = g_unk00963200[4];
  else if (lever->colour_code == 'p')
    lever->colour = g_unk00963200[5];
  else if (lever->colour_code == 'w')
    lever->colour = g_unk00963200[6];
  else if (lever->colour_code == 'u')
    lever->colour = g_unk00963200[7];
  else if (lever->colour_code == 'l')
    lever->colour = g_unk00963200[8];
}

// GLOBAL: LEGOBATMAN 0x00960044
extern f32 g_unk00960044;

i32 EdFileReadInt();
char EdFileReadChar();
i16 EdFileReadShort();
f32 EdFileReadFloat();
void EdFileRead(void *buf, i32 len);
void EdFileReadNuVec(nuvec_s *v);
int NuSpecialExistsFn(nuhspecial_s *sp);

// FUNCTION: LEGOBATMAN 0x005b9fd0
i32 Levers_Load(void *world_ptr, void *unused) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  char name[0x100];
  if (world->lever_count == 0) {
    i32 version = EdFileReadInt();
    world->lever_count = EdFileReadInt();
    for (i32 i = 0; i < world->lever_count; i++) {
      EdFileRead(world->levers[i].name, 0x10);
      EdFileReadNuVec(&world->levers[i].pos68);
      world->levers[i].rot_y = EdFileReadShort();
      world->levers[i].colour_code = EdFileReadChar();
      if (version >= 2)
        world->levers[i].b5 = EdFileReadChar() & 1;
      if (version >= 3)
        world->levers[i].f80 = EdFileReadFloat();
      else
        world->levers[i].f80 = 2.0f;
      if (version >= 4)
        world->levers[i].b8 = EdFileReadChar() & 1;
      else
        world->levers[i].b8 = 0;
      if (version >= 5) {
        EdFileReadNuVec(&world->levers[i].v4c);
        world->levers[i].f84 = EdFileReadFloat();
      } else {
        world->levers[i].v4c.x = world->levers[i].v4c.y = 0.0f;
        world->levers[i].v4c.z = -g_unk00960044;
        world->levers[i].f84 = 1.0f;
      }
      if (version >= 6)
        world->levers[i].b9 = EdFileReadChar() & 1;
      else
        world->levers[i].b9 = 0;
      if (version >= 7) {
        i32 len = EdFileReadChar();
        if (len != 0) {
          memset(name, 0, sizeof(name));
          EdFileRead(name, len);
          NuSpecialFind(world->scn140, &world->levers[i].special, name, 0);
          if (!NuSpecialExistsFn(&world->levers[i].special))
            memset(&world->levers[i].special, 0, sizeof(nuhspecial_s));
        }
      }
      if (version >= 8)
        world->levers[i].b13 = EdFileReadChar() & 1;
      else
        world->levers[i].b13 = 0;
      if (version >= 9) {
        world->levers[i].s96 = EdFileReadChar();
        if (EdFileReadChar())
          world->levers[i].s96 |= 0x40;
      } else {
        world->levers[i].s96 = 0;
      }
      world->levers[i].plat = -1;
    }
    return 1;
  }
  return 0;
}

// GLOBAL: LEGOBATMAN 0x0096004c
extern i32 g_unk0096004c; // lever platform type

extern "C" i16 NewPlatPickupInst(void *m, i32 type);
extern "C" void PlatInstRotate(i32 id, i32 rotate);

// FUNCTION: LEGOBATMAN 0x005ba330
void Levers_InitTerrain(WORLDINFO_s *world) {
  if (world->levers == NULL || g_unk0096004c == -1)
    return;
  for (i32 i = 0; i < world->lever_count; i++) {
    world->levers[i].plat = NewPlatPickupInst(&world->levers[i], g_unk0096004c);
    PlatInstRotate(world->levers[i].plat, 1);
  }
}

// FUNCTION: LEGOBATMAN 0x005ba3b0
void *Levers_ReserveBufferSpace(void *world_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  world->levers = NULL;
  world->lever_count = 0;
  if (world->current_level->max_levers > 0) {
    world->buf104.addr = (world->buf104.addr + 0xf) & ~0xf;
    world->levers = (LEVER *)world->buf104.void_ptr;
    world->buf104.addr += world->current_level->max_levers * sizeof(LEVER);
  }
  return world->levers;
}

// GLOBAL: LEGOBATMAN 0x00960540
extern i32 g_unk00960540;

// FUNCTION: LEGOBATMAN 0x005ba400
void Lever_Release(GameObject_s *obj) {
  if (obj->b9db == g_unk00960540 && obj->techno != NULL &&
      !((LEVER *)obj->techno)->being_pulled)
    ((LEVER *)obj->techno)->b0 = 0;
}

// FUNCTION: LEGOBATMAN 0x005ba430
void Lever_SetVisibility(GIZMO *gizmo, i32 visible) {
  if (gizmo != NULL && gizmo->object != NULL) {
    LEVER *lever = (LEVER *)gizmo->object;
    lever->visible = visible != 0;
    if (lever->plat != -1)
      PlatOnOff(lever->plat, lever->visible);
  }
}

// FUNCTION: LEGOBATMAN 0x005ba490
i32 Lever_ActivateRev(GIZMO *gizmo, i32 value, i32 query) {
  if (gizmo != NULL && gizmo->object != NULL) {
    LEVER *lever = (LEVER *)gizmo->object;
    if (query & 1) {
      if (lever->active)
        return value;
      return value == 0;
    }
    lever->active = value == 0;
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x005ba4f0
void Lever_GetAbsTargetPos(LEVER *lever, nuvec_s *out) {
  if (lever != NULL && out != NULL) {
    nuvec_s t = lever->v40;
    t.y += 0.005f;
    *out = t;
  }
}

f32 NuVecDistSqr(nuvec_s *a, nuvec_s *b, nuvec_s *d);

// FUNCTION: LEGOBATMAN 0x005ba530
LEVER *Lever_FindNearest(WORLDINFO_s *world, nuvec_s *pos, GameObject_s *obj,
                         f32 *out) {
  LEVER *nearest = NULL;
  f32 d;
  LEVER *lever = world->levers;
  f32 best = 1000000000.0f;
  for (i32 i = 0; i < world->lever_count; i++, lever++) {
    if (obj != NULL) {
      if (!lever->active || !lever->visible || (lever->state & 3) ||
          lever->pull != 0.0f || lever->v40.y == 2000000.0)
        continue;
      nuvec_s t;
      Lever_GetAbsTargetPos(lever, &t);
      d = NuVecDistSqr(pos, &t, NULL);
    } else {
      d = NuVecDistSqr(pos, &lever->pos68, NULL);
    }
    if (d < best) {
      best = d;
      nearest = lever;
    }
  }
  if (out != NULL)
    *out = best;
  return nearest;
}

void Levers_Draw(void *world, void *unused, f32 dt);

// FUNCTION: LEGOBATMAN 0x005babb0
i32 Levers_GetMaxGizmos(void *world_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  if (world != NULL)
    return world->current_level->max_levers;
  return 0;
}

// FUNCTION: LEGOBATMAN 0x005babd0
void Levers_AddGizmos(GIZMOSYS_s *gizmo_sys, i32 type_id, void *world_ptr,
                      void *unused) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  for (i32 i = 0; i < world->lever_count; i++) {
    if (NuStrLen(world->levers[i].name) != 0)
      AddGizmo(gizmo_sys, type_id, NULL, &world->levers[i]);
  }
}

// FUNCTION: LEGOBATMAN 0x005bac40
char *Lever_GetGizmoName(GIZMO *gizmo) {
  return gizmo != NULL ? ((LEVER *)gizmo->object)->name : NULL;
}

// FUNCTION: LEGOBATMAN 0x005bac60
i32 Lever_FullyPulledDown(LEVER *lever) {
  if (lever->visible && lever->being_pulled && lever->pull >= 0.6f)
    return 1;
  return 0;
}

// FUNCTION: LEGOBATMAN 0x005bac90
i32 Lever_BeingPulled(LEVER *lever) { return lever->being_pulled; }

// FUNCTION: LEGOBATMAN 0x005bacb0
i32 Lever_GetOutput(GIZMO *gizmo, i32 output, i32 b) {
  LEVER *lever = (LEVER *)gizmo->object;
  if ((lever->active && lever->visible) || b != 0) {
    switch (output) {
    case 0:
      if (Lever_FullyPulledDown(lever))
        return 1;
      break;
    case 1:
      if (lever->goodie && Lever_FullyPulledDown(lever))
        return 1;
      break;
    case 2:
      if (lever->baddie && Lever_FullyPulledDown(lever))
        return 1;
      break;
    }
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x005bad20
char *Lever_GetOutputName(GIZMO *gizmo, i32 output) {
  switch (output) {
  case 0:
    return Lever_OutputName;
  case 1:
    return "Down(Goodie)";
  case 2:
    return "Down(Baddie)";
  }
  return NULL;
}

// FUNCTION: LEGOBATMAN 0x005bad50
i32 Lever_GetNumOutputs(GIZMO *gizmo) { return 3; }

// FUNCTION: LEGOBATMAN 0x005bad60
void *Levers_AllocateProgressData(VARIPTR *buf, VARIPTR *buf_end) {
  return GameBufferAllocProgressUnk005bbaf0(buf, buf_end,
                                            sizeof(LEVERPROGRESS));
}

// FUNCTION: LEGOBATMAN 0x005bad80
void Levers_ClearProgress(void *world, void *progress_ptr) {
  LEVERPROGRESS *progress = (LEVERPROGRESS *)progress_ptr;
  if (progress != NULL) {
    progress->down[0] = 0;
    progress->active[0] = 0xffffffff;
    progress->visible[0] = 0xffffffff;
  }
}

WORLDINFO_s *WorldInfo_CurrentlyActive(void);
i32 ActivePlayerInRange(nuvec_s *position, f32 range_squared, f32 *out);

// FUNCTION: LEGOBATMAN 0x005bae50
i32 Levers_UpdateHint(void *hint) {
  WORLDINFO_s *world = WorldInfo_CurrentlyActive();
  LEVER *lever = (LEVER *)world->levers;
  if (lever == NULL)
    return 0;
  for (i32 i = 0; i < world->lever_count; i++, lever++) {
    if (lever->active && lever->visible && !(lever->state & 3) &&
        lever->pull == 0.0f && ActivePlayerInRange(&lever->v40, 0.16f, NULL))
      return 1;
  }
  return 0;
}

// GLOBAL: LEGOBATMAN 0x009f7a44
extern nuvec_s g_unk009f7a44;

void NuVecRotateY(nuvec_s *v, nuvec_s *v0, i32 a);
void NuVecAdd(nuvec_s *out, nuvec_s *a, nuvec_s *b);
f32 GameShadow(void *object, nuvec_s *position, f32 probe_height, i32 a);
void FindAnglesZX(nuvec_s *normal, u16 *x_rotation, u16 *z_rotation);
void NuMtxSetRotationY(numtx_s *m, i32 a);
void NuMtxTranslate(numtx_s *m, nuvec_s *v);

static inline void Lever_AbsTargetPosInline(LEVER *lever, nuvec_s *out) {
  nuvec_s t = lever->v40;
  t.y += 0.005f;
  *out = t;
}

// FUNCTION: LEGOBATMAN 0x005baed0
static void Lever_NewPos(LEVER *lever, nuvec_s *pos) {
  nuvec_s p;
  lever->pos68 = *pos;
  lever->v40.x = lever->v40.y = 0.0f;
  lever->v40.z = g_unk00960044;
  NuVecRotateY(&lever->v40, &lever->v40, lever->rot_y + 0x8000);
  NuVecAdd(&lever->v40, &lever->v40, &lever->pos68);
  Lever_AbsTargetPosInline(lever, &p);
  p.y = lever->pos68.y;
  lever->v40.y = GameShadow(NULL, &lever->v40, 0.2f, -1);
  lever->v4c.y = GameShadow(NULL, &p, 0.2f, -1);
  if (lever->v4c.y != 2000000.0) {
    lever->v4c.y += 0.005f;
    FindAnglesZX(&g_unk009f7a44, &lever->s90, &lever->s92);
  }
  NuMtxSetRotationY((numtx_s *)lever, lever->rot_y);
  NuMtxTranslate((numtx_s *)lever, &lever->pos68);
}

// FUNCTION: LEGOBATMAN 0x005bafe0
static void Lever_Reset(LEVER *lever) {
  Lever_SetColour(lever);
  Lever_NewPos(lever, &lever->pos68);
  lever->state |= 0x90;
  lever->fac = 0.0f;
  lever->s8e = 0;
  if (!(lever->state & 2) || !(lever->state & 0x20)) {
    lever->state &= 0xe7b0;
    lever->pull = 0.0f;
    lever->f7c = 0.0f;
  }
  if (lever->plat != -1)
    PlatOnOff(lever->plat, lever->visible);
}

// FUNCTION: LEGOBATMAN 0x005bb060
void Lever_Activate(GIZMO *gizmo, i32 active) {
  if (gizmo != NULL && gizmo->object != NULL) {
    LEVER *lever = (LEVER *)gizmo->object;
    lever->active = active != 0;
    if (active)
      Lever_Reset(lever);
  }
}

// FUNCTION: LEGOBATMAN 0x005bb0a0
void Levers_Reset(void *world_ptr, void *unused, void *progress_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  if (world == NULL)
    return;
  LEVER *lever = (LEVER *)world->levers;
  if (lever == NULL)
    return;
  LEVERPROGRESS *progress = (LEVERPROGRESS *)progress_ptr;
  for (i32 i = 0; i < world->lever_count; i++, lever++) {
    Lever_Reset(lever);
    if (progress != NULL && i < 32) {
      i32 word = i / 32;
      u32 bit = 1 << (i & 31);
      if (progress->down[word] & bit) {
        lever->being_pulled = 1;
        lever->pull = 49.0f;
        lever->s8e = 0x8000;
      }
      lever->visible = (progress->visible[word] & bit) != 0;
      lever->active = (progress->active[word] & bit) != 0;
      if (lever->plat != -1)
        PlatOnOff(lever->plat, lever->visible);
    }
  }
}

// GLOBAL: LEGOBATMAN 0x00a95fe0
extern f32 FRAMETIME;
// GLOBAL: LEGOBATMAN 0x009cabf4
extern i32 g_unk009cabf4;

nuvec_s *NuSpecialGetDrawPos(nuhspecial_s *sp);
void GameAudio_PlaySfx(i32 sfx, nuvec_s *position, i32 flags, i32 volume);
f32 SeekLinearF(f32 current, f32 target, f32 step);

// FUNCTION: LEGOBATMAN 0x005bb1a0
void Levers_Update(void *world_ptr, void *unused, f32 dt) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  nuvec_s p;
  LEVER *lever = world->levers;
  if (lever == NULL)
    return;
  for (i32 i = 0; i < world->lever_count; i++, lever++) {
    if (NuSpecialExistsFn(&lever->special))
      Lever_NewPos(lever, NuSpecialGetDrawPos(&lever->special));
    if ((lever->state & 0x2000) && lever->active) {
      Lever_AbsTargetPosInline(lever, &p);
      p.y = lever->pos68.y;
      lever->v4c.y = GameShadow(NULL, &p, 0.25f, -1);
      if (lever->v4c.y != 2000000.0) {
        lever->v4c.y += 0.005f;
        FindAnglesZX(&g_unk009f7a44, &lever->s90, &lever->s92);
      }
    }
    if (lever->being_pulled || (lever->state & 0x1800)) {
      if (!lever->being_pulled)
        lever->being_pulled = 1;
      if (lever->f7c > 0.0f) {
        if ((lever->f7c -= FRAMETIME) <= 0.0f) {
          GameAudio_PlaySfx(0x93, &lever->pos68, 0, 0);
          lever->f7c = 0.0f;
          lever->state = (lever->state & ~0x800) | 0x1040;
        }
      } else if (!lever->b6 && !(lever->state & 0x1000)) {
        if (lever->pull < 17.0f) {
          if (lever->pull == 0.0f)
            GameAudio_PlaySfx(0x94, &lever->pos68, 0, 0);
          if ((lever->pull += FRAMETIME * 30.0f) >= 17.0f) {
            lever->pull = 17.0f;
            if (lever->b5)
              lever->f7c = lever->f80 + 0.001f;
          }
        }
      } else {
        if (!lever->b6)
          lever->b6 = 1;
        if ((lever->pull += FRAMETIME * 30.0f) >= 49.0f) {
          lever->state &= 0xefb0;
          lever->pull = 0.0f;
          lever->f7c = 0.0f;
          lever->f74 = 0.0f;
        }
      }
      lever->s8e = (u16)(lever->pull / 0.6f * -32768.0f);
    } else if (!lever->b0) {
      lever->s8e = 0;
    }
    f32 target;
    if (g_unk009cabf4 != 0 || (lever->active && lever->visible &&
                               !(lever->state & 3) && lever->pull == 0.0f))
      target = 1.0f;
    else
      target = 0.0f;
    if (lever->fac != target)
      lever->fac = SeekLinearF(lever->fac, target, FRAMETIME * 4.0f);
  }
}

// FUNCTION: LEGOBATMAN 0x005bada0
void Levers_StoreProgress(void *world_ptr, void *unused, void *progress_ptr) {
  LEVERPROGRESS *progress = (LEVERPROGRESS *)progress_ptr;
  if (progress == NULL)
    return;
  progress->down[0] = 0;
  memset(progress->active, 0xff, 8);
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  if (world == NULL)
    return;
  LEVER *lever = (LEVER *)world->levers;
  if (lever == NULL)
    return;
  for (i32 i = 0; i < world->lever_count; i++, lever++) {
    if (i >= 32)
      break;
    i32 word = i / 32;
    u32 bit = 1 << (i & 31);
    if (lever->being_pulled && lever->pull >= 0.6f)
      progress->down[word] |= bit;
    if (!lever->visible)
      progress->visible[word] &= ~bit;
    if (!lever->active)
      progress->active[word] &= ~bit;
  }
}

// GLOBAL: LEGOBATMAN 0x00960048
i32 lever_gizmotype_id = -1;

// FUNCTION: LEGOBATMAN 0x005bb4f0
ADDGIZMOTYPE *Levers_RegisterGizmo(i32 type_id) {
  // GLOBAL: LEGOBATMAN 0x00960100
  static char *name = "Lever";
  // GLOBAL: LEGOBATMAN 0x00ab0778
  static ADDGIZMOTYPE addtype;

  addtype = Default_ADDGIZMOTYPE;
  addtype.name = name;
  addtype.prefix = "";
  addtype.progress_size = sizeof(LEVERPROGRESS);
  addtype.fns[0] = (void *)Levers_GetMaxGizmos;
  addtype.fns[1] = (void *)Levers_AddGizmos;
  addtype.fns[2] = NULL;
  addtype.fns[3] = (void *)Levers_Update;
  addtype.fns[4] = (void *)Levers_Draw;
  addtype.fns[5] = NULL;
  addtype.fns[6] = (void *)Lever_GetGizmoName;
  addtype.fns[7] = (void *)Lever_GetOutput;
  addtype.fns[8] = (void *)Lever_GetOutputName;
  addtype.fns[9] = (void *)Lever_GetNumOutputs;
  addtype.fns[10] = (void *)Lever_Activate;
  addtype.fns[11] = (void *)Lever_ActivateRev;
  addtype.fns[12] = (void *)Lever_SetVisibility;
  addtype.fns[13] = NULL;
  addtype.fns[14] = NULL;
  addtype.fns[15] = NULL;
  addtype.fns[16] = NULL;
  addtype.fns[17] = NULL;
  addtype.fns[18] = NULL;
  addtype.fns[19] = (void *)Levers_AllocateProgressData;
  addtype.fns[20] = (void *)Levers_ClearProgress;
  addtype.fns[21] = (void *)Levers_StoreProgress;
  addtype.fns[22] = (void *)Levers_Reset;
  addtype.fns[23] = (void *)Levers_ReserveBufferSpace;
  addtype.fns[24] = (void *)Levers_Load;
  addtype.fns[25] = NULL;
  addtype.fns[26] = NULL;
  addtype.fns[27] = NULL;
  lever_gizmotype_id = type_id;
  return &addtype;
}
