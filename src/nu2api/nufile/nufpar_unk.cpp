// nu2api/nufile/nufpar_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nucore/nustring.h"

typedef u16 NUWCHAR16;

typedef struct nufpar_s {
  u32 pad0[2];
  char file_name[0x100]; // 0x08
  u32 pad108[(0x910 - 0x108) / 4];
  char *word_buf; // 0x910
  u32 pad914[(0x930 - 0x914) / 4];
  struct nufpcomjump_s *jump_ctx[8];  // 0x930
  struct nufpcomjump_s *jump_ctx2[8]; // 0x950
  i32 command_pos;                    // 0x970
  u32 pad974;
  char is_utf16; // 0x978
} NUFPAR;

i32 NuFParGetWord(NUFPAR *parser);
void NuUnicodeToAscii(char *dst, NUWCHAR16 *src);

// STUB: LEGOBATMAN 0x006da8d0
// tangled quote/separator state machine (ebp=1 constant, tail jump); not
// attempted
#if 0
i32 NuFParGetWordW(NUFPAR *parser);

static i32 old_line_pos;

#define CLAMP_LINE(pos) pos &(parser->line_buf_size - 1)
#define CLAMP_WORD(pos) pos &(parser->word_buf_size - 1)
#define CLAMP_WIDE_LINE(pos) pos &((parser->line_buf_size >> 1) - 1)
#define CLAMP_WIDE_WORD(pos) pos &((parser->word_buf_size >> 1) - 1)

i32 NuFParGetLine(NUFPAR *parser);

// from saga nu2api/nufile/nufpar.cpp
i32 NuFParGetWord(NUFPAR *parser) {
    i32 len;
    i32 in_quoted_text;
    i32 found_quotes = 0;

    if (parser->is_utf16) {
        return NuFParGetWordW(parser);
    }

    len = 0;
    in_quoted_text = 0;

    old_line_pos = parser->line_pos;

    while (parser->line_buf[CLAMP_LINE(parser->line_pos)] != 0) {
        char c = parser->line_buf[parser->line_pos];

        if (parser->separator_tokens != NULL && !in_quoted_text) {
            if (NuStrChr(parser->separator_tokens, c) != NULL) {
                if (len == 0) {
                    parser->word_buf[len] = c;
                    len++;
                    parser->line_pos++;
                }

                parser->word_buf[CLAMP_WORD(len)] = '\0';

                return len;
            }
        }

        if (parser->separator_list != NULL && !in_quoted_text) {
            if (NuStrChr(parser->separator_list, c) != NULL) {
                c = ' ';
            }
        }

        switch (c) {
            case ' ':
            case ',':
            case '\t':
                if (!in_quoted_text) {
                    if (len != 0) {
                        parser->word_buf[CLAMP_WORD(len)] = '\0';
                        return len;
                    }

                    break;
                }
            default:
                if (c == '"') {
                    in_quoted_text = 1 - in_quoted_text;
                    found_quotes = 1;
                } else {
                    parser->word_buf[CLAMP_WORD(len)] = c;
                    len++;
                }

                break;
        }

        parser->line_pos++;

        if (found_quotes && in_quoted_text == 0 && len == 0) {
            break;
        }
    }

    parser->word_buf[CLAMP_WORD(len)] = '\0';
    return len;
}
#endif

// FUNCTION: LEGOBATMAN 0x006daa40
f32 NuFParGetFloat(NUFPAR *parser) {
  char buf[64];

  NuFParGetWord(parser);
  if (parser->is_utf16) {
    NuUnicodeToAscii(buf, (NUWCHAR16 *)parser->word_buf);
  } else {
    char *dst = buf;
    char *src = parser->word_buf;
    if (src != 0) {
      while (*src != '\0') {
        *dst++ = *src++;
      }
    }
    *dst = '\0';
  }

  if (buf[0] != '\0') {
    return NuAToF(buf);
  } else {
    return 0.0f;
  }
}

typedef i32 NUFILE;

struct numemfile_s {
  char *buffer;
  char *end;
  char *ptr;
  i32 mode;
  i32 used;
};

// GLOBAL: LEGOBATMAN 0x00afa0a8
extern numemfile_s memfiles[20];

NUFPAR *NuFParOpen(NUFILE file_handle);
void NuFileClose(NUFILE file);

static inline NUFILE NuMemFileOpen(void *buf, i32 buf_size, i32 mode) {
  i32 i;

  if (buf_size > 0 && (mode == 0 || mode == 1)) {
    for (i = 0; i < 20; i++) {
      if (!memfiles[i].used) {
        memfiles[i].buffer = (char *)buf;
        memfiles[i].end = (char *)buf + buf_size - 1;
        memfiles[i].ptr = memfiles[i].buffer;
        memfiles[i].mode = mode;
        memfiles[i].used = 1;

        return i + 0x400;
      }
    }
  }

  return 0;
}

i32 NuHexStringToI(char *string);

