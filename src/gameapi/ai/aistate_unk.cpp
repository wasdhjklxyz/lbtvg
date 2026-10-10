// gameapi/ai/aistate_unk.cpp: placed by tools/new.py; file name unproven.
// from saga gameapi/ai/aisys/aistate.cpp

#include "../../nu2api/nucore/common.h"
#include "../../nu2api/nucore/nustring.h"
#include "../../nu2api/numath/nuinline_unk.h"
#include "../../nu2api/numath/numtx_inline_unk.h"
#include "../../nu2api/numath/nutrig_unk.h"
#include "aisys_unk.h"
#include <stddef.h>
#include <string.h>

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x006a1370
static void NuVecZeroInline(f32 *v);

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x006a14a0
static f32 NuVecMagInline(f32 *v);

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x006a1550
static void NuMtxCopyInline(f32 *dst, f32 *src);

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x006a10d0
static f32 NuFabs(f32 f);
// FUNCTION: LEGOBATMAN 0x006a10f0
static f32 NuFdiv(f32 a, f32 b);
// FUNCTION: LEGOBATMAN 0x006a1150
static f32 NuFsign(f32 f);
// FUNCTION: LEGOBATMAN 0x006a12c0
static f32 NuSinApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x006a1360
static f32 NuCosApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x006a1380
static void NuVec4Copy(f32 *dst, f32 *src);
// FUNCTION: LEGOBATMAN 0x006a13a0
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);
// FUNCTION: LEGOBATMAN 0x006a1400
static void NuVecScaleInline(f32 *dst, f32 *src, f32 s);

// FUNCTION: LEGOBATMAN 0x006a1950
AISTATE *AIStateFind(char *name, AISCRIPT *script) {
  AISTATE *state;

  state = (AISTATE *)NuListGetHead(&script->states);

  if (name != NULL) {
    while (state != NULL) {
      if (NuStrICmp(name, state->name) == 0) {
        return state;
      }

      state = (AISTATE *)NuListGetNext(&script->states, &state->list_node);
    }
  }

  return NULL;
}

// FUNCTION: LEGOBATMAN 0x006a62b0
i32 AIScriptSetInterrupt(AISCRIPTPROCESS *processor, u8 priority, u8 id,
                         char *state_name, f32 time) {
  AISTATE *state;

  if (priority >= processor->interrupt_priority) {
    state = AIStateFind(state_name, processor->script);

    if (state != NULL) {
      processor->interrupt_priority = priority;
      processor->interrupt_id = id;
      processor->interrupt_state = state;
      processor->interrupt_timer = time;

      return 1;
    }
  }

  return 0;
}

struct AIPATHNODE_s {
  char *name; // 0x00
  u8 pad04[0x5c - 0x04];
};

struct AIPATH_s {
  char name[0x10]; // 0x00
  u8 node_count;   // 0x10
  u8 pad11[0x7c - 0x11];
  AIPATHNODE_s *nodes; // 0x7c
};

struct AIPATHSYS_s {
  u8 path_count; // 0x00
  u8 pad01[3];
  AIPATH_s **paths;      // 0x04
  AIPATH_s *active_path; // 0x08
};

struct AILOCATOR_s {
  char name[0x10]; // 0x00
  u8 pad10[0x44 - 0x10];
};

struct AILOCATORSET_s {
  char name[0x10]; // 0x00
  u8 pad10[0x1c - 0x10];
};

struct AIAREA_s {
  char name[0x10]; // 0x00
  u8 pad10[0x40 - 0x10];
};

struct AISYS_s {
  u8 pad000[0x21c];
  AIPATHSYS_s *path_sys; // 0x21c
  u8 pad220[0x230 - 0x220];
  i32 locator_count;            // 0x230
  AILOCATOR_s *locators;        // 0x234
  i32 locator_set_count;        // 0x238
  AILOCATORSET_s *locator_sets; // 0x23c
  i32 area_count;               // 0x240
  AIAREA_s *areas;              // 0x244
};

// FUNCTION: LEGOBATMAN 0x006a9ac0
AILOCATOR_s *AIPathFindLocator(AISYS_s *sys, char *name) {
  if (sys != NULL) {
    for (i32 i = 0; i < sys->locator_count; ++i) {
      if (NuStrICmp(sys->locators[i].name, name) == 0)
        return &sys->locators[i];
    }
  }
  return NULL;
}

