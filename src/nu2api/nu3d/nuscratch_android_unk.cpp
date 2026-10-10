// nu2api/nu3d/nuscratch_android_unk.cpp: placed by tools/new.py; file name
// unproven.

#include "../nucore/common.h"
#include "../numath/nuinline_unk.h"
#include <stddef.h>

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x00528690
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

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

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_nuscratch_android_unk(f32 *v, f32 a, i32 i) {
  NuVec4Set(v, a, a, a, a);
}
