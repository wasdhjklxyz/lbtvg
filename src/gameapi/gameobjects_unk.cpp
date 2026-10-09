// gameapi/gameobjects_unk.cpp: placed by tools/new.py; file name unproven.

#include "../batman/worldinfo_unk.h"
#include "../nu2api/nu3d/nuspecial.h"
#include "../nu2api/nucore/common.h"
#include <string.h>

struct CHARPLATFORM_s {
  nuhspecial_s special; // 0x00
  i16 object_id;        // 0x0c
  i16 platform_id;      // 0x0e
  GameObject_s *object; // 0x10
};

struct CHARPLATFORMSYS_s {
  i32 field_0x00;
  i32 platform_count;          // 0x04
  CHARPLATFORM_s platforms[1]; // 0x08
};

// GLOBAL: LEGOBATMAN 0x00aca574
extern i32 VehicleArea;
// GLOBAL: LEGOBATMAN 0x0095fd44
extern u32 LAYER_HOVERIGNORE;

void PlatOnOff(i32 index, i32 enabled);
extern "C" f32 NewShadowEx(nuvec_s *position, f32 unk, f32 height_above,
                           f32 height_below, i32 terrain_mask);

// FUNCTION: LEGOBATMAN 0x005aefd0
f32 GameShadow(GameObject_s *object, nuvec_s *position, f32 probe_height,
               i32 terrain_mask) {
  i32 disabled_platforms[16];
  i32 disabled_platform_count = 0;

  if (object != 0) {
    WORLDINFO_s *world = WorldInfo_CurrentlyActive();
    if (object->platform_id != -1)
      disabled_platforms[disabled_platform_count++] = object->platform_id;

    if (world != 0 && world->char_platform_sys != 0 && VehicleArea == 0 &&
        (object->flags1fc & 0x80) == 0) {
      CHARPLATFORMSYS_s *system = world->char_platform_sys;
      for (i32 i = 0; i < system->platform_count; i++) {
        if (system->platforms[i].object != 0)
          disabled_platforms[disabled_platform_count++] =
              system->platforms[i].object->platform_id;
      }
    }

    for (i32 i = 0; i < disabled_platform_count; i++)
      PlatOnOff(disabled_platforms[i], 0);

    if (LAYER_HOVERIGNORE != 0xffffffff && object->p54->p24->fbc == 0.0f)
      terrain_mask &= ~LAYER_HOVERIGNORE;
  }

  f32 shadow_height =
      NewShadowEx(position, 0.0f, probe_height, probe_height, terrain_mask);

  if (object != 0 && object->platform_id != -1)
    PlatOnOff(object->platform_id, 1);
  for (i32 i = 0; i < disabled_platform_count; i++)
    PlatOnOff(disabled_platforms[i], 1);
  return shadow_height;
}

// FUNCTION: LEGOBATMAN 0x005b0f40
void *GameBufferAlloc(variptr_u *buf, variptr_u *buf_end, i32 size) {
  void *ptr = 0;
  if (buf != 0 && buf_end != 0 && buf->addr + size < buf_end->addr) {
    ptr = (void *)((buf->addr + 15) & ~15);
    buf->addr = ((buf->addr + 15) & ~15) + size;
    if (ptr != 0) {
      memset(ptr, 0, size);
    }
  }
  return ptr;
}

struct EQUIVALENTOBJECTGROUP_s {
  i16 object_count;
  i16 byte_size;
  nuhspecial_s objects[];
};

i32 NuSpecialExistsFn(void *special);

i32 NuSpecialCompare(nuhspecial_s *first, nuhspecial_s *second);

i32 NuSpecialGetVisibilityFn(void *special);

struct SOCKPOSITION_s {
  u8 unk0;
  i8 sock; // 0x01
};

// GLOBAL: LEGOBATMAN 0x00ab3960
extern GameObject_s *Player[8];
// GLOBAL: LEGOBATMAN 0x00960894
extern WORLDINFO_s *WORLD;

void NuVecAdd(nuvec_s *out, nuvec_s *a, nuvec_s *b);
void NuVecScale(nuvec_s *out, nuvec_s *v, float s);
void ComplexSockPosition(SOCKSYS_s *sys, nuvec_s *pos, i32 a, i32 b,
                         SOCKPOSITION_s *out);

struct RememberLevelData_s {
  u8 pad0[0x64];
  u32 flags; // 0x64
};

struct RememberCharData_s {
  u8 pad0[4];
  u32 model_flags; // 0x04
  u8 pad8[0x48 - 8];
};

