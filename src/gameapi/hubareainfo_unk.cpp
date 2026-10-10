// gameapi/, file unknown: HubAreaInfo_* / HubEpisodeInfo_* (Mac order:
// HubAreaInfo_Set, _Get, _Init, _FindFromAreaIndex, _IsAreaOpen,
// HubEpisodeInfo_Set, _Get, _Init, _FindFromEpisodeIndex).

#include "../batman/worldinfo_unk.h"
#include <stddef.h>
#include <string.h>

struct nugspline_s {
  i16 len; // 0x00
};

// Raw view: only the area index is evidenced.
struct HIAREADATA_s {
  u8 pad00[0x84];
  u8 index; // 0x84
};

// Raw view of LEVELDATA_s: only the episode is evidenced.
struct HILEVELDATA_s {
  u8 pad00[0x1a];
  u8 episode; // 0x1a
  u8 pad1b[0x20 - 0x1b];
};

// 0x40 bytes, terminated by a NULL area name.
typedef struct HUBAREAINFO_s {
  char *area;    // 0x00
  char *gizmo;   // 0x04
  char *special; // 0x08
  char *gizmo2;  // 0x0c
  char *camera;  // 0x10
  u8 pad14[0x1d - 0x14];
  u8 b1d; // 0x1d
  u8 pad1e[0x20 - 0x1e];
  HIAREADATA_s *areadata;     // 0x20
  GIZMO_s *gizmo_ptr;         // 0x24
  nuhspecial_s *sp_dummy[3];  // 0x28, an nuhspecial_s
  GIZMO_s *gizmo2_ptr;        // 0x34
  GIZMO_s *camera_gizmo;      // 0x38
  nugspline_s *camera_spline; // 0x3c
} HUBAREAINFO;

// 0x3c bytes, terminated by episode -1.
typedef struct HUBEPISODEINFO_s {
  i16 episode;    // 0x00
  char *gizmo;    // 0x04
  char *special;  // 0x08
  char *special2; // 0x0c
  char *spline;   // 0x10
  u8 pad14[0x18 - 0x14];
  HILEVELDATA_s *level;    // 0x18
  GIZMO_s *gizmo_ptr;      // 0x1c
  u32 sp[3];               // 0x20, an nuhspecial_s
  u32 sp2[3];              // 0x2c, an nuhspecial_s
  nugspline_s *spline_ptr; // 0x38
} HUBEPISODEINFO;

GIZMO_s *GizmoFindByName(GIZMOSYS_s *gizmo_sys, i32 type_id, char *name);
nugspline_s *NuSplineFind(nugscn_s *scene, char *name);
HIAREADATA_s *Area_FindByName(char *name, i32 *index);
i32 Unk005bc700(GIZMOSYS_s *gizmo_sys, GIZMO_s *gizmo, i32 a, i32 b);
extern "C" int NuSPrintf(char *buf, char *fmt, ...);

// GLOBAL: LEGOBATMAN 0x00acb6e4
HUBAREAINFO *g_unk00acb6e4;
// GLOBAL: LEGOBATMAN 0x00acb6e8
HUBEPISODEINFO *g_unk00acb6e8;
// GLOBAL: LEGOBATMAN 0x00aca594
extern u8 *g_unk00aca594; // per-area progress, 12 bytes each
// GLOBAL: LEGOBATMAN 0x00ad11c4
extern i32 g_unk00ad11c4;
// GLOBAL: LEGOBATMAN 0x00ad11c0
extern HILEVELDATA_s *g_unk00ad11c0; // level data list, 0x20 bytes each

// FUNCTION: LEGOBATMAN 0x006180a0
void HubAreaInfo_Set(HUBAREAINFO *info) { g_unk00acb6e4 = info; }

// FUNCTION: LEGOBATMAN 0x006180b0
HUBAREAINFO *HubAreaInfo_Get() { return g_unk00acb6e4; }

