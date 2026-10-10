// gameapi/, file unknown: HubAreaInfo_* / HubEpisodeInfo_* (Mac order:
// HubAreaInfo_Set, _Get, _Init, _FindFromAreaIndex, _IsAreaOpen,
// HubEpisodeInfo_Set, _Get, _Init, _FindFromEpisodeIndex).

#include "../batman/worldinfo_unk.h"
#include <stddef.h>
#include <string.h>

struct nugspline_s {
  i16 len; // 0x00
  u8 pad02[8 - 2];
  nuvec_s *pts; // 0x08
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

i32 NuSpecialExistsFn(nuhspecial_s *sp);
nuvec_s *NuSpecialGetDrawPos(nuhspecial_s *special);
float NuVecDistSqr(nuvec_s *a, nuvec_s *b, nuvec_s *d);
extern GameObject_s *Player[8];

// STUB: LEGOBATMAN 0x006183e0
// nearest episode special to the given players (mask); orig loads the list via
// eax and recomputes &info->sp2 per pass, ours keeps it as a second IV
HUBEPISODEINFO *HubEpisodeInfo_FindNearestMapToPlayers(u32 players) {
  HUBEPISODEINFO *best = NULL;
  HUBEPISODEINFO *info = g_unk00acb6e8;
  if (info != NULL) {
    f32 best_dist = 25.0f;
    for (; info->episode != -1; info++) {
      if (NuSpecialExistsFn((nuhspecial_s *)info->sp2)) {
        for (i32 i = 0; i < 2; i++) {
          if (players & (1 << i)) {
            f32 d = NuVecDistSqr(NuSpecialGetDrawPos((nuhspecial_s *)info->sp2),
                                 &Player[i]->position, NULL);
            if (d < best_dist) {
              best_dist = d;
              best = info;
            }
          }
        }
      }
    }
  }
  return best;
}

// Raw view of AREADATA_s (0xbc bytes, area_unk.cpp).
struct HIADATA_s {
  u8 pad00[0x60];
  i16 levels[0xe]; // 0x60
  u32 flags;       // 0x7c
  u8 pad80[0xbc - 0x80];
};
// Raw view of LEVELDATA_s: only the area index is evidenced.
struct HIDOORLEVEL_s {
  u8 pad00[0xab];
  i8 area_index; // 0xab
};
struct DOOR_s;
extern HIADATA_s *ADataList;
// GLOBAL: LEGOBATMAN 0x00963898
i32 last_hub_area = -1;
// GLOBAL: LEGOBATMAN 0x00963894
extern i32 hub_new_level;
// GLOBAL: LEGOBATMAN 0x009630b4
extern i32 g_unk009630b4; // door menu for bonus areas
// GLOBAL: LEGOBATMAN 0x009630b8
extern i32 g_unk009630b8; // door menu otherwise
// GLOBAL: LEGOBATMAN 0x00acb6ac
extern f32 hub_episode_time;
// GLOBAL: LEGOBATMAN 0x00acb6a4
extern f32 hub_area_time;
// GLOBAL: LEGOBATMAN 0x00a97c58
extern f32 MainRenderTargetTime;
void MakeMenuPacket();
void NeedScreenGrab(i32 needed);
void Unk005a4170();
void Unk005d4de0();
extern "C" void NewMenuBatte(i32 a, i32 b, i32 c);

// FUNCTION: LEGOBATMAN 0x006184a0
void Hub_ActivateDoorMenu(WORLDINFO_s *world, DOOR_s *door,
                          HIDOORLEVEL_s **level) {
  i32 area = (*level)->area_index;
  i32 menu;

  if (area == -1)
    return;
  if (last_hub_area == -1) {
    HUBAREAINFO *info = HubAreaInfo_FindFromAreaIndex(area);
    if (info != NULL && info->gizmo_ptr == NULL &&
        HubAreaInfo_IsAreaOpen(world, info))
      last_hub_area = area;
  }
  if (last_hub_area != area) {
    *level = NULL;
    return;
  }
  menu = (ADataList[area].flags & 4) ? g_unk009630b4 : g_unk009630b8;
  if (menu == -1)
    return;
  MakeMenuPacket();
  hub_new_level = ADataList[area].levels[0];
  *level = NULL;
  hub_episode_time = 0.0f;
  hub_area_time = 0.0f;
  MainRenderTargetTime = 0.0f;
  NeedScreenGrab(1);
  Unk005a4170();
  Unk005d4de0();
  NewMenuBatte(menu, -1, -1);
}

struct TERRSURFACE_s {
  f32 f0;
  u32 flags; // 0x04
  u32 pad8[5];
};
extern TERRSURFACE_s TerSurface[32];
i32 Unk005cfd10(GameObject_s *obj);
// GLOBAL: LEGOBATMAN 0x0096057c
extern i32 g_unk0096057c;
// GLOBAL: LEGOBATMAN 0x00960580
extern i32 g_unk00960580;

// STUB: LEGOBATMAN 0x00618580
// register allocation only: orig keeps flags in ecx and mask in edx (so the
// out path reloads TerSurface and ors an immediate 0x10); ours swaps them.
u32 Hub_CanStartMenu(u32 mask, u32 *out) {
  u32 players = 0;
  i32 i;

  if (out != NULL)
    *out = 0;
  for (i = 0; i < 2; i++) {
    GameObject_s *obj = Player[i];
    if (obj == NULL || !(obj->flags1fc & 0x80))
      continue;
    if (obj->b24d == 0 && !(obj->f11c0 > 0.0f))
      continue;
    if (!Unk005cfd10(obj) &&
        (g_unk0096057c == -1 || obj->b9db != g_unk0096057c) &&
        (g_unk00960580 == -1 || obj->b9db != g_unk00960580))
      continue;
    i32 surface = obj->surface;
    u32 flags = TerSurface[surface].flags;
    if (surface == 12) {
      if (mask == 0x10) {
        players |= 1 << i;
        if (out != NULL)
          *out |= 0x10;
        continue;
      }
    }
    if ((u32)surface < 32 && (mask & flags)) {
      players |= 1 << i;
      if (out != NULL)
        *out |= TerSurface[surface].flags & mask;
    }
  }
  return players;
}

extern HUBEPISODEINFO *g_unk00acb6c8; // current hub episode (worldmap_unk.cpp)

// FUNCTION: LEGOBATMAN 0x00618690
i32 Hub_GetSelectAreaCamPos(nuvec_s *start, nuvec_s *next) {
  if (g_unk00acb6c8 != NULL && g_unk00acb6c8->spline_ptr != NULL) {
    if (start != NULL)
      *start = g_unk00acb6c8->spline_ptr->pts[0];
    if (next != NULL)
      *next = g_unk00acb6c8->spline_ptr->pts[1];
  }
  return 1;
}
