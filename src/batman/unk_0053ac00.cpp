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

struct OGGMEM_s {
  u8 pad0[0x104];
  u8 *data; // 0x104
  u32 size; // 0x108
};

i32 ov_clear(OggVorbis_File *vf);
i32 ov_raw_seek(OggVorbis_File *vf, __int64 pos);
__int64 ov_pcm_total(OggVorbis_File *vf, i32 i);
i32 ov_time_seek(OggVorbis_File *vf, double pos);
__int64 ov_pcm_tell(OggVorbis_File *vf);
void NuMemCpy(void *dst, const void *src, u32 size);
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
  virtual i32 SetOffset(f32 offset);

  unsigned char flag; // 0x40, 1 = ogg
  u8 pad41[0x48 - 0x41];
  u8 vf[0x318 - 0x48];          // 0x48, OggVorbis_File
  OGGMEM_s *mem;                // 0x318, the in-memory .ogg being decoded
  u32 mem_pos;                  // 0x31c
  unsigned char alternate[0xc]; // 0x320, the decoded WAVEFORMATEX
  u16 block_align;              // 0x32c
};

// FUNCTION: LEGOBATMAN 0x0053ab50
OggReader::OggReader() {
  flag = 0;
  mem = 0;
  mem_pos = 0;
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
  if (mem != 0)
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
  if (mem != 0)
    return ov_raw_seek((OggVorbis_File *)vf, 0) < 0 ? E_FAIL : S_OK;
  return E_FAIL;
}

// FUNCTION: LEGOBATMAN 0x0053ad60
HRESULT OggReader::Close() {
  if (!flag)
    return WavReader::Close();
  if (mem == 0)
    return E_FAIL;
  ov_clear((OggVorbis_File *)vf);
  mem = 0;
  return S_OK;
}

// FUNCTION: LEGOBATMAN 0x0053ada0
i32 OggReader::SetOffset(f32 offset) {
  if (!flag)
    return WavReader::SetOffset(offset);
  if (mem == 0)
    return -1;
  ov_time_seek((OggVorbis_File *)vf, offset);
  return (i32)ov_pcm_tell((OggVorbis_File *)vf) * block_align;
}

// STUB: LEGOBATMAN 0x0053ae00
// register allocation differs (orig keeps the data pointer in ebp)
u32 ovcb_read(void *ptr, u32 size, u32 nmemb, void *datasource) {
  OggReader *reader = (OggReader *)datasource;
  u8 *data;
  if (reader->mem != 0 && (data = reader->mem->data) != 0) {
    u32 avail = reader->mem->size - reader->mem_pos;
    u32 bytes = size * nmemb;
    if (bytes >= avail)
      bytes = avail;
    NuMemCpy(ptr, data + reader->mem_pos, bytes);
    reader->mem_pos += bytes;
    return bytes / size;
  }
  return 0;
}

// STUB: LEGOBATMAN 0x0053ae70
// SEEK_END: orig adds size + pos then offset; ours adds offset first
i32 ovcb_seek(void *datasource, __int64 offset, i32 whence) {
  OggReader *reader = (OggReader *)datasource;
  if (reader->mem != 0) {
    if (whence == 0)
      return reader->mem_pos = (u32)offset;
    if (whence == 1) {
      u32 pos = reader->mem_pos + (u32)offset;
      reader->mem_pos = pos;
      return pos;
    }
    if (whence == 2) {
      u32 pos = reader->mem->size + reader->mem_pos + (u32)offset;
      reader->mem_pos = pos;
      return pos;
    }
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x0053aed0
i32 ovcb_close(void *datasource) {
  ((OggReader *)datasource)->mem = 0;
  return 1;
}

// FUNCTION: LEGOBATMAN 0x0053aef0
i32 ovcb_tell(void *datasource) {
  OggReader *reader = (OggReader *)datasource;
  if (reader->mem == 0)
    return 0;
  return reader->mem_pos;
}
