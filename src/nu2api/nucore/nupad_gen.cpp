// nu2api/nucore/nupad_gen.cpp: certain range 0x006d5700..0x006e0530 (two
// __FILE__ anchors). The string helpers from ~0x006d7500 on may belong to a
// neighbouring TU; see the report.

#include "common.h"

// Outside this file; names unknown.
void Unk006e2ac0(void);
void Unk006e3c90(void);
void Unk006e3ca0(void);
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

// 12-byte entries; +4 is a valid flag, +0 the value.
struct NuPadUnk0aecb64 {
  int value;
  int valid;
  int pad;
};

// GLOBAL: LEGOBATMAN 0x00aecb64
NuPadUnk0aecb64 g_nuPadUnk0aecb64[1];

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
unsigned short g_nuPadUnk0b03924;

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
int NuPadUnk006d6500(void) { return g_nuPadUnk0adf658; }

// FUNCTION: LEGOBATMAN 0x006d65d0
int NuPadUnk006d65d0(void) { return g_nuPadUnk0adf654; }

// FUNCTION: LEGOBATMAN 0x006d65e0
void NuPadUnk006d65e0(void) {}

// FUNCTION: LEGOBATMAN 0x006d65f0
void NuPadUnk006d65f0(float value) {
  g_nuPadUnk0adf6e0 = value;
  Unk006e2ac0();
}

// FUNCTION: LEGOBATMAN 0x006d6830
int NuPadUnk006d6830(void) { return g_nuPadUnk0adf648; }

// FUNCTION: LEGOBATMAN 0x006d6840
void NuPadUnk006d6840(int value) {
  g_nuPadUnk0adf648 = value;
  Unk006e3c90();
}

// FUNCTION: LEGOBATMAN 0x006d6850
int NuPadUnk006d6850(void) { return g_nuPadUnk0adf64c; }

// FUNCTION: LEGOBATMAN 0x006d6860
void NuPadUnk006d6860(int value) {
  g_nuPadUnk0adf64c = value;
  Unk006e3ca0();
}

struct NuPadUnk006d7400 {
  unsigned char b0;
  unsigned char pad[3];
  int i4;
  int i8;
  int ic;

  NuPadUnk006d7400 *Clear();
  NuPadUnk006d7400 *Set(int start, int length);
};

// FUNCTION: LEGOBATMAN 0x006d7400
NuPadUnk006d7400 *NuPadUnk006d7400::Clear() {
  b0 = 0;
  i4 = 0;
  i8 = 0;
  ic = 0;
  return this;
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
void NuPadUnk006d8750(int value) { g_nuPadUnk099f308 = value; }

// FUNCTION: LEGOBATMAN 0x006d8770
void NuPadUnk006d8770(unsigned int value) {
  if (value <= 2)
    g_nuPadUnk0b03924 = (unsigned short)value;
}

struct NuPadUnk006dd5b0 {
  int a;
  int b;
};

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

// STUB: LEGOBATMAN 0x006d8560
int NuAsciiToUnicode(unsigned short *dst, const char *src) {
  int n = 0;
  *dst = 0;
  if (src) {
    unsigned char c = *src;
    while (c) {
      dst[n] = c;
      c = src[1 + n];
      n++;
    }
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

// ids 0..0x3ff
struct NuFileUnk0ae3718 {
  __int64 size;
  unsigned char pad[0x40 - 8];
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
  int i24;
  int i28;
  unsigned char pad[0x30 - 0x2c];
};
// ids 0x2000..
struct NuFileUnk0b00884 {
  int i0;
  int i4;
  int i8;
  int ic;
};

// GLOBAL: LEGOBATMAN 0x00ae3718
NuFileUnk0ae3718 g_nuFileUnk0ae3718[1];
// GLOBAL: LEGOBATMAN 0x00afa0a8
NuFileUnk0afa0a8 g_nuFileUnk0afa0a8[1];
// GLOBAL: LEGOBATMAN 0x00ae3f80
NuFileUnk0ae3f80 g_nuFileUnk0ae3f80[1];
// GLOBAL: LEGOBATMAN 0x00b00884
NuFileUnk0b00884 g_nuFileUnk0b00884[1];
// GLOBAL: LEGOBATMAN 0x00b038a4
int g_nuFileUnk0b038a4;

// STUB: LEGOBATMAN 0x006d9370
__int64 NuFileUnk006d9370(int id) {
  if (id >= 0x2000)
    return g_nuFileUnk0b038a4 ? g_nuFileUnk0b00884[id - 0x2000].i0 : 0;
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
  return g_nuFileUnk0ae3718[id].size;
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

int NuFileOpenUnk006dd7a0(char *path, int mode, int a, int b);
int NuFileReadUnk006de860(int file, void *dst, int size);
extern "C" void *NuMemAllocFn(int size, const char *file, int line);
void Unk006d24b0(void);

static inline int NuFileReadInt(int file) {
  int v;
  NuFileReadUnk006de860(file, &v, 4);
  return v;
}

// FUNCTION: LEGOBATMAN 0x006e0530
void NuPadRecordLoad(char *filepath, VARIPTR *buffer, VARIPTR end) {
  g_nuPadRec.mode = 0;
  if (filepath != 0) {
    int file = NuFileOpenUnk006dd7a0(filepath, 0, g_unk00b03884, 0);
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