// FUNCTION: LEGOBATMAN 0x006dd060
i32 NuFParGetInt(NUFPAR *parser) {
  char buf[64];

  NuFParGetWord(parser);
  if (parser->is_utf16) {
    NuUnicodeToAscii(buf, (NUWCHAR16 *)parser->word_buf);
  } else {
    char *dst = buf;
    char *src = parser->word_buf;
    if (src != 0) {
      while (*src != '\0') {
        *dst++ = *src++;
      }
    }
    *dst = '\0';
  }

  if (buf[0] != '\0') {
    if (buf[0] == '$') {
      return NuHexStringToI(buf + 1);
    } else if (buf[0] == '0' && (buf[1] == 'x' || buf[1] == 'X')) {
      return NuHexStringToI(buf + 2);
    } else {
      return NuAToI(buf);
    }
  } else {
    return 0;
  }
}

typedef void nufpcomfn(NUFPAR *parser);
typedef void nufpcomctxfn(NUFPAR *parser, void *ctx);

typedef struct nufpcomjump_s {
  char *fn_name;
  nufpcomctxfn *fn;
} NUFPCOMJUMP;

// GLOBAL: LEGOBATMAN 0x00b038c0
extern nufpcomfn *fnInterpreterError;

// STUB: LEGOBATMAN 0x006dd2d0
// close: the same check-then-reload of jump_ctx2[pos] as the CTX version
// below (orig cmp [mem],0 then reload; ours loads once and tests)
i32 NuFParInterpretWord(NUFPAR *parser) {
  char buf[64];
  i32 i;

  if (parser->is_utf16) {
    NuUnicodeToAscii(buf, (NUWCHAR16 *)parser->word_buf);
  } else {
    char *dst = buf;
    char *src = parser->word_buf;
    if (src != 0) {
      while (*src != '\0') {
        *dst++ = *src++;
      }
    }
    *dst = '\0';
  }

  if (buf[0] == '\0')
    return 0;
  if (buf[0] == ';')
    return 0;

  i32 pos = parser->command_pos;
  if (pos >= 0) {
    NUFPCOMJUMP *jump = parser->jump_ctx[pos];
    char *name;
    for (i = 0; (name = jump[i].fn_name) != 0; i++) {
      if (NuStrICmp(name, buf) == 0) {
        ((nufpcomfn *)jump[i].fn)(parser);
        return 1;
      }
    }

    if (parser->jump_ctx2[pos] == 0)
      goto error;
    jump = parser->jump_ctx2[pos];
    for (i = 0; (name = jump[i].fn_name) != 0; i++) {
      if (NuStrICmp(name, buf) == 0) {
        ((nufpcomfn *)jump[i].fn)(parser);
        return 1;
      }
    }
  }
error:

  if (fnInterpreterError != 0)
    (*fnInterpreterError)(parser);
  return 0;
}

// STUB: LEGOBATMAN 0x006dd430
// close: orig tests jump_ctx2[pos] with cmp [mem],0 then reloads it (same
// unsolved check-then-reload as FastWeaponOut); ours keeps it in edi
i32 NuFParInterpretWordCTX(NUFPAR *parser, void *ctx) {
  char buf[64];
  i32 i;

  if (parser->is_utf16) {
    NuUnicodeToAscii(buf, (NUWCHAR16 *)parser->word_buf);
  } else {
    char *dst = buf;
    char *src = parser->word_buf;
    if (src != 0) {
      while (*src != '\0') {
        *dst++ = *src++;
      }
    }
    *dst = '\0';
  }

  if (buf[0] == '\0')
    return 0;
  if (buf[0] == ';')
    return 0;

  i32 pos = parser->command_pos;
  if (pos >= 0) {
    NUFPCOMJUMP *jump = parser->jump_ctx[pos];
    char *name;
    for (i = 0; (name = jump[i].fn_name) != 0; i++) {
      if (NuStrICmp(name, buf) == 0) {
        jump[i].fn(parser, ctx);
        return 1;
      }
    }

    if (parser->jump_ctx2[pos] != 0) {
      NUFPCOMJUMP *jump2 = parser->jump_ctx2[pos];
      for (i = 0; (name = jump2[i].fn_name) != 0; i++) {
        if (NuStrICmp(name, buf) == 0) {
          jump2[i].fn(parser, ctx);
          return 1;
        }
      }
    }
  }

  if (fnInterpreterError != 0)
    (*fnInterpreterError)(parser);
  return 0;
}

// FUNCTION: LEGOBATMAN 0x006dfc60
NUFPAR *NuFParCreateMem(char *name, char *buffer, i32 bufferSize) {
  if (bufferSize != 0 && buffer != 0) {
    NUFILE file_handle = NuMemFileOpen(buffer, bufferSize, 0);
    if (file_handle != 0) {
      NUFPAR *parser = NuFParOpen(file_handle);
      if (parser != 0) {
        NuStrCpy(parser->file_name, name);
        return parser;
      }

      NuFileClose(file_handle);
    }
  }

  return 0;
}
