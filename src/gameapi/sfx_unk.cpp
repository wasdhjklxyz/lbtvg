// gameapi/sfx_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nu2api/nucore/common.h"
#include <stdio.h>
#include <string.h>

typedef struct WORLDINFO_s {
  unsigned char pad0[0x80];
  char config_file[0x84]; // 0x80
  variptr_u buf104;       // 0x104
  variptr_u bufEnd108;    // 0x108
  unsigned char pad10c[0x140 - 0x10c];
  struct nugscn_s *scn140; // 0x140
  unsigned char pad144[0x52fc - 0x144];
  struct PUZZLE_s *puzzle; // 0x52fc
} WORLDINFO;

i32 NuFileExists(char *name);
void SpecialSfxLoad(char *path, WORLDINFO *world);

#include "../nu2api/numath/nuvec.h"

void PlaySfxByIdEx(i32 sfx_id, nuvec_s *position, f32 volume, f32 pitch);

// from saga legoapi/audio/sfx.cpp
// FUNCTION: LEGOBATMAN 0x005797f0
void PlaySfxById(i32 sfx_id, nuvec_s *position) {
  PlaySfxByIdEx(sfx_id, position, 1.0f, 1.0f);
}

i32 GetSfxId(const char *name);

// from saga legoapi/audio/sfx.cpp
// FUNCTION: LEGOBATMAN 0x005798a0
void PlaySfx(char *name, struct nuvec_s *position) {
  i32 sfx_id = GetSfxId(name);
  if (sfx_id != -1) {
    PlaySfxById(sfx_id, position);
  }
}

// FUNCTION: LEGOBATMAN 0x00675430
void LoadSpecialSfxFile(WORLDINFO *world) {
  char path[256];
  sprintf(path, "%s.sfx", world->config_file);
  if (NuFileExists(path)) {
    SpecialSfxLoad(path, world);
  }
}

typedef struct nufpar_s {
  u32 pad0[0x910 / 4];
  char *word_buf; // 0x910
} NUFPAR;

NUFPAR *NuFParCreate(char *file);
i32 NuFParGetLine(NUFPAR *parser);
i32 NuFParGetWord(NUFPAR *parser);
i32 NuFParGetInt(NUFPAR *parser);
f32 NuFParGetFloat(NUFPAR *parser);
void NuFParDestroy(NUFPAR *parser);
i32 NuStrICmp(const char *a, const char *b);
i32 CharIDFromName(char *name);
i32 ActionFromName(char *name);

typedef struct CHARPIVOT_s {
  i16 action;       // 0x00
  i16 locator;      // 0x02
  f32 start_frame;  // 0x04
  f32 end_frame;    // 0x08
  i16 character_id; // 0x0c
  u16 pad;
} CHARPIVOT;

// GLOBAL: LEGOBATMAN 0x00ad2a54
static CHARPIVOT *CharPivot;

// FUNCTION: LEGOBATMAN 0x006754c0
void CharPivot_Init(char *file, VARIPTR *buf) {
  i32 count = 0;
  CharPivot = 0;
  if (file == 0)
    return;
  NUFPAR *parser = NuFParCreate(file);
  if (parser == 0)
    return;
  buf->addr = (buf->addr + 3) & ~3;
  CharPivot = (CHARPIVOT *)buf->addr;
  while (NuFParGetLine(parser)) {
    CharPivot[count].start_frame = 0.0f;
    CharPivot[count].end_frame = 0.0f;
    CharPivot[count].character_id = -1;
    CharPivot[count].action = -1;
    CharPivot[count].locator = -1;
    while (NuFParGetWord(parser)) {
      if (NuStrICmp(parser->word_buf, "character") == 0) {
        if (NuFParGetWord(parser))
          CharPivot[count].character_id = CharIDFromName(parser->word_buf);
      } else if (NuStrICmp(parser->word_buf, "action") == 0) {
        if (NuFParGetWord(parser))
          CharPivot[count].action = ActionFromName(parser->word_buf);
      } else if (NuStrICmp(parser->word_buf, "locator") == 0) {
        i32 locator = NuFParGetInt(parser);
        if (locator >= 0 && locator < 20)
          CharPivot[count].locator = locator;
      } else if (NuStrICmp(parser->word_buf, "start_frame") == 0) {
        CharPivot[count].start_frame = NuFParGetFloat(parser);
      } else if (NuStrICmp(parser->word_buf, "end_frame") == 0) {
        CharPivot[count].end_frame = NuFParGetFloat(parser);
      }
    }
    if (CharPivot[count].character_id != -1 && CharPivot[count].action != -1)
      count++;
  }
  NuFParDestroy(parser);
  if (count > 0) {
    CharPivot[count].character_id = -1;
    buf->addr += (count + 1) * sizeof(CHARPIVOT);
  } else {
    CharPivot = 0;
  }
}

struct nugscn_s;
struct nuhspecial_s {
  nugscn_s *scene;
  void *special;
  void *display_special;
};

extern "C" i32 NuMtlSetCurrentRenderPlane(i32 render_plane);
nugscn_s *NuGScnRead(variptr_u *buf, variptr_u buf_end, char *path);
i32 NuSpecialFind(nugscn_s *scene, nuhspecial_s *dest, char *name, i32 flags);
int NuSpecialExistsFn(nuhspecial_s *sp);
extern "C" void *NuSpecialGetMtl(nuhspecial_s *special, int index);

class InteractiveDisplay {
public:
  void InitializeLevel(WORLDINFO_s *world);
  void InitializePerm(char *name, variptr_u *buffer, variptr_u *buffer_end);

