// nu2api/nufile/nufpar_com_unk.cpp: NuFPar position and command-stack helpers,
// just before nulist (0x006d40d0) as on Mac; file name unproven.
// from saga nu2api/nufile/nufpar.cpp

#include "../nucore/common.h"

#include "../numath/nuinline_unk.h"
#include <stddef.h>

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x006d2590
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

typedef void nufpcomfn(struct nufpar_s *parser);

typedef struct nufparpos_s {
  i32 line_num;  // 0x00
  i32 line_pos;  // 0x04
  i32 char_pos;  // 0x08
  i32 buf_start; // 0x0c
  i32 buf_end;   // 0x10
} NUFPARPOS;

typedef struct nufpar_s {
  u32 pad0[0x91c / 4];
  i32 line_num;    // 0x91c
  i32 line_pos;    // 0x920
  i32 char_pos;    // 0x924
  i32 buf_start;   // 0x928
  i32 buf_end;     // 0x92c
  void *jump[8];   // 0x930
  void *jump2[8];  // 0x950
  i32 command_pos; // 0x970
} NUFPAR;

// FUNCTION: LEGOBATMAN 0x006d3e40
void NuFParGetPos(NUFPAR *parser, NUFPARPOS *position) {
  position->line_num = parser->line_num;
  position->line_pos = parser->line_pos;
  position->char_pos = parser->char_pos;
  position->buf_start = parser->buf_start;
  position->buf_end = parser->buf_end;
}

// FUNCTION: LEGOBATMAN 0x006d3e80
void NuFParSetPos(NUFPAR *parser, NUFPARPOS *position) {
  parser->char_pos = position->char_pos;
  parser->line_pos = position->line_pos;
  if (parser->char_pos < parser->buf_start ||
      parser->char_pos > parser->buf_end)
    parser->buf_end = parser->char_pos - 1;
}

// FUNCTION: LEGOBATMAN 0x006d3ec0
i32 NuFParPushCom(NUFPAR *parser, void *commands) {
  if (parser->command_pos >= 7)
    return -1;
  parser->command_pos++;
  parser->jump[parser->command_pos] = commands;
  parser->jump2[parser->command_pos] = NULL;
  return parser->command_pos;
}

// FUNCTION: LEGOBATMAN 0x006d3f00
i32 NuFParPushComCTX(NUFPAR *parser, void *commands) {
  if (parser->command_pos >= 7)
    return -1;
  parser->command_pos++;
  parser->jump[parser->command_pos] = commands;
  parser->jump2[parser->command_pos] = NULL;
  return parser->command_pos;
}

// FUNCTION: LEGOBATMAN 0x006d3f40
i32 NuFParPushCom2(NUFPAR *parser, void *commands, void *commands2) {
  if (parser->command_pos >= 7)
    return -1;
  parser->command_pos++;
  parser->jump[parser->command_pos] = commands;
  parser->jump2[parser->command_pos] = commands2;
  return parser->command_pos;
}

// FUNCTION: LEGOBATMAN 0x006d3f80
i32 NuFParPushComCTX2(NUFPAR *parser, void *commands, void *commands2) {
  if (parser->command_pos >= 7)
    return -1;
  parser->command_pos++;
  parser->jump[parser->command_pos] = commands;
  parser->jump2[parser->command_pos] = commands2;
  return parser->command_pos;
}

// FUNCTION: LEGOBATMAN 0x006d3fc0
void NuFParPopCom(NUFPAR *parser) {
  if (parser->command_pos >= 0)
    parser->command_pos--;
}

// annotated in nufpar_unk.cpp (0x00b038c0)
extern nufpcomfn *fnInterpreterError;

// FUNCTION: LEGOBATMAN 0x006d3fe0
nufpcomfn *NuFParSetInterpreterErrorHandler(nufpcomfn *fn) {
  nufpcomfn *old = fnInterpreterError;
  fnInterpreterError = fn;
  return old;
}

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_nufpar_com_unk(f32 *v, f32 a, i32 i) {
  NuVec4Set(v, a, a, a, a);
}
