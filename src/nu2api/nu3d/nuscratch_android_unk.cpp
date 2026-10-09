// nu2api/nu3d/nuscratch_android_unk.cpp: placed by tools/new.py; file name
// unproven.

#include "../nucore/common.h"
#include <stddef.h>

// GLOBAL: LEGOBATMAN 0x009d17b0
u8 PS2_SCRATCH_BASE[0x8000];

// GLOBAL: LEGOBATMAN 0x0094d4a0
static u8 *ps2_scratch_free;

// FUNCTION: LEGOBATMAN 0x005288a0
void NuScratchReset(void) { ps2_scratch_free = PS2_SCRATCH_BASE; }

// FUNCTION: LEGOBATMAN 0x005288b0
void *NuScratchAlloc32(i32 size) {
  u8 *previous = ps2_scratch_free;
  u8 *allocation = (u8 *)(((u32)ps2_scratch_free + 3) & ~3);
  ps2_scratch_free = allocation + ((size + 3) & ~3);
  *(u8 **)ps2_scratch_free = previous;
  ps2_scratch_free += sizeof(previous);
  return allocation;
}

// FUNCTION: LEGOBATMAN 0x005288e0
void *NuScratchAlloc64(i32 size) {
  u8 *previous = ps2_scratch_free;
  u8 *allocation = (u8 *)(((u32)ps2_scratch_free + 7) & ~7);
  ps2_scratch_free = allocation + ((size + 3) & ~3);
  *(u8 **)ps2_scratch_free = previous;
  ps2_scratch_free += sizeof(previous);
  return allocation;
}

// FUNCTION: LEGOBATMAN 0x00528910
void *NuScratchAlloc128(i32 size) {
  u8 *previous = ps2_scratch_free;
  u8 *allocation = (u8 *)(((u32)ps2_scratch_free + 15) & ~15);
  ps2_scratch_free = allocation + ((size + 3) & ~3);
  *(u8 **)ps2_scratch_free = previous;
  ps2_scratch_free += sizeof(previous);
  return allocation;
}

// FUNCTION: LEGOBATMAN 0x00528940
void NuScratchRelease(void) {
  ps2_scratch_free = *(u8 **)((u32)ps2_scratch_free - sizeof(ps2_scratch_free));
}