struct RememberGameCharData_s {
  u8 pad0[0x148];
  u32 flags148; // 0x148
  u8 pad14c[0x22b - 0x14c];
  u8 b22b; // 0x22b
  u8 pad22c[0x240 - 0x22c];
};

// GLOBAL: LEGOBATMAN 0x00acb6c0
extern i32 g_unk00acb6c0;
// GLOBAL: LEGOBATMAN 0x00aca574
extern i32 VehicleArea;
// GLOBAL: LEGOBATMAN 0x00ab0950
extern i32 g_unk00ab0950;
// GLOBAL: LEGOBATMAN 0x00acb81c
extern RememberCharData_s *CDataList;
// GLOBAL: LEGOBATMAN 0x00acb82c
extern RememberGameCharData_s *g_unk00acb82c;
// GLOBAL: LEGOBATMAN 0x0096067c
extern i32 PlayerID[2];
// GLOBAL: LEGOBATMAN 0x00960684
extern i32 g_unk00960684[2];
// GLOBAL: LEGOBATMAN 0x0096068c
extern i32 g_unk0096068c[2];

i32 Collection_Got(i32 id);

// GLOBAL: LEGOBATMAN 0x00a958a4
extern variptr_u things_buffer;
// GLOBAL: LEGOBATMAN 0x00a958a8
extern variptr_u things_buffer_end;
// GLOBAL: LEGOBATMAN 0x00a958ac
extern nugscn_s *things_scene;
// GLOBAL: LEGOBATMAN 0x00a958b0
extern void *things_terrain;

extern "C" i32 NuMtlSetCurrentRenderPlane(i32 render_plane);
int NuStrCpy(char *dst, const char *src);
extern "C" void edbitsRegisterThingsScene(struct nugscn_s *scene);
extern "C" void *TerrainInitEx(i32 level_idx, variptr_u *buffer,
                               variptr_u buffer_end, i32 a, char *path,
                               void *gscn, i32 b, u32 groups, u32 groups2,
                               u32 platforms);
void Unk00564210(char *path, void **terrain);

// FUNCTION: LEGOBATMAN 0x005b2a40
void LoadThingsScene() {
  char path[0x40];
  things_buffer.addr = (things_buffer.addr + 3) & ~3;
  i32 plane = NuMtlSetCurrentRenderPlane(0);
  things_scene =
      NuGScnRead(&things_buffer, things_buffer_end, "stuff\\things.gsc");
  NuStrCpy(path, "stuff\\things");
  NuMtlSetCurrentRenderPlane(plane);
  if (things_scene != NULL) {
    edbitsRegisterThingsScene(things_scene);
    if (things_scene->display_list != NULL)
      *(u32 *)((u8 *)things_scene->display_list + 0x74) |= 0x10;
  }
  things_buffer.addr = (things_buffer.addr + 3) & ~3;
  things_terrain = TerrainInitEx(-1, &things_buffer, things_buffer_end, 0, path,
                                 things_scene, 0, 0x14, 0x14, 0x14);
  Unk00564210(path, &things_terrain);
}

// FUNCTION: LEGOBATMAN 0x005c16b0
void SetPlayerIDs(i32 id0, i32 id1) {
  PlayerID[0] = id0;
  PlayerID[1] = id1;
  if (id0 != -1) {
    u32 flags = g_unk00acb82c[id0].flags148;
    if (flags & 0x100000)
      g_unk00960684[0] = id0;
    else if (flags & 0x200000)
      g_unk0096068c[0] = id0;
  }
  if (id1 != -1) {
    u32 flags = g_unk00acb82c[id1].flags148;
    if (flags & 0x100000)
      g_unk00960684[1] = id1;
    else if (flags & 0x200000)
      g_unk0096068c[1] = id1;
  }
}

// FUNCTION: LEGOBATMAN 0x005c1730
void RememberPlayerIDs(i32 a, i32 b, i32 c) {
  if (g_unk00acb6c0 != 0 || VehicleArea != 0 || g_unk00ab0950 != 0) {
    return;
  }
  if (a == 0) {
    u32 flags = ((RememberLevelData_s *)g_unk00960894->current_level)->flags;
    if (!(flags & 2) || (flags & 0x4e0)) {
      return;
    }
  }
  i32 ids[2];
  ids[0] = b;
  ids[1] = c;
  for (i32 i = 0; i < 2; i++) {
    i32 id = ids[i];
    if (id != -1 && !(CDataList[id].model_flags & 0x2000) &&
        id != PlayerID[i]) {
      i32 got = Collection_Got(id);
      if (got != 0 && got == 1) {
        if (g_unk00acb82c[id].b22b != 0 && id != PlayerID[(i - 1) & 1]) {
          PlayerID[i] = id;
          if (g_unk00acb82c[id].flags148 & 0x100000) {
            g_unk00960684[i] = id;
          } else if (g_unk00acb82c[id].flags148 & 0x200000) {
            g_unk0096068c[i] = id;
          }
        }
      }
    }
  }
  if (b != c && b == PlayerID[1] && c == PlayerID[0]) {
    i32 tmp = PlayerID[0];
    PlayerID[0] = PlayerID[1];
    PlayerID[1] = tmp;
  }
}

