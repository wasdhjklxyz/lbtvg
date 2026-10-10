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
};

struct GAMEANIMSET_s {
  u8 pad0[0x24];
  GAMEANIMOBJ_s *objects; // 0x24
};

struct GIZFORCE_s {
  u8 pad0[0x28];
  GAMEANIMSET_s *anim_set; // 0x28
  u8 pad2c[0x82 - 0x2c];
  i16 sfx_process;  // 0x82
  i16 sfx_complete; // 0x84
  i16 sfx_return;   // 0x86
  u8 pad88[0xa4 - 0x88];
};

struct GIZFORCESYS_s {
  GIZFORCE_s *forces; // 0x00
  u8 pad4[0xe - 4];
  u16 count; // 0x0e
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
