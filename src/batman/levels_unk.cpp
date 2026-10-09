// batman/, per-level files unknown: small level Init/Reset functions.

#include "../nu2api/nucore/nustring.h"
#include "worldinfo_unk.h"
#include <stddef.h>

// GLOBAL: LEGOBATMAN 0x009ca23c
GIZMOBLOWUP_s *g_unk009ca23c;
// GLOBAL: LEGOBATMAN 0x009ca264
GIZAIMESSAGE_s *g_unk009ca264;
// GLOBAL: LEGOBATMAN 0x00ad210c
extern GIZAIMESSAGESYS_s *g_unk00ad210c;
// GLOBAL: LEGOBATMAN 0x009ce9f0
AILOCATOR_s *g_unk009ce9f0[4];
// GLOBAL: LEGOBATMAN 0x009ce960
extern nuhspecial_s g_unk009ce960;
// GLOBAL: LEGOBATMAN 0x009ce994
extern nuhspecial_s g_unk009ce994;
// GLOBAL: LEGOBATMAN 0x009ce7a4
extern nuhspecial_s g_unk009ce7a4;
// GLOBAL: LEGOBATMAN 0x009ce8c4
extern nuhspecial_s g_unk009ce8c4;
// GLOBAL: LEGOBATMAN 0x009ca180
GIZMO_s *g_unk009ca180;
// GLOBAL: LEGOBATMAN 0x0095fb5c
extern i32 g_unk0095fb5c;

void Unk005f8b20(void *p);
void Unk005f8b10(void *p);
extern u8 g_unk009623b4;
extern u8 g_unk00aca1f8;

// FUNCTION: LEGOBATMAN 0x005088e0
void NastySewersC_Init(WORLDINFO_s *wi) {
  g_unk009ca23c = GizmoBlowUp_FindByName(wi, "sonar_mirror1");
}

// FUNCTION: LEGOBATMAN 0x005101b0
void Fairground_C_Reset(WORLDINFO_s *wi) {
  g_unk009ca264 = CheckGizAIMessage(g_unk00ad210c, "BossFightPhase", 0);
}

// STUB: LEGOBATMAN 0x005026f0
// original defers one add esp,0x40 across the first six calls; ordering of the
// stores differs
void FortBloxHero_B_Init(WORLDINFO_s *wi) {
  g_unk009ce9f0[0] = AIPathFindLocator(wi->aiSys2bf8, "ElectricFloor_1");
  g_unk009ce9f0[1] = AIPathFindLocator(wi->aiSys2bf8, "ElectricFloor_2");
  g_unk009ce9f0[2] = AIPathFindLocator(wi->aiSys2bf8, "ElectricFloor_3");
  g_unk009ce9f0[3] = AIPathFindLocator(wi->aiSys2bf8, "ElectricFloor_4");
  NuSpecialFind(wi->scn140, &g_unk009ce960, "Coils_Lane1", 0);
  NuSpecialFind(wi->scn140, &g_unk009ce994, "Coils_Lane2", 0);
  NuSpecialFind(wi->scn140, &g_unk009ce7a4, "Coils_Lane3", 0);
  NuSpecialFind(wi->scn140, &g_unk009ce8c4, "Coils_Lane4", 0);
  g_unk009ca264 = CheckGizAIMessage(g_unk00ad210c, "InLaserRoom", 0);
}

// STUB: LEGOBATMAN 0x00502b60
// same stack-cleanup grouping difference as FortBloxHero_B_Init
void FortBloxHero_B_Reset(WORLDINFO_s *wi) {
  g_unk009ce9f0[0] = AIPathFindLocator(wi->aiSys2bf8, "ElectricFloor_1");
  g_unk009ce9f0[1] = AIPathFindLocator(wi->aiSys2bf8, "ElectricFloor_2");
  g_unk009ce9f0[2] = AIPathFindLocator(wi->aiSys2bf8, "ElectricFloor_3");
  g_unk009ce9f0[3] = AIPathFindLocator(wi->aiSys2bf8, "ElectricFloor_4");
  NuSpecialFind(wi->scn140, &g_unk009ce960, "Coils_Lane1", 0);
  NuSpecialFind(wi->scn140, &g_unk009ce994, "Coils_Lane2", 0);
  NuSpecialFind(wi->scn140, &g_unk009ce7a4, "Coils_Lane3", 0);
  NuSpecialFind(wi->scn140, &g_unk009ce8c4, "Coils_Lane4", 0);
}

