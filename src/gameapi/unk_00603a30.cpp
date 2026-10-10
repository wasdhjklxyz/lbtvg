// gameapi/unk_00603a30.cpp: TU of unknown name, found by its header-static
// copies (the functions after them are not matched yet).

#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"

// FUNCTION: LEGOBATMAN 0x00603a30
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_00603a30(f32 *v, f32 a, i32 i) { NuVec4Set(v, a, a, a, a); }

// The rest of this TU is the Mac's GameAnimSet/GameAnimSys code (saga's
// legoapi/characters/motion/gameanim.cpp has a later version of it; the
// Batman layout differs: two system lists, a 0x28-byte set).

#include "../nu2api/nucore/nulist.h"
#include <stddef.h>

struct nuhspecial_s {
  void *scene;
  void *special;
  void *display_special;
};

typedef struct nuinstanim_s {
  u8 pad0[0x40];
  f32 tfactor; // 0x40
  u8 pad44[0x4c - 0x44];
  f32 ltime; // 0x4c
  u32 flags; // 0x50, 1 playing
} NUINSTANIM;

typedef struct GAMEANIMOBJ_s {
  struct GAMEANIMOBJ_s *next;  // 0x00
  nuhspecial_s special;        // 0x04
  nuinstanim_s *instance_anim; // 0x10
  void *animation;             // 0x14
  f32 start_frame;             // 0x18
  f32 end_frame;               // 0x1c
  u32 f0 : 3;                  // 0x20
  u32 check_visibility : 1;    // 0x20 bit 3
  u32 f4 : 12;                 // 0x20
  u32 no_visi_test : 1;        // 0x22 bit 0
  void *object_data;           // 0x24
} GAMEANIMOBJ;

typedef struct GAMEANIMOBJPOOL_s {
  u16 object_data_size; // 0x00
  u16 active_count;     // 0x02
  u16 capacity;         // 0x04
  u16 pad6;             // 0x06
  GAMEANIMOBJ *objects; // 0x08
  GAMEANIMOBJ *free;    // 0x0c
  void *object_data;    // 0x10
} GAMEANIMOBJPOOL;

struct GAMEANIMSET_s;

typedef struct GAMEANIMSYS_s {
  NULISTHDR active;            // 0x00
  NULISTHDR stopped;           // 0x08
  i32 set_count;               // 0x10
  struct GAMEANIMSET_s **sets; // 0x14
} GAMEANIMSYS;

typedef struct GAMEANIMSET_s {
  NULISTLNK links;       // 0x00
  u8 object_count;       // 0x08
  u8 animated_count;     // 0x09
  u16 flags;             // 0x0a: 1, 2 stop requested, 4 active, 8 stopped
  f32 fc;                // 0x0c
  f32 min_frame;         // 0x10
  f32 max_frame;         // 0x14
  i32 state;             // 0x18
  GAMEANIMOBJPOOL *pool; // 0x1c
  GAMEANIMSYS *system;   // 0x20
  GAMEANIMOBJ *objects;  // 0x24
} GAMEANIMSET;

typedef struct GAMEANIMSYSPROGRESS_s {
  i32 count;      // 0x00
  i32 entry_size; // 0x04
  u8 **entries;   // 0x08
} GAMEANIMSYSPROGRESS;

// GLOBAL: LEGOBATMAN 0x00aca6a0
GAMEANIMSYSPROGRESS gameanimsysprogress;
// GLOBAL: LEGOBATMAN 0x00aca6ac
i32 g_unk00aca6ac;

void *GameBufferAlloc(variptr_u *buf, variptr_u *buf_end, i32 size);
extern "C" void NuListAppend(NULISTHDR *list, NULISTLNK *node);
extern "C" void NuListRemove(NULISTHDR *list, NULISTLNK *node);
void NuSpecialSetNoVisiTest(nuhspecial_s *special, i32 enabled);
i32 NuSpecialGetVisibilityFn(nuhspecial_s *sp);

// FUNCTION: LEGOBATMAN 0x00603a50
void GameAnimSys_AllocateLevelProgressData(variptr_u *buf, variptr_u *buf_end,
                                           i32 capacity, i32 level_count) {
  i32 i;

  if (buf == NULL || buf_end == NULL)
    return;
  gameanimsysprogress.count = level_count;
  gameanimsysprogress.entry_size = capacity;
  gameanimsysprogress.entries =
      (u8 **)GameBufferAlloc(buf, buf_end, level_count * sizeof(u8 *));
  if (gameanimsysprogress.entries != NULL) {
    for (i = 0; i < level_count; i++)
      gameanimsysprogress.entries[i] =
          (u8 *)GameBufferAlloc(buf, buf_end, capacity);
  }
}

