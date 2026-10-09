// gameapi/unk_00595ff0.cpp: placed by tools/new.py; file name unproven.

#include "../nu2api/nucore/common.h"
#include <stddef.h>

struct CHARACTERANIM_s {
  u32 pad00;
  u32 flags; // 0x04
};

struct CHARACTERMODEL_s {
  u32 pad00[2];
  void **model_data_a; // 0x08
  void **model_data_b; // 0x0c
};

// FUNCTION: LEGOBATMAN 0x00595ff0
u32 AnimFlags(CHARACTERMODEL_s *model, i32 animation) {
  if (animation != -1 && model->model_data_b[animation] != 0)
    return ((CHARACTERANIM_s *)model->model_data_a[animation])->flags;
  return 0;
}
