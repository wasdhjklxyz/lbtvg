// nu2api/nu3d, file unknown: NuFmvStreamPCBink (RTTI vtable 0x86873c; Mac
// NuFmvStreamPCBink::*). Bink is called through function pointers.

#include "../nucore/common.h"
#include <d3d9.h>
#include <string.h>

// Bink is imported from binkw32.dll (IAT 0x2a17c6c..0x2a17ca4).
struct FMVBINK_s;
extern "C" {
__declspec(dllimport) void __stdcall BinkSetVolume(FMVBINK_s *bink, u32 track,
                                                   i32 volume);
__declspec(dllimport) void __stdcall BinkClose(FMVBINK_s *bink);
__declspec(dllimport) void __stdcall BinkGetFrameBuffersInfo(FMVBINK_s *bink,
                                                             void *buffers);
__declspec(dllimport) void __stdcall BinkRegisterFrameBuffers(FMVBINK_s *bink,
                                                              void *buffers);
__declspec(dllimport) void __stdcall BinkPause(FMVBINK_s *bink, i32 pause);
__declspec(dllimport) void __stdcall BinkGoto(FMVBINK_s *bink, u32 frame,
                                              i32 flags);
__declspec(dllimport) i32 __stdcall BinkDoFrame(FMVBINK_s *bink);
__declspec(dllimport) i32 __stdcall BinkShouldSkip(FMVBINK_s *bink);
__declspec(dllimport) void __stdcall BinkNextFrame(FMVBINK_s *bink);
__declspec(dllimport) i32 __stdcall BinkWait(FMVBINK_s *bink);
__declspec(dllimport) u32 __stdcall BinkGetRects(FMVBINK_s *bink, u32 flags);
__declspec(dllimport) i32 __stdcall BinkCopyToBuffer(FMVBINK_s *bink,
                                                     void *dest, i32 pitch,
                                                     u32 height, u32 x, u32 y,
                                                     u32 flags);
__declspec(dllimport) i32 __stdcall
BinkCopyToBufferRect(FMVBINK_s *bink, void *dest, i32 pitch, u32 height, u32 x,
                     u32 y, u32 src_x, u32 src_y, u32 src_w, u32 src_h,
                     u32 flags);
}

// Raw view of the BINK handle.
struct FMVBINK_s {
  u32 width;  // 0x00
  u32 height; // 0x04
  u32 frames; // 0x08
  u32 frame;  // 0x0c
  u8 pad010[0x34 - 0x10];
  struct {
    i32 x, y, w, h;
  } rects[8]; // 0x34, FrameRects
  u8 pad0b4[0xd0 - 0xb4];
  u32 num_tracks; // 0xd0
  u8 pad0d4[0x250 - 0xd4];
  u32 *track_ids; // 0x250
};

void Unk005290c0();                                       // render lock
void Unk005290d0();                                       // render unlock
void Unk00595520(void *device, void *textures, bool all); // Free_Bink_textures
void Unk005314d0(IDirect3DTexture9 **tex);
i32 Unk00595670(void *device, void *textures, bool all); // Create_Bink_textures
void Unk00530110(u32 width, u32 height, i32 levels, i32 usage, i32 format,
                 i32 pool, IDirect3DTexture9 **out); // d3dCreateTexture

void NuProcessSystemEvents();
i32 NuIsRenderingPossible();
void NudxInput_Clearflags();
void NuShaderManagerBindShader(void *shader);
void Unk00595910(void *textures); // Lock_Bink_textures
void Unk00595a20(void *device, void *textures,
                 FMVBINK_s *bink); // Unlock_Bink_textures

// GLOBAL: LEGOBATMAN 0x0094d3a0
extern i32 g_unk0094d3a0; // app active
// GLOBAL: LEGOBATMAN 0x029dcbec
extern void *g_nutex_029dcbec;
// GLOBAL: LEGOBATMAN 0x029dcbf0
extern void *g_nutex_029dcbf0;

class CNuConsole {
public:
  void SendD3DWarning(long hr, char *fmt, ...);
};
// GLOBAL: LEGOBATMAN 0x009d0380
extern CNuConsole g_nuConsole;

// GLOBAL: LEGOBATMAN 0x009d10c4
extern i32 g_unk009d10c4; // pixel shader model
// GLOBAL: LEGOBATMAN 0x009d08d8
extern void *g_unk009d08d8; // the D3D device

