// gameapi/, file unknown: the screen fade classes (RTTI FadeBase, Fade,
// BlackWipe, StillScreenWipe, CrossFade, BlackCrossFade, StillScreen,
// SpinWipe), 0x6710d0..0x671b30. Slot order from the PC vtables
// (0x869d88..0x869e30): deleting dtor, Init, InitFade, UpdateFade, DrawFade,
// GetFadeType.

#include "../nu2api/nucore/common.h"
#include "../nu2api/numath/nutrig_unk.h"

struct FADEINFO_s {
  i32 mask;  // 0x00
  f32 fade;  // 0x04
  f32 rate;  // 0x08
  i32 busy;  // 0x0c
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

struct FADETYPE {
  i32 type;
};

// Mac: FadeSystem; starts with the FADEINFO_s handed to every fade.
struct FadeSystem : FADEINFO_s {
  void Init();
  void Draw();
  void SetStage(char stage);
  i32 AddFade(FadeBase *fade);
  i32 SetFade(FADETYPE const &type, u32 frames);
  void Update();

  FadeBase *fades[7]; // 0x14
  i32 current;        // 0x30
  i32 i34;            // 0x34
};

struct nuvec_s;

// Raw view of the current area (WORLD + 0x12c).
struct FDAREA_s {
  u8 pad00[0x64];
  u32 flags; // 0x64
};

struct FDWORLD_s {
  u8 pad000[0x12c];
  FDAREA_s *area; // 0x12c
};

// GLOBAL: LEGOBATMAN 0x00960894
extern FDWORLD_s *WORLD;
// GLOBAL: LEGOBATMAN 0x00acb070
extern i32 g_unk00acb070;
// GLOBAL: LEGOBATMAN 0x00acb714
extern i32 g_unk00acb714;
// GLOBAL: LEGOBATMAN 0x00aca8a8
extern FDAREA_s *g_unk00aca8a8;
// GLOBAL: LEGOBATMAN 0x00ad29f0
extern i32 g_unk00ad29f0;

void GameAudio_PlaySfx(i32 sfx, nuvec_s *position, i32 flags, i32 volume);
void NeedScreenGrab(i32 a);
void DrawSpinScreen(i32 a, f32 t);

i32 qrand(void);
void DrawFadeScreenWipe();
void DrawStillScreen(i32 a);
void DrawPauseScreenWipe();

struct numtl_s;

void Unk0071af80(i32 x, i32 y, i32 w, i32 h, u32 colour, numtl_s *mtl);
void Unk0071ae30(f32 x, f32 y, f32 z, f32 w, f32 h, f32 u0, f32 v0, f32 u1,
                 f32 v1, u32 colour, numtl_s *mtl);

// numtl_s view: the alpha the cross fade writes.
struct FDMTL_s {
  u8 pad00[0x70];
  f32 alpha; // 0x70
};

FDMTL_s *Unk005a5ab0(); // returns the pause/still-screen material
i32 NuRndrBeginScene(i32 flags);
void NuRndrClear(i32 clear_flags, i32 background_colour, f32 alpha);
void NuRndrGradRectUV2di(i32 x, i32 y, i32 width, i32 height, f32 u0, f32 v0,
                         f32 u1, f32 v1, i32 *colours, FDMTL_s *material);
void NuRndrEndScene();

// GLOBAL: LEGOBATMAN 0x00968550
extern i32 g_unk00968550[4]; // cross fade rect: x, y, width, height

// GLOBAL: LEGOBATMAN 0x00a97cf0
extern numtl_s *g_unk00a97cf0; // fade material

// Body in nutrig_unk.h: this TU's copy of the static.
// FUNCTION: LEGOBATMAN 0x00670c40
static f32 NuSinApprox(i32 angle);

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

// FUNCTION: LEGOBATMAN 0x00670d20
void SetFramesToWait(u32 frames) { FRAMES_TO_WAIT = frames; }

// STUB: LEGOBATMAN 0x00670d30
// area/flags registers swapped (orig: area in ecx, "mov eax, ecx" for flags)
void FadeSystem_PlayWipeSfx() {
  FDAREA_s *area = WORLD->area;
  if (area == 0)
    return;
  if ((area->flags & 0x1000000) && g_unk00acb070 != 0)
    return;
  if (g_unk00acb714 != 0)
    return;
  if (area->flags & 0xe0)
    return;
  if (area == g_unk00aca8a8)
    return;
  GameAudio_PlaySfx(0x40, 0, 0, 0);
}

// FUNCTION: LEGOBATMAN 0x00670d80
void DrawStillScreenWithAlpha(i32 clear, f32 alpha) {
  FDMTL_s *mtl = Unk005a5ab0();
  mtl->alpha = alpha;
  NuRndrBeginScene(1);
  if (clear != 0)
    NuRndrClear(0x500, 0, 1.0f);
  i32 colour = ((i32)(alpha * 128.0f) << 24) | 0x808080;
  i32 colours[4] = {colour, colour, colour, colour};
  NuRndrGradRectUV2di(g_unk00968550[0], g_unk00968550[1], g_unk00968550[2],
                      g_unk00968550[3], 0.0f, 0.0f, 1.0f, 1.0f, colours, mtl);
  NuRndrEndScene();
}

// FUNCTION: LEGOBATMAN 0x00670e30
void DrawCurrentScreenWithAlpha(i32 clear, f32 alpha) {
  NuRndrBeginScene(1);
  if (clear != 0)
    NuRndrClear(0x500, 0, 1.0f);
  Unk0071ae30(-48.0f, -28.0f, 1.0f, 10260.0f, 3592.0f, 0.0f, 0.0f, 1.0f, 1.0f,
              (u32)((1.0f - alpha) * 128.0f) << 24, g_unk00a97cf0);
  NuRndrEndScene();
}

// FUNCTION: LEGOBATMAN 0x00670ef0
void FadeSystem::Init() {
  fades[0] = 0;
  fades[1] = 0;
  fades[2] = 0;
  fades[3] = 0;
  fades[4] = 0;
  fades[5] = 0;
  fades[6] = 0;
  i34 = 1;
  current = -1;
}

extern f32 FRAMETIME;

// FUNCTION: LEGOBATMAN 0x00670f20
void FadeSystem::Update() {
  f32 old_fade = fade;
  i32 type = current;
  if (type == -1)
    return;

  fade = fade + rate * FRAMETIME;
  if (fade > 1.0f)
    fade = 1.0f;
  else if (fade < 0.0f)
    fade = 0.0f;

  if (old_fade < 1.0f && fade == 1.0f)
    busy = 1;
  else if (busy != 0)
    --busy;

  if (fades[type] != 0)
    fades[type]->UpdateFade();
  if (fade == 0.0f || fade == 1.0f) {
    if (fade == 0.0f)
      current = -1;
    rate = 0.0f;
  }
}

// FUNCTION: LEGOBATMAN 0x00671000
void FadeSystem::Draw() {
  if (current != -1 && g_unk00ad29f0 == 0 && fades[current] != 0)
    fades[current]->DrawFade();
}

// FUNCTION: LEGOBATMAN 0x00671030
void FadeSystem::SetStage(char stage) {
  flags = stage;
  busy = 0;
  if (current != -1 && fades[current] != 0)
    fades[current]->InitFade();
}

// FUNCTION: LEGOBATMAN 0x00671060
i32 FadeSystem::AddFade(FadeBase *fade) {
  if (fade == 0)
    return 0;
  i32 type = fade->GetFadeType();
  fade->Init(this);
  fades[type] = fade;
  return 1;
}

// FUNCTION: LEGOBATMAN 0x006710a0
i32 FadeSystem::SetFade(FADETYPE const &type, u32 frames) {
  i32 t = type.type;
  if (t != -1 && fades[t] != 0) {
    current = t;
    mask = frames;
    return 1;
  }
  current = -1;
  return 0;
}

// FUNCTION: LEGOBATMAN 0x006710d0
void Fade::Init(FADEINFO_s *info) { this->info = info; }

// FUNCTION: LEGOBATMAN 0x006710e0
void Fade::InitFade() {
  if (info->flags & 1) {
    info->fade = 1.0f;
    info->rate = -1.3333334f;
  } else {
    info->fade = 0.0f;
    info->rate = 2.0f;
  }
}

// FUNCTION: LEGOBATMAN 0x00671110
void Fade::UpdateFade() {}

// FUNCTION: LEGOBATMAN 0x00671120
void Fade::DrawFade() {
  if (info->fade > 0.0f && g_unk00a97cf0 != 0)
    Unk0071af80(
        0, 0, 0x2800, 0xe00,
        (u32)(NuSinApprox((i32)((info->fade - 0.0f) * 16384.0f + 49152.0f) +
                          0x4000) *
              128.0f)
            << 24,
        g_unk00a97cf0);
}

// FUNCTION: LEGOBATMAN 0x006711b0
void BlackWipe::Init(FADEINFO_s *info) { this->info = info; }

// FUNCTION: LEGOBATMAN 0x006711c0
void BlackWipe::InitFade() {
  i32 old = info->mask;
  if (info->flags & 1) {
    info->fade = 1.0f;
    info->rate = -1.3333334f;
  } else {
    info->fade = 0.0f;
    info->rate = 2.0f;
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
    info->fade = 1.0f;
    info->rate = -1.3333334f;
    FadeSystem_PlayWipeSfx();
  } else {
    g_unk00a97d34 = 0;
    info->fade = 1.0f;
    info->rate = 2.0f;
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
    info->fade = 1.0f;
    info->rate = -1.3333334f;
    FadeSystem_PlayWipeSfx();
  } else {
    g_unk00a97d34 = 0;
    info->fade = 1.0f;
    info->rate = 2.0f;
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

// FUNCTION: LEGOBATMAN 0x00671430
void CrossFade::DrawFade() {
  if (wait_till_next_frame == 0) {
    if (info->flags & 2) {
      DrawStillScreen(1);
    } else {
      f32 alpha =
          NuSinApprox((i32)(info->fade * 16384.0f + 32768.0f) + 0x4000) + 1.0f;
      DrawStillScreenWithAlpha(0, alpha);
      g_unk00a97d34 = 0;
    }
  }
}

// FUNCTION: LEGOBATMAN 0x006714a0
void BlackCrossFade::Init(FADEINFO_s *info) { this->info = info; }

// FUNCTION: LEGOBATMAN 0x006714b0
void BlackCrossFade::InitFade() {
  if (info->flags & 1) {
    info->fade = 1.0f;
    info->rate = -1.3333334f;
    FadeSystem_PlayWipeSfx();
  } else {
    g_unk00a97d34 = 0;
    info->fade = 0.0f;
    info->rate = 2.0f;
    if (g_unk00a97da0)
      NeedScreenGrab(1);
    else
      g_unk00a97da0 = 1;
    wait_till_next_frame = FRAMES_TO_WAIT;
  }
}

// FUNCTION: LEGOBATMAN 0x00671560
void BlackCrossFade::DrawFade() {
  if (wait_till_next_frame == 0) {
    if (info->flags & 2) {
      g_unk00a97d34 = 0;
      f32 alpha =
          1.0f -
          (NuSinApprox((i32)(info->fade * 16384.0f + 32768.0f) + 0x4000) +
           1.0f);
      DrawCurrentScreenWithAlpha(0, alpha);
    } else {
      g_unk00a97d34 = 0;
      f32 alpha =
          1.0f -
          (NuSinApprox((i32)(info->fade * 16384.0f + 32768.0f) + 0x4000) +
           1.0f);
      DrawCurrentScreenWithAlpha(0, alpha);
    }
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
    info->fade = 0.0f;
    info->rate = 0.0f;
  } else {
    info->fade = 1.0f;
    info->rate = 2.0f;
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
    info->fade = 1.0f;
    info->rate = -2.0f;
    FadeSystem_PlayWipeSfx();
  } else {
    g_unk00a97d34 = 0;
    info->fade = 0.0f;
    info->rate = 1.0f / SPINFADETIME;
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
      DrawSpinScreen(1, info->fade);
    else
      DrawFadeScreenWipe();
  }
}
