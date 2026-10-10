// nu2api/gamelib/crc_unk.c: CRC-32 (MSB first) over the table built by
// CRC_Init; after ref/saga/src/gamelib/crc/crc.cpp. File name unproven; it
// sits between the vorbis lsp.c and the sfx code, as on the Mac. C: the
// Mac symbols are unmangled. Probably one TU with its neighbours: the Mac
// unity has NuError/NuWarning/NuDebugMsg, NuSinApprox, CRC_*, then SfxBit*,
// and the PC NuSinApprox copy (0x557ef0) and the register-arg static matrix
// helpers right before CRC_Init are called from as far as 0x57d660.

#include "../nucore/common.h"

#define CRC32_POLY 0x04c11db7

// GLOBAL: LEGOBATMAN 0x00a28b5c
i32 g_crc_initialised;
// GLOBAL: LEGOBATMAN 0x00a0f9c0
u32 *g_crc_table;

// FUNCTION: LEGOBATMAN 0x00558290
void CRC_Init(VARIPTR *buffer_start) {
  u32 i;
  u32 crc;
  u32 top;
  i32 j;

  if (g_crc_initialised != 0)
    return;
  g_crc_table = (u32 *)((buffer_start->addr + 3) & ~3);
  buffer_start->addr = ((buffer_start->addr + 3) & ~3) + 0x400;
  for (i = 0; i < 0x100; i++) {
    crc = i << 24;
    for (j = 0; j < 8; j++) {
      top = crc >> 31;
      crc <<= 1;
      if (top != 0)
        crc ^= CRC32_POLY;
    }
    g_crc_table[i] = crc;
  }
  g_crc_initialised = 1;
}

// FUNCTION: LEGOBATMAN 0x00558370
u32 CRC_Process(const void *data, u32 size) {
  u32 crc = 0;
  u32 i;

  for (i = 0; i < size; i++)
    crc = (crc << 8) ^ g_crc_table[((const u8 *)data)[i] ^ (crc >> 24)];
  return crc;
}

// STUB: LEGOBATMAN 0x005583b0
// register allocation: orig keeps str in esi (pushed at entry), ours in edx;
// no source form or /O flag changes it (same for the three below).
u32 CRC_ProcessString(const char *str) {
  u32 crc = 0;
  char c;

  for (c = *str; c != '\0'; c = *++str)
    crc = (crc << 8) ^ g_crc_table[(crc >> 24) ^ c];
  return crc;
}

// STUB: LEGOBATMAN 0x005583f0
// register allocation, see CRC_ProcessString.
u32 CRC_ProcessStringN(const char *str, u32 size) {
  u32 crc = 0;
  u32 i;

  for (i = 0; str[i] != '\0'; i++) {
    if (i >= size)
      break;
    crc = (crc << 8) ^ g_crc_table[str[i] ^ (crc >> 24)];
  }
  return crc;
}

// STUB: LEGOBATMAN 0x00558440
// register allocation, see CRC_ProcessString.
u32 CRC_ProcessStringIgnoreCase(const char *str) {
  u32 crc = 0;
  char c;

  while (*str != '\0') {
    c = *str++;
    if ((u8)(c - 'a') <= 'z' - 'a')
      c -= 'a' - 'A';
    crc = (crc << 8) ^ g_crc_table[c ^ (crc >> 24)];
  }
  return crc;
}

// STUB: LEGOBATMAN 0x00558480
// register allocation, see CRC_ProcessString.
u32 CRC_ProcessStringNIgnoreCase(const char *str, u32 size) {
  u32 crc = 0;
  u32 i;
  char c;

  for (i = 0; str[i] != '\0'; i++) {
    if (i >= size)
      break;
    c = str[i];
    if ((u8)(c - 'a') <= 'z' - 'a')
      c -= 'a' - 'A';
    crc = (crc << 8) ^ g_crc_table[c ^ (crc >> 24)];
  }
  return crc;
}
