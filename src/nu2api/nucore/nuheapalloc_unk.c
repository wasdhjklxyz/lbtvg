// nu2api/nucore/nuheapalloc_unk.c: the NuHeapAlloc wrappers of saga
// nu2api/nucore/nuheap.cpp, a TU of their own here (between nupad_gen.cpp
// copies); file name unproven.

#include "common.h"
#include <stddef.h>

void *NuHeapAllocAlignedNamed(void *heap, u32 size, u32 alignment, char *name);

// FUNCTION: LEGOBATMAN 0x006d8e70
void *NuHeapAlloc(void *heap, u32 size) {
  return NuHeapAllocAlignedNamed(heap, size, 4, NULL);
}

// FUNCTION: LEGOBATMAN 0x006d8e90
void *NuHeapAllocNamed(void *heap, u32 size, char *name) {
  return NuHeapAllocAlignedNamed(heap, size, 4, name);
}

// FUNCTION: LEGOBATMAN 0x006d8eb0
void *NuHeapAllocAligned(void *heap, u32 size, u32 alignment) {
  return NuHeapAllocAlignedNamed(heap, size, alignment, NULL);
}
