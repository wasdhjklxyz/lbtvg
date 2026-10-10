// batman/, file unknown: WorldMapBase's settings parser (Mac order:
// parse_worldmap_end .. parse_mapimageaspectratio, LoadSettings),
// 0x679840..0x679c6c.

#include "../nu2api/nucore/common.h"
#include "../nu2api/nucore/nustring.h"
#include <string.h>

typedef struct nufpar_s {
  unsigned char pad0[0x910];
  char *word_buf; // 0x910
} NUFPAR;

i32 NuFParGetWord(NUFPAR *parser);
f32 NuFParGetFloat(NUFPAR *parser);
i32 NuFParGetInt(NUFPAR *parser);
NUFPAR *NuFParCreate(char *filename);
i32 NuFParPushCom(NUFPAR *parser, void *commands);
i32 NuFParGetLine(NUFPAR *parser);
i32 NuFParInterpretWord(NUFPAR *parser);
void NuFParDestroy(NUFPAR *parser);
void Unk00678b90(char *name); // empty in the release build

struct WORLDINFO_s;
struct nugscn_s;

// Raw view of AREADATA_s.
struct WMAREA_s {
  u8 pad00[0x40];
  char name[0x3c]; // 0x40
  u32 flags;       // 0x7c
  u8 pad80[0x84 - 0x80];
  u8 id; // 0x84
  u8 pad85[0x8e - 0x85];
  i8 b8e; // 0x8e
};

// Mac: WorldMapLocation (0x50 bytes, 16 of them in WorldMapInfo).
struct WMLOCATION_s {
  u8 pad00[0x20];
  WMAREA_s *area; // 0x20
  u8 pad24[0x30 - 0x24];
  i32 open;     // 0x30
  i32 complete; // 0x34
  i32 id;       // 0x38
  u8 pad3c[0x50 - 0x3c];
};

struct WMCOLOUR_s {
  f32 r, g, b;
};

class WorldMapBase {
public:
  static void parse_worldmap_end(NUFPAR *fp);
  static void parse_levelpath(NUFPAR *fp);
  static void parse_reversedirection(NUFPAR *fp);
  static void parse_zoomplaybackminmax(NUFPAR *fp);
  static void parse_zoominteractiveminmax(NUFPAR *fp);
  static void parse_pointerradius(NUFPAR *fp);
  static void parse_magnificationfactor(NUFPAR *fp);
  static void parse_colour_mainline(NUFPAR *fp);
  static void parse_colour_nextlocationnode(NUFPAR *fp);
  static void parse_mapimagefilename(NUFPAR *fp);
  static void parse_overlayimagefilename(NUFPAR *fp);
  static void parse_episodeid(NUFPAR *fp);
  static void parse_specialobjectname(NUFPAR *fp);
  static void parse_mapimageaspectratio(NUFPAR *fp);
  void LoadSettings(char *name);
  static void parse_worldmap_start(NUFPAR *fp);
  void DumpLevel(WORLDINFO_s *world);
  i32 GetLocationId(i32 id) const;
  WMLOCATION_s *GetLocation(WMAREA_s const *area);
  i32 GetLastCompletedLocationId() const;
  void SetAreaOpenComplete(i32 area_id, i32 open, i32 complete);
  void FadeIn(f32 duration, f32 target);
  void FadeOut(f32 duration);

  void *vtable;
  u8 pad004[0x10 - 4];
  char level_name[0x40];      // 0x010
  char transition_name[0x40]; // 0x050
  u8 pad090[0x230 - 0x90];
  i32 reverse_direction; // 0x230
  u8 pad234[0x23c - 0x234];
  char map_name[0x40];     // 0x23c
  char overlay_name[0x40]; // 0x27c
  u8 pad2bc[0x344 - 0x2bc];
  i32 episode_id; // 0x344
  i32 mode;       // 0x348, 1 = interactive, 2 = playback
  u8 pad34c[0x360 - 0x34c];
  i32 i360; // 0x360, WorldMapInfo starts here
  u8 pad364[0x370 - 0x364];
  WMLOCATION_s locations[16]; // 0x370
  i32 location_count;         // 0x870
  // 0x874..0x8a8 sit in the WorldMapInfo member at 0x360 (its defaults are
  // set in WorldMapInfo::WorldMapInfo, 0x4f4540).
  f32 map_aspect_ratio;           // 0x874
  f32 zoom_playback_min;          // 0x878
  f32 zoom_playback_max;          // 0x87c
  f32 zoom_interactive_min;       // 0x880
  f32 zoom_interactive_max;       // 0x884
  f32 pointer_radius;             // 0x888
  f32 magnification;              // 0x88c
  WMCOLOUR_s colour_mainline;     // 0x890
  WMCOLOUR_s colour_nextlocation; // 0x89c
  u8 pad8a8[0x8b0 - 0x8a8];
  i32 fade_mode; // 0x8b0, 1 = in, 2 = out
  f32 f8b4;      // 0x8b4
  f32 fade_from; // 0x8b8
  f32 alpha;     // 0x8bc
  u8 pad8c0[0x8f0 - 0x8c0];
  f32 fade_target;   // 0x8f0
  f32 fade_duration; // 0x8f4
  u8 pad8f8[0x958 - 0x8f8];
  nugscn_s *scene; // 0x958
  struct {
    i32 a, b, c;
  } v95c; // 0x95c
};

