#pragma once
// Layout from ref/saga/src/nu2api/numath/numtx.h.

#include "nuvec.h"

typedef struct numtx_s {
  f32 m00;
  f32 m01;
  f32 m02;
  f32 m03;
  f32 m10;
  f32 m11;
  f32 m12;
  f32 m13;
  f32 m20;
  f32 m21;
  f32 m22;
  f32 m23;
  f32 m30;
  f32 m31;
  f32 m32;
  f32 m33;
} NUMTX;

// GLOBAL: LEGOBATMAN 0x009696f0
extern numtx_s numtx_identity;
