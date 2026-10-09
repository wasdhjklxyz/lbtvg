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
