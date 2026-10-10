// nu2api/nu3d, file unknown: NuFmvStreamPCBink (RTTI vtable 0x86873c; Mac
// NuFmvStreamPCBink::*). Bink is called through function pointers.

#include "../nucore/common.h"

// GLOBAL: LEGOBATMAN 0x02a17c78
extern void(__stdcall *g_unk02a17c78)(struct FMVBINK_s *bink, u32 track,
                                      i32 volume); // BinkSetVolume
// GLOBAL: LEGOBATMAN 0x02a17c88
extern void(__stdcall *g_unk02a17c88)(struct FMVBINK_s *bink); // BinkClose

// Raw view of the BINK handle.
struct FMVBINK_s {
  u8 pad000[0xd0];
  u32 num_tracks; // 0xd0
  u8 pad0d4[0x250 - 0xd4];
  u32 *track_ids; // 0x250
};

void Unk005290c0();                                       // render lock
void Unk005290d0();                                       // render unlock
void Unk00595520(void *device, void *textures, bool all); // Free_Bink_textures
void Unk005314d0(void *textures);

// GLOBAL: LEGOBATMAN 0x009d10c4
extern i32 g_unk009d10c4; // pixel shader model
// GLOBAL: LEGOBATMAN 0x009d08d8
extern void *g_unk009d08d8; // the D3D device

class NuFmvStreamPCBink {
public:
  virtual void Vfn0();
  virtual bool ReOpen(char const *name);
  virtual void Close();
  virtual void Start();
  virtual void Stop();
  virtual void SetVolume(f32 volume);

  void ReleaseTextures(bool all);
  bool TrackExists(u32 track);
  void ReleaseVideoResources();
  void CreateTextures(bool all);
  void RestoreVideoResources();

  u8 open; // 0x04
  u8 pad05[8 - 5];
  FMVBINK_s *bink; // 0x08
  u8 pad0c[0x14 - 0xc];
  u32 tracks[5];   // 0x14
  u32 track_count; // 0x28
  i32 i2c;         // 0x2c
  u8 b30;          // 0x30
  u8 b31;          // 0x31
  u8 b32;          // 0x32
  u8 pad33;
  u8 textures[0x318 - 0x34]; // 0x34, BINKTEXTURESET
  u8 textures_sm1[4];        // 0x318
  u8 pad31c[0x320 - 0x31c];
};

// GLOBAL: LEGOBATMAN 0x00a94028
extern u8 g_unk00a94028; // video resources released
// GLOBAL: LEGOBATMAN 0x00a94029
extern u8 g_unk00a94029; // video resources restored
// GLOBAL: LEGOBATMAN 0x00a94030
extern NuFmvStreamPCBink g_unk00a94030[2]; // the PC FMV streams

// FUNCTION: LEGOBATMAN 0x005941c0
void NuFmvStreamPCBink::Start() { b32 = 0; }

// FUNCTION: LEGOBATMAN 0x005941d0
void NuFmvStreamPCBink::Stop() { b31 = 1; }

// FUNCTION: LEGOBATMAN 0x005941e0
void NuFmvStreamPCBink::SetVolume(f32 volume) {
  if (bink != 0) {
    i32 v = (i32)(volume * 0.75f * 32768.0f);
    for (u32 i = 0; i != track_count; i++)
      g_unk02a17c78(bink, tracks[i], v);
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
    Unk00595520(g_unk009d08d8, textures, all);
  else
    Unk005314d0(textures_sm1);
  Unk005290d0();
}

// FUNCTION: LEGOBATMAN 0x00594400
void NuFmvStreamPCBink::ReleaseVideoResources() {
  if (bink != 0) {
    ReleaseTextures(true);
    g_unk00a94028 = 1;
  }
}

// FUNCTION: LEGOBATMAN 0x005945a0
void PCFMVReleaseVideoResources() {
  for (i32 i = 0; i != 2; i++) {
    if (g_unk00a94030[i].open && g_unk00a94030[i].bink != 0)
      g_unk00a94030[i].ReleaseVideoResources();
  }
}

// FUNCTION: LEGOBATMAN 0x00594690
void NuFmvStreamPCBink::Close() {
  if (bink != 0) {
    ReleaseTextures(false);
    g_unk02a17c88(bink);
    bink = 0;
  }
  i2c = 0;
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
