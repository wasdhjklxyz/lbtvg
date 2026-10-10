// batman/, file unknown: gizmo flow actions (0x00483e20).

#include "../nu2api/nucore/nustring.h"

struct GIZFLOW_s;
struct FLOWBOX_s;

struct GIZMOSYS_s;
struct GIZFLOW_s {
  GIZMOSYS_s *gizmo_sys; // 0x00
};

struct GIZOBSTACLE_s {
  u8 pad0[0xc8];
  u32 flags_c8_lo : 13;
  u32 stay_open : 1; // 0xc8 bit 13
  u32 stay_shut : 1; // 0xc8 bit 14
  u32 flags_c8_hi : 17;
};

struct GIZMO_s {
  void *object; // 0x00
};

// GLOBAL: LEGOBATMAN 0x0095ff24
extern i32 obstacle_gizmotype_id;

GIZMO_s *GizmoFindByName(GIZMOSYS_s *gizmo_sys, i32 type_id, char *name);
void GizObstacle_JumpToEnd(GIZOBSTACLE_s *obstacle);
void GizObstacle_PlayForwards(GIZOBSTACLE_s *obstacle);
void GizObstacle_JumpToStart(GIZOBSTACLE_s *obstacle);
void GizObstacle_PlayBackwards(GIZOBSTACLE_s *obstacle);

// from saga legoapi/gizmo/gizmos/gizactions.cpp
// FUNCTION: LEGOBATMAN 0x00483750
void GizActions_PlayObstacle(GIZFLOW_s *flow, FLOWBOX_s *box, char **params,
                             int count) {
  i32 forwards = 1;
  i32 snap = 0;
  i32 stay_open = 0;
  char *name = 0;
  i32 stay_shut = 0;
  for (i32 index = 0; index < count; ++index) {
    char *value = NuStrIStr(params[index], "Name");
    if (value != 0) {
      name = value + NuStrLen("Name") + 1;
    } else if (NuStrICmp(params[index], "BACKWARD") == 0) {
      forwards = 0;
    } else if (NuStrICmp(params[index], "FORWARD") == 0) {
      forwards = 1;
    } else if (NuStrICmp(params[index], "SNAP") == 0) {
      snap = 1;
    } else if (NuStrICmp(params[index], "STAYOPEN") == 0) {
      stay_open = 1;
    } else if (NuStrICmp(params[index], "STAYSHUT") == 0) {
      stay_shut = 1;
    }
  }
  if (name == 0)
    return;
  GIZMO_s *gizmo =
      GizmoFindByName(flow->gizmo_sys, obstacle_gizmotype_id, name);
  GIZOBSTACLE_s *obstacle = gizmo != 0 ? (GIZOBSTACLE_s *)gizmo->object : 0;
  if (obstacle == 0)
    return;
  if (forwards != 0) {
    if (snap != 0)
      GizObstacle_JumpToEnd(obstacle);
    else
      GizObstacle_PlayForwards(obstacle);
  } else if (snap != 0) {
    GizObstacle_JumpToStart(obstacle);
  } else {
    GizObstacle_PlayBackwards(obstacle);
  }
  obstacle->stay_open = stay_open;
  obstacle->stay_shut = stay_shut;
}

void PlayRadio(char *special, char *blowup, i32 loop);

// FUNCTION: LEGOBATMAN 0x00483e20
void GizActions_PlayRadio(GIZFLOW_s *flow, FLOWBOX_s *box, char **args,
                          int argc) {
  i32 loop = 1;
  char *special = 0;
  char *blowup = 0;
  char *s;
  i32 i;
  for (i = 0; i < argc; i++) {
    if ((s = NuStrIStr(args[i], "BlowUp=")))
      blowup = s + NuStrLen("BlowUp=");
    else if ((s = NuStrIStr(args[i], "Special=")))
      special = s + NuStrLen("Special=");
    else if (!NuStrICmp(args[i], "FALSE"))
      loop = 0;
  }
  if (special || blowup)
    PlayRadio(special, blowup, loop);
}

