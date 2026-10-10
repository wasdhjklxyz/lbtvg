// nu2api/nucore/nuheap_unk.c: saga nu2api/nucore/nuheap.cpp (block helpers
// 0x006d25c0.., NuHeapCreate, NuHeapAllocAlignedNamed, NuHeapFree,
// NuHeapDestroy, the stats getters); the PC heap adds a critical section.

#include "common.h"
#include <stddef.h>

typedef struct NUHEAPBLOCK_s {
  u32 size_and_flags;
  struct NUHEAPBLOCK_s *next_free;
  struct NUHEAPBLOCK_s *previous_free;
} NUHEAPBLOCK;

typedef struct NUHEAP_s {
  void *base;
  u32 size;
  i32 cs; // 0x08, critical section
  u32 allocated_blocks;
  u32 allocated_bytes;
} NUHEAP;

#define NUHEAPBLOCK_IN_USE 0x80000000u
#define NUHEAPBLOCK_SIZE_MASK 0x7fffffffu

i32 NuThreadCreateCriticalSection(void);
void NuThreadDestroyCriticalSection(i32 cs);
void NuThreadCriticalSectionBegin(i32 cs);
void NuThreadCriticalSectionEnd(i32 cs);

// FUNCTION: LEGOBATMAN 0x006d25c0
static void NuHeapBlock_SetInUse(NUHEAPBLOCK *block, u32 in_use) {
  if (in_use)
    block->size_and_flags |= NUHEAPBLOCK_IN_USE;
  else
    block->size_and_flags &= NUHEAPBLOCK_SIZE_MASK;
}

// FUNCTION: LEGOBATMAN 0x006d25e0
static u32 NuHeapBlock_GetInUse(NUHEAPBLOCK *block) {
  return block->size_and_flags >> 31;
}

// FUNCTION: LEGOBATMAN 0x006d25f0
static void NuHeapBlock_SetSize(NUHEAPBLOCK *block, u32 size) {
  block->size_and_flags &= NUHEAPBLOCK_IN_USE;
  block->size_and_flags |= size;
}

// FUNCTION: LEGOBATMAN 0x006d2600
static u32 NuHeapBlock_GetSize(NUHEAPBLOCK *block) {
  return block->size_and_flags & NUHEAPBLOCK_SIZE_MASK;
}

// FUNCTION: LEGOBATMAN 0x006d2610
static void NuHeapBlock_SetNextFree(NUHEAPBLOCK *block, NUHEAPBLOCK *next) {
  block->next_free = next;
}

// FUNCTION: LEGOBATMAN 0x006d2620
static NUHEAPBLOCK *NuHeapBlock_GetNextFree(NUHEAPBLOCK *block) {
  return block->next_free;
}

// FUNCTION: LEGOBATMAN 0x006d2630
static void NuHeapBlock_SetPrevFree(NUHEAPBLOCK *block, NUHEAPBLOCK *previous) {
  block->previous_free = previous;
}

// FUNCTION: LEGOBATMAN 0x006d2640
static void *NuHeapBlock_GetUsableMemory(NUHEAPBLOCK *block) {
  return block + 1;
}

// FUNCTION: LEGOBATMAN 0x006d2650
static void NuHeapBlock_WriteFooter(NUHEAPBLOCK *block) {
  *(NUHEAPBLOCK **)((u8 *)block + sizeof(NUHEAPBLOCK) +
                    NuHeapBlock_GetSize(block)) = block;
}

// FUNCTION: LEGOBATMAN 0x006d2660
static void NuHeapBlock_RemoveFromFreeList(NUHEAPBLOCK *block) {
  if (block->previous_free != NULL)
    block->previous_free->next_free = block->next_free;
  if (block->next_free != NULL)
    block->next_free->previous_free = block->previous_free;
}

// STUB: LEGOBATMAN 0x006d2680
// Convention: the original takes block, size and alignment on the stack and
// allocated/remainder in eax/edx; ours passes block in eax.
static void NuHeapBlock_SplitFreeBlock(NUHEAPBLOCK *block, u32 *size,
                                       NUHEAPBLOCK **allocated,
                                       NUHEAPBLOCK **remainder, u32 alignment) {
  u32 remaining_size;
  u8 *base = (u8 *)block;
  u32 total_size =
      NuHeapBlock_GetSize(block) + sizeof(NUHEAPBLOCK) + sizeof(NUHEAPBLOCK *);
  u32 address;
  u32 aligned_address;

  remaining_size =
      total_size - *size - 2 * (sizeof(NUHEAPBLOCK) + sizeof(NUHEAPBLOCK *));
  *remainder = (NUHEAPBLOCK *)base;
  *allocated = (NUHEAPBLOCK *)(base + remaining_size + sizeof(NUHEAPBLOCK) +
                               sizeof(NUHEAPBLOCK *));
  if (alignment > 0) {
    address = (u32)(*allocated + 1);
    aligned_address = (alignment + address - 1) & ~(alignment - 1);
    *allocated =
        (NUHEAPBLOCK *)((u8 *)*allocated + (aligned_address - address));
    remaining_size += aligned_address - address;
    *size += address - aligned_address;
  }
  NuHeapBlock_SetInUse(*allocated, 1);
  NuHeapBlock_SetSize(*allocated, *size);
  NuHeapBlock_SetNextFree(*allocated, NULL);
  NuHeapBlock_SetPrevFree(*allocated, NULL);
  NuHeapBlock_WriteFooter(*allocated);
  NuHeapBlock_SetInUse(*remainder, 0);
  NuHeapBlock_SetSize(*remainder, remaining_size);
  NuHeapBlock_SetNextFree(*remainder, NULL);
  NuHeapBlock_SetPrevFree(*remainder, NULL);
  NuHeapBlock_WriteFooter(*remainder);
}

