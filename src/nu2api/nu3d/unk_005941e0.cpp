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

  u8 pad04[8 - 4];
  FMVBINK_s *bink; // 0x08
  u8 pad0c[0x14 - 0xc];
  u32 tracks[5];   // 0x14
  u32 track_count; // 0x28
  i32 i2c;         // 0x2c
  u8 b30;          // 0x30
  u8 b31;          // 0x31
  u8 b32;          // 0x32
};

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

// FUNCTION: LEGOBATMAN 0x00594690
void NuFmvStreamPCBink::Close() {
  if (bink != 0) {
    ReleaseTextures(false);
    g_unk02a17c88(bink);
    bink = 0;
  }
  i2c = 0;
}
