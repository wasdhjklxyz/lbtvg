// batman/, file unknown: level config "BL" (before-load) keyword parsers
// (0x004aa400..0x004aac20), reached through the keyword table at 0x0093faa8

#include "leveldata_unk.h"

typedef struct nufpar_s NUFPAR;

i32 NuFParGetInt(NUFPAR *parser);
f32 NuFParGetFloat(NUFPAR *parser);

extern LEVELDATA *levelconfig_ldata;

// FUNCTION: LEGOBATMAN 0x004aa400
void LC_BL_max_tightropes(NUFPAR *parser) {
  levelconfig_ldata->max_tightropes = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x004aa420
void LC_BL_max_signals(NUFPAR *parser) {
  levelconfig_ldata->max_signals = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x004aa440
void LC_BL_max_levers(NUFPAR *parser) {
  levelconfig_ldata->max_levers = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x004aa460
void LC_BL_max_technos(NUFPAR *parser) {
  levelconfig_ldata->max_technos = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x004aa480
void LC_BL_max_zipups(NUFPAR *parser) {
  levelconfig_ldata->max_zipups = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x004aa4a0
void LC_BL_max_grapples(NUFPAR *parser) {
  levelconfig_ldata->max_grapples = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x004aa4c0
void LC_BL_max_obstacles(NUFPAR *parser) {
  levelconfig_ldata->max_obstacles = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x004aa4e0
void LC_BL_max_obstacle_objects(NUFPAR *parser) {
  levelconfig_ldata->max_obstacle_objects = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x004aa510
void LC_BL_maxdig(NUFPAR *parser) {
  levelconfig_ldata->maxdig = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x004aa530
void LC_BL_maxdig_objects(NUFPAR *parser) {
  levelconfig_ldata->maxdig_objects = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x004aa550
void LC_BL_max_turrets(NUFPAR *parser) {
  levelconfig_ldata->max_turrets = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x004aa570
void LC_BL_max_buildits(NUFPAR *parser) {
  levelconfig_ldata->max_buildits = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x004aa590
void LC_BL_max_buildit_objects(NUFPAR *parser) {
  levelconfig_ldata->max_buildit_objects = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x004aa5c0
void LC_BL_max_spinneranim_objs(NUFPAR *parser) {
  levelconfig_ldata->max_spinneranim_objs = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x004aa5e0
void LC_BL_max_climb_objects(NUFPAR *parser) {
  levelconfig_ldata->max_climb_objects = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x004aa600
void LC_BL_max_shards(NUFPAR *parser) {
  levelconfig_ldata->max_shards = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x004aa660
void LC_BL_max_minicuts(NUFPAR *parser) {
  levelconfig_ldata->max_minicuts = NuFParGetInt(parser);
}

// keyword "max_minicut_stages"
// FUNCTION: LEGOBATMAN 0x004aa680
void LC_BL_max_minicutParts(NUFPAR *parser) {
  levelconfig_ldata->max_minicutParts = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x004aa7c0
void LC_BL_max_attractos(NUFPAR *parser) {
  levelconfig_ldata->max_attractos = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x004aa7e0
void LC_BL_max_whippers(NUFPAR *parser) {
  levelconfig_ldata->max_whippers = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x004aa800
void LC_BL_max_timers(NUFPAR *parser) {
  levelconfig_ldata->max_timers = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x004aa820
void LC_BL_max_ledges(NUFPAR *parser) {
  levelconfig_ldata->max_ledges = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x004aa840
void LC_BL_max_securitydoors(NUFPAR *parser) {
  levelconfig_ldata->max_securitydoors = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x004aa860
void LC_BL_max_tubes(NUFPAR *parser) {
  levelconfig_ldata->max_tubes = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x004aa880
void LC_BL_max_pickups(NUFPAR *parser) {
  levelconfig_ldata->max_pickups = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x004aa8b0
void LC_BL_max_gameantinodes(NUFPAR *parser) {
  levelconfig_ldata->max_gameantinodes = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x004aa8e0
void LC_BL_max_gizpanels(NUFPAR *parser) {
  levelconfig_ldata->max_gizpanels = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x004aa900
void LC_BL_max_force(NUFPAR *parser) {
  levelconfig_ldata->max_force = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x004aa920
void LC_BL_max_force_objects(NUFPAR *parser) {
  levelconfig_ldata->max_force_objects = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x004aa950
void LC_BL_max_bombgen_objects(NUFPAR *parser) {
  levelconfig_ldata->max_bombgen_objects = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x004aa980
void LC_BL_max_bombgens(NUFPAR *parser) {
  levelconfig_ldata->max_bombgens = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x004aa9a0
void LC_BL_max_gizspecials(NUFPAR *parser) {
  levelconfig_ldata->max_gizspecials = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x004aa9c0
void LC_BL_max_doors(NUFPAR *parser) {
  levelconfig_ldata->max_doors = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x004aa9e0
void LC_BL_max_teleports(NUFPAR *parser) {
  levelconfig_ldata->max_teleports = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x004aaa00
void LC_BL_max_puzzles(NUFPAR *parser) {
  levelconfig_ldata->max_puzzles = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x004aaa20
void LC_BL_max_gizrandoms(NUFPAR *parser) {
  levelconfig_ldata->max_gizrandoms = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x004aaa40
void LC_BL_max_torpmachines(NUFPAR *parser) {
  levelconfig_ldata->max_torpmachines = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x004aaa60
void LC_BL_max_plugs(NUFPAR *parser) {
  levelconfig_ldata->max_plugs = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x004aaa80
void LC_BL_max_railcreatures(NUFPAR *parser) {
  levelconfig_ldata->max_railcreatures = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x004aaaa0
void LC_BL_max_flocks(NUFPAR *parser) {
  levelconfig_ldata->max_flocks = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x004aaac0
void LC_BL_max_flock_creatures(NUFPAR *parser) {
  levelconfig_ldata->max_flock_creatures = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x004aaaf0
void LC_BL_max_flock_antinodes(NUFPAR *parser) {
  levelconfig_ldata->max_flock_antinodes = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x004aab10
void LC_BL_max_dynamic_flocks(NUFPAR *parser) {
  levelconfig_ldata->max_dynamic_flocks = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x004aab30
void LC_BL_max_dynamic_flock_creatures(NUFPAR *parser) {
  levelconfig_ldata->max_dynamic_flock_creatures = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x004aab60
void LC_BL_max_dynamic_flock_antinodes(NUFPAR *parser) {
  levelconfig_ldata->max_dynamic_flock_antinodes = NuFParGetInt(parser);
}

// keyword "windspeed"
// FUNCTION: LEGOBATMAN 0x004aaba0
void LC_BL_wind_speed(NUFPAR *parser) {
  levelconfig_ldata->wind_speed = NuFParGetFloat(parser);
}

// keyword "windsize"
// FUNCTION: LEGOBATMAN 0x004aabc0
void LC_BL_wind_size(NUFPAR *parser) {
  levelconfig_ldata->wind_size = NuFParGetFloat(parser);
}

// FUNCTION: LEGOBATMAN 0x004aabe0
void LC_BL_slide_speed(NUFPAR *parser) {
  levelconfig_ldata->slide_speed = NuFParGetFloat(parser);
}

// FUNCTION: LEGOBATMAN 0x004aac00
void LC_BL_lateral_slide_speed(NUFPAR *parser) {
  levelconfig_ldata->lateral_slide_speed = NuFParGetFloat(parser);
}

// FUNCTION: LEGOBATMAN 0x004aa620
void LC_BL_max_spinners(NUFPAR *parser) {
  levelconfig_ldata->max_spinners = NuFParGetInt(parser);
  if (levelconfig_ldata->max_spinners > 0x20)
    levelconfig_ldata->max_spinners = 0x20;
}

// FUNCTION: LEGOBATMAN 0x004aa740
void LC_BL_max_pushblocks(NUFPAR *parser) {
  levelconfig_ldata->max_pushblocks = NuFParGetInt(parser);
  if (levelconfig_ldata->max_pushblocks > 16)
    levelconfig_ldata->max_pushblocks = 16;
}

// FUNCTION: LEGOBATMAN 0x004aa780
void LC_BL_max_pushblock_endpos(NUFPAR *parser) {
  levelconfig_ldata->max_pushblock_endpos = NuFParGetInt(parser);
  if (levelconfig_ldata->max_pushblock_endpos > 0x40)
    levelconfig_ldata->max_pushblock_endpos = 0x40;
}

// Shares the field with max_dynamic_flock_antinodes in the shipped code.
// FUNCTION: LEGOBATMAN 0x004aab80
void LC_BL_flock_extras(NUFPAR *parser) {
  levelconfig_ldata->max_dynamic_flock_antinodes = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x004aa6a0
void LC_BL_max_gizmoblowuptypes(NUFPAR *parser) {
  levelconfig_ldata->max_gizmoblowuptypes = NuFParGetInt(parser);
  if (levelconfig_ldata->max_gizmoblowuptypes > 0xff)
    levelconfig_ldata->max_gizmoblowuptypes = 0xff;
}

// FUNCTION: LEGOBATMAN 0x004aa6f0
void LC_BL_max_gizmoblowups(NUFPAR *parser) {
  levelconfig_ldata->max_gizmoblowups = NuFParGetInt(parser);
  if (levelconfig_ldata->max_gizmoblowups > 0x200)
    levelconfig_ldata->max_gizmoblowups = 0x200;
}
