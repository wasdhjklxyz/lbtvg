// nu2api/nusound/nusound3_include_unk.cpp: placed by tools/new.py; file name
// unproven.

#include "../nucore/common.h"

typedef struct nusound_filename_info_s nusound_filename_info_s;

struct MusicManager {
  void Initialise(char *cfg, void *unk, VARIPTR *buffer_start,
                  VARIPTR buffer_end);
  void GetSoundFiles(nusound_filename_info_s **finfo, void *unk);
};

// GLOBAL: LEGOBATMAN 0x00a95dc0
extern MusicManager music_man;
// GLOBAL: LEGOBATMAN 0x0095d910
extern char *audio_ps2_music_ext;
// GLOBAL: LEGOBATMAN 0x00a958f0
extern i16 *ActionPairTab;
// GLOBAL: LEGOBATMAN 0x00a95d84
extern i16 *AmbientPairTab;

// FUNCTION: LEGOBATMAN 0x005a0830
nusound_filename_info_s *ConfigureMusic(char *file, VARIPTR *bufferStart,
                                        VARIPTR *bufferEnd) {
  nusound_filename_info_s *finfo;

  music_man.Initialise("audio\\music.cfg", 0, bufferStart, *bufferEnd);
  music_man.GetSoundFiles(&finfo, 0);

  audio_ps2_music_ext = ".mib";

  u8 *p = (u8 *)((bufferStart->addr + 3) & ~3);
  *(i32 *)p = 0;
  p += 0x1c;
  *(i16 *)p = -1;
  ActionPairTab = (i16 *)p;
  p += 0x1c;
  *(i16 *)p = -1;
  AmbientPairTab = (i16 *)p;
  p += 4;
  bufferStart->void_ptr = p;

  return finfo;
}