// GLOBAL: LEGOBATMAN 0x00ad2af4
extern WorldMapBase *g_unk00ad2af4; // the world map being parsed
// GLOBAL: LEGOBATMAN 0x00968b38
extern u8 g_unk00968b38[]; // WorldMapBase keyword table
// GLOBAL: LEGOBATMAN 0x009c5870
extern i32 g_unk009c5870;

class InteractiveDisplay {
public:
  void DumpLevel(WORLDINFO_s *world);
};

void NuGScnRemove(nugscn_s *scene);

// FUNCTION: LEGOBATMAN 0x00678e00
void WorldMapBase::SetAreaOpenComplete(i32 area_id, i32 open, i32 complete) {
  for (i32 i = 0; i < location_count; i++) {
    if (locations[i].area != 0 && locations[i].area->id == area_id) {
      locations[i].complete = complete;
      locations[i].open = open;
    }
  }
}

// FUNCTION: LEGOBATMAN 0x00678fc0
void WorldMapBase::DumpLevel(WORLDINFO_s *world) {
  ((InteractiveDisplay *)this)->DumpLevel(world);
  if (NuStrICmp((char *)world, level_name) == 0) {
    if (scene != 0) {
      NuGScnRemove(scene);
      scene = 0;
      memset(&v95c, 0, sizeof(v95c));
    }
  }
}

// FUNCTION: LEGOBATMAN 0x006790c0
void WorldMapBase::FadeIn(f32 duration, f32 target) {
  if (duration == 0.0f) {
    alpha = 1.0f;
  } else {
    fade_duration = duration;
    fade_mode = 1;
    fade_target = -target;
    fade_from = alpha;
  }
}

// FUNCTION: LEGOBATMAN 0x00679110
void WorldMapBase::FadeOut(f32 duration) {
  if (duration == 0.0f) {
    alpha = 0.0f;
    mode = 0;
    fade_mode = 0;
  } else {
    fade_duration = duration;
    fade_mode = 2;
    fade_target = 0.0f;
    fade_from = alpha;
  }
}

// FUNCTION: LEGOBATMAN 0x00679510
i32 IsBonusArea(WMAREA_s *area) {
  if (area->flags & 4)
    return 1;
  if (area->flags & 0x40)
    return 0;
  return area->b8e < 0;
}

