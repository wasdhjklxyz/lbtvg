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
