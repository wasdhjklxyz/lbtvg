#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/numtx_inline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x00588d80
static void NuVecZeroInline(f32 *v);

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x00589050
static void NuMtxCopyInline(f32 *dst, f32 *src);

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x00588c90
static f32 NuFdiv(f32 a, f32 b);
// FUNCTION: LEGOBATMAN 0x00588cd0
static f32 NuSinApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x00588d70
static f32 NuCosApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x00588fd0
static void NuVec4Copy(f32 *dst, f32 *src);
// FUNCTION: LEGOBATMAN 0x00588ff0
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);
// FUNCTION: LEGOBATMAN 0x00589030
static void NuVecScaleInline(f32 *dst, f32 *src, f32 s);
// gameapi/, file unknown (between terrain.c and rtleditor.cpp by link order).

// GLOBAL: LEGOBATMAN 0x0095e230
int g_unk_0095e230;

struct nulsthdr_s;
struct nulnkhdr_s;

// GLOBAL: LEGOBATMAN 0x00a37468
extern struct nulsthdr_s *rtl_dynamic_pool;
// GLOBAL: LEGOBATMAN 0x00a3746c
extern int rtl_dynamic_max;
// GLOBAL: LEGOBATMAN 0x00a37470
extern int rtl_dynamic_cnt;

struct nulnkhdr_s *NuLstGetByIdx(struct nulsthdr_s *list, int index);
void NuLstFree(struct nulnkhdr_s *node);

// from saga legoapi/render/core/rtl.c
// FUNCTION: LEGOBATMAN 0x0058b230
extern "C" void rtlDynamicFree(int id) {
  if (rtl_dynamic_pool != 0 && id >= 0 && id < rtl_dynamic_max) {
    struct nulnkhdr_s *light = NuLstGetByIdx(rtl_dynamic_pool, id);
    if (light != 0) {
      NuLstFree(light);
      --rtl_dynamic_cnt;
    }
  }
}

// Called with 0/1 around level loading; returns the previous value.
// FUNCTION: LEGOBATMAN 0x0058b6d0
int SetUnk0095e230(int value) {
  int old = g_unk_0095e230;
  g_unk_0095e230 = value;
  return old;
}

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_unk_0058b6d0(f32 *v, f32 a, i32 i) {
  v[3] = NuFdiv(a, v[4]);
  v[0] = NuSinApprox(i);
  v[1] = NuCosApprox(i);
  NuVec4Copy(v + 4, v);
  NuVec4Set(v, a, a, a, a);
  NuVecScaleInline(v + 8, v, a);
}

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_2_unk_0058b6d0(f32 *v, f32 a, i32 i) {
  NuMtxCopyInline(v + 32, v + 16);
}

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_3_unk_0058b6d0(f32 *v, f32 a, i32 i) {
  NuVecZeroInline(v + 80);
}
