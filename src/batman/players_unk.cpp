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

struct CHARACTERDATA_s {
  u8 pad0[4];
  u32 model_flags; // 0x04
  u8 pad8[0x48 - 8];
};

// GLOBAL: LEGOBATMAN 0x00acb81c
extern CHARACTERDATA_s *CDataList;

struct Unk00a94740Entry {
  u8 pad0[0xc];
  i32 *abilities; // 0x0c
  u8 pad10[0x6c - 0x10];
};

struct Unk00a94740 {
  u8 pad0[0x18];
  Unk00a94740Entry *entries; // 0x18
  i16 *character_entry;      // 0x1c, -1 = none
};

// GLOBAL: LEGOBATMAN 0x00a94740
extern Unk00a94740 *g_unk00a94740;
// GLOBAL: LEGOBATMAN 0x00961a44
extern i16 g_unk00961a44;

// FUNCTION: LEGOBATMAN 0x005c1aa0
i32 CanPullLevers(i32 character_id) {
  u32 flags = CDataList[character_id].model_flags;
  if ((flags & 0x01000010) == 0x01000010)
    return 0;
  if (flags & 0x00040088)
    return 1;
  if (g_unk00961a44 != 0) {
    i16 entry = g_unk00a94740->character_entry[character_id];
    if (entry != -1 &&
        g_unk00a94740->entries[entry].abilities[g_unk00961a44] != 0)
      return 1;
  }
  return 0;
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

// GLOBAL: LEGOBATMAN 0x00ab39ac
extern void (*SetPlayerControlFn)(GameObject_s *object);
// GLOBAL: LEGOBATMAN 0x00ab399c
extern f32 g_unk00ab399c[2];

struct Flags1fc {
  u32 low : 7;
  u32 active : 1; // bit 7
  u32 high : 24;
};

// FUNCTION: LEGOBATMAN 0x005c1cc0
void SetPlayerControl(GameObject_s *object, i32 on) {
  ((Flags1fc *)&object->flags1fc)->active = on;
  if (SetPlayerControlFn != 0)
    SetPlayerControlFn(object);
  if ((object->flags1fc & 0x80) && (object->b24c == 0 || object->b24c == 1))
    g_unk00ab399c[object->b24c] = 0.0f;
}

struct GAMECAMERA_s {
  u8 pad0[0x1fc];
  u16 input_yaw; // 0x1fc
};

// GLOBAL: LEGOBATMAN 0x0095f624
extern GAMECAMERA_s *GameCam;

i32 RotDiff(u16 current, u16 target);

// STUB: LEGOBATMAN 0x005c1d20
// close: orig loads pad->input_angle into ecx before obj->u246 and adds the
// camera yaw from memory; ours loads the camera yaw first.
i32 MovingBackwards(GameObject_s *object) {
  Unk_GameObject112c *pad = object->p112c;
  if (pad->operator_data != 0 && pad->f28 != 0.0f) {
    i32 difference =
        RotDiff(pad->input_angle + GameCam->input_yaw, object->u246);
    if (difference < -0x4000 || difference > 0x4000)
      return 1;
  }
  return 0;
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

extern "C" void *NuSpecialGetMtl(nuhspecial_s *special, int index);

struct IDMTL_s {
  u8 pad0[0x74];
  u16 tid; // 0x74
};

struct VuVec;
struct numtl_s;

struct IDVEC_s {
  f32 x, y, z, w;
};

struct InteractiveDisplay {
  static void *GetFirstSpecialMaterial(nugscn_s *scene, char *name);
  void RenderRectangle(VuVec const &pos, f32 width, f32 height,
                       u32 const &colour, numtl_s *mtl, f32 depth) const;
  void RenderSquare(VuVec const &pos, f32 size, u32 const &colour, numtl_s *mtl,
                    f32 depth) const;
  i32 Unk005c3ed0(WORLDINFO_s *world);
  void UpdateWhiteNoise(f32 dt);
  void RenderWhiteNoise(f32 alpha) const;
  void UpdateInGameSurfaceData();

  // vtable 0x0085d75c, see src/batman/unk_004f6ca0.cpp
  virtual void InitializePerm(char *name, variptr_u *buf, variptr_u *end); // 0
  virtual void InitializeLevel(WORLDINFO_s *world);                        // 1
  virtual void ActivateLevel(WORLDINFO_s *world);                          // 2
  virtual void DumpLevel(WORLDINFO_s *world);                              // 3
  virtual i32 GetDoesLevelLoadRender() const;                              // 4
  virtual const char *GetClassNameA() const;                               // 5
  virtual i32 GetUsesStrobePattern() const;                                // 6
  virtual i32 GetUsesWhiteNoise() const;                                   // 7
  virtual i32 GetUsesInterlacePattern() const;                             // 8
  virtual i32 GetUsesOverlayTexture() const;                               // 9
  virtual void Update(f32 dt);                                             // 10
  virtual void Render();                                                   // 11
  virtual i32 ShouldUpdate() const;                                        // 12
  virtual i32 IsVisible() const;                                           // 13
  virtual i32 RenderWhenPaused() const;                                    // 14
  virtual i32 IsCameraTarget() const;                                      // 15
  virtual i32 IsInteractiveMode() const;                                   // 16
  virtual f32 GetDisplayAspectRatio() const;                               // 17
  virtual void LoadSettings(char *name);                                   // 18
  virtual f32 GetTextScaleMultiplier() const;                              // 19
  virtual void Vfn20();                                                    // 20
  virtual i32 IsCameraTransitioning() const;                               // 21

  void UpdateBackgroundAlpha(f32 dt);
  void Unk005c3f20(const IDVEC_s &v);
  static const char *Unk005c3f50();
  char *Unk005c3f60();
  IDMTL_s *Unk005c3f70() const;
  char *Unk005c3f80();
  u8 Unk005c3f90() const;
  void Unk005c3fd0(void *a, void *b);

  u8 pad4[0x10 - 4];
  char level_name[0x40];      // 0x010
  char transition_name[0x40]; // 0x050
  u8 pad090[0x100 - 0x90];
  IDVEC_s v100; // 0x100
  u8 pad110[0x200 - 0x110];
  IDVEC_s v200; // 0x200
  IDVEC_s v210; // 0x210
  f32 f220;     // 0x220
  i32 i224;     // 0x224
  i32 i228;     // 0x228
  u8 pad22c[0x234 - 0x22c];
  f32 f234;                  // 0x234
  f32 f238;                  // 0x238, strobe phase
  char texture_name_a[0x40]; // 0x23c
  char texture_name_b[0x40]; // 0x27c
  IDMTL_s *mtl_a;            // 0x2bc
  IDMTL_s *mtl_b;            // 0x2c0
  IDMTL_s *interlace_mtl;    // 0x2c4
  IDMTL_s *strobe_mtl;       // 0x2c8
  IDMTL_s *white_noise_mtl;  // 0x2cc
  u16 noise[50];             // 0x2d0
  f32 f334;                  // 0x334
  f32 f338;                  // 0x338
  i32 i33c;                  // 0x33c
};

// FUNCTION: LEGOBATMAN 0x005c4080
void *InteractiveDisplay::GetFirstSpecialMaterial(nugscn_s *scene, char *name) {
  nuhspecial_s special;
  NuSpecialFind(scene, &special, name, 0);
  return NuSpecialGetMtl(&special, 0);
}

class DynamicTextureManager {
public:
  u16 Unk0067aae0(char *name, WORLDINFO_s *world);
};

// GLOBAL: LEGOBATMAN 0x00ad2d78
extern DynamicTextureManager g_unk00ad2d78;

void NuMtlUpdate(IDMTL_s *mtl);

// FUNCTION: LEGOBATMAN 0x005c3f20
void InteractiveDisplay::Unk005c3f20(const IDVEC_s &v) {
  v100.x = v.x;
  v100.y = v.y;
  v100.z = v.z;
  v100.w = v.w;
}

// FUNCTION: LEGOBATMAN 0x005c3f50
const char *InteractiveDisplay::Unk005c3f50() {
  return "stuff/interactivedisplay/common/interlacepattern";
}

// FUNCTION: LEGOBATMAN 0x005c3f60
char *InteractiveDisplay::Unk005c3f60() { return texture_name_a; }

// FUNCTION: LEGOBATMAN 0x005c3f70
IDMTL_s *InteractiveDisplay::Unk005c3f70() const { return mtl_a; }

// FUNCTION: LEGOBATMAN 0x005c3f80
char *InteractiveDisplay::Unk005c3f80() { return texture_name_b; }

// FUNCTION: LEGOBATMAN 0x005c3f90
u8 InteractiveDisplay::Unk005c3f90() const { return (u8)(f234 * 128.0f); }

void Unk00716660(void *a, void *b, i32 c, i32 d);

// FUNCTION: LEGOBATMAN 0x005c3fd0
void InteractiveDisplay::Unk005c3fd0(void *a, void *b) {
  Unk00716660(a, b, 1, 0);
}

// FUNCTION: LEGOBATMAN 0x005c3ff0
void InteractiveDisplay::DumpLevel(WORLDINFO_s *world) {
  if (NuStrICmp((char *)world, level_name) == 0) {
    if (GetUsesInterlacePattern() && interlace_mtl != 0) {
      interlace_mtl->tid = 0;
      NuMtlUpdate(interlace_mtl);
    }
    if (mtl_a != 0) {
      mtl_a->tid = 0;
      NuMtlUpdate(mtl_a);
    }
    if (mtl_b != 0) {
      mtl_b->tid = 0;
      NuMtlUpdate(mtl_b);
    }
    i228 = 0;
  }
}

extern WORLDINFO_s *WORLD;

float NuRandFloat(void);
unsigned int NuRandInt(void);

// FUNCTION: LEGOBATMAN 0x005c65f0
void InteractiveDisplay::UpdateWhiteNoise(f32 dt) {
  if (GetUsesWhiteNoise()) {
    for (i32 i = 0; i < 50; i++) {
      noise[i] = 0;
      if (!(f334 < 1.0f && NuRandFloat() > f334))
        noise[i] = (i32)(NuRandInt() >> 25) * 64 / 128 + 64;
    }
  }
}

// FUNCTION: LEGOBATMAN 0x005c6de0
void InteractiveDisplay::UpdateBackgroundAlpha(f32 dt) { f234 = 1.0f; }

// FUNCTION: LEGOBATMAN 0x005c64e0
i32 InteractiveDisplay::IsVisible() const {
  if (WORLD == 0)
    return 0;
  if (NuStrICmp(level_name, (char *)WORLD) != 0)
    return 0;
  if (i228 == 0)
    return 0;
  if (i224 == 0)
    return 0;
  if ((v200.x < -1.0f && v210.x < -1.0f) || (v200.x > 1.0f && v210.x > 1.0f) ||
      (v200.y < -1.0f && v210.y < -1.0f) || (v200.y > 1.0f && v210.y > 1.0f) ||
      (v200.z < 0.0f && v210.z < 0.0f) || (v200.z > 1.0f && v210.z > 1.0f))
    return 0;
  return 1;
}

struct VUFNT;
VUFNT *NuQFntDuplicate(VUFNT *font, i32 flags, i32 render_plane, variptr_u *buf,
                       variptr_u *buf_end);

// GLOBAL: LEGOBATMAN 0x00ab39cc
extern VUFNT *InteractiveDisplayQFont3DZ;
// GLOBAL: LEGOBATMAN 0x00ad7434
extern VUFNT *g_unk00ad7434;

class DynamicMaterialManager {
public:
  void *GetMaterial(char const *a, char const *b, int c);
};

// GLOBAL: LEGOBATMAN 0x00ad2af8
extern DynamicMaterialManager g_dynamicMaterialManager;

// FUNCTION: LEGOBATMAN 0x005c71d0
void InteractiveDisplay::InitializePerm(char *name, variptr_u *buf,
                                        variptr_u *end) {
  if (InteractiveDisplayQFont3DZ == 0 && g_unk00ad7434 != 0)
    InteractiveDisplayQFont3DZ =
        NuQFntDuplicate(g_unk00ad7434, 0x4c, 2, buf, end);
  if (GetUsesWhiteNoise())
    white_noise_mtl = (IDMTL_s *)g_dynamicMaterialManager.GetMaterial(0, 0, 4);
  if (GetUsesStrobePattern())
    strobe_mtl = (IDMTL_s *)g_dynamicMaterialManager.GetMaterial(0, 0, 6);
  if (GetUsesInterlacePattern())
    interlace_mtl = (IDMTL_s *)g_dynamicMaterialManager.GetMaterial(
        "stuff/interactivedisplay/common/interlacepattern", level_name, 5);
  mtl_a = (IDMTL_s *)g_dynamicMaterialManager.GetMaterial(texture_name_a,
                                                          level_name, 1);
  if (GetUsesOverlayTexture())
    mtl_b = (IDMTL_s *)g_dynamicMaterialManager.GetMaterial(texture_name_b,
                                                            level_name, 7);
  f238 = 0.0f;
  i224 = 0;
  i228 = 0;
  f338 = 1000.0f;
  i33c = 0;
  f220 = 1.0f;
  f334 = 1.0f;
}

// FUNCTION: LEGOBATMAN 0x005c7300
void InteractiveDisplay::InitializeLevel(WORLDINFO_s *world) {
  if (NuStrICmp((char *)world, level_name) == 0) {
    if (GetUsesInterlacePattern() && interlace_mtl != 0) {
      interlace_mtl->tid = g_unk00ad2d78.Unk0067aae0(
          "stuff/interactivedisplay/common/interlacepattern", world);
      NuMtlUpdate(interlace_mtl);
    }
    if (mtl_a != 0) {
      mtl_a->tid = g_unk00ad2d78.Unk0067aae0(texture_name_a, world);
      NuMtlUpdate(mtl_a);
    }
    if (mtl_b != 0) {
      mtl_b->tid = g_unk00ad2d78.Unk0067aae0(texture_name_b, world);
      NuMtlUpdate(mtl_b);
    }
    i224 = 0;
    i228 = Unk005c3ed0(world);
    f338 = 1000.0f;
    i33c = 0;
    f334 = 1.0f;
  }
}

// FUNCTION: LEGOBATMAN 0x005c7480
void InteractiveDisplay::Render() { RenderWhiteNoise(1.0f); }

// FUNCTION: LEGOBATMAN 0x005c7fa0
void InteractiveDisplay::Update(f32 dt) {
  UpdateInGameSurfaceData();
  f234 = 1.0f;
  UpdateWhiteNoise(dt);
  if (GetUsesStrobePattern()) {
    f238 += dt * 0.2f;
    f238 = f238 > 0.2f ? 0.0f : f238;
  }
  if (IsInteractiveMode() != i33c) {
    f338 = 0.0f;
    i33c = IsInteractiveMode();
  } else {
    f338 += dt;
  }
}

// Mac order: RenderRectangle(VuVec const&, float, float, ...), RenderSquare.
// FUNCTION: LEGOBATMAN 0x005c4c30
void InteractiveDisplay::RenderSquare(VuVec const &pos, f32 size,
                                      u32 const &colour, numtl_s *mtl,
                                      f32 depth) const {
  RenderRectangle(pos, size, size, colour, mtl, depth);
}
