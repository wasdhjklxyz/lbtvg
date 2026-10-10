// gameapi/doors_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nu2api/nucore/common.h"
#include "../nu2api/nucore/nustring.h"
#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"
#include "../nu2api/numath/nuvec.h"
#include <stddef.h>

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x00614760
static f32 NuFabs(f32 f);
// FUNCTION: LEGOBATMAN 0x00614780
static f32 NuSinApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x00614820
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);
// FUNCTION: LEGOBATMAN 0x00616280
static f32 NuFdiv(f32 a, f32 b);
// FUNCTION: LEGOBATMAN 0x00616360
static f32 NuCosApprox(i32 angle);

typedef struct DOORSPLINE_s {
  i16 length; // 0x00
  u16 pad2;
  u32 pad4;
  nuvec_s *pts; // 0x08
} DOORSPLINE_s;

typedef struct DOOR_s {
  char name[0x80];               // 0x00
  char camera_spline_name[0x20]; // 0x80
  DOORSPLINE_s *spline;          // 0xa0
  u32 pada4[(0xd4 - 0xa4) / 4];
  nuvec_s pos;                 // 0xd4
  f32 radius;                  // 0xe0
  nuvec_s normal;              // 0xe4
  i16 level;                   // 0xf0
  i16 level_f2;                // 0xf2
  i16 freeplay_level;          // 0xf4
  u8 teleport_in;              // 0xf6, 0 = "air"
  u8 teleport_out;             // 0xf7, 0 = "air"
  u8 next_sock;                // 0xf8
  u8 flags;                    // 0xf9
  u8 vehicle;                  // 0xfa
  u8 active;                   // 0xfb
  DOORSPLINE_s *camera_spline; // 0xfc
  f32 camera_wait;             // 0x100
  f32 camera_blend_time;       // 0x104
  u64 takeover_character_mask; // 0x108
  u32 pad110[(0x120 - 0x110) / 4];
  void *cutscene; // 0x120
  u32 pad124;
} DOOR_s;

typedef struct WORLDINFO_s {
  u8 pad0[0x104];
  VARIPTR buf104; // 0x104
  u8 pad108[0x120 - 0x108];
  i32 level_idx; // 0x120
  u8 pad124[0x12c - 0x124];
  struct LEVELDATA_s *current_level; // 0x12c
  u8 pad130[0x140 - 0x130];
  struct nugscn_s *scn140; // 0x140
  u8 pad144[0x2af0 - 0x144];
  void *cutscene_sys; // 0x2af0
  u8 pad2af4[0x2b0c - 0x2af4];
  struct GIZMOSYS_s *gizmo_sys; // 0x2b0c
  u8 pad2b10[0x47a8 - 0x2b10];
  DOOR_s *doors;  // 0x47a8
  i32 door_count; // 0x47ac
} WORLDINFO_s;

// GLOBAL: LEGOBATMAN 0x00acb000
char Door_ExitName[64];

// GLOBAL: LEGOBATMAN 0x00acb060
i32 Door_Start;

// GLOBAL: LEGOBATMAN 0x00963654
i32 Door_NextSock = -1;

typedef struct nufpar_s {
  u32 pad0[0x910 / 4];
  char *word_buf; // 0x910
} NUFPAR;

NUFPAR *NuFParCreateMem(char *name, char *buffer, i32 bufferSize);
void NuFParPushCom(NUFPAR *parser, void *commands);
i32 NuFParGetLine(NUFPAR *parser);
i32 NuFParGetWord(NUFPAR *parser);
i32 NuFParInterpretWord(NUFPAR *parser);
void NuFParDestroy(NUFPAR *parser);

// GLOBAL: LEGOBATMAN 0x00963658
extern u8 Door_ConfigKeywords[];
// GLOBAL: LEGOBATMAN 0x00acaffc
extern WORLDINFO_s *D_worldinfo;
// GLOBAL: LEGOBATMAN 0x00acafe0
static DOOR_s *D_door;
// GLOBAL: LEGOBATMAN 0x00ab056c
extern nuvec_s v000;
// GLOBAL: LEGOBATMAN 0x0095fd2c
extern nuvec_s v001;

