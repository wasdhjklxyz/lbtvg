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

struct TerrainQuery_s {
  u32 pad0[0x28 / 4];
  f32 mx; // 0x28 movement
  f32 my; // 0x2c
  f32 mz; // 0x30
  u32 pad34[(0x44 - 0x34) / 4];
  f32 movement_pitch;  // 0x44
  f32 movement_yaw;    // 0x48
  f32 movement_length; // 0x4c
};

// GLOBAL: LEGOBATMAN 0x009e9564
static TerrainQuery_s *TerI;

i32 NuAtan2DA(f32 dx, f32 dy);
f32 NuFsqrt(f32 f);

// FUNCTION: LEGOBATMAN 0x005672e0
void DerotateMovementVector() {
  TerI->movement_yaw = (f32)NuAtan2DA(TerI->mx, TerI->mz);
  TerI->movement_pitch = (f32)NuAtan2DA(
      -TerI->my, NuFsqrt(TerI->mx * TerI->mx + TerI->mz * TerI->mz));
  TerI->movement_length =
      NuFsqrt(TerI->mx * TerI->mx + TerI->my * TerI->my + TerI->mz * TerI->mz);
}

// STUB: LEGOBATMAN 0x0056fd80
// switch on hit type compiles to a jump table; not attempted
#if 0
#include "../nu2api/numath/nuvec.h"

void TerrainMoveImpactData();

i32 terrhitflags;

void RotateVec(nuvec_s *source, nuvec_s *destination);

// from saga legoapi/render/core/terrain.cpp
void TerrainImpactNorm() {
    TerrainMoveImpactData();

    const u8 maximum_supported_hit_type = TERRAIN_HIT_TYPE_SECOND_NORMAL | TERRAIN_HIT_TYPE_SPHERE;
    if (TerI->hit_type > maximum_supported_hit_type) {
        return;
    }

    const i32 hit_type_flag = 1 << TerI->hit_type;
    const i32 sphere_class_mask = 1 << TERRAIN_HIT_TYPE_SPHERE;
    const i32 rotate_and_mark_mask = sphere_class_mask | (sphere_class_mask << TERRAIN_HIT_TYPE_SECOND_NORMAL);
    const i32 rotated_surface_mask = (1 << TERRAIN_HIT_TYPE_CYLINDER) | (1 << TERRAIN_HIT_TYPE_VERTEX);
    const i32 rotate_mask = rotated_surface_mask | (rotated_surface_mask << TERRAIN_HIT_TYPE_SECOND_NORMAL);
    const i32 face_class_mask = 1 << TERRAIN_HIT_TYPE_FACE;
    const i32 direct_normal_mask = face_class_mask | (face_class_mask << TERRAIN_HIT_TYPE_SECOND_NORMAL);

    if ((hit_type_flag & rotate_and_mark_mask) != 0) {
        terrhitflags |= 4;
    }

    const bool rotated_hit = (hit_type_flag & (rotate_and_mark_mask | rotate_mask)) != 0;
    if (rotated_hit) {
        RotateVec(&TerI->movement_normal, &TerI->movement_normal);
    } else if ((hit_type_flag & direct_normal_mask) == 0) {
        return;
    }

    TerrainQuery_s *query = TerI;
    // Curved hits are produced in collision-height-scaled space and need to
    // be transformed back. Face normals already come from the terrain in
    // object space; the target's direct-face branch deliberately bypasses
    // this scaling before copying the normal below.
    if (rotated_hit && (query->hit_type & TERRAIN_HIT_TYPE_SECOND_NORMAL) == 0) {
        query->movement_normal.x *= query->inverse_collision_radius;
        query->movement_normal.y *= query->inverse_collision_radius;
        query->movement_normal.z *= query->inverse_collision_radius;
    }

    if (query->object_scale == 1.0f) {
        query->impact_normal = query->movement_normal;
        return;
    }

    const f32 normal_length =
        NuFsqrt(query->movement_normal.x * query->movement_normal.x +
                query->movement_normal.y * query->movement_normal.y * query->inverse_object_scale_sq +
                query->movement_normal.z * query->movement_normal.z);
    f32 inverse_normal_length = 0.0f;
    if (normal_length != 0.0f) {
        inverse_normal_length = 1.0f / normal_length;
    }

    query = TerI;
    query->impact_normal.x = query->movement_normal.x * inverse_normal_length;
    query->impact_normal.y = query->movement_normal.y * query->inverse_object_scale * inverse_normal_length;
    query->impact_normal.z = query->movement_normal.z * inverse_normal_length;
}
#endif

// STUB: LEGOBATMAN 0x0056fef0
// skipped: switch on hit_type compiles to a byte-indexed jump table; not
// attempted.
#if 0
#include "../nu2api/numath/nuvec.h"

void TerrainMoveImpactData();

static TerrainQuery_s *TerI;

// from saga legoapi/render/core/terrain.cpp
void RayImpact(NUVEC *movement) {
    TerrainMoveImpactData();
    switch (TerI->hit_type) {
        case 1:
        case 2:
        case 3:
        case 4: {
            TerI->hit_time -= TerI->separation_epsilon;
            if (TerI->hit_time < 0.0f)
                TerI->hit_time = 0.0f;
            f32 time = TerI->hit_time;
            movement->x = TerI->movement.x * time;
            movement->y = TerI->movement.y * time;
            movement->z = TerI->movement.z * time;
            break;
        }
        case 0x11:
        case 0x12:
        case 0x13:
        case 0x14:
            movement->x = 0.0f;
            movement->y = 0.0f;
            movement->z = 0.0f;
            break;
    }
}
#endif

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