// FUNCTION: LEGOBATMAN 0x00504100
void BotanicGardens_B_Reset(WORLDINFO_s *wi) {
  Unk005f8b20(&g_unk009623b4);
  Unk005f8b10(&g_unk00aca1f8);
  g_unk009ca180 = GizmoFindByName(wi->gizmoSys2b0c, g_unk0095fb5c, "techno1");
  g_unk009ca23c = GizmoBlowUp_FindByName(wi, "bomb_dropb1");
}

typedef struct LEVELDATA_s {
  unsigned char pad0[0x40];
  char name[0x20]; // 0x40
  unsigned char pad60[0x64 - 0x60];
  u32 flags;             // 0x64
  void (*fns[10])(void); // 0x68; per-level callbacks set by Levels_FixUp
  unsigned char pad90[0xd9 - 0x90];
  u8 blob_shadow_fade_near; // 0xd9
  u8 blob_shadow_fade_far;  // 0xda
  unsigned char paddb[0xe0 - 0xdb];
  f32 conveyor_x_speed; // 0xe0
  f32 conveyor_z_speed; // 0xe4
  unsigned char pade8[0x150 - 0xe8];
} LEVELDATA;

// GLOBAL: LEGOBATMAN 0x00aca8a4
i32 LEVELCOUNT;

// GLOBAL: LEGOBATMAN 0x00aca894
LEVELDATA *LDataList = 0;

typedef struct MENU_s {
  u32 pad0[0x94 / 4];
  f32 draw_x; // 0x94
} MENU;

// GLOBAL: LEGOBATMAN 0x00ab093c
extern i32 Paused;
// GLOBAL: LEGOBATMAN 0x00aca81c
extern i32 PauseMenus_Align;
// GLOBAL: LEGOBATMAN 0x00aca820
extern f32 PauseMenus_X;
// GLOBAL: LEGOBATMAN 0x00ad735c
extern i32 dme_align;
// GLOBAL: LEGOBATMAN 0x00ad7304
extern u8 MenuA;

void DrawMenuEntryEx(MENU *menu, char *text, i32 alpha);

// FUNCTION: LEGOBATMAN 0x0060cea0
void GameDrawMenuEntry(MENU_s *menu, char *text) {
  if (Paused != 0) {
    dme_align = PauseMenus_Align;
    menu->draw_x = PauseMenus_X;
  }
  DrawMenuEntryEx(menu, text, MenuA);
}

struct Unk00ad69e4 {
  i16 menu;
  u16 pad2;
  u32 pad4[(0xe0 - 4) / 4];
};

struct Unk0099e4e0 {
  i32 id;
  u32 pad4[(0x1c - 4) / 4];
};

// GLOBAL: LEGOBATMAN 0x0099e384
extern i32 g_unk0099e384;
// GLOBAL: LEGOBATMAN 0x00ad69e4
extern Unk00ad69e4 g_unk00ad69e4[];
// GLOBAL: LEGOBATMAN 0x0099e4e0
extern Unk0099e4e0 g_unk0099e4e0[];

// FUNCTION: LEGOBATMAN 0x0060cee0
i32 GetMenuID() {
  i16 menu = g_unk00ad69e4[g_unk0099e384].menu;
  if (menu == -1) {
    return -1;
  }
  return g_unk0099e4e0[menu].id;
}

// FUNCTION: LEGOBATMAN 0x0060d6c0
LEVELDATA *Level_FindByName(char *name, i32 *idx_out) {
  for (i32 i = 0; i < LEVELCOUNT; i++) {
    if (NuStrICmp(LDataList[i].name, name) == 0) {
      if (idx_out != 0) {
        *idx_out = i;
      }

      return &LDataList[i];
    }
  }

  if (idx_out != 0) {
    *idx_out = -1;
  }

  return 0;
}

typedef struct nufpar_s {
  unsigned char pad0[0x910];
  char *word_buf; // 0x910
} NUFPAR;
typedef struct nufpcomjmp_s {
  char *name;
  void (*fn)(NUFPAR *parser);
} nufpcomjmp_s;

