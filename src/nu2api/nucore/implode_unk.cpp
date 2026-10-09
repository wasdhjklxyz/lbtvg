// nu2api/nucore/implode.cpp (saga): ImplodeError..ImplodeHufDecodeStart, Mac
// order.

#include "common.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

// FUNCTION: LEGOBATMAN 0x006d6910
// from saga nu2api/nucore/implode.cpp
void ImplodeError(char *msg, ...) {
  va_list args;
  va_start(args, msg);

  putc('\n', stderr);
  vfprintf(stderr, msg, args);
  putc('\n', stderr);

  exit(1);
}

// GLOBAL: LEGOBATMAN 0x0099f2f0
static unsigned int getmasktbl[5] = {0, 0xff, 0xffff, 0xffffff, 0xffffffff};

// GLOBAL: LEGOBATMAN 0x00adf73c
unsigned char *implode_inbuffer;
// GLOBAL: LEGOBATMAN 0x00ae4340
unsigned char *implode_outbuffer;

// FUNCTION: LEGOBATMAN 0x006d6960
// from saga nu2api/nucore/implode.cpp
int ImplodeGetI(void *buf, int size) {
  unsigned char *char_ptr;
  unsigned int buf_reversed;

  char_ptr = (unsigned char *)buf;
  buf_reversed = *char_ptr | *(char_ptr + 1) << 0x8 | *(char_ptr + 2) << 0x10 |
                 *(char_ptr + 3) << 0x18;

  return buf_reversed & getmasktbl[size];
}

// FUNCTION: LEGOBATMAN 0x006d6990
void *ImplodePutI(void *buf, unsigned int value, int size) {
  unsigned char *p = (unsigned char *)buf;
  for (; size != 0; size--) {
    *p++ = value;
    value >>= 8;
  }
  return buf;
}

// FUNCTION: LEGOBATMAN 0x006d69c0
unsigned char ImplodeGetByteFromMem(void) { return *implode_inbuffer++; }

// FUNCTION: LEGOBATMAN 0x006d69e0
void ImplodePutByteToMem(unsigned char c) { *implode_outbuffer++ = c; }

// GLOBAL: LEGOBATMAN 0x00ae4348
unsigned int implode_bitbuf;
// GLOBAL: LEGOBATMAN 0x00ae4550
static unsigned int subbitbuf;
// GLOBAL: LEGOBATMAN 0x00aec758
static unsigned int bitcount;
// GLOBAL: LEGOBATMAN 0x00af90b0
unsigned int implode_compsize;

// FUNCTION: LEGOBATMAN 0x006d6a00
// from saga nu2api/nucore/implode.cpp
void ImplodeFillBuf(int count) {
  implode_bitbuf <<= count;

  while (count > bitcount) {
    implode_bitbuf |= subbitbuf << (count -= bitcount);

    if (implode_compsize != 0) {
      implode_compsize--;

      subbitbuf = ImplodeGetByteFromMem() & 0xff;
    } else {
      subbitbuf = 0;
    }

    bitcount = 8;
  }

  implode_bitbuf |= subbitbuf >> (bitcount -= count);
}

// FUNCTION: LEGOBATMAN 0x006d6a80
unsigned int ImplodeGetBits(int count) {
  unsigned int value;

  if (count == 0) {
    return 0;
  }

  value = implode_bitbuf >> (0x20u - count);

  ImplodeFillBuf(count);

  return value;
}

// FUNCTION: LEGOBATMAN 0x006d6af0
void ImplodeInitGetBits(void) {
  implode_bitbuf = 0;
  subbitbuf = 0;
  bitcount = 0;

  ImplodeFillBuf(0x20);
}

// GLOBAL: LEGOBATMAN 0x0099f304
static int gExplodeInitialised = 1;

// FUNCTION: LEGOBATMAN 0x006d6b10
void ExplodeExit(void) {
  if (gExplodeInitialised != 0)
    gExplodeInitialised = 0;
}

// GLOBAL: LEGOBATMAN 0x00af41b0
static unsigned int blocksize;

// FUNCTION: LEGOBATMAN 0x006d6cd0
void ImplodeHufDecodeStart(void) {
  ImplodeInitGetBits();

  blocksize = 0;
}
