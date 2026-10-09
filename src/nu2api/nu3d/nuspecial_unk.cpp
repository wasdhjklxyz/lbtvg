// nu2api/nu3d/nuspecial_unk.cpp: NuSpecial* handle API, between the
// nuanim_gen.cpp (0x0070b350) and nutexanm_gen.cpp (0x00711580) anchors.

#include "nuspecial.h"

int NuStrICmp(const char *a, const char *b);
float NuVecMag(nuvec_s *v);
// Instance-scene search; name unknown.
int NuSpecialFindUnk0070eda0(nugscn_s *scene, nuhspecial_s *out, char *name,
                             int flag);

// FUNCTION: LEGOBATMAN 0x0070ee40
int NuSpecialFind(nugscn_s *scene, nuhspecial_s *out, char *name) {
  int i;
  nuspecial_s *sp;
  if (!scene || !name) {
    out->scene = 0;
    out->special = 0;
    out->display_special = 0;
    return 0;
  }
  if (scene->display_list)
    return NuSpecialFindUnk0070eda0(scene, out, name, 1);
  sp = scene->specials;
  for (i = 0; i < scene->numspecial; i++, sp++) {
    if (NuStrICmp(name, sp->name) == 0) {
      out->scene = scene;
      out->special = sp;
      out->display_special = 0;
      return 1;
    }
  }
  out->scene = 0;
  out->special = 0;
  out->display_special = 0;
  return 0;
}

// FUNCTION: LEGOBATMAN 0x0070f250
nuvec_s *NuSpecialGetPos(nuhspecial_s *sp) {
  if (sp->display_special)
    return (nuvec_s *)&sp->display_special->instance_mtx.m30;
  if (sp->special)
    return &sp->special->pos;
  return 0;
}

