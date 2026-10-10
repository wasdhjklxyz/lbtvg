// batman/, file unknown: WorldMapBase's settings parser (Mac order:
// parse_worldmap_end .. parse_mapimageaspectratio, LoadSettings),
// 0x679840..0x679c6c.

#include "../nu2api/nucore/common.h"
#include "../nu2api/nucore/nustring.h"

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
  u8 pad348[0x874 - 0x348];
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
};

// GLOBAL: LEGOBATMAN 0x00ad2af4
extern WorldMapBase *g_unk00ad2af4; // the world map being parsed
// GLOBAL: LEGOBATMAN 0x00968b38
extern u8 g_unk00968b38[]; // WorldMapBase keyword table
// GLOBAL: LEGOBATMAN 0x009c5870
extern i32 g_unk009c5870;

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
