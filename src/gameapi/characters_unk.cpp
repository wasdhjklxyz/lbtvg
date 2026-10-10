// gameapi/characters_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nu2api/nucore/common.h"
#include "../nu2api/nucore/nustring.h"
#include "../nu2api/numath/nuinline_unk.h"

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x0061f0e0
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

typedef struct CHARACTERDATA_s {
  u32 pad0[3];
  char *file; // 0x0c
  u32 pad10[(0x48 - 0x10) / 4];
} CHARACTERDATA;

// GLOBAL: LEGOBATMAN 0x00acb820
extern i32 CHARCOUNT;

// GLOBAL: LEGOBATMAN 0x00acb81c
extern CHARACTERDATA *CDataList;

// FUNCTION: LEGOBATMAN 0x0061f190
i32 CharIDFromName(char *name) {
  for (i32 i = 0; i < CHARCOUNT; i++) {
    if (NuStrICmp(CDataList[i].file, name) == 0) {
      return i;
    }
  }

  return -1;
}

struct GameObject_s;
struct numtl_s;
struct numtx_s;

struct HOSEPART_s {
  u8 pad0[0x9a];
  u8 type; // 0x9a, 5 = hose texture
  u8 pad9b[0xb8 - 0x9b];
  u16 tid; // 0xb8
};

struct HOSEPARTS_s {
  u8 pad0[0xc];
  HOSEPART_s **parts; // 0x0c
  i32 count;          // 0x10
};

struct HOSECHARDATA_s {
  u8 pad0[0x204];
  u8 hose_locator; // 0x204, 0xff = none
};

struct HOSEOBJ_s {
  u8 pad0[0x50];
  struct {
    u8 pad0[4];
    HOSEPARTS_s *parts; // 0x04
  } *p50;               // 0x50
  struct {
    u8 pad0[0x24];
    HOSECHARDATA_s *data; // 0x24
  } *p54;                 // 0x54
  u8 pad58[0xb98 - 0x58];
  u8 locator_mtx[0x11d4 - 0xb98]; // 0xb98
  f32 hose_width;                 // 0x11d4
};

struct HOSEMTL_s {
  u8 pad0[0x74];
  u16 tid; // 0x74
};

// GLOBAL: LEGOBATMAN 0x00acb814
extern HOSEMTL_s *g_hoseMtl;
// GLOBAL: LEGOBATMAN 0x00acb818
extern HOSEMTL_s *g_hoseMtlTextured;
// GLOBAL: LEGOBATMAN 0x00963fb4
extern i32 g_hoseUseCharTexture;

void NuMtlUpdate(HOSEMTL_s *mtl);
void DrawHoseEx(HOSECHARDATA_s *data, numtx_s *mtx, HOSEMTL_s *mtl, f32 width);

// FUNCTION: LEGOBATMAN 0x00620370
void DrawHose(GameObject_s *object) {
  HOSEOBJ_s *obj = (HOSEOBJ_s *)object;
  HOSEMTL_s *mtl = g_hoseMtl;
  if (obj != 0 && obj->p54->data->hose_locator != 0xff) {
    for (i32 i = 0; g_hoseUseCharTexture != 0 && i < obj->p50->parts->count;
         i++) {
      HOSEPART_s *part = obj->p50->parts->parts[i];
      if (part->type == 5) {
        g_hoseMtlTextured->tid = part->tid;
        NuMtlUpdate(g_hoseMtlTextured);
        mtl = g_hoseMtlTextured;
      }
    }
    DrawHoseEx(obj->p54->data, (numtx_s *)obj->locator_mtx, mtl,
               obj->hose_width);
  }
}

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_characters_unk(f32 *v, f32 a, i32 i) {
  NuVec4Set(v, a, a, a, a);
}
