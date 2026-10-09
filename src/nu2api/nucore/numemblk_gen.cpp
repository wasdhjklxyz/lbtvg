// nu2api/nucore/numemblk_gen.cpp: __FILE__ anchor at 0x006d4260
// (NuMemBlkCreate). Fixed-size block pools; bodies after saga
// nu2api/nucore/numem.cpp.

#include "common.h"
#include <string.h>

extern "C" void *NuMemAllocFn(int size, const char *file, int line);
extern "C" void NuMemFreeFn(void *ptr, const char *file, int line);

typedef struct numemblklink_s {
  struct numemblklink_s *next;
} NUMEMBLKLINK;

typedef struct numemblk_s {
  NUMEMBLKLINK *free_list; // 0x00
  u32 stride;              // 0x04
  i32 capacity;            // 0x08
  u16 flags;               // 0x0c, 1 = external storage
  i16 free_count;          // 0x0e
} NUMEMBLK;

// FUNCTION: LEGOBATMAN 0x006d4260
NUMEMBLK *NuMemBlkCreate(i32 element_size, i32 count, u32 alignment_mask) {
  if (element_size < (i32)sizeof(NUMEMBLKLINK))
    element_size = sizeof(NUMEMBLKLINK);
  u32 stride = (element_size + alignment_mask) & ~alignment_mask;
  NUMEMBLK *pool = (NUMEMBLK *)NuMemAllocFn(
      ((sizeof(NUMEMBLK) + alignment_mask) & ~alignment_mask) + stride * count,
      __FILE__, 0x6c);
  NUMEMBLKLINK *link =
      (NUMEMBLKLINK *)(((u32)pool + sizeof(NUMEMBLK) + alignment_mask) &
                       ~alignment_mask);
  pool->stride = stride;
  pool->free_list = link;
  pool->capacity = count;
  pool->free_count = count;
  pool->flags = 0;
  u32 stride_words = stride / sizeof(u32);
  for (i32 i = 0; i < count - 1; i++) {
    link->next = (NUMEMBLKLINK *)((u32 *)link + stride_words);
    link = link->next;
  }
  link->next = NULL;
  return pool;
}

// FUNCTION: LEGOBATMAN 0x006d42e0
NUMEMBLK *NuMemBlkCreateEx(i32 element_size, i32 count, u32 alignment_mask,
                           void *storage) {
  if (element_size < (i32)sizeof(NUMEMBLKLINK))
    element_size = sizeof(NUMEMBLKLINK);
  u32 stride = (element_size + alignment_mask) & ~alignment_mask;
  NUMEMBLK *pool = (NUMEMBLK *)storage;
  u32 data = (u32)pool;
  data += (sizeof(NUMEMBLK) + alignment_mask) & ~alignment_mask;
  pool->stride = stride;
  pool->capacity = count;
  pool->free_count = count;
  pool->flags = 1;
  if (count != 0) {
    pool->free_list = (NUMEMBLKLINK *)data;
    NUMEMBLKLINK *link = pool->free_list;
    u32 stride_words = stride / sizeof(u32);
    for (i32 i = 0; i < count - 1; i++) {
      link->next = (NUMEMBLKLINK *)((u32 *)link + stride_words);
      link = link->next;
    }
    link->next = NULL;
  } else {
    pool->free_list = NULL;
  }
  return pool;
}

// FUNCTION: LEGOBATMAN 0x006d4350
i32 NuMemBlkSize(i32 element_size, i32 count, i32 alignment_mask) {
  i32 header_size = (sizeof(NUMEMBLK) + alignment_mask) & ~alignment_mask;
  i32 stride = (element_size + alignment_mask) & ~alignment_mask;
  return stride * count + header_size;
}

// FUNCTION: LEGOBATMAN 0x006d4370
void NuMemBlkDestroy(NUMEMBLK *pool) {
  if (!(pool->flags & 1))
    NuMemFreeFn(pool, __FILE__, 0xda);
}

// FUNCTION: LEGOBATMAN 0x006d4390
void NuMemBlkCheckFreeList(NUMEMBLK *pool) {
  for (NUMEMBLKLINK *link = pool->free_list; link != NULL; link = link->next)
    ;
}

// FUNCTION: LEGOBATMAN 0x006d43b0
void *NuMemBlkAlloc(NUMEMBLK *pool) {
  NUMEMBLKLINK *block;
  if ((block = pool->free_list) != NULL) {
    pool->free_list = pool->free_list->next;
    pool->free_count--;
    memset(block, -1, pool->stride);
  }
  return block;
}

// FUNCTION: LEGOBATMAN 0x006d43e0
void NuMemBlkFree(NUMEMBLK *pool, void *block) {
  NUMEMBLKLINK *link = (NUMEMBLKLINK *)block;
  pool->free_count++;
  memset(block, -2, pool->stride);
  link->next = pool->free_list;
  pool->free_list = link;
}