// FUNCTION: LEGOBATMAN 0x006d2740
static NUHEAPBLOCK *NuHeapBlock_GetNext(NUHEAPBLOCK *block) {
  u8 *next = (u8 *)block;
  next +=
      NuHeapBlock_GetSize(block) + sizeof(NUHEAPBLOCK) + sizeof(NUHEAPBLOCK *);
  return (NUHEAPBLOCK *)next;
}

// FUNCTION: LEGOBATMAN 0x006d2750
static NUHEAPBLOCK *NuHeapBlock_GetPrev(NUHEAPBLOCK *block) {
  u8 *footer = (u8 *)block;
  footer -= sizeof(NUHEAPBLOCK *);
  return *(NUHEAPBLOCK **)footer;
}

// FUNCTION: LEGOBATMAN 0x006d2760
void *NuHeapCreate(VARIPTR *buffer, void *buffer_end, u32 size) {
  u8 *base;
  NUHEAP *control;
  NUHEAPBLOCK *free_head;
  NUHEAPBLOCK *start;
  NUHEAPBLOCK *first;
  NUHEAPBLOCK *end;

  if (size < sizeof(NUHEAP) + 4 * 0x10)
    return NULL;
  base = (u8 *)buffer->void_ptr;
  buffer->addr += size;
  control = (NUHEAP *)base;
  free_head = (NUHEAPBLOCK *)(base + sizeof(NUHEAP));
  start = (NUHEAPBLOCK *)(base + sizeof(NUHEAP) + 0x10);
  first = (NUHEAPBLOCK *)(base + sizeof(NUHEAP) + 2 * 0x10);
  end = (NUHEAPBLOCK *)(base + size - 0x10);

  NuHeapBlock_SetInUse(free_head, 0);
  NuHeapBlock_SetSize(free_head, 0);
  NuHeapBlock_SetNextFree(free_head, first);
  NuHeapBlock_SetPrevFree(free_head, NULL);
  NuHeapBlock_WriteFooter(free_head);

  NuHeapBlock_SetInUse(start, 1);
  NuHeapBlock_SetSize(start, 0);
  NuHeapBlock_SetNextFree(start, NULL);
  NuHeapBlock_SetPrevFree(start, NULL);
  NuHeapBlock_WriteFooter(start);

  NuHeapBlock_SetInUse(end, 1);
  NuHeapBlock_SetSize(end, 0);
  NuHeapBlock_SetNextFree(end, NULL);
  NuHeapBlock_SetPrevFree(end, NULL);
  NuHeapBlock_WriteFooter(end);

  NuHeapBlock_SetInUse(first, 0);
  NuHeapBlock_SetSize(first, size - sizeof(NUHEAP) - 4 * 0x10);
  NuHeapBlock_SetNextFree(first, NULL);
  NuHeapBlock_SetPrevFree(first, free_head);
  NuHeapBlock_WriteFooter(first);

  control->base = base;
  control->cs = NuThreadCreateCriticalSection();
  control->size = size;
  control->allocated_blocks = 0;
  control->allocated_bytes = 0;
  return control;
}

// FUNCTION: LEGOBATMAN 0x006d2800
void NuHeapDestroy(NUHEAP *heap) { NuThreadDestroyCriticalSection(heap->cs); }

// STUB: LEGOBATMAN 0x006d2810
// Only the call into NuHeapBlock_SplitFreeBlock differs (its convention).
void *NuHeapAllocAlignedNamed(void *heap, u32 size, u32 alignment, char *name) {
  NUHEAPBLOCK *block;
  NUHEAP *control = (NUHEAP *)heap;
  NUHEAPBLOCK *free_head;
  u32 required_size;
  void *result;
  NUHEAPBLOCK *remainder;
  NUHEAPBLOCK *allocated;

  if (size == 0)
    return NULL;
  NuThreadCriticalSectionBegin(control->cs);
  free_head = (NUHEAPBLOCK *)((u8 *)control->base + sizeof(NUHEAP));
  size += alignment;
  required_size = size + sizeof(NUHEAPBLOCK) + sizeof(NUHEAPBLOCK *);
  block = free_head;
  while (1) {
    if (block == NULL) {
      NuThreadCriticalSectionEnd(control->cs);
      return NULL;
    }
    if (NuHeapBlock_GetSize(block) > required_size)
      break;
    block = NuHeapBlock_GetNextFree(block);
  }
  NuHeapBlock_RemoveFromFreeList(block);
  NuHeapBlock_SplitFreeBlock(block, &size, &allocated, &remainder, alignment);
  if (free_head->next_free != NULL)
    NuHeapBlock_SetPrevFree(free_head->next_free, remainder);
  NuHeapBlock_SetNextFree(remainder, free_head->next_free);
  NuHeapBlock_SetPrevFree(remainder, free_head);
  NuHeapBlock_SetNextFree(free_head, remainder);
  control->allocated_blocks++;
  control->allocated_bytes += NuHeapBlock_GetSize(allocated) +
                              sizeof(NUHEAPBLOCK) + sizeof(NUHEAPBLOCK *);
  NuThreadCriticalSectionEnd(control->cs);
  result = NuHeapBlock_GetUsableMemory(allocated);
  return result;
}