#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"
#include "worldinfo_unk.h"

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x00486010
static f32 NuSinApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x004860d0
static f32 NuCosApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x004860f0
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

struct GAMEANIMOBJ_s {
  GAMEANIMOBJ_s *next; // 0x00
  u32 special[3];      // 0x04, nuhspecial_s
  u8 pad10[0x24 - 0x10];
  void *object_data; // 0x24
};

struct GAMEANIMSET_s {
  u8 pad0[0x24];
  GAMEANIMOBJ_s *objects; // 0x24
};

struct GIZFORCE_s {
  char name[0x10];         // 0x00
  nuvec_s v10;             // 0x10
  nuvec_s position;        // 0x1c
  GAMEANIMSET_s *anim_set; // 0x28
  u32 special2c[3];        // 0x2c, nuhspecial_s
  u8 pad38[0x44 - 0x38];
  f32 f44; // 0x44
  u8 pad48[0x4c - 0x48];
  f32 f4c; // 0x4c
  u8 pad50[0x54 - 0x50];
  f32 f54; // 0x54
  u8 pad58[0x5c - 0x58];
  f32 f5c; // 0x5c
  f32 f60; // 0x60
  f32 f64; // 0x64
  u8 pad68[0x6c - 0x68];
  f32 f6c;     // 0x6c
  f32 f70;     // 0x70
  u32 flags74; // 0x74
  u8 pad78[0x7c - 0x78];
  u8 b7c; // 0x7c
  u8 b7d; // 0x7d
  u8 b7e; // 0x7e
  u8 pad7f[0x82 - 0x7f];
  i16 sfx_process;  // 0x82
  i16 sfx_complete; // 0x84
  i16 sfx_return;   // 0x86
  i16 s88;          // 0x88, resolved in PostLoad when flags bit 10 is set
  u16 score;        // 0x8a
  i16 s8c;          // 0x8c
  i16 s8e;          // 0x8e
  nuvec_s v90;      // 0x90
  f32 f9c;          // 0x9c
  union {
    u32 flags; // 0xa0
    struct {
      u32 active : 1;  // bit 0
      u32 visible : 1; // bit 1
    };
  };
};

struct GIZFORCESYS_s {
  GIZFORCE_s *forces; // 0x00
  void *p4;           // 0x04
  void *p8;           // 0x08
  u16 max;            // 0x0c
  u16 count;          // 0x0e
  u8 pad10[0x14 - 0x10];
  void *pool; // 0x14
  u8 pad18[0x158 - 0x18];
};

typedef struct nufpar_s {
  u8 pad0[0x910];
  char *word_buf; // 0x910
} NUFPAR;

typedef struct nufpcomjmp_s {
  char *fn_name;
  void (*fn)(NUFPAR *parser);
} NUFPCOMJMP;

// GLOBAL: LEGOBATMAN 0x0093e148
extern i32 GizForceSFX_load_version;
// GLOBAL: LEGOBATMAN 0x009c6f3c
extern WORLDINFO_s *GizForceSFX_worldinfo;
// GLOBAL: LEGOBATMAN 0x009c6f38
extern GIZFORCE_s *GizForceSFX_force;
// GLOBAL: LEGOBATMAN 0x0093e14c
extern NUFPCOMJMP GizForceSFX_ConfigKeywords[];

NUFPAR *NuFParCreateMem(char *name, char *buffer, i32 bufferSize);
i32 NuFParPushCom(NUFPAR *parser, NUFPCOMJMP *commands);
i32 NuFParGetLine(NUFPAR *parser);
i32 NuFParGetWord(NUFPAR *parser);
i32 NuFParInterpretWord(NUFPAR *parser);
void NuFParPopCom(NUFPAR *parser);
void NuFParDestroy(NUFPAR *parser);

