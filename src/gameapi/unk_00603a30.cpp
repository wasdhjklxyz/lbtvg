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
#include "../nu2api/numath/numtx.h"
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
  union {
    u32 flags; // 0x50, 1 playing
    struct {
      u32 playing : 1;
      u32 b1 : 1;
      u32 b2 : 1;
      u32 repeating : 1; // bit 3
    };
  };
  f32 fparam1; // 0x54
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
extern "C" void NuSpecialSetVisibility(nuhspecial_s *sp, i32 visible);
struct numtx_s *NuSpecialGetMtx(nuhspecial_s *sp);
void NuSpecialSetDrawMtx(nuhspecial_s *sp, struct numtx_s *mtx);
f32 GameShadow(void *object, nuvec_s *position, f32 probe_height, i32 a);
// GLOBAL: LEGOBATMAN 0x009ca98c
extern f32 g_unk009ca98c;
void SetAnimFrame(nuhspecial_s *special, f32 frame);

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

// FUNCTION: LEGOBATMAN 0x006040c0
void GameAnimSet_RemoveObject(GAMEANIMSET *set, GAMEANIMOBJ *object) {
  GAMEANIMOBJ *p;

  if (set != NULL && object != NULL) {
    if (object == set->objects) {
      set->objects = object->next;
    } else {
      for (p = set->objects; p != NULL; p = p->next) {
        if (p->next == object) {
          p->next = object->next;
          break;
        }
      }
    }
    object->next = NULL;
    set->object_count--;
    set->pool->active_count--;
    object->next = set->pool->free;
    set->pool->free = object;
    GameAnimSet_GetFrameRangeUnk00603ea0(set);
  }
}

// FUNCTION: LEGOBATMAN 0x00604120
void GameAnimSet_RemoveAllObjects(GAMEANIMSET *set) {
  if (set != NULL) {
    while (set->objects != NULL)
      GameAnimSet_RemoveObject(set, set->objects);
  }
}

// FUNCTION: LEGOBATMAN 0x00604150
void GameAnimSet_SetTFactor(GAMEANIMSET *set, f32 factor) {
  GAMEANIMOBJ *obj;

  if (set == NULL)
    return;
  for (obj = set->objects; obj != NULL; obj = obj->next) {
    nuinstanim_s *animation = obj->instance_anim;
    if (animation != NULL) {
      f32 direction = obj->start_frame > obj->end_frame ? -1.0f : 1.0f;
      if (animation->fparam1 != 0.0f)
        obj->instance_anim->tfactor =
            obj->instance_anim->fparam1 * factor * direction;
      else
        animation->tfactor = direction * factor;
    }
  }
}

// FUNCTION: LEGOBATMAN 0x006041d0
void GameAnimSet_ScaleFParam1(GAMEANIMSET *set, f32 scale) {
  GAMEANIMOBJ *obj;

  if (set != NULL) {
    for (obj = set->objects; obj != NULL; obj = obj->next) {
      if (obj->instance_anim != NULL)
        obj->instance_anim->fparam1 = obj->instance_anim->fparam1 * scale;
    }
  }
}

