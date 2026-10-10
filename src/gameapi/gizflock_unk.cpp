// gameapi/gizflock_unk.cpp: flock_extras config parser, between rtleditor.cpp
// and listman_gen.cpp; Mac puts it in the GizFlock file. File name unproven.

#include "../nu2api/nucore/common.h"
#include "../nu2api/numath/nuinline_unk.h"

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x00665780
static f32 NuFabs(f32 f);
// FUNCTION: LEGOBATMAN 0x00665a40
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);
// FUNCTION: LEGOBATMAN 0x00665a30
static void NuVecZeroInline(f32 *v);
// FUNCTION: LEGOBATMAN 0x00665ad0
static void NuVecScaleInline(f32 *dst, f32 *src, f32 s);

typedef struct nufpar_s NUFPAR;

f32 NuFParGetFloat(NUFPAR *parser);

typedef struct GIZFLOCKEXTRA_s {
  u32 pad0;
  f32 anim_speed; // 0x04
} GIZFLOCKEXTRA;

// GLOBAL: LEGOBATMAN 0x00ad258c
extern GIZFLOCKEXTRA *g_unk00ad258c;

// keyword "anim_speed" in table 0x00967db8
// FUNCTION: LEGOBATMAN 0x00665f60
void GizFlock_Config_anim_speed(NUFPAR *parser) {
  g_unk00ad258c->anim_speed = NuFParGetFloat(parser);
}

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_gizflock_unk(f32 *v, f32 a, i32 i) {
  v[2] = NuFabs(a);
  NuVec4Set(v, a, a, a, a);
  NuVecScaleInline(v + 8, v, a);
  NuVecZeroInline(v + 80);
}

// The gizmo callbacks (Mac GizFlock_*), named by their RegisterGizmo
// 0x66c150 slot.

#include "../batman/leveldata_unk.h"
#include "../batman/worldinfo_unk.h"
#include <stddef.h>
#include <string.h>

struct nuhspecial_s {
  void *scene;
  void *special;
  void *display_special;
};

typedef struct GIZFLOCKCREATURE_s {
  u8 pad0[0x38];
} GIZFLOCKCREATURE;

// Editor waypoints of a flock, 0x60 bytes each.
typedef struct GIZFLOCKPOINT_s {
  nuvec_s pos;    // 0x00
  nuvec_s v0c;    // 0x0c
  f32 f18;        // 0x18
  u8 type;        // 0x1c
  u8 flags;       // 0x1d, 1 moved to the end
  u16 pad1e;      // 0x1e
  u32 name[0x10]; // 0x20
} GIZFLOCKPOINT;

typedef struct sGizFlock {
  char name[0x10];             // 0x00
  GIZFLOCKCREATURE *creatures; // 0x10
  u16 creature_count;          // 0x14
  u16 pad16;
  u32 flags_lo : 16;     // 0x18
  u32 extra_special : 1; // 0x18 bit 16
  u32 char_special : 1;  // 0x18 bit 17
  u32 flags_hi : 14;
  nuvec_s position; // 0x1c
  u8 pad28[0x34 - 0x28];
  f32 f34[4]; // 0x34
  f32 f44[4]; // 0x44
  u16 w54;    // 0x54
  u16 pad56;
  f32 f58[5]; // 0x58
  f32 f6c[5]; // 0x6c
  f32 f80;    // 0x80
  u8 pad84[0x90 - 0x84];
  f32 f90;               // 0x90
  nuvec_s v94;           // 0x94
  f32 fa0;               // 0xa0
  GIZFLOCKPOINT *points; // 0xa4
  u16 point_count;       // 0xa8
  u16 padaa;
  void *pac; // 0xac
  u8 padb0[0xb4 - 0xb0];
  f32 fb4;              // 0xb4
  f32 fb8;              // 0xb8
  nuhspecial_s special; // 0xbc
  f32 anim_speed;       // 0xc8
  u8 padcc[0xd0 - 0xcc];
  u16 char_id; // 0xd0
  u8 padd2[0x188 - 0xd2];
  f32 f188; // 0x188
  f32 f18c; // 0x18c
  u8 b190;  // 0x190
  u8 b191;  // 0x191
  u8 pad192[2];
  nuvec_s v194; // 0x194
  f32 f1a0;     // 0x1a0
  nuvec_s v1a4; // 0x1a4
  u8 pad1b0[0x1d0 - 0x1b0];
  u16 b0 : 1;
  u16 active : 1;    // 0x1d0 bit 1
  u16 triggered : 1; // 0x1d0 bit 2
  u16 finished : 1;  // 0x1d0 bit 3
  u8 pad1d2[0x1d4 - 0x1d2];
} GIZFLOCK;

