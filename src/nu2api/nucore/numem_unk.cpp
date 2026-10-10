// nu2api/nucore/numem_unk.cpp: between nufile_gen.cpp (0x006e0830) and
// nufile_pc.cpp (0x006e3430).

#include "../numath/nuinline_unk.h"
#include "common.h"
#include <string.h>

// Empty debug hooks (allocation failure / bad size reports).
// FUNCTION: LEGOBATMAN 0x006e1b90
static void NuMemDebugUnk006e1b90(...) {}

static void NuMemDebugUnk006e1ba0(...) {}

// FUNCTION: LEGOBATMAN 0x006e1bb0
static void NuMemDebugUnk006e1bb0(...) {}

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x006e1bc0
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

void *NuFilePakLoadKeyPrePad(char *filepath, VARIPTR *buf, VARIPTR buf_end,
                             i32 alignment, unsigned char *key, u32 key_len,
                             i32 pre_pad);

// FUNCTION: LEGOBATMAN 0x006e0dc0
void *NuFilePakLoad(char *filepath, VARIPTR *buf, VARIPTR buf_end,
                    i32 alignment) {
  return NuFilePakLoadKeyPrePad(filepath, buf, buf_end, alignment, 0, 0, 0);
}

// FUNCTION: LEGOBATMAN 0x006e0d90
void *NuFilePakLoadKey(char *filepath, VARIPTR *buf, VARIPTR buf_end,
                       i32 alignment, unsigned char *key, u32 key_len) {
  return NuFilePakLoadKeyPrePad(filepath, buf, buf_end, alignment, key, key_len,
                                0);
}

// FUNCTION: LEGOBATMAN 0x006e0df0
void *NuFilePakLoadPrePad(char *filepath, VARIPTR *buf, VARIPTR buf_end,
                          i32 alignment, i32 pre_pad) {
  return NuFilePakLoadKeyPrePad(filepath, buf, buf_end, alignment, 0, 0,
                                pre_pad);
}

// Loop shape right; original strength-reduces src as dst+(src-dst) and
// schedules the count decrement before the store. Not reproduced yet.
// STUB: LEGOBATMAN 0x006e2470
void NuMemCpy(unsigned char *dst, unsigned char *src, int n) {
  if (dst < src) {
    while (n) {
      *dst++ = *src++;
      n--;
    }
  } else {
    src += n;
    dst += n;
    while (n) {
      *--dst = *--src;
      n--;
    }
  }
}

// numem.cpp (saga) small entry points.

struct NUMEMDISCARDABLE {
  i32 capacity;  // 0x00
  i32 remaining; // 0x04
  u8 *cursor;    // 0x08
  u32 pad0c;
};

struct NUMEMEXTERNAL {
  variptr_u *cursor; // 0x00
  variptr_u end;     // 0x04
};

// GLOBAL: LEGOBATMAN 0x00b03960
void *NuMem_Heap;
// GLOBAL: LEGOBATMAN 0x00b03954
static NUMEMDISCARDABLE *discardbuff;
// GLOBAL: LEGOBATMAN 0x00b03944
static NUMEMEXTERNAL memext;
// GLOBAL: LEGOBATMAN 0x00b0395c
static NUMEMEXTERNAL *memexternal;
// GLOBAL: LEGOBATMAN 0x00b03968
static i32 g_nuMemUnk00b03968;

void NuHeapFree(void *heap, void *ptr);
extern "C" void free(void *ptr);

// --- NuMemAllocFn (matohero numem.c shape plus LB debug hooks) --------------

struct MEMBLK_HEAD {
  MEMBLK_HEAD *succ; // 0x00
  MEMBLK_HEAD *prev; // 0x04
  i32 line;          // 0x08
  i32 cnt;           // 0x0c
  i32 size;          // 0x10
  char mark[12];     // 0x14
};

struct MEMBLK_TAIL {
  char mark[12];
};

// GLOBAL: LEGOBATMAN 0x0099f3e0
static char blk_hdr[13] = "MEMBLKHEADER";
// GLOBAL: LEGOBATMAN 0x0099f3f0
static char blk_end[13] = "MEMBLKTAIL01";
// GLOBAL: LEGOBATMAN 0x00b0394c
static MEMBLK_HEAD *alloc_list;
// GLOBAL: LEGOBATMAN 0x00b03950
static i32 totalloc;
// GLOBAL: LEGOBATMAN 0x00b03958
static i32 alloccnt;
// GLOBAL: LEGOBATMAN 0x00b03964
i32 highallocaddr;

