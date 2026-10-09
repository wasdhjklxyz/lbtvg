// batman/, per-level files unknown: small level Init/Reset functions.

#include "../nu2api/nucore/nustring.h"
#include "worldinfo_unk.h"

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
  unsigned char pad60[0xe0 - 0x60];
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