NUFPAR *NuFParCreateMem(char *name, char *buffer, i32 size);
void NuFParPushCom2(NUFPAR *parser, nufpcomjmp_s *a, nufpcomjmp_s *b);
i32 NuFParGetLine(NUFPAR *parser);
i32 NuFParGetWord(NUFPAR *parser);
i32 NuFParInterpretWord(NUFPAR *parser);
void NuFParDestroy(NUFPAR *parser);

// GLOBAL: LEGOBATMAN 0x00aca828
extern LEVELDATA *levelconfig_ldata;
// GLOBAL: LEGOBATMAN 0x0096324c
extern nufpcomjmp_s LevelConfig_BeforeLoad_GenericKeywords[];

typedef void (*LEVELFIXUPFN)(void);

// Batman's table carries ten callbacks (saga has seven), copied to
// LEVELDATA +0x68..+0x8c.
typedef struct LEVELFIXUP {
  char *name;
  LEVELDATA **level;
  LEVELFIXUPFN fns[10]; // 0x08
} LEVELFIXUP;

// from saga legoapi/world/level.cpp
// FUNCTION: LEGOBATMAN 0x0060d7a0
void Levels_FixUp(LEVELFIXUP *fixup) {
  if (fixup == NULL) {
    return;
  }
  for (; fixup->name != NULL; fixup++) {
    if (fixup->level == NULL || *fixup->level != NULL) {
      continue;
    }
    LEVELDATA *level = Level_FindByName(fixup->name, NULL);
    *fixup->level = level;
    if (level == NULL) {
      continue;
    }
    if (fixup->fns[0] != NULL) {
      level->fns[0] = fixup->fns[0];
    }
    if (fixup->fns[1] != NULL) {
      level->fns[1] = fixup->fns[1];
    }
    if (fixup->fns[2] != NULL) {
      level->fns[2] = fixup->fns[2];
    }
    if (fixup->fns[3] != NULL) {
      level->fns[3] = fixup->fns[3];
    }
    if (fixup->fns[4] != NULL) {
      level->fns[4] = fixup->fns[4];
    }
    if (fixup->fns[5] != NULL) {
      level->fns[5] = fixup->fns[5];
    }
    if (fixup->fns[6] != NULL) {
      level->fns[6] = fixup->fns[6];
    }
    if (fixup->fns[7] != NULL) {
      level->fns[7] = fixup->fns[7];
    }
    if (fixup->fns[8] != NULL) {
      level->fns[8] = fixup->fns[8];
    }
    if (fixup->fns[9] != NULL) {
      level->fns[9] = fixup->fns[9];
    }
  }
}

// FUNCTION: LEGOBATMAN 0x0060d920
void LevelConfig_BeforeLoad(LEVELDATA *level, char *buffer,
                            nufpcomjmp_s *keywords) {
  NUFPAR *parser = NuFParCreateMem("levelbeforeload", buffer, 0xffff);
  if (parser == 0) {
    return;
  }

  levelconfig_ldata = level;
  NuFParPushCom2(parser, LevelConfig_BeforeLoad_GenericKeywords, keywords);
  while (NuFParGetLine(parser) != 0) {
    if (NuFParGetWord(parser) != 0) {
      NuFParInterpretWord(parser);
    }
  }
  NuFParDestroy(parser);
}

f32 NuFParGetFloat(NUFPAR *parser);

// FUNCTION: LEGOBATMAN 0x0060e510
void LC_AL_conveyor(NUFPAR *fp) {
  while (NuFParGetWord(fp) != 0) {
    if (NuStrICmp(fp->word_buf, "xspeed") == 0) {
      levelconfig_ldata->conveyor_x_speed = NuFParGetFloat(fp);
    } else if (NuStrICmp(fp->word_buf, "zspeed") == 0) {
      levelconfig_ldata->conveyor_z_speed = NuFParGetFloat(fp);
    }
  }
}

// GLOBAL: LEGOBATMAN 0x00963278
extern nufpcomjmp_s LevelConfig_AfterLoad_GenericKeywords[];

