// gameapi/unk_006762a0.cpp: TU of unknown name, found by its header-static
// copies (the functions after them are not matched yet).

#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"

// FUNCTION: LEGOBATMAN 0x006762a0
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_006762a0(f32 *v, f32 a, i32 i) { NuVec4Set(v, a, a, a, a); }

#include "gameobject_unk.h"
#include <stddef.h>

// GLOBAL: LEGOBATMAN 0x00ab3960
extern GameObject_s *Player[8];

// Mac: last function of the WorldMap TU, same body.
// FUNCTION: LEGOBATMAN 0x006762c0
GameObject_s *PlayerIdToGameObj(i32 id) {
  GameObject_s *obj;

  if ((u32)id <= 1) {
    obj = Player[id];
    return (obj->b1fc & 0x80) ? obj : NULL;
  }
  return NULL;
}
