#pragma once

#include "../nu2api/nucore/common.h"

typedef struct LEVELDATA_s {
  unsigned char pad0[0x40];
  char name[0x20]; // 0x40
  unsigned char pad60[0x64 - 0x60];
  u32 flags;             // 0x64
  void (*fns[10])(void); // 0x68; per-level callbacks set by Levels_FixUp
  unsigned char pad90[0xd9 - 0x90];
  u8 blob_shadow_fade_near; // 0xd9
  u8 blob_shadow_fade_far;  // 0xda
  unsigned char paddb[0xe0 - 0xdb];
  f32 conveyor_x_speed; // 0xe0
  f32 conveyor_z_speed; // 0xe4
  unsigned char pade8[0xec - 0xe8];
  i16 max_gameantinodes; // 0xec
  u8 padee[0xf2 - 0xee];
  i16 max_pickups;          // 0xf2
  i16 max_obstacle_objects; // 0xf4
  i16 max_buildit_objects;  // 0xf6
  i16 max_force_objects;    // 0xf8
  i16 max_bombgen_objects;  // 0xfa
  u8 max_tightropes;        // 0xfc
  u8 max_timers;            // 0xfd
  u8 max_signals;           // 0xfe
  u8 max_levers;            // 0xff
  u8 max_technos;           // 0x100
  u8 max_zipups;            // 0x101
  u8 max_grapples;          // 0x102
  u8 max_obstacles;         // 0x103
  u8 max_buildits;          // 0x104
  u8 max_shards;            // 0x105
  u8 max_spinners;          // 0x106
  u8 max_minicuts;          // 0x107
  u8 pad108[0x109 - 0x108];
  u8 max_gizspecials;              // 0x109
  u8 max_attractos;                // 0x10a
  u8 max_climb_objects;            // 0x10b
  u8 max_ledges;                   // 0x10c
  u8 max_securitydoors;            // 0x10d
  u8 max_tubes;                    // 0x10e
  u8 max_gizpanels;                // 0x10f
  u8 max_force;                    // 0x110
  u8 max_pushblocks;               // 0x111
  u8 max_pushblock_endpos;         // 0x112
  u8 max_doors;                    // 0x113
  u8 max_teleports;                // 0x114
  u8 max_gizrandoms;               // 0x115
  u8 max_torpmachines;             // 0x116
  u8 max_spinneranim_objs;         // 0x117
  u8 max_turrets;                  // 0x118
  u8 max_bombgens;                 // 0x119
  u8 max_plugs;                    // 0x11a
  u8 max_railcreatures;            // 0x11b
  i16 max_flock_creatures;         // 0x11c
  u8 max_flocks;                   // 0x11e
  u8 max_flock_antinodes;          // 0x11f
  i16 max_dynamic_flock_creatures; // 0x120
  u8 max_dynamic_flocks;           // 0x122
  u8 max_dynamic_flock_antinodes;  // 0x123
  u8 max_whippers;                 // 0x124
  u8 maxdig_objects;               // 0x125
  u8 maxdig;                       // 0x126
  u8 max_puzzles;                  // 0x127
  u8 pad128[0x148 - 0x128];
  f32 slide_speed;         // 0x148
  f32 lateral_slide_speed; // 0x14c
} LEVELDATA;