extern "C" void *NuHeapAllocAligned(void *heap, u32 size, u32 alignment);
extern "C" void *malloc(unsigned int size);

// FUNCTION: LEGOBATMAN 0x006e1d50
extern "C" void *NuMemAllocFn(int size, const char *file, int line) {
  char *rv;
  int endaddr;
  void *ptr;
  MEMBLK_TAIL *tail;

  if (size <= 0)
    NuMemDebugUnk006e1b90(size, file, line);

  if (memexternal != 0) {
    memexternal->cursor->addr = (memexternal->cursor->addr + 0xf) & ~0xf;
    if (memexternal->end.addr - memexternal->cursor->addr >= (u32)size) {
      ptr = memexternal->cursor->void_ptr;
      memexternal->cursor->addr += size;
      return ptr;
    }
    NuMemDebugUnk006e1b90(size, file, line);
    return 0;
  }

  if (discardbuff != 0) {
    size = (size + 0xf) & ~0xf;
    if (size < discardbuff->remaining) {
      ptr = discardbuff->cursor;
      discardbuff->cursor += size;
      discardbuff->remaining -= size;
      return ptr;
    }
    return 0;
  }

  if (NuMem_Heap != 0) {
    rv = (char *)NuHeapAllocAligned(NuMem_Heap, size + strlen(file) + 0x2d, 4);
  } else {
    rv = (char *)malloc(size + strlen(file) + 0x2d);
    if (rv != 0)
      memset(rv, 0, size + strlen(file) + 0x2d);
  }

  if (rv == 0) {
    NuMemDebugUnk006e1bb0(size, file, line);
    NuMemDebugUnk006e1b90(size, file, line);
  } else {
    memset(rv, 0, size + strlen(file) + 0x2d);
    totalloc += size;
    ((MEMBLK_HEAD *)rv)->size = size;
    tail = (MEMBLK_TAIL *)(rv + sizeof(MEMBLK_HEAD) + size);
    memcpy(((MEMBLK_HEAD *)rv)->mark, blk_hdr, 12);
    memcpy(tail->mark, blk_end, 12);
    strcpy((char *)(tail + 1), file);
    ((MEMBLK_HEAD *)rv)->cnt = alloccnt++;
    ((MEMBLK_HEAD *)rv)->line = line;
    ((MEMBLK_HEAD *)rv)->succ = alloc_list;
    if (alloc_list != 0)
      alloc_list->prev = (MEMBLK_HEAD *)rv;
    ((MEMBLK_HEAD *)rv)->prev = 0;
    alloc_list = (MEMBLK_HEAD *)rv;
    rv += sizeof(MEMBLK_HEAD);
  }

  endaddr = (int)rv + size;
  if (endaddr > highallocaddr)
    highallocaddr = endaddr;
  if (endaddr > g_nuMemUnk00b03968)
    g_nuMemUnk00b03968 = endaddr;
  return rv;
}

// FUNCTION: LEGOBATMAN 0x006e1c70
extern "C" void *NuMemAlloc(int size) {
  void *ptr;
  int endaddr;

  if (memexternal != 0) {
    memexternal->cursor->addr = (memexternal->cursor->addr + 0xf) & ~0xf;
    if (memexternal->end.addr - memexternal->cursor->addr >= (u32)size) {
      ptr = memexternal->cursor->void_ptr;
      memexternal->cursor->addr += size;
      return ptr;
    }
    return 0;
  }

  if (discardbuff != 0) {
    size = (size + 0xf) & ~0xf;
    if (size < discardbuff->remaining) {
      ptr = discardbuff->cursor;
      discardbuff->cursor += size;
      discardbuff->remaining -= size;
      return ptr;
    }
    return 0;
  }

  totalloc += size;
  if (NuMem_Heap != 0)
    ptr = NuHeapAllocAligned(NuMem_Heap, size, 4);
  else
    ptr = malloc(size);
  memset(ptr, 0, size);
  endaddr = (int)ptr + size;
  if (endaddr > highallocaddr)
    highallocaddr = endaddr;
  if (endaddr > g_nuMemUnk00b03968)
    g_nuMemUnk00b03968 = endaddr;
  return ptr;
}

extern "C" void *realloc(void *ptr, unsigned int size);

