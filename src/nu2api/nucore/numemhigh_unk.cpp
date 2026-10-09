// nu2api/nucore/numemhigh.cpp (saga): 0x006d4410..0x006d4510, after
// numemblk_gen.cpp. Mac order: NuMemBlkFree, NuAllocHighInit, NuAllocHigh,
// NuFreeHigh.

#include "common.h"

struct NUMEMHIGHBLOCK {
  NUMEMHIGHBLOCK *next;
  u32 units;
  u8 reserved[8];
};

// GLOBAL: LEGOBATMAN 0x00b038c4
static NUMEMHIGHBLOCK *freep;

// FUNCTION: LEGOBATMAN 0x006d4410
void NuAllocHighInit(u32 buffer, u32 size) {
  // The masks are the original's (it never rounded correctly).
  if (buffer & 15) {
    buffer = (buffer - 1) & 15;
    size -= 15;
  }
  if (size & 15) {
    size = (size + 1) & 15;
  }
  freep = (NUMEMHIGHBLOCK *)buffer;
  freep->next = freep;
  freep->units = size;
}

// Original keeps freep in esi and previous in edx, reuses the >= flags for
// the == test and duplicates the return tail; neither saga nor K&R order
// reproduces it.
// STUB: LEGOBATMAN 0x006d4440
void *NuAllocHigh(u32 size) {
  NUMEMHIGHBLOCK *block;
  NUMEMHIGHBLOCK *previous;
  u32 units;

  units = (size + sizeof(NUMEMHIGHBLOCK) - 1) / sizeof(NUMEMHIGHBLOCK) + 1;
  previous = freep;
  block = previous->next;
  while (block->units < units) {
    if (block == freep)
      return 0;
    previous = block;
    block = block->next;
  }
  if (block->units == units) {
    previous->next = block->next;
  } else {
    block->units -= units;
    block += block->units;
    block->units = units;
  }
  freep = previous;
  return block + 1;
}

// FUNCTION: LEGOBATMAN 0x006d44a0
void NuFreeHigh(void *ptr) {
  NUMEMHIGHBLOCK *previous;
  NUMEMHIGHBLOCK *block = (NUMEMHIGHBLOCK *)ptr - 1;

  previous = freep;
  while (!(block > previous && block < previous->next)) {
    if (previous >= previous->next &&
        (block > previous || block < previous->next))
      break;
    previous = previous->next;
  }
  if (block + block->units == previous->next) {
    block->units += previous->next->units;
    block->next = previous->next->next;
  } else {
    block->next = previous->next;
  }
  if (previous + previous->units == block) {
    previous->units += block->units;
    previous->next = block->next;
  } else {
    previous->next = block;
  }
  freep = previous;
}
