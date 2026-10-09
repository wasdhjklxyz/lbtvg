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
