// nu2api/gamelib/unk_0067e270.cpp: TU of unknown name, found by its
// header-static copies (the functions after them are not matched yet).

#include "../numath/nuinline_unk.h"
#include "../numath/nutrig_unk.h"

// FUNCTION: LEGOBATMAN 0x0067e270
static f32 NuFabs(f32 f);
// FUNCTION: LEGOBATMAN 0x0067e290
static f32 NuFdiv(f32 a, f32 b);
// FUNCTION: LEGOBATMAN 0x0067e2d0
static f32 NuSinApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x0067e370
static f32 NuCosApprox(i32 angle);

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_0067e270(f32 *v, f32 a, i32 i) {
  v[2] = NuFabs(a);
  v[3] = NuFdiv(a, v[4]);
  v[0] = NuSinApprox(i);
  v[1] = NuCosApprox(i);
}

#include "../nucore/common.h"
#include <stddef.h>

typedef struct nufpar_s {
  u32 pad0[0x910 / 4];
  char *word_buf; // 0x910
} NUFPAR;

typedef struct SOCK_s {
  u8 pad0[0x184];
} SOCK;

typedef struct SOCKSYS_s {
  SOCK *sock; // 0x00
} SOCKSYS;

typedef struct nugscn_s NUGSCN;

NUFPAR *NuFParCreateMem(char *name, char *buffer, i32 size);
i32 NuFParPushComCTX(NUFPAR *parser, void *commands);
i32 NuFParGetLine(NUFPAR *parser);
i32 NuFParGetWord(NUFPAR *parser);
i32 NuFParGetInt(NUFPAR *parser);
void NuFParInterpretWordCTX(NUFPAR *parser, void *ctx);
void NuFParDestroy(NUFPAR *parser);
i32 NuStrICmp(const char *a, const char *b);

// GLOBAL: LEGOBATMAN 0x02a14dcc
extern NUGSCN *sockpar_scene;
// GLOBAL: LEGOBATMAN 0x00ad39b8
extern VARIPTR *sockpar_buffer_ptr;
// GLOBAL: LEGOBATMAN 0x00ad39cc
extern VARIPTR *sockpar_buffer_end;
// GLOBAL: LEGOBATMAN 0x02a14e2c
extern SOCK *sockpar_sock;
// GLOBAL: LEGOBATMAN 0x009693f8
extern u8 SockSys_ConfigKeywords[];

// from saga legoapi/props/system/socksysall.cpp (32 socks here, not 64)
// FUNCTION: LEGOBATMAN 0x00680d60
extern "C" void SockSys_Configure(SOCKSYS *sock_sys, char *config, i32 unused,
                                  VARIPTR *buf, VARIPTR *buf_end,
                                  NUGSCN *gscn) {
  NUFPAR *parser;
  i32 inside_sock;
  i32 sock_index;

  if (sock_sys == NULL || config == NULL || gscn == NULL)
    return;

  parser = NuFParCreateMem("socks", config, 0xffff);
  if (parser == NULL)
    return;

  sockpar_scene = gscn;
  sockpar_buffer_ptr = buf;
  sockpar_buffer_end = buf_end;
  NuFParPushComCTX(parser, SockSys_ConfigKeywords);

  inside_sock = 0;
  while (NuFParGetLine(parser) != 0) {
    NuFParGetWord(parser);
    if (parser->word_buf[0] == '\0')
      continue;

    if (inside_sock) {
      if (NuStrICmp(parser->word_buf, "sock_end") == 0)
        inside_sock = 0;
      else
        NuFParInterpretWordCTX(parser, NULL);
    } else if (NuStrICmp(parser->word_buf, "sock_start") == 0) {
      sock_index = NuFParGetInt(parser);
      if ((u32)sock_index <= 0x1f) {
        inside_sock = 1;
        sockpar_sock = &sock_sys->sock[sock_index];
      }
    }
  }
  NuFParDestroy(parser);
}
