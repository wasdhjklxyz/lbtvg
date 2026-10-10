// nu2api/nu3d, file unknown: HLSLShaderBuilder (RTTI vtable 0x86f85c; Mac
// HLSLShaderBuilder::*), buildHeader overloads.

#include "../nucore/common.h"
#include "../numath/nuinline_unk.h"

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x0069ae20
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

struct nushadermtldesc_s;
struct numtl_s;

class PreInterpretor {
public:
  void addSymbol(char const *name, char const *value, bool constant);
};

class ShaderMtlDescFilter {
public:
  ShaderMtlDescFilter(nushadermtldesc_s const *desc, numtl_s const *mtl, i32 a,
                      i32 b);

  u32 pad[0x20 / 4];
};

class ShaderBuilderGen {
public:
  virtual void Vfn0();
  virtual void Vfn1();
  virtual void Vfn2();
  virtual void Vfn3();
  virtual void Vfn4();
  // MSVC lays overloaded virtuals out in reverse: desc is slot 5, filter 6.
  virtual void buildHeader(ShaderMtlDescFilter const *filter,
                           PreInterpretor &interp);
  virtual void buildHeader(nushadermtldesc_s const *desc,
                           PreInterpretor &interp);
};

class HLSLShaderBuilder : public ShaderBuilderGen {
public:
  // MSVC lays overloaded virtuals out in reverse: desc is slot 5, filter 6.
  virtual void buildHeader(ShaderMtlDescFilter const *filter,
                           PreInterpretor &interp);
  virtual void buildHeader(nushadermtldesc_s const *desc,
                           PreInterpretor &interp);
};

// FUNCTION: LEGOBATMAN 0x0069c7f0
void HLSLShaderBuilder::buildHeader(nushadermtldesc_s const *desc,
                                    PreInterpretor &interp) {
  ShaderMtlDescFilter filter(desc, 0, 0, 0);
  buildHeader(&filter, interp);
}

// FUNCTION: LEGOBATMAN 0x0069c830
void HLSLShaderBuilder::buildHeader(ShaderMtlDescFilter const *filter,
                                    PreInterpretor &interp) {
  ShaderBuilderGen::buildHeader(filter, interp);
  interp.addSymbol("BUILD_VERTEX_SHADER", 0, false);
  interp.addSymbol("BUILD_PIXEL_SHADER", 0, false);
  interp.addSymbol("HLSL_LANGUAGE", 0, false);
  interp.addSymbol("COLOR_FACTOR", "2.0", true);
}

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_unk_0069c7f0(f32 *v, f32 a, i32 i) {
  NuVec4Set(v, a, a, a, a);
}