// FUNCTION: LEGOBATMAN 0x00603ac0
void GameAnimSys_StoreProgress(GAMEANIMSYS *system, i32 index) {
  u8 *progress;
  i32 i;

  if (system == NULL || system->sets == NULL || index < 0 ||
      index >= gameanimsysprogress.count)
    return;
  progress = gameanimsysprogress.entries[index];
  for (i = 0; i < gameanimsysprogress.entry_size && system->sets[i] != NULL;
       i++)
    progress[i] = system->sets[i]->state;
}

// FUNCTION: LEGOBATMAN 0x00603b20
u8 *GameAnimSys_GetProgressData(i32 index) {
  if (index < 0 || index >= gameanimsysprogress.count)
    return NULL;
  return gameanimsysprogress.entries[index];
}

// FUNCTION: LEGOBATMAN 0x00603b40
void GameAnimSys_ClearProgressUnk00603b40(i32 index) {
  u8 *progress;
  i32 i;

  if (index < 0 || index >= gameanimsysprogress.count)
    return;
  progress = gameanimsysprogress.entries[index];
  for (i = 0; i < gameanimsysprogress.entry_size; i++)
    progress[i] = 0;
}

// FUNCTION: LEGOBATMAN 0x00603b80
GAMEANIMSYS *GameAnimSys_Create(variptr_u *buf, variptr_u *buf_end) {
  GAMEANIMSYS *system =
      (GAMEANIMSYS *)GameBufferAlloc(buf, buf_end, sizeof(GAMEANIMSYS));
  if (system != NULL && gameanimsysprogress.entry_size != 0)
    system->sets = (GAMEANIMSET **)GameBufferAlloc(
        buf, buf_end, gameanimsysprogress.entry_size * sizeof(GAMEANIMSET *));
  return system;
}

// FUNCTION: LEGOBATMAN 0x00603bc0
GAMEANIMSET *GameAnimSet_Create(variptr_u *buf, variptr_u *buf_end,
                                GAMEANIMOBJPOOL *object_pool,
                                GAMEANIMSYS *system) {
  GAMEANIMSET *set = NULL;
  if (object_pool != NULL) {
    set = (GAMEANIMSET *)GameBufferAlloc(buf, buf_end, sizeof(GAMEANIMSET));
    if (set != NULL) {
      set->pool = object_pool;
      set->system = system;
      i32 index = system->set_count;
      if (index < gameanimsysprogress.entry_size) {
        system->sets[index] = set;
        system->set_count++;
      }
    }
  }
  return set;
}

// FUNCTION: LEGOBATMAN 0x00603c10
void GameAnimSet_AddToSystemList(GAMEANIMSET *set) {
  if (set != NULL && set->system != NULL) {
    if (set->flags & 8) {
      NuListRemove(&set->system->stopped, &set->links);
      set->flags &= ~8;
    }
    if (!(set->flags & 4)) {
      NuListAppend(&set->system->active, &set->links);
      set->flags |= 4;
    }
  }
}

// FUNCTION: LEGOBATMAN 0x00603c60
static void GameAnimSet_RestoreVisiTestUnk00603c60(GAMEANIMSET *set) {
  GAMEANIMOBJ *obj;

  if (g_unk00aca6ac != 0) {
    for (obj = set->objects; obj != NULL; obj = obj->next)
      NuSpecialSetNoVisiTest(&obj->special, obj->no_visi_test);
  }
}

// FUNCTION: LEGOBATMAN 0x00603c90
static void GameAnimSet_StopUnk00603c90(GAMEANIMSET *set) {
  if (set != NULL && set->system != NULL) {
    if (set->flags & 4) {
      NuListRemove(&set->system->active, &set->links);
      set->flags &= ~4;
      GameAnimSet_RestoreVisiTestUnk00603c60(set);
    }
    if (!(set->flags & 8)) {
      NuListAppend(&set->system->stopped, &set->links);
      set->flags |= 8;
    }
    set->flags = (set->flags & ~1) | 2;
  }
}

