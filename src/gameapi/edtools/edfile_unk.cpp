// gameapi/edtools/edfile_unk.cpp: saga's edfile.cpp; the TU was first found
// by its header-static copies.

#include "../../nu2api/numath/nuinline_unk.h"
#include "../../nu2api/numath/nutrig_unk.h"

// FUNCTION: LEGOBATMAN 0x0059cb30
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_0059cb30(f32 *v, f32 a, i32 i) { NuVec4Set(v, a, a, a, a); }

// The rest of this TU is saga's gameapi/edtools/edfile.cpp, in source order
// from 0x0059cb90; the four small functions before it touch the global right
// after edfile_buffer, so they are this file's too.

#include "../../nu2api/numath/nuvec.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

typedef i32 NUFILE;

NUFILE NuFileOpen(char *path, i32 mode);
void NuFileClose(NUFILE file);
i32 NuFileRead(NUFILE file, void *dst, i32 size);
i32 NuFileWrite(NUFILE file, void *src, i32 size);
i64 NuFileSize(char *path);
NUFILE NuMemFileOpen(void *buf, i32 buf_size, i32 mode);
i32 NuFilePakGetItem(void *pak, char *name);
void NuFilePakGetItemInfo(void *pak, i32 item, void **data, i32 *size);

// GLOBAL: LEGOBATMAN 0x0095eab0
i32 edfile_handle = -1;
// GLOBAL: LEGOBATMAN 0x00a94788
i32 edfile_buffer_pointer;
// GLOBAL: LEGOBATMAN 0x00a94790
i32 edfile_lock[2];
// GLOBAL: LEGOBATMAN 0x00a94798
char edfile_buffer[0x1000];
// GLOBAL: LEGOBATMAN 0x00a95798
i32 g_unk00a95798;
// GLOBAL: LEGOBATMAN 0x00a9579c
static i32 edfile_media;
// GLOBAL: LEGOBATMAN 0x00a957a0
static void *edfile_pakfile;
// GLOBAL: LEGOBATMAN 0x00a957a4
i32 edfile_write_flag;
// GLOBAL: LEGOBATMAN 0x00a957a8
i32 edfile_readwrongendianess;

// FUNCTION: LEGOBATMAN 0x0059cb50
i32 Unk0059cb50(void) { return 0; }

// FUNCTION: LEGOBATMAN 0x0059cb60
void Unk0059cb60(void) {}

// FUNCTION: LEGOBATMAN 0x0059cb70
void SetUnk00a95798(i32 value) { g_unk00a95798 = value; }

// FUNCTION: LEGOBATMAN 0x0059cb80
i32 GetUnk00a95798(void) { return g_unk00a95798; }

// FUNCTION: LEGOBATMAN 0x0059cb90
extern "C" void EdFileSwapEndianess16(void *data) {
  char *bytes;
  char tmp;

  bytes = (char *)data;
  tmp = bytes[0];
  bytes[0] = bytes[1];
  bytes[1] = tmp;
}

// FUNCTION: LEGOBATMAN 0x0059cba0
extern "C" void EdFileSwapEndianess32(void *data) {
  char *bytes;
  char tmp;

  bytes = (char *)data;
  tmp = bytes[0];
  bytes[0] = bytes[3];
  bytes[3] = tmp;
  tmp = bytes[1];
  bytes[1] = bytes[2];
  bytes[2] = tmp;
}

// FUNCTION: LEGOBATMAN 0x0059cbc0
extern "C" void EdFileSetReadWrongEndianess(i32 value) {}

// FUNCTION: LEGOBATMAN 0x0059cbd0
extern "C" void EdFileSetMedia(i32 media) { edfile_media = media; }

// FUNCTION: LEGOBATMAN 0x0059cbe0
extern "C" void EdFileSetPakFile(void *pak) { edfile_pakfile = pak; }

// FUNCTION: LEGOBATMAN 0x0059cbf0
extern "C" void EdFileResetBuffers(void) {
  memset(edfile_buffer, 0, sizeof(edfile_buffer));
  edfile_buffer_pointer = 0;
  edfile_lock[1] = 0;
  edfile_lock[0] = 0;
}

// FUNCTION: LEGOBATMAN 0x0059cc30
extern "C" void EdFileFillBuffer(void) {
  if (edfile_handle != -1) {
    memset(edfile_buffer, 0, sizeof(edfile_buffer));
    switch (edfile_media) {
    case 0:
      return;
    case 1:
      NuFileRead(edfile_handle, edfile_buffer, 0x1000);
      edfile_buffer_pointer = 0;
      break;
    }
  }
}

