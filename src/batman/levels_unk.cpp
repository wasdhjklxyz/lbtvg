// batman/, per-level files unknown: small level Init/Reset functions.

#include "../nu2api/nucore/nustring.h"
#include "worldinfo_unk.h"
#include <stddef.h>

// GLOBAL: LEGOBATMAN 0x009ca23c
GIZMOBLOWUP_s *g_unk009ca23c;
// GLOBAL: LEGOBATMAN 0x009ca264
GIZAIMESSAGE_s *g_unk009ca264;
// GLOBAL: LEGOBATMAN 0x00ad210c
extern GIZAIMESSAGESYS_s *gizaimessagesys;
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

// WORLDINFO +0x5220: 0x2c-byte records.
struct AABRESETITEM_s {
  u8 pad0[0x17];
  u8 kind;  // 0x17
  u8 flags; // 0x18
  u8 pad19[0x24 - 0x19];
  u8 active; // 0x24
  u8 pad25[0x2c - 0x25];
};

struct AABRESETLIST_s {
  AABRESETITEM_s *items; // 0x00
  u32 pad4;
  i32 count; // 0x08
};

// GLOBAL: LEGOBATMAN 0x009ce74c
extern u8 g_unk009ce74c;
// GLOBAL: LEGOBATMAN 0x009ce74d
extern u8 g_unk009ce74d;
// GLOBAL: LEGOBATMAN 0x009ce74e
extern u8 g_unk009ce74e;
// GLOBAL: LEGOBATMAN 0x009ce74f
extern u8 g_unk009ce74f;
// GLOBAL: LEGOBATMAN 0x009ce780
extern u8 g_unk009ce780;
// GLOBAL: LEGOBATMAN 0x009ce760
extern i32 g_aabGhostCount;
// GLOBAL: LEGOBATMAN 0x009ce764
extern void *g_aabGhosts[7];

extern "C" int NuSPrintf(char *buf, char *fmt, ...);
void *Unk0044c930(AISYS_s *sys, char *name);

#define AAB_LIST(w) (*(AABRESETLIST_s **)((u8 *)(w) + 0x5220))

// STUB: LEGOBATMAN 0x005013a0
// close: item/temp registers swapped eax-ecx in the reset loop, 3 tries
void ArkhamAsylum_B_Reset(WORLDINFO_s *world) {
  char name[12];
  AABRESETITEM_s *item = AAB_LIST(world)->items;
  g_unk009ce74f = 0;
  g_unk009ce74c = 0;
  g_unk009ce74e = 0;
  g_unk009ce74d = 0;
  g_unk009ce780 = 0;
  for (i32 i = 0; item != NULL && i < AAB_LIST(world)->count; i++, item++) {
    if ((item->flags & 8) == 0 && (u8)(item->kind - 1) <= 2)
      item->active = 0;
  }
  g_aabGhostCount = 0;
  for (i32 i = 0; i < 7; i++) {
    NuSPrintf(name, "GHOST%d", i + 1);
    g_aabGhosts[i] = Unk0044c930(world->aiSys2bf8, name);
    if (g_aabGhosts[i] == NULL)
      break;
    g_aabGhostCount++;
  }
}

// FUNCTION: LEGOBATMAN 0x005088e0
void NastySewersC_Init(WORLDINFO_s *wi) {
  g_unk009ca23c = GizmoBlowUp_FindByName(wi, "sonar_mirror1");
}

// GLOBAL: LEGOBATMAN 0x00967aec
extern i32 g_unk00967aec; // gizmo type of the tread elec bits
// GLOBAL: LEGOBATMAN 0x009cec2c
extern nuhspecial_s g_unk009cec2c;
// GLOBAL: LEGOBATMAN 0x009cec38
extern nuhspecial_s g_unk009cec38;
// GLOBAL: LEGOBATMAN 0x009cec64
extern u8 *g_unk009cec64;

i32 Unk005bc700(GIZMOSYS_s *sys, GIZMO_s *gizmo, i32 a3, i32 a4);
void Unk0050b6a0();
void Unk0050b7f0(WORLDINFO_s *world);
extern "C" void NuSpecialSetVisibility(void *special_ptr, int visible);

