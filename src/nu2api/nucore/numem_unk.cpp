// nu2api/nucore/numem_unk.cpp: between nufile_gen.cpp (0x006e0830) and
// nufile_pc.cpp (0x006e3430).

#include "common.h"

void *NuFilePakLoadKeyPrePad(char *filepath, VARIPTR *buf, VARIPTR buf_end,
                             i32 alignment, unsigned char *key, u32 key_len,
                             i32 pre_pad);

// FUNCTION: LEGOBATMAN 0x006e0dc0
void *NuFilePakLoad(char *filepath, VARIPTR *buf, VARIPTR buf_end,
                    i32 alignment) {
  return NuFilePakLoadKeyPrePad(filepath, buf, buf_end, alignment, 0, 0, 0);
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
extern void *NuMem_Heap;
// GLOBAL: LEGOBATMAN 0x00b03954
extern NUMEMDISCARDABLE *discardbuff;
// GLOBAL: LEGOBATMAN 0x00b03944
extern NUMEMEXTERNAL memext;
// GLOBAL: LEGOBATMAN 0x00b0395c
extern NUMEMEXTERNAL *memexternal;
// GLOBAL: LEGOBATMAN 0x00b03968
extern i32 g_nuMemUnk00b03968;

void NuHeapFree(void *heap, void *ptr);
extern "C" void free(void *ptr);

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
