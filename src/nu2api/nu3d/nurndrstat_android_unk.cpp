// nu2api/nu3d/nurndrstat_android_unk.cpp: placed by tools/new.py; file name
// unproven.

#include "../nucore/common.h"
#include <stddef.h>

// GLOBAL: LEGOBATMAN 0x00b102d4
extern i32 g_rndr_state_global_id;
// GLOBAL: LEGOBATMAN 0x00b102e6
extern u16 g_rndr_state_reflection_id;
// GLOBAL: LEGOBATMAN 0x00b10548
extern i32 g_rndr_state_reflection;
// GLOBAL: LEGOBATMAN 0x00b1054c
extern void *g_rndr_state_reflection_state;

// FUNCTION: LEGOBATMAN 0x006ebae0
void RndrStateSetReflection(i32 reflection) {
  g_rndr_state_reflection = reflection;
  g_rndr_state_global_id++;
  g_rndr_state_reflection_id++;
  g_rndr_state_reflection_state = NULL;
}
