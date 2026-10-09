// nu2api/numusic/numusic_unk.cpp: NuMusic config parsers (saga
// nu2api/numusic/numusic.cpp); between nusound.cpp and oggreader.cpp by link
// order, file name unproven.

#include "../nucore/common.h"
#include "../nucore/nustring.h"
#include <stddef.h>
#include <string.h>

typedef struct nufpar_s {
  u8 pad0[8];
  char name[0x100]; // 0x08
  u8 pad108[0x910 - 0x108];
  char *word_buf; // 0x910
  u8 pad914[0x91c - 0x914];
  i32 line_num; // 0x91c
} NUFPAR;

i32 NuFParGetWord(NUFPAR *parser);
f32 NuFParGetFloatRDP(NUFPAR *parser);
typedef void (*NUFPCOMFN)(NUFPAR *parser);
NUFPCOMFN NuFParSetInterpreterErrorHandler(NUFPCOMFN handler);
i32 NuFParPushComCTX(NUFPAR *parser, void *commands);
void NuFParPopCom(NUFPAR *parser);
void NuFParInterpretWordCTX(NUFPAR *parser, void *ctx);

// Empty in this build; called when a table is full.
// FUNCTION: LEGOBATMAN 0x00536250
static void Unk00536250(...) {}

struct Track {
  char *path;  // 0x00
  char *name;  // 0x04
  char *ident; // 0x08
  u8 padc[0x14 - 0xc];
  u32 clazz;        // 0x14
  f32 *entry_times; // 0x18
  i32 entry_count;  // 0x1c
  u8 pad20[0x28 - 0x20];
  i32 pitch;       // 0x28
  f32 duck_volume; // 0x2c
  f32 duck_fade;   // 0x30
  f32 attenuation; // 0x34
  u32 flags;       // 0x38, 1 = no duck, 2 = looping
};

struct Album {
  char *name;           // 0x00
  Track *tracks_source; // 0x04
  i32 tracks_count;     // 0x08
  u8 padc[0x24 - 0xc];
};

struct NuMusic {
  Album *albums;            // 0x00
  i32 album_count;          // 0x04
  Track *tracks;            // 0x08
  i32 track_count;          // 0x0c
  f32 *indexes;             // 0x10
  i32 index_count;          // 0x14
  i32 pitch_default;        // 0x18
  Album *current_album;     // 0x1c
  Track *current_track;     // 0x20
  char current_path[0x100]; // 0x24
  u8 strict;                // 0x124
  u8 pad125[0x12c - 0x125];
  char *string_pool_end; // 0x12c
  u8 pad130[0x1cc - 0x130];
  f32 global_attenuation; // 0x1cc
  u8 pad1d0[0x1d4 - 0x1d0];
  char *language; // 0x1d4

  void SubstituteString(char *dst, char *src, char *find, char *replace);

  char *FindString(const char *str);
  char *AllocString(const char *str) {
    char *ptr = FindString(str);
    if (ptr == 0) {
      ptr = string_pool_end;
      string_pool_end += NuStrLen(str) + 1;
      NuStrCpy(ptr, str);
    }
    return ptr;
  }

  static void GlobalParseErrorFn(NUFPAR *parser);
  static void TrackParseErrorFn(NUFPAR *parser);
  void ParseTrack(i32 track_class, NUFPAR *parser);

  void xIndex(NUFPAR *parser) {
    if (index_count >= 0x800)
      Unk00536250();
    indexes[index_count] = NuFParGetFloatRDP(parser);
    current_track->entry_count++;
    index_count++;
  }
  void xNoDuck(NUFPAR *parser) { current_track->flags |= 1; }
  void xLooping(NUFPAR *parser) { current_track->flags |= 2; }
  void xNonLooping(NUFPAR *parser) { current_track->flags &= ~2; }
  void xPath(NUFPAR *parser) {
    NuFParGetWord(parser);
    NuStrCpy(current_path, parser->word_buf);
  }
  void xStrict(NUFPAR *parser) {
    strict = 1;
    NuFParSetInterpreterErrorHandler(GlobalParseErrorFn);
  }
  void xIdent(NUFPAR *parser);
  void xNoMusic(NUFPAR *parser);
  void xDuck(NUFPAR *parser);
  void xAttenuation(NUFPAR *parser);
  void xAlbum(NUFPAR *parser);
  void xAction(NUFPAR *parser) { ParseTrack(2, parser); }
  void xQuiet(NUFPAR *parser) { ParseTrack(1, parser); }
  void xOverlay(NUFPAR *parser) { ParseTrack(8, parser); }
  void xSignature(NUFPAR *parser) { ParseTrack(4, parser); }
  void xCutscene(NUFPAR *parser) { ParseTrack(0x10, parser); }
  void xNoMusicC(NUFPAR *parser) { ParseTrack(0x20, parser); }
  void xGlobalAttenuation(NUFPAR *parser);

