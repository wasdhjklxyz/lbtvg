// batman/, file unknown: WavReader (RTTI .?AVWavReader@@, vtable 0x8675cc),
// 0x53a4b0..0x53aaf4. Slots: deleting dtor, Open(nudathdr_s*, ...),
// Open(char*, ...), Close, GetFormat, GetSize, ResetFile, Read, SetOffset.

#include "../nu2api/nucore/common.h"
// windows.h
#include <windows.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

struct nudathdr_s;

// Raw view of the WAVEFORMATEX at 0x18.
struct WRFORMAT_s {
  u16 format_tag;        // 0x18
  u16 channels;          // 0x1a
  u32 samples_per_sec;   // 0x1c
  u32 avg_bytes_per_sec; // 0x20
  u16 block_align;       // 0x24
  u16 bits_per_sample;   // 0x26
  u16 cb_size;           // 0x28
};

class WavReader {
public:
  WavReader();
  virtual ~WavReader();
  virtual HRESULT Open(nudathdr_s *hdr, char *name, WAVEFORMATEX *wfx);
  virtual HRESULT Open(char *name, WAVEFORMATEX *wfx);
  virtual HRESULT Close();
  virtual WRFORMAT_s *GetFormat();
  virtual u32 GetSize();
  virtual HRESULT ResetFile();
  virtual HRESULT Read(u8 *buf, u32 size, u32 *read);
  virtual i32 SetOffset(f32 offset);

  u32 MakeFourCC(char *s);
  BOOL SetFilePointer64(HANDLE file, LONGLONG distance, LONGLONG *new_pos,
                        DWORD method);

  u32 u04;     // 0x04
  HANDLE file; // 0x08
  u8 pad0c[0x18 - 0xc];
  WRFORMAT_s format; // 0x18
  i32 data_start;    // 0x2c
  i32 data_end;      // 0x30
  i32 position;      // 0x34
};

// FUNCTION: LEGOBATMAN 0x0053a4b0
WavReader::WavReader() { file = 0; }

// FUNCTION: LEGOBATMAN 0x0053a4c0
HRESULT WavReader::Open(char *name, WAVEFORMATEX *wfx) { return E_FAIL; }

// FUNCTION: LEGOBATMAN 0x0053a4d0
WRFORMAT_s *WavReader::GetFormat() { return &format; }

// FUNCTION: LEGOBATMAN 0x0053a4e0
u32 WavReader::MakeFourCC(char *s) {
  return ((((s[3] << 8) | s[2]) << 8 | s[1]) << 8) | s[0];
}

// FUNCTION: LEGOBATMAN 0x0053a510
u32 WavReader::GetSize() {
  if (file != 0)
    return (data_end - data_start) * format.block_align;
  return 0;
}

// FUNCTION: LEGOBATMAN 0x0053a530
HRESULT WavReader::ResetFile() {
  if (file != 0) {
    position = data_start;
    return S_OK;
  }
  return E_FAIL;
}

// FUNCTION: LEGOBATMAN 0x0053a550
HRESULT WavReader::Close() {
  if (file != 0) {
    CloseHandle(file);
    file = 0;
    return S_OK;
  }
  return E_FAIL;
}

// STUB: LEGOBATMAN 0x0053a580
// clamp result lands in eax instead of ecx (4 MIN/MAX spellings tried)
i32 WavReader::SetOffset(f32 offset) {
  u32 align = format.block_align;
  position = (i32)((f32)(format.avg_bytes_per_sec / align) * offset);
  position = MAX(data_start, MIN(data_end, position));
  return align * position;
}

// FUNCTION: LEGOBATMAN 0x0053a5f0
BOOL WavReader::SetFilePointer64(HANDLE file, LONGLONG distance,
                                 LONGLONG *new_pos, DWORD method) {
  LARGE_INTEGER dist;
  LARGE_INTEGER pos;
  dist.QuadPart = distance;
  BOOL ok = SetFilePointerEx(file, dist, &pos, method);
  if (new_pos != 0)
    *new_pos = pos.QuadPart;
  return ok;
}

// match.py cannot parse "~" names; the dtor body (0x53a630) matches by hand.
WavReader::~WavReader() {
  if (file != 0) {
    CloseHandle(file);
    file = 0;
  }
}
