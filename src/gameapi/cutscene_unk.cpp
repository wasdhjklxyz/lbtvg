// gameapi/, file unknown: cut scene script keyword parsers
// (0x00618d00..0x0061a040), reached through the CS_ keyword table.

#include "../nu2api/nucore/common.h"

typedef struct nufpar_s NUFPAR;

i32 NuFParGetInt(NUFPAR *parser);
f32 NuFParGetFloat(NUFPAR *parser);

struct CUTINFO_s {
  u8 pad0[0x50];
  u32 flags; // 0x50
  u8 pad54[0x60 - 0x54];
  f32 frames_per_second; // 0x60
  f32 burnout_threshold; // 0x64
  f32 burnout_intensity; // 0x68
  f32 burnout_flare;     // 0x6c
  u8 pad70[0xfb - 0x70];
  u8 blob_shadow_alpha;    // 0xfb
  u8 blob_shadow_fadenear; // 0xfc
  u8 blob_shadow_fadefar;  // 0xfd
  u8 reflect_range;        // 0xfe
  u8 render_group;         // 0xff
  u8 pad100[0x19c - 0x100];
  f32 stop_debris; // 0x19c
};

// GLOBAL: LEGOBATMAN 0x00acb77c
static CUTINFO_s *CS_CutInfo;

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
