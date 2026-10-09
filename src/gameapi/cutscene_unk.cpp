// gameapi/, file unknown: cut scene script keyword parsers
// (0x00618d00..0x0061a040), reached through the CS_ keyword table.

#include "../nu2api/nucore/common.h"
#include "../nu2api/nucore/nustring.h"

typedef struct nufpar_s NUFPAR;

i32 NuFParGetInt(NUFPAR *parser);
f32 NuFParGetFloat(NUFPAR *parser);

struct CUTINFO_s {
  u8 pad0[0x4f];
  u8 end_flags; // 0x4f
  u32 flags;    // 0x50
  u8 pad54[0x60 - 0x54];
  f32 frames_per_second; // 0x60
  f32 burnout_threshold; // 0x64
  f32 burnout_intensity; // 0x68
  f32 burnout_flare;     // 0x6c
  f32 near_clip;         // 0x70
  u8 pad74[0xec - 0x74];
  u16 far_clip; // 0xec
  u8 padee[0xf4 - 0xee];
  i16 goto_level;           // 0xf4
  i16 skip_level;           // 0xf6
  i16 suit_character;       // 0xf8
  u8 linked_audio;          // 0xfa
  u8 blob_shadow_alpha;     // 0xfb
  u8 blob_shadow_fadenear;  // 0xfc
  u8 blob_shadow_fadefar;   // 0xfd
  u8 reflect_range;         // 0xfe
  u8 render_group;          // 0xff
  char door_name[0x10];     // 0x100
  char next_cutscene[0x40]; // 0x110
  u8 pad150[0x170 - 0x150];
  struct {
    f32 time, r, g, b;
  } fades[2];      // 0x170
  u8 fade_type[2]; // 0x190
  u8 pad192[0x198 - 0x192];
  i32 music_handle; // 0x198
  f32 stop_debris;  // 0x19c
  u8 pad1a0[0x1a4 - 0x1a0];
  f32 fade_up_time;   // 0x1a4
  f32 fade_down_time; // 0x1a8
};

// GLOBAL: LEGOBATMAN 0x00acb77c
static CUTINFO_s *CS_CutInfo;

struct nufpar_s {
  unsigned char pad0[0x910];
  char *word_buf; // 0x910
};

i32 NuFParGetWord(NUFPAR *parser);
f32 NuAToF(char *string);

// GLOBAL: LEGOBATMAN 0x0096397c
extern f32 g_unk0096397c;

// FUNCTION: LEGOBATMAN 0x00618d00
void CS_fpsec(NUFPAR *parser) {
  CS_CutInfo->frames_per_second = NuFParGetFloat(parser);
}

// FUNCTION: LEGOBATMAN 0x00618f10
void CS_in_game(NUFPAR *parser) {
  CS_CutInfo->flags = (CS_CutInfo->flags & ~3) | 0x800;
}

// FUNCTION: LEGOBATMAN 0x00618f30
void CS_blobshadow_alpha(NUFPAR *parser) {
  i32 alpha = NuFParGetInt(parser);
  if (alpha > 0xfe)
    alpha = 0xfe;
  else if (alpha < 0)
    alpha = 0;
  CS_CutInfo->blob_shadow_alpha = alpha;
}

