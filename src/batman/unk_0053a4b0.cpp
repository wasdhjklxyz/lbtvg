// batman/, file unknown: WavReader (RTTI .?AVWavReader@@, vtable 0x8675cc),
// 0x53a4b0..0x53aaf4. Slots: deleting dtor, Open(nudathdr_s*, ...),
// Open(char*, ...), Close, GetFormat, GetSize, ResetFile, Read, SetOffset.

#include "../nu2api/nucore/common.h"
#include <string.h>
// windows.h
#include <windows.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

// Raw view of the dat header: only the archive file name is evidenced.
struct nudathdr_s {
  u8 pad00[0xbc];
  char *file_name; // 0xbc
};

// Looks a file up in the dat; returns its offset and size.
i32 Unk006ddac0(nudathdr_s *hdr, char *name, LARGE_INTEGER *offset, u32 *size);

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

// The 8-byte base at 0x10 does not pad the vfptr: 4-byte packing.
#pragma pack(push, 4)
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

  struct CHUNKINFO {
    u32 id;     // 0x00
    u32 size;   // 0x04
    u32 offset; // 0x08, data start relative to base
  };

  u32 MakeFourCC(char *s);
  BOOL FindChunk(HANDLE file, char *id, CHUNKINFO *ck);
  BOOL LeaveChunk(HANDLE file, CHUNKINFO *ck);
  BOOL SetFilePointer64(HANDLE file, LONGLONG distance, LONGLONG *new_pos,
                        DWORD method);

  u32 u04;     // 0x04
  HANDLE file; // 0x08
  u8 pad0c[0x10 - 0xc];
  LARGE_INTEGER base; // 0x10, start of the wave data in the file
  WRFORMAT_s format;  // 0x18
  i32 data_start;     // 0x2c
  i32 data_end;       // 0x30
  i32 position;       // 0x34
  i32 data_offset;    // 0x38
  u32 data_size;      // 0x3c
};
#pragma pack(pop)

// FUNCTION: LEGOBATMAN 0x0053a480
// Debug report, compiled out.
static void WavDebugUnk0053a480(...) {}

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

// FUNCTION: LEGOBATMAN 0x0053a630
WavReader::~WavReader() {
  if (file != 0) {
    CloseHandle(file);
    file = 0;
  }
}

// FUNCTION: LEGOBATMAN 0x0053a650
BOOL WavReader::FindChunk(HANDLE file, char *id, CHUNKINFO *ck) {
  u32 fourcc = MakeFourCC(id);
  DWORD read;
  LARGE_INTEGER pos;
  LARGE_INTEGER dist;
  while (ReadFile(file, ck, 8, &read, NULL) && read == 8) {
    if (ck->id == fourcc) {
      dist.QuadPart = 0;
      SetFilePointerEx(file, dist, &pos, FILE_CURRENT);
      ck->offset = pos.LowPart - base.LowPart;
      return TRUE;
    }
    dist.QuadPart = ck->size;
    if (SetFilePointerEx(file, dist, &pos, FILE_CURRENT) == -1)
      break;
  }
  return FALSE;
}

// FUNCTION: LEGOBATMAN 0x0053a710
BOOL WavReader::LeaveChunk(HANDLE file, CHUNKINFO *ck) {
  LARGE_INTEGER pos;
  LARGE_INTEGER dist;
  dist.QuadPart = (LONGLONG)ck->offset + ck->size + base.QuadPart;
  return SetFilePointerEx(file, dist, &pos, FILE_BEGIN) != -1;
}

// FUNCTION: LEGOBATMAN 0x0053a760
HRESULT WavReader::Open(nudathdr_s *hdr, char *name, WAVEFORMATEX *wfx) {
  struct {
    u32 riff;
    u32 size;
    u32 wave;
  } riff;
  DWORD read;
  CHUNKINFO ck;
  file = 0;
  base.QuadPart = 0;
  if (hdr != NULL) {
    u32 size;
    if (Unk006ddac0(hdr, name, &base, &size)) {
      WavDebugUnk0053a480(name);
      file = CreateFileA(hdr->file_name, GENERIC_READ, FILE_SHARE_READ, NULL,
                         OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
      if (file != INVALID_HANDLE_VALUE)
        SetFilePointer64(file, base.QuadPart, NULL, FILE_BEGIN);
      else {
        WavDebugUnk0053a480(hdr->file_name);
      }
    }
  } else {
    WavDebugUnk0053a480(name);
    file =
        CreateFileA(name, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
                    FILE_FLAG_SEQUENTIAL_SCAN | FILE_ATTRIBUTE_READONLY, NULL);
  }
  if (file != INVALID_HANDLE_VALUE) {
    if (ReadFile(file, &riff, 12, &read, NULL) && read == 12 &&
        riff.riff == 0x46464952 && riff.wave == 0x45564157 &&
        FindChunk(file, "fmt ", &ck) && ck.size >= 16) {
      memset(&format, 0, sizeof(WAVEFORMATEX));
      if (ReadFile(file, &format, 16, &read, NULL) && read == 16) {
        format.cb_size = 0;
        if (wfx != NULL)
          *wfx = *(WAVEFORMATEX *)&format;
        if (format.format_tag == 1) {
          LeaveChunk(file, &ck);
          if (FindChunk(file, "data", &ck)) {
            data_offset = ck.offset;
            data_size = ck.size;
            if (ck.size != 0) {
              data_end = (i32)ck.size / format.block_align - 1;
              data_start = 0;
              position = 0;
              return S_OK;
            }
          }
        }
      }
    }
    CloseHandle(file);
    file = 0;
    return E_FAIL;
  }
  WavDebugUnk0053a480(name);
  return E_FAIL;
}

// STUB: LEGOBATMAN 0x0053a990
// size/bytes/blocks land in ebx/edi/ebp instead of edi/ebx/ebp, frame 4 short
HRESULT WavReader::Read(u8 *buf, u32 size, u32 *read) {
  if (file != 0) {
    u32 align = format.block_align;
    i32 blocks = size / align;
    if (blocks >= data_end - position)
      blocks = data_end - position;
    u32 bytes = align * blocks;
    i32 offset = position * align;
    if (bytes != size) {
      WavDebugUnk0053a480();
      WavDebugUnk0053a480(size, bytes);
    }
    LARGE_INTEGER dist;
    LARGE_INTEGER pos;
    dist.QuadPart = offset + base.QuadPart + data_offset;
    if (SetFilePointerEx(file, dist, &pos, FILE_BEGIN)) {
      DWORD got;
      if (ReadFile(file, buf, bytes, &got, NULL) && bytes == got) {
        if (read != 0)
          *read = got;
        position += blocks;
        return S_OK;
      }
      WavDebugUnk0053a480();
      WavDebugUnk0053a480(bytes);
      WavDebugUnk0053a480(got);
      WavDebugUnk0053a480(size);
    } else {
      WavDebugUnk0053a480();
      WavDebugUnk0053a480(offset);
      WavDebugUnk0053a480(data_offset);
      WavDebugUnk0053a480(GetSize());
    }
  }
  if (read != 0)
    *read = 0;
  return E_FAIL;
}
