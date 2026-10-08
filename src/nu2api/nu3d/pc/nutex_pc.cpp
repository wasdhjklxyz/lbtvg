// nu2api/nu3d/pc/nutex_pc.cpp: certain range 0x006e6060..0x006e7bf0.

#include <d3d9.h>

// d3dCalls.cpp / d3dApiCalls.cpp wrappers (batman/), not yet named.
extern "C" void D3DUnk005290c0();
extern "C" void D3DUnk005290d0();
extern "C" void D3DUnk00531350(int a, int b, IDirect3DTexture9 **out);
extern "C" void D3DUnk005314d0(IDirect3DTexture9 **tex);
extern "C" void D3DUnk00531110(D3DVIEWPORT9 *vp);
extern "C" void D3DUnk00531060(void *p);
extern "C" void D3DUnk00530bf0(void *p);
extern "C" void NuUnk0069ca10(float f);

struct NuTex {
  int width;
  int height;
  unsigned char pad[0x74 - 8];
  int unk74;
  int unk78;
  IDirect3DTexture9 *d3dtex;
};

// GLOBAL: LEGOBATMAN 0x029dcbd4
int g_nutex_029dcbd4;
// GLOBAL: LEGOBATMAN 0x029dcbdc
int g_nutex_029dcbdc;
// GLOBAL: LEGOBATMAN 0x029dcbe8
IUnknown *g_nutex_029dcbe8;
// GLOBAL: LEGOBATMAN 0x029dcbec
void *g_nutex_029dcbec;
// GLOBAL: LEGOBATMAN 0x029dcbf0
void *g_nutex_029dcbf0;
// GLOBAL: LEGOBATMAN 0x00b0ba90
D3DVIEWPORT9 g_nutex_viewport;

// FUNCTION: LEGOBATMAN 0x006e6170
void NuTexUpdateSize(NuTex *tex) {
  D3DSURFACE_DESC desc;
  if (tex && tex->unk74 && tex->unk78) {
    D3DUnk005290c0();
    D3DUnk00531350(tex->unk74, tex->unk78, &tex->d3dtex);
    tex->d3dtex->GetLevelDesc(0, &desc);
    D3DUnk005290d0();
    tex->width = desc.Width;
    tex->height = desc.Height;
  }
}

// FUNCTION: LEGOBATMAN 0x006e61e0
void NuTexUnk006e61e0(NuTex *tex) {
  if (tex && tex->unk74 && tex->unk78) {
    D3DUnk005290c0();
    D3DUnk005314d0(&tex->d3dtex);
    D3DUnk005290d0();
  }
}

// FUNCTION: LEGOBATMAN 0x006e6220
int NuTexUnk006e6220() { return 0; }

// FUNCTION: LEGOBATMAN 0x006e6230
int NuTexUnk006e6230() { return -1; }

// FUNCTION: LEGOBATMAN 0x006e6240
void NuTexUnk006e6240() {}

// FUNCTION: LEGOBATMAN 0x006e6310
void NuTexUnk006e6310() {}

// FUNCTION: LEGOBATMAN 0x006e6330
void NuTexUnk006e6330() {}

// Bits per pixel by texture format; case bodies are in source order, the
// jump table is by case value.
// FUNCTION: LEGOBATMAN 0x006e6390
int NuTexFormatBpp(int format) {
  switch (format) {
  case 0:
  case 1:
    return 16;
  case 2:
    return 24;
  case 3:
    return 32;
  case 5:
  case 20:
    return 8;
  case 4:
    return 4;
  default:
    return 0;
  }
}

// FUNCTION: LEGOBATMAN 0x006e6ba0
void NuTexUnk006e6ba0() {
  g_nutex_029dcbd4 = 1;
  g_nutex_029dcbdc = 0;
}

// FUNCTION: LEGOBATMAN 0x006e6d80
void NuTexUnk006e6d80() {
  if (g_nutex_029dcbe8) {
    g_nutex_029dcbe8->Release();
    g_nutex_029dcbe8 = 0;
  }
}

// FUNCTION: LEGOBATMAN 0x006e6fa0
void NuTexUnk006e6fa0(float f) { NuUnk0069ca10(f); }

// FUNCTION: LEGOBATMAN 0x006e7020
void NuTexUnk006e7020(void **pair) {
  if (pair) {
    if (g_nutex_029dcbec != pair[0]) {
      D3DUnk00531060(pair[0]);
      g_nutex_029dcbec = pair[0];
    }
    if (g_nutex_029dcbf0 != pair[1]) {
      D3DUnk00530bf0(pair[1]);
      g_nutex_029dcbf0 = pair[1];
    }
  } else {
    D3DUnk00531060(0);
    D3DUnk00530bf0(0);
    g_nutex_029dcbec = 0;
    g_nutex_029dcbf0 = 0;
  }
}

// The six dwords at 0x00b0ba90 are a D3DVIEWPORT9 (MinZ 0, MaxZ 1).
// FUNCTION: LEGOBATMAN 0x006e7200
void NuTexSetViewport(int x, int y, int w, int h) {
  g_nutex_viewport.X = x;
  g_nutex_viewport.Y = y;
  g_nutex_viewport.Width = w;
  g_nutex_viewport.Height = h;
  g_nutex_viewport.MinZ = 0.0f;
  g_nutex_viewport.MaxZ = 1.0f;
  D3DUnk00531110(&g_nutex_viewport);
}