// FUNCTION: LEGOBATMAN 0x006794c0
WMLOCATION_s *WorldMapBase::GetLocation(WMAREA_s const *area) {
  if (area == 0)
    return 0;
  for (u32 i = 0; i < (u32)location_count; i++) {
    if (locations[i].area == area)
      return &locations[i];
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x00679540
i32 WorldMapBase::GetLocationId(i32 id) const {
  for (i32 i = 0; i < location_count; i++) {
    if (locations[i].id == id)
      return i;
  }
  return i360;
}

// FUNCTION: LEGOBATMAN 0x006797c0
i32 WorldMapBase::GetLastCompletedLocationId() const {
  i32 current = i360;
  i32 count = location_count;
  i32 last = -1;
  for (i32 i = 0; i < count; i++) {
    if (locations[i].area == 0 || (locations[i].area->flags & 4))
      break;
    if (i != current && locations[i].complete == 0)
      break;
    last = i;
  }
  return last;
}

// FUNCTION: LEGOBATMAN 0x00679810
void WorldMapBase::parse_worldmap_start(NUFPAR *fp) {
  WorldMapBase *map = g_unk00ad2af4;
  map->i360 = 0;
  map->location_count = 0;
  NuStrCpy(map->map_name, "");
}

// FUNCTION: LEGOBATMAN 0x00679840
void WorldMapBase::parse_worldmap_end(NUFPAR *fp) {}

// FUNCTION: LEGOBATMAN 0x00679850
void WorldMapBase::parse_levelpath(NUFPAR *fp) {
  if (NuFParGetWord(fp) && NuStrLen(fp->word_buf) < 0x40)
    NuStrCpy(g_unk00ad2af4->level_name, fp->word_buf);
}

// FUNCTION: LEGOBATMAN 0x006798a0
void WorldMapBase::parse_reversedirection(NUFPAR *fp) {
  g_unk00ad2af4->reverse_direction = 1;
}

// FUNCTION: LEGOBATMAN 0x006798b0
void WorldMapBase::parse_zoomplaybackminmax(NUFPAR *fp) {
  g_unk00ad2af4->zoom_playback_min = NuFParGetFloat(fp);
  g_unk00ad2af4->zoom_playback_max = NuFParGetFloat(fp);
}

// FUNCTION: LEGOBATMAN 0x006798e0
void WorldMapBase::parse_zoominteractiveminmax(NUFPAR *fp) {
  g_unk00ad2af4->zoom_interactive_min = NuFParGetFloat(fp);
  g_unk00ad2af4->zoom_interactive_max = NuFParGetFloat(fp);
}

// FUNCTION: LEGOBATMAN 0x00679910
void WorldMapBase::parse_pointerradius(NUFPAR *fp) {
  g_unk00ad2af4->pointer_radius = NuFParGetFloat(fp);
}

// FUNCTION: LEGOBATMAN 0x00679930
void WorldMapBase::parse_magnificationfactor(NUFPAR *fp) {
  g_unk00ad2af4->magnification = NuFParGetFloat(fp);
}

// STUB: LEGOBATMAN 0x00679950
// the struct copy goes through "add eax, 0x890" here; orig stores at
// [eax + 0x890] directly (field-wise copy is worse)
void WorldMapBase::parse_colour_mainline(NUFPAR *fp) {
  WMCOLOUR_s colour;
  colour.r = NuFParGetFloat(fp) / 255.0f;
  colour.g = NuFParGetFloat(fp) / 255.0f;
  colour.b = NuFParGetFloat(fp) / 255.0f;
  g_unk00ad2af4->colour_mainline = colour;
}

// STUB: LEGOBATMAN 0x006799c0
// the struct copy goes through "add eax, 0x890" here; orig stores at
// [eax + 0x890] directly (field-wise copy is worse)
void WorldMapBase::parse_colour_nextlocationnode(NUFPAR *fp) {
  WMCOLOUR_s colour;
  colour.r = NuFParGetFloat(fp) / 255.0f;
  colour.g = NuFParGetFloat(fp) / 255.0f;
  colour.b = NuFParGetFloat(fp) / 255.0f;
  g_unk00ad2af4->colour_nextlocation = colour;
}

// FUNCTION: LEGOBATMAN 0x00679a30
void WorldMapBase::parse_mapimagefilename(NUFPAR *fp) {
  if (NuFParGetWord(fp) && NuStrLen(fp->word_buf) < 0x40)
    NuStrCpy(g_unk00ad2af4->map_name, fp->word_buf);
}

// FUNCTION: LEGOBATMAN 0x00679a80
void WorldMapBase::parse_overlayimagefilename(NUFPAR *fp) {
  if (NuFParGetWord(fp) && NuStrLen(fp->word_buf) < 0x40)
    NuStrCpy(g_unk00ad2af4->overlay_name, fp->word_buf);
}

// FUNCTION: LEGOBATMAN 0x00679ad0
void WorldMapBase::parse_episodeid(NUFPAR *fp) {
  g_unk00ad2af4->episode_id = NuFParGetInt(fp);
}

// FUNCTION: LEGOBATMAN 0x00679af0
void WorldMapBase::parse_specialobjectname(NUFPAR *fp) {
  if (NuFParGetWord(fp) && NuStrLen(fp->word_buf) < 0x40 &&
      g_unk00ad2af4->transition_name[0] == 0)
    NuStrCpy(g_unk00ad2af4->transition_name, fp->word_buf);
}

// FUNCTION: LEGOBATMAN 0x00679b40
void WorldMapBase::parse_mapimageaspectratio(NUFPAR *fp) {
  g_unk00ad2af4->map_aspect_ratio = NuFParGetFloat(fp);
}

// FUNCTION: LEGOBATMAN 0x00679be0
void WorldMapBase::LoadSettings(char *name) {
  NUFPAR *fp = NuFParCreate(name);
  if (fp == 0) {
    Unk00678b90(name);
    g_unk009c5870 = 1;
    return;
  }
  g_unk00ad2af4 = this;
  NuFParPushCom(fp, g_unk00968b38);
  while (NuFParGetLine(fp)) {
    if (NuFParGetWord(fp))
      NuFParInterpretWord(fp);
  }
  NuFParDestroy(fp);
  g_unk00ad2af4 = 0;
}
