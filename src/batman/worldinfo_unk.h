#pragma once
// WORLDINFO_s: only fields with evidence.

#include "../gameapi/gameobject_unk.h"

struct nugscn_s;
struct nuhspecial_s;
struct GIZMOSYS_s;
struct GIZMO_s;
struct GIZMOBLOWUP_s;
struct GIZAIMESSAGESYS_s;
struct GIZAIMESSAGE_s;
struct AISYS_s;
struct AILOCATOR_s;
struct GAMECAMERA_s;

// 0x2c-byte entries of the list at WORLDINFO_s+0x5220.
struct Unk_WorldInfo5220Entry { // GIZMOPICKUP_s
  char name[0x14];
  char type_code; // 0x14
  u8 pad15[2];
  u8 b17;              // 0x17
  u16 state_bit0 : 1;  // 0x18
  u16 enabled : 1;     // bit 1
  u16 visible : 1;     // bit 2
  u16 state_bits3 : 4; //
  u16 activated : 1;   // bit 7
  u16 state_hi : 8;    //
  u8 pad1[0x24 - 0x1a];
  u8 b24;        // 0x24
  u8 type_index; // 0x25
  u8 pad2[0x2c - 0x26];
};

struct Unk_WorldInfo5220 {
  Unk_WorldInfo5220Entry *list; // 0x00
  u8 pad0[4];
  i32 count; // 0x08
};

struct Unk_WorldInfo2b04 {
  u8 pad0[0xd7e];
  u8 bd7e; // 0xd7e
  u8 pad1[0xf];
  u8 bd8e; // 0xd8e
  u8 pad2[0xf];
  u8 bd9e; // 0xd9e
};

