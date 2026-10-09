// gameapi/definedlocators_unk.cpp: DefinedLocators_* (Mac); file name
// unproven.

#include "../nu2api/nucore/common.h"
#include "../nu2api/nucore/nustring.h"

// GLOBAL: LEGOBATMAN 0x00acd768
extern char **DefinedLocatorNames;

// GLOBAL: LEGOBATMAN 0x00acd76c
extern i32 DefinedLocatorCount;

// FUNCTION: LEGOBATMAN 0x00620aa0
i16 DefinedLocators_FindIX(char *name) {
  if (DefinedLocatorNames != 0) {
    for (i32 i = 0; i < DefinedLocatorCount; i++) {
      char *locator = DefinedLocatorNames[i];
      if (locator != 0 && NuStrICmp(locator, name) == 0)
        return i;
    }
  }
  return -1;
}

// STUB: LEGOBATMAN 0x00620b30
// not attempted: orig takes the buffer in eax (static custom convention,
// caller in charconfig); saga body below for reference.
#if 0
typedef struct nufpar_s {
    char *file_buf;
    NUFILE file_handle;
    char file_name[256];

    char line_buf_store[514];
    char word_buf_store[514];
    char *line_buf;
    char *word_buf;
    i32 line_buf_size;
    i32 word_buf_size;

    i32 line_num;
    i32 line_pos;
    i32 char_pos;

    i32 buf_start;
    i32 buf_end;

    union {
        NUFPCOMJMP *jump[8];
        NUFPCOMJMPCTX *jump_ctx[8];
    } command_stack;
    union {
        NUFPCOMJMP *jump[8];
        NUFPCOMJMPCTX *jump_ctx[8];
    } command_stack2;
    i32 command_pos;

    i32 size;
    char is_utf16;
    char is_utf8;
    char *separator_list;
    char *separator_tokens;
} NUFPAR;

typedef int32_t i32;
typedef i32 NUFILE;

NUFPAR *NuFParCreateMem(char *name, char *buffer, i32 bufferSize);

i32 NuFParGetLine(NUFPAR *parser);

i32 Text_StripComments(char *text, char *destination, i32 separators);

i32 NuFParGetWord(NUFPAR *parser);

void NuFParDestroy(NUFPAR *parser);

// from saga legoapi/characters/core/charconfig.cpp
static i32 RedirectTextFile(char *text, char *filename, i32 strip_comments) {
    i32 result = 0;
    NUFPAR *parser = NuFParCreateMem("redirect", text, 0xffff);
    if (parser != NULL) {
        while (NuFParGetLine(parser) != 0) {
            if (strip_comments != 0 && Text_StripComments(parser->line_buf, parser->line_buf, 1) == 0)
                continue;
            if (NuFParGetWord(parser) == 0)
                continue;
            if (NuStrICmp(parser->word_buf, "txt_file") == 0) {
                if (result == 0 && NuFParGetWord(parser) != 0 && NuStrLen(parser->word_buf) < 64) {
                    result = 1;
                    NuStrCpy(filename, parser->word_buf);
                }
            } else if (result != 0) {
                result = 2;
                break;
            }
        }
        NuFParDestroy(parser);
    }
    return result;
}
#endif
