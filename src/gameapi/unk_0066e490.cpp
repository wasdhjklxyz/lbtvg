// gameapi/unk_0066e490.cpp: TU of unknown name, found by its header-static
// copies (the functions after them are not matched yet).

#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"

// FUNCTION: LEGOBATMAN 0x0066e490
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_0066e490(f32 *v, f32 a, i32 i) { NuVec4Set(v, a, a, a, a); }

// The rest of this TU is the Mac's Plugs file (saga
// legoapi/gizmos/door/plugs.cpp, Plugs_Load .. Plugs_RegisterGizmo
// 0x66f020); Batman's plugs have seven outputs and a bigger progress block.

#include "../batman/leveldata_unk.h"
#include "../batman/worldinfo_unk.h"
#include <stddef.h>
#include <string.h>

typedef struct PLUG_s {
  char name[0x10];  // 0x00
  nuvec_s position; // 0x10
  u8 pad1c[0x26 - 0x1c];
  u8 active : 1;  // 0x26
  u8 visible : 1; // 0x26 bit 1
  u8 plugged : 1; // 0x26 bit 2
  u8 pad27;
  i32 i28; // 0x28
  u16 w2c; // 0x2c
  u8 pad2e[0x44 - 0x2e];
} PLUG;

typedef struct PLUGSYS_s {
  PLUG *plugs; // 0x00
  i32 count;   // 0x04
} PLUGSYS;

typedef struct PLUGPROGRESS_s {
  u32 visible_mask; // 0x00
  u32 active_mask;  // 0x04
  u8 b08[0x40];     // 0x08
  u8 b48[0x80];     // 0x48
} PLUGPROGRESS;

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

i32 NuStrLen(const char *s);
void AddGizmo(GIZMOSYS_s *gizmo_sys, i32 type_id, void *a, void *object);
void *GameBufferAllocProgressUnk005bbaf0(VARIPTR *buf, VARIPTR *buf_end,
                                         i32 size);

// GLOBAL: LEGOBATMAN 0x00967ff4
i32 plug_gizmotype_id = -1;
// GLOBAL: LEGOBATMAN 0x00967ff8
static char *Plug_OutputNames[7] = {"Plugged", "ID1", "ID2",       "ID3",
                                    "ID4",     "ID5", "NotPlugged"};
// GLOBAL: LEGOBATMAN 0x009680c0
static char *Plug_Name = "Plug";

// STUB: LEGOBATMAN 0x0066e780
// the original keeps an 8-byte stack local (a dead store of the count after
// the pops) and uses edi for the size; the local PLUGSYS copy is a guess.
void *Plugs_ReserveBufferSpace(void *world_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  world->plug_sys = NULL;
  if (world->current_level->max_plugs > 0) {
    PLUGSYS sys;
    world->buf104.addr = (world->buf104.addr + 3) & ~3;
    sys.plugs = (PLUG *)world->buf104.void_ptr;
    world->buf104.addr = (world->buf104.addr +
                          world->current_level->max_plugs * sizeof(PLUG) + 3) &
                         ~3;
    world->plug_sys = (PLUGSYS *)world->buf104.void_ptr;
    world->buf104.addr += sizeof(PLUGSYS);
    sys.count = 0;
    *world->plug_sys = sys;
  }
  return world->plug_sys;
}

// FUNCTION: LEGOBATMAN 0x0066e800
void Plug_Activate(GIZMO *gizmo, i32 active) {
  if (gizmo == NULL)
    return;
  PLUG *plug = (PLUG *)gizmo->object;
  if (!plug->active && active != 0)
    plug->w2c = 0;
  plug->active = active != 0;
}

// FUNCTION: LEGOBATMAN 0x0066e830
void Plug_SetVisibility(GIZMO *gizmo, i32 visible) {
  if (gizmo == NULL)
    return;
  PLUG *plug = (PLUG *)gizmo->object;
  plug->visible = visible != 0;
}

