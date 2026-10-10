// gameapi/gizdig_unk.cpp: dig gizmo sound configuration (GizDigSFX_*);
// file name unproven.

#include "../batman/worldinfo_unk.h"
#include "../nu2api/nucore/common.h"
#include "../nu2api/nucore/nustring.h"
#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x0063ae60
static f32 NuFabs(f32 f);
// FUNCTION: LEGOBATMAN 0x0063ae80
static f32 NuSinApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x0063af20
static f32 NuCosApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x0063af30
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

typedef struct nufpar_s {
  u8 pad0[0x910];
  char *word_buf; // 0x910
} NUFPAR;

typedef struct nufpcomjmp_s {
  char *fn_name;
  void (*fn)(NUFPAR *parser);
} NUFPCOMJMP;

struct GAMEANIMOBJ_s {
  GAMEANIMOBJ_s *next; // 0x00
  u32 special[3];      // 0x04, nuhspecial_s
};

struct GAMEANIMSET_s {
  u8 pad0[0x18];
  i32 state; // 0x18
  u8 pad1c[0x24 - 0x1c];
  GAMEANIMOBJ_s *objects; // 0x24
};

struct GIZDIG_s {
  char name[0x10];  // 0x00
  nuvec_s position; // 0x10
  u8 pad1c[0x34 - 0x1c];
  GAMEANIMSET_s *anim_set; // 0x34
  u8 pad38[0x58 - 0x38];
  u32 flags58; // 0x58
  u8 pad5c[0x70 - 0x5c];
  u16 u70; // 0x70
  u8 pad72[0x76 - 0x72];
  u16 u76; // 0x76
  u8 pad78[0x7a - 0x78];
  i16 sfx_process; // 0x7a
  i16 s7c;         // 0x7c, resolved in PostLoad when flags bit 11 is set
  u16 score;       // 0x7e
  u8 pad80[0x9c - 0x80];
  union {
    u32 flags; // 0x9c
    struct {
      u32 active : 1;  // bit 0
      u32 visible : 1; // bit 1
    };
  };
};

struct GIZDIGSYS_s {
  GIZDIG_s *digs; // 0x00
  void *p4;       // 0x04
  void *p8;       // 0x08
  u16 max;        // 0x0c
  u16 count;      // 0x0e
  u8 pad10[0x14 - 0x10];
  void *pool; // 0x14
  void *p18;  // 0x18
};

NUFPAR *NuFParCreateMem(char *name, char *buffer, i32 bufferSize);
i32 NuFParPushCom(NUFPAR *parser, NUFPCOMJMP *commands);
i32 NuFParGetLine(NUFPAR *parser);
i32 NuFParGetWord(NUFPAR *parser);
i32 NuFParInterpretWord(NUFPAR *parser);
void NuFParPopCom(NUFPAR *parser);
void NuFParDestroy(NUFPAR *parser);
char *NuSpecialGetName(nuhspecial_s *sp);
i32 GetSfxId(char *name);

// GLOBAL: LEGOBATMAN 0x009656e0
extern i32 GizDigSFX_load_version;
// GLOBAL: LEGOBATMAN 0x00aced24
extern WORLDINFO_s *GizDigSFX_worldinfo;
// GLOBAL: LEGOBATMAN 0x00aced20
extern GIZDIG_s *GizDigSFX_dig;
// GLOBAL: LEGOBATMAN 0x009656e4
extern NUFPCOMJMP GizDigSFX_ConfigKeywords[];

