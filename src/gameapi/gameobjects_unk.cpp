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
