// gameapi/, file unknown: cut scene script keyword parsers
// (0x00618d00..0x0061a040), reached through the CS_ keyword table.

#include "../nu2api/nucore/common.h"
#include "../nu2api/nucore/nustring.h"
#include "../nu2api/numath/nuinline_unk.h"

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x00618710
static f32 NuFabs(f32 f);
// FUNCTION: LEGOBATMAN 0x00618730
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

typedef struct nufpar_s NUFPAR;

i32 NuFParGetInt(NUFPAR *parser);
f32 NuFParGetFloat(NUFPAR *parser);

struct CUTSCENEPLAYEROBJ_s {
  u8 special[0xc]; // 0x00, nuhspecial_s
  u8 flags;        // 0x0c: 1 show, 2 hide, 4 anim end
  u8 padd[3];
};

struct CUTINFO_s {
  u8 pad0[0x48];
  CUTSCENEPLAYEROBJ_s *state_entries; // 0x48
  u8 state_count;                     // 0x4c
  u8 pad4d[0x4f - 0x4d];
  u8 end_flags; // 0x4f
  u32 flags;    // 0x50
  u8 pad54[0x60 - 0x54];
  f32 frames_per_second; // 0x60
  f32 burnout_threshold; // 0x64
  f32 burnout_intensity; // 0x68
  f32 burnout_flare;     // 0x6c
  f32 near_clip;         // 0x70
  struct {
    i16 id;   // 0x00
    u8 flags; // 0x02
    u8 pad3;
    f32 frame;           // 0x04
    f32 x, y, z;         // 0x08
  } sfx[6];              // 0x74
  u16 far_clip;          // 0xec
  u16 map_overlay_count; // 0xee
  struct CUTSCENEMAPOVERLAY_s {
    f32 start_time;         // 0x00
    f32 end_time;           // 0x04
    i32 start_location;     // 0x08
    i32 end_location;       // 0x0c
    f32 fade_time;          // 0x10
    f32 max_alpha;          // 0x14
  } *map_overlays;          // 0xf0
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
  struct CUTINFO_texanim_s {
    f32 frame;             // 0x00
    i32 index;             // 0x04
  } texture_animations[4]; // 0x150
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

i32 GetSfxId(const char *name);

// FUNCTION: LEGOBATMAN 0x00619240
void CS_play_sfx(NUFPAR *fp) {
  if (NuFParGetWord(fp) == 0)
    return;
  i32 sfx_id = GetSfxId(fp->word_buf);
  if (sfx_id == -1)
    return;
  i32 slot;
  for (slot = 0; slot < 6 && CS_CutInfo->sfx[slot].id != -1; ++slot) {
  }
  if (slot >= 6)
    return;
  CS_CutInfo->sfx[slot].id = sfx_id;
  CS_CutInfo->sfx[slot].flags &= ~1;
  while (NuFParGetWord(fp) != 0) {
    if (NuStrICmp(fp->word_buf, "frame") == 0) {
      CS_CutInfo->sfx[slot].frame = NuFParGetFloat(fp);
      if (CS_CutInfo->sfx[slot].frame < 1.0f)
        CS_CutInfo->sfx[slot].frame = 1.0f;
    } else if (NuStrICmp(fp->word_buf, "pos") == 0) {
      if (NuFParGetFloat(fp) == 0.0f)
        continue;
      CS_CutInfo->sfx[slot].x = NuAToF(fp->word_buf);
      if (NuFParGetFloat(fp) == 0.0f)
        continue;
      CS_CutInfo->sfx[slot].y = NuAToF(fp->word_buf);
      if (NuFParGetFloat(fp) == 0.0f)
        continue;
      CS_CutInfo->sfx[slot].z = NuAToF(fp->word_buf);
      CS_CutInfo->sfx[slot].flags |= 1;
    }
  }
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

// GLOBAL: LEGOBATMAN 0x00acb738
static i32 CS_texanimcount;

// STUB: LEGOBATMAN 0x006195d0
// close: orig stores the GetFloat result straight to frame and re-reads it
// for the 1.0 compare; ours rounds through a stack temp (4 spellings tried).
void CS_tex_anim(NUFPAR *fp) {
  if (CS_texanimcount < 4) {
    i32 index = NuFParGetInt(fp);
    CS_CutInfo->texture_animations[CS_texanimcount].index = index;
    if (index != -1) {
      CUTINFO_s::CUTINFO_texanim_s *animation =
          &CS_CutInfo->texture_animations[CS_texanimcount];
      animation->frame = NuFParGetFloat(fp);
      if (animation->frame >= 1.0f)
        CS_texanimcount++;
    }
  }
}

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

struct CS_WORLDINFO_s {
  u8 pad0[0x140];
  struct nugscn_s *current_gscn; // 0x140
};

// GLOBAL: LEGOBATMAN 0x00acb790
extern CS_WORLDINFO_s *CS_worldinfo;

i32 NuSpecialFind(struct nugscn_s *scene, void *out, char *name, i32 a);

// GLOBAL: LEGOBATMAN 0x00acb768
extern VARIPTR *CS_buffptr;

// FUNCTION: LEGOBATMAN 0x00619e20
void CS_map_overlay(NUFPAR *fp) {
  if (CS_CutInfo->map_overlays == 0)
    CS_CutInfo->map_overlays =
        (CUTINFO_s::CUTSCENEMAPOVERLAY_s *)CS_buffptr->addr;
  CUTINFO_s::CUTSCENEMAPOVERLAY_s *overlay =
      &CS_CutInfo->map_overlays[CS_CutInfo->map_overlay_count];
  overlay->start_time = 0.0f;
  overlay->end_time = 0.0f;
  overlay->start_location = -1;
  overlay->end_location = -1;
  overlay->fade_time = 0.0f;
  overlay->max_alpha = 0.0f;
  while (NuFParGetWord(fp) != 0) {
    if (NuStrICmp(fp->word_buf, "start_location") == 0) {
      overlay->start_location = NuFParGetInt(fp);
    } else if (NuStrICmp(fp->word_buf, "end_location") == 0) {
      overlay->end_location = NuFParGetInt(fp);
    } else if (NuStrICmp(fp->word_buf, "start_time") == 0) {
      overlay->start_time = NuFabs(NuFParGetFloat(fp));
    } else if (NuStrICmp(fp->word_buf, "end_time") == 0) {
      overlay->end_time = NuFabs(NuFParGetFloat(fp));
    } else if (NuStrICmp(fp->word_buf, "fade_time") == 0) {
      overlay->fade_time = NuFabs(NuFParGetFloat(fp));
    } else if (NuStrICmp(fp->word_buf, "max_alpha") == 0) {
      overlay->max_alpha = NuFabs(NuFParGetFloat(fp));
    }
  }
  if (overlay->end_time > overlay->start_time && overlay->fade_time >= 0.0f &&
      overlay->max_alpha > 0.0f && overlay->start_location >= 0 &&
      overlay->end_location >= 0) {
    if (overlay->fade_time * 2.0f > overlay->end_time - overlay->start_time)
      overlay->fade_time = 0.0f;
    CS_buffptr->addr += sizeof(CUTINFO_s::CUTSCENEMAPOVERLAY_s);
    CS_CutInfo->map_overlay_count++;
  }
}

// FUNCTION: LEGOBATMAN 0x0061a040
void CS_cutsceneplayerobj(NUFPAR *fp) {
  if (CS_CutInfo->state_count >= 0x20 || NuFParGetWord(fp) == 0)
    return;
  if (NuSpecialFind(CS_worldinfo->current_gscn,
                    CS_CutInfo->state_entries[CS_CutInfo->state_count].special,
                    fp->word_buf, 1) == 0)
    return;
  CS_CutInfo->state_entries[CS_CutInfo->state_count].flags &= ~1;
  CS_CutInfo->state_entries[CS_CutInfo->state_count].flags &= ~2;
  while (NuFParGetWord(fp) != 0) {
    if (NuStrICmp(fp->word_buf, "on") == 0) {
      CS_CutInfo->state_entries[CS_CutInfo->state_count].flags |= 1;
      CS_CutInfo->state_entries[CS_CutInfo->state_count].flags &= ~2;
    } else if (NuStrICmp(fp->word_buf, "off") == 0) {
      CS_CutInfo->state_entries[CS_CutInfo->state_count].flags &= ~1;
      CS_CutInfo->state_entries[CS_CutInfo->state_count].flags |= 2;
    } else if (NuStrICmp(fp->word_buf, "end_anim") == 0 ||
               NuStrICmp(fp->word_buf, "endanim") == 0 ||
               NuStrICmp(fp->word_buf, "anim_end") == 0 ||
               NuStrICmp(fp->word_buf, "animend") == 0) {
      CS_CutInfo->state_entries[CS_CutInfo->state_count].flags &= ~1;
      CS_CutInfo->state_entries[CS_CutInfo->state_count].flags &= ~2;
      CS_CutInfo->state_entries[CS_CutInfo->state_count].flags |= 4;
    }
  }
  CS_CutInfo->state_count++;
}

// FUNCTION: LEGOBATMAN 0x00619720
void CS_fadescreen(NUFPAR *parser) { CS_fade(parser, 0); }

// FUNCTION: LEGOBATMAN 0x00619740
void CS_fadefog(NUFPAR *parser) { CS_fade(parser, 1); }

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_cutscene_unk(f32 *v, f32 a, i32 i) {
  NuVec4Set(v, a, a, a, a);
}