typedef struct GIZFLOCKEXTRASCENE_s {
  nugscn_s *scene; // 0x00
  f32 anim_speed;  // 0x04
} GIZFLOCKEXTRASCENE;

struct sGizFlockSys {
  GIZFLOCK *flocks; // 0x00
  u16 count;        // 0x04
  u16 pad6;
  GIZFLOCKCREATURE *creatures; // 0x08
  u8 padc[0x18 - 0xc];
  GIZFLOCKPOINT *points; // 0x18
  u16 point_count;       // 0x1c
  u16 pad1e;
  GIZFLOCKPOINT *points_end; // 0x20
  u16 moved_count;           // 0x24
  u16 pad26;
  GIZFLOCKPOINT *points_top; // 0x28
  u8 pad2c[0x30 - 0x2c];
  void *p30;                    // 0x30
  GIZFLOCKEXTRASCENE extras[5]; // 0x34
  u8 extra_count;               // 0x5c
};

typedef struct GIZFLOCKPROGRESS_s {
  u32 a0[1]; // 0x00
  u32 a4[1]; // 0x04
  u32 a8[1]; // 0x08
  u32 ac[1]; // 0x0c
} GIZFLOCKPROGRESS;

typedef struct GIZMO_s {
  void *object;
} GIZMO;

typedef struct ADDGIZMOTYPE_s {
  char *name;        // 0x00
  char *prefix;      // 0x04
  u16 progress_size; // 0x08
  void *fns[0x1c];   // 0x0c
} ADDGIZMOTYPE;

// GLOBAL: LEGOBATMAN 0x00960118
extern ADDGIZMOTYPE Default_ADDGIZMOTYPE;

i32 NuStrLen(const char *s);
void AddGizmo(GIZMOSYS_s *gizmo_sys, i32 type_id, void *a, void *object);
void *GameBufferAllocProgressUnk005bbaf0(VARIPTR *buf, VARIPTR *buf_end,
                                         i32 size);

// FUNCTION: LEGOBATMAN 0x00666400
i32 GizFlock_GetMaxGizmos(void *world_ptr) {
  return ((WORLDINFO_s *)world_ptr)->current_level->max_flocks;
}

// FUNCTION: LEGOBATMAN 0x00666420
void GizFlock_AddGizmos(GIZMOSYS_s *gizmo_sys, i32 type_id, void *world_ptr,
                        void *unused) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  if (world == NULL)
    return;
  for (i32 i = 0; i < world->gizflock_sys->count; i++) {
    if (NuStrLen(world->gizflock_sys->flocks[i].name) != 0)
      AddGizmo(gizmo_sys, type_id, NULL, &world->gizflock_sys->flocks[i]);
  }
}

// FUNCTION: LEGOBATMAN 0x00666560
char *GizFlock_GetGizmoName(GIZMO *gizmo) {
  return gizmo != NULL ? ((GIZFLOCK *)gizmo->object)->name : NULL;
}

