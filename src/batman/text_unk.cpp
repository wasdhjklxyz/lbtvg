// batman/, file unknown (between LoadPerm2 and Text_LoadStrings).

#include "../nu2api/nucore/nustring.h"
#include <string.h>

// FUNCTION: LEGOBATMAN 0x004f9a60
void Text_DecodeButtons(char *in, char *out) {
  if (!NuStrICmp(in, "wibble"))
    NuStrCpy(out, "cross");
}

// GLOBAL: LEGOBATMAN 0x00a957f4
extern char **TTab_Original;
// GLOBAL: LEGOBATMAN 0x00a95810
extern i32 Text_MaxStrings_Overall;
// GLOBAL: LEGOBATMAN 0x00a957f8
extern char **TTab;
// GLOBAL: LEGOBATMAN 0x00a95814
extern u32 *Text_StringBits;
// GLOBAL: LEGOBATMAN 0x0095eb64
extern char *Text_ErrString;

// STUB: LEGOBATMAN 0x0059d7c0
// close: register allocation only; orig aligns into eax and copies to esi
// for the table pointer, this aligns straight into esi.
void Text_InitStringTable(i32 count, variptr_u *buf, variptr_u *) {
  u32 addr = (buf->addr + 3) & ~3;
  u32 table_size = (count + 1) * sizeof(char *);
  char **tab = (char **)addr;
  buf->addr = (addr + table_size + 3) & ~3;
  TTab_Original = tab;
  memset(tab, 0, table_size);
  tab[0] = Text_ErrString;
  TTab = tab + 1;

  u32 *bits = buf->u32_ptr;
  i32 flags_size = (count + 31) / 32 * (i32)sizeof(u32);
  Text_MaxStrings_Overall = count;
  Text_StringBits = bits;
  memset(bits, 0, flags_size);
  buf->addr += flags_size;
}