struct WORLDINFO_s {
  u8 pad0[0x80];
  char config_file[0x84]; // 0x80
  variptr_u buf104;       // 0x104
  variptr_u bufEnd108;    // 0x108
  i32 config_count;       // 0x10c
  u8 pad1[0x11c - 0x110];
  u8 reset_flags; // 0x11c, bit 0 kill bombs on reset
  u8 pad11d[0x124 - 0x11d];
  i32 i124; // 0x124, SuperCounter_AreaCheck area
  u8 pad128[0x12c - 0x128];
  struct LEVELDATA_s *current_level; // 0x12c
  struct AREADATA_s *area;           // 0x130
  u8 pad1b[0x138 - 0x134];
  void *p138; // 0x138
  u8 pad2[0x140 - 0x13c];
  nugscn_s *scn140; // 0x140
  u8 pad3[0x148 - 0x144];
  nugscn_s *scn148; // 0x148
  u8 pad4[0x2968 - 0x14c];
  struct GAMEANIMSYS_s *game_anim_sys; // 0x2968
  u8 pad296c[0x2974 - 0x296c];
  i32 i2974; // 0x2974
  u8 pad5[0x29cc - 0x2978];
  struct SOCKSYS_s *sock_sys;                // 0x29cc
  struct Unk_WorldApiObjSys *api_object_sys; // 0x29d0, APIOBJECTSYS_s
  u8 pad5x[0x2adc - 0x29d4];
  i32 page_pp; // 0x2adc
  u8 pad5a[0x2aec - 0x2ae0];
  void *burnset;      // 0x2aec
  void *cutscene_sys; // 0x2af0
  void *rtl_set;      // 0x2af4
  u8 pad5c[0x2b04 - 0x2af8];
  Unk_WorldInfo2b04 *p2b04; // 0x2b04
  u8 pad6[0x2b0c - 0x2b08];
  GIZMOSYS_s *gizmoSys2b0c; // 0x2b0c
  void *p2b10;              // 0x2b10
  u8 pad7[0x2bec - 0x2b14];
  struct CHARPLATFORMSYS_s *char_platform_sys; // 0x2bec
  u8 pad7b[0x2bf8 - 0x2bf0];
  AISYS_s *aiSys2bf8;             // 0x2bf8
  i32 processor_count;            // 0x2bfc
  u8 processors[0x4780 - 0x2c00]; // 0x2c00, 0xdc-byte LEVELSCRIPTPROCESS
  void *ai_path_cnx_control_sys;  // 0x4780
  void *ai_path_cnx_helper_sys;   // 0x4784
  struct AITRIGGERSETSYS_s *ai_trigger_set_sys; // 0x4788
  u8 pad478c[0x4790 - 0x478c];
  struct TELEPORT_s *teleports; // 0x4790
  i32 teleport_count;           // 0x4794
  struct ZIPUP_s *zipups;       // 0x4798
  i32 zipup_count;              // 0x479c
  struct TUBE_s *tubes;         // 0x47a0
  i32 tube_count;               // 0x47a4
  u8 pad47a8[0x47b4 - 0x47a8];
  struct GIZOBSTACLESYS_s *giz_obstacle_sys;         // 0x47b4
  struct GIZBUILDITSYS_s *giz_buildit_sys;           // 0x47b8
  struct GIZFORCESYS_s *giz_force_sys;               // 0x47bc
  struct GIZDIGSYS_s *giz_dig_sys;                   // 0x47c0
  struct EQUIVALENTOBJECTGROUP_s *equivalent_groups; // 0x47c4
  i32 equivalent_group_count;                        // 0x47c8
  u8 pad47cc[0x47d0 - 0x47cc];
  struct PUSHBLOCK_s *pushblocks; // 0x47d0
  i32 pushblock_count;            // 0x47d4
  u8 pad8b[0x4800 - 0x47d8];
  struct GRABBERSYS_s *grabber_sys; // 0x4800
  u8 pad4804[0x4808 - 0x4804];
  struct RIPPLEEFFECT_s *ripple_effects; // 0x4808
  i32 ripple_effect_count;               // 0x480c
  i32 ripple_effect_indices[2];          // 0x4810
  u8 pad8b1[0x51a8 - 0x4818];
  struct FADER_s *faders; // 0x51a8
  i32 fader_count;        // 0x51ac
  u8 pad8b2[0x51bc - 0x51b0];
  struct PORTALDOOR_s *portal_doors; // 0x51bc
  i32 portal_door_count;             // 0x51c0
  u8 pad8c[0x51d4 - 0x51c4];
  struct SIGNAL_s *signals; // 0x51d4
  i32 signal_count;         // 0x51d8
  u8 pad51dc[0x51ec - 0x51dc];
  struct TECHNO_s *technos; // 0x51ec
  i32 techno_count;         // 0x51f0
  u8 pad51f4[0x51fc - 0x51f4];
  struct SHARD_s *shards; // 0x51fc
  i32 shard_count;        // 0x5200
  u8 pad5204[0x520c - 0x5204];
  struct ATTRACTO_s *attractos; // 0x520c
  i32 attracto_count;           // 0x5210
  struct LEDGE_s *ledges;       // 0x5214
  i32 ledge_count;              // 0x5218
  u8 pad521c[0x5220 - 0x521c];
  Unk_WorldInfo5220 *p5220; // 0x5220
  u8 pad9[0x5228 - 0x5224];
  i32 gizmo_blowup_count; // 0x5228
  u8 pad10[0x5230 - 0x522c];
  GIZMOBLOWUP_s *gizmo_blowups; // 0x5230
  u8 pad5234[0x5260 - 0x5234];
  struct MINICUT_s *minicuts;          // 0x5260
  struct MINICUTPART_s *minicut_parts; // 0x5264
  i32 minicut_count;                   // 0x5268
  struct GIZTIMER_s *giz_timers;       // 0x526c
  i32 giz_timer_count;                 // 0x5270
  u8 pad5274[0x5278 - 0x5274];
  struct GIZRANDOMSYS_s *giz_randoms;      // 0x5278
  struct GIZSPECIALSYS_s *giz_special_sys; // 0x527c
  u8 pad5280[0x5284 - 0x5280];
  struct EDGIZSHADOW_s *shadow_editor;     // 0x5284
  struct GIZBOMBGENSYS_s *giz_bombgen_sys; // 0x5288
  u8 pad528c[0x52f8 - 0x528c];
  struct PLUGSYS_s *plug_sys;     // 0x52f8
  struct PUZZLESYS_s *puzzle_sys; // 0x52fc
};

// GLOBAL: LEGOBATMAN 0x00960894
extern WORLDINFO_s *g_unk00960894;
// GLOBAL: LEGOBATMAN 0x0095f624
extern GAMECAMERA_s *g_unk0095f624;
// GLOBAL: LEGOBATMAN 0x0095eb8c
extern i32 g_unk0095eb8c; // language
// GLOBAL: LEGOBATMAN 0x00a96070
extern i32 g_unk00a96070;

i32 NuSpecialFind(nugscn_s *scene, nuhspecial_s *dest, char *name, i32 flags);
nugscn_s *NuGScnRead(variptr_u *buf, variptr_u buf_end, char *path);
GIZMO_s *GizmoFindByName(GIZMOSYS_s *sys, i32 type_id, char *name);
GIZMOBLOWUP_s *GizmoBlowUp_FindByName(WORLDINFO_s *wi, char *name);
GIZAIMESSAGE_s *CheckGizAIMessage(GIZAIMESSAGESYS_s *sys, char const *name,
                                  GIZAIMESSAGE_s *out);
AILOCATOR_s *AIPathFindLocator(AISYS_s *aisys, char *name);
WORLDINFO_s *WorldInfo_CurrentlyActive(void);