// STUB: LEGOBATMAN 0x00614d10
// close: orig keeps parser in ebp and in_door in the dead `config` slot;
// ours swaps them. Control flow and stores line up.
void Doors_Configure(WORLDINFO_s *world, char *config) {
  world->doors = 0;
  if (world->scn140 == 0)
    return;

  NUFPAR *parser = NuFParCreateMem("doors", config, 0xffff);
  if (parser == 0)
    return;

  world->buf104.addr = (world->buf104.addr + 15) & ~15;
  DOOR_s *door = (DOOR_s *)world->buf104.addr;
  world->doors = door;
  NuFParPushCom(parser, Door_ConfigKeywords);

  i32 in_door = 0;
  while (NuFParGetLine(parser) != 0) {
    if (NuFParGetWord(parser) == 0)
      continue;
    if (in_door) {
      if (NuStrICmp(parser->word_buf, "door_end") == 0) {
        in_door = 0;
        if (door->spline != 0 && door->level != -1) {
          if (door->freeplay_level == -1)
            door->freeplay_level = door->level;
          door++;
          world->door_count++;
        }
      } else {
        NuFParInterpretWord(parser);
      }
    } else if (NuStrICmp(parser->word_buf, "door_start") == 0) {
      door->name[0] = '\0';
      door->camera_spline_name[0] = '\0';
      door->spline = 0;
      door->pos = v000;
      door->radius = 1.0f;
      door->normal = v001;
      door->camera_wait = 0.0f;
      door->camera_blend_time = 1.0f;
      in_door = 1;
      D_worldinfo = world;
      D_door = door;
      door->level = -1;
      door->level_f2 = -1;
      door->freeplay_level = -1;
      door->next_sock = 0xff;
      door->flags = 0;
      door->vehicle = 0xff;
      door->active = 0;
      door->camera_spline = 0;
      door->takeover_character_mask = 0;
    }
  }

  NuFParDestroy(parser);
  if (world->door_count > 0)
    world->buf104.addr = ((u32)door + 15) & ~15;
  else
    world->doors = 0;
}

// name is a Mac pairing hint (order): verify
// from saga legoapi/props/doors/doors.cpp
// FUNCTION: LEGOBATMAN 0x00614f20
void Door_Reset() {
  Door_ExitName[0] = '\0';
  Door_Start = 0;
  Door_NextSock = -1;
}

// FUNCTION: LEGOBATMAN 0x00615190
DOOR_s *Door_FindByName(WORLDINFO_s *world, char *name) {
  DOOR_s *door = world->doors;
  if (door != 0) {
    for (i32 i = 0; i < world->door_count; i++, door++) {
      if (NuStrICmp(name, door->name) == 0) {
        return door;
      }
    }
  }
  return 0;
}

// GLOBAL: LEGOBATMAN 0x00acb068
extern i32 Door_UseCutCam;
// GLOBAL: LEGOBATMAN 0x00960894
extern WORLDINFO_s *WORLD;
// GLOBAL: LEGOBATMAN 0x00acb040
extern char Door_ExitCameraSplineName[];
// GLOBAL: LEGOBATMAN 0x00acafd4
extern nuvec_s Door_CutCamPos0;
// GLOBAL: LEGOBATMAN 0x00acafe8
extern nuvec_s Door_CutCamPos1;
// GLOBAL: LEGOBATMAN 0x00acafc8
extern f32 Door_CutCamWaitTime;
// GLOBAL: LEGOBATMAN 0x00acafe4
extern f32 Door_CutCamWait;
// GLOBAL: LEGOBATMAN 0x00acaff4
extern f32 Door_CutCamBlendTime;
// GLOBAL: LEGOBATMAN 0x00acb06c
extern i32 Door_CutLookAtPlayers;