// FUNCTION: LEGOBATMAN 0x0060e5b0
void LevelConfig_AfterLoad(LEVELDATA *level, char *buffer,
                           nufpcomjmp_s *keywords) {
  NUFPAR *parser = NuFParCreateMem("levelafterload", buffer, 0xffff);
  if (parser == 0) {
    return;
  }

  levelconfig_ldata = level;
  NuFParPushCom2(parser, LevelConfig_AfterLoad_GenericKeywords, keywords);
  while (NuFParGetLine(parser) != 0) {
    if (NuFParGetWord(parser) != 0) {
      NuFParInterpretWord(parser);
    }
  }
  NuFParDestroy(parser);

  if (level->blob_shadow_fade_near > level->blob_shadow_fade_far) {
    level->blob_shadow_fade_near = level->blob_shadow_fade_far;
  }
  level->flags |= 1;
}

typedef struct LEVELOBJECT {
  u8 kind;
  u8 pad_01;
  u16 reflection;
  char *name;
} LEVELOBJECT;

// GLOBAL: LEGOBATMAN 0x00aca8e0
extern i32 LEVELOBJECTCOUNT;
// GLOBAL: LEGOBATMAN 0x00aca8e4
extern i32 LEVELOBJECTMAX;
// GLOBAL: LEGOBATMAN 0x00aca8ec
extern char *ExtraLevelObject_NameTable;
// GLOBAL: LEGOBATMAN 0x00aca8f4
extern i32 ExtraLevelObject_NameTableIndex;
// GLOBAL: LEGOBATMAN 0x00aca8f0
extern i32 ExtraLevelObject_NameTableSize;
// GLOBAL: LEGOBATMAN 0x00aca8dc
extern LEVELOBJECT *ObjTabList;
// GLOBAL: LEGOBATMAN 0x00aca8e8
extern i32 EXTRALEVELOBJECTCOUNT;

struct LEVELSPLINE {
  struct nugscn_s **scene;
  const char *name;
  u16 min_points;
  u16 max_points;
  i16 level;
  i16 area;
};

// GLOBAL: LEGOBATMAN 0x00aca8d4
extern LEVELSPLINE *LevSplList;
// GLOBAL: LEGOBATMAN 0x00aca8d8
extern i32 LEVELSPLINECOUNT;
// GLOBAL: LEGOBATMAN 0x00963188
extern i32 levspl_i_start;
// GLOBAL: LEGOBATMAN 0x0096318c
extern i32 levspl_i_startcam;

// FUNCTION: LEGOBATMAN 0x0060e640
void LevelSplines_InitForGame(LEVELSPLINE *splines) {
  LevSplList = splines;
  LEVELSPLINECOUNT = 0;

  if (splines == 0) {
    return;
  }

  for (LEVELSPLINE *spline = splines; spline->name != 0; ++spline) {
    if (levspl_i_start == -1 && NuStrICmp(spline->name, "start") == 0) {
      levspl_i_start = LEVELSPLINECOUNT;
    } else if (levspl_i_startcam == -1 &&
               NuStrICmp(spline->name, "start_cam") == 0) {
      levspl_i_startcam = LEVELSPLINECOUNT;
    }
    ++LEVELSPLINECOUNT;
  }
}

// GLOBAL: LEGOBATMAN 0x00963198
extern i32 LEVOBJREF_FIRSTOBJ;
// GLOBAL: LEGOBATMAN 0x0096319c
extern i32 LEVOBJREF_LASTOBJ;
// GLOBAL: LEGOBATMAN 0x009631a0
extern i32 LEVOBJREF_FIRSTREFOBJ;
// GLOBAL: LEGOBATMAN 0x009631a4
extern i32 LEVOBJREF_LASTREFOBJ;

void FUN_0060d010(i32 size);