// FUNCTION: LEGOBATMAN 0x0059cc80
extern "C" i32 EdFileOpen(char *filepath, i32 mode) {
  i32 item_handle;
  void *item;
  i32 item_size;

  if (edfile_handle != -1)
    return 0;
  EdFileResetBuffers();
  edfile_handle = -1;
  edfile_write_flag = 0;
  switch (edfile_media) {
  case 0:
    return 0;
  case 1:
    switch (mode) {
    case 0:
      if (edfile_pakfile != NULL) {
        item_handle = NuFilePakGetItem(edfile_pakfile, filepath);
        if (item_handle != 0) {
          NuFilePakGetItemInfo(edfile_pakfile, item_handle, &item, &item_size);
          edfile_handle = NuMemFileOpen(item, item_size, 0);
          if (edfile_handle > 0) {
            EdFileFillBuffer();
            return 1;
          }
          edfile_handle = -1;
        }
      }
      edfile_handle = NuFileOpen(filepath, 0);
      if (edfile_handle > 0) {
        EdFileFillBuffer();
        return 1;
      }
      edfile_handle = -1;
      return 0;
    case 1:
      edfile_write_flag = 1;
      edfile_handle = NuFileOpen(filepath, 1);
      if (edfile_handle > 0)
        return 1;
      edfile_handle = -1;
      return 0;
    }
    break;
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x0059cdb0
extern "C" void EdFileFlushBuffer(void) {
  if (edfile_handle != -1 && edfile_buffer_pointer != 0) {
    switch (edfile_media) {
    case 0:
      return;
    case 1:
      NuFileWrite(edfile_handle, edfile_buffer, edfile_buffer_pointer);
      edfile_buffer_pointer = 0;
      break;
    }
  }
}

// FUNCTION: LEGOBATMAN 0x0059cdf0
extern "C" i32 EdFileClose(void) {
  edfile_pakfile = NULL;
  if (edfile_handle != -1) {
    if (edfile_write_flag != 0)
      EdFileFlushBuffer();
    switch (edfile_media) {
    case 0:
      return 0;
    case 1:
      NuFileClose(edfile_handle);
      edfile_handle = -1;
      edfile_media = 0;
      return 1;
    }
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x0059ce50
extern "C" void EdFileWrite(void *data, i32 len) {
  i32 to_write;

  while (len > 0) {
    to_write = 0x1000 - edfile_buffer_pointer;
    if (len < to_write)
      to_write = len;
    memcpy(edfile_buffer + edfile_buffer_pointer, data, to_write);
    edfile_buffer_pointer += to_write;
    len -= to_write;
    data = (char *)data + to_write;
    if (edfile_buffer_pointer == 0x1000)
      EdFileFlushBuffer();
  }
}

// FUNCTION: LEGOBATMAN 0x0059cee0
extern "C" void EdFileWriteFloat(f32 value) {
  f32 data = value;
  if (edfile_readwrongendianess)
    EdFileSwapEndianess32(&data);
  EdFileWrite(&data, 4);
}

// FUNCTION: LEGOBATMAN 0x0059cf30
extern "C" void EdFileWriteInt(i32 value) {
  i32 data = value;
  if (edfile_readwrongendianess)
    EdFileSwapEndianess32(&data);
  EdFileWrite(&data, 4);
}

// FUNCTION: LEGOBATMAN 0x0059cf70
extern "C" void EdFileWriteUnsignedInt(u32 value) {
  u32 data = value;
  if (edfile_readwrongendianess)
    EdFileSwapEndianess32(&data);
  EdFileWrite(&data, 4);
}

// FUNCTION: LEGOBATMAN 0x0059cfb0
extern "C" void EdFileWriteShort(i16 value) {
  i16 data = value;
  if (edfile_readwrongendianess)
    EdFileSwapEndianess16(&data);
  EdFileWrite(&data, 2);
}

// FUNCTION: LEGOBATMAN 0x0059cfe0
extern "C" void EdFileWriteUnsignedShort(u16 value) {
  u16 data = value;
  if (edfile_readwrongendianess)
    EdFileSwapEndianess16(&data);
  EdFileWrite(&data, 2);
}

// FUNCTION: LEGOBATMAN 0x0059d010
extern "C" void EdFileWriteChar(char value) { EdFileWrite(&value, 1); }

// FUNCTION: LEGOBATMAN 0x0059d020
extern "C" void EdFileWriteUnsignedChar(u8 value) { EdFileWrite(&value, 1); }

// FUNCTION: LEGOBATMAN 0x0059d030
extern "C" void EdFileWriteNuVec(nuvec_s *value) {
  EdFileWriteFloat(value->x);
  EdFileWriteFloat(value->y);
  EdFileWriteFloat(value->z);
}

// FUNCTION: LEGOBATMAN 0x0059d100
extern "C" void EdFileWriteString(char *s) { EdFileWrite(s, strlen(s)); }

// FUNCTION: LEGOBATMAN 0x0059d130
extern "C" void EdFileWriteStringV(char *fmt, ...) {
  char buf[0x400];
  va_list args;

  va_start(args, fmt);
  vsprintf(buf, fmt, args);
  EdFileWrite(buf, strlen(buf));
}

// FUNCTION: LEGOBATMAN 0x0059d1a0
extern "C" void EdFileRead(void *buf, i32 len) {
  i32 to_read;

  while (len > 0) {
    to_read = 0x1000 - edfile_buffer_pointer;
    if (len < to_read)
      to_read = len;
    if (to_read <= 0)
      return;
    memcpy(buf, edfile_buffer + edfile_buffer_pointer, to_read);
    edfile_buffer_pointer += to_read;
    len -= to_read;
    buf = (char *)buf + to_read;
    if (edfile_buffer_pointer == 0x1000)
      EdFileFillBuffer();
  }
}

// FUNCTION: LEGOBATMAN 0x0059d250
extern "C" void EdFileMovePointer(i32 len) {
  i32 to_move;

  while (len > 0) {
    to_move = 0x1000 - edfile_buffer_pointer;
    if (len < to_move)
      to_move = len;
    edfile_buffer_pointer += to_move;
    len -= to_move;
    if (edfile_buffer_pointer == 0x1000)
      EdFileFillBuffer();
  }
}

// FUNCTION: LEGOBATMAN 0x0059d2d0
extern "C" f32 EdFileReadFloat(void) {
  f32 data;

  EdFileRead(&data, 4);
  if (edfile_readwrongendianess)
    EdFileSwapEndianess32(&data);
  return data;
}

// FUNCTION: LEGOBATMAN 0x0059d310
extern "C" i32 EdFileReadInt(void) {
  i32 data;

  EdFileRead(&data, 4);
  if (edfile_readwrongendianess)
    EdFileSwapEndianess32(&data);
  return data;
}

// FUNCTION: LEGOBATMAN 0x0059d350
extern "C" u32 EdFileReadUnsignedInt(void) {
  u32 data;

  EdFileRead(&data, 4);
  if (edfile_readwrongendianess)
    EdFileSwapEndianess32(&data);
  return data;
}

// FUNCTION: LEGOBATMAN 0x0059d390
extern "C" i16 EdFileReadShort(void) {
  i16 data;

  EdFileRead(&data, 2);
  if (edfile_readwrongendianess)
    EdFileSwapEndianess16(&data);
  return data;
}

// FUNCTION: LEGOBATMAN 0x0059d3c0
extern "C" u16 EdFileReadUnsignedShort(void) {
  u16 data;

  EdFileRead(&data, 2);
  if (edfile_readwrongendianess)
    EdFileSwapEndianess16(&data);
  return data;
}

// FUNCTION: LEGOBATMAN 0x0059d3f0
extern "C" char EdFileReadChar(void) {
  char data;

  EdFileRead(&data, 1);
  return data;
}

// FUNCTION: LEGOBATMAN 0x0059d410
extern "C" u8 EdFileReadUnsignedChar(void) {
  u8 data;

  EdFileRead(&data, 1);
  return data;
}

// FUNCTION: LEGOBATMAN 0x0059d430
extern "C" void EdFileReadNuVec(nuvec_s *out) {
  out->x = EdFileReadFloat();
  out->y = EdFileReadFloat();
  out->z = EdFileReadFloat();
}

// FUNCTION: LEGOBATMAN 0x0059d500
extern "C" i32 EdFileWriteMemCard(char *filename, i32 size, void *data) {
  EdFileSetMedia(2);
  if (EdFileOpen(filename, 1)) {
    EdFileWrite(data, size);
    return EdFileClose();
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x0059d550
extern "C" i32 EdFileReadMemCard(char *filename, i32 size, void *data) {
  EdFileSetMedia(2);
  if (!EdFileOpen(filename, 0))
    return 0;
  EdFileRead(data, size);
  return EdFileClose();
}

// FUNCTION: LEGOBATMAN 0x0059d590
extern "C" i32 EdFileBackup(char *source, char *destination) {
  i64 remaining = NuFileSize(source);
  NUFILE output;
  NUFILE input;

  if (remaining <= 0)
    return 0;
  output = NuFileOpen(destination, 1);
  input = NuFileOpen(source, 0);
  if (input > 0 && output > 0) {
    while (remaining > 0) {
      NuFileRead(input, edfile_buffer,
                 (i32)remaining < 0x1000 ? (i32)remaining : 0x1000);
      NuFileWrite(output, edfile_buffer,
                  (i32)remaining < 0x1000 ? (i32)remaining : 0x1000);
      remaining -= 0x1000;
    }
    NuFileClose(input);
    NuFileClose(output);
    return 1;
  }
  if (input > 0)
    NuFileClose(input);
  if (output > 0)
    NuFileClose(output);
  return 0;
}
