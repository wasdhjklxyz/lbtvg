// batman/worldmap_unk.cpp: WorldMap (Mac WorldMap::*); file name unproven.

#include "../nu2api/nucore/common.h"
#include "../nu2api/nucore/nustring.h"
#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"
#include "worldinfo_unk.h"

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x00518430
static f32 NuVecMagInline(f32 *v);

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x00518250
static f32 NuFabs(f32 f);
// FUNCTION: LEGOBATMAN 0x00518270
static f32 NuSinApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x00518330
static void NuVec4Copy(f32 *dst, f32 *src);
// FUNCTION: LEGOBATMAN 0x00518350
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);
// FUNCTION: LEGOBATMAN 0x005183e0
static void NuVecScaleInline(f32 *dst, f32 *src, f32 s);

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
  u8 pad38[0x50 - 0x38];
};

// 0x14 bytes each; only the location pointer is evidenced.
struct WMBEACON_s {
  WMLOCATION_s *location; // 0x00
  f32 f4;                 // 0x04, ring position
  f32 f8;                 // 0x08
  f32 fc;                 // 0x0c
  i32 i10;                // 0x10
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
  void InitializePerm(char *name, variptr_u *buf, variptr_u *end);
  void InitializeLevel(WORLDINFO_s *world);
  void DumpLevel(WORLDINFO_s *world);
};

float NuRandFloat(void);

class DynamicMaterialManager {
public:
  void *GetMaterial(char const *a, char const *b, int c);
};

// GLOBAL: LEGOBATMAN 0x00ad2af8
extern DynamicMaterialManager g_dynamicMaterialManager;

struct nuhspecial_s {
  void *scene;
  void *special;
  void *display_special;
};

i32 NuSpecialExistsFn(nuhspecial_s *sp);
extern "C" WMMTL_s *NuSpecialGetMtl(nuhspecial_s *special, int index);
extern "C" i32 NuMtlSetCurrentRenderPlane(i32 render_plane);

class HiresTextureManager {
public:
  i32 GetTexture_Internal(char *name, WORLDINFO_s *world);

  // GLOBAL: LEGOBATMAN 0x00ad2d78
  static HiresTextureManager m_oSingleton;
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

  static void *GetFirstSpecialMaterial(nugscn_s *scene, char *name);
  void RenderStrobePattern();
  void RenderInterlacePattern();
  void RenderOverlay();
  void RenderBackground(u8 r, u8 g, u8 b, numtl_s *mtl);
};

class WorldMap : public InteractiveDisplay {
public:
  char *GetPointerSpecialName(WORLDINFO_s *world, int pointer) const;
  void InitializePerm(char *name, variptr_u *buf, variptr_u *end);
  void InitializeLevel(WORLDINFO_s *world);
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
  u8 pad364[0x370 - 0x364];
  WMLOCATION_s locations[0x10]; // 0x370, 0x50 each (count unproven)
  i32 location_count;           // 0x870
  u8 pad874[0x958 - 0x874];
  nugscn_s *scene958; // 0x958
  u8 pad95c[0x980 - 0x95c];
  WMBEACON_s *beacons;  // 0x980
  f32 f984;             // 0x984
  f32 f988;             // 0x988
  f32 location_depth;   // 0x98c
  i32 i990;             // 0x990
  WMMTL_s *mtl994;      // 0x994, circle_a
  WMMTL_s *mtl998;      // 0x998
  WMMTL_s *mtl99c;      // 0x99c
  WMMTL_s *mtl9a0;      // 0x9a0
  WMMTL_s *mtl9a4;      // 0x9a4
  f32 f9a8;             // 0x9a8
  f32 f9ac;             // 0x9ac
  WMLOCATION_s *loc9b0; // 0x9b0
  i32 i9b4;             // 0x9b4
  f32 f9b8;             // 0x9b8
  i32 i9bc;             // 0x9bc
  nugscn_s *scene;      // 0x9c0
  void *icons[8];       // 0x9c4
};

// GLOBAL: LEGOBATMAN 0x009cf69c
extern char g_unk009cf69c[]; // shared name buffer