// FUNCTION: LEGOBATMAN 0x006e1f30
extern "C" void *NuMemReAllocFn(void *addr, int size, const char *file,
                                int line) {
  char *rv;
  int endaddr;
  void *ptr;
  MEMBLK_HEAD *head;
  MEMBLK_TAIL *tail;
  MEMBLK_HEAD *list;

  if (memexternal != 0) {
    NuMemDebugUnk006e1b90();
    memexternal->cursor->addr = (memexternal->cursor->addr + 0xf) & ~0xf;
    if (memexternal->end.addr - memexternal->cursor->addr >= (u32)size) {
      ptr = memexternal->cursor->void_ptr;
      memexternal->cursor->addr += size;
      return ptr;
    }
    return 0;
  }

  if (discardbuff != 0) {
    size = (size + 0xf) & ~0xf;
    if (size < discardbuff->remaining) {
      ptr = discardbuff->cursor;
      discardbuff->cursor += size;
      discardbuff->remaining -= size;
      return ptr;
    }
    return 0;
  }

  head = (MEMBLK_HEAD *)addr - 1;
  if (head->succ != 0)
    head->succ->prev = head->prev;
  if (head->prev != 0) {
    head->prev->succ = head->succ;
    list = alloc_list;
  } else {
    list = head->succ;
    alloc_list = list;
  }

  rv = (char *)realloc(head, size + strlen(file) + 0x2d);
  if (rv == 0) {
    NuMemDebugUnk006e1bb0(size, file, line);
  } else {
    totalloc += size - ((MEMBLK_HEAD *)rv)->size;
    ((MEMBLK_HEAD *)rv)->size = size;
    tail = (MEMBLK_TAIL *)(rv + sizeof(MEMBLK_HEAD) + size);
    memcpy(tail->mark, blk_end, 12);
    strcpy((char *)(tail + 1), file);
    ((MEMBLK_HEAD *)rv)->line = line;
    ((MEMBLK_HEAD *)rv)->succ = list;
    if (list != 0)
      list->prev = (MEMBLK_HEAD *)rv;
    ((MEMBLK_HEAD *)rv)->prev = 0;
    alloc_list = (MEMBLK_HEAD *)rv;
    rv += sizeof(MEMBLK_HEAD);
  }

  endaddr = (int)rv + size;
  if (endaddr > highallocaddr)
    highallocaddr = endaddr;
  if (endaddr > g_nuMemUnk00b03968)
    g_nuMemUnk00b03968 = endaddr;
  return rv;
}

// STUB: LEGOBATMAN 0x006e2090
// The inlined memcmps: the original branches straight to the report on a
// mismatch; ours materialises memcmp's -1/0/1 first (484 B).
extern "C" void NuMemFreeFn(void *addr, const char *file, int line) {
  MEMBLK_HEAD *head;
  MEMBLK_TAIL *tail;

  if (memexternal != 0)
    NuMemDebugUnk006e1b90();
  if (addr == 0) {
    NuMemDebugUnk006e1bb0(file, line);
    return;
  }
  head = (MEMBLK_HEAD *)addr - 1;
  tail = (MEMBLK_TAIL *)((char *)addr + head->size);
  if (memcmp(tail->mark, blk_end, 12) != 0)
    NuMemDebugUnk006e1bb0(file, line, tail);
  if (memcmp(head->mark, blk_hdr, 12) != 0)
    NuMemDebugUnk006e1bb0(file, line, head->mark);
  if (head->succ == 0 && head->prev == 0)
    NuMemDebugUnk006e1bb0(file, line);
  if (head->succ != 0)
    head->succ->prev = head->prev;
  if (head->prev != 0)
    head->prev->succ = head->succ;
  else
    alloc_list = head->succ;
  totalloc -= head->size;
  head->succ = 0;
  head->prev = 0;
  if (NuMem_Heap != 0)
    NuHeapFree(NuMem_Heap, head);
  else
    free(head);
}

// STUB: LEGOBATMAN 0x006e23d0
// Loop tail (corruption checks) and the push scratch register differ.
extern "C" void NuMemDumpFn(int min_size) {
  MEMBLK_HEAD *p;
  int size;

  for (p = alloc_list; p != 0;) {
    size = p->size;
    if (size >= min_size)
      NuMemDebugUnk006e1bb0((char *)p + size + 0x2c, p->line, p->cnt, size);
    p = p->succ;
    if (p == (MEMBLK_HEAD *)0xfefefefe)
      NuMemDebugUnk006e1b90();
    if (p == 0)
      break;
    if (p->succ == (MEMBLK_HEAD *)0xfefefefe)
      NuMemDebugUnk006e1b90();
  }
  NuMemDebugUnk006e1bb0(totalloc);
}

