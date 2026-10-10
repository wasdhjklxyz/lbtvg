// batman/worldmap_unk.cpp: WorldMap (Mac WorldMap::*); file name unproven.

#include "../nu2api/nucore/common.h"
#include "../nu2api/nucore/nustring.h"

struct WORLDINFO_s;

extern "C" int NuSPrintf(char *buf, char *fmt, ...);
char *NuStrIStr(char *str, const char *sub);

struct nugscn_s;

// Raw view of numtl_s: only the field WorldMap resets.
struct WMMTL_s {
  u8 pad00[0x74];
  u16 u74; // 0x74
};

void NuMtlUpdate(WMMTL_s *mtl);
void NuGScnRemove(nugscn_s *scene);

// Mac WorldMapLocation, see src/batman/unk_00679840.cpp.
struct WMLOCATION_s {
  u8 pad00[0x20];
  void *area; // 0x20
  u8 pad24[0x30 - 0x24];
  i32 open;     // 0x30
  i32 complete; // 0x34
};

// 0x14 bytes each; only the location pointer is evidenced.
struct WMBEACON_s {
  WMLOCATION_s *location; // 0x00
  u8 pad04[0x14 - 4];
};

// Raw views for UpdatePlayerCharacterIcons.
struct WMCHARMODEL_s {
  u8 pad00[0xc];
  char *file; // 0x0c
  u8 pad10[0x16 - 0x10];
  i16 icon; // 0x16
};

struct WMPLAYER_s {
  u8 pad00[0x54];
  WMCHARMODEL_s *p54; // 0x54
};

struct WMCDATA_s {
  u8 pad00[0x16];
  i16 icon; // 0x16
  u8 pad18[0x48 - 0x18];
};

// GLOBAL: LEGOBATMAN 0x00ab3960
extern WMPLAYER_s *g_unk00ab3960[2];
// GLOBAL: LEGOBATMAN 0x0096067c
extern i32 PlayerID[2];
// GLOBAL: LEGOBATMAN 0x00acb81c
extern WMCDATA_s *CDataList;
// GLOBAL: LEGOBATMAN 0x009c5870
extern i32 g_unk009c5870;
// GLOBAL: LEGOBATMAN 0x00acb6c8
extern i16 *g_unk00acb6c8;
// GLOBAL: LEGOBATMAN 0x00945e54
extern char *g_unk00945e54[]; // per-episode cutscene names

i32 GetMenuID();
i32 MenuHasParentWithId(i32 id);
void Hub_SetLevelSelectedCutscene(char *name);

class WorldMapBase {
public:
  void DumpLevel(WORLDINFO_s *world);
};

struct numtl_s;
union variptr_u;
struct nuvec_s;

// Slot order from the PC vtable at 0x0085d75c (src/batman/unk_004f6ca0.cpp).
class InteractiveDisplay {
public:
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

  void RenderStrobePattern();
  void RenderInterlacePattern();
  void RenderOverlay();
  void RenderBackground(u8 r, u8 g, u8 b, numtl_s *mtl);
};

class WorldMap : public InteractiveDisplay {
public:
  char *GetPointerSpecialName(WORLDINFO_s *world, int pointer) const;
  void ActivateLevel(WORLDINFO_s *world);
  void DumpLevel(WORLDINFO_s *world);
  void RenderLocations();
  void RenderFadeLayer();
  void RenderSelectedLocationInfo();
  void RenderPointer();
  void Render();
  void UpdateOverallMode();
  i32 IsBeaconVisible(int index) const;
  void ToggleInteractive(int on, int instant); // WorldMapBase

  u8 pad004[0x10 - 4];
  char level_name[0x40]; // 0x010, WorldMapBase
  u8 pad050[0x22c - 0x50];
  f32 depth; // 0x22c, InteractiveDisplay render depth
  u8 pad230[0x338 - 0x230];
  f32 f338; // 0x338
  u8 pad33c[0x344 - 0x33c];
  i32 episode_id; // 0x344, WorldMapBase
  i32 mode;       // 0x348, WorldMapBase
  u8 pad34c[0x360 - 0x34c];
  i32 current_location; // 0x360, WorldMapBase
  u8 pad364[0x980 - 0x364];
  WMBEACON_s *beacons; // 0x980
  u8 pad984[0x98c - 0x984];
  f32 location_depth; // 0x98c
  i32 i990;           // 0x990
  i32 i994;           // 0x994
  WMMTL_s *mtl998;    // 0x998
  WMMTL_s *mtl99c;    // 0x99c
  WMMTL_s *mtl9a0;    // 0x9a0
  WMMTL_s *mtl9a4;    // 0x9a4
  u8 pad9a8[0x9bc - 0x9a8];
  i32 i9bc;        // 0x9bc
  nugscn_s *scene; // 0x9c0
};