// FUNCTION: LEGOBATMAN 0x0070f270
nuvec_s *NuSpecialGetDrawPos(nuhspecial_s *sp) {
  nuinstance_s *info;
  nuinstanim_s *anim;
  if (sp->display_special) {
    anim = sp->display_special->instance_animation;
    if (anim != (nuinstanim_s *)-1 && anim)
      return (nuvec_s *)&anim->mtx.m30;
    return (nuvec_s *)&sp->display_special->draw_mtx.m30;
  }
  if (sp->special) {
    info = sp->special->instance;
    if (info->animation)
      return (nuvec_s *)&info->animation->mtx.m30;
    return (nuvec_s *)&info->mtx.m30;
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x0070f2b0
numtx_s *NuSpecialGetMtx(nuhspecial_s *sp) {
  if (sp->display_special != 0)
    return (numtx_s *)sp->display_special;
  return (numtx_s *)sp->special;
}

// FUNCTION: LEGOBATMAN 0x0070f2c0
void NuSpecialSetMtx(nuhspecial_s *sp, numtx_s *matrix) {
  if (sp->display_special != 0)
    sp->display_special->instance_mtx = *matrix;
  else
    *(numtx_s *)sp->special = *matrix;
}

// FUNCTION: LEGOBATMAN 0x0070f2e0
void *NuSpecialGetAppData(nuhspecial_s *sp) {
  if (sp->display_special != 0)
    return sp->display_special->app_data;
  return sp->special->app_data;
}

// FUNCTION: LEGOBATMAN 0x0070f300
void NuSpecialSetAppData(nuhspecial_s *sp, void *data) {
  if (sp->display_special != 0)
    sp->display_special->app_data = data;
  else
    sp->special->app_data = data;
}

// FUNCTION: LEGOBATMAN 0x0070f330
char *NuSpecialGetName(nuhspecial_s *sp) {
  if (sp == 0)
    return 0;
  if (sp->display_special != 0)
    return sp->display_special->name;
  return sp->special != 0 ? sp->special->name : 0;
}

// GLOBAL: LEGOBATMAN 0x029f3f1c
void *nuspecial_vertex_states;

// GLOBAL: LEGOBATMAN 0x00b102d4
extern int nurender_global_id;

// GLOBAL: LEGOBATMAN 0x00b102e4
extern unsigned short nurender_vertex_groups_id;

// FUNCTION: LEGOBATMAN 0x0070f360
void NuSpecialVertexStates(void *states) {
  nuspecial_vertex_states = states;
  ++nurender_global_id;
  ++nurender_vertex_groups_id;
}

// GLOBAL: LEGOBATMAN 0x029f3f18
unsigned int nuspecial_vertex_offsets;

// GLOBAL: LEGOBATMAN 0x029f3f14
int nuspecial_vertex_noffsets;

// FUNCTION: LEGOBATMAN 0x0070f380
void NuSpecialVertexOffsets(int count, unsigned int offsets) {
  nuspecial_vertex_offsets = offsets;
  nuspecial_vertex_noffsets = count;
}

// FUNCTION: LEGOBATMAN 0x0070f3a0
int NuSpecialExistsFn(nuhspecial_s *sp) {
  if (sp && (sp->special || sp->display_special))
    return 1;
  return 0;
}

// GLOBAL: LEGOBATMAN 0x029f3f10
extern unsigned int nuspecial_draw_state;

// GLOBAL: LEGOBATMAN 0x029f3f68
extern void **nurndr_forced_mtl_table;

// GLOBAL: LEGOBATMAN 0x029f3f6c
extern int nurndr_nforced_mtls;

// FUNCTION: LEGOBATMAN 0x0070f3c0
void NuSpecialMtlMap(int count, void **materials) {
  if (count == 0) {
    nurndr_forced_mtl_table = 0;
    nuspecial_draw_state &= ~4;
  } else {
    nuspecial_draw_state |= 4;
    nurndr_forced_mtl_table = materials;
    nurndr_nforced_mtls = count;
  }
}

// GLOBAL: LEGOBATMAN 0x029f3f64
void *nurndr_forced_mtl;

// FUNCTION: LEGOBATMAN 0x0070f3f0
void NuSpecialForceMtl(void *material) {
  if (material != 0) {
    nuspecial_draw_state |= 8;
    nurndr_forced_mtl = material;
  } else {
    nuspecial_draw_state &= ~8;
    nurndr_forced_mtl = 0;
  }
}

// FUNCTION: LEGOBATMAN 0x0070f420
void NuSpecialMtl(void *material) { nurndr_forced_mtl = material; }

// GLOBAL: LEGOBATMAN 0x009a247c
extern float nuspecial_const_alpha;

// GLOBAL: LEGOBATMAN 0x009a2480
extern nuvec_s nuspecial_const_tint;

// GLOBAL: LEGOBATMAN 0x029f3f08
int nuspecial_const_alpha_enabled;

// GLOBAL: LEGOBATMAN 0x029f3f0c
int nuspecial_const_tint_enabled;

void NuRndrSetConstColourUnk006eb920(int alpha_enabled, int tint_enabled,
                                     float alpha, nuvec_s *tint);

// FUNCTION: LEGOBATMAN 0x0070f430
void NuSpecialConstAlpha(int enabled, float alpha) {
  if (enabled != 0) {
    nuspecial_const_alpha = alpha;
    nuspecial_draw_state |= 1;
  } else {
    enabled = 0;
    nuspecial_draw_state &= ~1;
  }
  nuspecial_const_alpha_enabled = enabled;
  NuRndrSetConstColourUnk006eb920(nuspecial_const_alpha_enabled,
                                  nuspecial_const_tint_enabled,
                                  nuspecial_const_alpha, &nuspecial_const_tint);
}

// STUB: LEGOBATMAN 0x0070f480
// one swap: orig zeroes ecx before the draw_state &= ~2, ours after.
void NuSpecialConstTint(int enabled, nuvec_s *tint) {
  if (enabled != 0) {
    nuspecial_const_tint_enabled = enabled;
    nuspecial_const_tint = *tint;
    nuspecial_draw_state |= 2;
  } else {
    enabled = 0;
    nuspecial_draw_state &= ~2;
    nuspecial_const_tint_enabled = enabled;
  }
  NuRndrSetConstColourUnk006eb920(nuspecial_const_alpha_enabled,
                                  nuspecial_const_tint_enabled,
                                  nuspecial_const_alpha, &nuspecial_const_tint);
}

// FUNCTION: LEGOBATMAN 0x0070f5d0
void NuSpecialSetCollision(nuhspecial_s *sp, int on) {
  if (sp && sp->scene) {
    if (sp->special) {
      if (on)
        sp->special->flags |= 0x200;
      else
        sp->special->flags &= ~0x200;
    } else if (sp->display_special) {
      if (on)
        sp->display_special->flags |= 0x200;
      else
        sp->display_special->flags &= ~0x200;
    }
  }
}

// FUNCTION: LEGOBATMAN 0x0070f630
int NuSpecialGetCollision(nuhspecial_s *sp) {
  if (sp && sp->scene) {
    if (sp->special)
      return sp->special->flags & 0x200;
    if (sp->display_special)
      return sp->display_special->flags & 0x200;
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x0070f670
int NuSpecialGetVisibilityFn(nuhspecial_s *sp) {
  if (sp->scene) {
    if (sp->special)
      return sp->special->instance->flags & 1;
    if (sp->display_special)
      return (sp->display_special->flags >> 1) & 1;
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x0070f6a0
void NuSpecialSetNoVisiTest(nuhspecial_s *special, int enabled) {
  if (special->scene == 0)
    return;
  if (special->special != 0) {
    special->special->instance->no_visibility_test = enabled;
    return;
  }
  if (special->display_special == 0)
    return;
  if (enabled != 0)
    special->display_special->flags |= 0x80;
  else
    special->display_special->flags &= ~0x80;
}

// FUNCTION: LEGOBATMAN 0x0070f6f0
int NuSpecialGetNoVisiTestFn(nuhspecial_s *special) {
  if (special->scene == 0)
    return 0;
  if (special->special != 0)
    return special->special->instance->no_visibility_test;
  NUDISPLAYSPECIAL *display = special->display_special;
  return display != 0 ? display->flags & 0x80 : 0;
}

// FUNCTION: LEGOBATMAN 0x0070f730
void NuSpecialSetOnScreen(nuhspecial_s *special, int enabled) {
  if (special->scene == 0)
    return;
  if (special->special != 0) {
    special->special->instance->on_screen = enabled;
    return;
  }
  if (special->display_special == 0)
    return;
  if (enabled != 0)
    special->display_special->flags |= 4;
  else
    special->display_special->flags &= ~4;
}

// STUB: LEGOBATMAN 0x0070f780
// register pick only: orig keeps the handle in ecx, ours in eax.
int NuSpecialGetOnScreenFn(nuhspecial_s *special) {
  nuspecial_s *legacy;
  if (special->scene == 0 || (legacy = special->special) == 0)
    return 1;
  return legacy->instance->on_screen;
}

// FUNCTION: LEGOBATMAN 0x0070f7b0
int NuSpecialGetNumSpecials(nugscn_s *scene) {
  if (scene->numspecial != 0)
    return scene->numspecial;
  if (scene->display_list != 0 && scene->display_list->nspecials != 0)
    return scene->display_list->nspecials;
  return 0;
}

// 0x006fced0 is a lone `ret` (debug report compiled out).
void NuErrorUnk006fced0(void);

// FUNCTION: LEGOBATMAN 0x0070f7d0
int NuSpecialGetFirst(nugscn_s *scene, nuhspecial_s *special, int flags) {
  if (scene->specials != 0) {
    special->scene = scene;
    special->special = scene->specials;
    special->display_special = 0;
    return 1;
  }
  if (scene->display_list->nspecials != 0) {
    special->scene = scene;
    special->special = 0;
    special->display_special = scene->display_list->specials;
    return 1;
  }
  special->scene = 0;
  special->special = 0;
  special->display_special = 0;
  if (flags != 0)
    NuErrorUnk006fced0();
  return 0;
}

// FUNCTION: LEGOBATMAN 0x0070f830
void NuSpecialGetNext(nuhspecial_s *special) {
  if (special->special != 0)
    special->special++;
  else if (special->display_special != 0)
    special->display_special++;
}

// STUB: LEGOBATMAN 0x0070f860
// register pick only: orig keeps the handle in ecx (an "int i = 0" at the top
// gets ecx but hoists the xor too early).
int NuSpecialGetInstanceix(nuhspecial_s *special) {
  int i;
  nuspecial_s *legacy = special->special;
  if (legacy != 0) {
    nugscn_s *scene = special->scene;
    i = 0;
    for (; i < scene->instance_count; ++i) {
      if (&scene->instances[i] == legacy->instance)
        return i;
    }
    return -1;
  }
  NUDISPLAYSPECIAL *display = special->display_special;
  return display != 0 ? display->instance_ix : -1;
}

// FUNCTION: LEGOBATMAN 0x0070f8b0
int NuSpecialNumMtls(nuhspecial_s *special) {
  int count = 0;
  nuspecial_s *legacy = special->special;
  if (legacy != 0) {
    nugobject_s *object =
        special->scene->objects[legacy->instance->object_index];
    while (object->next != 0)
      object = object->next;
    for (numtllink_s *link = object->materials; link != 0; link = link->next)
      ++count;
    return count;
  }
  if (special->display_special != 0) {
    NUDISPLAYSPECIAL *display = special->display_special;
    int level = 0;
    while (display->clip_range[level] != 0.0f)
      ++level;
    return display->clip_objects[level].f0;
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x0070f930
extern "C" void *NuSpecialGetMtl(nuhspecial_s *special, int index) {
  nuspecial_s *legacy = special->special;
  if (legacy != 0) {
    nugobject_s *object =
        special->scene->objects[legacy->instance->object_index];
    while (object->next != 0)
      object = object->next;
    numtllink_s *link = object->materials;
    while (index != 0) {
      if (link == 0)
        return 0;
      --index;
      link = link->next;
    }
    return link->material;
  } else if (special->display_special != 0) {
    NUDISPLAYSPECIAL *display = special->display_special;
    int level = 0;
    while (display->clip_range[level] != 0.0f)
      ++level;
    return special->scene->display_list
        ->mtls[display->clip_objects[level].material_ids[index]];
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x0070f9e0
extern "C" void NuSpecialGetBounds(nuhspecial_s *special, nuvec_s *min,
                                   nuvec_s *max) {
  if (special->special != 0) {
    nugobject_s *object =
        special->scene->objects[special->special->instance->object_index];
    while (object->next != 0)
      object = object->next;
    *min = object->min;
    *max = object->max;
  } else if (special->display_special != 0) {
    min->x = special->display_special->min.x;
    min->y = special->display_special->min.y;
    min->z = special->display_special->min.z;
    max->x = special->display_special->max.x;
    max->y = special->display_special->max.y;
    max->z = special->display_special->max.z;
  }
}

// FUNCTION: LEGOBATMAN 0x0070fb20
void NuSpecialGetRadius(nuhspecial_s *sp, nuvec_s *center, float *radius) {
  nuvec_s *c;
  if (sp->special) {
    *radius = sp->scene->objects[sp->special->instance->object_index]->radius;
    c = &sp->scene->objects[sp->special->instance->object_index]->center;
    *center = *c;
  } else {
    c = &sp->display_special->center;
    *center = *c;
    *radius = sp->display_special->radius;
  }
}

// FUNCTION: LEGOBATMAN 0x0070fba0
float NuSpecialGetOriginRadius(nuhspecial_s *sp) {
  if (sp->special)
    return sp->scene->objects[sp->special->instance->object_index]
        ->origin_radius;
  return NuVecMag(&sp->display_special->center) + sp->display_special->radius;
}

// FUNCTION: LEGOBATMAN 0x0070fbf0
numtx_s *NuSpecialGetDrawMtx(nuhspecial_s *sp) {
  nuinstanim_s *anim;
  if (sp->special) {
    anim = sp->special->instance->animation;
    if (anim)
      return &anim->mtx;
    return &sp->special->instance->mtx;
  }
  if (sp->display_special) {
    anim = sp->display_special->instance_animation;
    if (anim != (nuinstanim_s *)-1 && anim)
      return &anim->mtx;
    return &sp->display_special->draw_mtx;
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x0070fce0
nuinstanim_s *NuSpecialGetInstAnim(nuhspecial_s *sp) {
  nuinstanim_s *anim;
  if (sp->special)
    return sp->special->instance->animation;
  if (sp->display_special) {
    anim = sp->display_special->instance_animation;
    if (anim != (nuinstanim_s *)-1 && anim)
      return anim;
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x0070fd10
int NuSpecialCompare(nuhspecial_s *a, nuhspecial_s *b) {
  if (a->special && a->special == b->special)
    return 1;
  if (a->display_special && a->display_special == b->display_special)
    return 1;
  return 0;
}

// FUNCTION: LEGOBATMAN 0x0070ffe0
nugspline_s *NuSplineFind(nugscn_s *scene, char *name) {
  int i;
  nugspline_s *s;
  if (scene) {
    s = scene->splines;
    for (i = 0; i < scene->numsplines; i++, s++) {
      if (NuStrICmp(name, s->name) == 0)
        return s;
    }
  }
  return 0;
}
