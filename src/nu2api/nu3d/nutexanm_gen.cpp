// nu2api/nu3d/nutexanm_gen.cpp: certain range 0x00711580..0x00712030.
// Texture-animation script compiler: tokens from a NuParse stream become
// 16-bit opcodes in a program block.

#include "../nucore/common.h"
#include <string.h>

// Not yet named (nupad_gen.cpp / numemblk_gen.cpp ranges).
extern "C" void *NuMemAllocUnk006e1d50(int size, const char *file, int line);
extern "C" int NuParseUnk006da8d0(struct NuParse *p);
extern "C" int NuParseIntUnk006dd060(struct NuParse *p);
extern "C" int NuStrCmpUnk006dc3a0(const char *a, const char *b);
extern "C" void NuStrCpyUnk006d7590(char *dst, const char *src);
// An empty inline function in TT.s build (0x006fceb0 is a lone `ret`): the
// compiler knows it clobbers nothing, so callers keep values in ecx across it.
static void NuErrorUnk006fceb0(...) {}

struct NuParse {
  unsigned char pad0[0x910];
  char *token;
  unsigned char pad1[0x91c - 0x914];
  int line;
};

struct NuTexAnmProg {
  int f0;
  int f4;
  char name[0x20];       // 0x08
  int onSignal[0x20];    // 0x28
  int offSignal[0x20];   // 0xa8
  unsigned int onMask;   // 0x128
  unsigned int offMask;  // 0x12c
  short labelName[0x20]; // 0x130
  short labelOp[0x20];
  int numLabels;
  short numOps;
  short pad1;
  unsigned short mask; // 0x1b8
  short ops[1];
};

struct NuTexAnm {
  int f0;
  int f4;
  unsigned char pad0[0x88 - 8];
  int f88;
  unsigned char pad1[0xcc - 0x8c];
  int fcc;
  int fd0;
  int fd4;
  int fd8;
  int fdc;
  int fe0;
  int fe4;
  int flags;
};

// GLOBAL: LEGOBATMAN 0x029f1888
NuTexAnmProg *g_texAnmProg;
// GLOBAL: LEGOBATMAN 0x029edc70
int g_texAnmNumNames;
// GLOBAL: LEGOBATMAN 0x029f2e50
char g_texAnmNames[0x40][0x15];
// GLOBAL: LEGOBATMAN 0x029e3adc
int g_texAnmNumVars;
// GLOBAL: LEGOBATMAN 0x029de428
char g_texAnmVars[0x100][0x15];
// GLOBAL: LEGOBATMAN 0x029e3b20
int g_texAnmNameOp[0x40];

// Line number of the allocation in TT's file is 564; it is an immediate. Only
// esi/edi are swapped against the original; not matched yet.
// STUB: LEGOBATMAN 0x00711580
NuTexAnm *NuTexAnmCreate(int *heap, int p2, int p3, int p4) {
  NuTexAnm *anm;
  if (heap) {
    anm = (NuTexAnm *)((*heap + 3) & ~3);
    *heap = (int)anm + 0xec;
  } else {
    anm = (NuTexAnm *)NuMemAllocUnk006e1d50(0xec, __FILE__, 564);
  }
  if (anm) {
    anm->f0 = p4;
    anm->f4 = 0;
    anm->f88 = 0;
    anm->fcc = 0;
    anm->fd0 = 0;
    anm->fd4 = 0;
    anm->fd8 = 0;
    anm->fdc = p2;
    anm->fe0 = p3;
    anm->fe4 = 0;
    if (heap)
      anm->flags &= ~1;
    else
      anm->flags |= 1;
  }
  return anm;
}

// 0 '=', 1 '<', 2 '>', 3 '<=', 4 '>=', 5 '!=' or '<>'. Bytes match except the
// placement of the shared `return 5` block; not matched yet.
// STUB: LEGOBATMAN 0x007116d0
static int NuTexAnmParseCompareOp(NuParse *p) {
  char *t;
  NuParseUnk006da8d0(p);
  t = p->token;
  switch (t[0]) {
  case '!':
    return 5;
  case '<':
    switch (t[1]) {
    case '>':
      return 5;
    case '=':
      return 3;
    }
    return 1;
  case '>':
    return t[1] == '=' ? 4 : 2;
  case '=':
    return 0;
  default:
    NuErrorUnk006fceb0(t, p->line);
    return 0;
  }
}

