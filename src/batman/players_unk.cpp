// batman/, file unknown: player activation (0x0043d430); saga keeps these in
// legoapi/characters/core/players.cpp.

#include "../gameapi/ai/aisys_unk.h"
#include "worldinfo_unk.h"

// GLOBAL: LEGOBATMAN 0x00a95fe0
extern f32 FRAMETIME;

void Player_ClearContext(GameObject_s *obj, i32 a);
void Player_ResetContexts(GameObject_s *obj);
void NewBuzz(nupad_s *pad, f32 duration, i32 mode);

// FUNCTION: LEGOBATMAN 0x0043d430
i32 DeactivatePlayer(GameObject_s *obj, f32 time, GameObject_s *by) {
  f32 saved;
  AISCRIPTPROCESS_s *process;
  if (obj->b9db != 0x17 || obj->f98c < time) {
    saved = obj->f11cc;
    Player_ClearContext(obj, 1);
    if (obj->b9db != 0x3e) {
      Player_ResetContexts(obj);
      obj->f11cc = saved;
      process = (AISCRIPTPROCESS_s *)obj->process290;
      obj->b9db = 0x17;
      if (AIScriptSetBaseScriptStateByName(process, "BeenDeactivated"))
        AIScriptProcess(g_unk00960894->aiSys2bf8, obj, process, process,
                        FRAMETIME);
      if (obj->p50->p0c->p204 && !(obj->p50->p08->p204->flags4 & 2))
        obj->s9d0 = 0x81;
      else if (obj->p50->p0c->p104)
        obj->s9d0 = 0x41;
      else
        obj->s9d0 = obj->s162c;
      obj->f98c = time;
      obj->f998 = 0.0f;
      obj->b9df = 0;
      obj->f988 = 0.0f;
      NewBuzz(obj->p112c->pad0, 0.1f, 0);
      return 1;
    }
  }
  return 0;
}

// GLOBAL: LEGOBATMAN 0x00ab3960
extern GameObject_s *Player[8];

typedef struct MISSIONSYS_s MISSIONSYS;
typedef struct MISSIONDATA_s MISSIONDATA;
MISSIONDATA *Mission_Active(MISSIONSYS *ms);

// GLOBAL: LEGOBATMAN 0x00ab0894
extern i32 FreePlay;
// GLOBAL: LEGOBATMAN 0x00ab084c
extern i32 ChallengeMode;
// GLOBAL: LEGOBATMAN 0x00ad1178
extern i32 Arcade;

char EdFileReadChar();

void EdFileRead(void *buf, i32 len);

// from saga legoapi/gizmo/base/gizmo.cpp
// FUNCTION: LEGOBATMAN 0x005bba90
i32 GizmoFileReadName(char *name) {
  i32 name_length = EdFileReadChar();
  if (name_length != 0) {
    EdFileRead(name, name_length);
    return 1;
  }
  return 0;
}

typedef struct GIZMOTYPE_s {
  char name[0x44];                         // 0x00
  char *(*get_gizmo_name_fn)(void *gizmo); // 0x44
  u32 pad48[(0xa0 - 0x48) / 4];
} GIZMOTYPE;

typedef struct GIZMOTYPES_s {
  i32 count;        // 0x00
  i32 unk4;         // 0x04
  GIZMOTYPE *types; // 0x08
} GIZMOTYPES;

// GLOBAL: LEGOBATMAN 0x00ab07f4
extern GIZMOTYPES *gizmotypes;

i32 NuStrCmp(const char *a, const char *b);

// FUNCTION: LEGOBATMAN 0x005bc370
i32 GizmoGetTypeIDByName(GIZMOSYS_s *gizmo_sys, char *name) {
  if (gizmotypes != 0 && gizmo_sys != 0 && name != 0) {
    for (i32 i = 0; i < gizmotypes->count; i++) {
      if (NuStrCmp(gizmotypes->types[i].name, name) == 0) {
        return i;
      }
    }
  }

  return -1;
}

typedef struct GIZMO_s {
  u32 data[2];
} GIZMO;

typedef struct GIZMOSET_s {
  u32 pad0;
  i32 count; // 0x04
  u32 pad8;
  GIZMO *gizmos; // 0x0c
  u32 pad10;
} GIZMOSET;

struct GIZMOSYS_s {
  GIZMOSET *sets; // 0x00
};