// FUNCTION: LEGOBATMAN 0x00618f70
void CS_blobshadow_fadenear(NUFPAR *parser) {
  CS_CutInfo->blob_shadow_fadenear = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x00618f90
void CS_blobshadow_fadefar(NUFPAR *parser) {
  CS_CutInfo->blob_shadow_fadefar = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x00618fb0
void CS_reflect_range(NUFPAR *parser) {
  CS_CutInfo->reflect_range = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x00618fd0
void CS_render_group(NUFPAR *parser) {
  CS_CutInfo->render_group = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x00618ff0
void CS_deb_page(NUFPAR *parser) {
  if (NuFParGetWord(parser) != 0) {
    if (NuStrICmp(parser->word_buf, "level") == 0)
      CS_CutInfo->flags |= 8;
    else
      CS_CutInfo->flags &= ~8;
  }
}

// FUNCTION: LEGOBATMAN 0x00619060
void CS_fade_up_at_start(NUFPAR *parser) {
  CS_CutInfo->fade_up_time = g_unk0096397c;
  if (NuFParGetWord(parser) != 0)
    CS_CutInfo->fade_up_time = NuAToF(parser->word_buf);
}

// FUNCTION: LEGOBATMAN 0x006190a0
void CS_fade_down_at_end(NUFPAR *parser) {
  CS_CutInfo->fade_down_time = g_unk0096397c;
  if (NuFParGetWord(parser) != 0)
    CS_CutInfo->fade_down_time = NuAToF(parser->word_buf);
}

// FUNCTION: LEGOBATMAN 0x00619030
void CS_stop_debris(NUFPAR *parser) {
  CS_CutInfo->stop_debris = NuFParGetFloat(parser);
}

// FUNCTION: LEGOBATMAN 0x00619150
void CS_burnout_threshold(NUFPAR *parser) {
  CS_CutInfo->burnout_threshold = NuFParGetFloat(parser);
  CS_CutInfo->flags |= 0x80;
}

// FUNCTION: LEGOBATMAN 0x00619170
void CS_burnout_intensity(NUFPAR *parser) {
  CS_CutInfo->burnout_intensity = NuFParGetFloat(parser);
  CS_CutInfo->flags |= 0x80;
}

// FUNCTION: LEGOBATMAN 0x00619190
void CS_burnout_flare(NUFPAR *parser) {
  CS_CutInfo->burnout_flare = NuFParGetFloat(parser);
  CS_CutInfo->flags |= 0x80;
}

// FUNCTION: LEGOBATMAN 0x00618d20
void CS_no_fog(NUFPAR *parser) { CS_CutInfo->flags |= 4; }

// FUNCTION: LEGOBATMAN 0x00618d30
void CS_draw_world(NUFPAR *parser) {
  if (NuFParGetWord(parser) != 0) {
    if (NuStrICmp(parser->word_buf, "on") == 0)
      CS_CutInfo->flags |= 2;
    else if (NuStrICmp(parser->word_buf, "off") == 0)
      CS_CutInfo->flags &= ~2;
  }
}

// FUNCTION: LEGOBATMAN 0x00618d90
void CS_draw_gizmo_sys(NUFPAR *parser) {
  if (NuFParGetWord(parser) != 0) {
    if (NuStrICmp(parser->word_buf, "on") == 0)
      CS_CutInfo->flags |= 0x8000;
    else if (NuStrICmp(parser->word_buf, "off") == 0)
      CS_CutInfo->flags &= ~0x8000;
  }
}

// FUNCTION: LEGOBATMAN 0x00618df0
void CS_update_gizmo_flow(NUFPAR *parser) {
  if (NuFParGetWord(parser) != 0) {
    if (NuStrICmp(parser->word_buf, "on") == 0)
      CS_CutInfo->flags |= 0x20000;
    else if (NuStrICmp(parser->word_buf, "off") == 0)
      CS_CutInfo->flags &= ~0x20000;
  }
}

// FUNCTION: LEGOBATMAN 0x00618e50
void CS_always_load(NUFPAR *parser) { CS_CutInfo->flags |= 0x10000; }

// FUNCTION: LEGOBATMAN 0x00618e60
void CS_unskippable(NUFPAR *parser) { CS_CutInfo->flags |= 0x40000; }

// FUNCTION: LEGOBATMAN 0x00619050
void CS_snap_out(NUFPAR *parser) { CS_CutInfo->flags |= 0x10; }

// FUNCTION: LEGOBATMAN 0x006190e0
void CS_cam_only(NUFPAR *parser) { CS_CutInfo->flags |= 0x20; }

// FUNCTION: LEGOBATMAN 0x006190f0
void CS_wipe_out(NUFPAR *parser) { CS_CutInfo->flags |= 0x100; }

// FUNCTION: LEGOBATMAN 0x00619100
void CS_new_mode(NUFPAR *parser) { CS_CutInfo->flags |= 0x400; }

// FUNCTION: LEGOBATMAN 0x00619110
void CS_replace_players(NUFPAR *parser) { CS_CutInfo->flags |= 0x40; }

// FUNCTION: LEGOBATMAN 0x00619120
void CS_looping(NUFPAR *parser) { CS_CutInfo->flags |= 0x200; }

// FUNCTION: LEGOBATMAN 0x00619130
void CS_start_cam(NUFPAR *parser) { CS_CutInfo->flags |= 0x2000; }

// FUNCTION: LEGOBATMAN 0x00619140
void CS_super_widescreen(NUFPAR *parser) { CS_CutInfo->flags |= 0x4000; }

// FUNCTION: LEGOBATMAN 0x006191b0
void CS_go_through_door(NUFPAR *parser) {
  if (NuFParGetWord(parser) != 0 && NuStrLen(parser->word_buf) < 0x10)
    NuStrCpy(CS_CutInfo->door_name, parser->word_buf);
}

struct MusicManager {
  i32 GetTrackHandle(i32 track_class, char *name);
};

extern MusicManager music_man;

// FUNCTION: LEGOBATMAN 0x00619200
void CS_sfx(NUFPAR *parser) {
  if (NuFParGetWord(parser) != 0)
    CS_CutInfo->music_handle = music_man.GetTrackHandle(0x10, parser->word_buf);
}

// FUNCTION: LEGOBATMAN 0x006194f0
void CS_next_cut_scene(NUFPAR *parser) {
  if (NuFParGetWord(parser) != 0 && NuStrLen(parser->word_buf) < 0x40 &&
      NuStrICmp(parser->word_buf, CS_CutInfo->next_cutscene) != 0) {
    NuStrCpy(CS_CutInfo->next_cutscene, parser->word_buf);
    CS_CutInfo->linked_audio = 1;
  }
}

i32 CharIDFromName(char *name);

// FUNCTION: LEGOBATMAN 0x00618e70
void CS_replace_suits(NUFPAR *parser) {
  CS_CutInfo->flags |= 0x80000;
  while (NuFParGetWord(parser) != 0) {
    if (NuStrICmp(parser->word_buf, "no_offsets") == 0) {
      CS_CutInfo->flags |= 0x100000;
    } else if (NuStrICmp(parser->word_buf, "only") == 0) {
      if (NuFParGetWord(parser) != 0)
        CS_CutInfo->suit_character = CharIDFromName(parser->word_buf);
    }
  }
}

// FUNCTION: LEGOBATMAN 0x00619570
void CS_level_intro(NUFPAR *parser) { CS_CutInfo->flags |= 0x1000; }

// FUNCTION: LEGOBATMAN 0x00619580
void CS_nearclip(NUFPAR *parser) {
  CS_CutInfo->near_clip = NuFParGetFloat(parser);
}

// FUNCTION: LEGOBATMAN 0x006195a0
void CS_farclip(NUFPAR *parser) {
  CS_CutInfo->far_clip = (u16)NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x006195c0
void CS_hold_audio(NUFPAR *parser) { CS_CutInfo->linked_audio = 1; }

// FUNCTION: LEGOBATMAN 0x0061a020
void CS_playonce(NUFPAR *parser) { CS_CutInfo->end_flags |= 1; }

// FUNCTION: LEGOBATMAN 0x0061a030
void CS_nextcutscene_inplayablelevel(NUFPAR *parser) {
  CS_CutInfo->end_flags |= 2;
}

typedef struct LEVELDATA_s {
  u8 pad0[0x62];
  i16 hub_level; // 0x62
  u32 flags;     // 0x64
  u8 pad68[0x150 - 0x68];
} LEVELDATA;
typedef struct AREADATA_s {
  u8 pad0[0xbc];
} AREADATA;

LEVELDATA *Level_FindByName(char *name, i32 *idx_out);
LEVELDATA *Area_FindStatusLevel(AREADATA *area, i32 *indexDest);

// GLOBAL: LEGOBATMAN 0x00aca8b4
extern LEVELDATA *g_unk00aca8b4;
extern LEVELDATA *LDataList;
extern AREADATA *ADataList;
// GLOBAL: LEGOBATMAN 0x00acb720
extern i32 CS_area;

// FUNCTION: LEGOBATMAN 0x006193f0
static void CS_level(NUFPAR *parser, i16 *dest) {
  i32 level_index;
  i32 status_index;
  if (NuFParGetWord(parser) == 0)
    return;
  if (g_unk00aca8b4 != 0 && NuStrICmp(parser->word_buf, "hub") == 0)
    level_index = g_unk00aca8b4->hub_level;
  else
    Level_FindByName(parser->word_buf, &level_index);
  if (level_index != -1 && (LDataList[level_index].flags & 0x1000000) &&
      CS_area != -1) {
    Area_FindStatusLevel(&ADataList[CS_area], &status_index);
    if (status_index != -1)
      level_index = status_index;
  }
  *dest = (i16)level_index;
}

// FUNCTION: LEGOBATMAN 0x006194b0
void CS_goto_level(NUFPAR *parser) {
  CS_level(parser, &CS_CutInfo->goto_level);
}

// FUNCTION: LEGOBATMAN 0x006194d0
void CS_skipto_level(NUFPAR *parser) {
  CS_level(parser, &CS_CutInfo->skip_level);
}

// GLOBAL: LEGOBATMAN 0x00acb79c
static i32 g_unk00acb79c;

// FUNCTION: LEGOBATMAN 0x00619630
static void CS_fade(NUFPAR *parser, i32 type) {
  f32 f;
  if (g_unk00acb79c < 2) {
    f = NuFParGetFloat(parser);
    CS_CutInfo->fades[g_unk00acb79c].time = f;
    if (f >= 0.0f) {
      f = NuFParGetFloat(parser);
      CS_CutInfo->fades[g_unk00acb79c].r = f;
      if (f >= 0.0f) {
        f = NuFParGetFloat(parser);
        CS_CutInfo->fades[g_unk00acb79c].g = f;
        if (f >= 0.0f) {
          f = NuFParGetFloat(parser);
          CS_CutInfo->fades[g_unk00acb79c].b = f;
          if (f >= 0.0f) {
            CS_CutInfo->fade_type[g_unk00acb79c] = type;
            g_unk00acb79c++;
          }
        }
      }
    }
  }
}

// FUNCTION: LEGOBATMAN 0x00619720
void CS_fadescreen(NUFPAR *parser) { CS_fade(parser, 0); }

// FUNCTION: LEGOBATMAN 0x00619740
void CS_fadefog(NUFPAR *parser) { CS_fade(parser, 1); }
