// batman/, file unknown (between pcbatman.cpp and pcapi.cpp by link order):
// OggReader (RTTI .?AVOggReader@@, vtable 0x867604), a WavReader that
// decodes through vorbisfile when the 0x40 flag is set, 0x53ab10..0x53b340.

#include "../nu2api/nucore/common.h"
// windows.h
#include <string.h>
#include <windows.h>

struct nudathdr_s;
struct WRFORMAT_s;

// Slots from the WavReader vtable 0x8675cc (src/batman/unk_0053a4b0.cpp).
class WavReader {
public:
  WavReader();
  virtual ~WavReader();
  virtual HRESULT Open(nudathdr_s *hdr, char *name, WAVEFORMATEX *wfx);
  virtual HRESULT Open(char *name, WAVEFORMATEX *wfx);
  virtual HRESULT Close();
  virtual void *GetFormat();
  virtual u32 GetSize();
  virtual HRESULT ResetFile();
  virtual HRESULT Read(u8 *buf, u32 size, u32 *read);
  virtual i32 SetOffset(f32 offset);

  u8 pad04[0x18 - 4];
  unsigned char primary[0x40 - 0x18]; // 0x18, the WAVEFORMATEX
};

struct OggVorbis_File;

i32 ov_clear(OggVorbis_File *vf);
i32 ov_raw_seek(OggVorbis_File *vf, __int64 pos);
__int64 ov_pcm_total(OggVorbis_File *vf, i32 i);
extern "C" i32 NuThreadCreateCriticalSection(void);

// GLOBAL: LEGOBATMAN 0x009e8178
extern i32 g_unk009e8178; // OggReader critical section
// GLOBAL: LEGOBATMAN 0x009e7c28
extern u8 g_unk009e7c28[0x550];

class OggReader : public WavReader {
public:
  OggReader();
  virtual HRESULT Open(char *name, WAVEFORMATEX *wfx);
  virtual HRESULT Close();
  virtual void *GetFormat();
  virtual u32 GetSize();
  virtual HRESULT ResetFile();

  unsigned char flag; // 0x40, 1 = ogg
  u8 pad41[0x48 - 0x41];
  u8 vf[0x318 - 0x48];          // 0x48, OggVorbis_File
  i32 opened;                   // 0x318
  i32 i31c;                     // 0x31c
  unsigned char alternate[0xc]; // 0x320, the decoded WAVEFORMATEX
  u16 block_align;              // 0x32c
};

// FUNCTION: LEGOBATMAN 0x0053ab50
OggReader::OggReader() {
  flag = 0;
  opened = 0;
  i31c = 0;
  if (g_unk009e8178 == 0)
    g_unk009e8178 = NuThreadCreateCriticalSection();
  memset(g_unk009e7c28, 0, sizeof(g_unk009e7c28));
}

// FUNCTION: LEGOBATMAN 0x0053abb0
HRESULT OggReader::Open(char *name, WAVEFORMATEX *wfx) { return E_FAIL; }

// FUNCTION: LEGOBATMAN 0x0053abc0
u32 OggReader::GetSize() {
  if (!flag)
    return WavReader::GetSize();
  if (opened != 0)
    return (u32)ov_pcm_total((OggVorbis_File *)vf, -1) * block_align;
  return 0;
}

// "default, then override" keeps the original's branch order; an early
// return swaps the two leas and does not match.
// FUNCTION: LEGOBATMAN 0x0053ac00
void *OggReader::GetFormat() {
  void *result = primary;
  if (flag)
    result = alternate;
  return result;
}

// FUNCTION: LEGOBATMAN 0x0053ac10
HRESULT OggReader::ResetFile() {
  if (!flag)
    return WavReader::ResetFile();
  if (opened != 0)
    return ov_raw_seek((OggVorbis_File *)vf, 0) < 0 ? E_FAIL : S_OK;
  return E_FAIL;
}

// FUNCTION: LEGOBATMAN 0x0053ad60
HRESULT OggReader::Close() {
  if (!flag)
    return WavReader::Close();
  if (opened == 0)
    return E_FAIL;
  ov_clear((OggVorbis_File *)vf);
  opened = 0;
  return S_OK;
}
