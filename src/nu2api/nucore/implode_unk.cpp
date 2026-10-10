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

// FUNCTION: LEGOBATMAN 0x006d6b30
// from saga nu2api/nucore/implode.cpp
int ExplodeBufferSize(char *buf) {
  char magic[] = "LZ2K";

  for (int i = 0; i < 4; i++) {
    int does_not_match;
    if ((does_not_match = *(buf++) != magic[i])) {
      return 0;
    }
  }

  int size = ImplodeGetI(buf, sizeof(int));
  return size;
}

// FUNCTION: LEGOBATMAN 0x006d6bb0
// from saga nu2api/nucore/implode.cpp
int ExplodeCompressedSize(char *buf) {
  char magic[] = "LZ2K";

  for (int i = 0; i < 4; i++) {
    int does_not_match;
    if ((does_not_match = *(buf++) != magic[i])) {
      return 0;
    }
  }

  buf += 4;
  int size = ImplodeGetI(buf, sizeof(int));
  if (size != 0) {
    size += 0xc;
  }

  return size;
}

// GLOBAL: LEGOBATMAN 0x00ae4350
static unsigned short offset_table[0x100];
// GLOBAL: LEGOBATMAN 0x00b00860
static unsigned char offset_len[0x13];
// GLOBAL: LEGOBATMAN 0x00af90b8
unsigned short implode_left[0x3fb];
// GLOBAL: LEGOBATMAN 0x00af98b0
unsigned short implode_right[0x3fb];

// FUNCTION: LEGOBATMAN 0x006d6c30
// from saga nu2api/nucore/implode.cpp
unsigned int ImplodeDecodeOffset(void) {
  unsigned int offset;

  offset = offset_table[implode_bitbuf >> 0x18];
  if (offset >= 0xe) {
    unsigned int mask = 0x800000;

    do {
      if ((implode_bitbuf & mask) != 0) {
        offset = implode_right[offset];
      } else {
        offset = implode_left[offset];
      }
      mask >>= 1;
    } while (offset >= 0xe);
  }
  ImplodeFillBuf(offset_len[offset]);

  if (offset != 0) {
    offset = (1u << (offset - 1)) + ImplodeGetBits(offset - 1);
  }

  return offset;
}

// GLOBAL: LEGOBATMAN 0x00af41b0
static unsigned int blocksize;

// FUNCTION: LEGOBATMAN 0x006d6cd0
void ImplodeHufDecodeStart(void) {
  ImplodeInitGetBits();

  blocksize = 0;
}

// FUNCTION: LEGOBATMAN 0x006d6d00
// from saga nu2api/nucore/implode.cpp
void ImplodeMakeTable(int size, unsigned char *len, int unknown,
                      unsigned short *table) {
  unsigned short symbol_count[17];
  unsigned short masks[17];
  unsigned short offsets[18];
  unsigned int symbol;
  int shift;
  unsigned int length;
  unsigned int i;
  unsigned int j;
  unsigned int k;
  unsigned int unknown2;
  unsigned int unknown3;
  unsigned int unknown4;
  unsigned short *cursor;

  for (i = 1; i <= 0x10; i++) {
    symbol_count[i] = 0;
  }

  for (i = 0; i < size; i++) {
    symbol_count[len[i]]++;
  }

  offsets[1] = 0;

  for (i = 1; i <= 0x10; i++) {
    offsets[i + 1] = offsets[i] + (symbol_count[i] << (0x10 - i));
  }

  if (offsets[0x11] != 0) {
    ImplodeError("Bad table");
  }

  shift = 0x10 - unknown;

  for (i = 1; i <= unknown; i++) {
    offsets[i] >>= shift;
    masks[i] = 1 << (unknown - i);
  }

  for (; i <= 0x10; i++) {
    masks[i] = 1 << (0x10 - i);
  }

  i = (int)offsets[unknown + 1] >> shift;
  if (i != 0) {
    for (j = 1 << unknown; i != j; i++) {
      table[i] = 0;
    }
  }

  unknown2 = size;
  unknown3 = 1 << (0xf - unknown);

  for (symbol = 0; symbol < size; symbol++) {
    if ((length = len[symbol]) == 0) {
      continue;
    }

    unknown4 = offsets[length] + masks[length];
    if (length <= unknown) {
      for (i = offsets[length]; i < unknown4; i++) {
        table[i] = symbol;
      }
    } else {
      j = offsets[length];
      cursor = table + (j >> shift);

      for (k = length - unknown; k != 0; k--) {
        if (*cursor == 0) {
          implode_left[unknown2] = 0;
          implode_right[unknown2] = implode_left[unknown2];
          *cursor = unknown2;

          unknown2++;
        }

        if ((j & unknown3) != 0) {
          cursor = implode_right + *cursor;
        } else {
          cursor = implode_left + *cursor;
        }

        j <<= 1;
      }

      *cursor = symbol;
    }

    offsets[length] = unknown4;
  }
}

