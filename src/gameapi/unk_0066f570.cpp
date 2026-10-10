// gameapi/unk_0066f570.cpp: TU of unknown name, found by its header-static
// copies (the functions after them are not matched yet).

#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"

// FUNCTION: LEGOBATMAN 0x0066f570
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_0066f570(f32 *v, f32 a, i32 i) { NuVec4Set(v, a, a, a, a); }

// The rest of this TU is the Mac's edGizShadow file (saga
// legoapi/gizmos/fx/edgizshadowmachine.cpp, edGizShadow_GetMaxGizmos ..
// EdGizShadowMachine_RegisterGizmo 0x66f990); Batman's version has a preset
// switch and a longer version history than saga's.

#include "../batman/worldinfo_unk.h"
#include <stddef.h>
#include <string.h>

typedef struct EDGIZSHADOW_s {
  nuvec_s direction; // 0x00
  f32 f0c;           // 0x0c
  f32 f10;           // 0x10
  f32 f14;           // 0x14
  f32 f18;           // 0x18
  f32 f1c;           // 0x1c
  f32 f20;           // 0x20
  f32 f24;           // 0x24
  f32 f28;           // 0x28
  f32 f2c;           // 0x2c
  f32 f30;           // 0x30
  f32 f34;           // 0x34
  i32 preset;        // 0x38
} EDGIZSHADOW;

struct AREADATA_s {
  u8 pad0[0x7c];
  u8 flags; // 0x7c
};

typedef struct GIZMO_s {
  void *object;
} GIZMO;

typedef struct ADDGIZMOTYPE_s {
  char *name;        // 0x00
  char *prefix;      // 0x04
  u16 progress_size; // 0x08
  void *fns[0x1c];   // 0x0c
} ADDGIZMOTYPE;

// GLOBAL: LEGOBATMAN 0x00960118
extern ADDGIZMOTYPE Default_ADDGIZMOTYPE;

GIZMO *AddGizmo(GIZMOSYS_s *gizmo_sys, i32 type_id, void *a, void *object);
char EdFileReadChar();
i32 EdFileReadInt();
f32 EdFileReadFloat();
void EdFileReadNuVec(nuvec_s *v);

// FUNCTION: LEGOBATMAN 0x0066f590
i32 edGizShadow_GetMaxGizmos(void *world) { return 1; }

// FUNCTION: LEGOBATMAN 0x0066f5a0
void edGizShadow_AddGizmos(GIZMOSYS_s *gizmo_sys, i32 type_id, void *world_ptr,
                           void *unused) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  AddGizmo(gizmo_sys, type_id, NULL, &world->shadow_editor);
}

// GLOBAL: LEGOBATMAN 0x00968270
static char *edGizShadow_Name = "ShadowDirection";
// GLOBAL: LEGOBATMAN 0x00968274
static char *edGizShadow_OutputName = "Dont Use Me!";

// FUNCTION: LEGOBATMAN 0x0066f5c0
char *edGizShadow_GetGizmoName(GIZMO *gizmo) { return edGizShadow_Name; }

// FUNCTION: LEGOBATMAN 0x0066f5d0
i32 edGizShadow_GetOutput(GIZMO *gizmo, i32 a, i32 b) { return 0; }

// FUNCTION: LEGOBATMAN 0x0066f5e0
char *edGizShadow_GetOutputName(GIZMO *gizmo, i32 output_index) {
  return edGizShadow_OutputName;
}

// FUNCTION: LEGOBATMAN 0x0066f5f0
i32 edGizShadow_GetNumOutputs(GIZMO *gizmo) { return 1; }

// FUNCTION: LEGOBATMAN 0x0066f600
static void edGizShadow_SetPreset(EDGIZSHADOW *shadow, i32 preset) {
  switch (preset) {
  case 0:
    shadow->preset = 0;
    break;
  case 1:
    shadow->f10 = 2.0f;
    shadow->f14 = 0.5f;
    shadow->f18 = 0.0001f;
    shadow->f1c = 0.0005f;
    shadow->f20 = 7.0f;
    shadow->f24 = 5.0f;
    shadow->f28 = 1.2f;
    shadow->f2c = 0.48f;
    shadow->f30 = 0.0f;
    shadow->f34 = -0.5f;
    shadow->f0c = 0.4f;
    shadow->preset = 1;
    break;
  case 2:
    shadow->f10 = 2.0f;
    shadow->f14 = 0.1f;
    shadow->f18 = 0.0005f;
    shadow->f1c = 0.01f;
    shadow->f20 = 25.0f;
    shadow->f24 = 15.0f;
    shadow->f28 = 0.5f;
    shadow->f2c = 0.0f;
    shadow->f30 = 0.5f;
    shadow->f34 = -15.0f;
    shadow->f0c = 0.4f;
    shadow->preset = 2;
    break;
  }
}

