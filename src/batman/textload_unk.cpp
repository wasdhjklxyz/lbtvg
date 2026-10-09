// batman/, file unknown: text string loader (0x0059d850..). Kept apart from
// text_unk.cpp: defining Text_LoadAndFixUpStrings next to its caller
// Text_LoadStrings changes the caller's register allocation, so they are
// separate TUs in the original.

#include "../nu2api/nucore/nustring.h"

typedef struct nufpar_s {
  u32 pad0[0x910 / 4];
  char *word_buf; // 0x910
  u32 pad914[(0x978 - 0x914) / 4];
  char is_utf16; // 0x978
  char is_utf8;  // 0x979
} NUFPAR;

NUFPAR *NuFParCreate(char *filename);
i32 NuFParGetLine(NUFPAR *parser);
i32 NuFParGetLineW(NUFPAR *parser);
i32 NuFParGetWord(NUFPAR *parser);
i32 NuFParGetWordW(NUFPAR *parser);
i32 NuFParGetInt(NUFPAR *parser);
void NuFParDestroy(NUFPAR *parser);
int NuStrLenW(const unsigned short *s);
unsigned char *NuPadUtf8Encode(unsigned char *dst, unsigned short c);

// STUB: LEGOBATMAN 0x0059d850
// close: register-arg static (parser in esi, matches); only the final "wii"
// test differs: orig branches to the shared `return 1`, ours if-converts it.
static i32 Text_PlatformSpecificIgnore(NUFPAR *parser) {
  if (NuStrICmp(parser->word_buf, "360") == 0)
    return 1;
  if (NuStrICmp(parser->word_buf, "ps2") == 0)
    return 1;
  if (NuStrICmp(parser->word_buf, "ps3") == 0)
    return 1;
  if (NuStrICmp(parser->word_buf, "psp") == 0)
    return 1;
  if (NuStrICmp(parser->word_buf, "pc") == 0) {
    NuFParGetWord(parser);
    return 0;
  }
  if (NuStrICmp(parser->word_buf, "wii") == 0)
    return 1;
  return 0;
}

// STUB: LEGOBATMAN 0x0059d900
// close: ecx/eax picks for the *buffer stores, NuStrLen result move, and the
// u8 -> u16 -> int double movzx before NuPadUtf8Encode.
void Text_LoadAndFixUpStrings(unsigned char *filename, unsigned char **buffer,
                              char **table, i32 count) {
  unsigned char *out = *buffer;
  NUFPAR *parser = NuFParCreate((char *)filename);
  if (parser != 0) {
    if (parser->is_utf16 != 0) {
      while (NuFParGetLineW(parser) != 0) {
        i32 index = NuFParGetInt(parser);
        if (index <= 0 || index >= count)
          continue;
        NuFParGetWordW(parser);
        unsigned short *wide = (unsigned short *)parser->word_buf;
        i32 length = NuStrLenW(wide);
        if (length <= 0)
          continue;
        table[index] = (char *)out;
        for (; length > 0; length--)
          out = NuPadUtf8Encode(out, *wide++);
        *out++ = 0;
      }
    } else if (parser->is_utf8 != 0) {
      while (NuFParGetLine(parser) != 0) {
        NuFParGetWord(parser);
        if (parser->word_buf[0] == 0)
          continue;
        i32 index = NuAToI(parser->word_buf);
        if (index <= 0 || index >= count)
          continue;
        NuFParGetWord(parser);
        if (Text_PlatformSpecificIgnore(parser))
          continue;
        i32 length = NuStrLen(parser->word_buf);
        table[index] = (char *)out;
        NuStrCpy((char *)out, parser->word_buf);
        out += length + 1;
      }
    } else {
      while (NuFParGetLine(parser) != 0) {
        i32 index = NuFParGetInt(parser);
        if (index <= 0 || index >= count)
          continue;
        NuFParGetWord(parser);
        if (Text_PlatformSpecificIgnore(parser))
          continue;
        char *word = parser->word_buf;
        i32 length = NuStrLen(word);
        if (length <= 0)
          continue;
        table[index] = (char *)out;
        for (; length > 0; length--, word++) {
          u8 character = (u8)*word;
          if (character < 0x80)
            *out++ = character;
          else
            out = NuPadUtf8Encode(out, character);
        }
        *out++ = 0;
      }
    }
    NuFParDestroy(parser);
  }
  *buffer = out;
}