// Bink's BINKTEXTURESET (0xd8 bytes).
struct BINKTEXTURESET_s {
  u8 pad00[0x40];
  u32 total_frames; // 0x40, BINKFRAMEBUFFERS
  u32 ya_width;     // 0x44
  u32 ya_height;    // 0x48
  u32 crcb_width;   // 0x4c
  u32 crcb_height;  // 0x50
  u8 pad54[0xb8 - 0x54];
  IDirect3DTexture9 *tex[8]; // 0xb8
};

// Texture descriptor the FMV planes are mapped to (0x80 bytes, as in
// nutex_pc.cpp).
struct NuTex {
  i32 width;  // 0x00
  i32 height; // 0x04
  u8 pad08[0x18 - 0x8];
  u32 u18;
  u32 u1c;
  u32 u20;
  u32 u24;
  u32 u28;
  u32 b2c : 5; // 0x2c
  u32 b2c_rest : 27;
  u32 u30;
  u32 u34;
  u32 u38;
  u32 u3c;
  u32 u40;
  u32 u44;
  u32 u48;
  u32 u4c;
  u32 u50;
  u32 u54;
  u8 pad58[0x74 - 0x58];
  u32 u74;
  u32 u78;
  IDirect3DTexture9 *d3dtex; // 0x7c
};

// Mac typeinfo: NuFmvStream (and NuFmvBuffer) have one public base at offset
// 4; NuFmvRefCountedObject is the only candidate. Non-polymorphic, so MSVC
// runs its constructor before the vfptr store.
struct NuFmvRefCountedObject {
  NuFmvRefCountedObject() { open = 0; }
  u8 open; // 0x04
};

class NuFmvStream : public NuFmvRefCountedObject {
public:
  NuFmvStream();
  virtual void Vfn0() = 0;
  virtual bool ReOpen(char const *name);
  virtual void Close() = 0;
};

class NuFmvStreamPCBink : public NuFmvStream {
public:
  NuFmvStreamPCBink();
  virtual void Vfn0();
  virtual void Close();
  virtual void Start();
  virtual void Stop();
  virtual void SetVolume(f32 volume);

  void ReleaseTextures(bool all);
  bool TrackExists(u32 track);
  void ReleaseVideoResources();
  i32 GetNextFrame();
  void BlitSM1Texture();
  bool CreateTextures(bool all);
  void MapBinkTexturesToTIDs();
  void RestoreVideoResources();

  FMVBINK_s *bink; // 0x08
  u8 pad0c[0x14 - 0xc];
  u32 tracks[5];   // 0x14
  u32 track_count; // 0x28
  i32 i2c;         // 0x2c
  u8 b30;          // 0x30
  u8 b31;          // 0x31
  u8 b32;          // 0x32
  u8 pad33;
  BINKTEXTURESET_s textures; // 0x34
  u32 u10c;                  // 0x10c
  u32 u110;                  // 0x110
  u32 u114;                  // 0x114
  NuTex tex[3];              // 0x118, Y/cR/cB planes
  u32 u298;                  // 0x298
  NuTex sm1;                 // 0x29c, shader model 1 path
  u8 b31c;                   // 0x31c
  u8 pad31d[0x320 - 0x31d];
};

// GLOBAL: LEGOBATMAN 0x00a94028
extern u8 g_unk00a94028; // video resources released
// GLOBAL: LEGOBATMAN 0x00a94029
extern u8 g_unk00a94029; // video resources restored
// GLOBAL: LEGOBATMAN 0x00a9402a
static u8
    g_unk00a9402a; // paused while inactive// GLOBAL: LEGOBATMAN 0x00a94030
extern NuFmvStreamPCBink g_unk00a94030[2]; // the PC FMV streams

// FUNCTION: LEGOBATMAN 0x005941b0
NuFmvStream::NuFmvStream() {}

// FUNCTION: LEGOBATMAN 0x005941c0
void NuFmvStreamPCBink::Start() { b32 = 0; }

// FUNCTION: LEGOBATMAN 0x005941d0
void NuFmvStreamPCBink::Stop() { b31 = 1; }

// FUNCTION: LEGOBATMAN 0x005941e0
void NuFmvStreamPCBink::SetVolume(f32 volume) {
  if (bink != 0) {
    i32 v = (i32)(volume * 0.75f * 32768.0f);
    for (u32 i = 0; i != track_count; i++)
      BinkSetVolume(bink, tracks[i], v);
  }
}

