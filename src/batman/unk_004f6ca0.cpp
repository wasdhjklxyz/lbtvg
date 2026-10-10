// batman/, file unknown: inline virtuals of the level-object classes,
// emitted between 0x4f6c80 and 0x4f7530 (before LoadPerm2).

#include "../nu2api/nucore/common.h"

// The Mac names end in A: windows.h maps GetClassName to GetClassNameA.

struct WORLDINFO_s;
union variptr_u;

// Slot order from the PC vtable at 0x0085d75c; names of the inline ones from
// the Mac emission order (ActivateLevel, GetDoesLevelLoadRender,
// GetUsesStrobePattern, GetUsesWhiteNoise, GetUsesInterlacePattern,
// GetUsesOverlayTexture, RenderWhenPaused, IsCameraTarget, IsInteractiveMode,
// LoadSettings, GetTextScaleMultiplier, IsCameraTransitioning), which the PC
// emission order follows.
class InteractiveDisplay {
public:
  f32 Unk005c6270() const;

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
  virtual void Vfn10();                                                    // 10
  virtual void Vfn11();                                                    // 11
  virtual i32 IsVisible() const;                                           // 12
  virtual i32 ShouldUpdate() const;                                        // 13
  virtual i32 RenderWhenPaused() const;                                    // 14
  virtual i32 IsCameraTarget() const;                                      // 15
  virtual i32 IsInteractiveMode() const;                                   // 16
  virtual void Vfn17();                                                    // 17
  virtual void LoadSettings(char *name);                                   // 18
  virtual f32 GetTextScaleMultiplier() const;                              // 19
  virtual void Vfn20();                                                    // 20
  virtual i32 IsCameraTransitioning() const;                               // 21

  u8 pad004[0x10 - 4];
  char level_name[0x338 - 0x10]; // 0x010
  f32 transition;                // 0x338
};

// vtable 0x0085d7e4: overrides slots 8 and 16, adds 22..24.
class WorldMapBase : public InteractiveDisplay {
public:
  virtual const char *GetClassNameA() const;
  virtual i32 GetUsesInterlacePattern() const;
  virtual i32 IsInteractiveMode() const;
  virtual char *GetPointerSpecialName(WORLDINFO_s *world, i32 pointer) const;
  virtual f32 GetPointerRadiusWithMaxScale() const;
  virtual f32 GetPointerRadiusWithScale() const;

  u8 pad33c[0x348 - 0x33c];
  i32 mode; // 0x348, 1 = interactive, 2 = playback
  u8 pad34c[0x888 - 0x34c];
  f32 pointer_radius; // 0x888
};

// vtable 0x0085d874.
class WorldMap : public WorldMapBase {
public:
  virtual i32 GetDoesLevelLoadRender() const;
  virtual const char *GetClassNameA() const;
  virtual i32 GetUsesInterlacePattern() const;
  virtual i32 GetUsesOverlayTexture() const;
  virtual i32 GetUsesWhiteNoise() const;
  virtual i32 IsCameraTransitioning() const;
  virtual i32 IsCameraTarget() const;
  virtual f32 GetPointerRadiusWithMaxScale() const;
  virtual f32 GetPointerRadiusWithScale() const;
  virtual f32 GetTextScaleMultiplier() const;

  u8 pad88c[0x984 - 0x88c];
  f32 pointer_scale; // 0x984
  u8 pad988[0x990 - 0x988];
  i32 i990; // 0x990
};

// vtable 0x0085d96c.
class SecurityCamera : public InteractiveDisplay {
public:
  virtual const char *GetClassNameA() const;
  virtual i32 GetUsesInterlacePattern() const;
  virtual i32 GetUsesOverlayTexture() const;
  virtual i32 GetUsesWhiteNoise() const;
  virtual i32 IsCameraTarget() const;
  virtual i32 RenderWhenPaused() const;

  u8 pad33c[0x3ec - 0x33c];
  i32 kind; // 0x3ec
};

// vtable 0x0085d8f4: Mac inline order IsVillainMode, IsInteractiveMode,
// GetClassNameA, GetUsesWhiteNoise, GetUsesInterlacePattern,
// GetUsesOverlayTexture, (GetHighlightedMenuItemId), RenderWhenPaused,
// IsCameraTransitioning.
class ShopComputer : public InteractiveDisplay {
public:
  virtual const char *GetClassNameA() const;
  virtual i32 GetUsesWhiteNoise() const;
  virtual i32 GetUsesInterlacePattern() const;
  virtual i32 GetUsesOverlayTexture() const;
  virtual i32 RenderWhenPaused() const;
  virtual i32 IsInteractiveMode() const;
  virtual i32 IsCameraTransitioning() const;
  virtual i32 IsVillainMode() const; // 22

