// nu2api/nucore/nupad_gen.cpp: certain range 0x006d5700..0x006e0530 (two
// __FILE__ anchors). The string helpers from ~0x006d7500 on may belong to a
// neighbouring TU; see the report.

#include "common.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

u8 NuToLower(u8 c);

// Empty debug hook; the original's callers know it clobbers nothing, so it
// is defined (not inlined) in this TU.
// FUNCTION: LEGOBATMAN 0x006d24b0
static void Unk006d24b0(...) {}

// Second empty debug hook (get_tok_rdpf's unknown-character report).
// FUNCTION: LEGOBATMAN 0x006d24c0
static void Unk006d24c0(...) {}

// Stands in for the TU's first real caller (0x006dad60 passes it a pointer),
// which is what keeps the calls to the empty hook and tells the compiler it
// clobbers nothing.
void Unk_InlineUser_nupad_gen_debug(char *p) { Unk006d24b0(p); }

// Outside this file; names unknown.
void NuVideoSetBrightnessPS(void);
void NuLanguageSetPS(void);
void NuRegionSetPS(void);
void Unk006dcb00(int);

// 16 pads, 0x84 bytes each (memset(g_nuPads, 0, 0x840) at 0x006d5e60).
struct NuPad {
  unsigned char connected;
  unsigned char pad0[3];
  int i4;
  unsigned char pad1[0x10 - 8];
  int i10;
  int i14;
  unsigned char pad2[0x1c - 0x18];
  int i1c;
  unsigned char pad3[0x84 - 0x20];
};

// GLOBAL: LEGOBATMAN 0x00af8870
NuPad g_nuPads[16];

// Player -> pad mapping (saga g_nupadMapping): 3 entries of 12 bytes.
struct NuPadUnk0aecb64 {
  int pad;
  int value; // port
  int valid; // is_active
};

// GLOBAL: LEGOBATMAN 0x00aecb60
NuPadUnk0aecb64 g_nuPadUnk0aecb64[3];

// Analogue response curve: (x, y) pairs, descending x.
struct NuPadCurvePoint {
  float x;
  float y;
};

// GLOBAL: LEGOBATMAN 0x0099f2b0
NuPadCurvePoint g_nuPadCurve[1];

// GLOBAL: LEGOBATMAN 0x0099f2dc
int g_nuPadUnk099f2dc;
// GLOBAL: LEGOBATMAN 0x0099f308
int g_nuPadUnk099f308;
// GLOBAL: LEGOBATMAN 0x00adf648
int g_nuPadUnk0adf648;
// GLOBAL: LEGOBATMAN 0x00adf64c
int g_nuPadUnk0adf64c;
// GLOBAL: LEGOBATMAN 0x00adf654
int g_nuPadUnk0adf654;
// GLOBAL: LEGOBATMAN 0x00adf658
int g_nuPadUnk0adf658;
// GLOBAL: LEGOBATMAN 0x00adf6e0
float g_nuPadUnk0adf6e0;
// GLOBAL: LEGOBATMAN 0x00b03924
short g_nuPadUnk0b03924; // CurrentStringBank

typedef struct nupad_s {
  int port;  // 0x00
  int slot;  // 0x04
  int stage; // 0x08
  unsigned char pad0c[0x90 - 0x0c];
  unsigned char analog_right_x; // 0x90
  unsigned char analog_right_y; // 0x91
  unsigned char analog_left_x;  // 0x92
  unsigned char analog_left_y;  // 0x93
  unsigned char pad94[0xa4 - 0x94];
} NUPAD;

extern "C" void *NuMemAllocFn(int size, const char *file, int line);
void NuPadOpenPS(NUPAD *pad);

// FUNCTION: LEGOBATMAN 0x006d5700
NUPAD *NuPadOpen(int port, int slot) {
  NUPAD *pad;

  pad = (NUPAD *)NuMemAllocFn(sizeof(NUPAD) + 0x800, __FILE__, 0xb4);

  if (pad != 0) {
    memset(pad, 0, sizeof(NUPAD));

    pad->port = port;
    pad->slot = slot;
    pad->stage = 0;

    pad->analog_right_x = pad->analog_right_y = pad->analog_left_x =
        pad->analog_left_y = 0x80;
  }

  NuPadOpenPS(pad);

  return pad;
}

// FUNCTION: LEGOBATMAN 0x006d5790
float NuPadApplyCurve(float x) {
  if (!(x < 1.0f))
    x = 1.0f;
  int i = 0;
  while (g_nuPadCurve[i].x < x)
    i++;
  return g_nuPadCurve[i].y;
}

// FUNCTION: LEGOBATMAN 0x006d5850
void NuPadSetConnected(int pad, char connected) {
  g_nuPads[pad].connected = connected;
}

// FUNCTION: LEGOBATMAN 0x006d5ca0
void NuPadUnk006d5ca0(int value) { g_nuPadUnk099f2dc = value; }

// FUNCTION: LEGOBATMAN 0x006d5df0
int NuPadUnk006d5df0(int i) {
  if (g_nuPadUnk0aecb64[i].valid)
    return g_nuPadUnk0aecb64[i].value;
  return -1;
}

// FUNCTION: LEGOBATMAN 0x006d5e10
int NuPadUnk006d5e10(int pad) {
  int result = -1;
  if (g_nuPads[pad].connected)
    result = g_nuPads[pad].i4;
  return result;
}

// FUNCTION: LEGOBATMAN 0x006d6500
int NuVideoGetAspect(void) { return g_nuPadUnk0adf658; }

// FUNCTION: LEGOBATMAN 0x006d65d0
int NuVideoGetSwapMode(void) { return g_nuPadUnk0adf654; }

// FUNCTION: LEGOBATMAN 0x006d65e0
void NuVideoRollingFrameRateReset(void) {}

// FUNCTION: LEGOBATMAN 0x006d65f0
void NuVideoSetBrightness(float value) {
  g_nuPadUnk0adf6e0 = value;
  NuVideoSetBrightnessPS();
}

// FUNCTION: LEGOBATMAN 0x006d6830
int NuLanguageGet(void) { return g_nuPadUnk0adf648; }

// FUNCTION: LEGOBATMAN 0x006d6840
void NuLanguageSet(int value) {
  g_nuPadUnk0adf648 = value;
  NuLanguageSetPS();
}

// FUNCTION: LEGOBATMAN 0x006d6850
int NuRegionGet(void) { return g_nuPadUnk0adf64c; }

// FUNCTION: LEGOBATMAN 0x006d6860
void NuRegionSet(int value) {
  g_nuPadUnk0adf64c = value;
  NuRegionSetPS();
}

struct NuPadUnk006d7400 {
  unsigned char b0;
  unsigned char pad[3];
  int i4;
  int i8;
  int ic;

  NuPadUnk006d7400 *Clear();
  NuPadUnk006d7400 *Set(int start, int length);
  NuPadUnk006d7400(int *pool, int size);
  ~NuPadUnk006d7400();
  void setExternalMemoryPool(void *pool, unsigned long size);
  void SetPool(int *pool, int size);
};

// FUNCTION: LEGOBATMAN 0x006d7400
NuPadUnk006d7400 *NuPadUnk006d7400::Clear() {
  b0 = 0;
  i4 = 0;
  i8 = 0;
  ic = 0;
  return this;
}

// Mac VirtualStackAllocator::~VirtualStackAllocator.
// FUNCTION: LEGOBATMAN 0x006d7440
NuPadUnk006d7400::~NuPadUnk006d7400() {
  if (b0 && ic)
    delete (char *)ic;
}

// FUNCTION: LEGOBATMAN 0x006d7460
void NuPadUnk006d7400::setExternalMemoryPool(void *pool, unsigned long size) {
  if (b0 && ic)
    delete (char *)ic;
  i4 = (int)pool;
  ic = (int)pool;
  b0 = 0;
  i8 = (int)pool + size;
}

// from saga nu2api/nucore/nustring_c.cpp
// FUNCTION: LEGOBATMAN 0x006d7f60
f32 NuAToF(char *string) {
  f32 dividend = 0.0f;
  f32 divisor = 1.0f;

  char c = *string;
  string++;

  if (c == '-') {
    divisor = -1.0f;

    c = *string;
    string++;
  }

  while (c >= '0' && c <= '9') {
    dividend *= 10.0f;
    dividend += (f32)(c - 0x30);

    c = *string;
    string++;
  }

  if (c == '.') {
    c = *string;
    string++;

    while (c >= '0' && c <= '9') {
      divisor *= 10.0f;
      dividend *= 10.0f;
      dividend += (f32)(c - 0x30);

      c = *string;
      string++;
    }
  }

  return dividend / divisor;
}

// FUNCTION: LEGOBATMAN 0x006dc180
NuPadUnk006d7400 *NuPadUnk006d7400::Set(int start, int length) {
  i4 = start;
  ic = start;
  b0 = 0;
  i8 = start + length;
  return this;
}

// FUNCTION: LEGOBATMAN 0x006dc120
// pool[1] is the caller's allocation cursor (pool[0] unknown).
void NuPadUnk006d7400::SetPool(int *pool, int size) {
  int align = size < 0x10 ? size : 0x10;
  int start = (pool[1] + align - 1) & ~(align - 1);
  pool[1] = start + size;
  setExternalMemoryPool((void *)start, size);
}

// FUNCTION: LEGOBATMAN 0x006dc1a0
NuPadUnk006d7400::NuPadUnk006d7400(int *pool, int size) {
  b0 = 0;
  i4 = 0;
  i8 = 0;
  ic = 0;
  SetPool(pool, size);
}

// FUNCTION: LEGOBATMAN 0x006d7500
void NuStrCat(char *dst, const char *src) {
  while (*dst)
    dst++;
  if (src) {
    do {
      *dst = *src;
      dst++;
    } while (*src++);
  }
}

// FUNCTION: LEGOBATMAN 0x006d7590
int NuStrCpy(char *dst, const char *src) {
  char *start = dst;
  if (src) {
    while (*src)
      *dst++ = *src++;
  }
  *dst = 0;
  return dst - start;
}