// FUNCTION: LEGOBATMAN 0x00666570
i32 GizFlock_GetOutput(GIZMO *gizmo, i32 output, i32 b) {
  if (gizmo != NULL && gizmo->object != NULL) {
    GIZFLOCK *flock = (GIZFLOCK *)gizmo->object;
    if (flock->active || b != 0) {
      switch (output) {
      case 0:
        if (flock->triggered)
          return 1;
        break;
      case 1:
        if (flock->finished)
          return 1;
        break;
      }
    }
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x006665c0
char *GizFlock_GetOutputName(GIZMO *gizmo, i32 output) {
  switch (output) {
  case 0:
    return "Triggered";
  case 1:
    return "Finished";
  }
  return NULL;
}

// FUNCTION: LEGOBATMAN 0x006665e0
i32 GizFlock_GetNumOutputs(GIZMO *gizmo) { return 2; }

// FUNCTION: LEGOBATMAN 0x006665f0
nuvec_s *GizFlock_GetPos(GIZMO *gizmo) {
  if (gizmo != NULL && gizmo->object != NULL)
    return &((GIZFLOCK *)gizmo->object)->position;
  return NULL;
}

// FUNCTION: LEGOBATMAN 0x00667300
void *GizFlock_AllocateProgressData(VARIPTR *buf, VARIPTR *buf_end) {
  return GameBufferAllocProgressUnk005bbaf0(buf, buf_end,
                                            sizeof(GIZFLOCKPROGRESS));
}

// FUNCTION: LEGOBATMAN 0x00667320
void GizFlock_ClearProgress(void *world, void *progress_ptr) {
  GIZFLOCKPROGRESS *progress = (GIZFLOCKPROGRESS *)progress_ptr;
  if (progress != NULL) {
    progress->a0[0] = 0xffffffff;
    progress->a4[0] = 0xffffffff;
    progress->a8[0] = 0;
    progress->ac[0] = 0;
  }
}

extern "C" i32 EdFileReadInt(void);
extern "C" u32 EdFileReadUnsignedInt(void);
extern "C" i16 EdFileReadShort(void);
extern "C" char EdFileReadChar(void);
extern "C" u8 EdFileReadUnsignedChar(void);
extern "C" f32 EdFileReadFloat(void);
extern "C" void EdFileRead(void *buf, i32 len);
extern "C" void EdFileReadNuVec(nuvec_s *out);
int NuStrCpy(char *dst, const char *src);
int NuStrCmp(const char *a, const char *b);
void *GameBufferAlloc(variptr_u *buf, variptr_u *buf_end, i32 size);
i32 Unk0065e690(WORLDINFO_s *world);
i32 Unk0065e6a0(WORLDINFO_s *world, i32 index);
i16 *Unk0065e6c0(WORLDINFO_s *world, i32 index);
void *Unk0065ecb0(WORLDINFO_s *world, char *name);
void GizFlock_Unk00666610(WORLDINFO_s *world);
void GizFlock_Unk00666750(WORLDINFO_s *world, void **p);
// GLOBAL: LEGOBATMAN 0x00ad1fd0
extern i32 g_unk00ad1fd0;
// GLOBAL: LEGOBATMAN 0x00ad68fc
extern i32 (*g_unk00ad68fc)(i32 id);
// GLOBAL: LEGOBATMAN 0x00ad6904
extern char *(*g_unk00ad6904)(i32 id);

// STUB: LEGOBATMAN 0x0066b7f0
// Register allocation: remaining/flocks swap ebx and esi.
static i32 GizFlock_AssignCreatures(WORLDINFO_s *world) {
  sGizFlockSys *sys;
  GIZFLOCKCREATURE *creature;
  i16 remaining;
  i32 i;

  if (world == NULL || (sys = world->gizflock_sys) == NULL)
    return 0;
  remaining = world->current_level->max_flock_creatures;
  creature = sys->creatures;
  for (i = 0; i < sys->count; i++) {
    if (remaining <= 0) {
      sys->flocks[i].creatures = NULL;
      sys->flocks[i].creature_count = 0;
    } else {
      sys->flocks[i].creatures = creature;
      if (sys->flocks[i].creature_count > remaining)
        sys->flocks[i].creature_count = remaining;
      remaining -= sys->flocks[i].creature_count;
      creature += sys->flocks[i].creature_count;
    }
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x0066baf0
static void GizFlock_SortPoints(sGizFlockSys *sys) {
  GIZFLOCKPOINT tmp;
  GIZFLOCKPOINT *last;
  i32 i;
  i32 j;
  i32 k;

  sys->points_end = sys->points_top = &sys->points[sys->point_count];
  last = sys->points_end - 1;
  sys->moved_count = 0;
  for (i = 0; i < sys->point_count; i++) {
    if (sys->points[i].flags & 1) {
      tmp = sys->points[i];
      sys->point_count--;
      for (j = i; j < sys->point_count + sys->moved_count; j++)
        sys->points[j] = sys->points[j + 1];
      *last = tmp;
      sys->points_end--;
      sys->moved_count++;
      for (k = 0; k < sys->count; k++) {
        if (sys->flocks[k].points > &sys->points[i])
          sys->flocks[k].points--;
      }
      i--;
    }
  }
}

// STUB: LEGOBATMAN 0x0066bc00
// Register allocation: loop index/pointer swap esi and edi.
static i32 GizFlock_FindExtraSpecial(WORLDINFO_s *world, nuhspecial_s *special,
                                     char *name, f32 *anim_speed) {
  sGizFlockSys *sys;
  i32 i;

  if (world == NULL || (sys = world->gizflock_sys) == NULL || special == NULL)
    return 0;
  for (i = 0; i < sys->extra_count; i++) {
    if (NuSpecialFind(sys->extras[i].scene, special, name, 1)) {
      *anim_speed = sys->extras[i].anim_speed;
      return 1;
    }
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x0066bc70
i32 GizFlock_Load(void *world_ptr, void *sys_ptr) {
  WORLDINFO_s *world = (WORLDINFO_s *)world_ptr;
  sGizFlockSys *sys = (sGizFlockSys *)sys_ptr;
  char name[0x100];
  GIZFLOCK *flock;
  i32 version;
  u32 count;
  u32 total;
  u32 i;
  i32 j;
  i32 n;
  GIZFLOCKPOINT *point;

  if (sys->count == 0) {
    count = Unk0065e690(world);
    total = 0;
    for (i = 0; i < count; i++) {
      if (Unk0065e6a0(world, i) == g_unk00ad1fd0)
        total += *Unk0065e6c0(world, i);
    }
    if (total > 0) {
      sys->p30 = GameBufferAlloc(&world->buf104, &world->bufEnd108, total * 4);
      GizFlock_Unk00666610(world);
    }
    version = EdFileReadInt();
    sys->count = EdFileReadShort();
    point = sys->points;
    flock = sys->flocks;
    for (i = 0; i < sys->count; i++, flock++) {
      n = EdFileReadChar();
      if (n != 0) {
        EdFileRead(flock->name, n);
        flock->name[n] = 0;
      }
      n = EdFileReadChar();
      memset(&flock->special, 0, sizeof(flock->special));
      if (n != 0) {
        EdFileRead(name, n);
        name[n] = 0;
      } else {
        NuStrCpy(name, "Bat_Special");
      }
      if (GizFlock_FindExtraSpecial(world, &flock->special, name,
                                    &flock->anim_speed)) {
        flock->char_special = 0;
        flock->extra_special = 1;
      } else if (g_unk00ad68fc != NULL && g_unk00ad6904 != NULL) {
        i32 id;
        for (j = 0; (id = g_unk00ad68fc(j)) != -1; j++) {
          if (NuStrCmp(name, g_unk00ad6904(id)) == 0) {
            flock->char_id = id;
            flock->extra_special = 0;
            flock->char_special = 1;
            break;
          }
        }
      }
      flock->creature_count = EdFileReadShort();
      *(u32 *)((u8 *)flock + 0x18) = EdFileReadUnsignedInt();
      EdFileReadNuVec(&flock->position);
      flock->f34[0] = EdFileReadFloat();
      flock->f34[1] = EdFileReadFloat();
      flock->f34[2] = EdFileReadFloat();
      flock->f34[3] = EdFileReadFloat();
      if (version > 1) {
        flock->f44[0] = EdFileReadFloat();
        flock->f44[1] = EdFileReadFloat();
        flock->f44[2] = EdFileReadFloat();
        flock->f44[3] = EdFileReadFloat();
      }
      flock->w54 = EdFileReadShort();
      flock->f58[0] = EdFileReadFloat();
      flock->f58[1] = EdFileReadFloat();
      flock->f58[2] = EdFileReadFloat();
      flock->f58[3] = EdFileReadFloat();
      flock->f58[4] = EdFileReadFloat();
      if (version > 1)
        flock->f80 = EdFileReadFloat();
      flock->f6c[0] = EdFileReadFloat();
      flock->f6c[1] = EdFileReadFloat();
      flock->f6c[2] = EdFileReadFloat();
      flock->f6c[3] = EdFileReadFloat();
      flock->f6c[4] = EdFileReadFloat();
      flock->f90 = EdFileReadFloat();
      EdFileReadNuVec(&flock->v94);
      flock->fa0 = EdFileReadFloat();
      n = EdFileReadChar();
      if (n != 0) {
        EdFileRead(name, n);
        name[n] = 0;
        flock->pac = Unk0065ecb0(world, name);
        if (flock->pac != NULL)
          GizFlock_Unk00666750(world, &flock->pac);
      }
      flock->fb4 = EdFileReadFloat();
      flock->fb8 = EdFileReadFloat();
      flock->f188 = EdFileReadFloat();
      flock->f18c = EdFileReadFloat();
      flock->point_count = EdFileReadShort();
      if (flock->point_count != 0) {
        flock->points = point;
        for (j = 0; j < flock->point_count; point++, j++) {
          flock->points[j].type = EdFileReadUnsignedChar();
          flock->points[j].flags = EdFileReadUnsignedChar();
          n = EdFileReadChar();
          if (n != 0) {
            EdFileRead(flock->points[j].name, n);
            flock->points[j].name[n] = 0;
          }
          EdFileReadNuVec(&flock->points[j].pos);
          if (flock->points[j].type == 0)
            flock->points[j].f18 = EdFileReadFloat();
          else if (flock->points[j].type == 1)
            EdFileReadNuVec(&flock->points[j].v0c);
          sys->point_count++;
        }
      }
      flock->b190 = EdFileReadUnsignedChar();
      flock->b191 = EdFileReadUnsignedChar();
      EdFileReadNuVec(&flock->v194);
      if (flock->b190 == 2 || flock->b190 == 3)
        flock->f1a0 = EdFileReadFloat();
      else if (flock->b190 == 4 || flock->b190 == 5)
        EdFileReadNuVec(&flock->v1a4);
    }
    GizFlock_AssignCreatures(world);
    GizFlock_SortPoints(sys);
    return 1;
  }
  return 0;
}

void GizFlock_EarlyUpdate(void *world, void *sys, f32 dt);
void GizFlock_Draw(void *world, void *sys, f32 dt);
void GizFlock_Activate(GIZMO *gizmo, i32 active);
void GizFlock_StoreProgress(void *world, void *unused, void *progress);
void GizFlock_Reset(void *world, void *unused, void *progress);
void *GizFlock_ReserveBufferSpace(void *world);

// GLOBAL: LEGOBATMAN 0x00967db4
i32 flock_gizmotype_id = -1;

// FUNCTION: LEGOBATMAN 0x0066c150
ADDGIZMOTYPE *GizFlock_RegisterGizmo(i32 type_id) {
  // GLOBAL: LEGOBATMAN 0x00ad2598
  static ADDGIZMOTYPE addtype;

  addtype = Default_ADDGIZMOTYPE;
  addtype.progress_size = 0;
  addtype.fns[3] = NULL;
  addtype.fns[5] = NULL;
  addtype.fns[11] = NULL;
  addtype.fns[12] = NULL;
  addtype.fns[13] = NULL;
  addtype.fns[15] = NULL;
  addtype.fns[16] = NULL;
  addtype.fns[17] = NULL;
  addtype.fns[18] = NULL;
  addtype.fns[25] = NULL;
  addtype.fns[26] = NULL;
  addtype.fns[27] = NULL;
  flock_gizmotype_id = type_id;
  addtype.name = "GizFlock";
  addtype.prefix = "";
  addtype.fns[0] = (void *)GizFlock_GetMaxGizmos;
  addtype.fns[1] = (void *)GizFlock_AddGizmos;
  addtype.fns[2] = (void *)GizFlock_EarlyUpdate;
  addtype.fns[4] = (void *)GizFlock_Draw;
  addtype.fns[6] = (void *)GizFlock_GetGizmoName;
  addtype.fns[7] = (void *)GizFlock_GetOutput;
  addtype.fns[8] = (void *)GizFlock_GetOutputName;
  addtype.fns[9] = (void *)GizFlock_GetNumOutputs;
  addtype.fns[10] = (void *)GizFlock_Activate;
  addtype.fns[14] = (void *)GizFlock_GetPos;
  addtype.fns[19] = (void *)GizFlock_AllocateProgressData;
  addtype.fns[20] = (void *)GizFlock_ClearProgress;
  addtype.fns[21] = (void *)GizFlock_StoreProgress;
  addtype.fns[22] = (void *)GizFlock_Reset;
  addtype.fns[23] = (void *)GizFlock_ReserveBufferSpace;
  addtype.fns[24] = (void *)GizFlock_Load;
  return &addtype;
}