// FUNCTION: LEGOBATMAN 0x006d28f0
void NuHeapFree(void *heap, void *ptr) {
  NUHEAP *control = (NUHEAP *)heap;
  NUHEAPBLOCK *free_head;
  u8 *address = (u8 *)ptr;
  NUHEAPBLOCK *block;
  NUHEAPBLOCK *next;
  NUHEAPBLOCK *previous;

  if (ptr == NULL)
    return;
  NuThreadCriticalSectionBegin(control->cs);
  free_head = (NUHEAPBLOCK *)((u8 *)control->base + sizeof(NUHEAP));
  if (address < (u8 *)control->base ||
      address > (u8 *)control->base + control->size) {
    NuThreadCriticalSectionEnd(control->cs);
    return;
  }
  {
    block = (NUHEAPBLOCK *)(address - sizeof(NUHEAPBLOCK));
    NuHeapBlock_SetInUse(block, 0);
    control->allocated_blocks--;
    control->allocated_bytes -= NuHeapBlock_GetSize(block);
    control->allocated_bytes -= sizeof(NUHEAPBLOCK);
    control->allocated_bytes -= sizeof(NUHEAPBLOCK *);
    if (free_head->next_free != NULL)
      NuHeapBlock_SetPrevFree(free_head->next_free, block);
    NuHeapBlock_SetNextFree(block, free_head->next_free);
    NuHeapBlock_SetPrevFree(block, free_head);
    NuHeapBlock_SetNextFree(free_head, block);
    next = NuHeapBlock_GetNext(block);
    if (!NuHeapBlock_GetInUse(next)) {
      NuHeapBlock_RemoveFromFreeList(next);
      NuHeapBlock_SetSize(
          block, NuHeapBlock_GetSize(block) + NuHeapBlock_GetSize(next) +
                     sizeof(NUHEAPBLOCK) + sizeof(NUHEAPBLOCK *));
      NuHeapBlock_WriteFooter(block);
    }
    previous = NuHeapBlock_GetPrev(block);
    if (!NuHeapBlock_GetInUse(previous)) {
      NuHeapBlock_RemoveFromFreeList(block);
      NuHeapBlock_SetSize(
          previous, NuHeapBlock_GetSize(previous) + NuHeapBlock_GetSize(block) +
                        sizeof(NUHEAPBLOCK) + sizeof(NUHEAPBLOCK *));
      NuHeapBlock_WriteFooter(previous);
    }
  }
  NuThreadCriticalSectionEnd(control->cs);
}

// FUNCTION: LEGOBATMAN 0x006d2a00
u32 NuHeapGetAllocatedBlockCount(void *heap) {
  NUHEAP *control = (NUHEAP *)heap;
  return control->allocated_blocks;
}

// FUNCTION: LEGOBATMAN 0x006d2a10
u32 NuHeapGetFreeBlockCount(void *heap) {
  NUHEAPBLOCK *block;
  u32 count;
  NUHEAP *control = (NUHEAP *)heap;
  NuThreadCriticalSectionBegin(control->cs);
  block = (NUHEAPBLOCK *)((u8 *)control->base + sizeof(NUHEAP));
  count = 0;
  while (block != NULL) {
    count++;
    block = NuHeapBlock_GetNextFree(block);
  }
  NuThreadCriticalSectionEnd(control->cs);
  return count;
}

// FUNCTION: LEGOBATMAN 0x006d2a50
u32 NuHeapGetTotalAllocated(void *heap) {
  NUHEAP *control = (NUHEAP *)heap;
  return control->allocated_bytes;
}

// FUNCTION: LEGOBATMAN 0x006d2a60
u32 NuHeapGetTotalFree(void *heap) {
  NUHEAPBLOCK *block;
  u32 size;
  NUHEAP *control = (NUHEAP *)heap;
  NuThreadCriticalSectionBegin(control->cs);
  block = (NUHEAPBLOCK *)((u8 *)control->base + sizeof(NUHEAP));
  size = 0;
  while (block != NULL) {
    size += NuHeapBlock_GetSize(block);
    block = NuHeapBlock_GetNextFree(block);
  }
  NuThreadCriticalSectionEnd(control->cs);
  return size;
}