// FUNCTION: LEGOBATMAN 0x00603cf0
void GameAnimSet_EvaluateState(GAMEANIMSET *set) {
  GAMEANIMOBJ *obj;
  i32 at_end = 1;
  i32 at_start = 1;
  f32 dir;

  if (set->flags & 4)
    return;
  for (obj = set->objects; obj != NULL; obj = obj->next) {
    if (obj->instance_anim == NULL)
      continue;
    if (obj->check_visibility && !NuSpecialGetVisibilityFn(&obj->special))
      continue;
    dir = obj->start_frame > obj->end_frame ? -1.0f : 1.0f;
    if (obj->end_frame * dir > obj->instance_anim->ltime * dir)
      at_end = 0;
    if (obj->start_frame * dir < obj->instance_anim->ltime * dir)
      at_start = 0;
  }
  set->state = 0;
  if (at_end)
    set->state = 2;
  else if (!at_start)
    set->state = 4;
}

// FUNCTION: LEGOBATMAN 0x00603db0
void GameAnimSet_RemoveFromSystemListUnk00603db0(GAMEANIMSET *set) {
  if (set != NULL && set->system != NULL) {
    if (set->flags & 4) {
      NuListRemove(&set->system->active, &set->links);
      set->flags &= ~5;
      GameAnimSet_RestoreVisiTestUnk00603c60(set);
    }
    GameAnimSet_EvaluateState(set);
  }
}

// STUB: LEGOBATMAN 0x00603df0
// The object_data size product loads its two u16 operands in swapped order.
GAMEANIMOBJPOOL *GameAnimSet_CreateObjectPool(variptr_u *buf,
                                              variptr_u *buf_end,
                                              i32 object_data_size,
                                              i32 capacity) {
  GAMEANIMOBJPOOL *pool = NULL;
  i32 i;

  if (capacity != 0) {
    pool = (GAMEANIMOBJPOOL *)GameBufferAlloc(buf, buf_end,
                                              sizeof(GAMEANIMOBJPOOL));
    if (pool != NULL) {
      pool->capacity = capacity;
      pool->object_data_size = object_data_size;
      pool->objects = (GAMEANIMOBJ *)GameBufferAlloc(
          buf, buf_end, pool->capacity * sizeof(GAMEANIMOBJ));
      if (object_data_size != 0)
        pool->object_data = GameBufferAlloc(
            buf, buf_end, pool->object_data_size * pool->capacity);
      for (i = 0; i < pool->capacity; i++) {
        pool->objects[i].next = pool->free;
        pool->free = &pool->objects[i];
      }
    }
  }
  return pool;
}

// FUNCTION: LEGOBATMAN 0x00603ea0
void GameAnimSet_GetFrameRangeUnk00603ea0(GAMEANIMSET *set) {
  GAMEANIMOBJ *obj;

  if (set != NULL) {
    set->min_frame = 1000000000.0f;
    obj = set->objects;
    set->max_frame = 1.0f;
    set->animated_count = 0;
    for (; obj != NULL; obj = obj->next) {
      if (obj->instance_anim != NULL) {
        if (obj->start_frame < set->min_frame)
          set->min_frame = obj->start_frame;
        if (obj->end_frame > set->max_frame)
          set->max_frame = obj->end_frame;
        set->animated_count++;
      }
    }
    if (set->animated_count == 0)
      set->max_frame = set->min_frame = 0.0f;
  }
}

// FUNCTION: LEGOBATMAN 0x00604730
i32 GameAnimSet_ResetUnk00604730(GAMEANIMSET *set) {
  GAMEANIMOBJ *obj;

  if (set != NULL) {
    for (obj = set->objects; obj != NULL; obj = obj->next) {
      if (obj->instance_anim != NULL) {
        obj->instance_anim->flags &= ~1;
        obj->instance_anim->flags &= ~4;
        obj->instance_anim->ltime = obj->start_frame;
        obj->instance_anim->tfactor = 1.0f;
      }
      GameAnimSet_RemoveFromSystemListUnk00603db0(set);
    }
    set->fc = 0.0f;
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x00604a80
i32 GameAnimSet_StopUnk00604a80(GAMEANIMSET *set) {
  GAMEANIMOBJ *obj;

  if (set != NULL && (set->state == 1 || set->state == 3)) {
    GameAnimSet_StopUnk00603c90(set);
    for (obj = set->objects; obj != NULL; obj = obj->next) {
      if (obj->instance_anim != NULL)
        obj->instance_anim->flags &= ~1;
    }
  }
  return 1;
}