// FUNCTION: LEGOBATMAN 0x006151e0
void Door_SetCutCam(DOOR_s *door) {
  Door_UseCutCam = 0;
  if (door->camera_spline != 0) {
    Door_UseCutCam = 1;
    Door_CutCamPos0 = door->camera_spline->pts[0];
    Door_CutCamPos1 = door->camera_spline->pts[1];
  } else {
    if (door->camera_spline_name[0] == '\0' ||
        door->level == WORLD->level_idx) {
      return;
    }
    Door_UseCutCam = 1;
    NuStrCpy(Door_ExitCameraSplineName, door->camera_spline_name);
  }
  Door_CutCamWait = Door_CutCamWaitTime = door->camera_wait;
  Door_CutCamBlendTime = door->camera_blend_time;
  Door_CutLookAtPlayers = door->flags & 2;
}

typedef struct PLAYERSTARTENTRY_s {
  nuvec_s *pos; // 0x00
  u32 pad4[2];
  i16 angle; // 0x0c
  u16 pade;
} PLAYERSTARTENTRY;

// GLOBAL: LEGOBATMAN 0x00ab3710
extern PLAYERSTARTENTRY PlayerStart[8];

void NuVecSub(nuvec_s *out, nuvec_s *a, nuvec_s *b);
i32 NuAtan2D(f32 dx, f32 dy);

typedef struct LEVELDATA_s {
  u8 pad00[0x64];
  u32 flags; // 0x64
  u8 pad68[0xab - 0x68];
  i8 area_index; // 0xab
  u8 padac[0xd8 - 0xac];
  i8 area_level_index; // 0xd8
  u8 padd9[0x113 - 0xd9];
  u8 max_doors; // 0x113
  u8 pad114[0x150 - 0x114];
} LEVELDATA;

// GLOBAL: LEGOBATMAN 0x00aca894
extern LEVELDATA *LDataList;

// 0x2e90 bytes per entry; only the completion flag byte is evidenced.
struct Unk009ca958 {
  u8 pad0000[0x2800];
  u8 flags2800; // 0x2800, bit 0 = completed
  u8 pad2801[0x2e90 - 0x2801];
};

// GLOBAL: LEGOBATMAN 0x009ca958
extern Unk009ca958 *g_unk009ca958;

i32 InStory(void);

// FUNCTION: LEGOBATMAN 0x006152f0
i32 Door_DestinationLevel(DOOR_s *door) {
  if (InStory() == 0 && door->freeplay_level != -1)
    return door->freeplay_level;
  if (door->level_f2 != -1) {
    if (LDataList[door->level].area_index != -1 &&
        LDataList[door->level_f2].area_index ==
            LDataList[door->level].area_index &&
        LDataList[door->level].area_level_index != -1 &&
        (g_unk009ca958[LDataList[door->level].area_level_index].flags2800 & 1))
      return door->level_f2;
  }
  return door->level;
}

// FUNCTION: LEGOBATMAN 0x00615390
i32 StartDoorPositions(void) {
  Door_Start = 0;
  if (Door_ExitName[0] != '\0') {
    DOOR_s *door = WORLD->doors;
    if (door != 0) {
      for (i32 i = 0; i < WORLD->door_count; i++, door++) {
        if (door->spline != 0 && NuStrICmp(door->name, Door_ExitName) == 0) {
          for (i32 i = 0; i < 8; i++) {
            i32 k = i * 2 + 4;
            if (door->spline->length > k + 1) {
              nuvec_s tmp;
              PlayerStart[i].pos = &door->spline->pts[k];
              NuVecSub(&tmp, &door->spline->pts[k + 1], PlayerStart[i].pos);
              PlayerStart[i].angle = NuAtan2D(tmp.x, tmp.z);
            } else {
              PlayerStart[i].pos = PlayerStart[i - 1].pos;
              PlayerStart[i].angle = PlayerStart[i - 1].angle;
            }
          }
          Door_Start = 1;
          return 1;
        }
      }
    }
    Door_ExitName[0] = '\0';
  }
  return 0;
}

i32 NuFParGetInt(NUFPAR *parser);
f32 NuFParGetFloat(NUFPAR *parser);

LEVELDATA *Level_FindByName(char *name, i32 *idx_out);

DOORSPLINE_s *NuSplineFind(struct nugscn_s *scene, char *name);

