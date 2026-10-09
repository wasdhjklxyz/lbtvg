// nu2api/nufile/nufile.cpp (saga): the typed readers over NuFileRead. PC
// order is float, int, uint, short, ushort, char, uchar, wchar; the twins
// (int/uint, short/ushort) compile identically, so which is which follows
// that grouping, not the code.

#include "../nucore/common.h"

int NuFileReadUnk006de860(int file, void *dst, int size);

// FUNCTION: LEGOBATMAN 0x006dea40
f32 NuFileReadFloat(int file) {
  f32 value;
  NuFileReadUnk006de860(file, &value, sizeof(f32));
  return value;
}

// FUNCTION: LEGOBATMAN 0x006dea60
i32 NuFileReadInt(int file) {
  i32 value;
  NuFileReadUnk006de860(file, &value, sizeof(i32));
  return value;
}

// FUNCTION: LEGOBATMAN 0x006dea80
u32 NuFileReadUnsignedInt(int file) {
  u32 value;
  NuFileReadUnk006de860(file, &value, sizeof(u32));
  return value;
}

// FUNCTION: LEGOBATMAN 0x006deaa0
i16 NuFileReadShort(int file) {
  i16 value;
  NuFileReadUnk006de860(file, &value, sizeof(i16));
  return value;
}

// FUNCTION: LEGOBATMAN 0x006deac0
u16 NuFileReadUnsignedShort(int file) {
  u16 value;
  NuFileReadUnk006de860(file, &value, sizeof(u16));
  return value;
}

// FUNCTION: LEGOBATMAN 0x006deae0
i8 NuFileReadChar(int file) {
  i8 value = 0;
  NuFileReadUnk006de860(file, &value, sizeof(i8));
  return value;
}

// FUNCTION: LEGOBATMAN 0x006deb00
u8 NuFileReadUnsignedChar(int file) {
  u8 value = 0;
  NuFileReadUnk006de860(file, &value, sizeof(u8));
  return value;
}

// FUNCTION: LEGOBATMAN 0x006deb20
u16 NuFileReadWChar(int file) {
  i16 value = 0;
  NuFileReadUnk006de860(file, &value, sizeof(u16));
  return value;
}

struct nudathdr_s;

nudathdr_s *NuDatOpenEx(char *filepath, variptr_u *buf, i32 *unused, i16 mode);

// FUNCTION: LEGOBATMAN 0x006df610
nudathdr_s *NuDatOpen(char *filepath, variptr_u *buf, i32 *unused) {
  return NuDatOpenEx(filepath, buf, unused, 0);
}
