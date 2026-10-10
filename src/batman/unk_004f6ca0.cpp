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
  virtual i32 Vfn12() const;                                               // 12
  virtual void Vfn13();                                                    // 13
  virtual i32 RenderWhenPaused() const;                                    // 14
  virtual i32 IsCameraTarget() const;                                      // 15
  virtual i32 IsInteractiveMode() const;                                   // 16
  virtual void Vfn17();                                                    // 17
  virtual void LoadSettings(char *name);                                   // 18
  virtual f32 GetTextScaleMultiplier() const;                              // 19
  virtual void Vfn20();                                                    // 20
  virtual i32 IsCameraTransitioning() const;                               // 21

  u8 pad004[0x338 - 4];
  f32 transition; // 0x338
};

struct WorldMapBase {
  const char *GetClassNameA() const;
};

struct SecurityCamera {
  const char *GetClassNameA() const;
};

struct LightFlickerOverlay {
  const char *GetClassNameA() const;
};

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

// FUNCTION: LEGOBATMAN 0x004f73f0
const char *SecurityCamera::GetClassNameA() const { return "SecurityCamera"; }

// FUNCTION: LEGOBATMAN 0x004f74c0
const char *LightFlickerOverlay::GetClassNameA() const {
  return "LightFlickerOverlay";
}
