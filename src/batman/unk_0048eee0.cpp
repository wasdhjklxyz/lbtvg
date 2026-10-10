// batman/unk_0048eee0.cpp: TU of unknown name, found by its header-static
// copies (the functions after them are not matched yet).

#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"

// FUNCTION: LEGOBATMAN 0x0048eee0
static f32 NuSinApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x0048efa0
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_0048eee0(f32 *v, f32 a, i32 i) {
  v[0] = NuSinApprox(i);
  NuVec4Set(v, a, a, a, a);
}

// The rest of this TU is the Mac's GizPanel file (GizPanel_Load ..
// GizPanel_MoveCode); callbacks named by their GizPanel_RegisterGizmo slot.

#include <stddef.h>

typedef struct GIZPANEL_s {
  u8 pad0[0x40];
  char name[0x22]; // 0x40
  u8 b62_0 : 1;    // 0x62
  u8 output : 1;   // 0x62 bit 1
  u8 b62_2 : 1;    // 0x62 bit 2
  u8 b62_3 : 1;    // 0x62 bit 3
} GIZPANEL;

typedef struct GIZMO_s {
  void *object;
} GIZMO;

// FUNCTION: LEGOBATMAN 0x00490780
char *GizPanel_GetGizmoName(GIZMO *gizmo) {
  GIZPANEL *panel;
  if (gizmo != NULL && (panel = (GIZPANEL *)gizmo->object) != NULL)
    return panel->name;
  return NULL;
}

// FUNCTION: LEGOBATMAN 0x004907b0
i32 GizPanel_GetOutput(GIZMO *gizmo, i32 a, i32 b) {
  GIZPANEL *panel = (GIZPANEL *)gizmo->object;
  if (panel->b62_3 && panel->b62_2)
    return panel->output;
  return 0;
}

// FUNCTION: LEGOBATMAN 0x004907e0
char *GizPanel_GetOutputName(GIZMO *gizmo, i32 output_index) {
  return output_index == 0 ? "Finished" : NULL;
}

// FUNCTION: LEGOBATMAN 0x00490800
i32 GizPanel_GetNumOutputs(GIZMO *gizmo) { return 1; }

void *GameBufferAllocProgressUnk005bbaf0(VARIPTR *buf, VARIPTR *buf_end,
                                         i32 size);

// FUNCTION: LEGOBATMAN 0x00490810
void *GizPanels_AllocateProgressData(VARIPTR *buf, VARIPTR *buf_end) {
  return GameBufferAllocProgressUnk005bbaf0(buf, buf_end, 0xc);
}

// FUNCTION: LEGOBATMAN 0x00490830
void GizPanels_ClearProgress(void *world, void *progress_ptr) {
  i32 *progress = (i32 *)progress_ptr;
  if (progress != NULL) {
    progress[0] = 0;
    progress[1] = -1;
    progress[2] = -1;
  }
}
