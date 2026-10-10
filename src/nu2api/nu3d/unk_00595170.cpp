// nu2api/, file unknown: NuFmvBuffer (RTTI .?AVNuFmvBuffer@@, vtable
// 0x8686f4: deleting dtor, Unlock, Alloc, Free) and NuFmvStream::ReOpen
// (vtable 0x868708 slot 1), 0x595130..0x5951d8.

#include "../nucore/common.h"
#include "../numath/nuinline_unk.h"

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x00595080
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

void NuHeapDestroy(void *heap);
void *NuHeapAllocAligned(void *heap, i32 size, i32 align);
void NuHeapFree(void *heap, void *ptr);

class NuFmvBuffer {
public:
  virtual ~NuFmvBuffer();
  virtual void Unlock();
  virtual void *Alloc(i32 size, i32 align);
  virtual void Free(void *ptr);

  u8 pad04[0x10 - 4];
  void *heap; // 0x10
  i32 count;  // 0x14
};

class NuFmvStream {
public:
  virtual void Vfn0() = 0;
  virtual bool ReOpen(char const *name);
};

// FUNCTION: LEGOBATMAN 0x00595170
void NuFmvBuffer::Unlock() { NuHeapDestroy(heap); }

// FUNCTION: LEGOBATMAN 0x00595180
void *NuFmvBuffer::Alloc(i32 size, i32 align) {
  void *ptr = NuHeapAllocAligned(heap, size, align);
  count++;
  return ptr;
}

// FUNCTION: LEGOBATMAN 0x005951b0
void NuFmvBuffer::Free(void *ptr) {
  count--;
  NuHeapFree(heap, ptr);
}

// FUNCTION: LEGOBATMAN 0x005951d0
bool NuFmvStream::ReOpen(char const *name) { return false; }

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_unk_00595170(f32 *v, f32 a, i32 i) {
  NuVec4Set(v, a, a, a, a);
}
