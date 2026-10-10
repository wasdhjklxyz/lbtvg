// nu2api/nu3d, file unknown: ShaderBuilderGen (RTTI vtable 0x86f83c; Mac
// ShaderBuilderGen::*).

#include "../nucore/common.h"

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

// FUNCTION: LEGOBATMAN 0x00694a00
void ShaderBuilderGen::buildHeader(nushadermtldesc_s const *desc,
                                   PreInterpretor &interp) {
  ShaderMtlDescFilter filter(desc, 0, 0, 0);
  buildHeader(&filter, interp);
}