// STUB: LEGOBATMAN 0x006180c0
// the original walks the list with a pointer to +4 (esi = info + 4)
void HubAreaInfo_Init(WORLDINFO_s *world) {
  HUBAREAINFO *info = g_unk00acb6e4;
  if (info != NULL && info->area != NULL) {
    do {
      info->gizmo_ptr =
          info->gizmo != NULL
              ? GizmoFindByName(world->gizmoSys2b0c, -1, info->gizmo)
              : NULL;
      info->gizmo2_ptr =
          info->gizmo2 != NULL
              ? GizmoFindByName(world->gizmoSys2b0c, -1, info->gizmo2)
              : NULL;
      info->camera_gizmo = NULL;
      info->camera_spline = NULL;
      if (info->camera != NULL) {
        char name[32];
        info->camera_gizmo =
            GizmoFindByName(world->gizmoSys2b0c, -1, info->camera);
        NuSPrintf(name, "cam_%s", info->camera);
        info->camera_spline = NuSplineFind(world->scn140, name);
        if (info->camera_spline != NULL && info->camera_spline->len < 2)
          info->camera_spline = NULL;
      }
      info->areadata = Area_FindByName(info->area, NULL);
      if (info->special != NULL)
        NuSpecialFind(world->scn140, (nuhspecial_s *)info->sp_dummy,
                      info->special, 1);
      else
        memset(info->sp_dummy, 0, sizeof(info->sp_dummy));
      info++;
    } while (info->area != NULL);
  }
}

// FUNCTION: LEGOBATMAN 0x006181e0
HUBAREAINFO *HubAreaInfo_FindFromAreaIndex(i32 index) {
  HUBAREAINFO *info = g_unk00acb6e4;
  if (info != NULL && info->area != NULL) {
    do {
      if (info->areadata != NULL && info->areadata->index == index)
        return info;
      info++;
    } while (info->area != NULL);
  }
  return NULL;
}

// FUNCTION: LEGOBATMAN 0x00618210
i32 HubAreaInfo_IsAreaOpen(WORLDINFO_s *world, HUBAREAINFO *info) {
  if (g_unk00aca594[info->areadata->index * 12 + 2])
    return 1;
  if (info->gizmo2_ptr != NULL &&
      Unk005bc700(world->gizmoSys2b0c, info->gizmo2_ptr, 0, 0))
    return 1;
  return info->b1d == 1;
}

// FUNCTION: LEGOBATMAN 0x00618270
void HubEpisodeInfo_Set(HUBEPISODEINFO *info) { g_unk00acb6e8 = info; }

// FUNCTION: LEGOBATMAN 0x00618280
HUBEPISODEINFO *HubEpisodeInfo_Get() { return g_unk00acb6e8; }

// STUB: LEGOBATMAN 0x00618290
// same +4 loop-pointer bias as HubAreaInfo_Init (esi = info + 4); otherwise
// identical
void HubEpisodeInfo_Init(WORLDINFO_s *world) {
  HUBEPISODEINFO *info = g_unk00acb6e8;
  if (info != NULL && info->episode != -1) {
    do {
      if (info->episode >= 0 && info->episode < g_unk00ad11c4)
        info->level = &g_unk00ad11c0[info->episode];
      else
        info->level = NULL;
      info->gizmo_ptr =
          info->gizmo != NULL
              ? GizmoFindByName(world->gizmoSys2b0c, -1, info->gizmo)
              : NULL;
      if (info->special != NULL)
        NuSpecialFind(world->scn140, (nuhspecial_s *)info->sp, info->special,
                      1);
      else
        memset(info->sp, 0, sizeof(info->sp));
      if (info->special2 != NULL)
        NuSpecialFind(world->scn140, (nuhspecial_s *)info->sp2, info->special2,
                      1);
      else
        memset(info->sp2, 0, sizeof(info->sp2));
      info->spline_ptr = info->spline != NULL
                             ? NuSplineFind(world->scn140, info->spline)
                             : NULL;
      info++;
    } while (info->episode != -1);
  }
}

// STUB: LEGOBATMAN 0x00618380
// the original reloads info->episode at every use; ours keeps it in cx
HUBEPISODEINFO *HubEpisodeInfo_FindFromEpisodeIndex(i32 index) {
  if (index < 0)
    return NULL;
  HUBEPISODEINFO *info = g_unk00acb6e8;
  if (info != NULL && info->episode != -1) {
    do {
      if (info->level != NULL) {
        if (info->level->episode == info->episode && info->episode == index)
          return info;
      } else if (info->episode >= g_unk00ad11c4 && info->episode == index) {
        return info;
      }
      info++;
    } while (info->episode != -1);
  }
  return NULL;
}