  static void xsIndex(NUFPAR *parser, void *thisptr);
  static void xsNoDuck(NUFPAR *parser, void *thisptr);
  static void xsLooping(NUFPAR *parser, void *thisptr);
  static void xsNonLooping(NUFPAR *parser, void *thisptr);
  static void xsPath(NUFPAR *parser, void *thisptr);
  static void xsStrict(NUFPAR *parser, void *thisptr);
  static void xsIdent(NUFPAR *parser, void *thisptr);
  static void xsNoMusic(NUFPAR *parser, void *thisptr);
  static void xsDuck(NUFPAR *parser, void *thisptr);
  static void xsAttenuation(NUFPAR *parser, void *thisptr);
  static void xsAlbum(NUFPAR *parser, void *thisptr);
  static void xsAction(NUFPAR *parser, void *thisptr);
  static void xsQuiet(NUFPAR *parser, void *thisptr);
  static void xsOverlay(NUFPAR *parser, void *thisptr);
  static void xsSignature(NUFPAR *parser, void *thisptr);
  static void xsCutscene(NUFPAR *parser, void *thisptr);
  static void xsNoMusicC(NUFPAR *parser, void *thisptr);
  static void xsGlobalAttenuation(NUFPAR *parser, void *thisptr);
};

// FUNCTION: LEGOBATMAN 0x00536cd0
void NuMusic::GlobalParseErrorFn(NUFPAR *parser) {
  Unk00536250(parser->word_buf, parser->line_num, parser->name);
}

// FUNCTION: LEGOBATMAN 0x00536cf0
void NuMusic::TrackParseErrorFn(NUFPAR *parser) {
  Unk00536250(parser->word_buf, parser->line_num, parser->name);
}

// FUNCTION: LEGOBATMAN 0x00538970
void NuMusic::xsIndex(NUFPAR *parser, void *thisptr) {
  ((NuMusic *)thisptr)->xIndex(parser);
}

// FUNCTION: LEGOBATMAN 0x005389b0
void NuMusic::xsNoDuck(NUFPAR *parser, void *thisptr) {
  ((NuMusic *)thisptr)->xNoDuck(parser);
}

// FUNCTION: LEGOBATMAN 0x005389c0
void NuMusic::xsLooping(NUFPAR *parser, void *thisptr) {
  ((NuMusic *)thisptr)->xLooping(parser);
}

// FUNCTION: LEGOBATMAN 0x005389d0
void NuMusic::xsNonLooping(NUFPAR *parser, void *thisptr) {
  ((NuMusic *)thisptr)->xNonLooping(parser);
}

// FUNCTION: LEGOBATMAN 0x00538ea0
void NuMusic::xsPath(NUFPAR *parser, void *thisptr) {
  ((NuMusic *)thisptr)->xPath(parser);
}

// FUNCTION: LEGOBATMAN 0x00538ed0
void NuMusic::xsStrict(NUFPAR *parser, void *thisptr) {
  ((NuMusic *)thisptr)->xStrict(parser);
}

// FUNCTION: LEGOBATMAN 0x005390d0
void NuMusic::xsIdent(NUFPAR *parser, void *thisptr) {
  ((NuMusic *)thisptr)->xIdent(parser);
}

// FUNCTION: LEGOBATMAN 0x005390e0
void NuMusic::xsNoMusic(NUFPAR *parser, void *thisptr) {
  ((NuMusic *)thisptr)->xNoMusic(parser);
}

// FUNCTION: LEGOBATMAN 0x005390f0
void NuMusic::xsDuck(NUFPAR *parser, void *thisptr) {
  ((NuMusic *)thisptr)->xDuck(parser);
}

// FUNCTION: LEGOBATMAN 0x00539100
void NuMusic::xsAttenuation(NUFPAR *parser, void *thisptr) {
  ((NuMusic *)thisptr)->xAttenuation(parser);
}

// FUNCTION: LEGOBATMAN 0x00539110
void NuMusic::xsAlbum(NUFPAR *parser, void *thisptr) {
  ((NuMusic *)thisptr)->xAlbum(parser);
}

// FUNCTION: LEGOBATMAN 0x00539120
void NuMusic::xsAction(NUFPAR *parser, void *thisptr) {
  ((NuMusic *)thisptr)->xAction(parser);
}

// FUNCTION: LEGOBATMAN 0x00539140
void NuMusic::xsQuiet(NUFPAR *parser, void *thisptr) {
  ((NuMusic *)thisptr)->xQuiet(parser);
}

// FUNCTION: LEGOBATMAN 0x00539160
void NuMusic::xsOverlay(NUFPAR *parser, void *thisptr) {
  ((NuMusic *)thisptr)->xOverlay(parser);
}

// FUNCTION: LEGOBATMAN 0x00539180
void NuMusic::xsSignature(NUFPAR *parser, void *thisptr) {
  ((NuMusic *)thisptr)->xSignature(parser);
}