int NuStrICmp(const char *a, const char *b);

// FUNCTION: LEGOBATMAN 0x005bc570
GIZMO *GizmoFindByName(GIZMOSYS_s *gizmo_sys, i32 type_id, char *name) {
  if (gizmotypes != 0 && gizmo_sys != 0 && name != 0) {
    if (type_id >= 0 && type_id <= gizmotypes->count) {
      GIZMOTYPE &type = gizmotypes->types[type_id];
      GIZMOSET &set = gizmo_sys->sets[type_id];
      if (type.get_gizmo_name_fn != 0) {
        GIZMO *gizmo = set.gizmos;
        for (i32 i = 0; i < set.count; ++i, ++gizmo) {
          char *gizmo_name = type.get_gizmo_name_fn(gizmo);
          if (NuStrICmp(gizmo_name, name) == 0) {
            return gizmo;
          }
        }
      }
    } else {
      GIZMOTYPE *type = gizmotypes->types;
      GIZMOSET *set = gizmo_sys->sets;
      for (i32 type_index = 0; type_index < gizmotypes->count;
           ++type_index, ++type, ++set) {
        if (type->get_gizmo_name_fn != 0) {
          GIZMO *gizmo = set->gizmos;
          for (i32 gizmo_index = 0; gizmo_index < set->count;
               ++gizmo_index, ++gizmo) {
            char *gizmo_name = type->get_gizmo_name_fn(gizmo);
            if (NuStrICmp(gizmo_name, name) == 0) {
              return gizmo;
            }
          }
        }
      }
    }
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x005bdcd0
i32 InStory() {
  if (FreePlay != 0 || ChallengeMode != 0 || Mission_Active(0) != 0 ||
      Arcade != 0) {
    return 0;
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x005c0f70
void SetFlicker(GameObject_s *object, float duration) {
  object->flicker_time = duration;
  object->flags1310 &= ~0x380000;
}

// FUNCTION: LEGOBATMAN 0x005c1860
GameObject_s *Player_FindByID(i32 id) {
  for (i32 i = 0; i < 8; i++) {
    GameObject_s *object = Player[i];
    if (object != 0 && (object->flags1fc & 1) && (object->flags1fc & 0x1000) &&
        object->type15b0 == id)
      return object;
  }
  return 0;
}

typedef struct PLAYERSTARTENTRY_s {
  nuvec_s *pos;
  u32 pad4[3];
} PLAYERSTARTENTRY;

// GLOBAL: LEGOBATMAN 0x00ab3710
extern PLAYERSTARTENTRY PlayerStart[8];

// FUNCTION: LEGOBATMAN 0x005c18b0
nuvec_s *Player_StartPos(GameObject_s *obj) {
  i32 index;
  if (obj->b24c >= 0 && obj->b24c < 8) {
    index = obj->b24c;
  } else {
    index = obj->b259;
  }
  index %= 8;
  return PlayerStart[index].pos != 0 ? PlayerStart[index].pos
                                     : PlayerStart[0].pos;
}

void Unk005cc1a0(GameObject_s *object, i32 a);
void Unk00659fb0(GameObject_s *object);
void Unk00606d20(GameObject_s *object);
void Unk00645490(GameObject_s *object);
void Unk0062fc50(GameObject_s *object);
void Unk0066d9c0(GameObject_s *object);
void Unk005f1f50(GameObject_s *object);
void Unk005ba400(GameObject_s *object);
void Unk005fed80(GameObject_s *object);

// GLOBAL: LEGOBATMAN 0x00ab39a8
extern void (*Player_ClearContextFn)(GameObject_s *object, i32 mode);

// FUNCTION: LEGOBATMAN 0x005c1b10
void Player_ClearContext(GameObject_s *object, i32 mode) {
  Unk005cc1a0(object, 0);
  Unk00659fb0(object);
  if (Player_ClearContextFn != 0)
    Player_ClearContextFn(object, mode);
  Unk00606d20(object);
  Unk00645490(object);
  Unk0062fc50(object);
  object->flags1310 &= ~0x6000;
  Unk0066d9c0(object);
  Unk005f1f50(object);
  Unk005ba400(object);
  Unk005fed80(object);
}

// GLOBAL: LEGOBATMAN 0x009604c0
extern i32 g_unk009604c0;

i32 qrand(void);
void ReleaseTakeOver(GameObject_s *object, i32 unk);

// FUNCTION: LEGOBATMAN 0x005c1b80
void Player_ResetContexts(GameObject_s *object) {
  object->f9e8 = 0.0f;
  object->b9ec = -1;
  if (object->flags130c & 0x80000)
    object->weapon_scale = 1.0f;
  else
    object->weapon_scale = 0.0f;
  object->weapon_scale_rate = 5.0f;
  object->f11cc = 0.0f;
  object->weapon_scale_state = 0;
  object->f11d4 = 0.0f;
  object->b9db = -1;
  object->f11c4 = 0.0f;
  object->b9e0 = 0;
  object->b9df = 0;
  object->p112c->flags5a &= ~4;
  object->p112c->flags5a &= ~0x10;
  object->flags130c &= ~0x400;
  object->i11bc = 0;
  object->i11b0 = 0;
  i32 r = qrand();
  object->f11f0 = 0.0f;
  object->f1204 = 0.0f;
  object->f1208 = 0.0f;
  object->f11d0 = 0.0f;
  object->b132b = r / 0x8000;
  object->f1228 = 0.0f;
  object->f123c = 0.0f;
  object->s12fc = -1;
  object->s1300 = -1;
  object->s1302 = -1;
  object->s9d2 = 0;
  object->b9d8 = 0;
  object->b131c = 0;
  if (object->b9db == g_unk009604c0)
    ReleaseTakeOver(object, 1);
  object->f1250 = 0.0f;
  object->f1264 = 0.0f;
  object->f1270 = 0.0f;
  object->f1274 = 0.0f;
}

// FUNCTION: LEGOBATMAN 0x005c2bb0
i32 Players_BothActive() {
  if (Player[0] != 0 && (Player[0]->flags1fc & 0x80) && Player[1] != 0 &&
      (Player[1]->flags1fc & 0x80)) {
    return 1;
  }
  return 0;
}

typedef struct CHEAT_s {
  u32 pad0[4];
} CHEAT_s;

// GLOBAL: LEGOBATMAN 0x00936f10
extern CHEAT_s g_unk00936f10[];
// GLOBAL: LEGOBATMAN 0x00960b08
extern i32 g_unk00960b08;
// GLOBAL: LEGOBATMAN 0x00960674
extern i32 g_unk00960674;
// GLOBAL: LEGOBATMAN 0x00960678
extern i32 g_unk00960678;

i32 Cheat_IsOn(CHEAT_s *cheat);

unsigned __int64 Cheats_CheckFlags(unsigned __int64 flags);

struct Unk_GameObject1144 {
  u8 pad0[0x14];
  u8 flags14; // 0x14
};

// GLOBAL: LEGOBATMAN 0x00960b10
extern i32 g_unk00960b10;

// FUNCTION: LEGOBATMAN 0x005c2be0
i32 Player_HasInvincibility(GameObject_s *object) {
  if (Cheats_CheckFlags(0x80) != 0)
    return 1;
  if (object != 0 && object->f1278 > 0.0f)
    return 1;
  return 0;
}

// FUNCTION: LEGOBATMAN 0x005c2c20
i32 Player_HasFastBuild(GameObject_s *player) {
  if (Cheats_CheckFlags(0x4000) != 0)
    return 1;
  if (player != 0 && player->f1278 > 0.0f)
    return 1;
  return (player->p54->p24->b150 >> 1) & 1;
}

// FUNCTION: LEGOBATMAN 0x005c2c70
i32 Player_HasDeflectBolts(GameObject_s *object) {
  if (Cheats_CheckFlags(0x80000) != 0)
    return 1;
  if (object->p1144 != 0 && (object->p1144->flags14 & 4) &&
      g_unk00960b10 != 0 && Cheat_IsOn(&g_unk00936f10[g_unk00960b10]))
    return 1;
  if (object->f1278 > 0.0f)
    return 1;
  return 0;
}

i32 Player_HasDoubleBoltDamage(GameObject_s *object) {
  if (Cheats_CheckFlags(2) != 0)
    return 1;
  if (object != 0 && object->f1278 > 0.0f)
    return 1;
  return 0;
}

typedef struct BOLT_s {
  u8 pad0[0xf0];
  unsigned __int64 flags; // 0xf0
} BOLT_s;

// FUNCTION: LEGOBATMAN 0x005c2d10
i32 Player_HasDoubleBoltDamage_FromBolt(BOLT_s *bolt) {
  i32 player;
  if (bolt->flags & 1)
    player = 0;
  else if (bolt->flags & 2)
    player = 1;
  else
    return 0;
  return Player_HasDoubleBoltDamage(Player[player]);
}

// FUNCTION: LEGOBATMAN 0x005c2d90
i32 Player_HasDoubleWeaponDamage(GameObject_s *object) {
  if (Cheats_CheckFlags(0x400) != 0)
    return 1;
  if (object != 0 && object->f1278 > 0.0f)
    return 1;
  return 0;
}

// FUNCTION: LEGOBATMAN 0x005c2dd0
i32 Player_HasFastDig(GameObject_s *object) {
  if (Cheats_CheckFlags(0x8000000) != 0)
    return 1;
  if (object != 0 && object->f1278 > 0.0f)
    return 1;
  return 0;
}

// FUNCTION: LEGOBATMAN 0x005c2e10
i32 Player_HasFastMech(GameObject_s *object) {
  if (Cheats_CheckFlags(0x10000000) != 0)
    return 1;
  if (object != 0 && object->f1278 > 0.0f)
    return 1;
  return 0;
}

float NuVecDistSqr(nuvec_s *a, nuvec_s *b, nuvec_s *d);

// FUNCTION: LEGOBATMAN 0x005c2e50
i32 ActivePlayerInRange(nuvec_s *position, float range_squared,
                        float *distance_squared) {
  f32 distance;
  i32 i;
  for (i = 0; i < 8; i++) {
    if (Player[i] != 0 && (Player[i]->flags1fc & 0x80) &&
        (distance = NuVecDistSqr(&Player[i]->v80, position, 0)) <
            range_squared) {
      if (distance_squared != 0)
        *distance_squared = distance;
      return 1;
    }
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x005c30e0
i32 GetMaxHitPoints(GameObject_s *obj) {
  if (obj->b24c != -1 && g_unk00960b08 != -1 &&
      Cheat_IsOn(&g_unk00936f10[g_unk00960b08]))
    return g_unk00960678;
  return g_unk00960674;
}

// FUNCTION: LEGOBATMAN 0x005c3120
void SetHitPoints(GameObject_s *obj, i32 hp) {
  obj->current_hp = (u8)hp;
  if ((i8)hp > (i32)(u32)obj->hitpoints) {
    obj->current_hp = obj->hitpoints;
  }
}

// FUNCTION: LEGOBATMAN 0x005c3150
void Players_UpdateHitPoints(void) {
  i32 maxhp;
  i32 i;
  if (g_unk00960b08 != -1 && Cheat_IsOn(&g_unk00936f10[g_unk00960b08]))
    maxhp = g_unk00960678;
  else
    maxhp = g_unk00960674;
  for (i = 0; i < 8; i++) {
    if (Player[i] != 0) {
      if (maxhp > Player[i]->hitpoints)
        Player[i]->current_hp =
            Player[i]->current_hp * maxhp / Player[i]->hitpoints;
      Player[i]->hitpoints = (u8)maxhp;
      if (Player[i]->current_hp > Player[i]->hitpoints)
        Player[i]->current_hp = Player[i]->hitpoints;
    }
  }
}

struct GAMEMESSAGE_s {
  u8 pad000[0x101];
  i8 player; // 0x101, 0/1 = that player's pad, else all players
};

// 0x60 bytes per player; only the pad pointer is evidenced.
struct Unk00a96388 {
  nupad_s *pad; // 0x00
  u8 pad04[0x60 - 4];
};

// GLOBAL: LEGOBATMAN 0x00a96388
extern Unk00a96388 g_unk00a96388[];

void NewRumbleAllPlayers(f32 strength, f32 duration, i32 frames, i32 flags);
void NewBuzzFrames(nupad_s *pad, i32 frames, i32 flags);

// FUNCTION: LEGOBATMAN 0x005c32c0
void LoseHP_EndDelay(GAMEMESSAGE_s *message) {
  i8 player = message->player;
  if (player != 0 && player != 1)
    NewRumbleAllPlayers(0.0f, 0.0f, 1, 0);
  else
    NewBuzzFrames(g_unk00a96388[player].pad, 1, 0);
}
