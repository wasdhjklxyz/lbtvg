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
  virtual void InitFade();
  virtual void UpdateFade();
  virtual void DrawFade();
};

class CrossFade : public Fade {
public:
  virtual void Init(FADEINFO_s *info);
  virtual void InitFade();
  virtual void UpdateFade();
  virtual void DrawFade();
};

class BlackCrossFade : public Fade {
public:
  virtual void Init(FADEINFO_s *info);
  virtual void InitFade();
  virtual void UpdateFade();
  virtual void DrawFade();
};

class StillScreen : public Fade {
public:
  virtual void Init(FADEINFO_s *info);
  virtual void InitFade();
  virtual void UpdateFade();
  virtual void DrawFade();
};

class SpinWipe : public Fade {
public:
  virtual void Init(FADEINFO_s *info);
  virtual void InitFade();
  virtual void UpdateFade();
  virtual void DrawFade();
};

void FadeSystem_PlayWipeSfx();
void NeedScreenGrab(i32 a);
void DrawSpinScreen(i32 a, f32 t);

i32 qrand(void);
void DrawFadeScreenWipe();
void DrawStillScreen(i32 a);
void DrawPauseScreenWipe();

// GLOBAL: LEGOBATMAN 0x00ad29ec
extern i32 wait_till_next_frame;
// GLOBAL: LEGOBATMAN 0x00a97d34
extern i32 g_unk00a97d34;
// GLOBAL: LEGOBATMAN 0x00a97da0
extern i32 g_unk00a97da0;
// GLOBAL: LEGOBATMAN 0x00968494
extern i32 FRAMES_TO_WAIT;
// GLOBAL: LEGOBATMAN 0x009684a8
extern f32 SPINFADETIME;

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

// FUNCTION: LEGOBATMAN 0x00671250
void StillScreenWipe::InitFade() {
  i32 old = info->mask;
  if (info->flags & 1) {
    info->f4 = 1.0f;
    info->f8 = -1.3333334f;
    FadeSystem_PlayWipeSfx();
  } else {
    g_unk00a97d34 = 0;
    info->f4 = 1.0f;
    info->f8 = 2.0f;
    if (g_unk00a97da0)
      NeedScreenGrab(1);
    else
      g_unk00a97da0 = 1;
    wait_till_next_frame = FRAMES_TO_WAIT;
  }
  do {
    info->mask = 1 << (qrand() / 0x4000);
  } while (info->mask == old);
}

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

// FUNCTION: LEGOBATMAN 0x00671350
void CrossFade::Init(FADEINFO_s *info) { this->info = info; }

// FUNCTION: LEGOBATMAN 0x00671360
void CrossFade::InitFade() {
  i32 old = info->mask;
  if (info->flags & 1) {
    info->f4 = 1.0f;
    info->f8 = -1.3333334f;
    FadeSystem_PlayWipeSfx();
  } else {
    g_unk00a97d34 = 0;
    info->f4 = 1.0f;
    info->f8 = 2.0f;
    if (g_unk00a97da0)
      NeedScreenGrab(1);
    else
      g_unk00a97da0 = 1;
    wait_till_next_frame = FRAMES_TO_WAIT;
  }
  do {
    info->mask = 1 << (qrand() / 0x4000);
  } while (info->mask == old);
}

// FUNCTION: LEGOBATMAN 0x00671400
void CrossFade::UpdateFade() {
  if (wait_till_next_frame > 0)
    wait_till_next_frame--;
  if ((info->flags & 2) && wait_till_next_frame == 0)
    g_unk00a97d34 = 1;
}

// FUNCTION: LEGOBATMAN 0x006714a0
void BlackCrossFade::Init(FADEINFO_s *info) { this->info = info; }

// FUNCTION: LEGOBATMAN 0x006714b0
void BlackCrossFade::InitFade() {
  if (info->flags & 1) {
    info->f4 = 1.0f;
    info->f8 = -1.3333334f;
    FadeSystem_PlayWipeSfx();
  } else {
    g_unk00a97d34 = 0;
    info->f4 = 0.0f;
    info->f8 = 2.0f;
    if (g_unk00a97da0)
      NeedScreenGrab(1);
    else
      g_unk00a97da0 = 1;
    wait_till_next_frame = FRAMES_TO_WAIT;
  }
}

// FUNCTION: LEGOBATMAN 0x00671530
void BlackCrossFade::UpdateFade() {
  if (wait_till_next_frame > 0)
    wait_till_next_frame--;
  if ((info->flags & 2) && wait_till_next_frame == 0)
    g_unk00a97d34 = 1;
}

// FUNCTION: LEGOBATMAN 0x00671600
void StillScreen::Init(FADEINFO_s *info) { this->info = info; }

// FUNCTION: LEGOBATMAN 0x00671610
void StillScreen::InitFade() {
  i32 old = info->mask;
  if (info->flags & 1) {
    info->f4 = 0.0f;
    info->f8 = 0.0f;
  } else {
    info->f4 = 1.0f;
    info->f8 = 2.0f;
    NeedScreenGrab(1);
    wait_till_next_frame = FRAMES_TO_WAIT;
  }
  do {
    info->mask = 1 << (qrand() / 0x4000);
  } while (info->mask == old);
}

// FUNCTION: LEGOBATMAN 0x00671680
void StillScreen::UpdateFade() {
  if (info->flags & 2)
    g_unk00a97d34 = 1;
}

// FUNCTION: LEGOBATMAN 0x006716a0
void StillScreen::DrawFade() {
  if (wait_till_next_frame == 0) {
    if (info->flags & 2)
      DrawStillScreen(1);
    else
      DrawPauseScreenWipe();
  } else {
    wait_till_next_frame--;
  }
}

// FUNCTION: LEGOBATMAN 0x00671a10
void SpinWipe::Init(FADEINFO_s *info) { this->info = info; }

// FUNCTION: LEGOBATMAN 0x00671a20
void SpinWipe::InitFade() {
  i32 old = info->mask;
  if (info->flags & 1) {
    info->f4 = 1.0f;
    info->f8 = -2.0f;
    FadeSystem_PlayWipeSfx();
  } else {
    g_unk00a97d34 = 0;
    info->f4 = 0.0f;
    info->f8 = 1.0f / SPINFADETIME;
    if (g_unk00a97da0)
      NeedScreenGrab(1);
    else
      g_unk00a97da0 = 1;
    wait_till_next_frame = FRAMES_TO_WAIT;
  }
  do {
    info->mask = 1 << (qrand() / 0x4000);
  } while (info->mask == old);
}

// FUNCTION: LEGOBATMAN 0x00671ad0
void SpinWipe::UpdateFade() {
  if (wait_till_next_frame > 0)
    wait_till_next_frame--;
  if ((info->flags & 2) && wait_till_next_frame == 0)
    g_unk00a97d34 = 1;
}

// FUNCTION: LEGOBATMAN 0x00671b00
void SpinWipe::DrawFade() {
  if (wait_till_next_frame == 0) {
    if (info->flags & 2)
      DrawSpinScreen(1, info->f4);
    else
      DrawFadeScreenWipe();
  }
}