// FUNCTION: LEGOBATMAN 0x00711770
int NuTexAnmFindOrAddName(char *name) {
  int i;
  if (strlen(name) >= 0x15)
    name[0x14] = 0;
  for (i = 0; i < g_texAnmNumNames; i++) {
    if (NuStrCmpUnk006dc3a0(g_texAnmNames[i], name) == 0)
      return i;
  }
  if (g_texAnmNumNames >= 0x40)
    NuErrorUnk006fceb0();
  NuStrCpyUnk006d7590(g_texAnmNames[g_texAnmNumNames++], name);
  return g_texAnmNumNames - 1;
}

// FUNCTION: LEGOBATMAN 0x00711800
int NuTexAnmFindOrAddVar(char *name) {
  int i;
  if (strlen(name) >= 0x15)
    name[0x14] = 0;
  for (i = 0; i < g_texAnmNumVars; i++) {
    if (NuStrCmpUnk006dc3a0(g_texAnmVars[i], name) == 0)
      return i;
  }
  if (g_texAnmNumVars >= 0x100)
    NuErrorUnk006fceb0();
  NuStrCpyUnk006d7590(g_texAnmVars[g_texAnmNumVars++], name);
  return g_texAnmNumVars - 1;
}

// FUNCTION: LEGOBATMAN 0x00711890
void pftaScriptMask(NuParse *p) {
  g_texAnmProg->mask = (unsigned short)NuParseIntUnk006dd060(p);
}

// FUNCTION: LEGOBATMAN 0x007118b0
void pftaTex(NuParse *p) {
  int a = NuParseIntUnk006dd060(p);
  NuTexAnmProg *prog = g_texAnmProg;
  prog->ops[prog->numOps++] = 0;
  prog->ops[prog->numOps++] = (short)a;
}

// FUNCTION: LEGOBATMAN 0x00711900
void pftaTexR(NuParse *p) {
  int a = NuParseIntUnk006dd060(p);
  NuTexAnmProg *prog = g_texAnmProg;
  prog->ops[prog->numOps++] = 1;
  prog->ops[prog->numOps++] = (short)a;
}

// FUNCTION: LEGOBATMAN 0x00711950
void pftaTexAdj(NuParse *p) {
  int a = NuParseIntUnk006dd060(p);
  int b = NuParseIntUnk006dd060(p);
  int c = NuParseIntUnk006dd060(p);
  NuTexAnmProg *prog = g_texAnmProg;
  prog->ops[prog->numOps++] = 2;
  prog->ops[prog->numOps++] = (short)a;
  prog->ops[prog->numOps++] = (short)b;
  prog->ops[prog->numOps++] = (short)c;
}

// FUNCTION: LEGOBATMAN 0x007119f0
void pftaTexAdjR(NuParse *p) {
  int a = NuParseIntUnk006dd060(p);
  int b = NuParseIntUnk006dd060(p);
  int c = NuParseIntUnk006dd060(p);
  int d = NuParseIntUnk006dd060(p);
  NuTexAnmProg *prog = g_texAnmProg;
  prog->ops[prog->numOps++] = 3;
  prog->ops[prog->numOps++] = (short)a;
  prog->ops[prog->numOps++] = (short)b;
  prog->ops[prog->numOps++] = (short)c;
  prog->ops[prog->numOps++] = (short)d;
}

// FUNCTION: LEGOBATMAN 0x00711ab0
void pftaWait(NuParse *p) {
  int a = NuParseIntUnk006dd060(p);
  int b = NuParseIntUnk006dd060(p);
  NuTexAnmProg *prog = g_texAnmProg;
  prog->ops[prog->numOps++] = 4;
  prog->ops[prog->numOps++] = (short)(int)(a * (1.0f / 60.0f) * 4096.0f);
  prog->ops[prog->numOps++] = (short)(int)(b * (1.0f / 60.0f) * 4096.0f);
}

// FUNCTION: LEGOBATMAN 0x00711b50
void pftaRate(NuParse *p) {
  int a = NuParseIntUnk006dd060(p);
  int b = NuParseIntUnk006dd060(p);
  NuTexAnmProg *prog = g_texAnmProg;
  prog->ops[prog->numOps++] = 7;
  prog->ops[prog->numOps++] = (short)(int)(a * (1.0f / 60.0f) * 4096.0f);
  prog->ops[prog->numOps++] = (short)(int)(b * (1.0f / 60.0f) * 4096.0f);
}

