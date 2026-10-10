// batman/, file unknown: InteractiveDisplayManager (Mac: the tail of
// InteractiveDisplay.cpp), 0x6000c0..0x6009c0.

#include "../nu2api/nucore/common.h"
#include "../nu2api/nucore/nustring.h"
#include <string.h>

struct WORLDINFO_s;
struct nuvec_s;
union variptr_u;

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
  virtual f32 GetDisplayAspectRatio() const;                               // 17
  virtual void LoadSettings(char *name);                                   // 18
  virtual f32 GetTextScaleMultiplier() const;                              // 19
  virtual void UpdateGameCamera(nuvec_s &pos, nuvec_s &target);            // 20
  virtual i32 IsCameraTransitioning() const;                               // 21

  u8 pad004[0x10 - 4];
  char level_name[0x40]; // 0x010
};

// Short name so match.py finds the long method names on one line.
typedef InteractiveDisplay IDISPLAY;

class InteractiveDisplayManager {
public:
  InteractiveDisplayManager();
  static void AddDisplay(InteractiveDisplay *display);
  static void InitializeLevel(WORLDINFO_s *world);
  static void ActivateLevel(WORLDINFO_s *world);
  static void DumpLevel(WORLDINFO_s *world);
  void UpdateInputs();
  void UpdateInternal(f32 dt);
  static InteractiveDisplay *GetTransitioningDisplay();
  static InteractiveDisplay *GetCameraTarget();
  void UpdateCamera_Internal(nuvec_s &pos, nuvec_s &target);
  static InteractiveDisplay *IsDisplayOfClassInteractiveMode(char const *name);
  static i32 NumDisplaysOfClassRegistered(char const *name);
  static IDISPLAY *GetDisplayOfClass(char const *name, i32 index);
  static IDISPLAY *IsDisplayOfClassPresent(char const *name);
  static void Render(i32 scene);
  static void InitializePerm(variptr_u *buf, variptr_u *end);

  // GLOBAL: LEGOBATMAN 0x00aca638
  static InteractiveDisplayManager m_oSingleton;

  InteractiveDisplay *displays[16]; // 0x00
  i32 count;                        // 0x40
  f32 time;                         // 0x44
  f32 f48;                          // 0x48
  f32 f4c;                          // 0x4c
  f32 f50;                          // 0x50
  i32 i54;                          // 0x54
  i32 i58;                          // 0x58
  i32 level_load_render;            // 0x5c
  i32 i60;                          // 0x60
};

// GLOBAL: LEGOBATMAN 0x00ab093c
extern i32 Paused;

void Unk0067aaa0(WORLDINFO_s *world);

void NuRndrBeginScene(i32 flags);
void NuRndrEndScene();
void Unk006ef3d0(i32 cs); // Mac: NuPrimSetCoordinateSystem at this spot

// GLOBAL: LEGOBATMAN 0x029dcd40
extern i32 g_unk029dcd40; // coordinate-system stack depth
// GLOBAL: LEGOBATMAN 0x00b0bab8
extern i32 g_unk00b0bab8[];

class DynamicMaterialManager {
public:
  static void InitializePerm(variptr_u *buf, variptr_u *end);

  // GLOBAL: LEGOBATMAN 0x00ad2af8
  static DynamicMaterialManager m_oSingleton;

  void *a[0x20]; // 0x000
  void *b[0x40]; // 0x080
  void *c[0x40]; // 0x180
};

class HiresTextureManager {
public:
  static void InitializePerm(variptr_u *buf, variptr_u *end);

  // GLOBAL: LEGOBATMAN 0x00ad2d78
  static HiresTextureManager m_oSingleton;

  void *a[0x10]; // 0x00
};

// FUNCTION: LEGOBATMAN 0x006000c0
InteractiveDisplayManager::InteractiveDisplayManager() {
  time = f48 = f4c = f50 = 0.0f;
  i54 = i58 = count = level_load_render = 0;
}

// FUNCTION: LEGOBATMAN 0x006000e0
void InteractiveDisplayManager::AddDisplay(InteractiveDisplay *display) {
  m_oSingleton.level_load_render |= display->GetDoesLevelLoadRender();
  m_oSingleton.displays[m_oSingleton.count++] = display;
}

// FUNCTION: LEGOBATMAN 0x00600110
void InteractiveDisplayManager::InitializeLevel(WORLDINFO_s *world) {
  m_oSingleton.i60 = 0;
  for (i32 i = 0; i < m_oSingleton.count; i++)
    m_oSingleton.displays[i]->InitializeLevel(world);
}

// FUNCTION: LEGOBATMAN 0x00600150
void InteractiveDisplayManager::ActivateLevel(WORLDINFO_s *world) {
  for (i32 i = 0; i < m_oSingleton.count; i++)
    m_oSingleton.displays[i]->ActivateLevel(world);
}

// FUNCTION: LEGOBATMAN 0x00600180
void InteractiveDisplayManager::DumpLevel(WORLDINFO_s *world) {
  Unk0067aaa0(world);
  for (i32 i = 0; i < m_oSingleton.count; i++)
    m_oSingleton.displays[i]->DumpLevel(world);
}