// FUNCTION: LEGOBATMAN 0x00614840
void D_spline(NUFPAR *parser) {
  if (NuFParGetWord(parser) != 0 && NuStrLen(parser->word_buf) < 64) {
    NuStrCpy(D_door->name, parser->word_buf);
    D_door->spline = NuSplineFind(D_worldinfo->scn140, D_door->name);
    if (D_door->spline == 0 || D_door->spline->length < 4) {
      D_door->spline = 0;
    } else {
      i32 i;
      for (i = 0; i < D_worldinfo->door_count; i++) {
        if (D_worldinfo->doors[i].spline == D_door->spline)
          break;
      }
      if (i < D_worldinfo->door_count)
        D_door->spline = 0;
    }
    if (D_door->spline == 0)
      D_door->name[0] = '\0';
  }
}

// FUNCTION: LEGOBATMAN 0x00614900
void D_level(NUFPAR *parser) {
  if (NuFParGetWord(parser) != 0) {
    i32 index;
    Level_FindByName(parser->word_buf, &index);
    if (index != -1)
      D_door->level = (i16)index;
  }
}

// FUNCTION: LEGOBATMAN 0x00614940
void D_level_again(NUFPAR *parser) {
  if (NuFParGetWord(parser) != 0) {
    i32 index;
    Level_FindByName(parser->word_buf, &index);
    if (index != -1)
      D_door->level_f2 = (i16)index;
  }
}

// FUNCTION: LEGOBATMAN 0x00614980
void D_level_freeplay(NUFPAR *parser) {
  if (NuFParGetWord(parser) != 0) {
    i32 index;
    Level_FindByName(parser->word_buf, &index);
    if (index != -1 && (LDataList[index].flags & 0xe0) == 0)
      D_door->freeplay_level = (i16)index;
  }
}

WORLDINFO_s *WorldInfo_CurrentlyLoading(void);
void *CutScene_Find(void *cutscene_sys, char *name);

i32 NuStrICmp(const char *a, const char *b);

// annotated in batman/aiactions_unk.cpp (0x00ad68ec)
extern u8 (*g_unk00ad68ec)(char *name);
// GLOBAL: LEGOBATMAN 0x0095fd48
extern u64 g_unk0095fd48;

// FUNCTION: LEGOBATMAN 0x00614b10
void D_vehicle(NUFPAR *parser) {
  if (NuFParGetWord(parser) != 0) {
    if (NuStrICmp(parser->word_buf, "all") == 0) {
      D_door->takeover_character_mask = g_unk0095fd48;
    } else {
      i32 type = g_unk00ad68ec(parser->word_buf);
      if (type < 64)
        D_door->takeover_character_mask |= (u64)1 << type;
    }
  }
}

static i32 D_ParseGround(NUFPAR *parser) {
  if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "air") == 0)
    return 0;
  return 1;
}

// FUNCTION: LEGOBATMAN 0x00614c20
void D_teleport_player(NUFPAR *parser) {
  D_door->flags |= 0x20;
  D_door->teleport_in = 1;
  D_door->teleport_out = 1;
  while (NuFParGetWord(parser) != 0) {
    if (NuStrICmp(parser->word_buf, "in") == 0) {
      D_door->teleport_in = D_ParseGround(parser);
    } else if (NuStrICmp(parser->word_buf, "out") == 0) {
      D_door->teleport_out = D_ParseGround(parser);
    }
  }
}

// FUNCTION: LEGOBATMAN 0x00614bb0
void D_cut_scene(NUFPAR *parser) {
  if (NuFParGetWord(parser) != 0) {
    D_door->cutscene = CutScene_Find(WorldInfo_CurrentlyLoading()->cutscene_sys,
                                     parser->word_buf);
  }
}

// FUNCTION: LEGOBATMAN 0x006149e0
void D_cam_spline(NUFPAR *parser) {
  if (NuFParGetWord(parser) != 0 && NuStrLen(parser->word_buf) < 32) {
    NuStrCpy(D_door->camera_spline_name, parser->word_buf);
    D_door->camera_spline =
        NuSplineFind(D_worldinfo->scn140, D_door->camera_spline_name);
    if (D_door->camera_spline != 0 && D_door->camera_spline->length != 2) {
      D_door->camera_spline = 0;
      D_door->camera_spline_name[0] = '\0';
    }
  }
}