  u8 pad33c[0x340 - 0x33c];
  i32 villain_mode; // 0x340
};

i32 GetMenuID();

// GLOBAL: LEGOBATMAN 0x009cf570
extern i32 g_unk009cf570; // security camera type the camera follows

char *NuStrIStr(char *str, const char *sub);

// vtable 0x0085d9f4: overrides slots 12 and 13 (Mac inline order
// GetClassNameA, IsVisible, ShouldUpdate).
class LightFlickerOverlay : public InteractiveDisplay {
public:
  virtual const char *GetClassNameA() const;
  virtual i32 IsVisible() const;
  virtual i32 ShouldUpdate() const;
};

// Raw view of WORLD->area (0x130) and its flags byte (0x7c).
struct LFO_AREADATA {
  u8 pad[0x7c];
  u8 flags; // 0x7c, 0x40 = lights flicker
};
struct LFO_WORLD {
  u8 pad[0x130];
  LFO_AREADATA *area; // 0x130
};

extern WORLDINFO_s *WORLD; // 0x00960894

// FUNCTION: LEGOBATMAN 0x004f6c80
void InteractiveDisplay::ActivateLevel(WORLDINFO_s *world) {}

// FUNCTION: LEGOBATMAN 0x004f6c90
i32 InteractiveDisplay::GetDoesLevelLoadRender() const { return 0; }

// FUNCTION: LEGOBATMAN 0x004f6ca0
const char *InteractiveDisplay::GetClassNameA() const {
  return "InteractiveDisplay";
}

// FUNCTION: LEGOBATMAN 0x004f6cb0
i32 InteractiveDisplay::GetUsesStrobePattern() const { return 1; }

// FUNCTION: LEGOBATMAN 0x004f6cc0
i32 InteractiveDisplay::GetUsesWhiteNoise() const { return 0; }

// FUNCTION: LEGOBATMAN 0x004f6cd0
i32 InteractiveDisplay::GetUsesInterlacePattern() const { return 0; }

// FUNCTION: LEGOBATMAN 0x004f6ce0
i32 InteractiveDisplay::GetUsesOverlayTexture() const { return 0; }

// FUNCTION: LEGOBATMAN 0x004f6cf0
i32 InteractiveDisplay::RenderWhenPaused() const { return 0; }

// FUNCTION: LEGOBATMAN 0x004f6d00
i32 InteractiveDisplay::IsCameraTarget() const {
  if (IsInteractiveMode() || IsCameraTransitioning())
    return 1;
  return 0;
}

// FUNCTION: LEGOBATMAN 0x004f6d30
i32 InteractiveDisplay::IsInteractiveMode() const { return 0; }

// FUNCTION: LEGOBATMAN 0x004f6e60
void InteractiveDisplay::LoadSettings(char *name) {}

// FUNCTION: LEGOBATMAN 0x004f6e70
f32 InteractiveDisplay::GetTextScaleMultiplier() const { return 4.5f; }

// FUNCTION: LEGOBATMAN 0x004f6e80
i32 InteractiveDisplay::IsCameraTransitioning() const {
  if (transition < 1.0f)
    return 1;
  return 0;
}

// FUNCTION: LEGOBATMAN 0x004f6f70
const char *WorldMapBase::GetClassNameA() const { return "WorldMapBase"; }

// FUNCTION: LEGOBATMAN 0x004f6f80
i32 WorldMapBase::GetUsesInterlacePattern() const { return 0; }

// FUNCTION: LEGOBATMAN 0x004f6f90
i32 WorldMapBase::IsInteractiveMode() const { return mode == 1; }

// FUNCTION: LEGOBATMAN 0x004f6fa0
char *WorldMapBase::GetPointerSpecialName(WORLDINFO_s *world,
                                          i32 pointer) const {
  return "";
}

// FUNCTION: LEGOBATMAN 0x004f6fc0
f32 WorldMapBase::GetPointerRadiusWithMaxScale() const {
  return pointer_radius;
}

// FUNCTION: LEGOBATMAN 0x004f6fd0
f32 WorldMapBase::GetPointerRadiusWithScale() const { return pointer_radius; }

// FUNCTION: LEGOBATMAN 0x004f7020
i32 WorldMap::GetDoesLevelLoadRender() const { return 0; }

