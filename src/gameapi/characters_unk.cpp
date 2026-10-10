// gameapi/characters_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nu2api/nucore/common.h"
#include "../nu2api/nucore/nustring.h"
#include "../nu2api/numath/nuinline_unk.h"
#include <stddef.h>

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x0061f0e0
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

typedef struct CHARACTERDATA_s {
  u32 pad0;
  u32 model_flags; // 0x04
  u32 pad8;
  char *file; // 0x0c
  u32 pad10[2];
  void *move_fn;    // 0x18
  void *animate_fn; // 0x1c
  void *draw_fn;    // 0x20
  u32 pad24[(0x48 - 0x24) / 4];
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

typedef struct GCDATA_s {
  u8 pad0[0x13c];
  u32 flags13c;  // 0x13c
  u32 abilities; // 0x140
  u8 pad144[0x22b - 0x144];
  i8 movement_type; // 0x22b
  u8 pad22c[0x240 - 0x22c];
} GCDATA;

// GLOBAL: LEGOBATMAN 0x00acb82c
extern GCDATA *GCDataList;

// Batman's form of saga legoapi/characters/motion/move.cpp: one loop, every
// test optional.
// FUNCTION: LEGOBATMAN 0x0061fd50
void SetMoveAndAnimateFunctions(u32 model_flag_mask, u32 model_flag_value,
                                u32 game_flag_mask, u32 game_flag_value,
                                i32 movement_type, void *move_function,
                                void *animate_function, void *draw_function) {
  for (i32 i = 0; i < CHARCOUNT; i++) {
    if ((model_flag_mask == 0 ||
         (CDataList[i].model_flags & model_flag_mask) == model_flag_value) &&
        (game_flag_mask == 0 ||
         (GCDataList[i].flags13c & game_flag_mask) == game_flag_value) &&
        (movement_type == -1 || GCDataList[i].movement_type == movement_type)) {
      if (move_function != 0)
        CDataList[i].move_fn = move_function;
      if (animate_function != 0)
        CDataList[i].animate_fn = animate_function;
      if (draw_function != 0)
        CDataList[i].draw_fn = draw_function;
    }
  }
}

struct APICHARACTERMODELLIST_s {
  i16 model_id; // 0x00, -1 ends the list
  u8 load;      // 0x02, 2 = always
  u8 pad03;
};

typedef struct AREADATA_s {
  u8 pad0[0x7c];
  u8 flags7c; // 0x7c, 0x40: keep area models during cutscenes
  u8 pad7d[0xbc - 0x7d];
} AREADATA;

// GLOBAL: LEGOBATMAN 0x00aca554
extern AREADATA *ADataList;

i32 CutScenePlayer_Active(void);
void APILoadCharacterModels(APICHARACTERMODELLIST_s *list, i32 append,
                            VARIPTR *buf, VARIPTR buf_end, i32 area_models,
                            void *a, void *b, i32 c);

// Batman's form of saga legoapi/items/objects/gameobjects.cpp (two more
// pass-through arguments, an area flag instead of HUB_ADATA); same as the
// Mac's.
// FUNCTION: LEGOBATMAN 0x0061fe10
void GameLoadCharacterModels(APICHARACTERMODELLIST_s *list, i32 append,
                             VARIPTR *buf, VARIPTR *buf_end, i32 area_models,
                             i32 area, void *a, void *b) {
  if (area_models != 0) {
    i32 strip;
    APICHARACTERMODELLIST_s *model;

    if (CutScenePlayer_Active() != 0 && area != -1 &&
        !(ADataList[area].flags7c & 0x40))
      strip = 1;
    else
      strip = 0;
    for (model = list; model->model_id != -1; model++) {
      if (model->load != 2 && strip)
        model->load = 0;
    }
  }
  APILoadCharacterModels(list, append, buf, *buf_end, area_models, a, b, 1);
}

i32 Collection_Got(i32 id);

// FUNCTION: LEGOBATMAN 0x0061fea0
i32 Ability_InModelList(APICHARACTERMODELLIST_s *list, u32 ability,
                        i32 *index) {
  if (list != NULL) {
    i32 i = 0;
    for (; list->model_id != -1; list++, i++) {
      if ((GCDataList[list->model_id].abilities & ability) == ability &&
          Collection_Got(list->model_id)) {
        if (index != NULL)
          *index = i;
        return 1;
      }
    }
  }
  return 0;
}

typedef struct CHARVARIANT_s {
  char *name;
} CHARVARIANT;

// GLOBAL: LEGOBATMAN 0x00acb848
CHARVARIANT *CharVariants;
// GLOBAL: LEGOBATMAN 0x00acb84c
i32 CharVariantCount;

// FUNCTION: LEGOBATMAN 0x0061ff20
void CharVariants_Init(CHARVARIANT *variants, i32 count) {
  if (variants != NULL && count > 0) {
    CharVariants = variants;
    CharVariantCount = count;
  }
}

// FUNCTION: LEGOBATMAN 0x0061ff40
i32 CharVariant_Find(char *name) {
  if (CharVariants != NULL) {
    for (i32 i = 0; i < CharVariantCount; i++) {
      if (NuStrICmp(CharVariants[i].name, name) == 0)
        return i;
    }
  }
  return -1;
}

typedef struct EXTRAMODEL_s {
  i16 *required; // 0x00, needs this model loaded
  i32 type;      // 0x04, or any model of this movement type
  i16 *list;     // 0x08
} EXTRAMODEL;

// Inlined into ExtraModels_MakeList, emitted all the same.
// FUNCTION: LEGOBATMAN 0x0061ff90
static i32 IdInList(i32 id, i16 *list, i32 count) {
  for (i32 i = 0; i < count; i++) {
    if (list[i] == id)
      return 1;
  }
  return 0;
}

// Batman's form of the Mac's ExtraModels_MakeList.
// FUNCTION: LEGOBATMAN 0x0061ffc0
void ExtraModels_MakeList(APICHARACTERMODELLIST_s *models, i32 count,
                          EXTRAMODEL *extra, i16 *out, i32 max) {
  i32 n;
  i32 i;
  i16 id;

  if (models == NULL || count < 1 || extra == NULL || out == NULL || max < 2)
    return;
  n = 0;
  while (n < max - 1) {
    if (extra->required == NULL && extra->type == -1)
      break;
    if (extra->list != NULL && (id = *extra->list) != -1 &&
        !IdInList(id, out, n)) {
      if (extra->required != NULL) {
        if (*extra->required != -1) {
          for (i = 0; i < count; i++) {
            if (models[i].model_id == *extra->required) {
              out[n++] = id;
              break;
            }
          }
        }
      } else {
        for (i = 0; i < count; i++) {
          if (GCDataList[models[i].model_id].movement_type == extra->type) {
            out[n++] = id;
            break;
          }
        }
      }
    }
    extra++;
  }
  out[n] = -1;
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