char *NuSpecialGetName(nuhspecial_s *sp);
i32 GetSfxId(char *name);

// FUNCTION: LEGOBATMAN 0x00486150
static GIZFORCE_s *GizForceFindByNameUnk00486150(WORLDINFO_s *world,
                                                 char *name) {
  GIZFORCESYS_s *system = world->giz_force_sys;
  if (system != 0) {
    GIZFORCE_s *force = system->forces;
    for (i32 i = 0; i < system->count; i++, force++) {
      if (force->anim_set != 0) {
        for (GAMEANIMOBJ_s *object = force->anim_set->objects; object != 0;
             object = object->next) {
          char *special_name =
              NuSpecialGetName((nuhspecial_s *)object->special);
          if (special_name != 0 && NuStrICmp(name, special_name) == 0)
            return force;
        }
      }
    }
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x004861e0
void GizForceSFX_forcename(NUFPAR *parser) {
  GizForceSFX_force = 0;
  if (NuFParGetWord(parser))
    GizForceSFX_force =
        GizForceFindByNameUnk00486150(GizForceSFX_worldinfo, parser->word_buf);
}

// FUNCTION: LEGOBATMAN 0x00486230
void GizForceSFX_processsfx(NUFPAR *parser) {
  if (GizForceSFX_force != 0 && NuFParGetWord(parser) &&
      GizForceSFX_force->sfx_process == -1)
    GizForceSFX_force->sfx_process = GetSfxId(parser->word_buf);
}

// FUNCTION: LEGOBATMAN 0x00486290
void GizForceSFX_completesfx(NUFPAR *parser) {
  if (GizForceSFX_force != 0 && NuFParGetWord(parser) &&
      GizForceSFX_force->sfx_complete == -1)
    GizForceSFX_force->sfx_complete = GetSfxId(parser->word_buf);
}

// FUNCTION: LEGOBATMAN 0x004862f0
void GizForceSFX_returnsfx(NUFPAR *parser) {
  if (GizForceSFX_force != 0 && NuFParGetWord(parser) &&
      GizForceSFX_force->sfx_return == -1)
    GizForceSFX_force->sfx_return = GetSfxId(parser->word_buf);
}

// from saga legoapi/gizmos/traps/gizforce.cpp
// FUNCTION: LEGOBATMAN 0x00486350
void GizForceSFX_Configure(WORLDINFO_s *world, char *config) {
  if (GizForceSFX_load_version >= 16 || world == 0 ||
      world->giz_force_sys == 0 || world->giz_force_sys->count == 0)
    return;
  GizForceSFX_force = 0;
  GizForceSFX_worldinfo = world;
  NUFPAR *parser = NuFParCreateMem("ForceSFX", config, 0xffff);
  if (parser == 0)
    return;
  NuFParPushCom(parser, GizForceSFX_ConfigKeywords);
  i32 inside = 0;
  while (NuFParGetLine(parser)) {
    while (NuFParGetWord(parser)) {
      if (inside) {
        if (NuStrICmp(parser->word_buf, "forcesfx_end") == 0) {
          inside = 0;
        } else {
          NuFParInterpretWord(parser);
        }
      } else if (NuStrICmp(parser->word_buf, "forcesfx_start") == 0) {
        inside = 1;
      }
    }
  }
  NuFParPopCom(parser);
  NuFParDestroy(parser);
}

struct GIZTURRET_s;
struct GIZMOBLOWUP_s;

// GLOBAL: LEGOBATMAN 0x00967a14
extern i32 turret_gizmotype_id;
// GLOBAL: LEGOBATMAN 0x00961104
extern i32 blowup_gizmotype_id;

void GizTurrets_Hit(void *world, GIZTURRET_s *turret, nuvec_s *pos, i32 a,
                    i32 damage);
void GizmoBlowupBlowup(GIZMOBLOWUP_s *blowup, i32 a, i32 b, i32 damage,
                       GameObject_s *obj, i32 c);
f32 NuAToF(char *string);

// from saga legoapi/gizmo/gizmos/gizactions.cpp
// FUNCTION: LEGOBATMAN 0x00484590
void GizActions_HitBlowup(GIZFLOW_s *flow, FLOWBOX_s *box, char **params,
                          int count) {
  i8 gizmo_type = 0;
  i32 damage = 0;
  char *name = 0;
  for (i32 index = 0; index < count; ++index) {
    char *value = NuStrIStr(params[index], "name=");
    if (value != 0) {
      name = value + NuStrLen("name=");
    } else if (NuStrICmp(params[index], "BLOWUP") == 0) {
      gizmo_type = 0;
    } else if (NuStrICmp(params[index], "TURRET") == 0) {
      gizmo_type = 1;
    } else if ((value = NuStrIStr(params[index], "damage=")) != 0) {
      value += NuStrLen("damage=");
      damage = (i32)NuAToF(value);
    }
  }
  if (name == 0 || damage == 0)
    return;
  switch (gizmo_type) {
  case 0: {
    GIZMO_s *gizmo =
        GizmoFindByName(g_unk00960894->gizmoSys2b0c, blowup_gizmotype_id, name);
    if (gizmo != 0 && gizmo->object != 0)
      GizmoBlowupBlowup((GIZMOBLOWUP_s *)gizmo->object, 1, -1, damage, 0, 1);
    break;
  }
  case 1: {
    GIZMO_s *gizmo =
        GizmoFindByName(g_unk00960894->gizmoSys2b0c, turret_gizmotype_id, name);
    if (gizmo != 0 && gizmo->object != 0)
      GizTurrets_Hit(g_unk00960894, (GIZTURRET_s *)gizmo->object, 0, -1,
                     damage);
    break;
  }
  }
}

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_gizactions_unk(f32 *v, f32 a, i32 i) {
  v[0] = NuSinApprox(i);
  v[1] = NuCosApprox(i);
  NuVec4Set(v, a, a, a, a);
}

// The gizmo callbacks (Mac GizForces_* / GizmoForce_*), named by their
// RegisterGizmo 0x48a620 slot.

#include "leveldata_unk.h"
#include <string.h>

typedef struct GIZFORCEPROGRESS_s {
  u32 a0[4];    // 0x00
  u32 a10[4];   // 0x10
  u32 a20[4];   // 0x20
  u32 a30[4];   // 0x30
  u32 a40[4];   // 0x40
  u32 a50[4];   // 0x50
  u32 a60[4];   // 0x60
  u8 a70[0x40]; // 0x70
} GIZFORCEPROGRESS;

typedef struct ADDGIZMOTYPE_s {
  char *name;        // 0x00
  char *prefix;      // 0x04
  u16 progress_size; // 0x08
  void *fns[0x1c];   // 0x0c
} ADDGIZMOTYPE;

// GLOBAL: LEGOBATMAN 0x00960118
extern ADDGIZMOTYPE Default_ADDGIZMOTYPE;

void AddGizmo(GIZMOSYS_s *gizmo_sys, i32 type_id, void *a, void *object);
void *GameBufferAlloc(variptr_u *buf, variptr_u *buf_end, i32 size);
void *GameBufferAllocProgressUnk005bbaf0(VARIPTR *buf, VARIPTR *buf_end,
                                         i32 size);
void Unk00604f10(void *parts, i32 visible);
void *Unk00603df0(variptr_u *buf, variptr_u *buf_end, i32 a, i32 count);
void *Unk00603bc0(variptr_u *buf, variptr_u *buf_end, void *pool,
                  struct GAMEANIMSYS_s *anim_sys);
i16 Unk005de820(void *world, i32 id);

i32 EdFileReadInt();
char EdFileReadChar();
i16 EdFileReadShort();
f32 EdFileReadFloat();
void EdFileRead(void *buf, i32 len);
void EdFileReadNuVec(nuvec_s *v);
void GizmoFileReadGameAnimSet(GAMEANIMSET_s *set, void *world,
                              void (*fn)(GAMEANIMOBJ_s *, u8), u8 version,
                              char *type_name, char *name);
i32 GizmoBlowupGetNameTableId(char *name);
i32 GizmoFileReadName(char *name);
int NuSpecialFind(void *scene, nuhspecial_s *out, char *name, i32 a);

struct AREADATA_s {
  u8 pad0[0x7c];
  u32 flags7c; // 0x7c
};

// STUB: LEGOBATMAN 0x004864a0
// register-arg puzzle: orig takes size on the stack, ours in ecx.
static char *GizForceUnk004864a0(char *src, char *dst, i32 size) {
  i32 i = 0;
  if (dst == NULL || src == NULL)
    return NULL;
  char *ret = dst;
  while (i++ < size - 1) {
    char c = *src;
    *dst++ = c;
    src++;
    if (c == '_' || *src == 0)
      break;
  }
  return ret;
}

// STUB: LEGOBATMAN 0x004864f0
// matches except the 0x4864a0 call (ours passes size in ecx, orig pushes it).
static void GizForceUnk004864f0(GIZFORCE_s *force) {
  char name[0x40];
  if (force == NULL)
    return;
  memset(force->special2c, 0, sizeof(force->special2c));
  if (!(force->flags74 & 0x40))
    return;
  for (GAMEANIMOBJ_s *obj = force->anim_set->objects; obj != NULL;
       obj = obj->next) {
    char *special_name = NuSpecialGetName((nuhspecial_s *)obj->special);
    if (special_name != NULL) {
      memset(name, 0, sizeof(name));
      if (GizForceUnk004864a0(special_name, name, sizeof(name))) {
        NuStrCat(name, "b");
        if (NuSpecialFind(*(void **)obj->special,
                          (nuhspecial_s *)force->special2c, name, 1))
          return;
      }
    }
  }
}

// FUNCTION: LEGOBATMAN 0x004865d0
static void edgizforce_ReadAnimSetData(GAMEANIMOBJ_s *object, u8 version) {
  if (object == NULL)
    return;
  u32 fallback[2];
  u16 *data = (u16 *)object->object_data;
  if (data == NULL)
    data = (u16 *)fallback;
  if (version > 8)
    data[0] = EdFileReadShort();
}

// FUNCTION: LEGOBATMAN 0x00486610
i32 GizForces_Load(void *world_ptr, void *sys_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  GIZFORCESYS_s *sys = (GIZFORCESYS_s *)sys_ptr;
  nuvec_s tmp;
  char sfx_name[64];
  char blowup_name[32];
  u8 version = EdFileReadChar();
  GizForceSFX_load_version = version;
  sys->count = EdFileReadShort();
  if (sys->count == 0)
    return 1;
  GIZFORCE_s *force = sys->forces;
  for (i32 i = 0; i < sys->count; i++, force++) {
    EdFileRead(force->name, 0x10);
    EdFileReadNuVec(&force->v10);
    if (version == 1)
      EdFileReadNuVec(&tmp);
    force->f44 = EdFileReadFloat();
    if (version >= 8)
      force->f4c = EdFileReadFloat();
    force->f54 = EdFileReadFloat();
    if (version == 1) {
      EdFileReadNuVec(&tmp);
      EdFileReadShort();
    }
    force->flags74 = EdFileReadInt();
    if (version <= 12)
      force->flags74 |= 0x800;
    force->b7c = EdFileReadChar();
    if (version >= 11) {
      force->b7d = EdFileReadChar();
      if (version == 11 && force->b7d)
        force->b7d = 1 << (force->b7d - 1);
    }
    force->b7e = EdFileReadChar();
    if (version == 1)
      EdFileReadChar();
    GizmoFileReadGameAnimSet(force->anim_set, world, edgizforce_ReadAnimSetData,
                             version, "GizForce", force->name);
    force->f5c = EdFileReadFloat();
    force->f60 = EdFileReadFloat();
    if (version >= 6)
      force->f64 = EdFileReadFloat();
    if (version >= 7)
      force->f70 = EdFileReadFloat();
    else
      force->f70 = 1.0f;
    if (version >= 3)
      force->f6c = EdFileReadFloat();
    force->s88 = -1;
    if (version >= 4) {
      if (version >= 5) {
        i32 len = EdFileReadChar();
        if (len != 0) {
          EdFileRead(blowup_name, len);
          force->s88 = GizmoBlowupGetNameTableId(blowup_name);
          if (force->s88 != -1)
            force->flags |= 0x400;
        }
      } else {
        force->s88 = EdFileReadShort();
      }
      force->score = EdFileReadShort();
      force->s8c = EdFileReadShort();
      force->s8e = EdFileReadShort();
      EdFileReadNuVec(&force->v90);
    }
    if (version >= 10)
      force->f9c = EdFileReadFloat();
    else if (world->area != NULL && (world->area->flags7c & 1))
      force->f9c = 12.0f;
    else
      force->f9c = 1.75f;
    force->sfx_process = -1;
    force->sfx_complete = -1;
    force->sfx_return = -1;
    if (version >= 14) {
      if (GizmoFileReadName(sfx_name))
        force->sfx_process = GetSfxId(sfx_name);
      if (GizmoFileReadName(sfx_name))
        force->sfx_complete = GetSfxId(sfx_name);
      if (GizmoFileReadName(sfx_name))
        force->sfx_return = GetSfxId(sfx_name);
    }
    if (version <= 14) {
      force->sfx_process = -1;
      force->sfx_complete = -1;
      force->sfx_return = -1;
    }
    if (force->flags74 & 0x40)
      GizForceUnk004864f0(force);
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x004869f0
void GizForces_PostLoad(void *world, void *sys_ptr) {
  GIZFORCESYS_s *sys = (GIZFORCESYS_s *)sys_ptr;
  if (sys == NULL)
    return;
  GIZFORCE_s *force = sys->forces;
  for (i32 i = 0; i < sys->count; i++, force++) {
    if (force->flags & 0x400) {
      force->s88 = Unk005de820(world, force->s88);
      force->flags &= ~0x400;
    }
  }
}

// FUNCTION: LEGOBATMAN 0x00486b30
void *GizForces_ReserveBufferSpace(void *world_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  GIZFORCESYS_s *sys = (GIZFORCESYS_s *)GameBufferAlloc(
      &world->buf104, &world->bufEnd108, sizeof(GIZFORCESYS_s));
  sys->max = world->current_level->max_force;
  sys->forces = (GIZFORCE_s *)GameBufferAlloc(&world->buf104, &world->bufEnd108,
                                              sys->max * sizeof(GIZFORCE_s));
  sys->p8 = GameBufferAlloc(&world->buf104, &world->bufEnd108, sys->max * 4);
  sys->p4 = GameBufferAlloc(&world->buf104, &world->bufEnd108, sys->max * 4);
  sys->pool = Unk00603df0(&world->buf104, &world->bufEnd108, 8,
                          (u16)world->current_level->max_force_objects);
  for (i32 i = 0; i < sys->max; i++)
    sys->forces[i].anim_set = (GAMEANIMSET_s *)Unk00603bc0(
        &world->buf104, &world->bufEnd108, sys->pool, world->game_anim_sys);
  world->giz_force_sys = sys;
  return sys;
}

// FUNCTION: LEGOBATMAN 0x00486c60
void GizForce_SetVisibility(GIZFORCE_s *force, i32 visible) {
  if (force != NULL) {
    Unk00604f10(force->anim_set, visible);
    force->visible = visible != 0;
  }
}

// FUNCTION: LEGOBATMAN 0x00486cb0
void GizmoForce_SetVisibility(GIZMO_s *gizmo, i32 visible) {
  if (gizmo != NULL)
    GizForce_SetVisibility((GIZFORCE_s *)gizmo->object, visible);
}

// FUNCTION: LEGOBATMAN 0x00486d00
nuvec_s *GizmoForce_GetPos(GIZMO_s *gizmo) {
  if (gizmo != NULL && gizmo->object != NULL)
    return &((GIZFORCE_s *)gizmo->object)->position;
  return NULL;
}

// FUNCTION: LEGOBATMAN 0x00487810
i32 GizForces_GetMaxGizmos(void *world_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  if (world != NULL)
    return world->current_level->max_force;
  return 0;
}

// FUNCTION: LEGOBATMAN 0x00487830
void GizForces_AddGizmos(GIZMOSYS_s *gizmo_sys, i32 type_id, void *world,
                         void *sys_ptr) {
  GIZFORCESYS_s *sys = (GIZFORCESYS_s *)sys_ptr;
  if (sys == NULL)
    return;
  for (i32 i = 0; i < sys->count; i++) {
    if (NuStrLen(sys->forces[i].name) != 0)
      AddGizmo(gizmo_sys, type_id, NULL, &sys->forces[i]);
  }
}

// FUNCTION: LEGOBATMAN 0x004878b0
char *GizmoForce_GetGizmoName(GIZMO_s *gizmo) {
  return gizmo != NULL ? ((GIZFORCE_s *)gizmo->object)->name : NULL;
}

// FUNCTION: LEGOBATMAN 0x004879f0
char *GizmoForce_GetOutputName(GIZMO_s *gizmo, i32 output) {
  switch (output) {
  case 0:
    return "AtEnd";
  case 1:
    return "NotAtStart";
  case 2:
    return "AtStart";
  case 3:
    return "StackComplete";
  case 4:
    return "StackCompleteInOrder";
  case 5:
    return "Destroyed/Thrown";
  case 6:
    return "Complete";
  case 7:
    return "BeingUsed";
  }
  return NULL;
}

// FUNCTION: LEGOBATMAN 0x00487a70
i32 GizmoForce_GetNumOutputs(GIZMO_s *gizmo) { return 8; }

// FUNCTION: LEGOBATMAN 0x00487a80
void *GizForces_AllocateProgressData(VARIPTR *buf, VARIPTR *buf_end) {
  return GameBufferAllocProgressUnk005bbaf0(buf, buf_end,
                                            sizeof(GIZFORCEPROGRESS));
}

// FUNCTION: LEGOBATMAN 0x00487aa0
void GizForces_ClearProgress(void *world, void *progress_ptr) {
  GIZFORCEPROGRESS *progress = (GIZFORCEPROGRESS *)progress_ptr;
  if (progress != NULL) {
    memset(progress->a0, 0xff, sizeof(progress->a0));
    memset(progress->a10, 0xff, sizeof(progress->a10));
    memset(progress->a20, 0, sizeof(progress->a20));
    memset(progress->a30, 0, sizeof(progress->a30));
    memset(progress->a40, 0, sizeof(progress->a40));
    memset(progress->a50, 0, sizeof(progress->a50));
    memset(progress->a60, 0, sizeof(progress->a60));
    memset(progress->a70, -1, sizeof(progress->a70));
  }
}

// GLOBAL: LEGOBATMAN 0x0093e144
i32 force_gizmotype_id = -1;

// FUNCTION: LEGOBATMAN 0x00488710
GIZFORCE_s *GizForces_FindForce(WORLDINFO_s *world, char *name) {
  GIZMO_s *gizmo =
      GizmoFindByName(world->gizmoSys2b0c, force_gizmotype_id, name);
  if (gizmo != NULL)
    return (GIZFORCE_s *)gizmo->object;
  return NULL;
}

// FUNCTION: LEGOBATMAN 0x00488750
u32 GizForce_TotalScore(void *world) {
  u32 total = 0;
  GIZFORCESYS_s *sys = ((WORLDINFO_s *)world)->giz_force_sys;
  if (sys != NULL) {
    GIZFORCE_s *force = sys->forces;
    if (force != NULL) {
      for (i32 i = 0; i < sys->count; i++, force++)
        total += force->score;
    }
  }
  return total;
}

i32 GizForces_Load(void *world, void *sys);
void GizForces_AddLevelSfx(void *world, void *sys, i32 *sfx_ids, i32 *sfx_count,
                           i32 max_sfx);
void GizForces_Update(void *world, void *sys, f32 dt);
void GizForces_Draw(void *world, void *sys, f32 dt);
i32 GizmoForce_GetOutput(GIZMO_s *gizmo, i32 output, i32 b);
void GizmoForce_Activate(GIZMO_s *gizmo, i32 active);
i32 GizmoForce_ActivateRev(GIZMO_s *gizmo, i32 value, i32 query);
void GizForces_BoltHitPlat(void);
void GizForces_GetBestBoltTarget(void);
void GizForces_BoltHit(void);
void GizForces_StoreProgress(void *world, void *sys, void *progress);
void GizForces_Reset(void *world, void *sys, void *progress);

// FUNCTION: LEGOBATMAN 0x0048a620
ADDGIZMOTYPE *GizForce_RegisterGizmo(i32 type_id) {
  // GLOBAL: LEGOBATMAN 0x0093e250
  static char *name = "GizForce";
  // GLOBAL: LEGOBATMAN 0x009c8e90
  static ADDGIZMOTYPE addtype;

  addtype = Default_ADDGIZMOTYPE;
  addtype.name = name;
  addtype.prefix = "";
  addtype.progress_size = sizeof(GIZFORCEPROGRESS);
  addtype.fns[0] = (void *)GizForces_GetMaxGizmos;
  addtype.fns[1] = (void *)GizForces_AddGizmos;
  addtype.fns[2] = NULL;
  addtype.fns[3] = (void *)GizForces_Update;
  addtype.fns[4] = (void *)GizForces_Draw;
  addtype.fns[5] = NULL;
  addtype.fns[6] = (void *)GizmoForce_GetGizmoName;
  addtype.fns[7] = (void *)GizmoForce_GetOutput;
  addtype.fns[8] = (void *)GizmoForce_GetOutputName;
  addtype.fns[9] = (void *)GizmoForce_GetNumOutputs;
  addtype.fns[10] = (void *)GizmoForce_Activate;
  addtype.fns[11] = (void *)GizmoForce_ActivateRev;
  addtype.fns[12] = (void *)GizmoForce_SetVisibility;
  addtype.fns[13] = NULL;
  addtype.fns[14] = (void *)GizmoForce_GetPos;
  addtype.fns[15] = NULL;
  addtype.fns[16] = (void *)GizForces_BoltHitPlat;
  addtype.fns[17] = (void *)GizForces_GetBestBoltTarget;
  addtype.fns[18] = (void *)GizForces_BoltHit;
  addtype.fns[19] = (void *)GizForces_AllocateProgressData;
  addtype.fns[20] = (void *)GizForces_ClearProgress;
  addtype.fns[21] = (void *)GizForces_StoreProgress;
  addtype.fns[22] = (void *)GizForces_Reset;
  addtype.fns[23] = (void *)GizForces_ReserveBufferSpace;
  addtype.fns[24] = (void *)GizForces_Load;
  addtype.fns[25] = NULL;
  addtype.fns[26] = (void *)GizForces_PostLoad;
  addtype.fns[27] = (void *)GizForces_AddLevelSfx;
  force_gizmotype_id = type_id;
  return &addtype;
}