// FUNCTION: LEGOBATMAN 0x00614a70
void D_cam_wait(NUFPAR *parser) {
  D_door->camera_wait = NuFParGetFloat(parser);
}

// FUNCTION: LEGOBATMAN 0x00614a90
void D_cam_blend_time(NUFPAR *parser) {
  D_door->camera_blend_time = NuFParGetFloat(parser);
}

// FUNCTION: LEGOBATMAN 0x00614ab0
void D_cam_lookatplayers(NUFPAR *parser) { D_door->flags |= 2; }

// FUNCTION: LEGOBATMAN 0x00614ac0
void D_one_way(NUFPAR *parser) { D_door->flags |= 1; }

// FUNCTION: LEGOBATMAN 0x00614ad0
void D_two_player_only(NUFPAR *parser) { D_door->flags |= 0x10; }

// FUNCTION: LEGOBATMAN 0x00614ae0
void D_do_not_use(NUFPAR *parser) { D_door->flags |= 4; }

// FUNCTION: LEGOBATMAN 0x00614af0
void D_next_sock(NUFPAR *parser) {
  i32 next_sock = NuFParGetInt(parser);
  if ((u32)next_sock <= 31)
    D_door->next_sock = next_sock;
}

// FUNCTION: LEGOBATMAN 0x00614ba0
void D_use_as_start(NUFPAR *parser) { D_door->flags |= 8; }

struct MINICAM_ADDSUBTITLE_s {
  i16 id;     // 0x00
  u8 b2;      // 0x02
  u8 b3;      // 0x03
  u8 b4;      // 0x04
  u8 b5;      // 0x05
  u8 b6;      // 0x06
  u8 flags;   // 0x07, 1/2: start/end times are relative
  f32 start;  // 0x08
  f32 end;    // 0x0c
  f32 f10[6]; // 0x10
};

struct MINICAM_SUBTITLE_s {
  i16 id; // 0x00
  u8 b2;  // 0x02
  u8 b3;  // 0x03
  u8 b4;  // 0x04
  u8 b5;  // 0x05
  u8 b6;  // 0x06
  u8 pad7;
  f32 start;  // 0x08
  f32 end;    // 0x0c
  f32 f10[6]; // 0x10
};

// One block: writing a subtitle may alias the count and the clock.
struct MINICAM_s {
  MINICAM_SUBTITLE_s subtitles[8]; // 0x000
  i8 subtitle_count;               // 0x140
  u8 pad141[0x1b4 - 0x141];
  f32 time; // 0x1b4
};

// GLOBAL: LEGOBATMAN 0x00acb4b8
extern MINICAM_s Minicam;

// FUNCTION: LEGOBATMAN 0x006163b0
void Minicam_AddSubtitle(const MINICAM_ADDSUBTITLE_s *add) {
  if (add != 0 && add->id != -1 && Minicam.subtitle_count < 8) {
    MINICAM_SUBTITLE_s *sub = &Minicam.subtitles[Minicam.subtitle_count];
    sub->id = add->id;
    sub->b2 = add->b2;
    sub->b3 = add->b3;
    sub->b4 = add->b4;
    sub->b5 = add->b5;
    sub->b6 = add->b6;
    sub->id = add->id;
    if (add->flags & 1)
      sub->start = add->start + Minicam.time;
    else
      sub->start = add->start;
    if (add->flags & 2)
      sub->end = add->end + Minicam.time;
    else
      sub->end = add->end;
    sub->f10[0] = add->f10[0];
    sub->f10[1] = add->f10[1];
    sub->f10[2] = add->f10[2];
    sub->f10[3] = add->f10[3];
    sub->f10[4] = add->f10[4];
    sub->f10[5] = add->f10[5];
    Minicam.subtitle_count++;
  }
}

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
// GLOBAL: LEGOBATMAN 0x00963708
i32 door_gizmotype_id = -1;

void AddGizmo(struct GIZMOSYS_s *gizmo_sys, i32 type_id, void *a, void *object);
void GizmoGetUniqueName(struct GIZMOSYS_s *gizmo_sys, char *prefix, char *name,
                        char *out, i32 size);