// FUNCTION: LEGOBATMAN 0x006a9b20
AILOCATORSET_s *AIPathFindLocatorSet(AISYS_s *system, char *name) {
  if (system != NULL) {
    for (i32 index = 0; index < system->locator_set_count; ++index) {
      if (NuStrICmp(system->locator_sets[index].name, name) == 0)
        return &system->locator_sets[index];
    }
  }
  return NULL;
}

// FUNCTION: LEGOBATMAN 0x006a9b90
AIAREA_s *AISysFindArea(AISYS_s *sys, char *name) {
  if (sys != NULL) {
    for (i32 i = 0; i < sys->area_count; ++i) {
      AIAREA_s *area = &sys->areas[i];
      if (NuStrICmp(name, area->name) == 0)
        return area;
    }
  }
  return NULL;
}

// FUNCTION: LEGOBATMAN 0x006a9bf0
AIPATH_s *AISysFindPath(AISYS_s *sys, char *name) {
  if (sys != NULL && sys->path_sys != NULL) {
    for (i32 i = 0; i < sys->path_sys->path_count; ++i) {
      if (NuStrICmp(sys->path_sys->paths[i]->name, name) == 0)
        return sys->path_sys->paths[i];
    }
  }
  return NULL;
}

// FUNCTION: LEGOBATMAN 0x006a9c50
AIPATHNODE_s *AIPathFindNode(AISYS_s *system, AIPATH_s *path, char *name) {
  if (path != NULL || (system != NULL && system->path_sys != NULL &&
                       system->path_sys->path_count != 0 &&
                       (path = system->path_sys->active_path) != NULL)) {
    for (i32 index = 0; index < path->node_count; ++index) {
      if (path->nodes[index].name != NULL &&
          NuStrICmp(path->nodes[index].name, name) == 0)
        return &path->nodes[index];
    }
  }
  return NULL;
}

// 0x006a10a0 is a lone `ret` (debug report compiled out).
static void AIDebugUnk006a10a0(...) {}

struct AIPATHCNXTYPE_s {
  u32 connection_flag;   // 0x00
  void *context;         // 0x04
  char name[0x40];       // 0x08
  char short_name[0x20]; // 0x48
  u32 flags;             // 0x68
};

// GLOBAL: LEGOBATMAN 0x00ad4548
static AIPATHCNXTYPE_s aipathcnxtypes[32];

// GLOBAL: LEGOBATMAN 0x00ad52c8
static i32 naipathcnxtypes;

// FUNCTION: LEGOBATMAN 0x006a5fe0
void AISysClearAllPathCnxTypes(void) {
  naipathcnxtypes = 0;
  memset(aipathcnxtypes, 0, sizeof(aipathcnxtypes));
}

// STUB: LEGOBATMAN 0x006a6000
// VC8 drops calls to an empty static unless some call in the TU passes it a
// pointer argument (see NuMtlSetCurrentRenderPlane); needs that caller first.
void AISysRegisterPathCnxType(char *name, char *short_name, u32 connection_flag,
                              void *context, u32 flags) {
  if (name == NULL || strlen(name) >= 0x40 || connection_flag == 0 ||
      naipathcnxtypes >= 32) {
    AIDebugUnk006a10a0();
    return;
  }
  for (i32 i = 0; i < naipathcnxtypes; i++) {
    if (aipathcnxtypes[i].connection_flag & connection_flag)
      AIDebugUnk006a10a0();
  }
  AIPATHCNXTYPE_s *type = &aipathcnxtypes[naipathcnxtypes++];
  strcpy(type->name, name);
  strcpy(type->short_name, short_name);
  type->connection_flag = connection_flag;
  type->context = context;
  type->flags = flags;
}

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_aistate_unk(f32 *v, f32 a, i32 i) {
  v[2] = NuFabs(a);
  v[3] = NuFdiv(a, v[4]);
  v[5] = NuFsign(a);
  v[0] = NuSinApprox(i);
  v[1] = NuCosApprox(i);
  NuVec4Copy(v + 4, v);
  NuVec4Set(v, a, a, a, a);
  NuVecScaleInline(v + 8, v, a);
}

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_2_aistate_unk(f32 *v, f32 a, i32 i) {
  NuMtxCopyInline(v + 32, v + 16);
}

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_3_aistate_unk(f32 *v, f32 a, i32 i) {
  v[48] = NuVecMagInline(v);
}

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_4_aistate_unk(f32 *v, f32 a, i32 i) {
  NuVecZeroInline(v + 80);
}
