// gameapi/, file unknown: the screen fade classes (RTTI FadeBase, Fade,
// BlackWipe, StillScreenWipe, CrossFade, BlackCrossFade, StillScreen,
// SpinWipe), 0x6710d0..0x671b30. Slot order from the PC vtables
// (0x869d88..0x869e30): deleting dtor, Init, InitFade, UpdateFade, DrawFade,
// GetFadeType.

#include "../nu2api/nucore/common.h"

struct FADEINFO_s {
  i32 mask; // 0x00
  f32 f4;   // 0x04
  f32 f8;   // 0x08
  u8 pad0c[4];
  u32 flags; // 0x10
};

class FadeBase {
public:
  virtual ~FadeBase();
  virtual void Init(FADEINFO_s *info) = 0;
  virtual void InitFade() = 0;
  virtual void UpdateFade() = 0;
  virtual void DrawFade() = 0;
  virtual i32 GetFadeType() const = 0;
};

class Fade : public FadeBase {
public:
  virtual void Init(FADEINFO_s *info);
  virtual void InitFade();
  virtual void UpdateFade();
  virtual void DrawFade();
  virtual i32 GetFadeType() const;

  FADEINFO_s *info; // 0x04
};

class BlackWipe : public Fade {
public:
  virtual void Init(FADEINFO_s *info);
  virtual void InitFade();
  virtual void UpdateFade();
  virtual void DrawFade();
};

class StillScreenWipe : public Fade {
public:
  virtual void Init(FADEINFO_s *info);
  virtual void UpdateFade();
  virtual void DrawFade();
};

i32 qrand(void);
void DrawFadeScreenWipe();
void DrawStillScreen(i32 a);
void DrawPauseScreenWipe();

// GLOBAL: LEGOBATMAN 0x00ad29ec
extern i32 wait_till_next_frame;
// GLOBAL: LEGOBATMAN 0x00a97d34
extern i32 g_unk00a97d34;

// FUNCTION: LEGOBATMAN 0x006710d0
void Fade::Init(FADEINFO_s *info) { this->info = info; }

// FUNCTION: LEGOBATMAN 0x006710e0
void Fade::InitFade() {
  if (info->flags & 1) {
    info->f4 = 1.0f;
    info->f8 = -1.3333334f;
  } else {
    info->f4 = 0.0f;
    info->f8 = 2.0f;
  }
}

// FUNCTION: LEGOBATMAN 0x00671110
void Fade::UpdateFade() {}

// FUNCTION: LEGOBATMAN 0x006711b0
void BlackWipe::Init(FADEINFO_s *info) { this->info = info; }

// FUNCTION: LEGOBATMAN 0x006711c0
void BlackWipe::InitFade() {
  i32 old = info->mask;
  if (info->flags & 1) {
    info->f4 = 1.0f;
    info->f8 = -1.3333334f;
  } else {
    info->f4 = 0.0f;
    info->f8 = 2.0f;
  }
  do {
    info->mask = 1 << (qrand() / 0x4000);
  } while (info->mask == old);
}

// FUNCTION: LEGOBATMAN 0x00671220
void BlackWipe::UpdateFade() {}

// FUNCTION: LEGOBATMAN 0x00671230
void BlackWipe::DrawFade() { DrawFadeScreenWipe(); }

// FUNCTION: LEGOBATMAN 0x00671240
void StillScreenWipe::Init(FADEINFO_s *info) { this->info = info; }

// FUNCTION: LEGOBATMAN 0x006712f0
void StillScreenWipe::UpdateFade() {
  if (wait_till_next_frame > 0)
    wait_till_next_frame--;
  if ((info->flags & 2) && wait_till_next_frame == 0)
    g_unk00a97d34 = 1;
}

// FUNCTION: LEGOBATMAN 0x00671320
void StillScreenWipe::DrawFade() {
  if (wait_till_next_frame == 0) {
    if (info->flags & 2)
      DrawStillScreen(1);
    else
      DrawPauseScreenWipe();
  }
}
