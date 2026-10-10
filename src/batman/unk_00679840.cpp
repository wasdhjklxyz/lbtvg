// batman/, file unknown: WorldMapBase's settings parser (Mac order:
// parse_worldmap_end .. parse_mapimageaspectratio, LoadSettings),
// 0x679840..0x679c6c.

#include "../nu2api/nucore/common.h"
#include "../nu2api/nucore/nustring.h"
#include "../nu2api/numath/nuinline_unk.h"
#include <string.h>

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x00678c80
static f32 NuVecMagInline(f32 *v);

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x00678ba0
static void NuVec4Copy(f32 *dst, f32 *src);
// FUNCTION: LEGOBATMAN 0x00678bc0
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);
// FUNCTION: LEGOBATMAN 0x00678c30
static void NuVecScaleInline(f32 *dst, f32 *src, f32 s);

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
union variptr_u;

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

f32 NuFsqrt(f32 f);

// Mac: class VuVec.
struct __declspec(align(16)) VuVec {
  VuVec() {}
  VuVec(f32 x, f32 y, f32 z) : x(x), y(y), z(z) {}
  VuVec &operator=(const VuVec &v) {
    x = v.x;
    y = v.y;
    z = v.z;
    w = v.w;
    return *this;
  }
  VuVec operator-(const VuVec &v) const {
    return VuVec(x - v.x, y - v.y, z - v.z);
  }
  f32 LengthSq() const { return x * x + y * y + z * z; }
  f32 Length() const { return NuFsqrt(LengthSq()); }
  f32 x, y, z, w;
};

// Mac: WorldMapLocation (0x50 bytes, 16 of them in WorldMapInfo).
struct WorldMapLocation {
  WorldMapLocation &operator=(const WorldMapLocation &o);

  VuVec pos;      // 0x00
  VuVec v10;      // 0x10
  WMAREA_s *area; // 0x20
  f32 f24;        // 0x24
  i32 type;       // 0x28, 2 = not on the path
  i32 i2c;        // 0x2c
  i32 open;       // 0x30
  i32 complete;   // 0x34
  i32 id;         // 0x38
  f32 scale;      // 0x3c
  i32 scaling;    // 0x40
  i32 i44;        // 0x44
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
  i32 GetLocationId(int id) const;
  i32 GetLocationId(char *name) const;
  WorldMapLocation *GetLocation(WMAREA_s const *area);
  i32 GetLastCompletedLocationId() const;
  void SetAreaOpenComplete(i32 area_id, i32 open, i32 complete);
  void FadeIn(f32 duration, f32 target);
  void FadeOut(f32 duration);
  void UpdateLocationNodesScale(f32 dt, i32 reset);
  f32 GetTotalDistance(int from, int to);
  VuVec GetDefaultCameraPosition() const;
  void ZoomFull(f32 duration);
  void UpdatePointer(f32 dt);
  void Unk00679170(f32 dt);
  void Unk006792c0(f32 dt);
  // Slots 0..16 as in InteractiveDisplay (src/batman/unk_006000c0.cpp).
  virtual void InitializePerm(char *name, variptr_u *buf, variptr_u *end);
  virtual void InitializeLevel(WORLDINFO_s *world);
  virtual void ActivateLevel(WORLDINFO_s *world);
  virtual void DumpLevel(WORLDINFO_s *world);
  virtual i32 GetDoesLevelLoadRender() const;
  virtual const char *GetClassNameA() const;
  virtual i32 GetUsesStrobePattern() const;
  virtual i32 GetUsesWhiteNoise() const;
  virtual i32 GetUsesInterlacePattern() const;
  virtual i32 GetUsesOverlayTexture() const;
  virtual void Update(f32 dt);
  virtual void Render();
  virtual i32 ShouldUpdate() const;
  virtual i32 IsVisible() const;
  virtual i32 RenderWhenPaused() const;
  virtual i32 IsCameraTarget() const;
  virtual i32 IsInteractiveMode() const;