// FUNCTION: LEGOBATMAN 0x00594230
bool NuFmvStreamPCBink::TrackExists(u32 track) {
  FMVBINK_s *b = bink;
  u32 n = b->num_tracks;
  for (u32 i = 0; i != n; i++) {
    if (b->track_ids[i] == track)
      return true;
  }
  return false;
}

// FUNCTION: LEGOBATMAN 0x00594270
void NuFmvStreamPCBink::ReleaseTextures(bool all) {
  Unk005290c0();
  if (g_unk009d10c4 >= 4)
    Unk00595520(g_unk009d08d8, &textures, all);
  else
    Unk005314d0(&sm1.d3dtex);
  Unk005290d0();
}

// FUNCTION: LEGOBATMAN 0x005942c0
void NuFmvStreamPCBink::MapBinkTexturesToTIDs() {
  tex[0].b2c = 0;
  tex[0].u18 = 0;
  tex[0].u1c = 0;
  tex[0].u20 = u10c;
  tex[0].u24 = 0;
  tex[0].u28 = 0;
  tex[0].u30 = 0;
  tex[0].u34 = 0;
  tex[0].u38 = 0;
  tex[0].u3c = 0;
  tex[0].u40 = 1;
  tex[0].u44 = 0;
  tex[0].u48 = 0;
  tex[0].u4c = 0;
  tex[0].u50 = 0;
  tex[0].u54 = 0;
  tex[0].u74 = 0;
  tex[0].u78 = 0;
  tex[1] = tex[0];
  tex[2] = tex[0];
  if (g_unk009d10c4 >= 4) {
    tex[0].width = textures.ya_width;
    tex[0].height = textures.ya_height;
    tex[0].d3dtex = textures.tex[4];
    tex[1].width = textures.crcb_width;
    tex[1].height = textures.crcb_height;
    tex[1].d3dtex = textures.tex[5];
    tex[2].width = textures.crcb_width;
    tex[2].height = textures.crcb_height;
    tex[2].d3dtex = textures.tex[6];
  } else {
    sm1.width = bink->width;
    sm1.height = bink->height;
    IDirect3DTexture9 *d3dtex = sm1.d3dtex;
    sm1 = tex[0];
    sm1.d3dtex = d3dtex;
  }
}

// FUNCTION: LEGOBATMAN 0x00594400
void NuFmvStreamPCBink::ReleaseVideoResources() {
  if (bink != 0) {
    ReleaseTextures(true);
    g_unk00a94028 = 1;
  }
}

// FUNCTION: LEGOBATMAN 0x00594420
void NuFmvStreamPCBink::BlitSM1Texture() {
  if (bink != 0) {
    u32 n = BinkGetRects(bink, 0);
    i32 limit = (bink->width * bink->height * 75) / 100;
    i32 area = 0;
    u32 i;
    for (i = 0; i != n; i++) {
      area += bink->rects[i].w * bink->rects[i].h;
      if (area >= limit)
        break;
    }
    D3DLOCKED_RECT lr;
    if (area < limit) {
      for (i = 0; i != n; i++) {
        RECT r;
        r.left = bink->rects[i].x;
        r.right = bink->rects[i].x + bink->rects[i].w;
        r.top = bink->rects[i].y;
        r.bottom = bink->rects[i].y + bink->rects[i].h;
        lr.Pitch = 0;
        lr.pBits = 0;
        HRESULT hr = sm1.d3dtex->LockRect(0, &lr, &r, 0);
        if (hr < 0)
          g_nuConsole.SendD3DWarning(hr, "BlitSM1Texture");
        BinkCopyToBufferRect(bink, lr.pBits, lr.Pitch, bink->height, 0, 0,
                             bink->rects[i].x, bink->rects[i].y,
                             bink->rects[i].w, bink->rects[i].h, 0x80080003);
        sm1.d3dtex->UnlockRect(0);
      }
    } else {
      sm1.d3dtex->LockRect(0, &lr, 0, 0x2000);
      BinkCopyToBuffer(bink, lr.pBits, lr.Pitch, bink->height, 0, 0,
                       0x80080003);
      sm1.d3dtex->UnlockRect(0);
    }
  }
}