// FUNCTION: LEGOBATMAN 0x005391a0
void NuMusic::xsCutscene(NUFPAR *parser, void *thisptr) {
  ((NuMusic *)thisptr)->xCutscene(parser);
}

// FUNCTION: LEGOBATMAN 0x005391c0
void NuMusic::xsNoMusicC(NUFPAR *parser, void *thisptr) {
  ((NuMusic *)thisptr)->xNoMusicC(parser);
}

// FUNCTION: LEGOBATMAN 0x005391e0
void NuMusic::xsGlobalAttenuation(NUFPAR *parser, void *thisptr) {
  ((NuMusic *)thisptr)->xGlobalAttenuation(parser);
}

// FUNCTION: LEGOBATMAN 0x005389e0
void NuMusic::xIdent(NUFPAR *parser) {
  NuFParGetWord(parser);
  current_track->ident = AllocString(parser->word_buf);
}

f32 NuExp10(f32 x);

static inline f32 NuSoundSystem_dBToAmplitude(f32 db) {
  if (db <= -100.0)
    return 0.0f;
  if (db >= 0.0f)
    return 1.0f;
  return NuExp10(db / 20.0f);
}

// FUNCTION: LEGOBATMAN 0x00538a40
void NuMusic::xNoMusic(NUFPAR *parser) {
  NuFParGetWord(parser);
  char buf[256];
  SubstituteString(buf, parser->word_buf, "$lang", language);
  current_track->name = AllocString(buf);
}

// FUNCTION: LEGOBATMAN 0x00538af0
void NuMusic::xDuck(NUFPAR *parser) {
  current_track->duck_volume = NuFParGetFloatRDP(parser);
  if (current_track->duck_volume < 0.0f)
    current_track->duck_volume =
        NuSoundSystem_dBToAmplitude(current_track->duck_volume);
  current_track->duck_fade = NuFParGetFloatRDP(parser);
}

// FUNCTION: LEGOBATMAN 0x00538be0
void NuMusic::xAttenuation(NUFPAR *parser) {
  current_track->attenuation = NuFParGetFloatRDP(parser);
  if (current_track->attenuation < 0.0f)
    current_track->attenuation =
        NuSoundSystem_dBToAmplitude(current_track->attenuation);
}

// track keyword table
extern u8 track_jmp_tab[];

static inline char *RemovePath(char *path) {
  char *last = NULL;
  for (char *p = path; *p != '\0'; p++) {
    if (*p == '\\' || *p == '/')
      last = p;
  }
  if (last != NULL)
    return last + 1;
  return path;
}

// FUNCTION: LEGOBATMAN 0x00538c90
void NuMusic::ParseTrack(i32 track_class, NUFPAR *parser) {
  NUFPCOMFN prev_handler = NULL;
  if (track_count >= 0x800)
    Unk00536250();
  current_track = &tracks[track_count++];
  if (current_album != NULL)
    current_album->tracks_count++;
  memset(current_track, 0, sizeof(Track));
  current_track->entry_times = indexes + index_count;
  current_track->clazz = track_class;
  current_track->pitch = pitch_default;
  current_track->duck_volume = 1.0f;
  current_track->duck_fade = 1.0f;
  current_track->attenuation = 1.0f;
  if (track_class == 4 || track_class == 8 || track_class == 0x10)
    current_track->flags &= ~2;
  else
    current_track->flags |= 2;
  char path[256];
  char buf[256];
  NuFParGetWord(parser);
  NuStrCpy(buf, current_path);
  NuStrCat(buf, parser->word_buf);
  SubstituteString(path, buf, "$lang", language);
  current_track->path = AllocString(path);
  current_track->ident = RemovePath(current_track->path);
  if (strict)
    prev_handler = NuFParSetInterpreterErrorHandler(TrackParseErrorFn);
  NuFParPushComCTX(parser, track_jmp_tab);
  while (*parser->word_buf != '\0' && *parser->word_buf != ';') {
    NuFParGetWord(parser);
    NuFParInterpretWordCTX(parser, this);
  }
  NuFParPopCom(parser);
  if (strict)
    NuFParSetInterpreterErrorHandler(prev_handler);
}

// FUNCTION: LEGOBATMAN 0x00538ff0
void NuMusic::xGlobalAttenuation(NUFPAR *parser) {
  global_attenuation = NuFParGetFloatRDP(parser);
  if (global_attenuation < 0.0f)
    global_attenuation = NuSoundSystem_dBToAmplitude(global_attenuation);
}

// FUNCTION: LEGOBATMAN 0x00538ef0
void NuMusic::xAlbum(NUFPAR *parser) {
  if (album_count > 0x200)
    Unk00536250();
  current_album = &albums[album_count++];
  current_album->tracks_source = &tracks[track_count];
  current_album->tracks_count = 0;
  NuFParGetWord(parser);
  current_album->name = AllocString(parser->word_buf);
}