// from saga legoapi/world/levelobjects.cpp
// STUB: LEGOBATMAN 0x0060e8e0
// one-instruction diff: orig schedules the LEVELOBJECTMAX store after the
// "or reg, -1" chain; insensitive to source position.
void LevelObjects_InitForGame(LEVELOBJECT *tab, VARIPTR *buf, VARIPTR *buf_end,
                              i32 max, i32 name_table_size) {
  LEVELOBJECTMAX = max;
  LEVOBJREF_FIRSTOBJ = -1;
  LEVOBJREF_LASTOBJ = -1;
  LEVOBJREF_FIRSTREFOBJ = -1;
  LEVOBJREF_LASTREFOBJ = -1;
  ObjTabList = tab;
  LEVELOBJECTCOUNT = 0;

  for (; tab->kind != 0xff; ++tab, ++LEVELOBJECTCOUNT) {
    if (tab->reflection == 1) {
      LEVOBJREF_LASTOBJ = LEVELOBJECTCOUNT;
      if (LEVOBJREF_FIRSTOBJ == -1) {
        LEVOBJREF_FIRSTOBJ = LEVELOBJECTCOUNT;
      }
    }
    if (tab->reflection == 2) {
      LEVOBJREF_LASTREFOBJ = LEVELOBJECTCOUNT;
      if (LEVOBJREF_FIRSTREFOBJ == -1) {
        LEVOBJREF_FIRSTREFOBJ = LEVELOBJECTCOUNT;
      }
    }
  }

  const i32 object_range = LEVOBJREF_LASTOBJ - LEVOBJREF_FIRSTOBJ;
  const i32 reflection_range = LEVOBJREF_LASTREFOBJ - LEVOBJREF_FIRSTREFOBJ;
  if (reflection_range < object_range) {
    LEVOBJREF_LASTOBJ = LEVOBJREF_FIRSTOBJ + reflection_range;
  } else if (reflection_range > object_range) {
    LEVOBJREF_LASTREFOBJ = LEVOBJREF_FIRSTREFOBJ + object_range;
  }

  if (name_table_size > 0) {
    u8 *table = buf->u8_ptr;
    ExtraLevelObject_NameTable = (char *)table;
    table += name_table_size;
    buf->u8_ptr = table;
    if (table < buf_end->u8_ptr) {
      ExtraLevelObject_NameTableSize = name_table_size;
      return;
    }
    ExtraLevelObject_NameTableSize = name_table_size;
    FUN_0060d010(name_table_size);
  }
}

// FUNCTION: LEGOBATMAN 0x0060e9c0
i32 LevelObject_AddExtra(char *name, i32 kind) {
  if (LEVELOBJECTCOUNT >= LEVELOBJECTMAX)
    return 0;
  if (ExtraLevelObject_NameTable == 0)
    return 0;
  i32 nameLen = NuStrLen(name);
  i32 index = ExtraLevelObject_NameTableIndex;
  if (index + nameLen + 1 >= ExtraLevelObject_NameTableSize)
    return 0;
  ObjTabList[LEVELOBJECTCOUNT].kind = (u8)kind;
  char *nameDest = ExtraLevelObject_NameTable + index;
  ObjTabList[LEVELOBJECTCOUNT].name = nameDest;
  LEVELOBJECTCOUNT++;
  EXTRALEVELOBJECTCOUNT++;
  NuStrCpy(nameDest, name);
  ExtraLevelObject_NameTableIndex += nameLen + 1;
  return 1;
}

// FUNCTION: LEGOBATMAN 0x0060ec80
i32 LevelObject_FindIndexFromName(char *name) {
  if (ObjTabList == 0)
    return -1;
  for (i32 i = 0; i < LEVELOBJECTCOUNT; i++) {
    if (NuStrICmp(ObjTabList[i].name, name) == 0) {
      return i;
    }
  }
  return -1;
}

// GLOBAL: LEGOBATMAN 0x00aa0568
extern char ConfigBuffer[];

i32 NuFileLoadBuffer(char *name, void *buffer, i32 size);
i32 Text_StripComments(char *in, char *out, i32 flag);

// FUNCTION: LEGOBATMAN 0x0060ee60
void Level_LoadConfigFile(WORLDINFO_s *world) {
  char name[128];

  ConfigBuffer[0] = '\0';
  NuSPrintf(name, "%s.txt", world->config_file);

  world->buf104.addr = (world->buf104.addr + 3) & ~3;
  i32 bytesRead = NuFileLoadBuffer(name, world->buf104.void_ptr, 0x10000);
  world->config_count = bytesRead;
  if (bytesRead > 0) {
    ((char *)world->buf104.void_ptr)[bytesRead] = '\0';
    bytesRead =
        Text_StripComments((char *)world->buf104.void_ptr, ConfigBuffer, 1);
    world->config_count = bytesRead;
  }
}
