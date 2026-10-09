// gameapi/gameobjects_unk.cpp: placed by tools/new.py; file name unproven.

#include "../batman/worldinfo_unk.h"
#include "../nu2api/nu3d/nuspecial.h"
#include "../nu2api/nucore/common.h"
#include <string.h>

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
