#pragma once
// Layout from ref/saga/src/nu2api/numath/nuvec.h.

#include "../nucore/common.h"

typedef struct nuvec_s {
  f32 x;
  f32 y;
  f32 z;
} NUVEC;

f32 NuVecMag(nuvec_s *v);
