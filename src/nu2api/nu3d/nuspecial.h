// nu2api/nu3d/nuspecial.h: the special handle (scene, legacy special, display
// special) that the NuSpecial* API works on. Names and layouts follow
// ref/saga/src/nu2api/nu3d/nuhspecial.h, nuspecial.h and nugscn.h; only
// evidenced fields declared.

#pragma once

#include "../numath/numath.h"

struct nuinstanim_s {
  numtx_s mtx; // 0x00, matrix-first record
};

// Legacy scene instance, 0x50 bytes (saga NuLegacyInstanceLayout).
struct nuinstance_s {
  numtx_s mtx;        // 0x00
  short object_index; // 0x40
  unsigned char pad0[2];
  unsigned int flags;      // 0x44, bit 0 = visible
  nuinstanim_s *animation; // 0x48
  unsigned char pad1[0x50 - 0x4c];
};

// Legacy scene special, 0x50 bytes (saga NuSpecialLegacyLayout).
struct nuspecial_s {
  unsigned char pad0[0x30];
  nuvec_s pos; // 0x30
  unsigned char pad1[0x40 - 0x3c];
  nuinstance_s *instance; // 0x40
  char *name;             // 0x44
  unsigned int flags;     // 0x48, 0x200 = collision
  unsigned char pad2[0x50 - 0x4c];
};

// Per-object bounds (saga NuSpecialLegacyObjectBoundsLayout).
struct nugobject_s {
  unsigned char pad0[0x14];
  float origin_radius; // 0x14
  unsigned char pad1[0x34 - 0x18];
  nuvec_s center; // 0x34
  float radius;   // 0x40
};

// 0xc bytes per entry in nugscn_s::splines.
struct nugspline_s {
  int f0;
  char *name;
  int f8;
};

// Display-list special, 0xd0 bytes (saga NUDISPLAYSPECIAL).
struct NUDISPLAYSPECIAL {
  numtx_s instance_mtx; // 0x00
  numtx_s draw_mtx;     // 0x40
  unsigned char pad0[0xa0 - 0x80];
  nuvec_s center; // 0xa0
  float radius;   // 0xac
  unsigned char pad1[0xb8 - 0xb0];
  unsigned int flags; // 0xb8, 0x200 = collision, bit 1 = visible
  unsigned char pad2[0xc4 - 0xbc];
  nuinstanim_s *instance_animation; // 0xc4, -1 when unset
  unsigned char pad3[0xd0 - 0xc8];
};

struct nugscn_s {
  unsigned char pad0[0x18];
  nugobject_s **objects; // 0x18, indexed by nuinstance_s::object_index
  unsigned char pad1[0x24 - 0x1c];
  int numspecial;        // 0x24
  nuspecial_s *specials; // 0x28
  unsigned char pad2[0x30 - 0x2c];
  int numsplines;       // 0x30
  nugspline_s *splines; // 0x34
  unsigned char pad3[0x110 - 0x38];
  void *display_list; // 0x110, non-zero once specials became display specials
};

struct nuhspecial_s {
  nugscn_s *scene;
  nuspecial_s *special;
  NUDISPLAYSPECIAL *display_special;
};