// FUNCTION: LEGOBATMAN 0x0050bb30
void EvilArctic_C_Update(WORLDINFO_s *world) {
  GIZMO_s *tread1 =
      GizmoFindByName(world->gizmoSys2b0c, g_unk00967aec, "qaz_tread1_elecbit");
  GIZMO_s *tread2 =
      GizmoFindByName(world->gizmoSys2b0c, g_unk00967aec, "qaz_tread2_elecbit");
  GizmoFindByName(world->gizmoSys2b0c, g_unk00967aec, "qaz_tread3_elecbit");
  GizmoFindByName(world->gizmoSys2b0c, g_unk00967aec, "qaz_tread4_elecbit");
  Unk0050b6a0();
  if (Unk005bc700(world->gizmoSys2b0c, tread1, 0, 0) &&
      Unk005bc700(world->gizmoSys2b0c, tread2, 0, 0)) {
    g_unk009cec64[4] = 1;
    Unk0050b7f0(world);
    return;
  }
  NuSpecialSetVisibility(&g_unk009cec2c, 0);
  NuSpecialSetVisibility(&g_unk009cec38, 0);
  g_unk009cec64[4] = 0;
}

// FUNCTION: LEGOBATMAN 0x005101b0
void Fairground_C_Reset(WORLDINFO_s *wi) {
  g_unk009ca264 = CheckGizAIMessage(gizaimessagesys, "BossFightPhase", 0);
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
  g_unk009ca264 = CheckGizAIMessage(gizaimessagesys, "InLaserRoom", 0);
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

// FUNCTION: LEGOBATMAN 0x00504680
AILOCATOR_s *GetPlantLocator(char *name) {
  AILOCATOR_s *locator = NULL;
  if (NuStrICmp(name, "Plant1") == 0)
    locator =
        AIPathFindLocator(WorldInfo_CurrentlyActive()->aiSys2bf8, "SEED1");
  else if (NuStrICmp(name, "Plant2") == 0)
    locator =
        AIPathFindLocator(WorldInfo_CurrentlyActive()->aiSys2bf8, "SEED2");
  else if (NuStrICmp(name, "Plant3") == 0)
    locator =
        AIPathFindLocator(WorldInfo_CurrentlyActive()->aiSys2bf8, "SEED3");
  return locator;
}

#include "leveldata_unk.h"

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

// GLOBAL: LEGOBATMAN 0x00960048
extern i32 g_unk00960048; // gizmo type id of levers

class InteractiveDisplayBase {
public:
  void InitializePerm(char *name, variptr_u *buffer, variptr_u *buffer_end);
  void DumpLevel(WORLDINFO_s *world);
};

class DynamicMaterialManager {
public:
  void *GetMaterial(char const *a, char const *b, int c);
};

// GLOBAL: LEGOBATMAN 0x00ad2af8
extern DynamicMaterialManager g_dynamicMaterialManager;
// GLOBAL: LEGOBATMAN 0x00960684
extern u8 g_unk00960684[];
// GLOBAL: LEGOBATMAN 0x0096068c
extern u8 g_unk0096068c[];

class SecurityCamera : public InteractiveDisplayBase {
public:
  // GLOBAL: LEGOBATMAN 0x00945bf4
  static f32 m_noFocusCameraTime;

  enum SecurityCameraType {};
  void ActivateLevel(WORLDINFO_s *world);
  void DumpLevel(WORLDINFO_s *world);
  static i32 AllowPlayerDropOut();
  static i32 AreVillainLevelsAvailable();
  void InitializePerm(SecurityCameraType type, variptr_u *buffer,
                      variptr_u *buffer_end);

  u8 pad0[0x10];
  char level_name[0x40];              // 0x010
  char transition_name[0x23c - 0x50]; // 0x050
  char background_name[0x40];         // 0x23c
  char frame_name[0x40];              // 0x27c
  u8 pad2bc[0x3d0 - 0x2bc];
  u8 *p3d0; // 0x3d0
  u8 pad3d4[0x3ec - 0x3d4];
  i32 kind;     // 0x3ec, 1 = hologram
  void *mtl3f0; // 0x3f0
  void *mtl3f4; // 0x3f4
  u8 b3f8;      // 0x3f8
  u8 pad3f9[3];
  GIZMO_s *lever; // 0x3fc
};

// FUNCTION: LEGOBATMAN 0x00513430
void SecurityCamera::InitializePerm(SecurityCameraType type, variptr_u *buffer,
                                    variptr_u *buffer_end) {
  kind = type;
  NuStrCpy(level_name, "levels\\batcave\\batcave_a");
  if (kind == 1) {
    NuStrCpy(background_name,
             "Stuff\\interactivedisplay\\securitycamera\\bg_seevillains");
    NuStrCpy(frame_name, "Stuff\\interactivedisplay\\frames\\frame_hero");
    if (transition_name[0] == 0)
      NuStrCpy(transition_name, "transition_monitor1");
    p3d0 = g_unk0096068c;
  } else {
    NuStrCpy(background_name,
             "Stuff\\interactivedisplay\\securitycamera\\bg_seeheroes");
    NuStrCpy(frame_name, "Stuff\\interactivedisplay\\frames\\frame_villain");
    if (transition_name[0] == 0)
      NuStrCpy(transition_name, "transition_monitor2");
    p3d0 = g_unk00960684;
  }
  mtl3f4 = g_dynamicMaterialManager.GetMaterial(
      "Stuff\\interactivedisplay\\securitycamera\\icon_play", level_name, 4);
  mtl3f0 = g_dynamicMaterialManager.GetMaterial(NULL, level_name, 3);
  InteractiveDisplayBase::InitializePerm("", buffer, buffer_end);
  b3f8 = 0;
  lever = NULL;
}

// FUNCTION: LEGOBATMAN 0x00512fe0
void SecurityCamera::ActivateLevel(WORLDINFO_s *world) {
  if (NuStrICmp((char *)world, level_name) == 0) {
    lever = GizmoFindByName(g_unk00960894->gizmoSys2b0c, g_unk00960048,
                            kind == 1 ? "hologram_lever" : "projectorLever");
    lever = (lever != NULL && ((u8 *)lever)[6] == g_unk00960048) ? lever : NULL;
  }
}

struct SCMTL_s {
  u8 pad0[0x74];
  u16 tid; // 0x74
};

void NuMtlUpdate(SCMTL_s *mtl);

class InteractiveDisplay;

class InteractiveDisplayManager {
public:
  static InteractiveDisplay *GetTransitioningDisplay();
  static InteractiveDisplay *GetCameraTarget();
};

char *NuStrIStr(char *str, const char *sub);
i32 Episode_CountOpenAreas(i32 episode, i32 area);

// FUNCTION: LEGOBATMAN 0x00512f80
i32 SecurityCamera::AllowPlayerDropOut() {
  if (g_unk00960894 != NULL && NuStrIStr((char *)g_unk00960894, "batcave_a")) {
    if (m_noFocusCameraTime < 5.0f)
      return 0;
    if (InteractiveDisplayManager::GetTransitioningDisplay() != NULL)
      return 0;
    return InteractiveDisplayManager::GetCameraTarget() == NULL;
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x00513070
void SecurityCamera::DumpLevel(WORLDINFO_s *world) {
  InteractiveDisplayBase::DumpLevel(world);
  if (NuStrICmp((char *)world, level_name) == 0) {
    if (mtl3f4 != NULL) {
      ((SCMTL_s *)mtl3f4)->tid = 0;
      NuMtlUpdate((SCMTL_s *)mtl3f4);
    }
    lever = NULL;
    m_noFocusCameraTime = 1000.0f;
  }
}

// FUNCTION: LEGOBATMAN 0x005130e0
i32 SecurityCamera::AreVillainLevelsAvailable() {
  if (Episode_CountOpenAreas(3, -1))
    return 1;
  if (Episode_CountOpenAreas(4, -1))
    return 1;
  return Episode_CountOpenAreas(5, -1) != 0;
}

i32 Unk006004d0(char *class_name);
void *Unk00600530(char *class_name, i32 index);

struct SHOPMENU_s {
  void Unk00511aa0(WORLDINFO_s *world, i32 a2, i32 a3);

  void **icons; // 0x00
  u32 pad4;
  f32 f8; // 0x08
  u32 padc[(0x1c - 0xc) / 4];
  f32 f1c; // 0x1c
  u32 pad20[(0x118 - 0x20) / 4];
};

class InteractiveDisplay {
public:
  void InitializeLevel(WORLDINFO_s *world);
};

class ShopComputer : public InteractiveDisplay {
public:
  static i32 IsAnyMenuChanging();
  void InitializeLevel(WORLDINFO_s *world);
  static void parse_reversedirection(struct nufpar_s *fp);
  static void parse_levelpath(struct nufpar_s *fp);
  static void parse_bgimagefilename(struct nufpar_s *fp);
  static void parse_overlayimagefilename(struct nufpar_s *fp);
  static void parse_specialobjectname(struct nufpar_s *fp);
  static void parse_villainmode(struct nufpar_s *fp);
  void LoadSettings(char *name);

  void **vtable;
  u8 pad4[0x10 - 4];
  char level_name[0x40];      // 0x010
  char transition_name[0x40]; // 0x050
  u8 pad090[0x230 - 0x90];
  i32 reverse_direction; // 0x230
  u8 pad234[0x23c - 0x234];
  char bg_name[0x40];      // 0x23c
  char overlay_name[0x40]; // 0x27c
  u8 pad2bc[0x340 - 0x2bc];
  i32 villain_mode; // 0x340
  u8 pad344[0x388 - 0x344];
  nugscn_s *icons_scene; // 0x388
  SHOPMENU_s menu_data;  // 0x38c
  SHOPMENU_s *menu;      // 0x4a4
  u8 pad4a8[0x4c8 - 0x4a8];
  i32 i4c8; // 0x4c8
  f32 f4cc; // 0x4cc
  u8 pad4d0[4];
  f32 f4d4; // 0x4d4
  u8 pad4d8[4];
  f32 f4dc[6]; // 0x4dc
};

typedef i32(__thiscall *ShopComputerVFn)(ShopComputer *);

extern "C" i32 NuMtlSetCurrentRenderPlane(i32 render_plane);

// GLOBAL: LEGOBATMAN 0x00945ce4
extern char *g_shopIconNames[];

struct InteractiveDisplayStatics {
  static void *GetFirstSpecialMaterial(nugscn_s *scene, char *name);
};

typedef struct nufpar_s {
  unsigned char pad0[0x910];
  char *word_buf; // 0x910
} NUFPAR;

i32 NuFParGetWord(NUFPAR *parser);
NUFPAR *NuFParCreate(char *filename);
i32 NuFParPushCom(NUFPAR *parser, void *commands);
i32 NuFParGetLine(NUFPAR *parser);
i32 NuFParInterpretWord(NUFPAR *parser);
void NuFParDestroy(NUFPAR *parser);
void Unk00514330(char *name); // empty in the release build

// GLOBAL: LEGOBATMAN 0x009cf618
extern ShopComputer *g_unk009cf618; // the shop computer being parsed
// GLOBAL: LEGOBATMAN 0x00945d00
extern u8 g_unk00945d00[]; // ShopComputer keyword table

// FUNCTION: LEGOBATMAN 0x00516420
void ShopComputer::parse_reversedirection(NUFPAR *fp) {
  g_unk009cf618->reverse_direction = 1;
}

// FUNCTION: LEGOBATMAN 0x00516440
void ShopComputer::parse_levelpath(NUFPAR *fp) {
  if (NuFParGetWord(fp) && NuStrLen(fp->word_buf) < 0x40)
    NuStrCpy(g_unk009cf618->level_name, fp->word_buf);
}

// FUNCTION: LEGOBATMAN 0x005164a0
void ShopComputer::parse_bgimagefilename(NUFPAR *fp) {
  if (NuFParGetWord(fp) && NuStrLen(fp->word_buf) < 0x40)
    NuStrCpy(g_unk009cf618->bg_name, fp->word_buf);
}

// FUNCTION: LEGOBATMAN 0x00516500
void ShopComputer::parse_overlayimagefilename(NUFPAR *fp) {
  if (NuFParGetWord(fp) && NuStrLen(fp->word_buf) < 0x40)
    NuStrCpy(g_unk009cf618->overlay_name, fp->word_buf);
}

// FUNCTION: LEGOBATMAN 0x00516560
void ShopComputer::parse_specialobjectname(NUFPAR *fp) {
  if (NuFParGetWord(fp) && NuStrLen(fp->word_buf) < 0x40 &&
      g_unk009cf618->transition_name[0] == 0)
    NuStrCpy(g_unk009cf618->transition_name, fp->word_buf);
}

// FUNCTION: LEGOBATMAN 0x005165c0
void ShopComputer::parse_villainmode(NUFPAR *fp) {
  g_unk009cf618->villain_mode = 1;
}

// FUNCTION: LEGOBATMAN 0x005165e0
void ShopComputer::LoadSettings(char *name) {
  NUFPAR *fp = NuFParCreate(name);
  if (fp == NULL) {
    Unk00514330(name);
    return;
  }
  g_unk009cf618 = this;
  NuFParPushCom(fp, g_unk00945d00);
  while (NuFParGetLine(fp)) {
    if (NuFParGetWord(fp))
      NuFParInterpretWord(fp);
  }
  NuFParDestroy(fp);
  g_unk009cf618 = NULL;
}

// STUB: LEGOBATMAN 0x00514bd0
// close: only "pop ebp" is scheduled one store earlier than the orig (3
// store orders tried)
void ShopComputer::InitializeLevel(WORLDINFO_s *world) {
  InteractiveDisplay::InitializeLevel(world);
  if (NuStrICmp((char *)world, level_name) == 0) {
    menu_data.Unk00511aa0(world, 6, 0);
    i32 plane = NuMtlSetCurrentRenderPlane(3);
    icons_scene = NuGScnRead(
        &world->buf104, world->bufEnd108,
        "stuff\\interactivedisplay\\shopcomputer\\icons\\computer_icons.gsc");
    NuMtlSetCurrentRenderPlane(plane);
    if (icons_scene != NULL) {
      for (i32 i = 0; g_shopIconNames[i] != NULL; i++)
        menu_data.icons[i] = InteractiveDisplayStatics::GetFirstSpecialMaterial(
            icons_scene, g_shopIconNames[i]);
    }
    f4d4 = 0.0f;
    i4c8 = 0;
    f4cc = 0.0f;
    menu = &menu_data;
    f4dc[0] = 1.0f;
    f4dc[1] = 1.0f;
    f4dc[2] = 1.0f;
    f4dc[3] = 1.0f;
    f4dc[4] = 1.0f;
    f4dc[5] = 1.0f;
  }
}

// FUNCTION: LEGOBATMAN 0x00515150
i32 ShopComputer::IsAnyMenuChanging() {
  i32 count = Unk006004d0("ShopComputer");
  for (i32 i = 0; i < count; i++) {
    ShopComputer *shop = (ShopComputer *)Unk00600530("ShopComputer", i);
    if (shop != NULL && ((ShopComputerVFn)shop->vtable[16])(shop)) {
      SHOPMENU_s *menu = shop->menu;
      if (menu != NULL && (!(menu->f1c < 0.001f) || !(menu->f8 > (f64)0.999f)))
        return 1;
    }
  }
  return 0;
}

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

i32 NuFParGetInt(NUFPAR *parser);

// FUNCTION: LEGOBATMAN 0x0060d8b0
void LC_BL_mipmapmode(NUFPAR *parser) {
  levelconfig_ldata->mipmapmode = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x0060d8e0
void LC_BL_max_ter_groups(NUFPAR *parser) {
  levelconfig_ldata->max_ter_groups = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x0060d900
void LC_BL_max_ter_platforms(NUFPAR *parser) {
  levelconfig_ldata->max_ter_platforms = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x0060da30
void LC_AL_backr_top(NUFPAR *parser) {
  levelconfig_ldata->backr_top = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x0060da50
void LC_AL_backg_top(NUFPAR *parser) {
  levelconfig_ldata->backg_top = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x0060da70
void LC_AL_backb_top(NUFPAR *parser) {
  levelconfig_ldata->backb_top = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x0060da90
void LC_AL_backr_bottom(NUFPAR *parser) {
  levelconfig_ldata->backr_bottom = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x0060dab0
void LC_AL_backg_bottom(NUFPAR *parser) {
  levelconfig_ldata->backg_bottom = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x0060dad0
void LC_AL_backb_bottom(NUFPAR *parser) {
  levelconfig_ldata->backb_bottom = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x0060dc20
void LC_AL_hover_height(NUFPAR *parser) {
  levelconfig_ldata->hover_height = NuFParGetFloat(parser);
}

// FUNCTION: LEGOBATMAN 0x0060dc40
void LC_AL_blobshadow_alpha(NUFPAR *parser) {
  levelconfig_ldata->blobshadow_alpha = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x0060dc60
void LC_AL_blobshadow_fadenear(NUFPAR *parser) {
  levelconfig_ldata->blob_shadow_fade_near = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x0060dc80
void LC_AL_blobshadow_fadefar(NUFPAR *parser) {
  levelconfig_ldata->blob_shadow_fade_far = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x0060dca0
void LC_AL_reflect_range(NUFPAR *parser) {
  levelconfig_ldata->reflect_range = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x0060dcc0
void LC_AL_reflect_y(NUFPAR *parser) {
  levelconfig_ldata->reflect_y = NuFParGetFloat(parser);
}

// FUNCTION: LEGOBATMAN 0x0060e0f0
void LC_AL_cam_tilt(NUFPAR *parser) {
  levelconfig_ldata->cam_tilt = NuFParGetFloat(parser);
}

// FUNCTION: LEGOBATMAN 0x0060e110
void LC_AL_raycaststep(NUFPAR *parser) {
  levelconfig_ldata->raycaststep = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x0060e130
void LC_AL_plat_scan_dist(NUFPAR *parser) {
  levelconfig_ldata->plat_scan_dist = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x0060e150
void LC_AL_waterripple_startcol_r(NUFPAR *parser) {
  levelconfig_ldata->waterripple_startcol_r = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x0060e170
void LC_AL_waterripple_startcol_g(NUFPAR *parser) {
  levelconfig_ldata->waterripple_startcol_g = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x0060e190
void LC_AL_waterripple_startcol_b(NUFPAR *parser) {
  levelconfig_ldata->waterripple_startcol_b = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x0060e1b0
void LC_AL_waterripple_startcol_a(NUFPAR *parser) {
  levelconfig_ldata->waterripple_startcol_a = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x0060e1d0
void LC_AL_waterripple_endcol_r(NUFPAR *parser) {
  levelconfig_ldata->waterripple_endcol_r = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x0060e1f0
void LC_AL_waterripple_endcol_g(NUFPAR *parser) {
  levelconfig_ldata->waterripple_endcol_g = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x0060e210
void LC_AL_waterripple_endcol_b(NUFPAR *parser) {
  levelconfig_ldata->waterripple_endcol_b = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x0060e230
void LC_AL_waterripple_endcol_a(NUFPAR *parser) {
  levelconfig_ldata->waterripple_endcol_a = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x0060e250
void LC_AL_waterripple_life(NUFPAR *parser) {
  levelconfig_ldata->waterripple_life = NuFParGetFloat(parser);
}

// FUNCTION: LEGOBATMAN 0x0060e340
void LC_AL_cam_look_rot_mul_x(NUFPAR *parser) {
  levelconfig_ldata->cam_look_rot_mul_x = NuFParGetFloat(parser);
}

// FUNCTION: LEGOBATMAN 0x0060e360
void LC_AL_cam_look_rot_mul_y(NUFPAR *parser) {
  levelconfig_ldata->cam_look_rot_mul_y = NuFParGetFloat(parser);
}

// FUNCTION: LEGOBATMAN 0x0060e380
void LC_AL_campos_seek(NUFPAR *parser) {
  levelconfig_ldata->campos_seek = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x0060e3a0
void LC_AL_camang_seek(NUFPAR *parser) {
  levelconfig_ldata->camang_seek = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x0060e3c0
void LC_AL_shadowtype(NUFPAR *parser) {
  levelconfig_ldata->shadowtype = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x0060e590
void LC_AL_char_clip_dist(NUFPAR *parser) {
  levelconfig_ldata->char_clip_dist = NuFParGetFloat(parser);
}

// FUNCTION: LEGOBATMAN 0x0060d9a0
void LC_AL_backr(NUFPAR *parser) {
  levelconfig_ldata->backr_bottom = NuFParGetInt(parser);
  levelconfig_ldata->backr_top = levelconfig_ldata->backr_bottom;
}

// FUNCTION: LEGOBATMAN 0x0060d9d0
void LC_AL_backg(NUFPAR *parser) {
  levelconfig_ldata->backg_bottom = NuFParGetInt(parser);
  levelconfig_ldata->backg_top = levelconfig_ldata->backg_bottom;
}

// FUNCTION: LEGOBATMAN 0x0060da00
void LC_AL_backb(NUFPAR *parser) {
  levelconfig_ldata->backb_bottom = NuFParGetInt(parser);
  levelconfig_ldata->backb_top = levelconfig_ldata->backb_bottom;
}

// FUNCTION: LEGOBATMAN 0x0060d8d0
void LC_BL_fix_strobing_anims(NUFPAR *parser) {
  levelconfig_ldata->flags |= 0x100;
}

// FUNCTION: LEGOBATMAN 0x0060dd20
void LC_AL_metal(NUFPAR *parser) {
  levelconfig_ldata->flags |= 0x1000;
  if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0)
    levelconfig_ldata->flags &= ~0x1000;
}

// FUNCTION: LEGOBATMAN 0x0060dd70
void LC_AL_in_space(NUFPAR *parser) {
  levelconfig_ldata->flags |= 0x40000;
  if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0)
    levelconfig_ldata->flags &= ~0x40000;
}

// FUNCTION: LEGOBATMAN 0x0060ddc0
void LC_AL_override_nopickupgravity(NUFPAR *parser) {
  levelconfig_ldata->flags |= 0x400000;
  if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0)
    levelconfig_ldata->flags &= ~0x400000;
}

// FUNCTION: LEGOBATMAN 0x0060ded0
void LC_AL_pickups_to_panel(NUFPAR *parser) {
  levelconfig_ldata->flags |= 0x80000;
  if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0)
    levelconfig_ldata->flags &= ~0x80000;
}

// FUNCTION: LEGOBATMAN 0x0060df20
void LC_AL_forget_takeovers(NUFPAR *parser) {
  levelconfig_ldata->flags |= 0x100000;
  if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0)
    levelconfig_ldata->flags &= ~0x100000;
}

// FUNCTION: LEGOBATMAN 0x0060df70
void LC_AL_narrow_socks(NUFPAR *parser) {
  levelconfig_ldata->flags |= 0x200000;
  if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0)
    levelconfig_ldata->flags &= ~0x200000;
}

// FUNCTION: LEGOBATMAN 0x0060dfc0
void LC_AL_camera_rain(NUFPAR *parser) {
  levelconfig_ldata->flags |= 0x4000;
  if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0)
    levelconfig_ldata->flags &= ~0x4000;
}

// FUNCTION: LEGOBATMAN 0x0060e010
void LC_AL_terrain_rain(NUFPAR *parser) {
  levelconfig_ldata->flags |= 0x8000;
  if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0)
    levelconfig_ldata->flags &= ~0x8000;
}

// FUNCTION: LEGOBATMAN 0x0060e060
void LC_AL_double_score(NUFPAR *parser) {
  levelconfig_ldata->flags |= 0x800;
  if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0)
    levelconfig_ldata->flags &= ~0x800;
}

// FUNCTION: LEGOBATMAN 0x0060e0b0
void LC_AL_flat_terrain(NUFPAR *parser) {
  levelconfig_ldata->flags |= 0x10;
  if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, "off") == 0)
    levelconfig_ldata->flags &= ~0x10;
}

i32 GetSfxId(char *name);

struct MusicManager {
  i32 GetTrackHandle(i32 track_class, char *name);
};

extern MusicManager music_man;

// FUNCTION: LEGOBATMAN 0x0060daf0
void LC_AL_farclip_hack(NUFPAR *parser) {
  f32 v = NuFParGetFloat(parser);
  if (v < 0.1f)
    v = 0.1f;
  else if (v > 50.0f)
    v = 50.0f;
  levelconfig_ldata->farclip_hack = v;
}

// FUNCTION: LEGOBATMAN 0x0060db70
void LC_AL_farclip(NUFPAR *parser) {
  i32 v = NuFParGetInt(parser);
  if (v < 10)
    v = 10;
  else if (v > 20000)
    v = 20000;
  levelconfig_ldata->farclip = v;
}

// FUNCTION: LEGOBATMAN 0x0060dbb0
void LC_AL_nearclip(NUFPAR *parser) {
  f32 v = NuFParGetFloat(parser);
  if (v < 0.001f)
    v = 0.001f;
  else if (v > 1.0f)
    v = 1.0f;
  levelconfig_ldata->nearclip = v;
}

// FUNCTION: LEGOBATMAN 0x0060de10
void LC_AL_hidden_icons(NUFPAR *parser) {
  if (NuFParGetWord(parser) != 0) {
    if (NuStrICmp(parser->word_buf, "on") == 0) {
      levelconfig_ldata->flags &= ~0x800000;
      return;
    }
    if (NuStrICmp(parser->word_buf, "off") == 0)
      levelconfig_ldata->flags |= 0x800000;
  }
}

// FUNCTION: LEGOBATMAN 0x0060de70
void LC_AL_underwater_hidden_icons(NUFPAR *parser) {
  if (NuFParGetWord(parser) != 0) {
    if (NuStrICmp(parser->word_buf, "on") == 0) {
      levelconfig_ldata->flags |= 0x10000000;
      return;
    }
    if (NuStrICmp(parser->word_buf, "off") == 0)
      levelconfig_ldata->flags &= ~0x10000000;
  }
}

// FUNCTION: LEGOBATMAN 0x0060e270
void LC_AL_cam_pullback_dist(NUFPAR *parser) {
  f32 v = NuFParGetFloat(parser);
  if (v < 0.0f)
    v = 0.0f;
  levelconfig_ldata->cam_pullback_dist = v;
}

// FUNCTION: LEGOBATMAN 0x0060e2c0
void LC_AL_cam_lateral_dist(NUFPAR *parser) {
  f32 v = NuFParGetFloat(parser);
  if (v < 0.0f)
    v = 0.0f;
  levelconfig_ldata->cam_lateral_dist = v;
}

// FUNCTION: LEGOBATMAN 0x0060e310
void LC_AL_cam_look_rot_mul(NUFPAR *parser) {
  f32 v = NuFParGetFloat(parser);
  levelconfig_ldata->cam_look_rot_mul_y = v;
  levelconfig_ldata->cam_look_rot_mul_x = v;
}

// FUNCTION: LEGOBATMAN 0x0060e3e0
void LC_AL_music(NUFPAR *parser) {
  if (NuFParGetWord(parser) != 0) {
    levelconfig_ldata->music_tracks[0][0] =
        music_man.GetTrackHandle(1, parser->word_buf);
    levelconfig_ldata->music_tracks[1][0] =
        music_man.GetTrackHandle(2, parser->word_buf);
    levelconfig_ldata->music_tracks[2][0] =
        music_man.GetTrackHandle(0x20, parser->word_buf);
  }
}

// FUNCTION: LEGOBATMAN 0x0060e460
void LC_AL_music_other(NUFPAR *parser) {
  if (NuFParGetWord(parser) != 0) {
    levelconfig_ldata->music_tracks[0][1] =
        music_man.GetTrackHandle(1, parser->word_buf);
    levelconfig_ldata->music_tracks[1][1] =
        music_man.GetTrackHandle(2, parser->word_buf);
    levelconfig_ldata->music_tracks[2][1] =
        music_man.GetTrackHandle(0x20, parser->word_buf);
  }
}

// FUNCTION: LEGOBATMAN 0x0060e4e0
void LC_AL_sfx_ambient(NUFPAR *parser) {
  if (NuFParGetWord(parser) != 0)
    levelconfig_ldata->sfx_ambient = GetSfxId(parser->word_buf);
}