// FUNCTION: LEGOBATMAN 0x00604690
i32 GameAnimSet_IsAnimationReset(GAMEANIMSET *set) {
  GAMEANIMOBJ *obj;

  if (set != NULL) {
    i32 animated_count = 0;
    i32 repeating_count = 0;
    for (obj = set->objects; obj != NULL; obj = obj->next) {
      nuinstanim_s *animation = obj->instance_anim;
      if (animation == NULL)
        continue;
      animated_count++;
      if (animation->repeating) {
        repeating_count++;
        continue;
      }
      if (animation->playing)
        return 0;
      f32 direction = obj->start_frame > obj->end_frame ? -1.0f : 1.0f;
      if (animation->ltime * direction > obj->start_frame * direction)
        return 0;
    }
    if (animated_count > 0 && repeating_count == animated_count)
      return 0;
  }
  return 1;
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

// FUNCTION: LEGOBATMAN 0x006047b0
i32 GameAnimSet_Play(GAMEANIMSET *set, f32 speed, i32 evaluate_state) {
  GAMEANIMOBJ *obj;

  if (set == NULL)
    return 1;
  if (evaluate_state != 0)
    GameAnimSet_EvaluateState(set);
  set->fc = 0.0f;
  if (speed >= 0.0f) {
    if (set->state == 2)
      return 1;
  } else if (speed < 0.0f) {
    if (set->state == 0)
      return 1;
  }
  if (set->animated_count == 0)
    return 1;
  for (obj = set->objects; obj != NULL; obj = obj->next) {
    if (obj->instance_anim != NULL) {
      obj->instance_anim->flags |= 1;
      obj->instance_anim->flags &= ~4;
      f32 direction = obj->start_frame > obj->end_frame ? -1.0f : 1.0f;
      if (direction * obj->start_frame > obj->instance_anim->ltime * direction)
        obj->instance_anim->ltime = obj->start_frame;
      else if (direction * obj->end_frame <
               obj->instance_anim->ltime * direction)
        obj->instance_anim->ltime = obj->end_frame;
      nuinstanim_s *animation = obj->instance_anim;
      if (animation->fparam1 != 0.0f)
        obj->instance_anim->tfactor =
            obj->instance_anim->fparam1 * speed * direction;
      else
        animation->tfactor = direction * speed;
    }
  }
  if (!(set->flags & 4))
    GameAnimSet_AddToSystemList(set);
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

// FUNCTION: LEGOBATMAN 0x00604ac0
i32 GameAnimSet_SetRepeating(GAMEANIMSET *set, i32 repeating) {
  GAMEANIMOBJ *obj;

  if (set != NULL) {
    for (obj = set->objects; obj != NULL; obj = obj->next) {
      if (obj->instance_anim != NULL)
        obj->instance_anim->repeating = repeating;
    }
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x00604b00
i32 GameAnimSet_JumpToEnd(GAMEANIMSET *set) {
  GAMEANIMOBJ *obj;

  if (set != NULL) {
    for (obj = set->objects; obj != NULL; obj = obj->next) {
      if (obj->instance_anim != NULL) {
        obj->instance_anim->flags &= ~1;
        obj->instance_anim->ltime = obj->end_frame;
      }
    }
    GameAnimSet_EvaluateState(set);
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x00604b40
i32 GameAnimSet_JumpToStart(GAMEANIMSET *set) {
  GAMEANIMOBJ *obj;

  if (set != NULL) {
    for (obj = set->objects; obj != NULL; obj = obj->next) {
      if (obj->instance_anim != NULL) {
        obj->instance_anim->flags &= ~1;
        obj->instance_anim->ltime = obj->start_frame;
      }
    }
    GameAnimSet_EvaluateState(set);
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x00604c50
i32 GameAnimSet_AllPlayingUnk00604c50(GAMEANIMSET *set) {
  GAMEANIMOBJ *obj;

  if (set != NULL) {
    for (obj = set->objects; obj != NULL; obj = obj->next) {
      if (obj->instance_anim != NULL && !(obj->instance_anim->flags & 1))
        return 0;
    }
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x00604c80
f32 GameAnimSet_GetCurrentFrame(GAMEANIMSET *set) {
  GAMEANIMOBJ *obj;

  if (set != NULL) {
    for (obj = set->objects; obj != NULL; obj = obj->next) {
      if (obj->instance_anim != NULL)
        return obj->instance_anim->ltime;
    }
  }
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x00604cb0
f32 GameAnimSet_GetMinFrameUnk00604cb0(GAMEANIMSET *set) {
  if (set != NULL)
    return set->min_frame;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x00604cc0
f32 GameAnimSet_GetMaxFrameUnk00604cc0(GAMEANIMSET *set) {
  if (set != NULL)
    return set->max_frame;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x00604cd0
f32 GameAnimSet_GetAnimPos(GAMEANIMOBJ *object) {
  f32 position = 0.0f;

  if (object != NULL && object->instance_anim != NULL &&
      object->animation != NULL) {
    if (object->start_frame == object->end_frame) {
      position = 1.0f;
    } else {
      position = (object->instance_anim->ltime - object->start_frame) /
                 (object->end_frame - object->start_frame);
      if (position < 0.0f)
        position = 0.0f;
      else if (position > 1.0f)
        position = 1.0f;
    }
  }
  return position;
}

// FUNCTION: LEGOBATMAN 0x00604d50
void GameAnimSet_SetAnimPos(GAMEANIMOBJ *object, f32 position) {
  if (object == NULL || object->instance_anim == NULL ||
      object->animation == NULL) {
    return;
  }

  if (position < 0.0f) {
    position = 0.0f;
  }
  if (position > 1.0f) {
    position = 1.0f;
  }
  object->instance_anim->ltime =
      (object->end_frame - object->start_frame) * position +
      object->start_frame;
}

// FUNCTION: LEGOBATMAN 0x00604ec0
i32 GameAnimSet_GetVisibility(GAMEANIMSET *set) {
  GAMEANIMOBJ *obj;
  i32 visible;

  if (set != NULL) {
    visible = 0;
    for (obj = set->objects; obj != NULL; obj = obj->next) {
      if (NuSpecialGetVisibilityFn(&obj->special))
        visible++;
    }
    if (visible == set->object_count)
      return 2;
    if (visible > 0)
      return 1;
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x00604f10
void GameAnimSet_SetVisibility(GAMEANIMSET *set, i32 visibility) {
  GAMEANIMOBJ *obj;

  if (set != NULL) {
    for (obj = set->objects; obj != NULL; obj = obj->next)
      NuSpecialSetVisibility(&obj->special, visibility);
  }
}

// FUNCTION: LEGOBATMAN 0x00604f40
void GameAnimSet_EvalAnim(GAMEANIMSET *set) {
  GAMEANIMOBJ *obj;

  if (set != NULL) {
    for (obj = set->objects; obj != NULL; obj = obj->next) {
      if (obj->instance_anim != NULL)
        SetAnimFrame(&obj->special, obj->instance_anim->ltime);
    }
  }
}

// STUB: LEGOBATMAN 0x00604b80
// x87 operand order in the first clamp compare (orig loads ltime first).
void GameAnimSet_JumpToAnimPos(GAMEANIMSET *set, f32 position) {
  GAMEANIMOBJ *obj;

  if (set == NULL)
    return;
  for (obj = set->objects; obj != NULL; obj = obj->next) {
    if (obj->instance_anim == NULL || obj->animation == NULL)
      continue;
    obj->instance_anim->flags &= ~1;
    f32 frame =
        (obj->end_frame - obj->start_frame) * position + obj->start_frame;
    obj->instance_anim->ltime = frame;
    f32 direction = obj->start_frame > obj->end_frame ? -1.0f : 1.0f;
    if (obj->end_frame * direction < obj->instance_anim->ltime * direction)
      obj->instance_anim->ltime = obj->end_frame;
    if (obj->instance_anim->ltime * direction < obj->start_frame * direction)
      obj->instance_anim->ltime = obj->start_frame;
  }
}

// FUNCTION: LEGOBATMAN 0x00604db0
void GameAnimSet_SetOffset(GAMEANIMSET *set, nuvec_s *offset) {
  GAMEANIMOBJ *obj;

  if (set == NULL)
    return;
  for (obj = set->objects; obj != NULL; obj = obj->next) {
    if (obj->f0 & 4)
      continue;
    numtx_s *source = NuSpecialGetMtx(&obj->special);
    if (source == NULL)
      continue;
    numtx_s matrix = *source;
    matrix.m30 += offset->x;
    matrix.m31 += offset->y;
    matrix.m32 += offset->z;
    NuSpecialSetDrawMtx(&obj->special, &matrix);
  }
}

// FUNCTION: LEGOBATMAN 0x00604f80
f32 GameAnimSet_AutoSetReflectY(GAMEANIMSET *set, nuvec_s *position,
                                numtx_s *matrix) {
  if (set != NULL && set->objects != NULL) {
    if (matrix == NULL)
      matrix = NuSpecialGetMtx(&set->objects->special);
    f32 height = matrix->m31;
    f32 ground = GameShadow(NULL, position, g_unk009ca98c, -1);
    if (ground != 2000000.0)
      return ground - height;
  }
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x00604e30
f32 GameAnimSet_GetCompletionRatio(GAMEANIMSET *set) {
  GAMEANIMOBJ *obj;
  f32 ratio = 0.0f;

  if (set != NULL) {
    for (obj = set->objects; obj != NULL; obj = obj->next) {
      if (obj->instance_anim != NULL) {
        if (obj->start_frame == obj->end_frame) {
          ratio = 1.0f;
        } else {
          ratio = (obj->instance_anim->ltime - obj->start_frame) /
                  (obj->end_frame - obj->start_frame);
          if (ratio > 1.0f)
            ratio = 1.0f;
          else if (ratio < 0.0f)
            ratio = 0.0f;
        }
        break;
      }
    }
  }
  return ratio;
}