// FUNCTION: LEGOBATMAN 0x00615580
i32 Door_GetMaxGizmos(void *door) {
  WORLDINFO_s *world = (WORLDINFO_s *)door;
  return world != NULL ? world->current_level->max_doors : 0;
}

// FUNCTION: LEGOBATMAN 0x006155a0
void Door_AddGizmos(struct GIZMOSYS_s *gizmo_sys, i32 type_id, void *world_ptr,
                    void *unused) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  if (world != NULL && world->doors != NULL) {
    for (i32 i = 0; i < world->door_count; i++) {
      GizmoGetUniqueName(world->gizmo_sys, "Door_", world->doors[i].name,
                         &world->doors[i].name[0x40], 0x40);
      AddGizmo(gizmo_sys, type_id, NULL, &world->doors[i]);
    }
  }
}

// FUNCTION: LEGOBATMAN 0x00615620
char *Door_GetGizmoName(GIZMO *gizmo) {
  if (gizmo == NULL || gizmo->object == NULL)
    return NULL;
  DOOR_s *door = (DOOR_s *)gizmo->object;
  return &door->name[0x40];
}

// FUNCTION: LEGOBATMAN 0x00615640
i32 Door_GetOutput(GIZMO *gizmo, i32 a, i32 b) {
  if (gizmo == NULL || gizmo->object == NULL)
    return 0;
  DOOR_s *door = (DOOR_s *)gizmo->object;
  return door->active == 0;
}

// FUNCTION: LEGOBATMAN 0x00615660
i32 Door_GetNumOutputs(GIZMO *gizmo) { return 1; }

// FUNCTION: LEGOBATMAN 0x00615670
char *Door_GetOutputName(GIZMO *gizmo, i32 output) { return "Active"; }

// FUNCTION: LEGOBATMAN 0x00615680
void Door_Activate(GIZMO *gizmo, i32 value) {
  if (gizmo == NULL || gizmo->object == NULL)
    return;
  DOOR_s *door = (DOOR_s *)gizmo->object;
  door->active = value == 0;
}

// FUNCTION: LEGOBATMAN 0x006156a0
ADDGIZMOTYPE *Door_RegisterGizmo(i32 type_id) {
  // GLOBAL: LEGOBATMAN 0x00acb088
  static ADDGIZMOTYPE addtype;

  addtype = Default_ADDGIZMOTYPE;
  addtype.name = "Door";
  addtype.prefix = "";
  addtype.progress_size = 0;
  addtype.fns[0] = (void *)Door_GetMaxGizmos;
  addtype.fns[1] = (void *)Door_AddGizmos;
  addtype.fns[2] = NULL;
  addtype.fns[3] = NULL;
  addtype.fns[4] = NULL;
  addtype.fns[5] = NULL;
  addtype.fns[6] = (void *)Door_GetGizmoName;
  addtype.fns[7] = (void *)Door_GetOutput;
  addtype.fns[8] = (void *)Door_GetOutputName;
  addtype.fns[9] = (void *)Door_GetNumOutputs;
  addtype.fns[10] = (void *)Door_Activate;
  addtype.fns[11] = NULL;
  addtype.fns[12] = NULL;
  addtype.fns[13] = NULL;
  addtype.fns[14] = NULL;
  addtype.fns[15] = NULL;
  addtype.fns[16] = NULL;
  addtype.fns[17] = NULL;
  addtype.fns[18] = NULL;
  addtype.fns[19] = NULL;
  addtype.fns[20] = NULL;
  addtype.fns[21] = NULL;
  addtype.fns[22] = NULL;
  addtype.fns[23] = NULL;
  addtype.fns[24] = NULL;
  addtype.fns[25] = NULL;
  addtype.fns[26] = NULL;
  addtype.fns[27] = NULL;
  door_gizmotype_id = type_id;
  return &addtype;
}

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_doors_unk(f32 *v, f32 a, i32 i) {
  v[2] = NuFabs(a);
  v[0] = NuSinApprox(i);
  NuVec4Set(v, a, a, a, a);
  v[3] = NuFdiv(a, v[4]);
  v[1] = NuCosApprox(i);
}