// FUNCTION: LEGOBATMAN 0x0051b2b0
void WorldMap::InitializeLevel(WORLDINFO_s *world) {
  ((WorldMapBase *)this)->InitializeLevel(world);
  if (NuStrICmp((char *)world, level_name) == 0) {
    nuhspecial_s sp;
    mtl994 = 0;
    NuSpecialFind(scene958, &sp, "circle_a", 0);
    if (NuSpecialExistsFn(&sp))
      mtl994 = NuSpecialGetMtl(&sp, 0);
    if (mtl998 != 0) {
      mtl998->u74 = HiresTextureManager::m_oSingleton.GetTexture_Internal(
          "stuff/interactivedisplay/worldmap/map_locations/map_location_outer",
          world);
      NuMtlUpdate(mtl998);
    }
    if (mtl99c != 0) {
      mtl99c->u74 = HiresTextureManager::m_oSingleton.GetTexture_Internal(
          "stuff/interactivedisplay/worldmap/map_locations/map_location_inner",
          world);
      NuMtlUpdate(mtl99c);
    }
    if (mtl9a0 != 0) {
      mtl9a0->u74 = HiresTextureManager::m_oSingleton.GetTexture_Internal(
          "stuff/interactivedisplay/worldmap/map_icons/rescue_no", world);
      NuMtlUpdate(mtl9a0);
    }
    if (mtl9a4 != 0) {
      mtl9a4->u74 = HiresTextureManager::m_oSingleton.GetTexture_Internal(
          "stuff/interactivedisplay/worldmap/map_icons/rescue_yes", world);
      NuMtlUpdate(mtl9a4);
    }
    i32 plane = NuMtlSetCurrentRenderPlane(3);
    scene = NuGScnRead(
        &world->buf104, world->bufEnd108,
        "stuff\\interactivedisplay\\worldmap\\map_icons\\map_icons.gsc");
    NuMtlSetCurrentRenderPlane(plane);
    char *names[8] = {"minkit_OFF",   "minikit_ON",   "bar_OFF",
                      "bar_ON",       "truehero_OFF", "truehero_ON",
                      "redBrick_OFF", "redBrick_ON"};
    if (scene != 0) {
      for (i32 i = 0; i < 8; i++)
        icons[i] = GetFirstSpecialMaterial(scene, names[i]);
    }
    f9ac = 0.0f;
    loc9b0 = &locations[1];
    i9bc = 0;
    f9b8 = 1.0f;
    f9a8 = 0.0f;
  }
}

// FUNCTION: LEGOBATMAN 0x0051b0e0
void WorldMap::InitializePerm(char *name, variptr_u *buf, variptr_u *end) {
  ((WorldMapBase *)this)->InitializePerm(name, buf, end);
  i32 count = location_count;
  beacons = (WMBEACON_s *)buf->void_ptr;
  buf->addr += count * sizeof(WMBEACON_s);
  for (i32 i = 0; i < count; i++) {
    f32 base = (f32)i / (f32)count;
    f32 pos = base + (NuRandFloat() - 0.5f) / (f32)count;
    WMBEACON_s *beacon = &beacons[i];
    beacon->location = &locations[i];
    beacon->i10 = 0;
    beacon->fc = 0.0f;
    beacon->f8 = 0.0f;
    beacon->f4 = pos;
  }
  f984 = 1.0f;
  f988 = 0.0f;
  location_depth = 0.0f;
  mtl994 = 0;
  mtl998 = (WMMTL_s *)g_dynamicMaterialManager.GetMaterial(
      "stuff/interactivedisplay/worldmap/map_locations/map_location_outer",
      level_name, 2);
  mtl99c = (WMMTL_s *)g_dynamicMaterialManager.GetMaterial(
      "stuff/interactivedisplay/worldmap/map_locations/map_location_inner",
      level_name, 2);
  mtl9a0 = (WMMTL_s *)g_dynamicMaterialManager.GetMaterial(
      "stuff/interactivedisplay/worldmap/map_icons/rescue_no", level_name, 3);
  mtl9a4 = (WMMTL_s *)g_dynamicMaterialManager.GetMaterial(
      "stuff/interactivedisplay/worldmap/map_icons/rescue_yes", level_name, 3);
  scene = 0;
  i9b4 = 0;
  f9ac = 0.0f;
  loc9b0 = &locations[1];
  f9b8 = 1.0f;
  f9a8 = 0.0f;
  i9bc = 0;
}

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
    if (mtl994 != 0)
      mtl994 = 0;
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

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_worldmap_unk(f32 *v, f32 a, i32 i) {
  v[2] = NuFabs(a);
  v[0] = NuSinApprox(i);
  NuVec4Copy(v + 4, v);
  NuVec4Set(v, a, a, a, a);
  NuVecScaleInline(v + 8, v, a);
}

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_2_worldmap_unk(f32 *v, f32 a, i32 i) {
  v[48] = NuVecMagInline(v);
}
