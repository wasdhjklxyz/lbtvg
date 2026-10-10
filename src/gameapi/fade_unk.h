#pragma once
// gameapi/fade_unk.h: the screen fade classes (RTTI FadeBase, Fade, ...).
// Their ctors, dtors, GetFadeType and deleting dtors are in
// screen_unk.cpp, which defines the fade globals; the rest is in
// unk_006710d0.cpp. GetFadeType values from the PC vtables.

#include "../nu2api/nucore/common.h"
#include <stddef.h>

struct FADEINFO_s {
  i32 mask;  // 0x00
  f32 fade;  // 0x04
  f32 rate;  // 0x08
  i32 busy;  // 0x0c
  u32 flags; // 0x10
};

class FadeBase {
public:
  FadeBase();
  virtual ~FadeBase();
  virtual void Init(FADEINFO_s *info) = 0;
  virtual void InitFade() = 0;
  virtual void UpdateFade() = 0;
  virtual void DrawFade() = 0;
  virtual i32 GetFadeType() const = 0;

  FADEINFO_s *info; // 0x04
};

class Fade : public FadeBase {
public:
  Fade();
  virtual ~Fade();
  virtual void Init(FADEINFO_s *info);
  virtual void InitFade();
  virtual void UpdateFade();
  virtual void DrawFade();
  virtual i32 GetFadeType() const;
};

class BlackWipe : public Fade {
public:
  BlackWipe();
  virtual ~BlackWipe();
  virtual void Init(FADEINFO_s *info);
  virtual void InitFade();
  virtual void UpdateFade();
  virtual void DrawFade();
  virtual i32 GetFadeType() const;
};

class StillScreenWipe : public Fade {
public:
  StillScreenWipe();
  virtual ~StillScreenWipe();
  virtual void Init(FADEINFO_s *info);
  virtual void InitFade();
  virtual void UpdateFade();
  virtual void DrawFade();
  virtual i32 GetFadeType() const;
};

class CrossFade : public Fade {
public:
  CrossFade();
  virtual ~CrossFade();
  virtual void Init(FADEINFO_s *info);
  virtual void InitFade();
  virtual void UpdateFade();
  virtual void DrawFade();
  virtual i32 GetFadeType() const;
};

class BlackCrossFade : public Fade {
public:
  BlackCrossFade();
  virtual ~BlackCrossFade();
  virtual void Init(FADEINFO_s *info);
  virtual void InitFade();
  virtual void UpdateFade();
  virtual void DrawFade();
  virtual i32 GetFadeType() const;
};

class StillScreen : public Fade {
public:
  StillScreen();
  virtual ~StillScreen();
  virtual void Init(FADEINFO_s *info);
  virtual void InitFade();
  virtual void UpdateFade();
  virtual void DrawFade();
  virtual i32 GetFadeType() const;
};

class SpinWipe : public Fade {
public:
  SpinWipe();
  virtual ~SpinWipe();
  virtual void Init(FADEINFO_s *info);
  virtual void InitFade();
  virtual void UpdateFade();
  virtual void DrawFade();
  virtual i32 GetFadeType() const;
};
