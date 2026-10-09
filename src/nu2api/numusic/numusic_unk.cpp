// nu2api/numusic/numusic_unk.cpp: NuMusic config parsers (saga
// nu2api/numusic/numusic.cpp); between nusound.cpp and oggreader.cpp by link
// order, file name unproven.

#include "../nucore/common.h"
#include "../nucore/nustring.h"

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
void NuFParSetInterpreterErrorHandler(void (*handler)(NUFPAR *parser));

// Empty in this build; called when a table is full.
// FUNCTION: LEGOBATMAN 0x00536250
static void Unk00536250(...) {}

struct Track {
  u8 pad0[8];
  char *ident; // 0x08
  u8 padc[0x1c - 0xc];
  i32 entry_count; // 0x1c
  u8 pad20[0x38 - 0x20];
  u32 flags; // 0x38, 1 = no duck, 2 = looping
};

struct Album {
  char *name;           // 0x00
  Track *tracks_source; // 0x04
  i32 tracks_count;     // 0x08
  u8 padc[0x24 - 0xc];
};

struct NuMusic {
  Album *albums;   // 0x00
  i32 album_count; // 0x04
  Track *tracks;   // 0x08
  i32 track_count; // 0x0c
  f32 *indexes;    // 0x10
  i32 index_count; // 0x14
  u8 pad18[0x1c - 0x18];
  Album *current_album;     // 0x1c
  Track *current_track;     // 0x20
  char current_path[0x100]; // 0x24
  u8 strict;                // 0x124
  u8 pad125[0x12c - 0x125];
  char *string_pool_end; // 0x12c

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
