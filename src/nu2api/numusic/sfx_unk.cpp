// nu2api/numusic/sfx_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nucore/common.h"

typedef struct nusoundinfo_s {
  char *sfx_name; // 0x00
  u32 pad4[3];
  i16 next; // 0x10
  u16 pad12;
  u32 pad14[(0x40 - 0x14) / 4];
} NUSOUNDINFO;

extern "C" u32 CRC_ProcessStringIgnoreCase(const char *str);
i32 NuStrNICmp(const char *a, const char *b, i32 n);

// GLOBAL: LEGOBATMAN 0x009f7a30
extern i16 *g_soundMap;
// GLOBAL: LEGOBATMAN 0x00a0f7e4
extern NUSOUNDINFO *g_soundInfo;
extern "C" u32 *g_crc_table; // crc_unk.c

// STUB: LEGOBATMAN 0x00558d90
// close: block layout only; orig puts the map/flag "return -1" inline and
// does not align the loop head, this moves it to the end and pads the loop.
i32 GetSfxId(const char *name) {
  if (name == 0)
    return -1;
  NUSOUNDINFO *info = g_soundInfo;
  if (info == 0)
    return -1;
  i16 *map = g_soundMap;
  if (map == 0 || g_crc_table == 0)
    return -1;
  i32 index = map[CRC_ProcessStringIgnoreCase(name) & 0xff];
  if (index != -1) {
    do {
      if (NuStrNICmp(name, info[index].sfx_name, 32) == 0) {
        return index;
      }
      info = g_soundInfo;
      index = info[index].next;
    } while (index != -1);
  }

  return -1;
}

#include "../numath/nuvec.h"

void PlaySfxByIdEx(i32 sfx_id, nuvec_s *position, f32 volume, f32 pitch);

// from saga legoapi/audio/sfx.cpp
// FUNCTION: LEGOBATMAN 0x00559480
void PlaySfxAndSetVolume(char *name, nuvec_s *position, f32 volume) {
  i32 id = GetSfxId(name);
  if (id != -1) {
    PlaySfxByIdEx(id, position, volume, 1.0f);
  }
}

typedef struct nufpar_s {
  u32 pad0[0x910 / 4];
  char *word_buf; // 0x910
} NUFPAR;

i32 NuFParGetWord(NUFPAR *parser);
f32 NuFParGetFloat(NUFPAR *parser);
i32 NuFParPushCom(NUFPAR *parser, void *commands);
i32 NuStrICmp(const char *a, const char *b);
i32 NuStrCpy(char *dst, const char *src);
void NuStrNCpy(char *dst, const char *src, i32 n);
void *NuSoundParseReverbEffect(NUFPAR *parser, void *buffer, void *ptr);
void *NuSoundCreateReverbEffect(void *buffer, void *ptr);
i32 GroupBuffer_MakeGroup(i32 sfx_id);

typedef struct SoundGroup {
  i16 first_sample;
  i16 sample_count;
  i16 field_0x4;
  i16 field_0x6;
} SoundGroup;

typedef struct reverbdef_s {
  char name[0x20]; // 0x00
  void *effect;    // 0x20
} REVERBDEF;

// GLOBAL: LEGOBATMAN 0x00a28b48
static f32 g_audioVersion;
// GLOBAL: LEGOBATMAN 0x009e81c8
extern SoundGroup g_groups[];
// GLOBAL: LEGOBATMAN 0x009e8a08
extern i16 g_groupBuffer[];
// GLOBAL: LEGOBATMAN 0x00a28b38
extern i32 g_lenGroupBuffer;
// GLOBAL: LEGOBATMAN 0x00a28c60
extern i32 num_reverb_defs;
// GLOBAL: LEGOBATMAN 0x009f5f00
extern REVERBDEF reverb_defs[];
// GLOBAL: LEGOBATMAN 0x009e8948
extern char reverb_buffer[];
// GLOBAL: LEGOBATMAN 0x009f7800
extern char reverb_ptr[];
// GLOBAL: LEGOBATMAN 0x00a28b60
extern char g_audioBasePath[];
// GLOBAL: LEGOBATMAN 0x0095da20
extern char audioCom[];

// keyword "Audio" in table 0x0095da20
// FUNCTION: LEGOBATMAN 0x00559690
void fnAudioAudio(NUFPAR *fpar) { g_audioVersion = NuFParGetFloat(fpar); }

// keyword "Group" in table 0x0095da20
// FUNCTION: LEGOBATMAN 0x0055a060
void fnAudioGroup(NUFPAR *fpar) {
  i32 first = 1;
  i32 group_id = -1;

  while (NuFParGetWord(fpar) != 0) {
    i32 sfx_id = GetSfxId(fpar->word_buf);
    if (first) {
      if (sfx_id == -1) {
        return;
      }
      group_id = GroupBuffer_MakeGroup(sfx_id);
      first = 0;
    } else {
      sfx_id = GetSfxId(fpar->word_buf);
      if (sfx_id != -1 && g_lenGroupBuffer != 1200) {
        g_groupBuffer[g_lenGroupBuffer++] = sfx_id;
        g_groups[group_id].sample_count++;
        g_groups[group_id].field_0x4 = 0;
      }
    }
  }
}

// FUNCTION: LEGOBATMAN 0x0055a100
void fnReverb(NUFPAR *fpar) {
  NuFParGetWord(fpar);
  if (num_reverb_defs < 0x40) {
    NuStrNCpy(reverb_defs[num_reverb_defs].name, fpar->word_buf, 0x20);
    void *effect = NuSoundParseReverbEffect(fpar, reverb_buffer, reverb_ptr);
    if (effect != 0 ||
        (effect = NuSoundCreateReverbEffect(reverb_buffer, reverb_ptr)) != 0) {
      reverb_defs[num_reverb_defs++].effect = effect;
    }
  }
}

// FUNCTION: LEGOBATMAN 0x0055a200
void fnIfPlatform(NUFPAR *fpar) {
  i32 match = 0;
  NuFParGetWord(fpar);
  while (fpar->word_buf[0] != '\0' && fpar->word_buf[0] != ';') {
    if (NuStrICmp(fpar->word_buf, "WINPC") == 0) {
      match = 1;
    }
    NuFParGetWord(fpar);
  }
  if (!match) {
    NuFParPushCom(fpar, audioCom);
  }
}

// FUNCTION: LEGOBATMAN 0x0055a270
void fnBasePath(NUFPAR *fpar) {
  NuFParGetWord(fpar);
  NuStrCpy(g_audioBasePath, fpar->word_buf);
}