// FUNCTION: LEGOBATMAN 0x004f7030
const char *WorldMap::GetClassNameA() const { return "WorldMap"; }

// FUNCTION: LEGOBATMAN 0x004f7040
i32 WorldMap::GetUsesInterlacePattern() const { return 1; }

// FUNCTION: LEGOBATMAN 0x004f7050
i32 WorldMap::GetUsesOverlayTexture() const { return 1; }

// FUNCTION: LEGOBATMAN 0x004f7060
i32 WorldMap::GetUsesWhiteNoise() const { return 1; }

// FUNCTION: LEGOBATMAN 0x004f7080
i32 WorldMap::IsCameraTransitioning() const {
  if (transition < 1.0f && i990 == 0)
    return 1;
  return 0;
}

// FUNCTION: LEGOBATMAN 0x004f70b0
i32 WorldMap::IsCameraTarget() const {
  if (IsInteractiveMode() || IsCameraTransitioning() || mode == 2)
    return 1;
  return 0;
}

// FUNCTION: LEGOBATMAN 0x004f70f0
f32 WorldMap::GetPointerRadiusWithMaxScale() const {
  return pointer_radius * 1.5f;
}

// FUNCTION: LEGOBATMAN 0x004f7110
f32 WorldMap::GetPointerRadiusWithScale() const {
  return pointer_scale * pointer_radius;
}

// FUNCTION: LEGOBATMAN 0x004f7130
f32 WorldMap::GetTextScaleMultiplier() const {
  return NuStrIStr((char *)level_name, "batcave_f") ? 2.75f : 4.5f;
}

// FUNCTION: LEGOBATMAN 0x004f72e0
i32 ShopComputer::IsVillainMode() const { return villain_mode; }

// FUNCTION: LEGOBATMAN 0x004f72f0
i32 ShopComputer::IsInteractiveMode() const {
  if (GetMenuID() == 13 && Unk005c6270() < 3.0f)
    return 1;
  return 0;
}

// FUNCTION: LEGOBATMAN 0x004f7330
const char *ShopComputer::GetClassNameA() const { return "ShopComputer"; }

// FUNCTION: LEGOBATMAN 0x004f7340
i32 ShopComputer::GetUsesWhiteNoise() const { return 1; }

// FUNCTION: LEGOBATMAN 0x004f7350
i32 ShopComputer::GetUsesInterlacePattern() const { return 1; }

// FUNCTION: LEGOBATMAN 0x004f7360
i32 ShopComputer::GetUsesOverlayTexture() const { return 1; }

// FUNCTION: LEGOBATMAN 0x004f7370
i32 ShopComputer::RenderWhenPaused() const { return 1; }

// FUNCTION: LEGOBATMAN 0x004f7380
i32 ShopComputer::IsCameraTransitioning() const { return 0; }

// FUNCTION: LEGOBATMAN 0x004f73f0
const char *SecurityCamera::GetClassNameA() const { return "SecurityCamera"; }

// FUNCTION: LEGOBATMAN 0x004f7400
i32 SecurityCamera::GetUsesInterlacePattern() const { return 1; }

// FUNCTION: LEGOBATMAN 0x004f7410
i32 SecurityCamera::GetUsesOverlayTexture() const { return 1; }

// FUNCTION: LEGOBATMAN 0x004f7420
i32 SecurityCamera::GetUsesWhiteNoise() const { return 1; }

// FUNCTION: LEGOBATMAN 0x004f7430
i32 SecurityCamera::IsCameraTarget() const { return g_unk009cf570 == kind; }

// FUNCTION: LEGOBATMAN 0x004f7450
i32 SecurityCamera::RenderWhenPaused() const { return 1; }

// FUNCTION: LEGOBATMAN 0x004f74c0
const char *LightFlickerOverlay::GetClassNameA() const {
  return "LightFlickerOverlay";
}

// FUNCTION: LEGOBATMAN 0x004f7500
i32 LightFlickerOverlay::IsVisible() const {
  if (WORLD != 0 && ((LFO_WORLD *)WORLD)->area != 0 &&
      (((LFO_WORLD *)WORLD)->area->flags & 0x40))
    return 1;
  return 0;
}

// FUNCTION: LEGOBATMAN 0x004f7530
i32 LightFlickerOverlay::ShouldUpdate() const {
  if (WORLD != 0 && ((LFO_WORLD *)WORLD)->area != 0 &&
      (((LFO_WORLD *)WORLD)->area->flags & 0x40))
    return 1;
  return 0;
}
