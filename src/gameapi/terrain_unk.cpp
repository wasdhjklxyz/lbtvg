// gameapi/terrain_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nu2api/nucore/common.h"

typedef struct LEVELDATA_s {
  unsigned char pad0[0x64];
  u32 flags; // 0x64
  unsigned char pad68[0xa2 - 0x68];
  u16 max_ter_platforms; // 0xa2
  u16 max_ter_groups;    // 0xa4
} LEVELDATA;

typedef struct WORLDINFO_s {
  unsigned char pad0[0x80];
  char config_file[0x84]; // 0x80
  u32 giz_buffer;         // 0x104
  void *giz_buffer_end;   // 0x108
  unsigned char pad10c[0x120 - 0x10c];
  i32 level_idx; // 0x120
  unsigned char pad124[0x12c - 0x124];
  LEVELDATA *current_level; // 0x12c
  unsigned char pad130[0x140 - 0x130];
  void *current_gscn; // 0x140
  unsigned char pad144[0x2964 - 0x144];
  void *terrain; // 0x2964
} WORLDINFO;

void *TerrainInitEx(i32 level_idx, u32 *buffer, void *buffer_end, i32 a,
                    char *path, void *gscn, i32 b, u32 groups, u32 groups2,
                    u32 platforms);

// FUNCTION: LEGOBATMAN 0x005c8eb0
void LoadTerrainFile(WORLDINFO *world) {
  world->terrain = 0;
  if ((world->current_level->flags & 8) != 0) {
    LEVELDATA *level = world->current_level;
    world->giz_buffer = (world->giz_buffer + 3) & ~3;
    world->terrain = TerrainInitEx(
        world->level_idx, &world->giz_buffer, world->giz_buffer_end, 0,
        world->config_file, world->current_gscn, 0, level->max_ter_groups,
        level->max_ter_groups, level->max_ter_platforms);
  }
}
