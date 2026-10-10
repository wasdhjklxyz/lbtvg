// nu2api/nucore/nuheap_unk.c: saga nu2api/nucore/nuheap.cpp (NuHeapDestroy,
// NuHeapAllocAlignedNamed at 0x006d2810, ...); file name unproven.

#include "common.h"

typedef struct NUHEAP_s {
  u32 pad0[2];
  i32 cs; // 0x08, critical section
} NUHEAP;

void NuThreadDestroyCriticalSection(i32 cs);

// FUNCTION: LEGOBATMAN 0x006d2800
void NuHeapDestroy(NUHEAP *heap) { NuThreadDestroyCriticalSection(heap->cs); }