  // members start at 0x10: the vfptr is padded to the 16-byte alignment
  char level_name[0x40];      // 0x010
  char transition_name[0x40]; // 0x050
  u8 pad090[0x230 - 0x90];
  i32 reverse_direction; // 0x230
  u8 pad234[0x23c - 0x234];
  char map_name[0x40];     // 0x23c
  char overlay_name[0x40]; // 0x27c
  u8 pad2bc[0x340 - 0x2bc];
  i32 active;     // 0x340
  i32 episode_id; // 0x344
  i32 mode;       // 0x348, 1 = interactive, 2 = playback
  u8 pad34c[0x360 - 0x34c];
  i32 i360; // 0x360, WorldMapInfo starts here
  u8 pad364[0x370 - 0x364];
  WorldMapLocation locations[16]; // 0x370
  i32 location_count;             // 0x870
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
  u8 pad8c0[0x8c4 - 0x8c0];
  f32 zoom_rate;     // 0x8c4
  f32 zoom_scale;    // 0x8c8
  VuVec zoom_from;   // 0x8d0
  VuVec zoom_to;     // 0x8e0
  f32 fade_target;   // 0x8f0
  f32 fade_duration; // 0x8f4
  u8 pad8f8[0x910 - 0x8f8];
  VuVec camera_pos; // 0x910
  f32 f920;         // 0x920
  u8 pad924[0x958 - 0x924];
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
// GLOBAL: LEGOBATMAN 0x00ab093c
extern i32 Paused;

class InteractiveDisplay {
public:
  void DumpLevel(WORLDINFO_s *world);
  void Update(f32 dt);
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

// STUB: LEGOBATMAN 0x00678e50
// only the min/max compare operand order differs (cmp from, to; jl)
f32 WorldMapBase::GetTotalDistance(int from, int to) {
  f32 total = 0.0f;
  int lo = to > from ? from : to;
  int hi = to < from ? from : to;
  for (int i = lo; i < hi; i++) {
    if (locations[i].type != 2) {
      VuVec d = locations[i + 1].pos - locations[i].pos;
      d.z = 0.0f;
      total += d.Length();
    }
  }
  return total;
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

static inline f32 WMMin(f32 a, f32 b) { return a > b ? b : a; }
static inline f32 WMMax(f32 a, f32 b) { return a < b ? b : a; }

// STUB: LEGOBATMAN 0x006793f0
// orig keeps separate x87 copies of the 1.0/0.0 clamp constants; ours merges
void WorldMapBase::UpdateLocationNodesScale(f32 dt, i32 reset) {
  for (i32 i = 0; i < location_count; i++) {
    if (reset) {
      locations[i].scale = 0.0f;
      locations[i].scaling = 0;
    } else if (locations[i].scaling != 0) {
      locations[i].scale += dt / 0.075f;
      locations[i].scale = WMMin(locations[i].scale, 1.0f);
      locations[i].scale = WMMax(locations[i].scale, 0.0f);
    }
  }
}

// FUNCTION: LEGOBATMAN 0x006794c0
WorldMapLocation *WorldMapBase::GetLocation(WMAREA_s const *area) {
  if (area == 0)
    return 0;
  for (u32 i = 0; i < (u32)location_count; i++) {
    if (locations[i].area == area)
      return &locations[i];
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x00679540
i32 WorldMapBase::GetLocationId(int id) const {
  for (i32 i = 0; i < location_count; i++) {
    if (locations[i].id == id)
      return i;
  }
  return i360;
}

// FUNCTION: LEGOBATMAN 0x00679580
i32 WorldMapBase::GetLocationId(char *name) const {
  for (i32 i = 0; i < location_count; i++) {
    if (locations[i].area != 0 && NuStrICmp(name, locations[i].area->name) == 0)
      return i;
  }
  return -1;
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

// FUNCTION: LEGOBATMAN 0x00679b60
WorldMapLocation &WorldMapLocation::operator=(const WorldMapLocation &o) {
  pos = o.pos;
  v10 = o.v10;
  area = o.area;
  f24 = o.f24;
  type = o.type;
  i2c = o.i2c;
  open = o.open;
  complete = o.complete;
  id = o.id;
  scale = o.scale;
  scaling = o.scaling;
  i44 = o.i44;
  return *this;
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

// FUNCTION: LEGOBATMAN 0x00679d90
void WorldMapBase::ZoomFull(f32 duration) {
  fade_duration = duration;
  fade_mode = 4;
  zoom_rate = 1.0f / f920;
  zoom_scale = zoom_interactive_max;
  fade_target = 0.0f;
  zoom_from = camera_pos;
  zoom_to = GetDefaultCameraPosition();
}

// FUNCTION: LEGOBATMAN 0x0067a360
void WorldMapBase::Update(f32 dt) {
  if (active == 0)
    return;
  ((InteractiveDisplay *)this)->Update(dt);
  if (Paused && !IsInteractiveMode())
    dt = 0.0f;
  UpdatePointer(dt);
  UpdateLocationNodesScale(dt, 0);
  Unk00679170(dt);
  Unk006792c0(dt);
}

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_unk_00679840(f32 *v, f32 a, i32 i) {
  NuVec4Copy(v + 4, v);
  NuVec4Set(v, a, a, a, a);
  NuVecScaleInline(v + 8, v, a);
}

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_2_unk_00679840(f32 *v, f32 a, i32 i) {
  v[48] = NuVecMagInline(v);
}