// The encoder's Huffman tree builder: ar002's maketree.c (Okumura), with the
// make_len overflow message compiled out.

// GLOBAL: LEGOBATMAN 0x00af3188
static int n;
// GLOBAL: LEGOBATMAN 0x00ae4554
static int heapsize;
// GLOBAL: LEGOBATMAN 0x00aec760
static short heap[0x1fe + 1];
// GLOBAL: LEGOBATMAN 0x00af41bc
static unsigned short *freq;
// GLOBAL: LEGOBATMAN 0x00aec75c
static unsigned short *sortptr;
// GLOBAL: LEGOBATMAN 0x00af6504
static unsigned short len_cnt[17];
// GLOBAL: LEGOBATMAN 0x00af41b8
static unsigned char *len;

// FUNCTION: LEGOBATMAN 0x006d6f90
static void count_len(int i) {
  // GLOBAL: LEGOBATMAN 0x00b03930
  static int depth = 0;

  if (i < n)
    len_cnt[(depth < 16) ? depth : 16]++;
  else {
    depth++;
    count_len(implode_left[i]);
    count_len(implode_right[i]);
    depth--;
  }
}

// FUNCTION: LEGOBATMAN 0x006d6ff0
static void make_len(int root) {
  int i, k;
  unsigned int cum;

  for (i = 0; i <= 16; i++)
    len_cnt[i] = 0;
  count_len(root);
  cum = 0;
  for (i = 16; i > 0; i--)
    cum += len_cnt[i] << (16 - i);
  while (cum != (1U << 16)) {
    len_cnt[16]--;
    for (i = 15; i > 0; i--) {
      if (len_cnt[i] != 0) {
        len_cnt[i]--;
        len_cnt[i + 1] += 2;
        break;
      }
    }
    cum--;
  }
  for (i = 16; i > 0; i--) {
    k = len_cnt[i];
    while (--k >= 0)
      len[*sortptr++] = i;
  }
}

// FUNCTION: LEGOBATMAN 0x006d7130
static void downheap(int i) {
  int j, k;

  k = heap[i];
  while ((j = 2 * i) <= heapsize) {
    if (j < heapsize && freq[heap[j]] > freq[heap[j + 1]])
      j++;
    if (freq[k] <= freq[heap[j]])
      break;
    heap[i] = heap[j];
    i = j;
  }
  heap[i] = k;
}

// STUB: LEGOBATMAN 0x006d71b0
// Open arg puzzle: our cl passes n in ebx, the original on the stack.
static void make_code(int n, unsigned char len[], unsigned short code[]) {
  int i;
  unsigned short start[18];

  start[1] = 0;
  for (i = 1; i <= 16; i++)
    start[i + 1] = (start[i] + len_cnt[i]) << 1;
  for (i = 0; i < n; i++)
    code[i] = start[len[i]]++;
}

// STUB: LEGOBATMAN 0x006d7270
// Registers follow make_code's ebx-vs-stack n (see above); body matches.
int ImplodeMakeTree(int nparm, unsigned short freqparm[],
                    unsigned char lenparm[], unsigned short codeparm[]) {
  int i, j, k, avail;

  n = nparm;
  freq = freqparm;
  len = lenparm;
  avail = n;
  heapsize = 0;
  heap[1] = 0;
  for (i = 0; i < n; i++) {
    len[i] = 0;
    if (freq[i])
      heap[++heapsize] = i;
  }
  if (heapsize < 2) {
    codeparm[heap[1]] = 0;
    return heap[1];
  }
  for (i = heapsize / 2; i >= 1; i--)
    downheap(i);
  sortptr = codeparm;
  do {
    i = heap[1];
    if (i < n)
      *sortptr++ = i;
    heap[1] = heap[heapsize--];
    downheap(1);
    j = heap[1];
    if (j < n)
      *sortptr++ = j;
    k = avail++;
    freq[k] = freq[i] + freq[j];
    heap[1] = k;
    downheap(1);
    implode_left[k] = i;
    implode_right[k] = j;
  } while (heapsize > 1);
  sortptr = codeparm;
  make_len(k);
  make_code(nparm, lenparm, codeparm);
  return k;
}