// FUNCTION: LEGOBATMAN 0x0066e850
i32 Plug_ActivateRev(GIZMO *gizmo, i32 value, i32 query) {
  if (gizmo != NULL) {
    PLUG *plug = (PLUG *)gizmo->object;
    if (plug != NULL) {
      if (query & 1) {
        if (plug->active)
          return value;
        return value == 0;
      }
      plug->active = value == 0;
    }
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x0066ec30
void Plugs_Draw(void *world, void *unused, f32 dt) {}

// FUNCTION: LEGOBATMAN 0x0066ec40
i32 Plugs_GetMaxGizmos(void *world_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  return world != NULL ? world->current_level->max_plugs : 0;
}

// FUNCTION: LEGOBATMAN 0x0066ec60
void Plugs_AddGizmos(GIZMOSYS_s *gizmo_sys, i32 type_id, void *world_ptr,
                     void *unused) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  if (world == NULL || world->plug_sys == NULL)
    return;
  for (i32 i = 0; i < world->plug_sys->count; i++) {
    if (NuStrLen(world->plug_sys->plugs[i].name) != 0)
      AddGizmo(gizmo_sys, type_id, NULL, &world->plug_sys->plugs[i]);
  }
}

// FUNCTION: LEGOBATMAN 0x0066ecd0
char *Plug_GetGizmoName(GIZMO *gizmo) {
  return gizmo != NULL ? ((PLUG *)gizmo->object)->name : NULL;
}

// FUNCTION: LEGOBATMAN 0x0066ece0
i32 Plug_GetNumOutputs(GIZMO *gizmo) { return 7; }

// FUNCTION: LEGOBATMAN 0x0066ecf0
char *Plug_GetOutputName(GIZMO *gizmo, i32 output_index) {
  if ((u32)output_index <= 6)
    return Plug_OutputNames[output_index];
  return NULL;
}

// FUNCTION: LEGOBATMAN 0x0066eda0
void *Plugs_AllocateProgressData(VARIPTR *buf, VARIPTR *buf_end) {
  return GameBufferAllocProgressUnk005bbaf0(buf, buf_end, sizeof(PLUGPROGRESS));
}

// STUB: LEGOBATMAN 0x0066edc0
// the two mask stores are scheduled around the first memset's pushes
// differently (stores first, stores after, memset form tried).
void Plugs_ClearProgress(void *world, void *progress_ptr) {
  PLUGPROGRESS *progress = (PLUGPROGRESS *)progress_ptr;
  if (progress == NULL)
    return;
  progress->visible_mask = 0xffffffff;
  progress->active_mask = 0xffffffff;
  memset(progress->b48, -1, sizeof(progress->b48));
  memset(progress->b08, 0, sizeof(progress->b08));
}

// FUNCTION: LEGOBATMAN 0x0066eef0
nuvec_s *Plug_GetPos(GIZMO *gizmo) {
  if (gizmo == NULL)
    return NULL;
  return &((PLUG *)gizmo->object)->position;
}

i32 Plugs_Load(void *world, void *unused);
void Plugs_Update(void *world, void *unused, f32 dt);
i32 Plug_GetOutput(GIZMO *gizmo, i32 output_index, i32 b);
void Plugs_StoreProgress(void *world, void *unused, void *progress);
void Plugs_Reset(void *world, void *unused, void *progress);

// FUNCTION: LEGOBATMAN 0x0066f020
ADDGIZMOTYPE *Plugs_RegisterGizmo(i32 type_id) {
  // GLOBAL: LEGOBATMAN 0x00ad26a0
  static ADDGIZMOTYPE addtype;

  addtype = Default_ADDGIZMOTYPE;
  addtype.name = Plug_Name;
  addtype.prefix = "";
  addtype.progress_size = sizeof(PLUGPROGRESS);
  addtype.fns[0] = (void *)Plugs_GetMaxGizmos;
  addtype.fns[1] = (void *)Plugs_AddGizmos;
  addtype.fns[2] = NULL;
  addtype.fns[3] = (void *)Plugs_Update;
  addtype.fns[4] = (void *)Plugs_Draw;
  addtype.fns[5] = NULL;
  addtype.fns[6] = (void *)Plug_GetGizmoName;
  addtype.fns[7] = (void *)Plug_GetOutput;
  addtype.fns[8] = (void *)Plug_GetOutputName;
  addtype.fns[9] = (void *)Plug_GetNumOutputs;
  addtype.fns[10] = (void *)Plug_Activate;
  addtype.fns[11] = (void *)Plug_ActivateRev;
  addtype.fns[12] = (void *)Plug_SetVisibility;
  addtype.fns[13] = NULL;
  addtype.fns[14] = (void *)Plug_GetPos;
  addtype.fns[15] = NULL;
  addtype.fns[16] = NULL;
  addtype.fns[17] = NULL;
  addtype.fns[18] = NULL;
  addtype.fns[19] = (void *)Plugs_AllocateProgressData;
  addtype.fns[20] = (void *)Plugs_ClearProgress;
  addtype.fns[21] = (void *)Plugs_StoreProgress;
  addtype.fns[22] = (void *)Plugs_Reset;
  addtype.fns[23] = (void *)Plugs_ReserveBufferSpace;
  addtype.fns[24] = (void *)Plugs_Load;
  addtype.fns[25] = NULL;
  addtype.fns[26] = NULL;
  addtype.fns[27] = NULL;
  plug_gizmotype_id = type_id;
  return &addtype;
}
