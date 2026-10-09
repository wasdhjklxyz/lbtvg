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