// FUNCTION: LEGOBATMAN 0x0063af70
static GIZDIG_s *GizDigFindByNameUnk0063af70(WORLDINFO_s *world, char *name) {
  GIZDIGSYS_s *system = world->giz_dig_sys;
  if (system != 0) {
    GIZDIG_s *dig = system->digs;
    for (i32 i = 0; i < system->count; i++, dig++) {
      if (dig->anim_set != 0) {
        for (GAMEANIMOBJ_s *object = dig->anim_set->objects; object != 0;
             object = object->next) {
          char *special_name =
              NuSpecialGetName((nuhspecial_s *)object->special);
          if (special_name != 0 && NuStrICmp(name, special_name) == 0)
            return dig;
        }
      }
    }
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x0063afe0
void GizDigSFX_digname(NUFPAR *parser) {
  GizDigSFX_dig = 0;
  if (NuFParGetWord(parser))
    GizDigSFX_dig =
        GizDigFindByNameUnk0063af70(GizDigSFX_worldinfo, parser->word_buf);
}

// FUNCTION: LEGOBATMAN 0x0063b020
void GizDigSFX_processsfx(NUFPAR *parser) {
  if (GizDigSFX_dig != 0 && NuFParGetWord(parser) &&
      GizDigSFX_dig->sfx_process == -1)
    GizDigSFX_dig->sfx_process = GetSfxId(parser->word_buf);
}

// FUNCTION: LEGOBATMAN 0x0063b070
void GizDigSFX_completesfx(NUFPAR *parser) {}

// FUNCTION: LEGOBATMAN 0x0063b080
void GizDigSFX_returnsfx(NUFPAR *parser) {}

// FUNCTION: LEGOBATMAN 0x0063b090
void GizDigSFX_Configure(WORLDINFO_s *world, char *config) {
  if (GizDigSFX_load_version >= 1 || world == 0 || world->giz_dig_sys == 0 ||
      world->giz_dig_sys->count == 0)
    return;
  GizDigSFX_dig = 0;
  GizDigSFX_worldinfo = world;
  NUFPAR *parser = NuFParCreateMem("digSFX", config, 0xffff);
  if (parser == 0)
    return;
  NuFParPushCom(parser, GizDigSFX_ConfigKeywords);
  i32 inside = 0;
  while (NuFParGetLine(parser)) {
    while (NuFParGetWord(parser)) {
      if (inside) {
        if (NuStrICmp(parser->word_buf, "digsfx_end") == 0) {
          inside = 0;
        } else {
          NuFParInterpretWord(parser);
        }
      } else if (NuStrICmp(parser->word_buf, "digsfx_start") == 0) {
        inside = 1;
      }
    }
  }
  NuFParPopCom(parser);
  NuFParDestroy(parser);
}

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_gizdig_unk(f32 *v, f32 a, i32 i) {
  v[2] = NuFabs(a);
  v[0] = NuSinApprox(i);
  v[1] = NuCosApprox(i);
  NuVec4Set(v, a, a, a, a);
}

// The gizmo callbacks (Mac GizDigs_* / GizmoDig_*), named by their
// RegisterGizmo 0x63c750 slot.

#include "../batman/leveldata_unk.h"
#include <stddef.h>
#include <string.h>

typedef struct GIZDIGPROGRESS_s {
  u32 active[4];  // 0x00
  u32 visible[4]; // 0x10
  u32 a20[4];     // 0x20, flags bit 10
  u32 a30[4];     // 0x30, flags bit 12
  u32 a40[4];     // 0x40, flags bit 15
} GIZDIGPROGRESS;

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
// GLOBAL: LEGOBATMAN 0x009604cc
extern i32 g_unk009604cc; // the dig special move

i32 NuStrLen(const char *s);
int NuStrICmp(const char *a, const char *b);
i32 NuAtan2D(f32 dx, f32 dy);
void AddGizmo(GIZMOSYS_s *gizmo_sys, i32 type_id, void *a, void *object);
void *GameBufferAlloc(variptr_u *buf, variptr_u *buf_end, i32 size);
void *GameBufferAllocProgressUnk005bbaf0(VARIPTR *buf, VARIPTR *buf_end,
                                         i32 size);
void AddLevelSfxFromId(i32 sfx_id, i32 *sfx_ids, i32 *sfx_count, i32 max_sfx);
void Unk00604f10(void *parts, i32 visible);
void Unk00604b40(GAMEANIMSET_s *parts);
void Unk00603cf0(GAMEANIMSET_s *parts);
void *Unk00603df0(variptr_u *buf, variptr_u *buf_end, i32 a, i32 count);
void *Unk00603bc0(variptr_u *buf, variptr_u *buf_end, void *pool,
                  struct GAMEANIMSYS_s *anim_sys);
i16 Unk005de820(void *world, i32 id);

// FUNCTION: LEGOBATMAN 0x0063b4c0
void GizDigs_PostLoad(void *world, void *sys_ptr) {
  GIZDIGSYS_s *sys = (GIZDIGSYS_s *)sys_ptr;
  if (sys == NULL)
    return;
  GIZDIG_s *dig = sys->digs;
  for (i32 i = 0; i < sys->count; i++, dig++) {
    if (dig->flags & 0x800) {
      dig->s7c = Unk005de820(world, dig->s7c);
      dig->flags &= ~0x800;
    }
  }
}

// FUNCTION: LEGOBATMAN 0x0063b520
void GizDigs_AddLevelSfx(void *world, void *sys_ptr, i32 *sfx_ids,
                         i32 *sfx_count, i32 max_sfx) {
  GIZDIGSYS_s *sys = (GIZDIGSYS_s *)sys_ptr;
  if (sys == NULL)
    return;
  GIZDIG_s *dig = sys->digs;
  for (i32 i = 0; i < sys->count; i++, dig++) {
    if (dig->sfx_process != -1)
      AddLevelSfxFromId(dig->sfx_process, sfx_ids, sfx_count, max_sfx);
  }
}

// FUNCTION: LEGOBATMAN 0x0063b580
void *GizDigs_ReserveBufferSpace(void *world_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  GIZDIGSYS_s *sys = (GIZDIGSYS_s *)GameBufferAlloc(
      &world->buf104, &world->bufEnd108, sizeof(GIZDIGSYS_s));
  sys->max = world->current_level->maxdig;
  sys->digs = (GIZDIG_s *)GameBufferAlloc(&world->buf104, &world->bufEnd108,
                                          sys->max * sizeof(GIZDIG_s));
  sys->p8 = GameBufferAlloc(&world->buf104, &world->bufEnd108, sys->max * 4);
  sys->p4 = GameBufferAlloc(&world->buf104, &world->bufEnd108, sys->max * 4);
  sys->pool = Unk00603df0(&world->buf104, &world->bufEnd108, 0xcc,
                          world->current_level->maxdig_objects);
  sys->p18 = GameBufferAlloc(&world->buf104, &world->bufEnd108, 0x200);
  for (i32 i = 0; i < 128; i++)
    sys->digs[i].anim_set = (GAMEANIMSET_s *)Unk00603bc0(
        &world->buf104, &world->bufEnd108, sys->pool, world->game_anim_sys);
  world->giz_dig_sys = sys;
  return sys;
}

// FUNCTION: LEGOBATMAN 0x0063b660
void GizmoDig_Activate(GIZMO *gizmo, i32 active) {
  if (gizmo == NULL)
    return;
  GIZDIG_s *dig = (GIZDIG_s *)gizmo->object;
  if (active && !dig->active)
    Unk00604b40(dig->anim_set);
  dig->active = active != 0;
  if (active) {
    dig->flags &= ~0x200a0;
    dig->u70 = 0;
    dig->u76 = 0;
    if (dig->flags58 & 8)
      dig->flags &= ~0x400;
    dig->flags &= ~0xf000;
    Unk00603cf0(dig->anim_set);
  }
}

// FUNCTION: LEGOBATMAN 0x0063b6f0
void GizDig_SetVisibility(GIZDIG_s *dig, i32 visible) {
  if (dig != NULL) {
    Unk00604f10(dig->anim_set, visible);
    dig->visible = visible != 0;
  }
}

// FUNCTION: LEGOBATMAN 0x0063b730
void GizmoDig_SetVisibility(GIZMO *gizmo, i32 visible) {
  if (gizmo != NULL)
    GizDig_SetVisibility((GIZDIG_s *)gizmo->object, visible);
}

// FUNCTION: LEGOBATMAN 0x0063b770
nuvec_s *GizmoDig_GetPos(GIZMO *gizmo) {
  if (gizmo != NULL && gizmo->object != NULL)
    return &((GIZDIG_s *)gizmo->object)->position;
  return NULL;
}

// FUNCTION: LEGOBATMAN 0x0063bb60
void GizDigs_EarlyUpdate(void *world_ptr, void *unused, f32 dt) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  if (world->giz_dig_sys == NULL)
    return;
  GIZDIG_s *dig = world->giz_dig_sys->digs;
  for (i32 i = 0; i < world->giz_dig_sys->count; i++, dig++)
    dig->flags &= ~0x200000;
}

// FUNCTION: LEGOBATMAN 0x0063c330
i32 GizDigs_AngleTodig(nuvec_s *pos, GIZDIG_s *dig) {
  return NuAtan2D(dig->position.x - pos->x, dig->position.z - pos->z);
}

// FUNCTION: LEGOBATMAN 0x0063c370
i32 GizDig_AnimComplete(GIZDIG_s *dig) {
  if (dig != NULL && dig->anim_set != NULL) {
    if (dig->flags & 0x80) {
      if (dig->anim_set->state != 0)
        return 0;
    } else if (dig->anim_set->state != 2) {
      return 0;
    }
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x0063c3f0
i32 GizDig_GameObjUsingdig(GameObject_s *obj, GIZDIG_s *dig) {
  if (dig != NULL && obj != NULL && obj->b9db == g_unk009604cc &&
      obj->dig == dig)
    return 1;
  return 0;
}

// FUNCTION: LEGOBATMAN 0x0063c420
i32 GizDigs_GetMaxGizmos(void *world_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  if (world != NULL)
    return world->current_level->maxdig;
  return 0;
}

// FUNCTION: LEGOBATMAN 0x0063c440
void GizDigs_AddGizmos(GIZMOSYS_s *gizmo_sys, i32 type_id, void *world,
                       void *sys_ptr) {
  GIZDIGSYS_s *sys = (GIZDIGSYS_s *)sys_ptr;
  if (sys == NULL)
    return;
  for (i32 i = 0; i < sys->count; i++) {
    if (NuStrLen(sys->digs[i].name) != 0)
      AddGizmo(gizmo_sys, type_id, NULL, &sys->digs[i]);
  }
}

// FUNCTION: LEGOBATMAN 0x0063c4a0
char *GizmoDig_GetGizmoName(GIZMO *gizmo) {
  return gizmo != NULL ? ((GIZDIG_s *)gizmo->object)->name : NULL;
}

// FUNCTION: LEGOBATMAN 0x0063c4b0
i32 GizmoDig_GetOutput(GIZMO *gizmo, i32 output, i32 b) {
  GIZDIG_s *dig = (GIZDIG_s *)gizmo->object;
  if (((dig->visible || dig->s7c != -1) && dig->active) || b != 0) {
    switch (output) {
    case 0:
      if (dig->anim_set != NULL && dig->anim_set->state == 2)
        return 1;
      break;
    case 1:
      if (dig->anim_set != NULL && dig->anim_set->state != 0)
        return 1;
      break;
    case 2:
      if (dig->anim_set != NULL && dig->anim_set->state == 0)
        return 1;
      break;
    }
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x0063c520
char *GizmoDig_GetOutputName(GIZMO *gizmo, i32 output) {
  switch (output) {
  case 0:
    return "AtEnd";
  case 1:
    return "NotAtStart";
  case 2:
    return "AtStart";
  }
  return NULL;
}

// FUNCTION: LEGOBATMAN 0x0063c550
i32 GizmoDig_GetNumOutputs(GIZMO *gizmo) { return 3; }

// FUNCTION: LEGOBATMAN 0x0063c560
void *GizDigs_AllocateProgressData(VARIPTR *buf, VARIPTR *buf_end) {
  return GameBufferAllocProgressUnk005bbaf0(buf, buf_end,
                                            sizeof(GIZDIGPROGRESS));
}

// FUNCTION: LEGOBATMAN 0x0063c580
void GizDigs_ClearProgress(void *world, void *progress_ptr) {
  GIZDIGPROGRESS *progress = (GIZDIGPROGRESS *)progress_ptr;
  if (progress != NULL) {
    memset(progress->active, 0xff, sizeof(progress->active));
    memset(progress->visible, 0xff, sizeof(progress->visible));
    memset(progress->a20, 0, sizeof(progress->a20));
    memset(progress->a30, 0, sizeof(progress->a30));
    memset(progress->a40, 0, sizeof(progress->a40));
  }
}

// FUNCTION: LEGOBATMAN 0x0063c5d0
void GizDigs_StoreProgress(void *world, void *sys_ptr, void *progress_ptr) {
  GIZDIGSYS_s *sys = (GIZDIGSYS_s *)sys_ptr;
  GIZDIGPROGRESS *progress = (GIZDIGPROGRESS *)progress_ptr;
  if (progress == NULL)
    return;
  memset(progress->active, 0xff, sizeof(progress->active));
  memset(progress->visible, 0xff, sizeof(progress->visible));
  memset(progress->a20, 0, sizeof(progress->a20));
  memset(progress->a30, 0, sizeof(progress->a30));
  memset(progress->a40, 0, sizeof(progress->a40));
  GIZDIG_s *dig = sys->digs;
  for (i32 i = 0; i < sys->count; i++, dig++) {
    if (i >= 128)
      break;
    i32 word = i / 32;
    u32 bit = 1 << (i & 31);
    if (!dig->visible)
      progress->visible[word] &= ~bit;
    if (!dig->active)
      progress->active[word] &= ~bit;
    if (dig->flags & 0x400)
      progress->a20[word] |= bit;
    if (dig->flags & 0x1000)
      progress->a30[word] |= bit;
    if (dig->flags & 0x8000)
      progress->a40[word] |= bit;
  }
}

// FUNCTION: LEGOBATMAN 0x0063c6b0
GIZDIG_s *GizDigs_FindDig(WORLDINFO_s *world, char *name) {
  if (world->giz_dig_sys != NULL) {
    GIZDIG_s *dig = world->giz_dig_sys->digs;
    for (i32 i = 0; i < world->giz_dig_sys->count; i++, dig++) {
      if (NuStrICmp(dig->name, name) == 0)
        return dig;
    }
  }
  return NULL;
}

// FUNCTION: LEGOBATMAN 0x0063c710
u32 GizDig_TotalScore(void *world) {
  u32 total = 0;
  GIZDIGSYS_s *sys = ((WORLDINFO_s *)world)->giz_dig_sys;
  if (sys != NULL) {
    GIZDIG_s *dig = sys->digs;
    if (dig != NULL) {
      for (i32 i = 0; i < sys->count; i++, dig++)
        total += dig->score;
    }
  }
  return total;
}

i32 GizDigs_Load(void *world, void *sys);
void GizDigs_Reset(void *world, void *sys, void *progress);
void GizDigs_LateUpdate(void *world, void *sys, f32 dt);
void GizDigs_Draw(void *world, void *sys, f32 dt);

// GLOBAL: LEGOBATMAN 0x009656d8
i32 dig_gizmotype_id = -1;

// FUNCTION: LEGOBATMAN 0x0063c750
ADDGIZMOTYPE *GizDig_RegisterGizmo(i32 type_id) {
  // GLOBAL: LEGOBATMAN 0x009657c8
  static char *name = "GizDig";
  // GLOBAL: LEGOBATMAN 0x00aced58
  static ADDGIZMOTYPE addtype;

  addtype = Default_ADDGIZMOTYPE;
  addtype.name = name;
  addtype.prefix = "";
  addtype.progress_size = sizeof(GIZDIGPROGRESS);
  addtype.fns[0] = (void *)GizDigs_GetMaxGizmos;
  addtype.fns[1] = (void *)GizDigs_AddGizmos;
  addtype.fns[2] = (void *)GizDigs_EarlyUpdate;
  addtype.fns[3] = (void *)GizDigs_LateUpdate;
  addtype.fns[4] = (void *)GizDigs_Draw;
  addtype.fns[5] = NULL;
  addtype.fns[6] = (void *)GizmoDig_GetGizmoName;
  addtype.fns[7] = (void *)GizmoDig_GetOutput;
  addtype.fns[8] = (void *)GizmoDig_GetOutputName;
  addtype.fns[9] = (void *)GizmoDig_GetNumOutputs;
  addtype.fns[10] = (void *)GizmoDig_Activate;
  addtype.fns[12] = (void *)GizmoDig_SetVisibility;
  addtype.fns[13] = NULL;
  addtype.fns[14] = (void *)GizmoDig_GetPos;
  addtype.fns[15] = NULL;
  addtype.fns[19] = (void *)GizDigs_AllocateProgressData;
  addtype.fns[20] = (void *)GizDigs_ClearProgress;
  addtype.fns[21] = (void *)GizDigs_StoreProgress;
  addtype.fns[22] = (void *)GizDigs_Reset;
  addtype.fns[23] = (void *)GizDigs_ReserveBufferSpace;
  addtype.fns[24] = (void *)GizDigs_Load;
  addtype.fns[25] = NULL;
  addtype.fns[26] = (void *)GizDigs_PostLoad;
  addtype.fns[27] = (void *)GizDigs_AddLevelSfx;
  dig_gizmotype_id = type_id;
  return &addtype;
}