// FUNCTION: LEGOBATMAN 0x005c18f0
i32 Players_AveragePos(nuvec_s *position, SOCKPOSITION_s *socket_position) {
  nuvec_s total = {0.0f, 0.0f, 0.0f};
  f32 player_count = 0.0f;

  for (i32 i = 0; i < 2; i++) {
    if (Player[i] != 0 && (Player[i]->flags1fc & 0x80)) {
      NuVecAdd(&total, &total, &Player[i]->position);
      player_count += 1.0f;
    }
  }

  if (player_count > 0.0f) {
    NuVecScale(position, &total, 1.0f / player_count);
    if (socket_position != 0) {
      ComplexSockPosition(WORLD->sock_sys, position, -1, -1, socket_position);
    }
    return 1;
  }

  if (socket_position != 0) {
    socket_position->sock = -1;
  }
  return 0;
}

struct Unk00ad2110 {
  u32 pad0[0x28 / 4];
  f32 f28; // 0x28
};

// GLOBAL: LEGOBATMAN 0x00ab3980
extern GameObject_s *player;
// GLOBAL: LEGOBATMAN 0x00ab3984
extern GameObject_s *player2;
// GLOBAL: LEGOBATMAN 0x00ad2110
extern Unk00ad2110 *g_unk00ad2110;

// FUNCTION: LEGOBATMAN 0x005c19c0
void SetPlayer() {
  if (Player[0] != 0 && (Player[0]->flags1fc & 0x80)) {
    player = Player[0];
    if (Player[0]->flags1418 & 0x800)
      player = Player[0]->p1158;
    if (Player[1] != 0 && (Player[1]->flags1fc & 0x80)) {
      player2 = Player[1];
      if (Player[1]->flags1418 & 0x800)
        player2 = Player[1]->p1158;
    } else {
      player2 = 0;
    }
  } else if (Player[1] != 0 && (Player[1]->flags1fc & 0x80)) {
    player = Player[1];
    player2 = 0;
    if (Player[1]->flags1418 & 0x800)
      player = Player[1]->p1158;
  } else {
    player = 0;
  }
  if (g_unk00ad2110 != 0) {
    f32 v = player2 != 0 ? 1.0f : 0.0f;
    g_unk00ad2110->f28 = v;
  }
}

typedef struct nufpar_s {
  u8 pad0[0x910];
  char *word_buf; // 0x910
} NUFPAR;

NUFPAR *NuFParCreateMem(char *name, char *buffer, i32 bufferSize);
i32 NuFParGetLine(NUFPAR *parser);
i32 NuFParGetWord(NUFPAR *parser);
void NuFParDestroy(NUFPAR *parser);
i32 NuStrICmp(const char *a, const char *b);
i32 NuFParGetInt(NUFPAR *parser);

typedef struct PORTALDOOR_s {
  nuhspecial_s special; // 0x00
  u16 flags;            // 0x0c
  u8 portal_id;         // 0x0e
  u8 pad_0f;
} PORTALDOOR;

// from saga legoapi/gizmos/transport/gizportal.cpp
// FUNCTION: LEGOBATMAN 0x005c8a00
void PortalDoors_Configure(WORLDINFO_s *world, char *config) {
  world->portal_doors = NULL;
  world->portal_door_count = 0;
  if (world->scn140 == NULL)
    return;

  NUFPAR *parser = NuFParCreateMem("portaldoors", config, 0xffff);
  if (parser == NULL)
    return;

  world->buf104.addr = (world->buf104.addr + 3) & ~3;
  PORTALDOOR *portal_door = (PORTALDOOR *)world->buf104.void_ptr;
  world->portal_doors = portal_door;

  while (NuFParGetLine(parser) != 0) {
    NuFParGetWord(parser);
    if (NuStrICmp(parser->word_buf, "portaldoor") != 0)
      continue;

    memset(portal_door, 0, sizeof(PORTALDOOR));
    if (NuFParGetWord(parser) == 0 ||
        NuSpecialFind(world->scn140, &portal_door->special, parser->word_buf,
                      1) == 0)
      continue;

    portal_door->portal_id = (u8)NuFParGetInt(parser);
    while (NuFParGetWord(parser) != 0) {
      if (NuStrICmp(parser->word_buf, "trigger_at_end") == 0) {
        portal_door->flags |= 1;
      }
    }

    ++world->portal_door_count;
    ++portal_door;
  }

  NuFParDestroy(parser);
  if (world->portal_door_count > 0)
    world->buf104.addr = ((u32)portal_door + 15) & ~15;
  else
    world->portal_doors = NULL;
}

