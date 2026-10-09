// nu2api/nu3d/nuspecial.h: the special handle (scene, legacy special, display
// special) that the NuSpecial* API works on. Names and layouts follow
// ref/saga/src/nu2api/nu3d/nuhspecial.h, nuspecial.h and nugscn.h; only
// evidenced fields declared.

#pragma once

#include "../numath/numath.h"

struct NUGSPLINE;

struct nuinstanim_s {
  numtx_s mtx; // 0x00, matrix-first record
  unsigned char pad0[0x4c - 0x40];
  float ltime; // 0x4c
  unsigned char pad1[0x5c - 0x50];
  unsigned short anim_ix; // 0x5c, index into nugscn_s::instance_animation_data
};

// Legacy scene instance, 0x50 bytes (saga NuLegacyInstanceLayout).
struct nuinstance_s {
  numtx_s mtx;        // 0x00
  short object_index; // 0x40
  unsigned char pad0[2];
  union {
    unsigned int flags; // 0x44, bit 0 = visible
    struct {
      unsigned int visible : 1;            // bit 0
      unsigned int on_screen : 1;          // bit 1
      unsigned int bit2 : 1;               // bit 2
      unsigned int no_visibility_test : 1; // bit 3
    };
  };
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
  void *app_data;         // 0x4c
};

// Per-object bounds (saga NuSpecialLegacyObjectBoundsLayout).
struct numtllink_s {
  numtllink_s *next; // 0x00
  void *material;    // 0x04
};

struct nugobject_s {
  unsigned char pad0[0xc];
  numtllink_s *materials; // 0x0c
  unsigned char pad10[0x14 - 0x10];
  float origin_radius; // 0x14
  unsigned char pad18[0x1c - 0x18];
  nuvec_s min;    // 0x1c
  nuvec_s max;    // 0x28
  nuvec_s center; // 0x34
  float radius;   // 0x40
  unsigned char pad44[0x4c - 0x44];
  nugobject_s *next; // 0x4c, LOD chain (NuSpecialGetMtl walks to the end)
};

// 0xc bytes per display-special LOD (saga clip_objects).
struct nuclipobject_s {
  int f0;
  int *material_ids; // 0x04
  int f8;
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
  nuvec_s min;          // 0x80
  unsigned char pad8c[0x90 - 0x8c];
  nuvec_s max; // 0x90
  unsigned char pad0[0xa0 - 0x9c];
  nuvec_s center;               // 0xa0
  float radius;                 // 0xac
  nuclipobject_s *clip_objects; // 0xb0
  char *name;                   // 0xb4
  unsigned int flags;           // 0xb8, 0x200 = collision, bit 1 = visible
  float *clip_range;            // 0xbc, LOD distances, 0-terminated
  unsigned char pad2[0xc4 - 0xc0];
  nuinstanim_s *instance_animation; // 0xc4, -1 when unset
  unsigned char pad3[0xcc - 0xc8];
  void *app_data; // 0xcc
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
  unsigned char pad3[0x54 - 0x38];
  void **instance_animation_data; // 0x54
  unsigned char pad4[0x110 - 0x58];
  struct nudisplaylist_s
      *display_list; // 0x110, non-zero once specials became display specials
};

struct nudisplaylist_s {
  unsigned char pad0[0x50];
  void **mtls; // 0x50
};

struct nuhspecial_s {
  nugscn_s *scene;
  nuspecial_s *special;
  NUDISPLAYSPECIAL *display_special;
};