// FUNCTION: LEGOBATMAN 0x00711bf0
void pftaOn(NuParse *p) {
  int signal = NuParseIntUnk006dd060(p);
  NuTexAnmProg *prog = g_texAnmProg;
  prog->onSignal[signal] = prog->numOps;
  prog->onMask |= 1 << signal;
}

// FUNCTION: LEGOBATMAN 0x00711c20
void pftaOff(NuParse *p) {
  int signal = NuParseIntUnk006dd060(p);
  NuTexAnmProg *prog = g_texAnmProg;
  prog->offSignal[signal] = prog->numOps;
  prog->offMask |= 1 << signal;
}

// FUNCTION: LEGOBATMAN 0x00711c60
void pftaLabel(NuParse *p) {
  int name;
  NuParseUnk006da8d0(p);
  name = NuTexAnmFindOrAddName(p->token);
  g_texAnmNameOp[name] = g_texAnmProg->numOps;
}

// FUNCTION: LEGOBATMAN 0x00711c90
void pftaXDef(NuParse *p) {
  int var;
  NuTexAnmProg *prog;
  NuParseUnk006da8d0(p);
  var = NuTexAnmFindOrAddVar(p->token);
  prog = g_texAnmProg;
  prog->labelName[prog->numLabels] = (short)var;
  prog->labelOp[prog->numLabels] = prog->numOps;
  prog->numLabels++;
}

// FUNCTION: LEGOBATMAN 0x00711ce0
void pftaGoto(NuParse *p) {
  int name;
  NuTexAnmProg *prog;
  NuParseUnk006da8d0(p);
  name = NuTexAnmFindOrAddName(p->token);
  prog = g_texAnmProg;
  prog->ops[prog->numOps++] = 9;
  prog->ops[prog->numOps++] = (short)name;
}

// FUNCTION: LEGOBATMAN 0x00711d40
void pftaXRef(NuParse *p) {
  int var;
  NuTexAnmProg *prog;
  NuParseUnk006da8d0(p);
  var = NuTexAnmFindOrAddVar(p->token);
  prog = g_texAnmProg;
  prog->ops[prog->numOps++] = 0x11;
  prog->ops[prog->numOps++] = (short)var;
}

// FUNCTION: LEGOBATMAN 0x00711da0
void pftaBtex(NuParse *p) {
  int op = NuTexAnmParseCompareOp(p);
  int value = NuParseIntUnk006dd060(p);
  int name;
  NuParseUnk006da8d0(p);
  name = NuTexAnmFindOrAddName(p->token);
  NuTexAnmProg *prog = g_texAnmProg;
  prog->ops[prog->numOps++] = 0xb;
  prog->ops[prog->numOps++] = (short)op;
  prog->ops[prog->numOps++] = (short)value;
  prog->ops[prog->numOps++] = (short)name;
}

// FUNCTION: LEGOBATMAN 0x00711e40
void pftaGosub(NuParse *p) {
  int name;
  NuTexAnmProg *prog;
  NuParseUnk006da8d0(p);
  name = NuTexAnmFindOrAddName(p->token);
  prog = g_texAnmProg;
  prog->ops[prog->numOps++] = 0xa;
  prog->ops[prog->numOps++] = (short)name;
}

// FUNCTION: LEGOBATMAN 0x00711ea0
void pftaRet(NuParse *p) {
  NuTexAnmProg *prog = g_texAnmProg;
  prog->ops[prog->numOps++] = 0xc;
}

// FUNCTION: LEGOBATMAN 0x00711ec0
void pftaRepeat(NuParse *p) {
  int count = NuParseIntUnk006dd060(p);
  if (count == 0)
    count = 0x7fffffff;
  int b = NuParseIntUnk006dd060(p);
  NuTexAnmProg *prog = g_texAnmProg;
  prog->ops[prog->numOps++] = 0xd;
  prog->ops[prog->numOps++] = (short)count;
  prog->ops[prog->numOps++] = (short)b;
}

// FUNCTION: LEGOBATMAN 0x00711f40
void pftaRepend(NuParse *p) {
  NuTexAnmProg *prog = g_texAnmProg;
  prog->ops[prog->numOps++] = 0xe;
}