// FUNCTION: LEGOBATMAN 0x006e2440
extern "C" void NuMemFlushFn(void) {
  MEMBLK_HEAD *p = alloc_list;
  MEMBLK_HEAD *next;

  while (p != 0) {
    next = p->succ;
    free(p);
    p = next;
  }
}

// Batman's version returns the previous heap.
// FUNCTION: LEGOBATMAN 0x006e1be0
void *NuMemSetHeap(void *heap) {
  void *previous = NuMem_Heap;
  NuMem_Heap = heap;
  return previous;
}

// FUNCTION: LEGOBATMAN 0x006e1bf0
NUMEMDISCARDABLE *NuMemSetDiscardable(NUMEMDISCARDABLE *buffer) {
  NUMEMDISCARDABLE *previous = discardbuff;
  discardbuff = buffer;
  return previous;
}

// FUNCTION: LEGOBATMAN 0x006e1c00
void NuMemFlushDiscardable(NUMEMDISCARDABLE *buffer) {
  if (buffer != 0) {
    NUMEMDISCARDABLE *pool = buffer;
    pool->cursor = (u8 *)(pool + 1);
    pool->remaining = pool->capacity;
  }
}

// FUNCTION: LEGOBATMAN 0x006e1c20
void NuMemSetExternal(variptr_u *cursor, variptr_u *end) {
  if (cursor != 0) {
    memexternal = &memext;
    memexternal->cursor = cursor;
    if (end != 0)
      memexternal->end = *end;
  } else {
    memexternal = 0;
  }
}

// FUNCTION: LEGOBATMAN 0x006e1c60
i32 NuMemGetUnk00b03968(void) { return g_nuMemUnk00b03968; }

// FUNCTION: LEGOBATMAN 0x006e1d30
void NuMemFree(void *ptr) {
  if (NuMem_Heap != 0)
    NuHeapFree(NuMem_Heap, ptr);
  else
    free(ptr);
}

// Mac order after NuMemCpy: numeminit, NuMemGetExternal, NuMemSet128,
// NuMemCheck, NuMemAllocFn, NuMemCreateDiscardable, NuMemCopy128.

extern "C" void *NuMemAllocFn(int size, const char *file, int line);
extern "C" void NuMemFreeFn(void *ptr, const char *file, int line);

// FUNCTION: LEGOBATMAN 0x006e24d0
int numeminit(void) { return 0; }

// FUNCTION: LEGOBATMAN 0x006e24e0
NUMEMEXTERNAL *NuMemGetExternal(void) { return memexternal; }

// FUNCTION: LEGOBATMAN 0x006e24f0
void NuMemSet128(void *dst, u8 value, i32 count) {
  memset(dst, value, count << 4);
}

// FUNCTION: LEGOBATMAN 0x006e2510
void NuMemCopy128(void *dst, void *src, i32 count) {
  NuMemCpy((unsigned char *)dst, (unsigned char *)src, count << 4);
}

// FUNCTION: LEGOBATMAN 0x006e2520
void NuMemCheck(void) {}

// esi/edi swapped (size vs previous) whichever order the locals take.
// STUB: LEGOBATMAN 0x006e2530
NUMEMDISCARDABLE *NuMemCreateDiscardable(i32 size) {
  NUMEMDISCARDABLE *previous;
  NUMEMDISCARDABLE *pool;

  previous = discardbuff;
  discardbuff = 0;
  pool = (NUMEMDISCARDABLE *)NuMemAllocFn(size + sizeof(NUMEMDISCARDABLE),
                                          __FILE__, 0x7b);
  discardbuff = previous;
  if (pool != 0) {
    pool->capacity = size;
    pool->remaining = size;
    pool->cursor = (u8 *)(pool + 1);
  }
  return pool;
}

// FUNCTION: LEGOBATMAN 0x006e2580
void NuMemDestroyDiscardable(NUMEMDISCARDABLE *buffer) {
  NuMemFreeFn(buffer, __FILE__, 0x8a);
}

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_numem_unk(f32 *v, f32 a, i32 i) {
  NuVec4Set(v, a, a, a, a);
}
