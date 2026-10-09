// gameapi/terrain_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nu2api/nucore/common.h"
#include <stdio.h>
#include <string.h>

typedef struct LEVELDATA_s {
  unsigned char pad0[0x64];
  u32 flags; // 0x64
  unsigned char pad68[0xa2 - 0x68];
  u16 max_ter_platforms; // 0xa2
  u16 max_ter_groups;    // 0xa4
} LEVELDATA;

typedef struct PORTALDOOR_s {
  u32 pad0[3];
  u16 flags; // 0xc
  u16 pade;
} PORTALDOOR;

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
  unsigned char pad2968[0x2adc - 0x2968];
  i32 unk2adc; // 0x2adc
  unsigned char pad2ae0[0x2ae4 - 0x2ae0];
  i32 page_anim;  // 0x2ae4
  i32 page_grass; // 0x2ae8
  unsigned char pad2aec[0x51bc - 0x2aec];
  PORTALDOOR *portal_doors; // 0x51bc
  i32 portal_door_count;    // 0x51c0
} WORLDINFO;

void *TerrainInitEx(i32 level_idx, u32 *buffer, void *buffer_end, i32 a,
                    char *path, void *gscn, i32 b, u32 groups, u32 groups2,
                    u32 platforms);

i32 NuFileExists(char *name);
i32 edanimLoadPage(char *path, void *scene, i32 unk);

// GLOBAL: LEGOBATMAN 0x00ab39e8
extern WORLDINFO WorldInfo[]; // 0xa790 bytes in all

// FUNCTION: LEGOBATMAN 0x005c8130
void WorldInfo_InitOnce(void) { memset(WorldInfo, 0, 0xa790); }

// GLOBAL: LEGOBATMAN 0x00960894
WORLDINFO *WORLD = &WorldInfo[0];

// from saga legoapi/world/world.cpp
// FUNCTION: LEGOBATMAN 0x005c8150
WORLDINFO *WorldInfo_CurrentlyActive(void) { return WORLD; }

// FUNCTION: LEGOBATMAN 0x005c8240
void WorldInfo_LoadObjectAnimFile(WORLDINFO *world) {
  if (world->page_anim == -1) {
    char path[256];
    sprintf(path, "%s.anm", world->config_file);
    if (NuFileExists(path)) {
      world->page_anim =
          edanimLoadPage(path, world->current_gscn, world->unk2adc);
    }
  }
}

// FUNCTION: LEGOBATMAN 0x005c8c50
void PortalDoors_Reset(WORLDINFO *world_info) {
  PORTALDOOR *door = world_info->portal_doors;
  if (door != 0) {
    for (i32 i = 0; i < world_info->portal_door_count; i++, door++) {
      door->flags &= ~6;
    }
  }
}

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

// GLOBAL: LEGOBATMAN 0x00abe184
extern i32 Grass_Available;

i32 edgraLoadPage(char *path, void *scene, void *terrain, u32 *buffer,
                  void **buffer_end);

// FUNCTION: LEGOBATMAN 0x005c8f30
void LoadGrassFile(WORLDINFO *world) {
  char path[256];

  world->page_grass = -1;
  if (Grass_Available) {
    sprintf(path, "%s.gra", world->config_file);
    if (NuFileExists(path)) {
      world->page_grass =
          edgraLoadPage(path, world->current_gscn, world->terrain,
                        &world->giz_buffer, &world->giz_buffer_end);
    }
  }
}