// FUNCTION: LEGOBATMAN 0x006d7610
char *NuStrChr(char *s, char c) {
  for (; *s; s++) {
    if (*s == c)
      return s;
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x006d7670
int NuStrLen(const char *s) {
  int n = 0;
  while (*s) {
    s++;
    n++;
  }
  return n;
}

// FUNCTION: LEGOBATMAN 0x006d7770
int NuStrCmp(const char *a, const char *b) {
  if (!a)
    return -1;
  if (!b)
    return 1;
  char ca;
  char cb;
  do {
    ca = *a;
    cb = *b;
    if (ca > cb)
      return 1;
    if (ca < cb)
      return -1;
    a++;
    b++;
  } while (ca && cb);
  return 0;
}

// FUNCTION: LEGOBATMAN 0x006d77b0
int NuStrNCmp(const char *a, const char *b, int n) {
  if (!a)
    return -1;
  if (!b)
    return 1;
  char ca;
  char cb;
  if (n) {
    if (n == -1)
      n = NuStrLen(a);
    else if (n == -2)
      n = NuStrLen(b);
    do {
      ca = *a;
      cb = *b;
      if (ca > cb)
        return 1;
      if (ca < cb)
        return -1;
      a++;
      b++;
      n--;
    } while (ca && cb && n);
  }
  return 0;
}

// Loop top is unaligned in the original; every form tried emits a pad.
// STUB: LEGOBATMAN 0x006d7900
int NuStrCpyW(unsigned short *dst, const unsigned short *src) {
  int n = 0;
  if (!src) {
    *dst = n;
    return n;
  }
  do {
    *dst++ = *src;
    n++;
  } while (*src++);
  return n;
}

// FUNCTION: LEGOBATMAN 0x006d79f0
int NuStrLenW(const unsigned short *s) {
  int n = 0;
  while (*s) {
    s++;
    n++;
  }
  return n;
}

// FUNCTION: LEGOBATMAN 0x006d8020
int NuAToI(const char *s) {
  int value = 0;
  int sign = 0;
  char c = *s++;
  if (c == '-') {
    c = *s;
    sign = -1;
    s++;
  }
  while (c >= '0' && c <= '9') {
    value = value * 10 + (c - '0');
    c = *s++;
  }
  if (sign)
    return sign * value;
  return value;
}

// FUNCTION: LEGOBATMAN 0x006d8400
unsigned char NuToUpper(unsigned char c) {
  if ((unsigned char)(c - 'a') <= 'z' - 'a' || c >= 0xe0)
    c -= 0x20;
  return c;
}

// FUNCTION: LEGOBATMAN 0x006d8750
void NuStringTableSetFormat(int value) { g_nuPadUnk099f308 = value; }

// FUNCTION: LEGOBATMAN 0x006d8770
void NuStringTableSetBank(unsigned int value) {
  if (value <= 2)
    g_nuPadUnk0b03924 = (short)value;
}

struct NuPadUnk006dd5b0 {
  int a;
  int b;
};

typedef unsigned short NUWCHAR;

typedef struct nustring_s {
  char *id;
  NUWCHAR *str_utf16;
} NUSTRING;

typedef struct nustringbank_s {
  NUSTRING *strings;
  int string_count;
  int max_strings;
} NUSTRINGBANK;

int NuStrICmp(const char *a, const char *b);

// GLOBAL: LEGOBATMAN 0x00b03900
extern NUSTRINGBANK StringBank[3];
// GLOBAL: LEGOBATMAN 0x0099f30c
extern NUWCHAR *NuBlankString;

// FUNCTION: LEGOBATMAN 0x006dc6b0
int NuStringTableGetIdByName(char *name) {
  int j;
  int i;

  for (i = 0; i < 3; i++) {
    for (j = 0; j < StringBank[i].string_count; j++) {
      if (StringBank[i].strings[j].id != NULL &&
          StringBank[i].strings[j].str_utf16 != NULL) {
        if (NuStrICmp(StringBank[i].strings[j].id, name) == 0) {
          return j + (i << 24);
        }
      }
    }
  }

  return -1;
}

// FUNCTION: LEGOBATMAN 0x006dc720
NUWCHAR *NuStringTableGetByName(char *name) {
  int j;
  int i;

  for (i = 0; i < 3; i++) {
    for (j = 0; j < StringBank[i].string_count; j++) {
      if (StringBank[i].strings[j].id != 0 &&
          StringBank[i].strings[j].str_utf16 != 0) {
        if (NuStrICmp(StringBank[i].strings[j].id, name) == 0) {
          return StringBank[i].strings[j].str_utf16;
        }
      }
    }
  }

  return NuBlankString;
}

// FUNCTION: LEGOBATMAN 0x006dd5b0
void NuPadUnk006dd5b0(NuPadUnk006dd5b0 *p) {
  p->a = 0;
  Unk006dcb00(p->b);
}

// FUNCTION: LEGOBATMAN 0x006d7640
char *NuStrRChr(char *s, char c) {
  char *p = s;
  while (*p)
    p++;
  while (p >= s) {
    if (*p == c)
      return p;
    p--;
  }
  return 0;
}

int NuPadWideToAnsi(char *dst, const unsigned short *src);
int NuAsciiToUnicode(unsigned short *dst, const char *src);
int NuStrLenW(const unsigned short *s);

// STUB: LEGOBATMAN 0x006dc5a0
// Ours inlines NuAsciiToUnicode (the original calls it) and swaps the two
// buffers.
int NuSPrintfW(NUWCHAR *dest, NUWCHAR *format, ...) {
  int length = 0;
  va_list args;
  char text[256];
  char ascii_format[256];
  NuPadWideToAnsi(ascii_format, format);
  va_start(args, format);
  vsprintf(text, ascii_format, args);
  va_end(args);
  NuAsciiToUnicode(dest, text);
  return NuStrLenW(dest);
}

// FUNCTION: LEGOBATMAN 0x006d8560
int NuAsciiToUnicode(unsigned short *dst, const char *src) {
  int n = 0;
  *dst = 0;
  if (src) {
    for (; src[n] != 0; n++)
      dst[n] = (unsigned char)src[n];
    dst[n] = 0;
  }
  return n;
}

// FUNCTION: LEGOBATMAN 0x006d7720
char *NuStrStr(char *haystack, const char *needle) {
  while (*haystack) {
    char *h = haystack;
    const char *n = needle;
    while (*n && *h && *h == *n) {
      h++;
      n++;
    }
    if (!*n)
      return haystack;
    haystack++;
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x006d86f0
unsigned char *NuPadUtf8Encode(unsigned char *dst, unsigned short c) {
  if (c < 0x80) {
    *dst++ = (unsigned char)c;
  } else if (c < 0x800) {
    *dst++ = (unsigned char)(c >> 6) | 0xc0;
    *dst++ = (c & 0x3f) | 0x80;
  } else {
    *dst++ = (unsigned char)(c >> 12) | 0xe0;
    *dst++ = ((c >> 6) & 0x3f) | 0x80;
    *dst++ = (c & 0x3f) | 0x80;
  }
  return dst;
}

// --- probably nupad proper ---------------------------------------------------

void Unk00522610(int);

// GLOBAL: LEGOBATMAN 0x0099f2d8
int g_nuPadUnk099f2d8;
// GLOBAL: LEGOBATMAN 0x00b038f4
int g_nuPadUnk0b038f4;
// GLOBAL: LEGOBATMAN 0x00b038f8
int g_nuPadUnk0b038f8;

// FUNCTION: LEGOBATMAN 0x006d57e0
void NuPadUnk006d57e0(int slot, int state) {
  int pad = g_nuPadUnk0aecb64[slot].value;
  if (pad >= 0) {
    if (state)
      g_nuPads[pad].i4 = slot;
    else
      g_nuPads[pad].i4 = -1;
  }
  if (state == 1) {
    if (!g_nuPadUnk0b038f8)
      g_nuPadUnk099f2d8 = pad;
    g_nuPadUnk0b038f8 = 1;
  }
  g_nuPadUnk0aecb64[slot].valid = state;
}

// FUNCTION: LEGOBATMAN 0x006d6100
void NuPadUnk006d6100(int *slot) {
  if (g_nuPadUnk0b038f4) {
    int pad = *slot;
    if (g_nuPads[pad].connected) {
      Unk00522610(pad);
      return;
    }
  }
  int pad = g_nuPadUnk0aecb64[*slot].value;
  if (pad != -1)
    Unk00522610(pad);
}

// FUNCTION: LEGOBATMAN 0x006d6150
void NuPadUnk006d6150(int keep) {
  g_nuPadUnk0b038f8 = 0;
  g_nuPadUnk099f2d8 = -1;
  g_nuPadUnk0aecb64[0].value = -1;
  g_nuPadUnk0aecb64[1].value = -1;
  if (!keep) {
    g_nuPads[0].i14 = 0;
    g_nuPads[0].i1c = 0;
    g_nuPads[0].i10 = 0;
    g_nuPads[1].i14 = 0;
    g_nuPads[1].i1c = 0;
    g_nuPads[1].i10 = 0;
    g_nuPads[2].i14 = 0;
    g_nuPads[2].i1c = 0;
    g_nuPads[2].i10 = 0;
    g_nuPads[3].i14 = 0;
    g_nuPads[3].i1c = 0;
    g_nuPads[3].i10 = 0;
  }
}

// Pad recording (1) / playback (2) of one float per frame.
// GLOBAL: LEGOBATMAN 0x00adf688
float g_nuPadRecValue;
// GLOBAL: LEGOBATMAN 0x00adf69c
int g_nuPadRecMode;
// GLOBAL: LEGOBATMAN 0x00adf6a4
float *g_nuPadRecEnd;
// GLOBAL: LEGOBATMAN 0x00adf6a8
float *g_nuPadRecPos;
// GLOBAL: LEGOBATMAN 0x00adf6c0
int g_nuPadUnk0adf6c0;

// FUNCTION: LEGOBATMAN 0x006d61c0
void NuPadRecStep(void) {
  if (g_nuPadUnk0adf6c0)
    g_nuPadUnk0adf6c0 = 0;
  switch (g_nuPadRecMode) {
  case 1:
    *g_nuPadRecPos = g_nuPadRecValue;
    g_nuPadRecPos++;
    if (g_nuPadRecPos > g_nuPadRecEnd - 0x610)
      g_nuPadRecMode = 0;
    break;
  case 2:
    if (g_nuPadRecPos == g_nuPadRecEnd) {
      g_nuPadRecMode = 0;
    } else {
      g_nuPadRecValue = *g_nuPadRecPos;
      g_nuPadRecPos++;
    }
    break;
  }
}

// --- probably nufile: handle tables by id range -----------------------------

// ids 1..0x3ff: saga's FILEINFO (file_info[id - 1])
struct FILEBUFINFO;
typedef struct fileinfo_s {
  int handle;              // 0x00
  __int64 pos;             // 0x08, read position
  __int64 file_pos;        // 0x10
  __int64 file_len;        // 0x18
  __int64 buf_start;       // 0x20
  int buf_len;             // 0x28
  int use_buf;             // 0x2c
  int unused;              // 0x30
  int i34;                 // 0x34
  int mode;                // 0x38
  struct FILEBUFINFO *buf; // 0x3c
} FILEINFO;
struct FILEBUFINFO {
  FILEINFO *file_info;
  int time;
  char data[0x400];
};
// ids 0x400..0x7ff
struct NuFileUnk0afa0a8 {
  int start;
  int end;
  int i8;
  int ic;
  int i10;
};
// ids 0x800..0xfff
struct NuFileUnk0ae3f80 {
  int *p0;
  int i4;
  __int64 i8;
  __int64 i10;
  int i18;
  int i1c;
  int i20;
  volatile int i24; // is_used
  int i28;
  unsigned char pad[0x30 - 0x2c];
};
// ids 0x2000..
struct NuFileUnk0b00878 {
  int i0;
  int i4;
  int i8;
  int ic;
};

// GLOBAL: LEGOBATMAN 0x00ae3740
FILEINFO file_info[0x21];
// GLOBAL: LEGOBATMAN 0x00afa0a8
NuFileUnk0afa0a8 g_nuFileUnk0afa0a8[20];
// GLOBAL: LEGOBATMAN 0x00ae3f80
NuFileUnk0ae3f80 g_nuFileUnk0ae3f80[20];
// GLOBAL: LEGOBATMAN 0x00b00878
NuFileUnk0b00878 g_nuFileUnk0b00878[1];
// GLOBAL: LEGOBATMAN 0x00b038a4
int g_nuFileUnk0b038a4;

int Unk006d3060(int file);

// FUNCTION: LEGOBATMAN 0x006d9370
__int64 NuFileOpenSize(int id) {
  if (id >= 0x2000)
    return Unk006d3060(id);
  if (id >= 0x1000)
    return 0;
  if (id >= 0x800) {
    NuFileUnk0ae3f80 *e = &g_nuFileUnk0ae3f80[id - 0x800];
    if (e->i28)
      return e->i1c;
    return e->i18;
  }
  if (id >= 0x400)
    return g_nuFileUnk0afa0a8[id - 0x400].end -
           g_nuFileUnk0afa0a8[id - 0x400].start;
  return file_info[id - 1].file_len;
}

// FUNCTION: LEGOBATMAN 0x006d9450
__int64 NuFileUnk006d9450(int id) {
  if (id >= 0x800) {
    NuFileUnk0ae3f80 *e = &g_nuFileUnk0ae3f80[id - 0x800];
    return e->i10 - e->i8;
  }
  id -= 0x400;
  return g_nuFileUnk0afa0a8[id].i8 - g_nuFileUnk0afa0a8[id].start;
}

// FUNCTION: LEGOBATMAN 0x006d9400
void NuFileUnk006d9400(int id) {
  if (id >= 0x800) {
    NuFileUnk0ae3f80 *e = &g_nuFileUnk0ae3f80[id - 0x800];
    int i = e->i20;
    if (i >= 0)
      e->p0[i * 4 + 0xd] = -1;
    e->i24 = 0;
    return;
  }
  g_nuFileUnk0afa0a8[id - 0x400].i10 = 0;
}

extern "C" __int64 NuPSFileLSeek(int fh, __int64 offset, int whence);
extern "C" int NuPSFileClose(int fh);

struct NUFILEDEVICE {
  int id;      // 0x0
  int unit_id; // 0x4
  unsigned char pad8[0x10 - 0x8];
  char separator; // 0x10
  unsigned char pad11[0x34 - 0x11];
  char name[16];      // 0x34
  int match_len;      // 0x44
  int name_len;       // 0x48
  char root[0x20];    // 0x4c
  char desc[0x40];    // 0x6c
  char app_dir[0x80]; // 0xac (saga cur_dir)
  char sys_dir[0x80]; // 0x12c
  char dll_dir[0x80]; // 0x1ac
  int (*format_name_fn)(NUFILEDEVICE *device, char *dst, char *path,
                        int size);             // 0x22c
  int (*interrogate_fn)(NUFILEDEVICE *device); // 0x230
};
// GLOBAL: LEGOBATMAN 0x0099f288
extern NUFILEDEVICE *g_unk0099f288;

// GLOBAL: LEGOBATMAN 0x00af6528
extern NUFILEDEVICE devices[];
// GLOBAL: LEGOBATMAN 0x00af318c
extern int numdevices;
int NuStrNICmp(const char *a, const char *b, int n);

// FUNCTION: LEGOBATMAN 0x006dc9e0
NUFILEDEVICE *NuFileGetDeviceFromPath(char *path) {
  NUFILEDEVICE *device;
  int i;
  int device_idx;

  device = NULL;
  for (i = 0; i < 8; i++) {
    if (path[i] == ':')
      break;
  }
  if (i < 8) {
    for (device_idx = 0; device_idx < numdevices; device_idx++) {
      if (NuStrNICmp(path, devices[device_idx].name,
                     devices[device_idx].match_len) == 0) {
        device = &devices[device_idx];
        break;
      }
    }
  }
  return device;
}

// GLOBAL: LEGOBATMAN 0x00af41c0
extern NUFILEDEVICE enum_devices[];
// GLOBAL: LEGOBATMAN 0x00b00850
int enum_numdevices;
extern int nufile_lasterror;

// FUNCTION: LEGOBATMAN 0x006d2cc0
extern "C" int NuFileGetLastError(void) { return nufile_lasterror; }

// FUNCTION: LEGOBATMAN 0x006d2cd0
NUFILEDEVICE *NuFileFindDevice(int id, int unit) {
  if (id == -3)
    return g_unk0099f288;
  if (id == 1 || id == 3)
    id = 2;
  for (int index = 0; index < numdevices; ++index) {
    if (devices[index].id == id && (unit < 0 || unit == devices[index].unit_id))
      return &devices[index];
  }
  return NULL;
}

// FUNCTION: LEGOBATMAN 0x006d2d40
int NuFileEnumerateDevices(NUFILEDEVICE **result) {
  int count = 0;
  for (int index = 0; index < numdevices; ++index) {
    if ((devices[index].interrogate_fn(&devices[index]) & 2) != 0)
      continue;
    enum_devices[count] = devices[index];
    ++count;
  }
  *result = enum_devices;
  enum_numdevices = count;
  return count;
}

// FUNCTION: LEGOBATMAN 0x006d2dc0
int NuFileGetDevices(NUFILEDEVICE **result) {
  if (result != NULL)
    *result = enum_devices;
  return enum_numdevices;
}

// FUNCTION: LEGOBATMAN 0x006d2de0
int NuFileRefreshDevices(NUFILEDEVICE **result) {
  for (int index = 0; index < numdevices; ++index) {
    if (devices[index].id == 1)
      devices[index].id = -1;
  }
  if (result != NULL)
    *result = devices;
  return numdevices;
}

// FUNCTION: LEGOBATMAN 0x006d2e20
void DebugHandler(void) { printf("Waiting for restart...\n"); }

// FUNCTION: LEGOBATMAN 0x006d2e30
int NuFileGetMediaMode(void) { return 0; }

// FUNCTION: LEGOBATMAN 0x006d2e40
NUFILEDEVICE *NuFileGetCurrentDevice(void) { return g_unk0099f288; }

// FUNCTION: LEGOBATMAN 0x006d2e50
void NuFileSetCurrentDevice(NUFILEDEVICE *device) { g_unk0099f288 = device; }

// FUNCTION: LEGOBATMAN 0x006d2e60
int NuFileGetCurrentPath(char *dest) {
  return g_unk0099f288->format_name_fn(g_unk0099f288, dest, "", 0x7fff);
}

// FUNCTION: LEGOBATMAN 0x006d2e90
int NuFileGetCurrentSysPath(char *dest) {
  return g_unk0099f288->format_name_fn(g_unk0099f288, dest, "", 0x7fff);
}

// FUNCTION: LEGOBATMAN 0x006d2ec0
int NuFileFormatName(char *dest, char *name, int capacity) {
  return g_unk0099f288->format_name_fn(g_unk0099f288, dest, name, capacity);
}

// FUNCTION: LEGOBATMAN 0x006d2ef0
int NuMemFileOpenSize(int file) {
  file -= 0x400;
  return g_nuFileUnk0afa0a8[file].end - g_nuFileUnk0afa0a8[file].start;
}

// FUNCTION: LEGOBATMAN 0x006d2f10
int NuMcFileOpenSize(int file) { return 0; }

// GLOBAL: LEGOBATMAN 0x0099f280
extern char *iop_image_name;
int NuStrCpy(char *dst, const char *src);
void NuStrCat(char *dst, const char *src);
int NuStrLen(const char *s);

void NuFileUpCase(NUFILEDEVICE *device, char *filepath);

// FUNCTION: LEGOBATMAN 0x006d91d0
void NuFileSetCurrentSysDirectory(char *path) {
  char *source;
  char *dest;
  char *separator = "\\";
  if (*path != '\0') {
    NuStrCpy(g_unk0099f288->sys_dir, path);
    NuStrCat(g_unk0099f288->sys_dir, separator);
  } else {
    NuStrCpy(g_unk0099f288->sys_dir, "sys");
    source = iop_image_name + 5;
    dest = g_unk0099f288->sys_dir + strlen(g_unk0099f288->sys_dir);
    while (*source != '.') {
      *dest = *source;
      ++dest;
      ++source;
    }
    *dest++ = '\\';
    *dest = '\0';
  }
  NuFileUpCase(g_unk0099f288, g_unk0099f288->sys_dir);
}

// FUNCTION: LEGOBATMAN 0x006d9290
void NuFileGetCurrentDllPath(char *dest) {
  NuStrCpy(dest, g_unk0099f288->root);
  NuStrCat(dest, g_unk0099f288->dll_dir);
  NuFileUpCase(g_unk0099f288, dest + NuStrLen(g_unk0099f288->root));
}

// FUNCTION: LEGOBATMAN 0x006d9320
void NuFileSetCurrentDllDirectory(char *path) {
  char *separator = "\\";
  if (*path != '\0') {
    NuStrCpy(g_unk0099f288->dll_dir, path);
    NuStrCat(g_unk0099f288->dll_dir, separator);
  } else {
    g_unk0099f288->dll_dir[0] = '\0';
  }
}

// FUNCTION: LEGOBATMAN 0x006d98d0
int NuFileAppendPath(char *dest, char *path, char *name) {
  NuStrCpy(dest, path);
  int length = strlen(dest);
  char *end = dest + length;
  // The original reads past the terminator before writing the separator.
  if (*end++ != '\\' && *end++ != '/')
    *end++ = '\\';
  NuStrCpy(end, name);
  return NuStrLen(dest);
}

// FUNCTION: LEGOBATMAN 0x006d9970
int NuFileExtractFilename(char *dest, char *path) {
  char *start = strrchr(path, '\\');
  char *slash = strrchr(path, '/');
  if (slash > start)
    start = slash;
  if (start != NULL)
    ++start;
  else
    start = path;
  NuStrCpy(dest, start);
  return NuStrLen(dest);
}

// FUNCTION: LEGOBATMAN 0x006d99e0
int NuFileExtractExt(char *dest, char *path) {
  char *start = strrchr(path, '.');
  if (start != NULL)
    ++start;
  else
    start = path;
  NuStrCpy(dest, start);
  return NuStrLen(dest);
}

__int64 NuFilePos(int file);

// FUNCTION: LEGOBATMAN 0x006d95e0
int NuFileEOF(int file) {
  __int64 position = NuFilePos(file);
  __int64 size = NuFileOpenSize(file);
  return position >= size;
}

// FUNCTION: LEGOBATMAN 0x006d8f90
void FixFilenamePath(char *path) {
  char *p;
  char *q;

  while (NuStrStr(path, "../") == path || NuStrStr(path, "..\\") == path)
    path += NuStrLen("../");
  for (;;) {
    p = NuStrStr(path, "../");
    if (p == NULL) {
      p = NuStrStr(path, "..\\");
      if (p == NULL)
        return;
    }
    q = p - 1;
    if (*q == '\\' || *q == '/')
      q--;
    while (*q != '\\' && *q != '/' && q > path)
      q--;
    q[1] = '\0';
    NuStrCat(path, p + 3);
  }
}

// FUNCTION: LEGOBATMAN 0x006d9060
NUFILEDEVICE *AddDevice(NUFILEDEVICE *device) {
  devices[numdevices] = *device;
  NuStrCpy(devices[numdevices].app_dir, g_unk0099f288->app_dir);
  NuStrCpy(devices[numdevices].sys_dir, g_unk0099f288->sys_dir);
  NuStrCpy(devices[numdevices].dll_dir, g_unk0099f288->dll_dir);
  ++numdevices;
  return &devices[numdevices - 1];
}

// FUNCTION: LEGOBATMAN 0x006d9130
void NuFileGetAppDirectory(char *dest) {
  NuStrCpy(dest, g_unk0099f288->app_dir);
}

// FUNCTION: LEGOBATMAN 0x006d9160
void NuFileSetAppDirectory(char *path) {
  int length;
  char separator[2] = "\\";
  if (*path != '\0') {
    length = NuStrCpy(g_unk0099f288->app_dir, path);
    if (length != 0 && path[length - 1] != '\\' && path[length - 1] != '/') {
      separator[0] = g_unk0099f288->separator;
      NuStrCat(g_unk0099f288->app_dir, separator);
    }
  } else {
    g_unk0099f288->app_dir[0] = '\0';
  }
}

// STUB: LEGOBATMAN 0x006d9540
// The 0x2000 branch indexes through esi (orig: lea eax, keeps esi).
__int64 NuFilePos(int file) {
  __int64 pos;

  if (file >= 0x2000)
    return g_nuFileUnk0b038a4 == 0 ? 0 : g_nuFileUnk0b00878[file - 0x2000].i4;
  if (file >= 0x1000)
    return 0;
  if (file >= 0x400)
    return NuFileUnk006d9450(file);
  file -= 1;
  FILEINFO *info = &file_info[file];
  if (info->use_buf != 0)
    return info->pos;
  pos = NuPSFileLSeek(file, 0, 1);
  while (pos < 0)
    pos = NuPSFileLSeek(file, 0, 1);
  return pos;
}

// --- more string helpers
// ------------------------------------------------------

// STUB: LEGOBATMAN 0x006d7540
int NuStrNCat(char *dst, const char *src, int n) {
  int count = 0;
  while (*dst)
    dst++;
  if (src) {
    while (n) {
      *dst = *src;
      char c = *src;
      dst++;
      count++;
      n--;
      src++;
      if (!c)
        break;
    }
  }
  return count;
}

// STUB: LEGOBATMAN 0x006d75c0
int NuStrNCpy(char *dst, const char *src, int n) {
  int count = 0;
  if (!src) {
    *dst = 0;
    return 0;
  }
  char c;
  do {
    n--;
    count++;
    if (n <= 0) {
      *dst = 0;
      return count - 1;
    }
    *dst = *src;
    c = *src;
    dst++;
    src++;
  } while (c);
  return count - 1;
}

// STUB: LEGOBATMAN 0x006d7690
int NuPadUtf8StrLen(const char *s) {
  int n = 0;
  int i = 0;
  if (*s) {
    do {
      unsigned char c = s[i + 1];
      i++;
      n++;
      while (c >= 0x80 && c <= 0xbf) {
        c = s[i + 1];
        i++;
      }
    } while (s[i]);
  }
  return n;
}

// STUB: LEGOBATMAN 0x006d76d0
int NuPadUtf8ByteLen(const char *s, int maxChars) {
  int i = 0;
  int n = 0;
  if (!maxChars)
    return i;
  if (*s) {
    do {
      unsigned char c = s[i + 1];
      i++;
      n++;
      while (c >= 0x80 && c <= 0xbf) {
        c = s[i + 1];
        i++;
      }
    } while (maxChars != n && s[i]);
  }
  return i;
}

// FUNCTION: LEGOBATMAN 0x006d7d60
void NuPadWStrCpyLower(unsigned short *dst, const unsigned short *src) {
  while (*src) {
    unsigned short c = *src;
    if ((unsigned short)(c - 'A') <= 'Z' - 'A' ||
        (unsigned short)(c - 0xc0) <= 0x1f)
      c += 0x20;
    *dst++ = c;
    src++;
  }
  *dst = *src;
}

// FUNCTION: LEGOBATMAN 0x006d7dc0
extern "C" int NuSPrintf(char *buf, char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vsprintf(buf, fmt, args);
  return NuStrLen(buf);
}

// FUNCTION: LEGOBATMAN 0x006d7df0
int NuIsAlNum(char c) {
  switch (c) {
  case '0':
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case '8':
  case '9':
  case 'A':
  case 'B':
  case 'C':
  case 'D':
  case 'E':
  case 'F':
  case 'G':
  case 'H':
  case 'I':
  case 'J':
  case 'K':
  case 'L':
  case 'M':
  case 'N':
  case 'O':
  case 'P':
  case 'Q':
  case 'R':
  case 'S':
  case 'T':
  case 'U':
  case 'V':
  case 'W':
  case 'X':
  case 'Y':
  case 'Z':
  case '_':
  case 'a':
  case 'b':
  case 'c':
  case 'd':
  case 'e':
  case 'f':
  case 'g':
  case 'h':
  case 'i':
  case 'j':
  case 'k':
  case 'l':
  case 'm':
  case 'n':
  case 'o':
  case 'p':
  case 'q':
  case 'r':
  case 's':
  case 't':
  case 'u':
  case 'v':
  case 'w':
  case 'x':
  case 'y':
  case 'z':
    return 1;
  default:
    return 0;
  }
}

// FUNCTION: LEGOBATMAN 0x006d84e0
void NuPadWideToUtf8(unsigned char *dst, const unsigned short *src) {
  if (!src)
    return;
  if (!dst)
    return;
  *dst = 0;
  while (*src) {
    if (*src < 0x80) {
      *dst++ = (unsigned char)*src;
    } else if (*src < 0x800) {
      *dst++ = ((*src >> 6) & 0x1f) | 0xc0;
      *dst++ = (*src & 0x3f) | 0x80;
    } else {
      *dst++ = ((*src >> 12) & 0xf) | 0xe0;
      *dst++ = ((*src >> 6) & 0x3f) | 0x80;
      *dst++ = (*src & 0x3f) | 0x80;
    }
    src++;
  }
  *dst = 0;
}

// FUNCTION: LEGOBATMAN 0x006d85a0
void NuUTF8ToUnicode(unsigned short *dst, unsigned char *src) {
  if (src == NULL)
    return;
  if (dst == NULL)
    return;
  *dst = 0;
  while (*src != 0) {
    if (*src < 0x80) {
      *dst = *src;
      ++src;
    } else if (*src < 0xe0) {
      *dst = (*src & 0x1f) << 6;
      ++src;
      *dst |= *src & 0x3f;
      ++src;
    } else if (*src < 0xf0) {
      *dst = (*src & 0x0f) << 12;
      ++src;
      *dst |= (*src & 0x3f) << 6;
      ++src;
      *dst |= *src & 0x3f;
      ++src;
    } else {
      *dst = '?';
      ++src;
    }
    ++dst;
  }
  *dst = 0;
}

// FUNCTION: LEGOBATMAN 0x006d8650
unsigned char *NuUnicodeCharFromUTF8(unsigned short *dst, unsigned char *src) {
  unsigned short c[3];

  c[0] = *src++;
  if (c[0] < 0x80) {
    *dst = c[0];
  } else if ((c[0] & 0xe0) == 0xc0) {
    c[1] = *src++;
    *dst = ((c[0] & 0x1f) << 6) | (c[1] & 0x3f);
  } else if ((c[0] & 0xf0) == 0xe0) {
    c[1] = *src++;
    c[2] = *src++;
    *dst = (c[0] << 12) | ((c[1] & 0x3f) << 6) | (c[2] & 0x3f);
  }
  return src;
}

// STUB: LEGOBATMAN 0x006d8440
int NuPadWideToAnsi(char *dst, const unsigned short *src) {
  int n = 0;
  if (!src)
    return n;
  if (!dst)
    return n;
  *dst = 0;
  if (*src) {
    do {
      unsigned short c = src[n];
      if (c & 0xff00) {
        switch (c) {
        case 0x2018:
          dst[n] = (char)0x91;
          break;
        case 0x2019:
          dst[n] = (char)0x92;
          break;
        case 0x2013:
          dst[n] = (char)0x96;
          break;
        case 0x2026:
          dst[n] = (char)0x85;
          break;
        case 0x2122:
          dst[n] = (char)0x99;
          break;
        default:
          dst[n] = '?';
          break;
        }
      } else {
        dst[n] = (char)c;
      }
      n++;
    } while (src[n]);
  }
  dst[n] = 0;
  return n;
}

// Same range tests as NuToUpper above (ASCII plus the Latin-1 letters).
#define NU_IS_LOWER(c)                                                         \
  ((unsigned char)((c) - 'a') <= 'z' - 'a' || (unsigned char)(c) >= 0xe0)
#define NU_IS_UPPER(c)                                                         \
  ((unsigned char)((c) - 'A') <= 'Z' - 'A' ||                                  \
   (unsigned char)((c) + 0x40) <= 0x1f)

// Register choice only (cl/dl swapped in the inner loop); logic verified.
// STUB: LEGOBATMAN 0x006dc1d0
// Second compare loop: the original keeps the pattern char in cl and
// folds through al; ours swaps the two (2 bytes longer).
int NuStrICmpWC(char *pattern, char *text, char *wildcard) {
  int index;
  char a, b;
  int suffix_length, text_length, wildcard_length;
  if (pattern == NULL)
    return -1;
  if (text == NULL)
    return 1;
  while (*pattern != '\0' && *pattern != '*') {
    a = NuToUpper((unsigned char)*pattern);
    b = NuToUpper((unsigned char)*text);
    if (a > b)
      return 1;
    if (a < b)
      return -1;
    ++pattern;
    ++text;
  }
  if (*pattern == '*') {
    ++pattern;
    suffix_length = NuStrLen(pattern);
    text_length = NuStrLen(text);
    wildcard_length = text_length - suffix_length;
    if (wildcard_length < 0)
      return 1;
    if (wildcard != NULL) {
      for (index = 0; index < wildcard_length; ++index)
        wildcard[index] = text[index];
      wildcard[index] = '\0';
    }
    text += text_length - suffix_length;
  } else if (wildcard != NULL) {
    *wildcard = '\0';
  }
  while (*pattern != '\0') {
    b = NuToUpper((unsigned char)*text);
    a = NuToUpper((unsigned char)*pattern);
    if (a > b)
      return 1;
    if (a < b)
      return -1;
    ++pattern;
    ++text;
  }
  return 0;
}

// STUB: LEGOBATMAN 0x006dc300
char *NuStrIStr(char *str, const char *sub) {
  const char *p;
  char *q;
  char a;
  char b;
  while (*str) {
    q = str;
    for (p = sub; *p; p++, q++) {
      a = *q;
      if (!a)
        break;
      if (NU_IS_LOWER(a))
        a -= 0x20;
      b = *p;
      if (NU_IS_LOWER(b))
        b -= 0x20;
      if (a != b)
        break;
    }
    if (!*p)
      return str;
    str++;
  }
  return 0;
}

// GLOBAL: LEGOBATMAN 0x00b038fc
int g_nuStrUnk0b038fc;

// Debug counter of exact string matches (name unknown, no callers).
// FUNCTION: LEGOBATMAN 0x006dc380
void NuStrUnk006dc380(const char *a, const char *b) {
  if (NuStrCmp(a, b) == 0)
    g_nuStrUnk0b038fc++;
}

// FUNCTION: LEGOBATMAN 0x006dc3a0
int NuStrICmp(const char *a, const char *b) {
  if (!a)
    return -1;
  if (!b)
    return 1;
  char ca;
  char cb;
  do {
    ca = *a;
    if (NU_IS_LOWER(ca))
      ca -= 0x20;
    cb = *b;
    if (NU_IS_LOWER(cb))
      cb -= 0x20;
    if (ca > cb)
      return 1;
    if (ca < cb)
      return -1;
    a++;
    b++;
  } while (ca && cb);
  return 0;
}

// FUNCTION: LEGOBATMAN 0x006dc410
int NuStrNICmp(const char *a, const char *b, int n) {
  if (!a)
    return -1;
  if (!b)
    return 1;
  char ca;
  char cb;
  if (n) {
    if (n == -1)
      n = NuStrLen(a);
    else if (n == -2)
      n = NuStrLen(b);
    do {
      ca = *a;
      if (NU_IS_LOWER(ca))
        ca -= 0x20;
      cb = *b;
      if (NU_IS_LOWER(cb))
        cb -= 0x20;
      if (ca > cb)
        return 1;
      if (ca < cb)
        return -1;
      a++;
      b++;
      n--;
    } while (ca && cb && n);
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x006dc4b0
void NuStrUpr(char *dst, const char *src) {
  char c;
  while ((c = *src) != 0) {
    if (NU_IS_LOWER(c))
      c -= 0x20;
    src++;
    *dst++ = c;
  }
  *dst = *src;
}

// FUNCTION: LEGOBATMAN 0x006dc540
int NuHexStringToI(char *string) {
  int value = 0;
  for (; *string != 0; string++) {
    value <<= 4;
    char c = *string;
    if (NU_IS_LOWER(c))
      c -= 0x20;
    int upper = (unsigned char)c;
    if (upper <= '9' && upper >= '0')
      value |= upper - 0x30;
    else if (upper <= 'F' && upper >= 'A')
      value |= upper - 0x37;
    else
      return 0;
  }
  return value;
}

// FUNCTION: LEGOBATMAN 0x006dc630
int NuStringTok(char *str, ...) {
  char *token;
  int value;
  va_list args;
  va_start(args, str);
  do {
    token = va_arg(args, char *);
    value = va_arg(args, int);
    if (token != NULL) {
      if (NuStrICmp(str, token) == 0)
        break;
    }
  } while (token != NULL);
  va_end(args);
  return value;
}

// FUNCTION: LEGOBATMAN 0x006dc670
void NuStrToLower(char *str) {
  char *cursor = str;
  while (*cursor != 0) {
    *cursor = NuToLower(*cursor);
    ++cursor;
  }
}

// FUNCTION: LEGOBATMAN 0x006dc4f0
void NuStrLwr(char *dst, const char *src) {
  char c;
  while ((c = *src) != 0) {
    if (NU_IS_UPPER(c))
      c += 0x20;
    src++;
    *dst++ = c;
  }
  *dst = *src;
}

// File extension table (saga nu2api/nufile/nufile.cpp, PC order).
typedef struct fileextinfo_s {
  char extension[13];
  char type;
  char platform;
  char len;
} FILEEXTINFO;

// GLOBAL: LEGOBATMAN 0x00aecb88
static FILEEXTINFO extensions[0x60];
// GLOBAL: LEGOBATMAN 0x00ad7630
static int num_extensions;
// GLOBAL: LEGOBATMAN 0x0099f270
extern int g_unk0099f270;

int Unk00524cf0(void);

// FUNCTION: LEGOBATMAN 0x006d9fe0
static int MatchExtension(char *extension, char *path_end, int path_len) {
  while (*extension != '\0') {
    --path_end;
    if (path_len == 0)
      return 0;
    char c = *path_end;
    if (NU_IS_UPPER(c))
      c += 0x20;
    if (*extension != c)
      return 0;
    ++extension;
    --path_len;
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x006da040
static void AddExtension(char *extension, int type, int platform) {
  FILEEXTINFO *next = &extensions[num_extensions];
  int len = NuStrLen(extension);
  next->len = len;
  char *ext = next->extension;
  while (len != 0) {
    len--;
    *ext = extension[len];
    ext++;
  }
  *ext = '\0';
  next->type = type;
  next->platform = platform;
  num_extensions++;
}

// FUNCTION: LEGOBATMAN 0x006da090
extern "C" void NuFileExtInitEx(int flags) {
  memset(extensions, 0, sizeof(extensions));
  num_extensions = 0;
  if (flags != 0)
    flags = g_unk0099f270;
  AddExtension("_360.an3", 0, 6);
  AddExtension(".cbs", 1, 6);
  AddExtension("_360.ghg", 2, 6);
  AddExtension("_360.gsc", 3, 6);
  AddExtension("_360.tex", 4, 6);
  AddExtension("_360.cu2", 5, 6);
  AddExtension(".wavx", 6, 6);
  AddExtension(".wavm", 7, 6);
  AddExtension(".wmv", 8, 6);
  AddExtension("_ps3.an3", 0, 5);
  AddExtension(".cbs", 1, 5);
  AddExtension("_ps3.ghg", 2, 5);
  AddExtension("_ps3.gsc", 3, 5);
  AddExtension("_ps3.tex", 4, 5);
  AddExtension("_ps3.cu2", 5, 5);
  AddExtension(".msf", 6, 5);
  AddExtension(".mib", 7, 5);
  AddExtension(".pam", 8, 5);
  if (flags & 1)
    AddExtension("_lr_pc.an3", 0, 4);
  else
    AddExtension("_pc.an3", 0, 4);
  if (flags & 2)
    AddExtension("_lr_pc.cu2", 5, 4);
  else
    AddExtension("_pc.cu2", 5, 4);
  if (flags & 4)
    AddExtension("_lr_pc.gsc", 3, 4);
  else
    AddExtension("_pc.gsc", 3, 4);
  if (flags & 8)
    AddExtension("_lr_pc.ghg", 2, 4);
  else
    AddExtension("_pc.ghg", 2, 4);
  AddExtension(".bsa", 1, 4);
  AddExtension("_pc.dds", 4, 4);
  AddExtension(".wav", 6, 4);
  AddExtension(".mib", 7, 4);
  AddExtension(".bik", 8, 4);
  AddExtension("_ngc.an3", 0, 7);
  AddExtension(".cbs", 1, 7);
  AddExtension("_wii.ghg", 2, 7);
  AddExtension("_wii.gsc", 3, 7);
  AddExtension("_wii.tex", 4, 7);
  AddExtension("_ngc.cu2", 5, 7);
  AddExtension(".dsp", 6, 7);
  AddExtension(".gcm", 7, 7);
  AddExtension(".h4m", 8, 7);
  AddExtension("_xbx.an3", 0, 1);
  AddExtension(".bsa", 1, 1);
  AddExtension(".hx2", 2, 1);
  AddExtension(".nx2", 3, 1);
  AddExtension(".dds", 4, 1);
  AddExtension(".cu2", 5, 1);
  AddExtension(".wavx", 6, 1);
  AddExtension(".wavm", 7, 1);
  AddExtension(".wmv", 8, 1);
  AddExtension("_psp.an3", 0, 3);
  AddExtension(".bsa", 1, 3);
  AddExtension("_psp.ghg", 2, 3);
  AddExtension("_psp.gsc", 3, 3);
  AddExtension("_psp.tex", 4, 3);
  AddExtension("_psp.cu2", 5, 3);
  AddExtension(".vag", 6, 3);
  AddExtension(".at3", 7, 3);
  AddExtension(".pss", 8, 3);
  AddExtension("_ps2.an3", 0, 0);
  AddExtension(".bsa", 1, 0);
  AddExtension(".ghg", 2, 0);
  AddExtension(".gsc", 3, 0);
  AddExtension(".pnt", 4, 0);
  AddExtension("_ps2.cu2", 5, 0);
  AddExtension(".vag", 6, 0);
  AddExtension(".mib", 7, 0);
  AddExtension(".pss", 8, 0);
  AddExtension(".an3", 0, -1);
  AddExtension(".ca3", 0, -1);
  AddExtension(".cu2", 5, -1);
  AddExtension(".cc2", 5, -1);
  AddExtension(".hgo", 2, -1);
  AddExtension(".nus", 3, -1);
  AddExtension(".rcu", 5, -1);
  AddExtension(".ran", 0, -1);
}

// FUNCTION: LEGOBATMAN 0x006da500
extern "C" void NuFileExtInit(void) { NuFileExtInitEx(Unk00524cf0() == 0); }

// FUNCTION: LEGOBATMAN 0x006da520
static FILEEXTINFO *NuFileExtGetInfo(char *path, int path_len) {
  if (path_len == 0)
    path_len = NuStrLen(path);
  char *path_end = path + path_len;
  FILEEXTINFO *info = extensions;
  while (info->extension[0] != '\0') {
    if (MatchExtension(info->extension, path_end, path_len) != 0)
      return info;
    ++info;
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x006da580
extern "C" int NuFileExtGetType(char *path, int path_len) {
  FILEEXTINFO *info = NuFileExtGetInfo(path, path_len);
  if (info != 0)
    return info->type;
  return -1;
}

// FUNCTION: LEGOBATMAN 0x006da5a0
extern "C" int NuFileExtConvert(char *dest, char *path) {
  int path_len = NuStrCpy(dest, path);
  FILEEXTINFO *source = NuFileExtGetInfo(path, path_len);
  if (source != 0) {
    int source_type = source->type;
    if (source->platform == 4)
      return 1;
    FILEEXTINFO *target;
    for (target = extensions; target != 0; target++) {
      if (target->platform == 4 && target->type == source_type) {
        char *out = &dest[path_len - source->len];
        char *ext = &target->extension[target->len - 1];
        int i;
        for (i = 0; i < target->len; i++)
          *out++ = *ext--;
        *out = '\0';
        return 1;
      }
    }
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x006da650
extern "C" int NuFileExtRemove(char *dest, char *path) {
  int length = NuStrCpy(dest, path);
  FILEEXTINFO *info = NuFileExtGetInfo(path, length);
  if (info != 0) {
    dest[length - info->len] = '\0';
    return info->type;
  }
  return -1;
}

void NuMemCpy(unsigned char *dst, unsigned char *src, int n);

// Small-file (pak item, ids 0x2000..) slot, close, read, seek and size;
// Mac NuFileSmallFileManager::GetNextSlot/Close/Read/Seek/Size.
// FUNCTION: LEGOBATMAN 0x006d2f20
int Unk006d2f20(void) {
  while (g_nuFileUnk0b00878[0].i0 != -1) {
  }
  g_nuFileUnk0b00878[0].i0 = -2;
  return 0;
}

// FUNCTION: LEGOBATMAN 0x006d2f40
void Unk006d2f40(int file) {
  int idx;

  if (g_nuFileUnk0b038a4 != 0) {
    idx = file - 0x2000;
    g_nuFileUnk0b00878[idx].i0 = -1;
    g_nuFileUnk0b00878[idx].i4 = -1;
    g_nuFileUnk0b00878[idx].i8 = -1;
    g_nuFileUnk0b00878[idx].ic = -1;
  }
}

// FUNCTION: LEGOBATMAN 0x006d2f80
int Unk006d2f80(int file, void *buf, int size) {
  int idx;
  int n;

  if (g_nuFileUnk0b038a4 == 0)
    return 0;
  idx = file - 0x2000;
  n = g_nuFileUnk0b00878[idx].ic - g_nuFileUnk0b00878[idx].i4;
  if (n > size)
    n = size;
  NuMemCpy((unsigned char *)buf,
           (unsigned char *)(g_nuFileUnk0b00878[idx].i8 +
                             g_nuFileUnk0b00878[idx].i4),
           n);
  g_nuFileUnk0b00878[idx].i4 += n;
  return n;
}

// FUNCTION: LEGOBATMAN 0x006d2fe0
int Unk006d2fe0(int file, int offset, int whence) {
  int idx;

  if (g_nuFileUnk0b038a4 == 0)
    return 0;
  idx = file - 0x2000;
  switch (whence) {
  case 0:
    g_nuFileUnk0b00878[idx].i4 = offset;
    break;
  case 1:
    g_nuFileUnk0b00878[idx].i4 += offset;
    break;
  case 2:
    g_nuFileUnk0b00878[idx].i4 = g_nuFileUnk0b00878[idx].ic + offset;
    break;
  }
  return g_nuFileUnk0b00878[idx].i4;
}
// FUNCTION: LEGOBATMAN 0x006d3060
int Unk006d3060(int file) {
  if (g_nuFileUnk0b038a4 == 0)
    return 0;
  return g_nuFileUnk0b00878[file - 0x2000].ic;
}

__int64 NuMemFileSeek(int file, __int64 offset, int whence);

__int64 NuDatFileSeek(int file, __int64 offset, int whence);

// FUNCTION: LEGOBATMAN 0x006d94a0
__int64 NuMemFileSeek(int file, __int64 offset, int whence) {
  if (file >= 0x800)
    return NuDatFileSeek(file, offset, whence);
  file -= 0x400;
  switch (whence) {
  default:
    g_nuFileUnk0afa0a8[file].i8 = g_nuFileUnk0afa0a8[file].start + (int)offset;
    break;
  case 1:
    g_nuFileUnk0afa0a8[file].i8 = g_nuFileUnk0afa0a8[file].i8 + (int)offset;
    break;
  case 2:
    g_nuFileUnk0afa0a8[file].i8 = g_nuFileUnk0afa0a8[file].end - (int)offset;
    break;
  }
  return g_nuFileUnk0afa0a8[file].i8 - g_nuFileUnk0afa0a8[file].start;
}

// FUNCTION: LEGOBATMAN 0x006d9620
__int64 NuFileSeek(int file, __int64 offset, int whence) {
  FILEINFO *info;

  if (file >= 0x2000)
    return Unk006d2fe0(file, (int)offset, whence);
  if (file >= 0x1000)
    return 0;
  if (file >= 0x400)
    return NuMemFileSeek(file, (int)offset, whence);
  file -= 1;
  info = &file_info[file];
  if (info->use_buf) {
    switch (whence) {
    default:
      info->pos = offset;
      break;
    case 1:
      info->pos += offset;
      break;
    case 2:
      info->pos = info->file_len - offset;
      break;
    }
  } else {
    return NuPSFileLSeek(file, offset, whence);
  }
  return info->pos;
}

struct NUDATOPENFILEINFO {
  int dat_file; // 0x0
  int info_idx; // 0x4
  __int64 pos;  // 0x8
};

// FUNCTION: LEGOBATMAN 0x006d38b0
__int64 NuDatFileSeek(int file, __int64 offset, int whence) {
  NuFileUnk0ae3f80 *info;
  NUDATOPENFILEINFO *open_info;
  __int64 offset_in_dat;

  file -= 0x800;
  info = &g_nuFileUnk0ae3f80[file];
  open_info = &((NUDATOPENFILEINFO *)info->p0)[info->i20 + 3];
  switch (whence) {
  default:
    offset_in_dat = offset + info->i8;
    break;
  case 1:
    offset_in_dat = offset + info->i10;
    break;
  case 2:
    offset_in_dat = info->i8 + info->i18 - offset;
    break;
  }
  info->i10 = NuFileSeek(open_info->dat_file, offset_in_dat, 0);
  open_info->pos = info->i10;
  return info->i10;
}

void NuPSFileInitDevices(int device_id, int reboot_iop, int eject);
extern "C" int NuThreadCreateCriticalSection(void);
// GLOBAL: LEGOBATMAN 0x00ad7640
int file_criticalsection;
// GLOBAL: LEGOBATMAN 0x00b00854
static NuFileUnk0ae3f80 *unpack_file_info;
// GLOBAL: LEGOBATMAN 0x00af90b4
static NUDATOPENFILEINFO *unpack_file_odi;
// GLOBAL: LEGOBATMAN 0x00ad7638
static __int64 unpack_file_pos;
// GLOBAL: LEGOBATMAN 0x00aecb84
static int decode_buffer_left;
// Buffer sizes unknown (saga's 0x40000 would overlap neighbouring globals).
// GLOBAL: LEGOBATMAN 0x00ae4558
static char decode_buffer[0x10000];
// GLOBAL: LEGOBATMAN 0x00ae434c
static int decode_buffer_pos;
// GLOBAL: LEGOBATMAN 0x00ad7648
static char read_buffer[0xc000];
// GLOBAL: LEGOBATMAN 0x00af886c
static volatile int read_buffer_size;
// GLOBAL: LEGOBATMAN 0x00afa23c
static volatile int read_buffer_decoded_size;

// FUNCTION: LEGOBATMAN 0x006d2ab0
static void NuDatFileDecodeInit(void) {
  unpack_file_info = 0;
  unpack_file_odi = 0;
  unpack_file_pos = -1;
}

// FUNCTION: LEGOBATMAN 0x006d2ae0
static void NuDatFileDecodeNewFile(NuFileUnk0ae3f80 *file,
                                   NUDATOPENFILEINFO *open_file) {
  unpack_file_info = file;
  unpack_file_odi = open_file;
  unpack_file_pos = file->i8;
  decode_buffer_left = 0;
}

int NuFileRead(int file, void *buf, int size);
int ExplodeBufferSize(char *buf);
int ExplodeCompressedSize(char *buf);
int ExplodeBufferNoHeader(char *in_buf, char *out_buf, int compressed_size,
                          int orig_size);

// STUB: LEGOBATMAN 0x006d8ed0
// The original reloads unpack_file_info for each half of the 64-bit pos and
// adds in registers; ours does a read-modify-write.
static void NuDatFileDecodeNext(void) {
  char char_buf[12];

  NuFileRead(unpack_file_odi->dat_file, char_buf, 0xc);
  read_buffer_decoded_size = ExplodeBufferSize(char_buf);
  read_buffer_size = ExplodeCompressedSize(char_buf) - 0xc;
  NuFileRead(unpack_file_odi->dat_file, read_buffer, read_buffer_size);
  unpack_file_info->i10 += read_buffer_size + 0xc;
  unpack_file_odi->pos = unpack_file_info->i10;
}

// FUNCTION: LEGOBATMAN 0x006de270
int NuDatFileRead(int file, void *buf, int size) {
  NuFileUnk0ae3f80 *info;
  NUDATOPENFILEINFO *open_file;
  int total_read;
  int bytes_read;

  file -= 0x800;
  info = &g_nuFileUnk0ae3f80[file];
  open_file = &((NUDATOPENFILEINFO *)info->p0)[info->i20 + 3];

  if (info->i28 == 2) {
    total_read = 0;
    while (size != 0) {
      if (decode_buffer_left == 0) {
        NuDatFileDecodeNext();
        if (read_buffer_size == read_buffer_decoded_size)
          memcpy(decode_buffer, read_buffer, read_buffer_size);
        else
          ExplodeBufferNoHeader(read_buffer, decode_buffer, read_buffer_size,
                                read_buffer_decoded_size);
        decode_buffer_pos = 0;
        decode_buffer_left = read_buffer_decoded_size;
      }
      bytes_read = size < decode_buffer_left ? size : decode_buffer_left;
      memcpy(buf, decode_buffer + decode_buffer_pos, bytes_read);
      buf = (void *)((char *)buf + bytes_read);
      decode_buffer_pos += bytes_read;
      decode_buffer_left -= bytes_read;
      size -= bytes_read;
      total_read += bytes_read;
    }
    return total_read;
  }

  if (info->i10 != open_file->pos) {
    NuFileSeek(open_file->dat_file, info->i10, 0);
    open_file->pos = info->i10;
  }
  bytes_read = size < (0 > info->i18 + info->i8 - info->i10
                           ? 0
                           : info->i18 + info->i8 - info->i10)
                   ? size
                   : (0 > info->i18 + info->i8 - info->i10
                          ? 0
                          : info->i18 + info->i8 - info->i10);
  if (bytes_read != 0) {
    total_read = NuFileRead(open_file->dat_file, buf, bytes_read);
    if (total_read >= 0) {
      info->i10 += total_read;
      open_file->pos = info->i10;
      return total_read;
    }
    return total_read;
  }
  return 0;
}

// STUB: LEGOBATMAN 0x006dca60
// Store scheduling: ours writes unpack_file_pos (-1) before the two NULLs.
int NuFileInitEx(int device_id, int reboot_iop, int eject) {
  NuPSFileInitDevices(device_id, reboot_iop, eject);
  memset(g_nuFileUnk0afa0a8, 0, 0x190);
  memset(g_nuFileUnk0ae3f80, 0, 0x3c0);
  file_criticalsection = NuThreadCreateCriticalSection();
  NuFileExtInit();
  NuDatFileDecodeInit();
  return device_id;
}

// FUNCTION: LEGOBATMAN 0x006dcae0
void NuFileInit(int device_id) { NuFileInitEx(device_id, 1, 0); }

// FUNCTION: LEGOBATMAN 0x006dcb00
void NuFileClose(int file) {
  FILEINFO *info;

  if (file >= 0x2000) {
    if (g_nuFileUnk0b038a4) {
      g_nuFileUnk0b00878[file - 0x2000].i0 = -1;
      g_nuFileUnk0b00878[file - 0x2000].i4 = -1;
      g_nuFileUnk0b00878[file - 0x2000].i8 = -1;
      g_nuFileUnk0b00878[file - 0x2000].ic = -1;
    }
    return;
  }
  if (file >= 0x1000)
    return;
  if (file >= 0x400) {
    NuFileUnk006d9400(file);
    return;
  }
  file -= 1;
  while (NuPSFileClose(file) < 0)
    Unk006d24b0();
  info = &file_info[file];
  if (info->buf != 0)
    info->buf->file_info = 0;
  memset(info, 0, sizeof(FILEINFO));
}

extern "C" int NuPSFileRead(int fh, void *buf, int size);
// GLOBAL: LEGOBATMAN 0x00af3190
static FILEBUFINFO file_buff[4];
// GLOBAL: LEGOBATMAN 0x00b03888
static int file_time_count;

// FUNCTION: LEGOBATMAN 0x006d30c0
static void AquireFileBuffer(FILEINFO *info) {
  int i;
  int buf_idx;
  int time;
  FILEBUFINFO *buf;

  buf_idx = 0;
  time = file_time_count;
  for (i = 0; i < 4; i++) {
    if (file_buff[i].time < time) {
      time = file_buff[i].time;
      buf_idx = i;
    }
  }
  buf = &file_buff[buf_idx];
  if (buf->file_info != 0)
    buf->file_info->buf = 0;
  buf->file_info = info;
  buf->time = file_time_count++;
  info->buf = buf;
  if (info->buf_len != 0) {
    NuPSFileLSeek(info->handle, -info->buf_len, 1);
    NuPSFileRead(info->handle, buf->data, info->buf_len);
  }
}

// FUNCTION: LEGOBATMAN 0x006d3160
int NuMemFileOpen(void *buf, int buf_size, int mode) {
  int i;

  if (buf_size > 0 && (mode == 0 || mode == 1)) {
    for (i = 0; i < 20; i++) {
      if (!g_nuFileUnk0afa0a8[i].i10) {
        g_nuFileUnk0afa0a8[i].start = (int)buf;
        g_nuFileUnk0afa0a8[i].end = (int)buf + buf_size - 1;
        g_nuFileUnk0afa0a8[i].i8 = g_nuFileUnk0afa0a8[i].start;
        g_nuFileUnk0afa0a8[i].ic = mode;
        g_nuFileUnk0afa0a8[i].i10 = 1;
        return i + 0x400;
      }
    }
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x006d31f0
void *NuMemFileAddr(int file) {
  return (void *)g_nuFileUnk0afa0a8[file - 0x400].i8;
}

// FUNCTION: LEGOBATMAN 0x006d3210
int NuMemFileWrite(int file, void *data, int size) {
  file -= 0x400;
  size = g_nuFileUnk0afa0a8[file].end - g_nuFileUnk0afa0a8[file].i8 + 1 < size
             ? g_nuFileUnk0afa0a8[file].end - g_nuFileUnk0afa0a8[file].i8 + 1
             : size;
  if (size != 0) {
    memcpy((void *)g_nuFileUnk0afa0a8[file].i8, data, size);
    g_nuFileUnk0afa0a8[file].i8 += size;
  }
  return size;
}

extern "C" int NuPSFileWrite(int fh, void *buf, int size);

// FUNCTION: LEGOBATMAN 0x006d3260
int NuFileWrite(int file, void *data, int size) {
  if (file >= 0x1000)
    return 0;
  if (file >= 0x400)
    return NuMemFileWrite(file, data, size);
  file -= 1;
  return NuPSFileWrite(file, data, size);
}

// FUNCTION: LEGOBATMAN 0x006d3520
int NuFileWriteString(int file, char *str) {
  return NuFileWrite(file, str, strlen(str));
}

// FUNCTION: LEGOBATMAN 0x006d3570
int NuFileWriteStringV(int file, const char *format, ...) {
  int length;
  va_list args;
  char text[1024];

  va_start(args, format);
  vsprintf(text, format, args);
  va_end(args);
  length = strlen(text);
  return NuFileWrite(file, text, length);
}

// Copies the directory part of path (up to the last '\\' or '/', inclusive)
// into dest; returns its length.
// FUNCTION: LEGOBATMAN 0x006d3640
int NuFileExtractPath(char *dest, char *path) {
  char *p;
  char *q;
  int len;

  p = strrchr(path, '\\');
  q = strrchr(path, '/');
  if (q > p)
    p = q;
  if (p != NULL)
    p++;
  else
    p = path;
  len = p - path;
  strncpy(dest, path, len);
  dest[len] = '\0';
  return len;
}

// Copies the file name of path without its extension into dest; returns its
// length.
// FUNCTION: LEGOBATMAN 0x006d3690
int NuFileExtractFile(char *dest, char *path) {
  char *start;
  char *q;
  char *end;
  int len;

  start = strrchr(path, '\\');
  q = strrchr(path, '/');
  if (q > start)
    start = q;
  if (start != NULL)
    start++;
  else
    start = path;
  end = strrchr(path, '.');
  if (end == NULL)
    end = path + strlen(path);
  len = end - start;
  strncpy(dest, start, len);
  dest[len] = '\0';
  return len;
}

// FUNCTION: LEGOBATMAN 0x006d3880
__int64 NuDatFilePos(int file) {
  NuFileUnk0ae3f80 *info;

  file -= 0x800;
  info = g_nuFileUnk0ae3f80 + file;
  return info->i10 - info->i8;
}

// FUNCTION: LEGOBATMAN 0x006d3970
void NuDatFileClose(int file) {
  NuFileUnk0ae3f80 *info;

  file -= 0x800;
  info = g_nuFileUnk0ae3f80 + file;
  if (info->i20 >= 0)
    info->p0[info->i20 * 4 + 0xd] = -1;
  info->i24 = 0;
}

// FUNCTION: LEGOBATMAN 0x006d3ad0
int NuDatFileOpenSize(int file) {
  NuFileUnk0ae3f80 *info;

  file -= 0x800;
  info = &g_nuFileUnk0ae3f80[file];
  if (info->i28 != 0)
    return info->i1c;
  return info->i18;
}

// NuFileInitAddress's address tables (saga nufile_plain.cpp); 0x006d37a0
// frees them.
// GLOBAL: LEGOBATMAN 0x00b038a8
static int MAX_FILE_ADDRESS;
struct NuFileAddress {
  void *address;
  __int64 position;
};
// GLOBAL: LEGOBATMAN 0x00b038ac
static NuFileAddress *addr_ref;
// GLOBAL: LEGOBATMAN 0x00b038b0
static NuFileAddress *addr_pos;
// GLOBAL: LEGOBATMAN 0x00b0085c
static int pos_count;
// GLOBAL: LEGOBATMAN 0x00b03878
static int ref_count;
extern "C" void NuMemFreeFn(void *ptr, const char *file, int line);

// FUNCTION: LEGOBATMAN 0x006d3700
void NuFileInitAddress(int count) {
  if (count != MAX_FILE_ADDRESS) {
    if (addr_ref != NULL)
      NuMemFreeFn(addr_ref, __FILE__, 0xc5e);
    if (addr_pos != NULL)
      NuMemFreeFn(addr_pos, __FILE__, 0xc5f);
    MAX_FILE_ADDRESS = count;
    addr_ref = (NuFileAddress *)NuMemAllocFn(count * 16, __FILE__, 0xc63);
    addr_pos =
        (NuFileAddress *)NuMemAllocFn(MAX_FILE_ADDRESS * 16, __FILE__, 0xc64);
  }
  pos_count = 0;
  ref_count = 0;
}

// FUNCTION: LEGOBATMAN 0x006d37a0
void NuFileTidyAddress(void) {
  if (addr_ref != NULL) {
    NuMemFreeFn(addr_ref, __FILE__, 0xc6e);
    addr_ref = NULL;
  }
  if (addr_pos != NULL) {
    NuMemFreeFn(addr_pos, __FILE__, 0xc73);
    addr_pos = NULL;
  }
  MAX_FILE_ADDRESS = 0;
}

__int64 NuFilePos(int file);
int NuFileWrite(int file, void *data, int size);

// FUNCTION: LEGOBATMAN 0x006d9a40
void NuFileWriteAddress(int file, void *address) {
  // GLOBAL: LEGOBATMAN 0x00b03934
  static int pad;
  if (address != NULL) {
    addr_ref[ref_count].address = address;
    addr_ref[ref_count].position = NuFilePos(file);
    ++ref_count;
  }
  NuFileWrite(file, &pad, 4);
}

// FUNCTION: LEGOBATMAN 0x006d9ac0
void NuFileSetAddress(int file, void *address) {
  int index;
  if (address != NULL) {
    for (index = 0; index < pos_count; ++index) {
      if (addr_pos[index].address == address)
        break;
    }
    if (index == pos_count)
      ++pos_count;
    addr_pos[index].address = address;
    addr_pos[index].position = NuFilePos(file);
  }
}

// FUNCTION: LEGOBATMAN 0x006d9b30
void NuFilePatchAddress(int file) {
  int ref;
  int pos;
  __int64 offset;
  for (ref = 0; ref < ref_count; ++ref) {
    for (pos = 0; pos < pos_count; ++pos) {
      if (addr_ref[ref].address == addr_pos[pos].address) {
        offset = addr_pos[pos].position - addr_ref[ref].position;
        NuFileSeek(file, addr_ref[ref].position, 0);
        NuFileWrite(file, &offset, 4);
      }
    }
  }
  NuFileSeek(file, 0, 2);
}

struct nufile_info_s {
  unsigned char pad0[0x10];
  char second; // 0x10
  char minute; // 0x11
  char hour;   // 0x12
  char day;    // 0x13
  char month;  // 0x14
  char pad15;
  short year; // 0x16
};

// FUNCTION: LEGOBATMAN 0x006d37f0
int NuFileIsNewer(nufile_info_s *first, nufile_info_s *second) {
  if (second->year > first->year)
    return 1;
  if (second->year < first->year)
    return 0;
  if (second->month > first->month)
    return 1;
  if (second->month < first->month)
    return 0;
  if (second->day > first->day)
    return 1;
  if (second->day < first->day)
    return 0;
  if (second->hour > first->hour)
    return 1;
  if (second->hour < first->hour)
    return 0;
  if (second->minute > first->minute)
    return 1;
  if (second->minute < first->minute)
    return 0;
  if (second->second > first->second)
    return 1;
  if (second->second < first->second)
    return 0;
  return 0;
}

// GLOBAL: LEGOBATMAN 0x00b0389c
int NuFile_SwapEndianOnWrite;

// FUNCTION: LEGOBATMAN 0x006d2b60
int NuFileSwapEndianOnWrite(int enabled) {
  int previous = NuFile_SwapEndianOnWrite;
  NuFile_SwapEndianOnWrite = enabled;
  return previous;
}

// FUNCTION: LEGOBATMAN 0x006d2b70
int NuFileGetEndianSwap(void) { return NuFile_SwapEndianOnWrite; }

// FUNCTION: LEGOBATMAN 0x006d2b10
static void NuFileEndianSwap16(void *value) {
  unsigned char *value_ptr;
  unsigned short swapped;
  unsigned char *swap_ptr;

  value_ptr = (unsigned char *)value;
  swap_ptr = (unsigned char *)&swapped;
  swap_ptr[0] = value_ptr[1];
  swap_ptr[1] = value_ptr[0];
  *((unsigned short *)value) = swapped;
}

// FUNCTION: LEGOBATMAN 0x006d2b30
static void NuFileEndianSwap32(void *value) {
  unsigned char *value_ptr;
  unsigned int swapped;
  unsigned char *swap_ptr;

  value_ptr = (unsigned char *)value;
  swap_ptr = (unsigned char *)&swapped;
  swap_ptr[0] = value_ptr[3];
  swap_ptr[1] = value_ptr[2];
  swap_ptr[2] = value_ptr[1];
  swap_ptr[3] = value_ptr[0];
  *((unsigned int *)value) = swapped;
}

// FUNCTION: LEGOBATMAN 0x006d3290
int NuFileWriteFloat(int file, float value) {
  float tmp = value;
  if (NuFile_SwapEndianOnWrite != 0)
    NuFileEndianSwap32(&tmp);
  return NuFileWrite(file, &tmp, sizeof(tmp));
}

// FUNCTION: LEGOBATMAN 0x006d3300
int NuFileWriteInt(int file, int value) {
  int tmp;

  tmp = value;
  if (NuFile_SwapEndianOnWrite != 0)
    NuFileEndianSwap32(&tmp);
  return NuFileWrite(file, &tmp, sizeof(int));
}

// FUNCTION: LEGOBATMAN 0x006d3370
unsigned int NuFileWriteUnsignedInt(int file, unsigned int value) {
  unsigned int tmp;

  tmp = value;
  if (NuFile_SwapEndianOnWrite != 0)
    NuFileEndianSwap32(&tmp);
  return NuFileWrite(file, &tmp, sizeof(unsigned int));
}

// FUNCTION: LEGOBATMAN 0x006d33e0
int NuFileWriteShort(int file, short value) {
  short tmp = value;
  if (NuFile_SwapEndianOnWrite != 0)
    NuFileEndianSwap16(&tmp);
  return NuFileWrite(file, &tmp, sizeof(tmp));
}

// FUNCTION: LEGOBATMAN 0x006d3440
int NuFileWriteUnsignedShort(int file, unsigned short value) {
  unsigned short tmp = value;
  if (NuFile_SwapEndianOnWrite != 0)
    NuFileEndianSwap16(&tmp);
  return NuFileWrite(file, &tmp, sizeof(tmp));
}

// FUNCTION: LEGOBATMAN 0x006d34a0
int NuFileWriteChar(int file, char value) {
  return NuFileWrite(file, &value, sizeof(value));
}

// FUNCTION: LEGOBATMAN 0x006d34e0
int NuFileWriteUnsignedChar(int file, unsigned char value) {
  return NuFileWrite(file, &value, sizeof(value));
}

int NuDatFileRead(int file, void *buf, int size);

// FUNCTION: LEGOBATMAN 0x006de800
int NuMemFileRead(int file, void *buf, int size) {
  if (file >= 0x800)
    return NuDatFileRead(file, buf, size);
  file -= 0x400;
  size = g_nuFileUnk0afa0a8[file].end - g_nuFileUnk0afa0a8[file].i8 + 1 < size
             ? g_nuFileUnk0afa0a8[file].end - g_nuFileUnk0afa0a8[file].i8 + 1
             : size;
  if (size != 0) {
    memcpy(buf, (void *)g_nuFileUnk0afa0a8[file].i8, size);
    g_nuFileUnk0afa0a8[file].i8 = g_nuFileUnk0afa0a8[file].i8 + size;
  }
  return size;
}

// FUNCTION: LEGOBATMAN 0x006de860
int NuFileRead(int file, void *buf, int size) {
  FILEINFO *info;
  int bytes_read;
  char *char_buf;
  int available;

  if (file >= 0x2000)
    return Unk006d2f80(file, buf, size);
  if (file >= 0x1000)
    return 0;
  if (file >= 0x400)
    return NuMemFileRead(file, buf, size);
  file -= 1;
  info = &file_info[file];
  if (info->use_buf) {
    if (info->buf == 0)
      AquireFileBuffer(info);
    bytes_read = 0;
    char_buf = (char *)buf;
    while (size > 0) {
      if (info->pos >= info->file_len)
        return bytes_read;
      if (info->pos < info->buf_start ||
          info->pos >= info->buf_start + info->buf_len) {
        if (info->pos != info->file_pos) {
          NuPSFileLSeek(info->handle, info->pos, 0);
          info->file_pos = info->pos;
        }
        info->buf_len = NuPSFileRead(info->handle, info->buf->data, 0x400);
        info->buf_start = info->file_pos;
        info->file_pos += info->buf_len;
      }
      available = info->buf_len + (int)(info->buf_start - info->pos) < size
                      ? info->buf_len + (int)(info->buf_start - info->pos)
                      : size;

      if (available != 0)
        memcpy(char_buf,
               (char *)info->buf + (int)(info->pos - info->buf_start) + 8,
               available);
      char_buf += available;
      bytes_read += available;
      size -= available;
      info->pos += available;
    }
    return bytes_read;
  }
  return NuPSFileRead(file, buf, size);
}

typedef struct nufpar_s {
  char *file_buf;             // 0x00
  int file_handle;            // 0x04
  char file_name[0x100];      // 0x08
  char line_buf_store[0x402]; // 0x108
  char word_buf_store[0x402]; // 0x50a
  char *line_buf;             // 0x90c
  char *word_buf;             // 0x910
  int line_buf_size;          // 0x914
  int word_buf_size;          // 0x918
  int line_num;               // 0x91c
  int line_pos;               // 0x920
  int char_pos;               // 0x924
  int buf_start;              // 0x928
  int buf_end;                // 0x92c
  unsigned char pad930[0x970 - 0x930];
  int command_pos; // 0x970
  int size;        // 0x974
  char is_utf16;   // 0x978
  char is_utf8;    // 0x979
  char pad97a[2];
  void *separator_list;   // 0x97c
  void *separator_tokens; // 0x980
} NUFPAR;

// Static: NuFParGetWordW keeps line_pos in a register across the store. So
// nufile/nufpar_unk.cpp (extern old_line_pos) belongs in this TU.
// GLOBAL: LEGOBATMAN 0x00ae4344
static int old_line_pos;

#define CLAMP_WIDE_LINE(pos) pos &((parser->line_buf_size >> 1) - 1)
#define CLAMP_WIDE_WORD(pos) pos &((parser->word_buf_size >> 1) - 1)

// FUNCTION: LEGOBATMAN 0x006da710
int NuFParGetWordW(NUFPAR *parser) {
  unsigned short *line = (unsigned short *)parser->line_buf;
  unsigned short *word = (unsigned short *)parser->word_buf;
  int len = 0;
  int in_quoted_text = 0;
  unsigned short c;

  old_line_pos = parser->line_pos;
  while (line[CLAMP_WIDE_LINE(parser->line_pos)] != 0) {
    c = line[parser->line_pos];
    if (parser->separator_tokens != NULL && !in_quoted_text) {
      if (NuStrChr((char *)parser->separator_tokens, c) != NULL) {
        if (len == 0) {
          word[len] = c;
          len++;
          parser->line_pos++;
        }
        word[CLAMP_WIDE_WORD(len)] = '\0';
        return len;
      }
    }
    if (parser->separator_list != NULL && !in_quoted_text) {
      if (NuStrChr((char *)parser->separator_list, c) != NULL)
        c = ' ';
    }
    switch (c & 0xff) {
    case ' ':
    case ',':
    case '\t':
      if (!in_quoted_text) {
        if (len != 0) {
          word[CLAMP_WIDE_WORD(len)] = '\0';
          return len;
        }
        break;
      }
    default:
      if (c == '"') {
        if (in_quoted_text && line[parser->line_pos + 1] == '"') {
          word[CLAMP_WIDE_WORD(len)] = c;
          len++;
          parser->line_pos++;
        } else {
          in_quoted_text = 1 - in_quoted_text;
        }
      } else {
        word[CLAMP_WIDE_WORD(len)] = c;
        len++;
      }
      break;
    }
    parser->line_pos++;
  }
  word[CLAMP_WIDE_WORD(len)] = '\0';
  return len;
}

#define CLAMP_LINE(pos) pos &(parser->line_buf_size - 1)
#define CLAMP_WORD(pos) pos &(parser->word_buf_size - 1)

// FUNCTION: LEGOBATMAN 0x006da8d0
int NuFParGetWord(NUFPAR *parser) {
  int len;
  int in_quoted_text;
  int found_quotes = 0;

  if (parser->is_utf16)
    return NuFParGetWordW(parser);
  len = 0;
  in_quoted_text = 0;
  old_line_pos = parser->line_pos;
  while (parser->line_buf[CLAMP_LINE(parser->line_pos)] != 0) {
    char c = parser->line_buf[parser->line_pos];

    if (parser->separator_tokens != NULL && !in_quoted_text) {
      if (NuStrChr((char *)parser->separator_tokens, c) != NULL) {
        if (len == 0) {
          parser->word_buf[len] = c;
          len++;
          parser->line_pos++;
        }
        parser->word_buf[CLAMP_WORD(len)] = '\0';
        return len;
      }
    }
    if (parser->separator_list != NULL && !in_quoted_text) {
      if (NuStrChr((char *)parser->separator_list, c) != NULL)
        c = ' ';
    }
    switch (c) {
    case ' ':
    case ',':
    case '\t':
      if (!in_quoted_text) {
        if (len != 0) {
          parser->word_buf[CLAMP_WORD(len)] = '\0';
          return len;
        }
        break;
      }
    default:
      if (c == '"') {
        if (in_quoted_text && len == 0) {
          parser->word_buf[0] = ' ';
          len = 1;
        }
        in_quoted_text = 1 - in_quoted_text;
        found_quotes = 1;
      } else {
        parser->word_buf[CLAMP_WORD(len)] = c;
        len++;
      }
      break;
    }
    parser->line_pos++;
    if (found_quotes && in_quoted_text == 0 && len == 0)
      break;
  }
  parser->word_buf[CLAMP_WORD(len)] = '\0';
  return len;
}

// FUNCTION: LEGOBATMAN 0x006d3e20
void NuFParUnGetWord(NUFPAR *parser) { parser->line_pos = old_line_pos; }

// FUNCTION: LEGOBATMAN 0x006dd570
void NuFParSuspend(NUFPAR *parser) {
  if (parser->file_handle >= 0x400 && parser->file_handle < 0x800)
    return;
  if (parser->file_name[0] == '\0' || parser->file_handle == 0)
    Unk006d24b0();
  NuFileClose(parser->file_handle);
  parser->file_handle = 0;
}

struct NuPadRec {
  int mode;                 // 0x0
  unsigned char *buf_start; // 0x4
  unsigned char *buf_end;   // 0x8
  unsigned char *record;    // 0xc
};

// GLOBAL: LEGOBATMAN 0x00adf69c
extern NuPadRec g_nuPadRec;
// GLOBAL: LEGOBATMAN 0x00b03884
extern int g_unk00b03884;

struct NUDATHDR;
int NuFileOpenDF(char *filepath, int mode, NUDATHDR *header, int quiet);

// Opens through the current data file system (the global swapped by
// NuWin32SetDFS).
// FUNCTION: LEGOBATMAN 0x006dd970
int NuFileOpen(char *path, int mode) {
  return NuFileOpenDF(path, mode, (NUDATHDR *)g_unk00b03884, 0);
}

// FUNCTION: LEGOBATMAN 0x006d3b10
extern "C" NUDATHDR *NuDatSet(NUDATHDR *header) {
  NUDATHDR *dat = (NUDATHDR *)g_unk00b03884;
  g_unk00b03884 = (int)header;
  return dat;
}

// FUNCTION: LEGOBATMAN 0x006d3b20
extern "C" NUDATHDR *NuDatGet(void) { return (NUDATHDR *)g_unk00b03884; }

struct NUDATFILEINFO {
  int i0;
  int i4;
  int len; // 0x8
  int ic;
};
struct NUDFNODE {
  short child_idx;   // 0x0
  short sibling_idx; // 0x2
  char *name;        // 0x4
};
struct NUDATHDR {
  int version;
  int file_count;
  NUDATFILEINFO *file_info;        // 0x8
  int tree_node_count;             // 0xc
  NUDFNODE *file_tree;             // 0x10
  int leaf_names_len;              // 0x14
  char *leaf_names;                // 0x18
  unsigned int *hash_idxs;         // 0x1c
  int hash_count;                  // 0x20
  int hashes_len;                  // 0x24
  char *hashes;                    // 0x28
  NUDATOPENFILEINFO open_files[8]; // 0x30
  short unknown;                   // 0xb0
  short mode;                      // 0xb2
  int unknown2;                    // 0xb4
  int unknown3;                    // 0xb8
  char *name;                      // 0xbc
};
// GLOBAL: LEGOBATMAN 0x00b0388c
extern int nufile_buffering_enabled;

// FUNCTION: LEGOBATMAN 0x006dc7a0
int NuStrFixExtPlatform(char *dst, char *src, char *ext, int dst_size,
                        char *platform_string) {
  char platform_ext[64];
  char *end;
  char *period;
  char *sep;
  char *win_sep;
  int len;
  int ext_len;

  if (dst == NULL || src == NULL || ext == NULL || *src == '\0' ||
      dst_size == 0)
    return 0;
  len = NuStrLen(src);
  end = src + len;
  if (end[-1] == '\\' || end[-1] == '/')
    return 0;
  NuStrCpy(platform_ext, "PC");
  NuStrCat(platform_ext, ".");
  NuStrCat(platform_ext, ext);
  ext_len = NuStrLen(platform_ext);
  if (len > ext_len && NuStrNICmp(end - ext_len, platform_ext, ext_len) == 0) {
    NuStrCpy(dst, src);
    return 1;
  }
  NuStrCpy(dst, src);
  period = NuStrRChr(dst, '.');
  sep = NuStrRChr(dst, '/');
  win_sep = NuStrRChr(dst, '\\');
  if (win_sep > sep)
    sep = win_sep;
  if (sep >= period) {
    NuStrCat(dst, "_");
    if (platform_string != NULL)
      NuStrCat(dst, platform_string);
    else
      NuStrCat(dst, "PC");
    NuStrCat(dst, ".");
    NuStrCat(dst, ext);
  } else {
    *period = '\0';
    NuStrCat(period, "_");
    if (platform_string != NULL)
      NuStrCat(period, platform_string);
    else
      NuStrCat(period, "PC");
    NuStrCat(period, ".");
    NuStrCat(period, ext);
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x006dc970
int NuStrFixExt(char *dst, char *src, char *ext, int dst_size) {
  return NuStrFixExtPlatform(dst, src, ext, dst_size, NULL);
}

// FUNCTION: LEGOBATMAN 0x006dc990
bool NuFileIsTxt(char *path) {
  if (NuStrICmp(NuStrRChr(path, '.'), ".txt") == 0)
    return true;
  return false;
}

int NuFileSmallFileOpen(char *path);
int NuDatFileOpen(NUDATHDR *hdr, char *filepath, int mode);
extern "C" int NuPSFileOpen(char *path, int mode);
void Unk006d24d0(...);
// FUNCTION: LEGOBATMAN 0x006d3080
int NuFileStatus(int file) {
  int _unused = 0;

  if (file >= 0x400)
    return 0;
  if (file >= 0x800) {
    file -= 0x800;
    NuFileUnk0ae3f80 *info = g_nuFileUnk0ae3f80 + file;
    NUDATOPENFILEINFO *open_info =
        &((NUDATOPENFILEINFO *)info->p0)[info->i20 + 3];
    return NuFileStatus(open_info->dat_file);
  }
  file -= 1;
  return _unused;
}

// FUNCTION: LEGOBATMAN 0x006dd7a0
int NuFileOpenDF(char *filepath, int mode, NUDATHDR *header, int quiet) {
  char buf[256];
  __int64 len;
  NUFILEDEVICE *device;
  int file_index;
  int file_handle;

  if (NuFileIsTxt(filepath)) {
    file_handle = NuFileSmallFileOpen(filepath);
    if (file_handle != 0)
      return file_handle;
  }

  device = NuFileGetDeviceFromPath(filepath);
  if (device == NULL) {
    if (header != NULL && mode != 1 && mode != 2) {
      file_handle = NuDatFileOpen(header, filepath, mode);
      if (file_handle != 0)
        return file_handle;
    }
    device = g_unk0099f288;
  }

  device->format_name_fn(device, buf, filepath, 0x100);

  if (device->id == 1)
    return 0x1000;

  file_index = NuPSFileOpen(buf, mode);
  if (file_index < 0)
    return 0;

  memset(&file_info[file_index], 0, sizeof(FILEINFO));
  file_info[file_index].handle = file_index;
  file_info[file_index].mode = mode;

  if (mode != 1) {
    if (file_index <= 16) {
      do {
        len = NuFileSeek(file_index + 1, 0, 2);
        if (len < 0 && !quiet)
          Unk006d24d0();
      } while (len < 0);
      file_info[file_index].file_len = len;
      if (mode == 3) {
        while (NuFileStatus(file_index + 1) != 0) {
        }
      }
      do {
        len = NuFileSeek(file_index + 1, 0, 0);
        if (len < 0 && !quiet)
          Unk006d24d0();
      } while (len < 0);
      if (mode == 3) {
        while (NuFileStatus(file_index + 1) != 0) {
        }
      }
    }
  } else {
    file_info[file_index].file_len = 0;
  }

  if (mode == 0)
    file_info[file_index].use_buf = nufile_buffering_enabled;
  file_info[file_index].unused = 0;
  return file_index + 1;
}

// FUNCTION: LEGOBATMAN 0x006d3940
static unsigned int NameToHash(char *name) {
  unsigned int hash = 0x811c9dc5;
  while (*name != '\0') {
    hash = (*name ^ hash) * 0x199933;
    name++;
  }
  return hash;
}

static __forceinline int NuStrCmpInline(const char *a, const char *b) {
  if (!a)
    return -1;
  if (!b)
    return 1;
  char ca;
  char cb;
  do {
    ca = *a;
    cb = *b;
    if (ca > cb)
      return 1;
    if (ca < cb)
      return -1;
    a++;
    b++;
  } while (ca && cb);
  return 0;
}

// FUNCTION: LEGOBATMAN 0x006d9f00
int NuDatFileFindHash(NUDATHDR *header, char *name) {
  unsigned int hash = NameToHash(name);
  int i;

  for (i = 0; i < header->file_count; i++) {
    if (header->hash_idxs[i] == hash)
      return i;
  }
  if (header->hash_count != 0) {
    VARIPTR ptr;
    ptr.char_ptr = header->hashes;
    for (i = 0; i < header->hash_count; i++) {
      if (NuStrCmpInline(ptr.char_ptr, name) == 0) {
        ptr.addr += NuStrLen(ptr.char_ptr) + 1;
        if ((ptr.addr & 1) != 0)
          ptr.addr++;
        return *ptr.i16_ptr;
      }
      ptr.addr += NuStrLen(ptr.char_ptr) + 3;
      if ((ptr.addr & 1) != 0)
        ptr.addr++;
    }
  }
  return -1;
}
// GLOBAL: LEGOBATMAN 0x00b03880
int DontDoFileUpperCaseHack;

// FUNCTION: LEGOBATMAN 0x006d2b80
void NuFileUpCase(NUFILEDEVICE *device, char *filepath) {
  char separator;

  if (device != NULL)
    separator = device->separator;
  else
    separator = '\\';

  if (DontDoFileUpperCaseHack) {
    for (; *filepath != '\0'; filepath++) {
      if (*filepath == '\\' || *filepath == '/')
        *filepath = separator;
    }
  } else {
    for (; *filepath != '\0'; filepath++) {
      switch (*filepath) {
      case 'a':
      case 'b':
      case 'c':
      case 'd':
      case 'e':
      case 'f':
      case 'g':
      case 'h':
      case 'i':
      case 'j':
      case 'k':
      case 'l':
      case 'm':
      case 'n':
      case 'o':
      case 'p':
      case 'q':
      case 'r':
      case 's':
      case 't':
      case 'u':
      case 'v':
      case 'w':
      case 'x':
      case 'y':
      case 'z':
        *filepath -= 0x20;
        break;
      case '/':
      case '\\':
        *filepath = separator;
        break;
      }
    }
  }
}

// FUNCTION: LEGOBATMAN 0x006d2c50
void NuFileReldirFix(NUFILEDEVICE *device, char *path) {
  char *cursor;
  char *inner;

  cursor = inner = path;
  while (*cursor != '\0') {
    if (*cursor == device->separator && cursor[1] == '.' && cursor[2] == '.' &&
        cursor[3] == device->separator) {
      inner = cursor;
      while (inner > path) {
        inner--;
        if (*inner == device->separator) {
          cursor += 3;
          break;
        }
        if (*inner == ':') {
          inner++;
          cursor += 4;
          break;
        }
      }
    }
    *inner = *cursor;
    inner++;
    cursor++;
  }
  *inner = '\0';
}

// FUNCTION: LEGOBATMAN 0x006dcbc0
int NuDatFileFindTree(NUDATHDR *header, char *name) {
  char buf[256];
  char *backslash;
  int node_idx;

  if (*name == '@')
    name = name + 4;
  NuStrCpy(buf, name);
  NuFileUpCase(0, buf);
  name = buf;
  if (header->file_tree == 0)
    return NuDatFileFindHash(header, name);
  node_idx = header->file_tree->child_idx;
  backslash = NuStrStr(name, "\\");
  if (backslash != 0)
    *backslash = '\0';
  do {
    if (NuStrICmp(header->file_tree[node_idx].name, name) == 0) {
      if (backslash == 0) {
        if (header->file_tree[node_idx].child_idx <= 0)
          return -header->file_tree[node_idx].child_idx;
        return -1;
      }
      node_idx = header->file_tree[node_idx].child_idx;
      name = backslash + 1;
      backslash = NuStrStr(name, "\\");
      if (backslash != 0)
        *backslash = '\0';
    } else {
      node_idx = header->file_tree[node_idx].sibling_idx;
    }
  } while (node_idx != 0);
  return -1;
}

// GLOBAL: LEGOBATMAN 0x00b03898
int datsys_offset;

// FUNCTION: LEGOBATMAN 0x006d3850
__int64 NuDatCalcPos(NUDATHDR *header, int index) {
  __int64 pos = index;

  if (header->version <= -2)
    pos <<= 8;
  pos += datsys_offset;
  return pos;
}

// GLOBAL: LEGOBATMAN 0x0099f284
int nufile_lsn_allowed = 1;

// FUNCTION: LEGOBATMAN 0x006ddac0
int NuDatGetFileInfo(NUDATHDR *header, char *name, __int64 *position,
                     int *length) {
  if (header == NULL)
    return -1;

  const int index = NuDatFileFindTree(header, name);
  if (index >= 0) {
    const __int64 file_position =
        NuDatCalcPos(header, header->file_info[index].i0);
    if (nufile_lsn_allowed != 0 && header->unknown2 != 0) {
      if (position != NULL)
        *position = header->unknown2 + (int)(file_position / 0x800);
      if (length != NULL)
        *length = header->file_info[index].i4;
      return index;
    }
    if (position != NULL)
      *position = file_position;
    if (length != NULL)
      *length = header->file_info[index].i4;
    return index;
  }
  return -1;
}

void NuThreadCriticalSectionBegin(int cs);
void NuThreadCriticalSectionEnd(int cs);

// FUNCTION: LEGOBATMAN 0x006d39a0
int NuDatFileGetFreeInfo(void) {
  int found;
  int i;

  found = -1;
  NuThreadCriticalSectionBegin(file_criticalsection);
  for (i = 0; i < 20; i++) {
    if (!g_nuFileUnk0ae3f80[i].i24) {
      g_nuFileUnk0ae3f80[i].i24 = 1;
      found = i;
      break;
    }
  }
  NuThreadCriticalSectionEnd(file_criticalsection);
  return found;
}

// FUNCTION: LEGOBATMAN 0x006d3a10
int NuDatFileGetFreeHandleIX(NUDATHDR *hdr, int info_idx) {
  int file_idx;
  int i;

  file_idx = -1;
  NuThreadCriticalSectionBegin(file_criticalsection);
  for (i = 0; i < 8; i++) {
    if (hdr->open_files[i].info_idx == -1) {
      hdr->open_files[i].info_idx = info_idx;
      file_idx = i;
      break;
    }
  }
  NuThreadCriticalSectionEnd(file_criticalsection);
  return file_idx;
}

// FUNCTION: LEGOBATMAN 0x006d3a80
static int OpenDatFileBase(NUDATHDR *hdr, int file_idx) {
  NUDATOPENFILEINFO *open_file;

  open_file = hdr->open_files + file_idx;
  if (open_file->dat_file == 0) {
    open_file->dat_file = NuFileOpenDF(hdr->name, hdr->mode, NULL, 0);
    open_file->pos = 0;
  }
  return open_file->dat_file;
}

extern "C" void NuMemFreeFn(void *ptr, const char *file, int line);
void NuFileClose(int file);

// FUNCTION: LEGOBATMAN 0x006dceb0
void NuDatClose(NUDATHDR *header) {
  for (int index = 0; index < 8; ++index) {
    if (header->open_files[index].dat_file != 0)
      NuFileClose(header->open_files[index].dat_file);
    if (header->open_files[index].info_idx >= 0)
      g_nuFileUnk0ae3f80[header->open_files[index].info_idx].i24 = 0;
  }
  if (header->unknown != 0)
    NuMemFreeFn(header, __FILE__, 0xf4c);
}

// STUB: LEGOBATMAN 0x006dccf0
// Register choice: the original spills file_idx (into the mode slot) and
// stores start before pos; ours keeps file_idx in edi.
int NuDatFileOpen(NUDATHDR *hdr, char *filepath, int mode) {
  int node_idx;
  int info_idx;
  int file_idx;
  NUDATOPENFILEINFO *open_info;
  NuFileUnk0ae3f80 *info;
  __int64 seek_pos;

  if (mode == 0) {
    node_idx = NuDatFileFindTree(hdr, filepath);
    if (node_idx >= 0 && hdr->file_info[node_idx].i4 != 0) {
      info_idx = NuDatFileGetFreeInfo();
      if (info_idx != -1) {
        file_idx = NuDatFileGetFreeHandleIX(hdr, info_idx);
        if (file_idx != -1) {
          open_info = &hdr->open_files[file_idx];
          OpenDatFileBase(hdr, file_idx);
          info = &g_nuFileUnk0ae3f80[info_idx];
          info->p0 = (int *)hdr;
          g_nuFileUnk0ae3f80[info_idx].i8 =
              NuDatCalcPos(hdr, hdr->file_info[node_idx].i0);
          g_nuFileUnk0ae3f80[info_idx].i10 = g_nuFileUnk0ae3f80[info_idx].i8;
          g_nuFileUnk0ae3f80[info_idx].i18 = hdr->file_info[node_idx].i4;
          g_nuFileUnk0ae3f80[info_idx].i1c = hdr->file_info[node_idx].len;
          g_nuFileUnk0ae3f80[info_idx].i28 = hdr->file_info[node_idx].ic;
          g_nuFileUnk0ae3f80[info_idx].i20 = file_idx;
          open_info->pos = g_nuFileUnk0ae3f80[info_idx].i10;
          do {
            seek_pos = NuFileSeek(open_info->dat_file,
                                  g_nuFileUnk0ae3f80[info_idx].i8, 0);
            if (seek_pos < 0)
              Unk006d24d0();
          } while (seek_pos < 0);
          Unk006d24d0(file_idx);
          if (g_nuFileUnk0ae3f80[info_idx].i28 == 2)
            NuDatFileDecodeNewFile(info, open_info);
          return info_idx + 0x800;
        }
        g_nuFileUnk0ae3f80[info_idx].i24 = 0;
        return 0;
      }
      Unk006d24d0();
      return 0;
    }
    Unk006d24d0(filepath);
  }
  return 0;
}
__int64 NuFileSeek(int file, __int64 offset, int whence);

// STUB: LEGOBATMAN 0x006dd990
// Same base/index swap as NuFileSize on the dat file_info load.
int NuFileExistQuiet(char *filepath) {
  int file;
  int buffering;
  NUDATHDR *dat;

  if (filepath == 0 || *filepath == '\0')
    return 0;
  buffering = nufile_buffering_enabled;
  dat = (NUDATHDR *)g_unk00b03884;
  nufile_buffering_enabled = 0;
  if (dat != 0) {
    int index = NuDatFileFindTree(dat, filepath);
    if (index >= 0) {
      nufile_buffering_enabled = buffering;
      return dat->file_info[index].len;
    }
  }
  file = NuFileOpenDF(filepath, 0, dat, 1);
  if (file != 0)
    NuFileClose(file);
  nufile_buffering_enabled = buffering;
  return file != 0;
}

// STUB: LEGOBATMAN 0x006dda10
// One addressing diff: [file_info + idx*16 + 8] base/index order swapped.
__int64 NuFileSize(char *filepath) {
  int file;
  __int64 pos;
  int buffering;
  NUDATHDR *dat;

  buffering = nufile_buffering_enabled;
  dat = (NUDATHDR *)g_unk00b03884;
  nufile_buffering_enabled = 0;
  if (dat != 0) {
    file = NuDatFileFindTree(dat, filepath);
    if (file >= 0)
      return dat->file_info[file].len;
  }
  pos = -1;
  if (filepath != 0 && *filepath != '\0') {
    file = NuFileOpenDF(filepath, 0, dat, 0);
    if (file != 0) {
      do {
        pos = NuFileSeek(file, 0, 2);
      } while (pos < 0);
      pos = NuFilePos(file);
      NuFileClose(file);
    }
  }
  nufile_buffering_enabled = buffering;
  return pos;
}
int NuFileReadUnk006de860(int file, void *dst, int size);
extern "C" void *NuMemAllocFn(int size, const char *file, int line);

static inline int NuFileReadInt(int file) {
  int v;
  NuFileReadUnk006de860(file, &v, 4);
  return v;
}

// The original's inlined seeks in functions after the first pointer-passing
// call to Unk006d24b0 keep a small-file bounds check that the out-of-line
// 0x006d2fe0 (and NuFilePatchAddress's inlined copy) lost; this copy of
// NuFileSeek's whence 0 path carries it.
static __forceinline int NuFileSmallSeekStart(int file, int offset) {
  if (g_nuFileUnk0b038a4 == 0)
    return 0;
  if (offset > g_nuFileUnk0b00878[file - 0x2000].ic + 1)
    Unk006d24b0();
  g_nuFileUnk0b00878[file - 0x2000].i4 = offset;
  return offset;
}

static __forceinline __int64 NuFileSeekStart(int file, __int64 offset) {
  FILEINFO *info;

  if (file >= 0x2000)
    return NuFileSmallSeekStart(file, (int)offset);
  if (file >= 0x1000)
    return 0;
  if (file >= 0x400)
    return NuMemFileSeek(file, (int)offset, 0);
  file -= 1;
  info = &file_info[file];
  if (info->use_buf) {
    info->pos = offset;
    return info->pos;
  }
  return NuPSFileLSeek(file, offset, 0);
}

// GLOBAL: LEGOBATMAN 0x00b03898
int g_nuDatUnk0b03898;
void NuFileClose(int file);

// STUB: LEGOBATMAN 0x006deec0
// Same size and instructions; the original places the buf == NULL failure
// block and the epilogue inside the second retry loop, and keeps offset in
// esi:ebp there (ours hoists a zero into ebp).
NUDATHDR *NuDatOpenEx(char *filepath, VARIPTR *buf, int *size_out, short mode) {
  char name[256];
  int file;
  int read_buf[2];
  __int64 offset;
  int size;
  int len;
  int i;
  NUDATHDR *hdr;
  VARIPTR p;
  int buffering;

  buffering = nufile_buffering_enabled;
  file = NuFileOpenDF(filepath, mode, NULL, 0);
  if (file != 0) {
    if (mode == 3) {
      while (NuFileStatus(file) != 0) {
      }
    }
    if (g_nuDatUnk0b03898 != 0)
      NuFileSeek(file, g_nuDatUnk0b03898, 0);
    while (NuFileRead(file, read_buf, 8) < 0) {
      Unk006d24d0();
      while (NuFileSeekStart(file, g_nuDatUnk0b03898) < 0)
        Unk006d24d0();
    }
    if (mode == 3) {
      while (NuFileStatus(file) != 0) {
      }
    }
    offset = read_buf[0];
    if (offset < 0)
      offset *= -0x100;
    offset += g_nuDatUnk0b03898;
    while (NuFileSeekStart(file, offset) < 0)
      Unk006d24d0();
    if (mode == 3) {
      while (NuFileStatus(file) != 0) {
      }
    }
    g_unk0099f288->format_name_fn(g_unk0099f288, name, filepath, 0x100);
    len = (NuStrLen(name) + 0x10) & ~0xf;
    size = read_buf[1] + len + sizeof(NUDATHDR);
    if (buf != NULL) {
      hdr = (NUDATHDR *)((buf->addr + 0xf) & ~0xf);
      buf->addr += size;
      if (size_out != NULL)
        *size_out = size;
      hdr->unknown = 0;
      p.addr = (unsigned int)hdr + sizeof(NUDATHDR) + len;
      hdr->name = (char *)(hdr + 1);
      NuStrCpy(hdr->name, name);
      while (NuFileRead(file, p.void_ptr, read_buf[1]) < 0) {
        Unk006d24d0();
        while (NuFileSeekStart(file, offset) < 0)
          Unk006d24d0();
      }
      if (mode == 3) {
        while (NuFileStatus(file) != 0) {
        }
      }
      hdr->version = p.u32_ptr[0];
      hdr->file_count = p.u32_ptr[1];
      p.addr += 8;
      hdr->file_info = (NUDATFILEINFO *)p.void_ptr;
      p.addr += hdr->file_count * sizeof(NUDATFILEINFO);
      hdr->tree_node_count = *p.u32_ptr;
      p.addr += 4;
      hdr->file_tree = (NUDFNODE *)p.void_ptr;
      p.addr += hdr->tree_node_count * sizeof(NUDFNODE);
      hdr->leaf_names_len = *p.u32_ptr;
      p.addr += 4;
      hdr->leaf_names = p.char_ptr;
      for (i = 0; i < hdr->tree_node_count; i++)
        hdr->file_tree[i].name += (unsigned int)p.addr;
      hdr->file_tree[0].name = NULL;
      p.addr += hdr->leaf_names_len;
      hdr->hash_idxs = NULL;
      hdr->hash_count = 0;
      hdr->hashes_len = 0;
      hdr->hashes = NULL;
      if (hdr->version <= -2) {
        hdr->hash_idxs = p.u32_ptr;
        p.addr += hdr->file_count * 4;
        hdr->hash_count = *p.u32_ptr;
        p.addr += 4;
        hdr->hashes_len = *p.u32_ptr;
        p.addr += 4;
        hdr->hashes = p.char_ptr;
        for (i = 0; i < hdr->hash_count; i++) {
          p.addr += NuStrLen(p.char_ptr) + 1;
          if (p.addr & 1)
            p.addr++;
          p.addr += 2;
        }
        p.void_ptr = hdr->file_info;
        NuMemCpy(p.u8_ptr, (unsigned char *)hdr->hash_idxs,
                 hdr->file_count * 4);
        hdr->hash_idxs = p.u32_ptr;
        p.addr += hdr->file_count * 4;
        if (hdr->hashes_len != 0) {
          hdr->hashes = p.char_ptr;
          NuMemCpy(p.u8_ptr, (unsigned char *)hdr->hashes, hdr->hashes_len);
          p.addr += hdr->hashes_len;
        }
        buf->addr = p.addr;
        hdr->file_info = NULL;
        hdr->tree_node_count = 0;
        hdr->leaf_names_len = 0;
        hdr->leaf_names = NULL;
      }
      for (i = 0; i < 8; i++) {
        hdr->open_files[i].info_idx = -1;
        hdr->open_files[i].dat_file = 0;
        hdr->open_files[i].pos = 0;
      }
      hdr->open_files[0].dat_file = file;
      hdr->open_files[0].pos = 0;
      hdr->mode = mode;
      hdr->unknown2 = 0;
      nufile_buffering_enabled = buffering;
      return hdr;
    }
    Unk006d24b0();
    NuFileClose(file);
  }
  nufile_buffering_enabled = buffering;
  return NULL;
}

// FUNCTION: LEGOBATMAN 0x006e0530
void NuPadRecordLoad(char *filepath, VARIPTR *buffer, VARIPTR end) {
  g_nuPadRec.mode = 0;
  if (filepath != 0) {
    int file = NuFileOpenDF(filepath, 0, (NUDATHDR *)g_unk00b03884, 0);
    if (file != 0) {
      int size = NuFileReadInt(file);
      g_nuPadRec.record =
          (unsigned char *)NuMemAllocFn(size + 4, __FILE__, 0x433);
      g_nuPadRec.record =
          (unsigned char *)(((unsigned int)g_nuPadRec.record + 3) & ~3);
      if (g_nuPadRec.record == 0) {
        Unk006d24b0();
        return;
      }
      g_nuPadRec.buf_start = g_nuPadRec.record;
      NuFileReadUnk006de860(file, g_nuPadRec.record, size);
      Unk006dcb00(file);
      g_nuPadRec.buf_end = g_nuPadRec.record + size;
      g_nuPadRec.mode = 2;
    }
  }
}

// GLOBAL: LEGOBATMAN 0x00adf6b0
char *g_nuPadRecFilename;
// GLOBAL: LEGOBATMAN 0x00adf6b4
int g_nuPadRecUnk0adf6b4;

int NuFileWriteInt(int file, int value);
int NuFileWrite(int file, void *data, int size);
unsigned int NuFileWriteUnsignedInt(int file, unsigned int value);

// Saves a finished pad recording (size, data, 0xe0f trailer) to filepath.
// FUNCTION: LEGOBATMAN 0x006ddca0
void Unk006ddca0(char *filepath) {
  if (g_nuPadRec.mode == 1 && g_nuPadRec.record != g_nuPadRec.buf_start) {
    if (filepath != 0) {
      int file = NuFileOpen(filepath, 1);
      if (file != 0) {
        int size = g_nuPadRec.record - g_nuPadRec.buf_start;
        unsigned char *start = g_nuPadRec.buf_start;
        NuFileWriteInt(file, size);
        NuFileWrite(file, start, size);
        NuFileWriteUnsignedInt(file, 0xe0f);
        Unk006dcb00(file);
      }
    }
    g_nuPadRecUnk0adf6b4 = 0;
    g_nuPadRec.mode = 0;
  }
}

// Ends recording (saving it) or playback.
// FUNCTION: LEGOBATMAN 0x006ddd30
void Unk006ddd30(void) {
  switch (g_nuPadRec.mode) {
  case 1:
    Unk006ddca0(g_nuPadRecFilename);
    break;
  case 2:
    g_nuPadRec.mode = 0;
    g_nuPadRec.record = g_nuPadRec.buf_start;
    break;
  }
  g_nuPadRecUnk0adf6b4 = 0;
}

// --- nupad.cpp accessors (Mac order: NuPadClose, ..., NuPadGetFirstActivePad,
// NuPadGetMaxGamePads, ..., NuPadMapPortToPS2Port, ..., NuPadPortUsedByPlayer,
// NuPadInit, NuPadSetDirectMappingState, ..., NuPadRecordStart,
// NuPadRecordSetPlayEndButtons, NuPadRecordSetRecordEndButtons) -------------

extern "C" void NuMemFreeFn(void *ptr, const char *file, int line);
void NuPadClosePS(NUPAD *pad);

// FUNCTION: LEGOBATMAN 0x006d5770
void NuPadClose(NUPAD *pad) {
  NuPadClosePS(pad);
  NuMemFreeFn(pad, __FILE__, 0xcc);
}

// FUNCTION: LEGOBATMAN 0x006d5870
int NuPadGetFirstActivePad(void) { return g_nuPadUnk099f2d8; }

// FUNCTION: LEGOBATMAN 0x006d5c90
int NuPadGetMaxGamePads(void) { return g_nuPadUnk099f2dc; }

// FUNCTION: LEGOBATMAN 0x006d5cb0
void NuPadMapPortToPS2Port(int pad, int port) {
  if (g_nuPadUnk0aecb64[pad].value >= 0)
    g_nuPads[g_nuPadUnk0aecb64[pad].value].i4 = -1;
  g_nuPadUnk0aecb64[pad].value = port;
  g_nuPads[port].i4 = pad;
  g_nuPads[port].i14 = 0;
}

void Unk0051f900(void);
void Unk00522720(void *data, int size);
// GLOBAL: LEGOBATMAN 0x0099f2e0
short PlayerLastPort[4];

// FUNCTION: LEGOBATMAN 0x006d5d00
void NuPadMapPlayerToPort(int player, int port) {
  if (port >= 0) {
    Unk0051f900();
    NuPadUnk006d57e0(player, 0);
    NuPadMapPortToPS2Port(player, port);
    NuPadUnk006d57e0(player, 1);
    PlayerLastPort[player] = port;
    Unk00522720(PlayerLastPort, 8);
  } else {
    NuPadUnk006d57e0(player, 0);
  }
}

// FUNCTION: LEGOBATMAN 0x006d5e30
int NuPadPortUsedByPlayer(int port) {
  int i;
  for (i = 0; i < 3; i++) {
    if (g_nuPadUnk0aecb64[i].valid && g_nuPadUnk0aecb64[i].value == port)
      return 1;
  }
  return 0;
}

void NuPadInitPS(NuPad *pad);
// GLOBAL: LEGOBATMAN 0x00b038f8
extern int g_nuPadUnk0b038f8;

// FUNCTION: LEGOBATMAN 0x006d5e60
void NuPadInit(void) {
  int i;

  memset(g_nuPads, 0, sizeof(g_nuPads));
  for (i = 0; i < 16; i++) {
    g_nuPads[i].i4 = -1;
    NuPadInitPS(&g_nuPads[i]);
  }
  for (i = 0; i < 3; i++) {
    g_nuPadUnk0aecb64[i].valid = 0;
    g_nuPadUnk0aecb64[i].pad = -1;
    g_nuPadUnk0aecb64[i].value = -1;
  }
  g_nuPadUnk0b038f8 = 0;
}

// FUNCTION: LEGOBATMAN 0x006d5ee0
void NuPadSetDirectMappingState(int state) { g_nuPadUnk0b038f4 = state; }

// FUNCTION: LEGOBATMAN 0x006d61b0
void NuPadRecordStart(void) { g_nuPadUnk0adf6c0 = 1; }

// nuapi.pad_record.end_play_buttons / end_record_buttons
// GLOBAL: LEGOBATMAN 0x00adf6bc
int g_nuPadRecEndPlayButtons;
// GLOBAL: LEGOBATMAN 0x00adf6b8
int g_nuPadRecEndRecordButtons;

// FUNCTION: LEGOBATMAN 0x006d6430
void NuPadRecordSetPlayEndButtons(int buttons) {
  g_nuPadRecEndPlayButtons = buttons;
}

// FUNCTION: LEGOBATMAN 0x006d6440
void NuPadRecordSetRecordEndButtons(int buttons) {
  g_nuPadRecEndRecordButtons = buttons;
}

// FUNCTION: LEGOBATMAN 0x006d6450
void SavePadRecord(void) {}

// FUNCTION: LEGOBATMAN 0x006d6460
void SetPadRecPtr(void) {}

// FUNCTION: LEGOBATMAN 0x006d6470
void InitPadPlayRecord(void) {}

// --- nuptrblock.cpp ---------------------------------------------------------

// FUNCTION: LEGOBATMAN 0x006d6480
void *NuPtrBlockFix(void *block) {
  unsigned int buf;
  unsigned int ptr;
  void *start;
  int count;
  int i;

  start = (char *)block + 4;
  buf = *(unsigned int *)block + (unsigned int)block;
  count = *(int *)buf;
  buf += 4;

  for (i = 0; i < count; i++) {
    ptr = *(unsigned int *)buf + buf;
    if (*(unsigned int *)ptr != 0)
      *(unsigned int *)ptr = *(unsigned int *)ptr + ptr;
    buf += 4;
  }

  return start;
}

// FUNCTION: LEGOBATMAN 0x006d64c0
void NuPtrBlockRelocate(void *block, unsigned int base) {
  unsigned int buf;
  unsigned int ptr;
  int count;
  int i;

  buf = *(unsigned int *)block + (unsigned int)block;
  count = *(int *)buf;
  buf += 4;

  for (i = 0; i < count; i++) {
    ptr = *(unsigned int *)buf + buf;
    if (*(unsigned int *)ptr != 0)
      *(unsigned int *)ptr = *(unsigned int *)ptr + base;
    buf += 4;
  }
}

// --- nuvideo.cpp (nuapi fields) ---------------------------------------------

// nuapi.video_mode
// GLOBAL: LEGOBATMAN 0x00adf650
int g_nuapiVideoMode;
// nuapi.screen_width / screen_height
// GLOBAL: LEGOBATMAN 0x00adf65c
int g_nuapiScreenWidth;
// GLOBAL: LEGOBATMAN 0x00adf660
int g_nuapiScreenHeight;
// nuapi.fps
// GLOBAL: LEGOBATMAN 0x00adf664
float g_nuapiFps;

// FUNCTION: LEGOBATMAN 0x006d6510
int NuVideoGetMode(void) { return g_nuapiVideoMode; }

// FUNCTION: LEGOBATMAN 0x006d6520
void NuVideoSetMode(int mode) { g_nuapiVideoMode = mode; }

// FUNCTION: LEGOBATMAN 0x006d6530
void NuVideoSetResolution(int width, int height) {
  g_nuapiScreenWidth = width;
  g_nuapiScreenHeight = height;
}

// FUNCTION: LEGOBATMAN 0x006d6600
int NuVideoGetNativeFPS(void) { return (int)g_nuapiFps; }

// FUNCTION: LEGOBATMAN 0x006d6610
void NuVideoWaitForVSync(void) {}

// --- nulanguage.cpp ---------------------------------------------------------

void NuLanguageInitPS(void);

// FUNCTION: LEGOBATMAN 0x006d6810
void NuLanguageInit(void) {
  g_nuPadUnk0adf648 = 2;
  g_nuPadUnk0adf64c = 0;
  NuLanguageInitPS();
}

// --- nustring_c.cpp wide helpers (saga bodies; Mac order NuToUpperW,
// NuToLowerW, NuStrCatW, NuStrNCatW, NuStrCpyW, NuStrNCpyW, NuStrChrW,
// NuStrRChrW, NuStrLenW, NuStrStrW, NuStrCmpW) -------------------------------

typedef unsigned short NUWCHAR;

// FUNCTION: LEGOBATMAN 0x006d7820
NUWCHAR NuToUpperW(NUWCHAR c) {
  if (c >= 0x61 && c <= 0x7a) {
    c -= 0x20;
  } else if (c > 0xdf && c < 0x100) {
    c -= 0x20;
  }

  return c;
}

// FUNCTION: LEGOBATMAN 0x006d7840
NUWCHAR NuToLowerW(NUWCHAR c) {
  if (c >= 0x41 && c <= 0x5a) {
    c += 0x20;
  } else if (c > 0xbf && c < 0xe0) {
    c += 0x20;
  }

  return c;
}

// FUNCTION: LEGOBATMAN 0x006d7860
void NuStrCatW(NUWCHAR *str, const NUWCHAR *ext) {
  while (*str != 0)
    ++str;
  if (ext != 0) {
    do {
      *str = *ext;
      ++str;
    } while (*ext++ != 0);
  }
}

// FUNCTION: LEGOBATMAN 0x006d78a0
int NuStrNCatW(NUWCHAR *str, const NUWCHAR *ext, int n) {
  while (*str != 0)
    ++str;
  int copied = 0;
  if (ext != 0) {
    do {
      if (n == 0)
        break;
      *str = *ext;
      ++str;
      ++copied;
      --n;
    } while (*ext++ != 0);
  }
  return copied;
}

// FUNCTION: LEGOBATMAN 0x006d7980
NUWCHAR *NuStrChrW(NUWCHAR *str, NUWCHAR c) {
  while (*str != 0) {
    if (*str == c)
      return str;
    ++str;
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x006d79b0
NUWCHAR *NuStrRChrW(NUWCHAR *str, NUWCHAR c) {
  NUWCHAR *cursor = str;
  while (*cursor != 0)
    ++cursor;
  while (cursor >= str) {
    if (*cursor == c)
      return cursor;
    --cursor;
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x006d7a10
NUWCHAR *NuStrStrW(NUWCHAR *str, const NUWCHAR *sub) {
  while (*str != 0) {
    NUWCHAR *cursor = str;
    const NUWCHAR *match = sub;
    while (*match != 0) {
      if (*cursor == 0)
        break;
      if (*cursor != *match)
        break;
      ++cursor;
      ++match;
    }
    if (*match == 0)
      return str;
    ++str;
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x006d7b10
int NuStrCmpW(const NUWCHAR *a, const NUWCHAR *b) {
  NUWCHAR ca;
  NUWCHAR cb;
  if (a == 0)
    return -1;
  if (b == 0)
    return 1;
  do {
    ca = *a;
    cb = *b;
    if (ca > cb)
      return 1;
    if (ca < cb)
      return -1;
    ++a;
    ++b;
  } while (ca != 0 && cb != 0);
  return 0;
}

// FUNCTION: LEGOBATMAN 0x006d7e70
int NuIsAl(char c) {
  switch (c) {
  case 'A':
  case 'B':
  case 'C':
  case 'D':
  case 'E':
  case 'F':
  case 'G':
  case 'H':
  case 'I':
  case 'J':
  case 'K':
  case 'L':
  case 'M':
  case 'N':
  case 'O':
  case 'P':
  case 'Q':
  case 'R':
  case 'S':
  case 'T':
  case 'U':
  case 'V':
  case 'W':
  case 'X':
  case 'Y':
  case 'Z':
  case '_':
  case 'a':
  case 'b':
  case 'c':
  case 'd':
  case 'e':
  case 'f':
  case 'g':
  case 'h':
  case 'i':
  case 'j':
  case 'k':
  case 'l':
  case 'm':
  case 'n':
  case 'o':
  case 'p':
  case 'q':
  case 'r':
  case 's':
  case 't':
  case 'u':
  case 'v':
  case 'w':
  case 'x':
  case 'y':
  case 'z':
    return 1;
  default:
    return 0;
  }
}

// FUNCTION: LEGOBATMAN 0x006d7ee0
char *NuIToA(i32 value, char *buffer, i32 radix) {
  char reversed[33];
  char *digit = reversed;
  char *out = buffer;
  if (value < 0) {
    value = -value;
    *out++ = '-';
  }
  do {
    *digit++ = value % radix + '0';
    value /= radix;
  } while (value != 0);
  while (digit != reversed)
    *out++ = *--digit;
  *out++ = '\0';
  return buffer;
}

// FUNCTION: LEGOBATMAN 0x006d8070
i32 NuStrToL(char *str, char **end, i32 radix) {
  i32 value = 0;
  i32 sign = 0;
  char c = *str++;
  while (c == ' ' || c == '\t')
    c = *str++;
  if (c == '-') {
    sign = -1;
    c = *str++;
  } else if (c == '+')
    c = *str++;
  if (radix == 0) {
    radix = 10;
    if (c == '0' && (*str == 'x' || *str == 'X')) {
      radix = 16;
      ++str;
      c = *str;
    }
  }
  while (1) {
    if (c >= '0' && c <= '9')
      c -= '0';
    else if (c >= 'A' && c <= 'Z')
      c -= 'A' - 10;
    else if (c >= 'a' && c <= 'z')
      c -= 'a' - 10;
    else
      break;
    if (c >= radix)
      break;
    value *= radix;
    value += c;
    c = *str++;
  }
  if (end != 0)
    *end = str - 1;
  if (sign)
    return sign * value;
  return value;
}

// FUNCTION: LEGOBATMAN 0x006d8130
int NuIsAlW(NUWCHAR c) {
  switch (c) {
  case 'A':
  case 'B':
  case 'C':
  case 'D':
  case 'E':
  case 'F':
  case 'G':
  case 'H':
  case 'I':
  case 'J':
  case 'K':
  case 'L':
  case 'M':
  case 'N':
  case 'O':
  case 'P':
  case 'Q':
  case 'R':
  case 'S':
  case 'T':
  case 'U':
  case 'V':
  case 'W':
  case 'X':
  case 'Y':
  case 'Z':
  case '_':
  case 'a':
  case 'b':
  case 'c':
  case 'd':
  case 'e':
  case 'f':
  case 'g':
  case 'h':
  case 'i':
  case 'j':
  case 'k':
  case 'l':
  case 'm':
  case 'n':
  case 'o':
  case 'p':
  case 'q':
  case 'r':
  case 's':
  case 't':
  case 'u':
  case 'v':
  case 'w':
  case 'x':
  case 'y':
  case 'z':
    return 1;
  default:
    return 0;
  }
}

// FUNCTION: LEGOBATMAN 0x006d81a0
NUWCHAR *NuIToAW(i32 value, NUWCHAR *buffer, i32 radix) {
  NUWCHAR reversed[33];
  NUWCHAR *digit = reversed;
  NUWCHAR *out = buffer;
  if (value < 0) {
    value = -value;
    *out++ = '-';
  }
  do {
    *digit++ = value % radix + '0';
    value /= radix;
  } while (value != 0);
  while (digit != reversed)
    *out++ = *--digit;
  *out++ = 0;
  return buffer;
}

// FUNCTION: LEGOBATMAN 0x006d8220
f32 NuAToFW(const NUWCHAR *str) {
  f32 value = 0.0f;
  f32 divisor = 1.0f;
  NUWCHAR c = *str++;
  if (c == '-') {
    divisor = -1.0f;
    c = *str++;
  }
  while (c >= '0' && c <= '9') {
    value *= 10.0f;
    value += c - '0';
    c = *str++;
  }
  if (c == '.') {
    c = *str++;
    while (c >= '0' && c <= '9') {
      divisor *= 10.0f;
      value *= 10.0f;
      value += c - '0';
      c = *str++;
    }
  }
  return value / divisor;
}

// FUNCTION: LEGOBATMAN 0x006d82f0
i32 NuAToIW(const NUWCHAR *str) {
  i32 value = 0;
  i32 sign = 0;
  NUWCHAR c = *str++;
  if (c == '-') {
    sign = -1;
    c = *str++;
  }
  while (c >= '0' && c <= '9') {
    value = (value << 3) + (value << 1);
    value = value + c - '0';
    c = *str++;
  }
  if (sign)
    return sign * value;
  return value;
}

// FUNCTION: LEGOBATMAN 0x006d8340
i32 NuStrToLW(NUWCHAR *str, NUWCHAR **end, i32 radix) {
  i32 value = 0;
  i32 sign = 0;
  NUWCHAR c = *str++;
  while (c == ' ' || c == '\t')
    c = *str++;
  if (c == '-') {
    sign = -1;
    c = *str++;
  } else if (c == '+')
    c = *str++;
  if (radix == 0) {
    radix = 10;
    if (c == '0' && (*str == 'x' || *str == 'X')) {
      radix = 16;
      ++str;
      c = *str;
    }
  }
  while (1) {
    if (c >= '0' && c <= '9')
      c -= '0';
    else if (c >= 'A' && c <= 'Z')
      c -= 'A' - 10;
    else if (c >= 'a' && c <= 'z')
      c -= 'a' - 10;
    else
      break;
    if (c >= radix)
      break;
    value *= radix;
    value += c;
    c = *str++;
  }
  if (end != 0)
    *end = str - 1;
  if (sign)
    return sign * value;
  return value;
}

// FUNCTION: LEGOBATMAN 0x006d7b60
i32 NuStrICmpW(NUWCHAR *a, NUWCHAR *b) {
  NUWCHAR a_cursor;
  NUWCHAR b_cursor;

  if (a == 0) {
    return -1;
  }

  if (b == 0) {
    return 1;
  }

  do {
    a_cursor = NuToUpperW(*a);
    b_cursor = NuToUpperW(*b);

    if (a_cursor > b_cursor) {
      return 1;
    }

    if (a_cursor < b_cursor) {
      return -1;
    }

    a++;
    b++;
  } while (a_cursor != '\0' && b_cursor != '\0');

  return 0;
}

// FUNCTION: LEGOBATMAN 0x006d7bf0
i32 NuStrNCmpW(const NUWCHAR *a, const NUWCHAR *b, i32 n) {
  NUWCHAR ca, cb;
  if (a == 0)
    return -1;
  if (b == 0)
    return 1;
  if (n == 0)
    return 0;
  if (n == -1)
    n = NuStrLenW(a);
  else if (n == -2)
    n = NuStrLenW(b);
  do {
    ca = *a;
    cb = *b;
    if (ca > cb)
      return 1;
    if (ca < cb)
      return -1;
    ++a;
    ++b;
    --n;
  } while (ca != 0 && cb != 0 && n != 0);
  return 0;
}

// FUNCTION: LEGOBATMAN 0x006d7c60
i32 NuStrNICmpW(const NUWCHAR *a, const NUWCHAR *b, i32 n) {
  NUWCHAR ca, cb;
  if (a == 0)
    return -1;
  if (b == 0)
    return 1;
  if (n == 0)
    return 0;
  if (n == -1)
    n = NuStrLenW(a);
  else if (n == -2)
    n = NuStrLenW(b);
  do {
    ca = NuToUpperW(*a);
    cb = NuToUpperW(*b);
    if (ca > cb)
      return 1;
    if (ca < cb)
      return -1;
    ++a;
    ++b;
    --n;
  } while (ca != 0 && cb != 0 && n != 0);
  return 0;
}

// --- string table accessors (saga bodies; Mac order NuStringTableSetFormat,
// NuStringTableGetFormat, NuStringTableSetBank, NuStringTableUnload,
// NuStringTableGetById) -------------------------------------------------------

// FUNCTION: LEGOBATMAN 0x006d8760
int NuStringTableGetFormat(void) { return g_nuPadUnk099f308; }

// FUNCTION: LEGOBATMAN 0x006d8780
void NuStringTableUnload(void) {
  StringBank[g_nuPadUnk0b03924].strings = 0;
  StringBank[g_nuPadUnk0b03924].max_strings = 0;
  StringBank[g_nuPadUnk0b03924].string_count = 0;
}

// FUNCTION: LEGOBATMAN 0x006d87b0
NUWCHAR *NuStringTableGetById(int id) {
  unsigned int bank = 0;

  if ((unsigned int)id >= 0x1000000 && id != -1) {
    bank = (unsigned int)id >> 24;
    id &= 0xffffff;
  }
  if (bank >= 3) {
    return 0;
  }
  if (id < 0) {
    return 0;
  }
  if (id >= StringBank[bank].string_count) {
    return 0;
  }
  return StringBank[bank].strings[id].str_utf16;
}

// FUNCTION: LEGOBATMAN 0x006d8800
void NuStrGetFilenameNoExt(char *dst, char *src, int size) {
  char *extension;
  char *separator;
  char *backslash;

  extension = NuStrRChr(src, '.');
  if (extension != NULL)
    *extension = '\0';
  separator = NuStrRChr(src, '/');
  backslash = NuStrRChr(src, '\\');
  if (backslash > separator)
    separator = backslash;
  if (separator != NULL)
    separator++;
  else
    separator = src;
  if (NuStrLen(separator) <= size)
    NuStrCpy(dst, separator);
  if (extension != NULL)
    *extension = '.';
}

// FUNCTION: LEGOBATMAN 0x006d88e0
void NuStrGetPath(char *dst, char *src) {
  char *separator = NuStrRChr(src, '/');
  char *backslash = NuStrRChr(src, '\\');
  if (backslash > separator)
    separator = backslash;
  if (separator == NULL) {
    *dst = 0;
  } else {
    int length = separator - src + 1;
    NuStrNCpy(dst, src, length);
  }
}

// FUNCTION: LEGOBATMAN 0x006d8960
void NuStrGetExt(char *dst, char *src) {
  char *separator;
  char *extension = NuStrRChr(src, '.');
  separator = NuStrRChr(src, '/');
  char *backslash = NuStrRChr(src, '\\');
  if (backslash > separator)
    separator = backslash;
  if (extension == NULL || separator > extension) {
    *dst = 0;
  } else {
    char *end = src + NuStrLen(src);
    int length = end - extension;
    if (length == 0)
      *dst = 0;
    else
      NuStrNCpy(dst, extension + 1, length);
  }
}

// FUNCTION: LEGOBATMAN 0x006d8a30
void NuStrSubstituteString(char *dst, char *src, char *search,
                           char *replacement) {
  int matched = 0;
  while (*src != 0) {
    while (search[matched] != 0) {
      if (NuToLower(src[matched]) != NuToLower(search[matched])) {
        matched = 0;
        break;
      }
      ++matched;
    }
    if (matched != 0) {
      src += matched;
      if (replacement != NULL) {
        matched = 0;
        while (replacement[matched] != 0) {
          *dst = replacement[matched];
          ++dst;
          ++matched;
        }
      }
      matched = 0;
    } else {
      *dst = *src;
      ++dst;
      ++src;
    }
  }
  *dst = 0;
}

// --- bad word filter (saga nustring_c.cpp) ----------------------------------

// GLOBAL: LEGOBATMAN 0x0099f310
static unsigned short bad_equiv[][2] = {{'a', '@'}, {'a', '4'}, {'e', '3'},
                                        {'i', '1'}, {'l', '1'}, {'o', '0'},
                                        {'t', '7'}, {0, 0}};
// GLOBAL: LEGOBATMAN 0x0099f330
unsigned short *bad_fluff = (unsigned short *)L"!$%^&*()_+-={}[]@~#?<>,. ";
// GLOBAL: LEGOBATMAN 0x00b03928
static NUWCHAR **BadWords;

// FUNCTION: LEGOBATMAN 0x006d8b10
static int NuStringCharEquiv(unsigned short a, unsigned short b) {
  int i = 0;
  if (a == b) {
    return 1;
  } else {
    while (bad_equiv[i][0] != 0) {
      if (bad_equiv[i][1] == a && bad_equiv[i][0] == b)
        return 1;
      if (bad_equiv[i][1] == b && bad_equiv[i][0] == a)
        return 1;
      i++;
    }
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x006d8b70
static int NuStringIsFluff(unsigned short character) {
  int i = 0;
  while (bad_fluff[i] != 0) {
    if (bad_fluff[i] == character)
      return 1;
    i++;
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x006d8ba0
static unsigned short *NuStringBadSubString(const unsigned short *text,
                                            const unsigned short *word,
                                            int *length, int whole_word) {
  const unsigned short *cursor;
  const unsigned short *match;
  int boundary = 1;
  while (*text != 0) {
    cursor = text;
    match = word;
    if (boundary || !whole_word) {
      while (*match != 0) {
        if (*cursor == 0)
          break;
        if (NuStringCharEquiv(*cursor, *match) == 0)
          break;
        cursor++;
        while (*cursor != 0) {
          if (!NuStringIsFluff(*cursor) && *cursor != *match)
            break;
          cursor++;
        }
        match++;
      }
      if (*match == 0) {
        if (!whole_word || cursor[1] == 0 || NuStringIsFluff(cursor[1])) {
          *length = cursor - text;
          return (unsigned short *)text;
        }
      }
    }
    if (NuStringIsFluff(*text))
      boundary = 1;
    else
      boundary = 0;
    text++;
  }
  return NULL;
}

// FUNCTION: LEGOBATMAN 0x006d8d00
int NuStringFilterBadWordsW(NUWCHAR *dest, NUWCHAR *source,
                            NUWCHAR *replacement) {
  int i = 0;
  int changed = 0;
  int whole_word;
  NUWCHAR *word;
  NUWCHAR *match;
  int length;
  NUWCHAR lower[256];
  if (source != dest)
    NuStrCpyW(dest, source);
  NuPadWStrCpyLower(lower, dest);
  while (BadWords[i] != NULL) {
    if (BadWords[i][0] == '!') {
      word = BadWords[i] + 1;
      whole_word = 1;
    } else {
      word = BadWords[i];
      whole_word = 0;
    }
    match = NuStringBadSubString(lower, word, &length, whole_word);
    if (match != NULL) {
      NuStrCpyW(dest, replacement);
      changed = 1;
    }
    i++;
  }
  return changed;
}

// FUNCTION: LEGOBATMAN 0x006d8de0
int NuStringFilterBadWords(unsigned char *dest, unsigned char *source,
                           unsigned char *replacement) {
  int changed;
  NUWCHAR text[128];
  NUWCHAR substitute[128];
  NuUTF8ToUnicode(text, source);
  NuUTF8ToUnicode(substitute, replacement);
  changed = NuStringFilterBadWordsW(text, text, substitute);
  NuPadWideToUtf8(dest, text);
  return changed;
}

// FUNCTION: LEGOBATMAN 0x006d7a70
NUWCHAR *NuStrIStrW(NUWCHAR *str, const NUWCHAR *sub) {
  while (*str != 0) {
    NUWCHAR *cursor = str;
    const NUWCHAR *match = sub;
    while (*match != 0) {
      if (*cursor == 0)
        break;
      if (NuToUpperW(*cursor) != NuToUpperW(*match))
        break;
      ++cursor;
      ++match;
    }
    if (*match == 0)
      return str;
    ++str;
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x006d8420
u8 NuToLower(u8 c) {
  if (c >= 'A' && c <= 'Z') {
    c += 0x20;
  } else if (c > 0xbf && c < 0xe0) {
    c += 0x20;
  }

  return c;
}

// FUNCTION: LEGOBATMAN 0x006ddb90
void NuFParResume(NUFPAR *parser) {
  if (parser->file_handle >= 0x400 && parser->file_handle < 0x800)
    return;
  if (parser->file_name[0] == '\0')
    Unk006d24b0();
  parser->file_handle = NuFileOpen(parser->file_name, 0);
  if (parser->file_handle == 0)
    Unk006d24b0();
  NuFileSeek(parser->file_handle, parser->char_pos, 0);
  parser->buf_end = parser->char_pos - 1;
}

// Integer expression parser (saga nu2api/nucore/nurdpi.cpp).
typedef int nurdpgetvarfn(char *name, float *fvalue, int *ivalue);

// Float expression parser (saga nu2api/nucore/nurdpf.cpp).
typedef struct rdpfctxt_s {
  int cur_tok;
  float cur_val;
  char *buf;
  int pos;
  nurdpgetvarfn *get_var_fn;
} RDPFCTXT;

// Multi-character token list (double-NUL terminated), set by
// NuRDPSetMultiCharTokens.
// GLOBAL: LEGOBATMAN 0x0099f290
extern char *rdp_multichar_tokens;

static int GetMultiCharToken(char *s);

// FUNCTION: LEGOBATMAN 0x006d4be0
static int isnumordot(char c) {
  if ((c >= '0' && c <= '9') || c == '.')
    return 1;
  return 0;
}

// STUB: LEGOBATMAN 0x006dad60
// Identifier loop only: the original spills p = &buf[pos] across the
// GetMultiCharToken call; ours recomputes it from ctx->buf.
static void get_tok_rdpf(RDPFCTXT *ctx) {
  char var_name_buf[256];
  float multiplier;
  int var_name_pos;
  int len;
  char *p;

  while (1) {
    if (ctx->buf[ctx->pos] == '\0') {
      ctx->cur_tok = 6;
      return;
    }
    if (ctx->buf[ctx->pos] == ' ') {
      ctx->buf[ctx->pos]++;
      continue;
    }
    if (ctx->buf[ctx->pos] == '\t') {
      ctx->buf[ctx->pos]++;
      continue;
    }
    break;
  }

  switch (ctx->buf[ctx->pos]) {
  case ';':
    ctx->cur_tok = 6;
    break;
  case '+':
    ctx->cur_tok = 0;
    ctx->pos++;
    break;
  case '-':
    ctx->cur_tok = 1;
    ctx->pos++;
    break;
  case '*':
    ctx->cur_tok = 2;
    ctx->pos++;
    break;
  case '/':
    ctx->cur_tok = 3;
    ctx->pos++;
    break;
  case '(':
    ctx->cur_tok = 4;
    ctx->pos++;
    break;
  case ')':
    ctx->cur_tok = 5;
    ctx->pos++;
    break;
  case '.':
  case '0':
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case '8':
  case '9':
    ctx->cur_tok = 7;
    ctx->cur_val = 0.0f;
    multiplier = 0.0f;
    while (isnumordot(ctx->buf[ctx->pos])) {
      if (ctx->buf[ctx->pos] == '.') {
        if (multiplier != 0.0f)
          Unk006d24b0();
        multiplier = 0.1f;
      } else if (multiplier != 0.0f) {
        ctx->cur_val += (float)(ctx->buf[ctx->pos] - 0x30) * multiplier;
        multiplier /= 10.0f;
      } else {
        ctx->cur_val *= 10.0f;
        ctx->cur_val += (float)(ctx->buf[ctx->pos] - 0x30);
      }
      ctx->pos++;
    }
    break;
  case 'A':
  case 'B':
  case 'C':
  case 'D':
  case 'E':
  case 'F':
  case 'G':
  case 'H':
  case 'I':
  case 'J':
  case 'K':
  case 'L':
  case 'M':
  case 'N':
  case 'O':
  case 'P':
  case 'Q':
  case 'R':
  case 'S':
  case 'T':
  case 'U':
  case 'V':
  case 'W':
  case 'X':
  case 'Y':
  case 'Z':
  case '_':
  case 'a':
  case 'b':
  case 'c':
  case 'd':
  case 'e':
  case 'f':
  case 'g':
  case 'h':
  case 'i':
  case 'j':
  case 'k':
  case 'l':
  case 'm':
  case 'n':
  case 'o':
  case 'p':
  case 'q':
  case 'r':
  case 's':
  case 't':
  case 'u':
  case 'v':
  case 'w':
  case 'x':
  case 'y':
  case 'z':
    var_name_pos = 0;
    while (1) {
      p = &ctx->buf[ctx->pos];
      len = GetMultiCharToken(p);
      if (len != 0) {
        NuStrNCpy(&var_name_buf[var_name_pos], p, len + 1);
        ctx->pos += len;
        var_name_pos += len;
      } else if (NuIsAlNum(*p)) {
        var_name_buf[var_name_pos] = *p;
        ctx->pos++;
        var_name_pos++;
      } else {
        break;
      }
    }
    var_name_buf[var_name_pos] = '\0';
    if (ctx->get_var_fn == NULL)
      Unk006d24b0(var_name_buf);
    ctx->cur_tok = 7;
    ctx->cur_val = 0.0f;
    ctx->get_var_fn(var_name_buf, &ctx->cur_val, NULL);
    return;
  default:
    Unk006d24c0(ctx->buf);
    break;
  }
}

// Default list (0x00b038c8) of the multi-character tokens.
// GLOBAL: LEGOBATMAN 0x00b038c8
extern char rdp_default_multichar_tokens[];

// FUNCTION: LEGOBATMAN 0x006d4c00
void NuRDPSetMultiCharTokens(char *tokens) {
  rdp_multichar_tokens = rdp_default_multichar_tokens;
  if (tokens != NULL)
    rdp_multichar_tokens = tokens;
}

// Length of the multi-character token that s starts with, or 0.
// FUNCTION: LEGOBATMAN 0x006dacf0
static int GetMultiCharToken(char *s) {
  char *tok = rdp_multichar_tokens;

  while (*tok) {
    int len = NuStrLen(tok);
    if (NuStrNCmp(tok, s, len) == 0)
      return len;
    tok += NuStrLen(tok) + 1;
  }
  return 0;
}

static float f_expr(RDPFCTXT *ctx);

// FUNCTION: LEGOBATMAN 0x006db120
static float prim_rdpf(RDPFCTXT *ctx) {
  float expr;

  switch (ctx->cur_tok) {
  case 8:
    Unk006d24b0();
  case 7:
    get_tok_rdpf(ctx);
    return ctx->cur_val;
  case 1:
    get_tok_rdpf(ctx);
    return -prim_rdpf(ctx);
  case 4:
    get_tok_rdpf(ctx);
    expr = f_expr(ctx);
    if (ctx->cur_tok != 5)
      Unk006d24b0();
    get_tok_rdpf(ctx);
    return expr;
  case 6:
    return 1.0f;
  default:
    Unk006d24b0();
    return 0.0f;
  }
}

// FUNCTION: LEGOBATMAN 0x006db1c0
static float term_rdpf(RDPFCTXT *ctx) {
  float term = prim_rdpf(ctx);

  while (1) {
    switch (ctx->cur_tok) {
    case 2:
      get_tok_rdpf(ctx);
      term *= prim_rdpf(ctx);
      break;
    case 3:
      get_tok_rdpf(ctx);
      term /= prim_rdpf(ctx);
      break;
    default:
      return term;
    }
  }
}

// FUNCTION: LEGOBATMAN 0x006db210
static float f_expr(RDPFCTXT *ctx) {
  float expr = term_rdpf(ctx);

  while (1) {
    switch (ctx->cur_tok) {
    case 0:
      get_tok_rdpf(ctx);
      expr += term_rdpf(ctx);
      break;
    case 1:
      get_tok_rdpf(ctx);
      expr -= term_rdpf(ctx);
      break;
    default:
      return expr;
    }
  }
}

// FUNCTION: LEGOBATMAN 0x006db270
float NuRDPFVar(char *input, nurdpgetvarfn *get_var_fn) {
  RDPFCTXT ctx;

  ctx.buf = input;
  ctx.pos = 0;
  ctx.get_var_fn = get_var_fn;
  get_tok_rdpf(&ctx);
  return f_expr(&ctx);
}

// FUNCTION: LEGOBATMAN 0x006db2a0
float NuRDPF(char *input) { return NuRDPFVar(input, NULL); }

// Relocates a memory file's self-relative pointer table in place and returns
// its data (Mac name unknown).
// FUNCTION: LEGOBATMAN 0x006db2d0
void *NuMemFileUnk006db2d0(int file) {
  char *p;
  void *data;
  int n;
  int *slot;
  int i;

  p = (char *)g_nuFileUnk0afa0a8[file - 0x400].i8;
  data = p + 4;
  p += *(int *)p;
  n = *(int *)p;
  p += 4;
  for (i = 0; i < n; i++) {
    slot = (int *)(p + *(int *)p);
    if (*slot != 0)
      *slot += (int)slot;
    p += 4;
  }
  return data;
}

typedef struct rdpictxt_s {
  int cur_tok;
  int cur_val;
  char *buf;
  int pos;
  nurdpgetvarfn *get_var_fn;
} RDPICTXT;

int NuIsAlNum(char c);

// FUNCTION: LEGOBATMAN 0x006d6730
static int ishexnum(char c) {
  switch (c) {
  case '0':
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case '8':
  case '9':
  case 'A':
  case 'B':
  case 'C':
  case 'D':
  case 'E':
  case 'F':
  case 'a':
  case 'b':
  case 'c':
  case 'd':
  case 'e':
  case 'f':
    return 1;
  default:
    return 0;
  }
}

// FUNCTION: LEGOBATMAN 0x006d67a0
static int hexcodetoint(char c) {
  switch (c) {
  case '0':
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case '8':
  case '9':
    return c - 0x30;
  case 'a':
  case 'b':
  case 'c':
  case 'd':
  case 'e':
  case 'f':
    return c - 0x57;
  case 'A':
  case 'B':
  case 'C':
  case 'D':
  case 'E':
  case 'F':
    return c - 0x37;
  default:
    return 0;
  }
}

static int isnum(char c) { return (unsigned char)(c - '0') <= 9; }

// STUB: LEGOBATMAN 0x006db510
// Close: switch char in dl/ecx (ours cl/edx), the hex loop shifts in ebx
// (ours in memory), and the digit loop is not rotated in the original.
static void get_tok_rdpi(RDPICTXT *ctx) {
  char var_name_buf[256];
  int var_name_pos;

  while (1) {
    if (ctx->buf[ctx->pos] == '\0') {
      ctx->cur_tok = 7;
      return;
    }
    if (ctx->buf[ctx->pos] == ' ') {
      ctx->buf[ctx->pos]++;
      continue;
    }
    if (ctx->buf[ctx->pos] == '\t') {
      ctx->buf[ctx->pos]++;
      continue;
    }
    break;
  }
  char c = ctx->buf[ctx->pos];
  switch (c) {
  case ';':
    ctx->cur_tok = 7;
    break;
  case '+':
    ctx->cur_tok = 0;
    ctx->pos++;
    break;
  case '-':
    ctx->cur_tok = 1;
    ctx->pos++;
    break;
  case '*':
    ctx->cur_tok = 2;
    ctx->pos++;
    break;
  case '/':
    ctx->cur_tok = 3;
    ctx->pos++;
    break;
  case '%':
    ctx->cur_tok = 4;
    ctx->pos++;
    break;
  case '(':
    ctx->cur_tok = 5;
    ctx->pos++;
    break;
  case ')':
    ctx->cur_tok = 6;
    ctx->pos++;
    break;
  case '|':
    ctx->cur_tok = 9;
    ctx->pos++;
    if (ctx->buf[ctx->pos] == '|') {
      ctx->cur_tok = 13;
      ctx->pos++;
    }
    return;
  case '&':
    ctx->cur_tok = 10;
    ctx->pos++;
    if (ctx->buf[ctx->pos] == '&') {
      ctx->cur_tok = 14;
      ctx->pos++;
    }
    return;
  case '=':
    ctx->cur_tok = 16;
    ctx->pos++;
    if (ctx->buf[ctx->pos] == '=')
      ctx->pos++;
    return;
  case '>':
    ctx->cur_tok = 19;
    ctx->pos++;
    if (ctx->buf[ctx->pos] == '=') {
      ctx->pos++;
      ctx->cur_tok = 21;
    }
    return;
  case '<':
    ctx->cur_tok = 18;
    ctx->pos++;
    if (ctx->buf[ctx->pos] == '=') {
      ctx->pos++;
      ctx->cur_tok = 20;
    }
    return;
  case '^':
    ctx->cur_tok = 12;
    ctx->pos++;
    break;
  case '!':
    ctx->cur_tok = 15;
    ctx->pos++;
    if (ctx->buf[ctx->pos] == '=') {
      ctx->cur_tok = 17;
      ctx->pos++;
    }
    return;
  case '~':
    ctx->cur_tok = 11;
    ctx->pos++;
    break;
  case '$':
    ctx->pos++;
    ctx->cur_tok = 8;
    ctx->cur_val = 0;
    while (ishexnum(ctx->buf[ctx->pos])) {
      ctx->cur_val <<= 4;
      ctx->cur_val += hexcodetoint(ctx->buf[ctx->pos]);
      ctx->pos++;
    }
    break;
  case '0':
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case '8':
  case '9':
    ctx->cur_tok = 8;
    ctx->cur_val = 0;
    for (;;) {
      if (!isnum(ctx->buf[ctx->pos]))
        break;
      ctx->cur_val *= 10;
      ctx->cur_val += ctx->buf[ctx->pos] - 0x30;
      ctx->pos++;
    }
    break;
  case 'A':
  case 'B':
  case 'C':
  case 'D':
  case 'E':
  case 'F':
  case 'G':
  case 'H':
  case 'I':
  case 'J':
  case 'K':
  case 'L':
  case 'M':
  case 'N':
  case 'O':
  case 'P':
  case 'Q':
  case 'R':
  case 'S':
  case 'T':
  case 'U':
  case 'V':
  case 'W':
  case 'X':
  case 'Y':
  case 'Z':
  case 'a':
  case 'b':
  case 'c':
  case 'd':
  case 'e':
  case 'f':
  case 'g':
  case 'h':
  case 'i':
  case 'j':
  case 'k':
  case 'l':
  case 'm':
  case 'n':
  case 'o':
  case 'p':
  case 'q':
  case 'r':
  case 's':
  case 't':
  case 'u':
  case 'v':
  case 'w':
  case 'x':
  case 'y':
  case 'z':
    var_name_pos = 0;
    while (NuIsAlNum(ctx->buf[ctx->pos])) {
      var_name_buf[var_name_pos] = ctx->buf[ctx->pos];
      ctx->pos++;
      var_name_pos++;
    }
    var_name_buf[var_name_pos] = '\0';
    if (ctx->get_var_fn == 0)
      Unk006d24b0(var_name_buf);
    ctx->cur_val = 0;
    ctx->cur_tok = 8;
    (*ctx->get_var_fn)(var_name_buf, 0, &ctx->cur_val);
    break;
  default:
    break;
  }
}

static int i_expr(RDPICTXT *ctx);

// FUNCTION: LEGOBATMAN 0x006dba10
static int prim(RDPICTXT *ctx) {
  int expr;

  switch (ctx->cur_tok) {
  case 8:
    get_tok_rdpi(ctx);
    return ctx->cur_val;
  case 1:
    get_tok_rdpi(ctx);
    return -prim(ctx);
  case 11:
    get_tok_rdpi(ctx);
    return ~prim(ctx);
  case 15:
    get_tok_rdpi(ctx);
    return !prim(ctx);
  case 5:
    get_tok_rdpi(ctx);
    expr = i_expr(ctx);
    if (ctx->cur_tok != 6)
      Unk006d24b0();
    get_tok_rdpi(ctx);
    return expr;
  case 7:
    return 1;
  default:
    Unk006d24b0();
    return 0;
  }
}

// FUNCTION: LEGOBATMAN 0x006dbae0
static int term(RDPICTXT *ctx) {
  int term = prim(ctx);

  while (1) {
    switch (ctx->cur_tok) {
    case 2:
      get_tok_rdpi(ctx);
      term *= prim(ctx);
      break;
    case 3:
      get_tok_rdpi(ctx);
      term /= prim(ctx);
      break;
    case 4:
      get_tok_rdpi(ctx);
      term %= prim(ctx);
      break;
    case 16:
      get_tok_rdpi(ctx);
      term = term == prim(ctx);
      break;
    case 17:
      get_tok_rdpi(ctx);
      term = term != prim(ctx);
      break;
    case 18:
      get_tok_rdpi(ctx);
      term = term < prim(ctx);
      break;
    case 19:
      get_tok_rdpi(ctx);
      term = term > prim(ctx);
      break;
    case 20:
      get_tok_rdpi(ctx);
      term = term <= prim(ctx);
      break;
    case 21:
      get_tok_rdpi(ctx);
      term = term >= prim(ctx);
      break;
    default:
      return term;
    }
  }
}

// FUNCTION: LEGOBATMAN 0x006dbc40
static int i_expr(RDPICTXT *ctx) {
  int rhs;
  int expr = term(ctx);

  while (1) {
    switch (ctx->cur_tok) {
    case 0:
      get_tok_rdpi(ctx);
      expr += term(ctx);
      break;
    case 1:
      get_tok_rdpi(ctx);
      expr -= term(ctx);
      break;
    case 10:
      get_tok_rdpi(ctx);
      expr &= term(ctx);
      break;
    case 9:
      get_tok_rdpi(ctx);
      expr |= term(ctx);
      break;
    case 12:
      get_tok_rdpi(ctx);
      expr ^= term(ctx);
      break;
    case 14:
      get_tok_rdpi(ctx);
      rhs = term(ctx);
      expr = expr != 0 && rhs != 0;
      break;
    case 13:
      get_tok_rdpi(ctx);
      rhs = term(ctx);
      expr = expr != 0 || rhs != 0;
      break;
    default:
      return expr;
    }
  }
}

// FUNCTION: LEGOBATMAN 0x006dbd30
int NuRDPIVar(char *input, nurdpgetvarfn *get_var_fn) {
  RDPICTXT ctx;

  ctx.buf = input;
  ctx.pos = 0;
  ctx.get_var_fn = get_var_fn;
  get_tok_rdpi(&ctx);
  return i_expr(&ctx);
}

// FUNCTION: LEGOBATMAN 0x006dbd70
int NuRDPI(char *input) {
  RDPICTXT ctx;

  ctx.buf = input;
  ctx.pos = 0;
  ctx.get_var_fn = 0;
  get_tok_rdpi(&ctx);
  return i_expr(&ctx);
}

// FUNCTION: LEGOBATMAN 0x006dd150
int NuFParGetIntRDP(NUFPAR *parser) {
  char buf[64];

  NuFParGetWord(parser);
  if (parser->is_utf16)
    NuPadWideToAnsi(buf, (unsigned short *)parser->word_buf);
  else
    NuStrCpy(buf, parser->word_buf);
  if (buf[0] != '\0')
    return NuRDPI(buf);
  return 0;
}

// FUNCTION: LEGOBATMAN 0x006dd210
float NuFParGetFloatRDP(NUFPAR *parser) {
  char buf[64];

  NuFParGetWord(parser);
  if (parser->is_utf16)
    NuPadWideToAnsi(buf, (unsigned short *)parser->word_buf);
  else
    NuStrCpy(buf, parser->word_buf);
  if (buf[0] != '\0')
    return NuRDPF(buf);
  return 0.0f;
}

struct NuFileReadBlock {
  int tag;
  int size;
  __int64 position;
};
// GLOBAL: LEGOBATMAN 0x00aed188
static NuFileReadBlock nufile_blkinfo[1024];
// GLOBAL: LEGOBATMAN 0x0099f274
static int blkcnt = 1;

__int64 NuFilePos(int file);

// FUNCTION: LEGOBATMAN 0x006d3630
int NuFileGetBlkSize(void) { return nufile_blkinfo[blkcnt - 1].size; }

// FUNCTION: LEGOBATMAN 0x006deb90
int NuFileBeginBlkRead(int file, int expected_tag) {
  int index = blkcnt++;
  __int64 position = NuFilePos(file);
  if (index > 1 && position >= nufile_blkinfo[index - 1].position +
                                   nufile_blkinfo[index - 1].size) {
    --blkcnt;
    return 0;
  }
  nufile_blkinfo[index].position = NuFilePos(file);
  NuFileRead(file, &nufile_blkinfo[index], 8);
  if (nufile_blkinfo[index].size < 0)
    nufile_blkinfo[index].size = -nufile_blkinfo[index].size;
  if (expected_tag != 0 && expected_tag != nufile_blkinfo[index].tag)
    Unk006d24b0();
  return nufile_blkinfo[index].tag;
}

// FUNCTION: LEGOBATMAN 0x006d96f0
void NuFileEndBlkRead(int file) {
  int index = --blkcnt;
  __int64 position = NuFilePos(file);
  if (position != nufile_blkinfo[index].position + nufile_blkinfo[index].size)
    NuFileSeek(file,
               nufile_blkinfo[index].position + nufile_blkinfo[index].size, 0);
}

struct NuFileWriteBlock {
  int tag;
  int size_sign;
  __int64 position;
};
// GLOBAL: LEGOBATMAN 0x00afc850
static NuFileWriteBlock nufile_wblkinfo[1024];
// GLOBAL: LEGOBATMAN 0x0099f278
static int wblkcnt = 1;
// GLOBAL: LEGOBATMAN 0x0099f27c
static int iff_padsize = 16;

// FUNCTION: LEGOBATMAN 0x006d9750
void NuFileBeginBlkWrite(int file, int tag, int negative_size) {
  int index = wblkcnt++;
  nufile_wblkinfo[index].position = NuFilePos(file);
  nufile_wblkinfo[index].tag = tag;
  nufile_wblkinfo[index].size_sign = negative_size == 1 ? -1 : 1;
  NuFileWriteInt(file, tag);
  NuFileWriteInt(file, tag);
}

// FUNCTION: LEGOBATMAN 0x006d97b0
void NuFileEndBlkWrite(int file) {
  __int64 position;
  int index;
  int padding;
  int size;
  int zero = 0;

  index = --wblkcnt;
  position = NuFilePos(file);
  size = position - nufile_wblkinfo[index].position;
  padding = iff_padsize - size % iff_padsize;
  if (padding != iff_padsize) {
    position += padding;
    size = size + padding;
    NuFileWrite(file, &zero, padding);
  }
  if (nufile_wblkinfo[index].size_sign < 0)
    size = -size;
  NuFileSeek(file, nufile_wblkinfo[index].position + 4, 0);
  NuFileWrite(file, &size, 4);
  NuFileSeek(file, position, 0);
}

static inline char NuFileReadChar(int file) {
  char value = 0;
  NuFileRead(file, &value, 1);
  return value;
}

// FUNCTION: LEGOBATMAN 0x006dec40
int NuFileAlignRead(int file, int mask) {
  int count = 0;
  __int64 position = NuFilePos(file);
  while ((position & mask) != 0) {
    ++position;
    ++count;
    NuFileReadChar(file);
  }
  return count;
}

// FUNCTION: LEGOBATMAN 0x006deb50
char NuFilePeekCharUnk006deb50(int file) {
  char c = 0;

  if (NuFileRead(file, &c, 1) <= 0)
    return -1;
  NuFileSeek(file, -1, 1);
  return c;
}

// FUNCTION: LEGOBATMAN 0x006df630
char NuGetChar(NUFPAR *parser) {
  char c;
  int len;
  int read_to = parser->buf_end >= 0 ? parser->buf_end : 0;

  if (parser->char_pos > parser->buf_end) {
    if (parser->buf_end < parser->size) {
      len = NuFileRead(parser->file_handle, parser->file_buf,
                       parser->size - read_to <= 0x1000 ? parser->size - read_to
                                                        : 0x1000);
      parser->buf_start = parser->buf_end + 1;
      parser->buf_end += len;
    } else {
      return '\0';
    }
    if (len == 0)
      return '\0';
  }
  c = parser->file_buf[parser->char_pos - parser->buf_start];
  parser->char_pos++;
  return c;
}

// FUNCTION: LEGOBATMAN 0x006df6c0
unsigned short NuGetWChar(NUFPAR *parser) {
  unsigned char lo;
  unsigned char hi;
  unsigned short c;

  lo = NuGetChar(parser);
  hi = NuGetChar(parser);
  c = lo + (hi << 8);
  return c;
}

// FUNCTION: LEGOBATMAN 0x006df6f0
int NuFParGetLineW(NUFPAR *parser) {
  unsigned short c;
  int len;
  unsigned short *line;
  int at_end_of_line;
  int is_done;
  int in_quoted_text;

  in_quoted_text = 0;
  line = (unsigned short *)parser->line_buf;
  len = 0;
  parser->line_pos = 0;
  parser->line_num++;
  is_done = 0;

  do {
    c = NuGetWChar(parser);
    switch (c) {
    case '\r':
      NuGetWChar(parser);
    case '\n':
      if (len == 0 || !is_done) {
        parser->line_num++;
        is_done = 0;
        break;
      }
    case '\0':
      line[len] = '\0';
      return len;
    case '"':
      in_quoted_text = 1 - in_quoted_text;
      is_done = 1;
      line[len & ((parser->word_buf_size >> 1) - 1)] = c;
      len++;
      break;
    case ';':
      if (!in_quoted_text) {
        if (len == 0 || !is_done) {
          at_end_of_line = 0;
          do {
            c = NuGetWChar(parser);
            switch (c) {
            case '\r':
              NuGetWChar(parser);
            case '\n':
            case '\0':
              at_end_of_line = 1;
              break;
            default:
              break;
            }
          } while (!at_end_of_line);
          len = 0;
          parser->line_pos = 0;
          parser->line_num++;
          is_done = 0;
        } else {
          do {
            c = NuGetWChar(parser);
            switch (c) {
            case '\r':
              NuGetWChar(parser);
            case '\n':
            case '\0':
              line[len] = '\0';
              return len;
            default:
              break;
            }
          } while (1);
          return len;
        }
        break;
      }
    default:
      is_done = 1;
    case '\t':
    case ' ':
      line[len & ((parser->word_buf_size >> 1) - 1)] = c;
      len++;
      break;
    }
  } while (1);
}

// FUNCTION: LEGOBATMAN 0x006df910
int NuFParGetLine(NUFPAR *parser) {
  char c;
  int len;
  int at_end_of_line;
  int is_done;
  int in_quoted_text;

  in_quoted_text = 0;
  if (parser->is_utf16)
    return NuFParGetLineW(parser);
  len = 0;
  parser->line_pos = 0;
  parser->line_num++;
  is_done = 0;

  do {
    c = NuGetChar(parser);
    switch (c) {
    case '\r':
      NuGetChar(parser);
    case '\n':
      if (len == 0 || !is_done) {
        parser->line_num++;
        is_done = 0;
        break;
      }
    case '\0':
      parser->line_buf[len] = '\0';
      return len;
    case '"':
      in_quoted_text = 1 - in_quoted_text;
      is_done = 1;
      parser->line_buf[len & (parser->line_buf_size - 1)] = c;
      len++;
      break;
    case ';':
      if (!in_quoted_text) {
        if (len == 0 || !is_done) {
          at_end_of_line = 0;
          do {
            c = NuGetChar(parser);
            switch (c) {
            case '\r':
              NuGetChar(parser);
            case '\n':
            case '\0':
              at_end_of_line = 1;
              break;
            default:
              break;
            }
          } while (!at_end_of_line);
          len = 0;
          parser->line_pos = 0;
          parser->line_num++;
          is_done = 0;
        } else {
          do {
            c = NuGetChar(parser);
            switch (c) {
            case '\r':
              NuGetChar(parser);
            case '\n':
            case '\0':
              parser->line_buf[len] = '\0';
              return len;
            default:
              break;
            }
          } while (1);
          return len;
        }
        break;
      }
    default:
      is_done = 1;
    case '\t':
    case ' ':
      parser->line_buf[len & (parser->line_buf_size - 1)] = c;
      len++;
      break;
    }
  } while (1);
}

NUFPAR *NuAllocFPar(void);
unsigned char NuFileReadUnsignedChar(int file);
int NuFileExists(char *name);

// FUNCTION: LEGOBATMAN 0x006dfab0
NUFPAR *NuFParOpen(int file_handle) {
  NUFPAR *parser = NuAllocFPar();
  if (parser != 0) {
    parser->file_handle = file_handle;
    parser->buf_end = -1;
    parser->line_num = -1;
    parser->command_pos = -1;
    parser->separator_list = 0;
    parser->separator_tokens = 0;
    parser->line_buf = parser->line_buf_store;
    parser->word_buf = parser->word_buf_store;
    parser->line_buf_size = 0x400;
    parser->word_buf_size = 0x400;

    unsigned short first = 0;
    NuFileRead(file_handle, &first, 2);
    parser->is_utf16 = 0;
    parser->is_utf8 = 0;

    unsigned char next;
    if (first == 0xfffe) {
      parser->is_utf16 = 2;
    } else if (first == 0xfeff) {
      parser->is_utf16 = 1;
    } else if (first == 0xbbef) {
      next = NuFileReadUnsignedChar(file_handle);
      if (next == 0xbf)
        parser->is_utf8 = 1;
    } else if (first == 0xefbb) {
      next = NuFileReadUnsignedChar(file_handle);
      if (next == 0xbf)
        parser->is_utf8 = 2;
    }
    if (parser->is_utf16 == 0 && parser->is_utf8 == 0)
      NuFileSeek(file_handle, 0, 0);
    parser->size = (int)NuFileOpenSize(file_handle);
  }
  return parser;
}

// FUNCTION: LEGOBATMAN 0x006dfbc0
NUFPAR *NuFParCreate(char *filename) {
  int file_handle;

  if (NuFileExists(filename)) {
    file_handle = NuFileOpen(filename, 0);
    if (file_handle != 0) {
      NUFPAR *parser = NuFParOpen(file_handle);
      if (parser != 0) {
        NuStrCpy(parser->file_name, filename);
        return parser;
      }
      NuFileClose(file_handle);
    }
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x006dfc20
NUFPAR *NuFParCreateGivenFH(char *filename, int file_handle) {
  if (file_handle != 0) {
    NUFPAR *parser = NuFParOpen(file_handle);
    if (parser != 0) {
      NuStrCpy(parser->file_name, filename);
      return parser;
    }
    NuFileClose(file_handle);
  }
  return 0;
}

// --- nufilepak (saga nufilepak.cpp) -----------------------------------------

typedef struct nufilepakhdr_s {
  u32 type;
  u32 item_count;
  u32 file_size;
  u32 checksum;
  u32 flags;
  u32 decrypted_checksum;
} NUFILEPAKHDR;
typedef struct nufilepakitemattr_s {
  u32 required : 1;
  u32 removed : 1;
} NUFILEPAKITEMATTR;
typedef struct nufilepakitem_s {
  i32 name_offset;
  i32 data_offset;
  i32 size;
  i32 alignment;
  NUFILEPAKITEMATTR attr;
  void *pre_move_fn;
  void *post_move_fn;
} NUFILEPAKITEM;

// GLOBAL: LEGOBATMAN 0x00b038b4
static i32 fpk_err;

void NuMemCpy(unsigned char *dst, unsigned char *src, int n);
i32 NuFileLoadBuffer(char *name, void *buffer, i32 size);

// FUNCTION: LEGOBATMAN 0x006d3c20
i32 NuFilePakGetLastError(void) { return fpk_err; }

// FUNCTION: LEGOBATMAN 0x006d3e00
u32 NuFilePakGetSize(void *hdr_ptr) {
  fpk_err = 0;
  NUFILEPAKHDR *hdr = (NUFILEPAKHDR *)hdr_ptr;
  return hdr->file_size;
}

// FUNCTION: LEGOBATMAN 0x006d3ce0
void NuFilePakRelocate(void *hdr_ptr, void *dest) {
  fpk_err = 0;
  NUFILEPAKHDR *hdr = (NUFILEPAKHDR *)hdr_ptr;
  NuMemCpy((unsigned char *)dest, (unsigned char *)hdr_ptr, hdr->file_size);
}

// FUNCTION: LEGOBATMAN 0x006d3bb0
static u32 CalcChecksum(void *data, i32 len) {
  unsigned char *bytes;
  u32 sum;

  sum = 0x12345678;
  bytes = (unsigned char *)data;
  while (len != 0) {
    len--;
    sum += *(bytes++);
  }
  return sum;
}

// FUNCTION: LEGOBATMAN 0x006d3be0
static void DecryptData(NUFILEPAKHDR *hdr, u32 data_len, unsigned char *key,
                        i32 key_len) {
  unsigned char *bytes;
  i32 i;

  data_len -= sizeof(NUFILEPAKHDR);
  i = 0;
  bytes = (unsigned char *)(hdr + 1);
  while (data_len != 0) {
    data_len--;
    *bytes = *bytes - key[i++];
    if (i >= key_len)
      i = 0;
    bytes++;
  }
  hdr->flags &= ~1;
}

// FUNCTION: LEGOBATMAN 0x006d3b90
static NUFILEPAKITEM *GetItems(NUFILEPAKHDR *hdr) {
  switch (hdr->type) {
  case 0x12345678:
    return (NUFILEPAKITEM *)((char *)hdr + 0xc);
  default:
    return (NUFILEPAKITEM *)((char *)hdr + 0x14);
  case 0x1234567a:
    return (NUFILEPAKITEM *)(hdr + 1);
  }
}

// FUNCTION: LEGOBATMAN 0x006d3c30
i32 NuFilePakCondenseTo(void *hdr_ptr, VARIPTR *buf) {
  NUFILEPAKHDR *hdr = (NUFILEPAKHDR *)hdr_ptr;
  NUFILEPAKITEM *items;
  u32 i;

  if (hdr->item_count == 0)
    return 0;
  items = GetItems(hdr);
  fpk_err = 0;
  for (i = 0; i < hdr->item_count; i++) {
    if (!items[i].attr.removed) {
      if (items[i].attr.required) {
        if (items[i].alignment != 0)
          buf->addr =
              (buf->addr + items[i].alignment - 1) & ~(items[i].alignment - 1);
        if (buf->addr != (u32)items[i].data_offset) {
          NuMemCpy(buf->u8_ptr, (unsigned char *)hdr + items[i].data_offset,
                   items[i].size);
          items[i].data_offset = buf->addr - (u32)hdr;
        }
        buf->addr += items[i].size;
      } else {
        items[i].attr.removed = 1;
      }
    }
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x006da6b0
i32 NuFilePakCondense(void *hdr_ptr) {
  NUFILEPAKHDR *hdr = (NUFILEPAKHDR *)hdr_ptr;
  VARIPTR buf;
  i32 size;
  i32 removed;

  if (hdr->item_count == 0)
    return 0;
  buf.addr = (u32)hdr + GetItems(hdr)->data_offset;
  NuFilePakCondenseTo(hdr, &buf);
  size = buf.addr - (u32)hdr;
  removed = hdr->file_size - size;
  hdr->file_size = size;
  return removed;
}

// FUNCTION: LEGOBATMAN 0x006d3d10
i32 NuFilePakGetItemInfo(void *hdr_ptr, i32 item_handle, void **addr,
                         i32 *size) {
  NUFILEPAKHDR *hdr;
  NUFILEPAKITEM *item;

  fpk_err = 0;
  hdr = (NUFILEPAKHDR *)hdr_ptr;
  item_handle -= 1;
  if (item_handle >= 0 && item_handle < hdr->item_count) {
    item = GetItems(hdr);
    item = &item[item_handle];
    if (!item->attr.removed) {
      if (addr != NULL)
        *addr = (void *)((char *)hdr_ptr + item->data_offset);
      if (size != NULL)
        *size = item->size;
      return 1;
    }
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x006d3d90
i32 NuFilePakSetItemRequired(void *hdr_ptr, i32 item_handle, i32 required) {
  fpk_err = 0;
  NUFILEPAKHDR *hdr = (NUFILEPAKHDR *)hdr_ptr;
  --item_handle;
  if (item_handle >= 0 && item_handle < hdr->item_count) {
    NUFILEPAKITEM *items = GetItems(hdr);
    items = &items[item_handle];
    if (!items->attr.removed)
      items->attr.required = required;
    return 1;
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x006e0b60
void *NuFilePakLoadKeyPrePad(char *filepath, VARIPTR *buf, VARIPTR buf_end,
                             i32 alignment, unsigned char *key, u32 key_len,
                             i32 pre_pad) {
  NUFILEPAKHDR *hdr;
  i32 len;
  u32 stored_sum;

  fpk_err = 0;
  if (alignment > 0x2000) {
    Unk006d24d0();
    fpk_err = 5;
    return NULL;
  }
  alignment = alignment > 4 ? alignment : 4;
  buf->addr = (buf->addr + alignment - 1) & ~(alignment - 1);
  hdr = (NUFILEPAKHDR *)buf->void_ptr;
  len = NuFileLoadBuffer(filepath, hdr, buf_end.addr - buf->addr);
  buf->addr += len;
  if (len == 0) {
    fpk_err = 1;
    return NULL;
  }
  if (len != hdr->file_size) {
    Unk006d24d0();
    fpk_err = 5;
    return NULL;
  }

  switch (hdr->type) {
  case 0x12345678:
    break;
  case 0x12345679:
    stored_sum = hdr->checksum;
    hdr->checksum = 0;
    if (stored_sum != CalcChecksum(hdr, len)) {
      Unk006d24d0();
      fpk_err = 3;
      return NULL;
    }
    break;
  case 0x1234567a:
    stored_sum = hdr->checksum;
    hdr->checksum = 0;
    if (stored_sum != CalcChecksum(hdr, len)) {
      Unk006d24d0();
      fpk_err = 3;
      return NULL;
    }
    if ((hdr->flags & 1) != 0) {
      if (key != NULL) {
        DecryptData(hdr, len, key, key_len);
        stored_sum = hdr->decrypted_checksum;
        hdr->decrypted_checksum = 0;
        if (stored_sum == CalcChecksum(hdr, len))
          break;
      }
      Unk006d24d0();
      fpk_err = 6;
      return NULL;
    }
    break;
  default:
    Unk006d24d0(hdr->type);
    fpk_err = 4;
    return NULL;
  }

  if (pre_pad != 0) {
    NUFILEPAKITEM *items = GetItems(hdr);
    u32 end;
    u32 i;

    end =
        (((u32)hdr + items[0].data_offset + pre_pad + items[0].alignment - 1) &
         ~(items[0].alignment - 1)) +
        items[0].size;
    for (i = 1; i < hdr->item_count; i++) {
      end = ((end + pre_pad + items[i].alignment - 1) &
             ~(items[i].alignment - 1)) +
            items[i].size;
    }
    if (end >= buf_end.addr) {
      fpk_err = 7;
      return NULL;
    }
    buf->addr = end;
    for (i = hdr->item_count; i != 0;) {
      unsigned char *src;
      unsigned char *dst;

      i--;
      src = (unsigned char *)hdr + items[i].data_offset;
      dst =
          (unsigned char *)((end - items[i].size) & ~(items[i].alignment - 1));
      NuMemCpy(dst, src, items[i].size);
      items[i].data_offset += dst - src;
      end = (u32)dst - pre_pad;
    }
  }
  return hdr;
}

// FUNCTION: LEGOBATMAN 0x006dcf10
i32 NuFilePakGetItem(void *hdr_ptr, char *item_name) {
  char buf[256];
  NUFILEPAKITEM *items;
  NUFILEPAKHDR *hdr;
  i32 j;
  u32 i;

  hdr = (NUFILEPAKHDR *)hdr_ptr;
  NuStrCpy(buf, item_name);
  for (j = 0; j < NuStrLen(item_name); j++) {
    if (buf[j] == '/')
      buf[j] = '\\';
    else if (buf[j] == '\\')
      buf[j] = '/';
  }
  fpk_err = 0;
  items = GetItems(hdr);
  for (i = 0; i < hdr->item_count; i++) {
    if (NuStrICmp((char *)hdr + items[i].name_offset, item_name) == 0) {
      if (items[i].attr.removed)
        return 0;
      return i + 1;
    }
    if (NuStrICmp((char *)hdr + items[i].name_offset, buf) == 0) {
      if (items[i].attr.removed)
        return 0;
      return i + 1;
    }
  }
  return 0;
}

// Opens a file held in the small-file pak (g_nuFileUnk0b038a4) as id 0x2000.
// FUNCTION: LEGOBATMAN 0x006dd740
int NuFileSmallFileOpen(char *path) {
  int item;

  if (g_nuFileUnk0b038a4 != 0) {
    item = NuFilePakGetItem((void *)g_nuFileUnk0b038a4, path);
    if (item != 0) {
      while (g_nuFileUnk0b00878[0].i0 != -1) {
      }
      g_nuFileUnk0b00878[0].i0 = item;
      g_nuFileUnk0b00878[0].i4 = 0;
      NuFilePakGetItemInfo((void *)g_nuFileUnk0b038a4, item,
                           (void **)&g_nuFileUnk0b00878[0].i8,
                           &g_nuFileUnk0b00878[0].ic);
      return 0x2000;
    }
  }
  return 0;
}

// GLOBAL: LEGOBATMAN 0x00b038a0
int nufile_lasterror;
// GLOBAL: LEGOBATMAN 0x00b03890
extern int nufile_try_packed;
int NuDatFileLoadBuffer(NUDATHDR *dat, char *name, void *dest, int buf_size);
struct NUPPHEADER {
  unsigned int magic;
  unsigned int unpacked_size;
  unsigned int packed_size;
};

#define NUPP_SWAP32(value)                                                     \
  (((value) >> 24) | (((value) & 0x00ff0000) >> 8) |                           \
   (((value) & 0x0000ff00) << 8) | ((value) << 24))

// FUNCTION: LEGOBATMAN 0x006dfd20
extern "C" int NuPPGetSize(int file) {
  int size;
  int original_position;
  NUPPHEADER header;
  original_position = (int)NuFileSeek(file, 0, 1);
  NuFileSeek(file, 0, 0);
  NuFileRead(file, &header, sizeof(header));
  if (header.magic == 0x02434e52) {
    header.unpacked_size = NUPP_SWAP32(header.unpacked_size);
    header.packed_size = NUPP_SWAP32(header.packed_size) + 6;
    size = header.unpacked_size;
  } else {
    size = (int)NuFileOpenSize(file);
  }
  NuFileSeek(file, original_position, 0);
  return size;
}

extern "C" void NuPPUnpack(void *source, void *destination);

// STUB: LEGOBATMAN 0x006dfde0
// saga body with the bounds-checked seek; the original lays the small-file
// seek path out of line and its retry loops differ (ours 1182 B).
int NuPPLoadBuffer(int file, void *buf, int buf_size) {
  int size;
  int loaded;
  unsigned char *source;
  NUPPHEADER header;
  do {
    while (NuFileSeekStart(file, 0) < 0)
      Unk006d24d0();
    loaded = NuFileRead(file, &header, sizeof(header));
    if (loaded < 0)
      Unk006d24d0();
  } while (loaded < 0);
  if (header.magic == 0x02434e52) {
    header.unpacked_size = NUPP_SWAP32(header.unpacked_size);
    header.packed_size = NUPP_SWAP32(header.packed_size) + 18;
    source = (unsigned char *)buf;
    source += header.unpacked_size + 256;
    source -= header.packed_size;
    do {
      while (NuFileSeekStart(file, 4) < 0)
        Unk006d24d0();
      loaded = NuFileRead(file, source, header.packed_size - 4);
      if (loaded < 0)
        Unk006d24d0();
    } while (loaded < 0);
    NuPPUnpack(source - 4, buf);
    size = header.unpacked_size;
  } else {
    size = (int)NuFileOpenSize(file);
    do {
      while (NuFileSeekStart(file, 0) < 0)
        Unk006d24d0();
      loaded = NuFileRead(file, buf, size);
      if (loaded < 0)
        Unk006d24d0();
    } while (loaded < 0);
  }
  return size;
}

// FUNCTION: LEGOBATMAN 0x006e0830
void *NuFileLoad(char *filepath) {
  int buffering;
  int file;
  int size;
  void *buf;

  buffering = nufile_buffering_enabled;
  buf = NULL;
  nufile_buffering_enabled = 0;
  file = NuFileOpen(filepath, 0);
  if (file != 0) {
    size = (int)NuFileOpenSize(file);
    if (size > 0) {
      buf = NuMemAllocFn(size, __FILE__, 0x82a);
      if (buf != NULL)
        NuFileRead(file, buf, size);
    }
    NuFileClose(file);
  }
  nufile_buffering_enabled = buffering;
  return buf;
}

// FUNCTION: LEGOBATMAN 0x006e08b0
int NuFileLoadBuffer(char *filepath, void *buf, int buf_size) {
  int file;
  int loaded;

  nufile_lasterror = 0;
  loaded = 0;
  if (g_unk00b03884 != 0) {
    loaded =
        NuDatFileLoadBuffer((NUDATHDR *)g_unk00b03884, filepath, buf, buf_size);
    if (nufile_lasterror == -1)
      return 0;
  }
  if (loaded == 0) {
    if (NuFileExists(filepath)) {
      file = NuFileOpen(filepath, 0);
      if (file != 0) {
        if (nufile_try_packed) {
          loaded = NuPPLoadBuffer(file, buf, buf_size);
        } else {
          loaded = (int)NuFileOpenSize(file);
          if (loaded <= buf_size && loaded != 0) {
            while (NuFileRead(file, buf, loaded) < 0) {
              Unk006d24d0();
              while (NuFileSeek(file, 0, 0) < 0)
                Unk006d24d0();
            }
          } else {
            nufile_lasterror = -1;
            loaded = 0;
          }
        }
        NuFileClose(file);
      } else {
        nufile_lasterror = -3;
      }
    } else {
      nufile_lasterror = -2;
    }
  }
  return loaded;
}

// FUNCTION: LEGOBATMAN 0x006e0a10
int NuFileLoadBufferVP(char *filepath, VARIPTR *buf, VARIPTR *buf_end) {
  int len;

  len = NuFileLoadBuffer(filepath, buf->void_ptr, buf_end->addr - buf->addr);
  buf->addr += len;
  return len;
}

// FUNCTION: LEGOBATMAN 0x006e0a40
int NuFileCopyEx(char *dst, char *src, void *buf, int buf_size) {
  int in;
  int out;
  int size;
  int n;
  int total;

  total = 0;
  in = NuFileOpen(src, 0);
  out = NuFileOpen(dst, 1);
  if (in != 0) {
    if (out != 0) {
      size = (int)NuFileOpenSize(in);
      if (size != 0) {
        do {
          n = NuFileRead(in, buf, size < buf_size ? size : buf_size);
          NuFileWrite(out, buf, n);
          total += n;
          size -= n;
        } while (size != 0);
      }
    }
    NuFileClose(in);
  }
  if (out != 0)
    NuFileClose(out);
  return total;
}

// FUNCTION: LEGOBATMAN 0x006e0b10
int NuFileCopy(char *dst, char *src) {
  char buf[0x400];

  return NuFileCopyEx(dst, src, buf, 0x400);
}

int NuFileStatus(int file);

// STUB: LEGOBATMAN 0x006decb0
// Matches as is once 0x006d2fe0's case 0 reads
// `if (offset > ic + 1) Unk006d24b0();`, but then 0x006d2fe0 and
// NuFilePatchAddress's inlined copy keep that call too (the originals drop
// it); string arguments drop it everywhere instead.
int NuDatFileLoadBuffer(NUDATHDR *dat, char *name, void *dest, int buf_size) {
  int file;
  char *buf;

  nufile_lasterror = 0;
  file = NuDatFileOpen(dat, name, 0);
  if (file != 0) {
    int size;

    if (dat->mode == 3) {
      while (NuFileStatus(file) != 0) {
      }
    }
    if (g_nuFileUnk0ae3f80[file - 0x800].i28 != 0)
      size = g_nuFileUnk0ae3f80[file - 0x800].i1c;
    else
      size = g_nuFileUnk0ae3f80[file - 0x800].i18;
    buf = (char *)dest;
    if (size <= buf_size && size != 0) {
      while (NuDatFileRead(file, buf, size) < 0) {
        Unk006d24d0();
        while (NuDatFileSeek(file, 0, 0) < 0)
          Unk006d24d0();
      }
      if (dat->mode == 3) {
        while (NuFileStatus(file) != 0) {
        }
      }
      NuFileClose(file);
      return size;
    }
    if (size != 0)
      nufile_lasterror = -1;
    NuFileClose(file);
  }
  return 0;
}

// --- nuqt (saga nu2api/nucore/nuqt.cpp) -------------------------------------

struct nuqtdim_s {
  f32 x0;
  f32 x1;
  f32 y0;
  f32 y1;
};

struct nuqtentry_s {
  i16 count;
  i16 child;
  union {
    struct {
      u8 *data;
      u32 field_08;
    };
    u16 children[4];
  };
};

struct nuqthdr_s {
  union {
    u32 field_00[5];
    f32 bounds_values[5];
  };
  nuqtentry_s *entries;
  i32 entry_count;
  i32 entry_capacity;
  u8 *data;
  i32 data_used;
  i32 data_capacity;
  i32 element_size;
  i32 field_30;
  i32 field_34;
};

// FUNCTION: LEGOBATMAN 0x006d4510
static int ElOverlaps(nuqtdim_s *a, nuqtdim_s *b) {
  if (b->x0 < a->x1 && b->x1 > a->x0 && b->y1 < a->y0 && b->y0 > a->y1)
    return 1;
  return 0;
}

// FUNCTION: LEGOBATMAN 0x006d4560
static i32 InsertData(nuqthdr_s *header, i32 index, void *item) {
  u8 *destination = header->entries[index].data;
  if (destination == NULL) {
    // The original tests the remaining capacity in this direction and copies
    // through the cached null pointer, even after assigning the entry data.
    if (header->data_capacity - header->data_used <= header->element_size) {
      header->entries[index].data = header->data + header->data_used;
      memcpy(destination, item, header->element_size);
      header->data_used += header->element_size;
      ++header->entries[index].count;
      return 1;
    }
  } else {
    u8 *source = header->data + header->data_used;
    u8 *target = source + header->element_size;
    if (target <= header->data + header->data_capacity) {
      while (source != destination) {
        --target;
        --source;
        *target = *source;
      }
      memcpy(destination, item, header->element_size);
      header->data_used += header->element_size;
      ++header->entries[index].count;
      return 1;
    }
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x006d4620
static void RemoveData(nuqthdr_s *header, char *data, i32 count) {
  char *source;
  char *destination;
  i32 length = header->element_size * count;
  char *end = (char *)header->data + header->data_used;
  destination = data;
  source = destination + length;
  while (source < end) {
    *destination = *source;
    ++destination;
    ++source;
  }
}

// FUNCTION: LEGOBATMAN 0x006d4650
static i32 AddNode(nuqthdr_s *header, i32 child) {
  i32 index = 0;
  if (header->entry_count < header->entry_capacity) {
    index = header->entry_count;
    memset(&header->entries[index], 0, sizeof(nuqtentry_s));
    header->entries[index].child = child;
    header->entries[index].count = -1;
    ++header->entry_count;
  }
  return index;
}

// FUNCTION: LEGOBATMAN 0x006d46a0
static i32 AddElementR(nuqthdr_s *header, i32 index, nuqtdim_s *bounds,
                       nuqtdim_s *item_bounds, void *item, i32 depth) {
  i32 result = 0;
  --depth;
  nuqtdim_s q0 = {bounds->x0, (bounds->x0 + bounds->x1) / 2.0f, bounds->y0,
                  (bounds->y0 + bounds->y1) / 2.0f};
  nuqtdim_s q1 = {(bounds->x0 + bounds->x1) / 2.0f, bounds->x1, bounds->y0,
                  (bounds->y0 + bounds->y1) / 2.0f};
  nuqtdim_s q2 = {bounds->x0, (bounds->x0 + bounds->x1) / 2.0f,
                  (bounds->y0 + bounds->y1) / 2.0f, bounds->y1};
  nuqtdim_s q3 = {(bounds->x0 + bounds->x1) / 2.0f, bounds->x1,
                  (bounds->y0 + bounds->y1) / 2.0f, bounds->y1};
  if (header->entries[index].count >= 0) {
    if (header->entries[index].count < header->field_30 || depth == 0) {
      result = InsertData(header, index, item);
      return result;
    }

    u8 *old_data = header->entries[index].data;
    header->entries[index].children[0] = AddNode(header, index);
    header->entries[index].children[1] = AddNode(header, index);
    header->entries[index].children[2] = AddNode(header, index);
    header->entries[index].children[3] = AddNode(header, index);
    // The original rejects a nonzero first child, unlike the other three
    // children.
    if (header->entries[index].children[0] != 0 ||
        header->entries[index].children[1] == 0 ||
        header->entries[index].children[2] == 0 ||
        header->entries[index].children[3] == 0) {
      header->entries[index].data = old_data;
      result = InsertData(header, index, item);
      return result;
    }
    i32 i;
    u8 *old_item = old_data;
    for (i = 0; i < header->entries[index].count; ++i) {
      // The original tests the incoming item's bounds when redistributing
      // existing data, and uses the saved data pointer for each recursive item.
      if (ElOverlaps(&q0, item_bounds))
        result = AddElementR(header, header->entries[index].children[0], &q0,
                             item_bounds, old_item, depth);
      if (ElOverlaps(&q1, item_bounds))
        result = AddElementR(header, header->entries[index].children[1], &q1,
                             item_bounds, old_item, depth);
      if (ElOverlaps(&q2, item_bounds))
        result = AddElementR(header, header->entries[index].children[2], &q2,
                             item_bounds, old_item, depth);
      if (ElOverlaps(&q3, item_bounds))
        result = AddElementR(header, header->entries[index].children[3], &q3,
                             item_bounds, old_item, depth);
      old_item += header->element_size;
    }
    RemoveData(header, reinterpret_cast<char *>(old_data),
               header->entries[index].count);
    header->entries[index].count = -1;
  }
  if (header->entries[index].count < 0) {
    if (ElOverlaps(&q0, item_bounds)) {
      if (header->entries[index].children[0] != 0)
        result = AddElementR(header, header->entries[index].children[0], &q0,
                             item_bounds, item, depth);
    }
    if (ElOverlaps(&q1, item_bounds)) {
      if (header->entries[index].children[1] != 0)
        result = AddElementR(header, header->entries[index].children[1], &q1,
                             item_bounds, item, depth);
    }
    if (ElOverlaps(&q2, item_bounds)) {
      if (header->entries[index].children[2] != 0)
        result = AddElementR(header, header->entries[index].children[2], &q2,
                             item_bounds, item, depth);
    }
    if (ElOverlaps(&q3, item_bounds)) {
      if (header->entries[index].children[3] != 0)
        result = AddElementR(header, header->entries[index].children[3], &q3,
                             item_bounds, item, depth);
    }
  }
  return result;
}

// FUNCTION: LEGOBATMAN 0x006d4a70
extern "C" void NuQTAddElement(nuqthdr_s *header, void *item, f32 x0, f32 x1,
                               f32 y0, f32 y1) {
  u16 root = 0;
  nuqtdim_s bounds;
  bounds.y0 = header->bounds_values[1];
  bounds.y1 = header->bounds_values[2];
  bounds.x0 = header->bounds_values[3];
  bounds.x1 = header->bounds_values[4];
  nuqtdim_s item_bounds;
  item_bounds.y0 = y0;
  item_bounds.y1 = y1;
  item_bounds.x0 = x0;
  item_bounds.x1 = x1;
  AddElementR(header, root, &bounds, &item_bounds, item, header->field_34);
}

// FUNCTION: LEGOBATMAN 0x006d4b70
static void NuQTUnfixAddress(nuqthdr_s *header) {
  unsigned int base = -(unsigned int)header;
  for (i32 index = 0; index < header->entry_count; ++index) {
    if (header->entries[index].count > 0)
      header->entries[index].data = header->entries[index].data + base;
  }
  header->entries = (nuqtentry_s *)((u8 *)header->entries + base);
  header->data = header->data + base;
}

// FUNCTION: LEGOBATMAN 0x006d4bb0
static void NuQTFixAddress(nuqthdr_s *header) {
  unsigned int base = (unsigned int)header;
  header->entries = (nuqtentry_s *)((u8 *)header->entries + base);
  header->data = header->data + base;
  for (i32 index = 0; index < header->entry_count; ++index) {
    if (header->entries[index].count > 0)
      header->entries[index].data = header->entries[index].data + base;
  }
}

// FUNCTION: LEGOBATMAN 0x006ddc00
extern "C" i32 NuQTWrite(char *path, nuqthdr_s *header) {
  int file = NuFileOpen(path, 1);
  if (file != 0) {
    unsigned int size = 0 - (unsigned int)header;
    size += (unsigned int)header->data;
    size += header->data_capacity;
    NuQTUnfixAddress(header);
    NuFileWrite(file, header, (i32)size);
    NuQTFixAddress(header);
    NuFileClose(file);
    return 1;
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x006d4ae0
extern "C" i32 NuQTCreate(i32 entry_capacity, i32 data_capacity,
                          i32 element_size, i32 field_34, i32 field_30,
                          f32 field_04, f32 field_0c, f32 field_08,
                          f32 field_10, u8 **cursor, u8 **end) {
  *cursor = reinterpret_cast<u8 *>(
      (reinterpret_cast<unsigned int>(*cursor) + 15) & ~unsigned int(15));
  i32 required_size = sizeof(nuqthdr_s) + sizeof(nuqtentry_s) * entry_capacity +
                      16 + data_capacity;
  if (reinterpret_cast<unsigned int>(*cursor) + required_size <=
      reinterpret_cast<unsigned int>(*end)) {
    nuqthdr_s *header = reinterpret_cast<nuqthdr_s *>(*cursor);
    *cursor += sizeof(nuqthdr_s);
    header->field_00[0] = 0;
    header->entry_count = 0;
    header->entry_capacity = entry_capacity;
    header->data_used = 0;
    header->data_capacity = data_capacity;
    header->bounds_values[1] = field_04;
    header->bounds_values[3] = field_0c;
    header->bounds_values[2] = field_08;
    header->bounds_values[4] = field_10;
    header->element_size = element_size;
    header->field_34 = field_34;
    header->field_30 = field_30;
    header->entries = reinterpret_cast<nuqtentry_s *>(*cursor);
    *cursor += sizeof(nuqtentry_s) * entry_capacity;
    *cursor = reinterpret_cast<u8 *>(
        (reinterpret_cast<unsigned int>(*cursor) + 15) & ~unsigned int(15));
    header->data = *cursor;
    *cursor += data_capacity;
  }
  return 0;
}

// Reads size bytes at offset; returns 0 without reading for an LSN-mode file
// when both are sector (0x800) aligned.
// FUNCTION: LEGOBATMAN 0x006de9e0
int Unk006de9e0(int file, void *buf, int size, int offset) {
  int read = 0;
  if (nufile_lsn_allowed && file_info[file - 1].unused && !(size & 0x7ff) &&
      !(offset & 0x7ff)) {
    // sector-aligned LSN read: compiled out on PC
  } else {
    NuFileSeek(file, offset, 0);
    read = NuFileRead(file, buf, size);
  }
  return read;
}
