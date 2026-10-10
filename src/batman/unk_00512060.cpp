// batman/, file unknown: LightFlickerOverlay's out-of-line members
// (Mac: InitializePerm, Render, Update) and LoadScreenDisplay's,
// 0x512060..0x512620; the Mac keeps both classes in one file too.

#include "../nu2api/nucore/common.h"
#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x00511ea0
static f32 NuFabs(f32 f);
// FUNCTION: LEGOBATMAN 0x00511ec0
static f32 NuSinApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x00511f80
static f32 NuCosApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x00511fa0
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);
// FUNCTION: LEGOBATMAN 0x00512000
static void NuVecScaleInline(f32 *dst, f32 *src, f32 s);

struct WORLDINFO_s;
union variptr_u;

typedef struct numtl_s {
  u8 pad0[0x40];
  u32 filter_mode : 4; // 0x40
  u32 alpha_mode : 2;
  u32 attrib6 : 2;
  u32 attrib8 : 2;
  u32 attrib10 : 2;
  u32 attrib12 : 2;
  u32 z_mode : 2;
  u32 attrib16 : 2;
  u32 attrib18 : 1;
  u32 attrib19 : 13;
  u8 pad44[0x54 - 0x44];
  f32 r; // 0x54
  f32 g; // 0x58
  f32 b; // 0x5c
  u8 pad60[0x74 - 0x60];
  u16 tid; // 0x74
} NUMTL;

NUMTL *NuMtlCreateEx3D(i32 count, i32 flags);
void NuMtlUpdate(NUMTL *mtl);

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

  void RenderBackground(u8 r, u8 g, u8 b, NUMTL *mtl);

  u8 pad004[0x22c - 4];
  f32 f22c; // 0x22c
  u8 pad230[0x234 - 0x230];
  f32 f234; // 0x234
  u8 pad238[0x33c - 0x238];
};

// vtable 0x0085d9f4.
class LightFlickerOverlay : public InteractiveDisplay {
public:
  void Unk00512060(f32 value);
  void Unk00512070(f32 value);
  virtual void InitializePerm(char *name, variptr_u *buf, variptr_u *end);
  virtual void Render();

  u8 pad33c[0x340 - 0x33c];
  f32 f340; // 0x340
  f32 f344; // 0x344
  u8 pad348[0x34c - 0x348];
  f32 f34c;   // 0x34c
  NUMTL *mtl; // 0x350
};

// FUNCTION: LEGOBATMAN 0x00512060
void LightFlickerOverlay::Unk00512060(f32 value) { f22c = value; }

// FUNCTION: LEGOBATMAN 0x00512070
void LightFlickerOverlay::Unk00512070(f32 value) { f234 = value; }

// FUNCTION: LEGOBATMAN 0x00512080
void LightFlickerOverlay::InitializePerm(char *name, variptr_u *buf,
                                         variptr_u *end) {
  InteractiveDisplay::InitializePerm(name, buf, end);
  f34c = 0.0f;
  f344 = 0.0f;
  f340 = 0.0f;
  mtl = NuMtlCreateEx3D(1, 0);
  mtl->r = 0.0f;
  mtl->g = 0.0f;
  mtl->b = 0.0f;
  mtl->z_mode = 3;
  mtl->filter_mode = 1;
  mtl->alpha_mode = 2;
  mtl->tid = 0;
  NuMtlUpdate(mtl);
}

// FUNCTION: LEGOBATMAN 0x00512500
void LightFlickerOverlay::Render() {
  f22c = 0.0f;
  RenderBackground(0, 0, 0, mtl);
}

// Mac: LoadScreenDisplay (same file on the Mac, right after
// LightFlickerOverlay).
class LoadScreenDisplay : public InteractiveDisplay {
public:
  void Unk00512560();
  virtual void InitializePerm(char *name, variptr_u *buf, variptr_u *end);
  virtual void Update(f32 dt);
  virtual void Render();

  // GLOBAL: LEGOBATMAN 0x009cf56c
  static i32 m_bForceAllVisible;
};

// FUNCTION: LEGOBATMAN 0x00512560
void LoadScreenDisplay::Unk00512560() { f22c += 0.05f; }

// FUNCTION: LEGOBATMAN 0x00512580
void LoadScreenDisplay::InitializePerm(char *name, variptr_u *buf,
                                       variptr_u *end) {
  LoadSettings(name);
  InteractiveDisplay::InitializePerm(name, buf, end);
}

// FUNCTION: LEGOBATMAN 0x005125b0
void LoadScreenDisplay::Update(f32 dt) { InteractiveDisplay::Update(dt); }

// FUNCTION: LEGOBATMAN 0x005125d0
void LoadScreenDisplay::Render() {
  f22c = 0.0f;
  RenderBackground(0, 0, 0, 0);
  f22c += 0.05f;
  m_bForceAllVisible = 0;
}

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_unk_00512060(f32 *v, f32 a, i32 i) {
  v[2] = NuFabs(a);
  v[0] = NuSinApprox(i);
  v[1] = NuCosApprox(i);
  NuVec4Set(v, a, a, a, a);
  NuVecScaleInline(v + 8, v, a);
}