// STUB: LEGOBATMAN 0x0066f6f0
// matches except that our cl inlines the version-8 SetPreset(shadow, 1)
// call, which the original keeps as a call (SetPreset(shadow, 0) tried).
i32 edGizShadow_Load(void *world_ptr, void *unused) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  if (world == NULL || world->shadow_editor == NULL)
    return 0;
  EDGIZSHADOW *shadow = world->shadow_editor;
  i32 version = EdFileReadChar();
  i32 count = EdFileReadChar();
  for (i32 i = 0; i < count; i++) {
    EdFileReadNuVec(&shadow->direction);
    shadow->f0c = EdFileReadFloat();
    if (version >= 2) {
      shadow->f10 = EdFileReadFloat();
      shadow->f14 = EdFileReadFloat();
    } else {
      shadow->f10 = 2.0f;
      shadow->f14 = 0.5f;
    }
    if (version >= 3) {
      shadow->f18 = EdFileReadFloat();
      shadow->f1c = EdFileReadFloat();
    } else {
      shadow->f18 = 0.0005f;
      shadow->f1c = 0.01f;
    }
    if (version >= 4)
      shadow->f20 = EdFileReadFloat();
    else
      shadow->f20 = 22.0f;
    if (version >= 5) {
      EdFileReadFloat();
      EdFileReadFloat();
    }
    if (version >= 6) {
      shadow->f28 = EdFileReadFloat();
      shadow->f2c = EdFileReadFloat();
      shadow->f30 = EdFileReadFloat();
    }
    if (version >= 7)
      shadow->f34 = EdFileReadFloat();
    if (version >= 8)
      shadow->preset = EdFileReadInt();
    if (version >= 9) {
      shadow->f24 = EdFileReadFloat();
      if (version == 9) {
        shadow->f24 = 1.8f;
        edGizShadow_SetPreset(shadow, version - 8);
        goto apply;
      }
    } else {
      shadow->f24 = 1.8f;
    }
    if (version < 8) {
      if (world->area != NULL)
        shadow->preset = (world->area->flags & 1) ? 2 : 1;
    } else if (version < 9) {
      shadow->f0c = 0.4f;
      edGizShadow_SetPreset(shadow, 1);
      shadow->preset = 0;
    } else if (version < 11) {
      edGizShadow_SetPreset(shadow, 1);
    } else if (version < 12) {
      if (shadow->f0c < 0.05)
        shadow->f0c = 0.4f;
    }
  apply:
    edGizShadow_SetPreset(shadow, shadow->preset);
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x0066f8b0
void *edGizShadow_ReserveBufferSpace(void *world_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  world->buf104.addr = (world->buf104.addr + 3) & ~3;
  world->shadow_editor = (EDGIZSHADOW *)world->buf104.void_ptr;
  world->buf104.addr += sizeof(EDGIZSHADOW);
  memset(world->shadow_editor, 0, sizeof(EDGIZSHADOW));
  world->shadow_editor->direction.x = -2.0f;
  world->shadow_editor->direction.y = -10.0f;
  world->shadow_editor->direction.z = -2.0f;
  edGizShadow_SetPreset(world->shadow_editor, 1);
  world->shadow_editor->preset = 0;
  return world->shadow_editor;
}

// GLOBAL: LEGOBATMAN 0x00ad27e8
i32 edGizShadow_gizmotype_id;

// FUNCTION: LEGOBATMAN 0x0066f990
ADDGIZMOTYPE *EdGizShadowMachine_RegisterGizmo(i32 type_id) {
  // GLOBAL: LEGOBATMAN 0x00ad27f0
  static ADDGIZMOTYPE addtype;

  addtype = Default_ADDGIZMOTYPE;
  addtype.name = "ShadowEditor";
  addtype.prefix = "";
  addtype.progress_size = 0;
  addtype.fns[0] = (void *)edGizShadow_GetMaxGizmos;
  addtype.fns[1] = (void *)edGizShadow_AddGizmos;
  addtype.fns[2] = NULL;
  addtype.fns[3] = NULL;
  addtype.fns[4] = NULL;
  addtype.fns[5] = NULL;
  addtype.fns[6] = (void *)edGizShadow_GetGizmoName;
  addtype.fns[7] = (void *)edGizShadow_GetOutput;
  addtype.fns[8] = (void *)edGizShadow_GetOutputName;
  addtype.fns[9] = (void *)edGizShadow_GetNumOutputs;
  addtype.fns[10] = NULL;
  addtype.fns[11] = NULL;
  addtype.fns[12] = NULL;
  addtype.fns[13] = NULL;
  addtype.fns[14] = NULL;
  addtype.fns[15] = NULL;
  addtype.fns[16] = NULL;
  addtype.fns[17] = NULL;
  addtype.fns[18] = NULL;
  addtype.fns[19] = NULL;
  addtype.fns[20] = NULL;
  addtype.fns[21] = NULL;
  addtype.fns[22] = NULL;
  addtype.fns[23] = (void *)edGizShadow_ReserveBufferSpace;
  addtype.fns[24] = (void *)edGizShadow_Load;
  edGizShadow_gizmotype_id = type_id;
  return &addtype;
}
