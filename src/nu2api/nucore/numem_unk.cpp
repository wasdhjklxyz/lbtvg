// nu2api/nucore/numem_unk.cpp: between nufile_gen.cpp (0x006e0830) and
// nufile_pc.cpp (0x006e3430).

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