// FUNCTION: LEGOBATMAN 0x00711f60
void pftaUntiltex(NuParse *p) {
  int op = NuTexAnmParseCompareOp(p);
  int value = NuParseIntUnk006dd060(p);
  NuTexAnmProg *prog = g_texAnmProg;
  prog->ops[prog->numOps++] = 0xf;
  prog->ops[prog->numOps++] = (short)op;
  prog->ops[prog->numOps++] = (short)value;
}

// FUNCTION: LEGOBATMAN 0x00711fd0
void pftaEnd(NuParse *p) {
  NuTexAnmProg *prog = g_texAnmProg;
  prog->ops[prog->numOps++] = 0x10;
}

// FUNCTION: LEGOBATMAN 0x00711ff0
void pftaScriptname(NuParse *p) {
  char *dst;
  NuParseUnk006da8d0(p);
  dst = g_texAnmProg->name;
  p->token[0x20] = 0;
  strcpy(dst, p->token);
}

// Not yet named (nufpar / numemblk ranges); saga names in comments.
extern "C" NuParse *NuParseOpenUnk006dfab0(int file);           // NuFParOpen
extern "C" void NuParsePushComUnk006d3ec0(NuParse *p, void *t); // NuFParPushCom
extern "C" int NuParseLineUnk006df910(NuParse *p);              // NuFParGetLine
extern "C" int NuParseInterpretUnk006dd2d0(NuParse *p); // NuFParInterpretWord
extern "C" void NuParseCloseUnk006d40c0(NuParse *p);    // NuFParClose
void NuTexAnmProgInitUnk007112b0(NuTexAnmProg *prog);
void NuTexAnmProgEndUnk00711480(NuTexAnmProg *prog);

// GLOBAL: LEGOBATMAN 0x009a6de0
extern unsigned char g_texAnmComTab[];
// GLOBAL: LEGOBATMAN 0x029f3f48
extern NuTexAnmProg *g_texAnmProgs;

// STUB: LEGOBATMAN 0x00712030
// close: orig keeps `end` in its stack slot and caches 0 in ebx (buffer only
// in eax); ours enregisters end. An inline helper taking &end fixes that but
// then the empty-error call disappears.
NuTexAnmProg *NuTexAnimProgParseFile(int file, VARIPTR *buffer, VARIPTR end,
                                     int p4) {
  NuTexAnmProg *prog;
  NuParse *p;
  int len = 0;

  if (buffer != 0) {
    prog = (NuTexAnmProg *)((buffer->addr + 3) & ~3);
  } else {
    prog = (NuTexAnmProg *)NuMemAllocUnk006e1d50(0x400, __FILE__, 0x3d2);
    end.addr = (unsigned int)prog + 0x3ff;
  }
  g_texAnmNumNames = 0;
  memset(g_texAnmNames, 0, sizeof(g_texAnmNames));
  p = NuParseOpenUnk006dfab0(file);
  if (p == 0)
    return (NuTexAnmProg *)len;
  NuParsePushComUnk006d3ec0(p, g_texAnmComTab);
  if ((unsigned int)(prog + 1) >= end.addr)
    NuErrorUnk006fceb0();
  NuTexAnmProgInitUnk007112b0(prog);
  g_texAnmProg = prog;
  while (NuParseLineUnk006df910(p) != 0) {
    len = NuParseUnk006da8d0(p);
    if (len != 0 && NuParseInterpretUnk006dd2d0(p) == 0 &&
        p->token[0] != '\0' && p->token[len - 1] == ':') {
      p->token[len - 1] = '\0';
      int name = NuTexAnmFindOrAddName(p->token);
      g_texAnmNameOp[name] = g_texAnmProg->numOps;
    }
    if ((unsigned int)&prog->ops[prog->numOps] >= end.addr)
      NuErrorUnk006fceb0();
  }
  if (buffer != 0)
    buffer->addr = (unsigned int)&prog->ops[prog->numOps];
  NuParseCloseUnk006d40c0(p);
  NuTexAnmProgEndUnk00711480(prog);
  prog->f0 = (int)g_texAnmProgs;
  if (g_texAnmProgs != 0)
    g_texAnmProgs->f4 = (int)prog;
  prog->f4 = 0;
  g_texAnmProgs = prog;
  return prog;
}