// FUNCTION: LEGOBATMAN 0x005945a0
void PCFMVReleaseVideoResources() {
  for (i32 i = 0; i != 2; i++) {
    if (g_unk00a94030[i].open && g_unk00a94030[i].bink != 0)
      g_unk00a94030[i].ReleaseVideoResources();
  }
}

// FUNCTION: LEGOBATMAN 0x00594600
NuFmvStreamPCBink::NuFmvStreamPCBink() {
  bink = 0;
  b30 = 0;
  b32 = 1;
  u10c = 0;
  u110 = 0;
  u114 = 0;
  u298 = 0;
  b31c = 0;
  memset(&tex[0], 0, sizeof(NuTex));
  memset(&tex[1], 0, sizeof(NuTex));
  memset(&tex[2], 0, sizeof(NuTex));
  memset(&sm1, 0, sizeof(NuTex));
}

// FUNCTION: LEGOBATMAN 0x00594690
void NuFmvStreamPCBink::Close() {
  if (bink != 0) {
    ReleaseTextures(false);
    BinkClose(bink);
    bink = 0;
  }
  i2c = 0;
}

// FUNCTION: LEGOBATMAN 0x005946c0
i32 NuFmvStreamPCBink::GetNextFrame() {
  if (b31)
    return 3;
  if (b32)
    return 1;
  NuProcessSystemEvents();
  NuIsRenderingPossible();
  if (g_unk0094d3a0 == 0) {
    g_unk00a9402a = 1;
    BinkPause(bink, 1);
    while (g_unk0094d3a0 == 0) {
      NuProcessSystemEvents();
      NuIsRenderingPossible();
      Sleep(100);
    }
    BinkPause(bink, 0);
    return 1;
  }
  if (g_unk00a9402a && g_unk00a94028 && !g_unk00a94029)
    return 1;
  if (g_unk009d10c4 >= 4) {
    Unk00595910(&textures);
    BinkRegisterFrameBuffers(bink, &textures.total_frames);
  }
  if (g_unk00a9402a && g_unk00a94028 && g_unk00a94029) {
    BinkGoto(bink, ((i32)bink->frame - 10 > 0) ? (i32)bink->frame - 10 : 0, 0);
    g_unk00a94028 = 0;
    g_unk00a9402a = 0;
    g_unk00a94029 = 0;
  }
  BinkDoFrame(bink);
  while (BinkShouldSkip(bink)) {
    BinkNextFrame(bink);
    BinkDoFrame(bink);
  }
  if (g_unk009d10c4 >= 4)
    Unk00595a20(g_unk009d08d8, &textures, bink);
  else
    BlitSM1Texture();
  if (BinkWait(bink))
    return 1;
  if (bink->frame >= bink->frames) {
    if (b30) {
      BinkGoto(bink, 0, 0);
    } else {
      b31 = 1;
      return 3;
    }
  }
  BinkNextFrame(bink);
  NudxInput_Clearflags();
  NuShaderManagerBindShader(0);
  g_nutex_029dcbec = 0;
  g_nutex_029dcbf0 = 0;
  return 1;
}

// FUNCTION: LEGOBATMAN 0x005948a0
bool NuFmvStreamPCBink::CreateTextures(bool all) {
  Unk005290c0();
  if (g_unk009d10c4 >= 4) {
    if (all) {
      memset(textures.tex, 0, sizeof(textures.tex));
      memset(&textures.total_frames, 0, 0x78);
    } else {
      memset(&textures, 0, sizeof(textures));
    }
    BinkGetFrameBuffersInfo(bink, &textures.total_frames);
    if (!Unk00595670(g_unk009d08d8, &textures, all))
      BinkClose(bink);
    else
      BinkRegisterFrameBuffers(bink, &textures.total_frames);
  } else {
    Unk00530110(bink->width, bink->height, 1, 0x200, 0x15, 0, &sm1.d3dtex);
  }
  MapBinkTexturesToTIDs();
  Unk005290d0();
  return true;
}

// FUNCTION: LEGOBATMAN 0x005949b0
void NuFmvStreamPCBink::RestoreVideoResources() {
  if (bink != 0) {
    CreateTextures(true);
    g_unk00a94029 = 1;
  }
}

// FUNCTION: LEGOBATMAN 0x005949d0
void PCFMVRestoreVideoResources() {
  for (i32 i = 0; i != 2; i++) {
    if (g_unk00a94030[i].open && g_unk00a94030[i].bink != 0)
      g_unk00a94030[i].RestoreVideoResources();
  }
}