// FUNCTION: LEGOBATMAN 0x00600370
void InteractiveDisplayManager::UpdateInternal(f32 dt) {
  if (Paused == 0) {
    time += dt;
    UpdateInputs();
    for (i32 i = 0; i < count; i++) {
      if (displays[i] != 0 && displays[i]->ShouldUpdate())
        displays[i]->Update(dt);
    }
  }
}

// FUNCTION: LEGOBATMAN 0x006003d0
InteractiveDisplay *InteractiveDisplayManager::GetTransitioningDisplay() {
  for (i32 i = 0; i < m_oSingleton.count; i++) {
    if (m_oSingleton.displays[i]->IsCameraTransitioning())
      return m_oSingleton.displays[i];
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x00600410
InteractiveDisplay *InteractiveDisplayManager::GetCameraTarget() {
  for (i32 i = 0; i < m_oSingleton.count; i++) {
    if (m_oSingleton.displays[i]->IsCameraTarget())
      return m_oSingleton.displays[i];
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x00600450
void InteractiveDisplayManager::UpdateCamera_Internal(nuvec_s &pos,
                                                      nuvec_s &target) {
  InteractiveDisplay *display = GetCameraTarget();
  if (display != 0)
    display->UpdateGameCamera(pos, target);
}

// clang-format off: match.py needs the name on the signature's first line.
// FUNCTION: LEGOBATMAN 0x00600470
IDISPLAY *InteractiveDisplayManager::IsDisplayOfClassInteractiveMode(
    char const *name) {
  // clang-format on
  i32 n = m_oSingleton.count;
  for (i32 i = 0; i < n; i++) {
    InteractiveDisplay *display = m_oSingleton.displays[i];
    if (display != 0 && NuStrICmp(display->GetClassNameA(), name) == 0 &&
        display->IsInteractiveMode())
      return display;
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x006004d0
i32 InteractiveDisplayManager::NumDisplaysOfClassRegistered(char const *name) {
  i32 found = 0;
  i32 n = m_oSingleton.count;
  for (i32 i = 0; i < n; i++) {
    InteractiveDisplay *display = m_oSingleton.displays[i];
    if (display != 0 && NuStrICmp(display->GetClassNameA(), name) == 0)
      found++;
  }
  return found;
}

// FUNCTION: LEGOBATMAN 0x00600530
IDISPLAY *InteractiveDisplayManager::GetDisplayOfClass(char const *name,
                                                       i32 index) {
  i32 found = 0;
  i32 n = m_oSingleton.count;
  for (i32 i = 0; i < n; i++) {
    InteractiveDisplay *display = m_oSingleton.displays[i];
    if (display != 0 && NuStrICmp(display->GetClassNameA(), name) == 0) {
      if (found == index)
        return display;
      found++;
    }
  }
  return 0;
}

// GLOBAL: LEGOBATMAN 0x00960894
extern WORLDINFO_s *WORLD;

// FUNCTION: LEGOBATMAN 0x00600590
IDISPLAY *InteractiveDisplayManager::IsDisplayOfClassPresent(char const *name) {
  if (WORLD == 0)
    return 0;
  i32 n = m_oSingleton.count;
  for (i32 i = 0; i < n; i++) {
    InteractiveDisplay *display = m_oSingleton.displays[i];
    if (display != 0 && NuStrICmp(display->GetClassNameA(), name) == 0 &&
        NuStrICmp(display->level_name, (char *)WORLD) == 0)
      return display;
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x00600600
void InteractiveDisplayManager::Render(i32 scene) {
  if (scene)
    NuRndrBeginScene(1);
  g_unk029dcd40++;
  Unk006ef3d0(1);
  for (i32 i = 0; i < m_oSingleton.count; i++) {
    if (m_oSingleton.displays[i] != 0 &&
        ((Paused == 0 && m_oSingleton.i60 == 0) ||
         m_oSingleton.displays[i]->RenderWhenPaused()) &&
        m_oSingleton.displays[i]->IsVisible())
      m_oSingleton.displays[i]->Render();
  }
  g_unk029dcd40--;
  Unk006ef3d0(g_unk00b0bab8[g_unk029dcd40]);
  if (scene)
    NuRndrEndScene();
  m_oSingleton.i60 = Paused;
}

// FUNCTION: LEGOBATMAN 0x006006d0
void DynamicMaterialManager::InitializePerm(variptr_u *buf, variptr_u *end) {
  memset(m_oSingleton.a, 0, sizeof(m_oSingleton.a));
  memset(m_oSingleton.b, 0, sizeof(m_oSingleton.b));
  memset(m_oSingleton.c, 0, sizeof(m_oSingleton.c));
}

// FUNCTION: LEGOBATMAN 0x00600710
void HiresTextureManager::InitializePerm(variptr_u *buf, variptr_u *end) {
  memset(m_oSingleton.a, 0, sizeof(m_oSingleton.a));
}

// FUNCTION: LEGOBATMAN 0x00600730
void InteractiveDisplayManager::InitializePerm(variptr_u *buf, variptr_u *end) {
  m_oSingleton.i60 = 0;
  HiresTextureManager::InitializePerm(buf, end);
  DynamicMaterialManager::InitializePerm(buf, end);
}