nuinstanim_s *NuSpecialGetInstAnim(nuhspecial_s *special);
f32 NuAnimEndFrameOld(void *animation);
void NuPortalSetActive(nugscn_s *scene, i32 portal_id, i32 active);

// FUNCTION: LEGOBATMAN 0x005c8b60
void PortalDoors_Update(WORLDINFO_s *world) {
  PORTALDOOR *door = world->portal_doors;
  if (door == NULL)
    return;

  for (i32 i = 0; i < world->portal_door_count; i++, door++) {
    nuinstanim_s *anim = NuSpecialGetInstAnim(&door->special);
    if (anim == NULL)
      continue;
    f32 end_frame = NuAnimEndFrameOld(
        door->special.scene->instance_animation_data[anim->anim_ix]);
    u16 flags = door->flags;
    bool closed;
    if (flags & 1)
      closed = !(anim->ltime >= end_frame);
    else
      closed = !(anim->ltime > 1.0f);
    if (!closed) {
      if (!(flags & 2)) {
        NuPortalSetActive(world->scn140, door->portal_id, 1);
        door->flags = (door->flags & ~4) | 2;
      }
    } else if ((flags & 2) || !(flags & 4)) {
      NuPortalSetActive(world->scn140, door->portal_id, 0);
      door->flags = (door->flags & ~2) | 4;
    }
  }
}

// from saga legoapi/items/objects/objectsall.cpp
// FUNCTION: LEGOBATMAN 0x005c8c90
void EquivalentObjects_Configure(WORLDINFO_s *world, char *config) {
  world->equivalent_groups = NULL;
  world->equivalent_group_count = 0;
  if (world->scn140 == NULL)
    return;

  NUFPAR *parser = NuFParCreateMem("equivalentobjects", config, 0xffff);
  if (parser == NULL)
    return;

  world->buf104.addr = (world->buf104.addr + 3) & ~3;
  u8 *cursor = world->buf104.u8_ptr;
  world->equivalent_groups = (EQUIVALENTOBJECTGROUP_s *)cursor;
  while (NuFParGetLine(parser) != 0) {
    NuFParGetWord(parser);
    if (NuStrICmp(parser->word_buf, "equivalentobjects") != 0)
      continue;

    EQUIVALENTOBJECTGROUP_s *group = (EQUIVALENTOBJECTGROUP_s *)cursor;
    group->object_count = 0;
    group->byte_size = 4;
    while (NuFParGetWord(parser) != 0) {
      if (NuSpecialFind(world->scn140, &group->objects[group->object_count],
                        parser->word_buf, 1) != 0) {
        ++group->object_count;
        group->byte_size += sizeof(nuhspecial_s);
      }
    }
    if (group->object_count > 0) {
      cursor += group->byte_size;
      ++world->equivalent_group_count;
    }
  }
  NuFParDestroy(parser);
  if (world->equivalent_group_count > 0)
    world->buf104.addr = ((u32)cursor + 15) & ~15;
  else
    world->equivalent_groups = NULL;
}

// FUNCTION: LEGOBATMAN 0x005c8de0
i32 EquivalentObject_Find(WORLDINFO_s *world, nuhspecial_s *special) {
  if (special != 0 && NuSpecialExistsFn(special)) {
    EQUIVALENTOBJECTGROUP_s *group = world->equivalent_groups;
    if (group != 0) {
      for (i32 group_index = 0; group_index < world->equivalent_group_count;
           ++group_index) {
        for (i32 i = 0; i < group->object_count; ++i) {
          if (NuSpecialCompare(special, &group->objects[i])) {
            for (i32 j = 0; j < group->object_count; ++j) {
              if (NuSpecialGetVisibilityFn(&group->objects[j])) {
                *special = group->objects[j];
                return 1;
              }
            }
            return 0;
          }
        }
        group = (EQUIVALENTOBJECTGROUP_s *)((u8 *)group + group->byte_size);
      }
    }
  }
  return 0;
}