// GLOBAL: LEGOBATMAN 0x009cf69c
extern char g_unk009cf69c[]; // shared name buffer

// FUNCTION: LEGOBATMAN 0x00518880
void WorldMap::ActivateLevel(WORLDINFO_s *world) {
  if (NuStrICmp((char *)world, level_name) == 0)
    i9bc = 0;
}

// FUNCTION: LEGOBATMAN 0x005188b0
void WorldMap::DumpLevel(WORLDINFO_s *world) {
  ((WorldMapBase *)this)->DumpLevel(world);
  if (NuStrICmp((char *)world, level_name) == 0) {
    mode = 0;
    i990 = 0;
    if (i994 != 0)
      i994 = 0;
    if (mtl998 != 0) {
      mtl998->u74 = 0;
      NuMtlUpdate(mtl998);
    }
    if (mtl99c != 0) {
      mtl99c->u74 = 0;
      NuMtlUpdate(mtl99c);
    }
    if (mtl9a0 != 0) {
      mtl9a0->u74 = 0;
      NuMtlUpdate(mtl9a0);
    }
    if (mtl9a4 != 0) {
      mtl9a4->u74 = 0;
      NuMtlUpdate(mtl9a4);
    }
    if (scene != 0) {
      NuGScnRemove(scene);
      scene = 0;
    }
  }
}

// FUNCTION: LEGOBATMAN 0x005189c0
void WorldMap::UpdateOverallMode() {
  if (g_unk009c5870 == 0 && GetMenuID() == 0x10 &&
      *g_unk00acb6c8 == episode_id) {
    if (!IsInteractiveMode()) {
      Hub_SetLevelSelectedCutscene(g_unk00945e54[episode_id]);
      ToggleInteractive(1, 0);
    }
    return;
  }
  if (MenuHasParentWithId(0x10) && *g_unk00acb6c8 == episode_id) {
    i990 = 1;
    mode = 2;
    return;
  }
  if (mode != 0)
    f338 = 0.0f;
  mode = 0;
  i990 = 0;
}

// FUNCTION: LEGOBATMAN 0x00518aa0
void UpdatePlayerCharacterIcons() {
  for (i32 i = 0; i < 2; i++) {
    if (g_unk00ab3960[i] && g_unk00ab3960[i]->p54 && PlayerID[i] >= 0 &&
        CDataList && NuStrIStr(g_unk00ab3960[i]->p54->file, "dummychar"))
      g_unk00ab3960[i]->p54->icon = CDataList[PlayerID[i]].icon;
  }
}

// STUB: LEGOBATMAN 0x00518b20
// orig computes the element address (lea) then loads [eax]; ours folds it
i32 WorldMap::IsBeaconVisible(int index) const {
  WMLOCATION_s *loc = beacons[index].location;
  if (loc->area == 0)
    return 0;
  if (index == current_location)
    return 0;
  if (loc->open || loc->complete)
    return 1;
  return 0;
}

// FUNCTION: LEGOBATMAN 0x00519370
char *WorldMap::GetPointerSpecialName(WORLDINFO_s *world, int pointer) const {
  char *suffix = "A";
  if (pointer == 1)
    suffix = "B";
  else if (pointer == 2)
    suffix = "C";
  if (NuStrIStr((char *)world, "batcave_f") != 0)
    NuSPrintf(g_unk009cf69c, "villain_pointer_%s", suffix);
  else
    NuSPrintf(g_unk009cf69c, "hero_pointer_%s", suffix);
  return g_unk009cf69c;
}

// FUNCTION: LEGOBATMAN 0x0051c580
void WorldMap::Render() {
  depth = 0.0f;
  depth += 0.05f;
  RenderBackground(0x66, 0x66, 0x66, 0);
  depth += 0.05f;
  RenderLocations();
  location_depth = depth;
  depth += 0.05f;
  RenderSelectedLocationInfo();
  depth += 0.05f;
  RenderPointer();
  depth += 0.05f;
  RenderFadeLayer();
  depth += 0.05f;
  RenderInterlacePattern();
  depth += 0.05f;
  RenderStrobePattern();
  depth += 0.05f;
  RenderOverlay();
}
