// gameapi/cutscene/unk_006c8f30.cpp: TU of unknown name, found by its
// header-static copies (the functions after them are not matched yet).

#include "../../nu2api/numath/nuinline_unk.h"
#include "../../nu2api/numath/numtx_inline_unk.h"
#include "../../nu2api/numath/nutrig_unk.h"

// FUNCTION: LEGOBATMAN 0x006c8f30
static void NuMtxCopyInline(f32 *dst, f32 *src);

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_006c8f30(f32 *v, f32 a, i32 i) {
  NuMtxCopyInline(v + 32, v + 16);
}