  void **vtable;
  u8 pad4[0x10 - 4];
  char level_name[0x224 - 0x10]; // 0x10
  u8 pad224[0x340 - 0x224];
  i32 i340; // 0x340
  u8 pad344[4];
  i32 i348; // 0x348
  f32 f34c; // 0x34c
  f32 f350; // 0x350
  u8 pad354[0x8b0 - 0x354];
  i32 i8b0; // 0x8b0
  u8 pad8b4[0x8bc - 0x8b4];
  f32 f8bc; // 0x8bc
  f32 f8c0; // 0x8c0
  u8 pad8c4[0x8f4 - 0x8c4];
  f32 f8f4; // 0x8f4
  u8 pad8f8[0x930 - 0x8f8];
  f32 f930; // 0x930
  f32 f934; // 0x934
  f32 f938; // 0x938
  u8 pad93c[0x958 - 0x93c];
};

class WorldMapBase : public InteractiveDisplay {
public:
  void InitializeLevel(WORLDINFO_s *world);
  void InitializePerm(char *name, variptr_u *buffer, variptr_u *buffer_end);

  nugscn_s *pointer_scene; // 0x958
  void *pointer_mtls[3];   // 0x95c
  void *p968[3];           // 0x968
  void *material;          // 0x974
};

class DynamicMaterialManager {
public:
  void *GetMaterial(char const *a, char const *b, int c);
};

// GLOBAL: LEGOBATMAN 0x00ad2af8
extern DynamicMaterialManager g_dynamicMaterialManager;
// GLOBAL: LEGOBATMAN 0x009c5a00
extern i32 g_unk009c5a00;
// GLOBAL: LEGOBATMAN 0x00968c74
extern f32 g_unk00968c74;

typedef void(__thiscall *WorldMapPermFn)(WorldMapBase *, char *);

struct PUZZLE_s {
  u8 pad0[8];
  nuhspecial_s pieces[9]; // 0x08
};

// GLOBAL: LEGOBATMAN 0x00a958ac
extern nugscn_s *things_scene;
// GLOBAL: LEGOBATMAN 0x00ad2a58
extern nuhspecial_s g_puzzleTarget;
// GLOBAL: LEGOBATMAN 0x00ad2a64
extern nuhspecial_s g_puzzleGlow;

extern "C" void NuSpecialSetVisibility(void *special_ptr, int visible);

// FUNCTION: LEGOBATMAN 0x006763f0
void Puzzle_PreLoad(void *world_ptr, void *unused) {
  char name[0x20];
  WORLDINFO *world = (WORLDINFO *)world_ptr;
  if (world != NULL && world->puzzle != NULL) {
    i32 i = 0;
    nuhspecial_s *piece = world->puzzle->pieces;
    for (; i < 9; i++, piece++) {
      sprintf(name, "Ac_%d", i + 1);
      if (NuSpecialFind(world->scn140, piece, name, 1))
        NuSpecialSetVisibility(piece, 0);
    }
    if (!NuSpecialExistsFn(&g_puzzleTarget))
      NuSpecialFind(things_scene, &g_puzzleTarget, "Ac_Target", 0);
    if (!NuSpecialExistsFn(&g_puzzleGlow))
      NuSpecialFind(things_scene, &g_puzzleGlow, "Ac_Glow", 0);
  }
}

// STUB: LEGOBATMAN 0x00679ca0
// close: only the scheduling of the leading int/float member stores and the
// name load differs (3 store orders tried)
void WorldMapBase::InitializePerm(char *name, variptr_u *buffer,
                                  variptr_u *buffer_end) {
  f8c0 = 1.0f;
  f8bc = 1.0f;
  f8f4 = 2.0f;
  f934 = 0.5f;
  f930 = 0.5f;
  f938 = 0.0f;
  i348 = 0;
  i8b0 = 0;
  ((WorldMapPermFn)vtable[18])(this, name);
  InteractiveDisplay::InitializePerm(name, buffer, buffer_end);
  material = g_dynamicMaterialManager.GetMaterial(NULL, NULL, 2);
  f34c = 0.005f;
  if (g_unk009c5a00)
    f350 = g_unk00968c74;
  else
    f350 = g_unk00968c74 * 0.5;
  pointer_scene = NULL;
  memset(pointer_mtls, 0, sizeof(pointer_mtls) + sizeof(p968));
  i340 = 1;
}

// vtable slot 22: name of the i-th pointer special
typedef char *(__thiscall *WorldMapSpecialNameFn)(WorldMapBase *, WORLDINFO_s *,
                                                  i32);

// FUNCTION: LEGOBATMAN 0x00678ef0
void WorldMapBase::InitializeLevel(WORLDINFO_s *world) {
  nuhspecial_s special;
  InteractiveDisplay::InitializeLevel(world);
  if (NuStrICmp((char *)world, level_name) == 0) {
    pointer_scene = NULL;
    memset(pointer_mtls, 0, sizeof(pointer_mtls));
    i32 plane = NuMtlSetCurrentRenderPlane(3);
    pointer_scene = NuGScnRead(
        &world->buf104, world->bufEnd108,
        "stuff/interactivedisplay/worldmap/map_pointers/map_pointers.gsc");
    NuMtlSetCurrentRenderPlane(plane);
    for (i32 i = 0; i < 3; i++) {
      NuSpecialFind(pointer_scene, &special,
                    ((WorldMapSpecialNameFn)vtable[22])(this, world, i), 0);
      if (NuSpecialExistsFn(&special))
        pointer_mtls[i] = NuSpecialGetMtl(&special, 0);
    }
  }
}
