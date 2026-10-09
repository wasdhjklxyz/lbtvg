// gameapi/ai/aisys/AIBugPit.cpp: certain range 0x006b1220..0x006be490.

#include "AIBugPit.h"
#include "../../../nu2api/nucore/nustring.h"
#include "../aisys_unk.h"

// FUNCTION: LEGOBATMAN 0x006b24d0
void AIBugPitGridBase::CellOf(int index, int *ix, int *iz) {
  int q = index / width;
  *iz = q;
  *ix = index - q * width;
}

// FUNCTION: LEGOBATMAN 0x006b21d0
void AIBugPitGrid48::WorldToCell(const AIVec *pos, int *ix, int *iz) {
  AIVec d;
  d.x = pos->x - originX;
  d.z = pos->z - originZ;
  float s = invCellSize;
  d.x *= s;
  d.z *= s;
  *ix = (int)d.x;
  *iz = (int)d.z;
}

// FUNCTION: LEGOBATMAN 0x006b22d0
void AIBugPitGrid32::WorldToCell(const AIVec *pos, int *ix, int *iz) {
  AIVec d;
  d.x = pos->x - originX;
  d.z = pos->z - originZ;
  float s = invCellSize;
  d.x *= s;
  d.z *= s;
  *ix = (int)d.x;
  *iz = (int)d.z;
}

// FUNCTION: LEGOBATMAN 0x006b2250
int AIBugPitGrid48::IndexOf(int ix, int iz) {
  if (clamp) {
    int cx;
    int cz;
    if (ix < 0)
      cx = 0;
    else if (ix > width - 1)
      cx = width - 1;
    else
      cx = ix;
    if (iz < 0)
      cz = 0;
    else if (iz > height - 1)
      cz = height - 1;
    else
      cz = iz;
    return width * cz + cx;
  }
  if (ix >= 0 && ix < width && iz >= 0 && iz < height)
    return width * iz + ix;
  return cellCount - 1;
}

// FUNCTION: LEGOBATMAN 0x006b2350
int AIBugPitGrid32::IndexOf(int ix, int iz) {
  if (clamp) {
    int cx;
    int cz;
    if (ix < 0)
      cx = 0;
    else if (ix > width - 1)
      cx = width - 1;
    else
      cx = ix;
    if (iz < 0)
      cz = 0;
    else if (iz > height - 1)
      cz = height - 1;
    else
      cz = iz;
    return width * cz + cx;
  }
  if (ix >= 0 && ix < width && iz >= 0 && iz < height)
    return width * iz + ix;
  return cellCount - 1;
}

// FUNCTION: LEGOBATMAN 0x006b2500
AIVec AIBugPitGrid48::CellCenter(int ix, int iz) {
  AIVec v;
  v.x = (float)((ix + 0.5) * cellSize + originX);
  v.y = 1.0f;
  v.z = (float)((iz + 0.5) * cellSize + originZ);
  return v;
}

// FUNCTION: LEGOBATMAN 0x006b2580
AIVec AIBugPitGrid32::CellCenter(int ix, int iz) {
  AIVec v;
  v.x = (float)((ix + 0.5) * cellSize + originX);
  v.y = 1.0f;
  v.z = (float)((iz + 0.5) * cellSize + originZ);
  return v;
}

// FUNCTION: LEGOBATMAN 0x006b25f0
void AIBugPitBufferA::Free() {
  g_memPool->Free(data, size, 0);
  data = 0;
}

// FUNCTION: LEGOBATMAN 0x006b2630
void AIBugPitBufferB::Free() {
  g_memPool->Free(data, size, 0);
  data = 0;
}

#include "../../../nu2api/nucore/common.h"
#include <stdio.h>

struct AISYS_s;

// GLOBAL: LEGOBATMAN 0x0099e290
extern i32 ai_usepackfile;

i32 NuFileSize(char *path);
void *NuFilePakLoad(char *filepath, VARIPTR *buf, VARIPTR buf_end,
                    i32 alignment);
void AIScriptLoadAllPakFile(void *pak, char *path, VARIPTR *buf,
                            VARIPTR *buf_end, AISYS_s *sys);

struct nufpar_s {
  u8 pad0[0x910];
  char *word_buf; // 0x910
};

i32 NuFParGetWord(struct nufpar_s *parser);
void NuFParSuspend(struct nufpar_s *parser);
void NuFParResume(struct nufpar_s *parser);
struct AISCRIPT_s;
void AIScriptOpenPakFileParse(AISCRIPT_s **script, void *pak, char *name,
                              char *path, VARIPTR *buf, VARIPTR *buf_end);

// annotated in gameapi/ai/aiscript_unk.cpp
extern VARIPTR *load_buff;
extern VARIPTR *load_endbuff;
extern AISCRIPT_s *load_aiscript;
// GLOBAL: LEGOBATMAN 0x00ad4518
extern char *load_path;
// GLOBAL: LEGOBATMAN 0x00ad451c
extern void *load_pakfile;

// keyword "INCLUDE" in table 0x0099dce0
// FUNCTION: LEGOBATMAN 0x006b32f0
void xInclude(struct nufpar_s *parser) {
  AISCRIPT_s *tmp_script = load_aiscript;
  VARIPTR *tmp_buf_end = load_endbuff;
  VARIPTR *tmp_buf = load_buff;
  NuFParGetWord(parser);
  NuFParSuspend(parser);
  AIScriptOpenPakFileParse(&load_aiscript, load_pakfile, parser->word_buf,
                           load_path, load_buff, load_endbuff);
  NuFParResume(parser);
  load_buff = tmp_buf;
  load_endbuff = tmp_buf_end;
  load_aiscript = tmp_script;
}

// FUNCTION: LEGOBATMAN 0x006b3a60
void AIScriptLoadAll(char *path, VARIPTR *buf, VARIPTR *buf_end, AISYS_s *sys) {
  void *pak;
  VARIPTR pak_start;
  char filepath[0x80];
  i32 pak_size;

  pak = 0;
  pak_start = *buf_end;

  if (ai_usepackfile) {
    sprintf(filepath, "%s\\ai.pak", path);

    pak_size = NuFileSize(filepath);
    if (pak_size > 0) {
      pak_start.addr = buf_end->addr - ((pak_size + 0x10) & ~0xf);
      pak = NuFilePakLoad(filepath, &pak_start, *buf_end, 0x10);
    }
  }

  AIScriptLoadAllPakFile(pak, path, buf, &pak_start, sys);
}

i32 AIScriptSetInterrupt(AISCRIPTPROCESS_s *processor, u8 priority, u8 id,
                         char *state_name, f32 time);

struct AILOCATOR_s *AIPathFindLocator(AISYS_s *aisys, char *name);
i32 NuRand(void *rand);

#define RESPAWN_LOCATOR(packet) (*(void **)((u8 *)(packet) + 0x1b4))

// FUNCTION: LEGOBATMAN 0x006b4170
i32 Action_SetRespawnLocator(AISYS_s *sys, AISCRIPTPROCESS_s *processor,
                             AIPACKET_s *packet, char **params, i32 param_count,
                             i32 first_time, f32 elapsed) {
  struct AILOCATOR_s *locators[32];
  if (packet == NULL || packet->pd0 == NULL || packet->pd0->obj == NULL)
    return 1;
  if (first_time != 0) {
    RESPAWN_LOCATOR(packet) = processor->locator_set;
    i32 count = 0;
    for (i32 i = 0; i < param_count; i++) {
      char *value = NuStrIStr(params[i], "locator");
      if (value != NULL) {
        if (count < 32) {
          locators[count] = AIPathFindLocator(sys, value + 8);
          if (locators[count] != NULL)
            count++;
        }
      } else if (NuStrICmp(params[i], "clear") == 0) {
        RESPAWN_LOCATOR(packet) = NULL;
        return 1;
      }
    }
    if (count != 0)
      RESPAWN_LOCATOR(packet) = locators[NuRand(NULL) % count];
  }
  return 1;
}

// AISYS_s +0x21c
struct AIBLOCKPATHSYS_s {
  u8 path_count; // 0x00
  u8 pad1[8 - 1];
  void *active_path; // 0x08
};

#define BP_PATHSYS(sys) (*(AIBLOCKPATHSYS_s **)((u8 *)(sys) + 0x21c))

void AIPathCnxSetTemporaryBlock(void *path, char *from, char *to, i32 blocked);

// FUNCTION: LEGOBATMAN 0x006b4350
i32 Action_BlockPath(AISYS_s *sys, AISCRIPTPROCESS_s *processor,
                     AIPACKET_s *packet, char **params, i32 param_count,
                     i32 first_time, f32 elapsed) {
  char *from = NULL;
  char *to = NULL;
  i32 blocked = 1;
  i32 both_ways = 0;
  if (sys != NULL && BP_PATHSYS(sys) != NULL &&
      BP_PATHSYS(sys)->path_count != 0 && first_time != 0) {
    for (i32 index = 0; index < param_count; ++index) {
      char *value = NuStrIStr(params[index], "from");
      if (value != NULL) {
        from = value + 5;
        continue;
      }
      value = NuStrIStr(params[index], "to");
      if (value != NULL)
        to = value + 3;
      else if (NuStrICmp(params[index], "bothways") == 0)
        both_ways = 1;
      else if (NuStrICmp(params[index], "FALSE") == 0)
        blocked = 0;
    }
    if (from != NULL && to != NULL) {
      AIPathCnxSetTemporaryBlock(BP_PATHSYS(sys)->active_path, from, to,
                                 blocked);
      if (both_ways)
        AIPathCnxSetTemporaryBlock(BP_PATHSYS(sys)->active_path, to, from,
                                   blocked);
    }
  }
  return 1;
}

struct AIPATHCNXOBS_s {
  u32 traversal_flags; // 0x00
  u8 pad4[0x16 - 4];
  u8 open; // 0x16
};

AIPATHCNXOBS_s *AIPathFindPathCnx(AISYS_s *sys, void *path, char *from,
                                  char *to, i32 *direction);

// FUNCTION: LEGOBATMAN 0x006b45a0
i32 Action_PathConnectionObstacle(AISYS_s *sys, AISCRIPTPROCESS_s *processor,
                                  AIPACKET_s *packet, char **params,
                                  i32 param_count, i32 first_time,
                                  f32 elapsed) {
  char *from = NULL;
  char *to = NULL;
  i32 open = 0;
  if (sys != NULL && first_time != 0) {
    for (i32 i = 0; i < param_count; i++) {
      char *value = NuStrIStr(params[i], "from");
      if (value != NULL)
        from = value + 5;
      else if ((value = NuStrIStr(params[i], "to")) != NULL)
        to = value + 3;
      else if (NuStrICmp(params[i], "open") == 0)
        open = 1;
    }
    if (from != NULL && to != NULL) {
      i32 direction;
      AIPATHCNXOBS_s *connection =
          AIPathFindPathCnx(sys, NULL, from, to, &direction);
      if (connection != NULL && (connection->traversal_flags & 0x20000000) != 0)
        connection->open = open;
    }
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x006b4670
i32 Action_SetInterrupt(AISYS_s *sys, AISCRIPTPROCESS_s *processor,
                        AIPACKET_s *packet, char **params, i32 param_count,
                        i32 is_first_time, f32 dt) {
  f32 time = 0.0f;
  u8 priority = 0;
  u8 id = 0;
  char *state_name = 0;
  char *value;
  if (is_first_time && processor != 0) {
    for (i32 i = 0; i < param_count; i++) {
      if ((value = NuStrIStr(params[i], "priority")) != 0) {
        priority = (u8)AIParamToFloatEx(packet, processor, value + 9);
      } else if ((value = NuStrIStr(params[i], "id")) != 0) {
        id = (u8)AIParamToFloatEx(packet, processor, value + 3);
      } else if ((value = NuStrIStr(params[i], "state")) != 0) {
        state_name = value + 6;
      } else if ((value = NuStrIStr(params[i], "time")) != 0) {
        time = AIParamToFloatEx(packet, processor, value + 5);
      }
    }
    if (state_name != 0)
      AIScriptSetInterrupt(processor, priority, id, state_name, time);
  }
  return 1;
}

void AISysRegisterPathCnxType(char *name, char *short_name, u32 connection_flag,
                              void *context, u32 flags);

i32 NuStrCmp(const char *a, const char *b);

static inline void AIScriptClearInterrupt(AISCRIPTPROCESS_s *processor,
                                          char *state_name) {
  if (processor->interrupt_state != 0) {
    if (NuStrCmp(state_name, processor->interrupt_state->name) == 0) {
      processor->interrupt_timer = 0.0f;
      processor->interrupt_priority = 0;
      processor->interrupt_id = 0;
      processor->interrupt_state = 0;
    }
  }
}

// STUB: LEGOBATMAN 0x006b47e0
// callee-saved pushes are lazy per check in the original (ebp=processor,
// ebx=count, separate epilogues); ours pushes both up front
i32 Action_ClearInterrupt(AISYS_s *sys, AISCRIPTPROCESS_s *processor,
                          AIPACKET_s *packet, char **params, i32 param_count,
                          i32 is_first_time, f32 dt) {
  char *state_name = 0;
  if (is_first_time && processor != 0 && param_count > 0) {
    for (i32 i = 0; i < param_count; i++) {
      char *value = NuStrIStr(params[i], "state");
      if (value != 0)
        state_name = value + 6;
    }
    if (state_name != 0)
      AIScriptClearInterrupt(processor, state_name);
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x006b4f70
void AISysRegisterDefaultPathCnxTypes(void) {
  AISysRegisterPathCnxType("Permanent Block", "PermBlock", 0x40000000, 0, 0);
  AISysRegisterPathCnxType("Temporary Block", "Block", 0x80000000, 0, 0);
  AISysRegisterPathCnxType("Link Obstacle", "Obstacle", 0x20000000, 0, 1);
}

// FUNCTION: LEGOBATMAN 0x006b9430
AIVec AIBugPitGrid48::CellCenterOf(const AIBugPitCell48 *cell) {
  int index = cell - cells;
  if (index >= 0 && index < cellCount) {
    int ix;
    int iz;
    CellOf(index, &ix, &iz);
    return CellCenter(ix, iz);
  }
  return AIVec(0.0f, 0.0f, 0.0f, 1.0f);
}

// FUNCTION: LEGOBATMAN 0x006b9520
AIVec AIBugPitGrid32::CellCenterOf(const AIBugPitCell32 *cell) {
  int index = cell - cells;
  if (index >= 0 && index < cellCount) {
    int ix;
    int iz;
    CellOf(index, &ix, &iz);
    return CellCenter(ix, iz);
  }
  return AIVec(0.0f, 0.0f, 0.0f, 1.0f);
}

// FUNCTION: LEGOBATMAN 0x006b1e40
void AIBugPitGrid32::AdvanceCells() {
  AIBugPitCell32 *cell = cells;
  for (int iz = 0; iz < height; iz++) {
    for (int ix = 0; ix < width; ix++) {
      cell->cur = cell->next;
      cell->next = 0;
      cell++;
    }
  }
}

// FUNCTION: LEGOBATMAN 0x006b95e0
void AIBugPitBufferA::Release() {
  if (owned) {
    g_memPool->Free(data, size, 0);
    data = 0;
  }
}

// FUNCTION: LEGOBATMAN 0x006b9640
void AIBugPitBufferB::Release() {
  if (owned) {
    g_memPool->Free(data, size, 0);
    data = 0;
  }
}

void FollowAPIObject(Unk_AIPacketObj *api, void *target, i32 flags, f32 param);
// annotated in batman/aiactions_unk.cpp (0x00ad6918)
extern i32 (*AIActionParseSpeedFn)(char *str, u8 *out);

// FUNCTION: LEGOBATMAN 0x006b9fd0
i32 Action_FollowOpponent(AISYS_s *sys, AISCRIPTPROCESS_s *processor,
                          AIPACKET_s *packet, char **params, i32 param_count,
                          i32 first_time, f32 elapsed) {
  if (packet == NULL)
    return 1;
  if (first_time != 0) {
    for (i32 index = 0; index < param_count; ++index) {
      if (AIActionParseSpeedFn != NULL &&
          AIActionParseSpeedFn(params[index], &packet->goal_speed_mode) != 0)
        continue;
      if (NuStrICmp(params[index], "ignore_radius") == 0)
        processor->action_data_1 |= 2;
      else if (NuStrICmp(params[index], "can_go_off_path") == 0)
        processor->action_data_1 |= 1;
      else
        packet->movement_param =
            AIParamToFloatEx(packet, processor, params[index]);
    }
  }
  Unk_AIPacketObj *target = packet->pe4;
  if (target != NULL && target->ai != NULL)
    FollowAPIObject(packet->pd0, target, processor->action_data_1,
                    packet->movement_param);
  return 0;
}

// FUNCTION: LEGOBATMAN 0x006ba0b0
i32 Action_FollowPlayer(AISYS_s *sys, AISCRIPTPROCESS_s *processor,
                        AIPACKET_s *packet, char **params, i32 param_count,
                        i32 first_time, f32 elapsed) {
  if (packet == NULL)
    return 1;
  if (first_time != 0) {
    for (i32 index = 0; index < param_count; ++index) {
      if (AIActionParseSpeedFn != NULL &&
          AIActionParseSpeedFn(params[index], &packet->goal_speed_mode) != 0)
        continue;
      if (NuStrICmp(params[index], "ignore_radius") == 0)
        processor->action_data_1 |= 2;
      else if (NuStrICmp(params[index], "can_go_off_path") == 0)
        processor->action_data_1 |= 1;
      else
        packet->movement_param =
            AIParamToFloatEx(packet, processor, params[index]);
    }
  }
  // 0x1698: sys->player_1
  Unk_AIPacketObj *player = *(Unk_AIPacketObj **)((u8 *)sys + 0x1698);
  if (player != NULL && player->ai != NULL)
    FollowAPIObject(packet->pd0, player, processor->action_data_1,
                    packet->movement_param);
  return 0;
}

// Batman's path cursor is 7 dwords.
struct AIPATHINFO7_s {
  u32 w[7];
};

// Inlined into its callers in this file; Batman drops saga's formation case.
static __forceinline void
AIMoveInstructionInline(AIPACKET_s *packet, nuvec_s *destination,
                        f32 stopping_distance, AIPATHINFO7_s *path_info,
                        i32 mode, f32 movement_parameter) {
  if (destination != NULL)
    *(nuvec_s *)((u8 *)packet + 0x1b8) = *destination;
  if (path_info != NULL)
    *(AIPATHINFO7_s *)((u8 *)packet + 0x1d0) = *path_info;
  *(f32 *)((u8 *)packet + 0x1c4) = stopping_distance;
  packet->movement_event_flags =
      (packet->movement_event_flags & ~7) | (mode & 7);
  *(f32 *)((u8 *)packet + 0x1c8) = movement_parameter;
}

// FUNCTION: LEGOBATMAN 0x006bad30
i32 Action_RetreatFromOpponent(AISYS_s *sys, AISCRIPTPROCESS_s *processor,
                               AIPACKET_s *packet, char **params,
                               i32 param_count, i32 first_time, f32 elapsed) {
  if (packet == NULL)
    return 1;
  if (first_time != 0) {
    packet->movement_param = 1.0f;
    for (i32 i = 0; i < param_count; i++) {
      if (AIActionParseSpeedFn == NULL ||
          AIActionParseSpeedFn(params[i], &packet->goal_speed_mode) == 0)
        packet->movement_param = AIParamToFloatEx(packet, processor, params[i]);
    }
  }
  if (packet->pe4 != NULL && packet->pe4->ai != NULL) {
    AIMoveInstructionInline(packet, (nuvec_s *)((u8 *)packet->pe4->ai + 0x174),
                            *(f32 *)((u8 *)packet->pe4->ai + 0x120),
                            (AIPATHINFO7_s *)((u8 *)packet->pe4->ai + 0x158), 2,
                            packet->movement_param);
  }
  return 0;
}

f32 NuVecXZDist(nuvec_s *a, nuvec_s *b, nuvec_s *d);
f32 NuRandFloat(void);

// 0x1698/0x169c: sys->player_1/player_2
#define PLAYER(sys, n) (((Unk_AIPacketObj **)((u8 *)(sys) + 0x1698))[n])

struct AILOCATOR_s *AIPathFindLocator(AISYS_s *sys, char *name);
extern nuvec_s *(*GetAICreatureOriginFn)(AISYS_s *sys, AIPACKET_s *packet);

// FUNCTION: LEGOBATMAN 0x006ba190
i32 Action_Circle(AISYS_s *sys, AISCRIPTPROCESS_s *processor,
                  AIPACKET_s *packet, char **params, i32 param_count,
                  i32 first_time, f32 elapsed) {
  nuvec_s difference;
  if (packet == NULL || sys == NULL || PLAYER(sys, 0) == NULL)
    return 1;
  if (first_time != 0) {
    for (i32 i = 0; i < param_count; i++) {
      if (AIActionParseSpeedFn != NULL &&
          AIActionParseSpeedFn(params[i], &packet->goal_speed_mode) != 0)
        continue;
      char *value;
      if (NuStrICmp(params[i], "ANTICLOCKWISE") == 0)
        packet->circle_clockwise = 0;
      else if (NuStrICmp(params[i], "CLOCKWISE") == 0)
        packet->circle_clockwise = 1;
      else if (NuStrICmp(params[i], "REVERSE") == 0)
        packet->circle_clockwise = !packet->circle_clockwise;
      else if (NuStrICmp(params[i], "facing") == 0)
        processor->action_data_1 |= 1;
      else if (NuStrICmp(params[i], "currentdist") == 0)
        processor->action_data_1 |= 4;
      else if ((value = NuStrIStr(params[i], "locator=")) != NULL) {
        processor->action_data_3 = AIPathFindLocator(sys, value + 8);
        if (processor->action_data_3 != NULL)
          processor->action_data_1 |= 0x10;
      } else if (NuStrIStr(params[i], "current_position") != NULL) {
        processor->action_data_1 |= 8;
        processor->action_pos = *(nuvec_s *)((u8 *)packet + 0x14c);
      } else if (NuStrICmp(params[i], "origin") == 0) {
        if (packet->origin_index != 0xff)
          processor->action_data_1 |= 0x20;
      } else
        packet->movement_param = AIParamToFloatEx(packet, processor, params[i]);
    }
    if ((processor->action_data_1 & 0x38) == 0) {
      processor->action_data_1 |= 8;
      processor->action_pos = *(nuvec_s *)((u8 *)packet + 0x14c);
    }
  }
  nuvec_s *position = NULL;
  AIPATHINFO7_s *path = NULL;
  if ((processor->action_data_1 & 8) != 0) {
    path = (AIPATHINFO7_s *)((u8 *)packet + 0x158);
    position = &processor->action_pos;
  } else if ((processor->action_data_1 & 0x10) != 0) {
    u8 *locator = (u8 *)processor->action_data_3;
    path = (AIPATHINFO7_s *)(locator + 0x20);
    position = (nuvec_s *)(locator + 0x10);
  } else if ((processor->action_data_1 & 0x20) != 0) {
    u8 *creature = *(u8 **)((u8 *)sys + 0x224) + packet->origin_index * 0xa8;
    path = (AIPATHINFO7_s *)(creature + 0x30);
    position = GetAICreatureOriginFn != NULL
                   ? GetAICreatureOriginFn(sys, packet)
                   : NULL;
    if (position == NULL)
      position = (nuvec_s *)(creature + 0x20);
  }
  if (path != NULL && position != NULL) {
    if ((processor->action_data_1 & 4) != 0)
      packet->movement_param =
          NuVecXZDist((nuvec_s *)((u8 *)packet + 0x14c), position, &difference);
    AIMoveInstructionInline(packet, position, 0.0f, path, 3,
                            packet->movement_param);
    if ((processor->action_data_1 & 1) != 0)
      packet->look_target = position;
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x006ba4b0
i32 Action_CirclePlayer(AISYS_s *sys, AISCRIPTPROCESS_s *processor,
                        AIPACKET_s *packet, char **params, i32 param_count,
                        i32 first_time, f32 elapsed) {
  nuvec_s difference;
  if (packet == NULL || sys == NULL || PLAYER(sys, 0) == NULL)
    return 1;
  if (first_time != 0) {
    for (i32 i = 0; i < param_count; i++) {
      if (AIActionParseSpeedFn != NULL &&
          AIActionParseSpeedFn(params[i], &packet->goal_speed_mode) != 0)
        continue;
      if (NuStrICmp(params[i], "ANTICLOCKWISE") == 0)
        packet->circle_clockwise = 0;
      else if (NuStrICmp(params[i], "CLOCKWISE") == 0)
        packet->circle_clockwise = 1;
      else if (NuStrICmp(params[i], "REVERSE") == 0)
        packet->circle_clockwise = !packet->circle_clockwise;
      else if (NuStrICmp(params[i], "facing") == 0)
        processor->action_data_1 |= 1;
      else if (NuStrICmp(params[i], "can_go_off_path") == 0)
        processor->action_data_1 |= 2;
      else if (NuStrICmp(params[i], "currentdist") == 0)
        processor->action_data_1 |= 4;
      else
        packet->movement_param = AIParamToFloatEx(packet, processor, params[i]);
    }
  }
  if (PLAYER(sys, 0) != NULL) {
    if ((processor->action_data_1 & 4) != 0)
      packet->movement_param = NuVecXZDist(
          (nuvec_s *)((u8 *)packet + 0x14c),
          (nuvec_s *)((u8 *)PLAYER(sys, 0)->ai + 0x174), &difference);
    nuvec_s *position = (processor->action_data_1 & 2) != 0
                            ? (nuvec_s *)((u8 *)PLAYER(sys, 0)->ai + 0x14c)
                            : (nuvec_s *)((u8 *)PLAYER(sys, 0)->ai + 0x174);
    AIMoveInstructionInline(packet, position,
                            *(f32 *)((u8 *)PLAYER(sys, 0)->ai + 0x120),
                            (AIPATHINFO7_s *)((u8 *)PLAYER(sys, 0)->ai + 0x158),
                            3, packet->movement_param);
    if ((processor->action_data_1 & 1) != 0)
      packet->look_target = position;
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x006ba710
i32 Action_CircleOpponent(AISYS_s *sys, AISCRIPTPROCESS_s *processor,
                          AIPACKET_s *packet, char **params, i32 param_count,
                          i32 first_time, f32 elapsed) {
  nuvec_s difference;
  if (packet == NULL)
    return 1;
  if (first_time != 0) {
    for (i32 i = 0; i < param_count; i++) {
      if (AIActionParseSpeedFn != NULL &&
          AIActionParseSpeedFn(params[i], &packet->goal_speed_mode) != 0)
        continue;
      if (NuStrICmp(params[i], "ANTICLOCKWISE") == 0)
        packet->circle_clockwise = 0;
      else if (NuStrICmp(params[i], "CLOCKWISE") == 0)
        packet->circle_clockwise = 1;
      else if (NuStrICmp(params[i], "REVERSE") == 0)
        packet->circle_clockwise = !packet->circle_clockwise;
      else if (NuStrICmp(params[i], "RANDOM") == 0) {
        if (NuRandFloat() < 0.5f)
          packet->circle_clockwise = 0;
        else
          packet->circle_clockwise = 1;
      } else if (NuStrICmp(params[i], "facing") == 0)
        processor->action_data_1 |= 1;
      else if (NuStrICmp(params[i], "can_go_off_path") == 0)
        processor->action_data_1 |= 2;
      else if (NuStrICmp(params[i], "currentdist") == 0)
        processor->action_data_1 |= 4;
      else
        packet->movement_param = AIParamToFloatEx(packet, processor, params[i]);
    }
  }
  if (packet->pe4 != NULL && packet->pe4->ai != NULL) {
    if ((processor->action_data_1 & 4) != 0)
      packet->movement_param =
          NuVecXZDist((nuvec_s *)((u8 *)packet + 0x14c),
                      (nuvec_s *)((u8 *)packet->pe4->ai + 0x174), &difference);
    nuvec_s *position = (processor->action_data_1 & 2) != 0
                            ? (nuvec_s *)((u8 *)packet->pe4->ai + 0x14c)
                            : (nuvec_s *)((u8 *)packet->pe4->ai + 0x174);
    AIMoveInstructionInline(packet, position,
                            *(f32 *)((u8 *)packet->pe4->ai + 0x120),
                            (AIPATHINFO7_s *)((u8 *)packet->pe4->ai + 0x158), 3,
                            packet->movement_param);
    if ((processor->action_data_1 & 1) != 0)
      packet->look_target = position;
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x006ba970
i32 Action_MoveAwayFromOpponent(AISYS_s *sys, AISCRIPTPROCESS_s *processor,
                                AIPACKET_s *packet, char **params,
                                i32 param_count, i32 first_time, f32 elapsed) {
  if (packet == NULL)
    return 1;
  if (first_time != 0) {
    for (i32 i = 0; i < param_count; i++) {
      if (AIActionParseSpeedFn != NULL &&
          AIActionParseSpeedFn(params[i], &packet->goal_speed_mode) != 0)
        continue;
      if (NuStrICmp(params[i], "face") == 0)
        processor->action_data_1 = 1;
      else
        packet->movement_param = AIParamToFloatEx(packet, processor, params[i]);
    }
  }
  Unk_AIPacketObj *target = packet->pe4;
  if (target != NULL && target->ai != NULL) {
    AIMoveInstructionInline(packet, (nuvec_s *)((u8 *)target->ai + 0x174),
                            *(f32 *)((u8 *)target->ai + 0x120),
                            (AIPATHINFO7_s *)((u8 *)target->ai + 0x158), 2,
                            packet->movement_param);
    if (processor->action_data_1 != 0)
      packet->look_target = &target->pos5c;
  }
  return 0;
}

// STUB: LEGOBATMAN 0x006baab0
// close: orig keeps processor in ebx (count from memory); ours keeps count.
i32 Action_MoveAwayFromPlayer(AISYS_s *sys, AISCRIPTPROCESS_s *processor,
                              AIPACKET_s *packet, char **params,
                              i32 param_count, i32 first_time, f32 elapsed) {
  if (packet == NULL)
    return 1;
  if (first_time != 0) {
    for (i32 i = 0; i < param_count; i++) {
      if (AIActionParseSpeedFn != NULL &&
          AIActionParseSpeedFn(params[i], &packet->goal_speed_mode) != 0)
        continue;
      if (NuStrICmp(params[i], "face") == 0)
        processor->action_data_1 = 1;
      else
        packet->movement_param = AIParamToFloatEx(packet, processor, params[i]);
    }
  }
  if (PLAYER(sys, 0) == NULL)
    return 0;
  u8 *target = (u8 *)PLAYER(sys, 0)->ai;
  AIMoveInstructionInline(
      packet, (nuvec_s *)(target + 0x174), *(f32 *)(target + 0x120),
      (AIPATHINFO7_s *)(target + 0x158), 2, packet->movement_param);
  if (processor->action_data_1 != 0)
    packet->look_target = &PLAYER(sys, 0)->pos5c;
  return 0;
}

// STUB: LEGOBATMAN 0x006babf0
// close: same register choice as MoveAwayFromPlayer.
i32 Action_MoveAwayFromPlayer2(AISYS_s *sys, AISCRIPTPROCESS_s *processor,
                               AIPACKET_s *packet, char **params,
                               i32 param_count, i32 first_time, f32 elapsed) {
  if (packet == NULL)
    return 1;
  if (first_time != 0) {
    for (i32 i = 0; i < param_count; i++) {
      if (AIActionParseSpeedFn != NULL &&
          AIActionParseSpeedFn(params[i], &packet->goal_speed_mode) != 0)
        continue;
      if (NuStrICmp(params[i], "face") == 0)
        processor->action_data_1 = 1;
      else
        packet->movement_param = AIParamToFloatEx(packet, processor, params[i]);
    }
  }
  if (PLAYER(sys, 1) == NULL)
    return 0;
  u8 *target = (u8 *)PLAYER(sys, 1)->ai;
  AIMoveInstructionInline(
      packet, (nuvec_s *)(target + 0x174), *(f32 *)(target + 0x120),
      (AIPATHINFO7_s *)(target + 0x158), 2, packet->movement_param);
  if (processor->action_data_1 != 0)
    packet->look_target = &PLAYER(sys, 1)->pos5c;
  return 0;
}

// the out-of-line copy (0x006b7110)
extern "C" void AIMoveInstruction(AIPACKET_s *packet, nuvec_s *destination,
                                  f32 stopping_distance,
                                  AIPATHINFO7_s *path_info, i32 mode,
                                  f32 movement_parameter);
f32 NuVecXZDistSqr(nuvec_s *a, nuvec_s *b, nuvec_s *d);
struct AIPATHNODE_s *AIPathFindNode(AISYS_s *system, struct AIPATH_s *path,
                                    char *name);

struct GTNPATHCNX_s {
  u8 pad0[0x10];
  u8 node_indices[2]; // 0x10
};

struct GTNPATHNODE_s {
  char *name;       // 0x00
  nuvec_s position; // 0x04
  u8 pad10[4];
  f32 radius_squared; // 0x14
  u8 pad18[0x28 - 0x18];
  u8 connection_count; // 0x28
  u8 pad29[0x34 - 0x29];
  GTNPATHCNX_s **connections; // 0x34
  u8 pad38[0x5c - 0x38];
};

struct GTNPATH_s {
  u8 pad0[0x7c];
  GTNPATHNODE_s *nodes; // 0x7c
};

// processor +0x84
struct GTNPATHINFO_s {
  GTNPATH_s *path;          // 0x00
  GTNPATHCNX_s *connection; // 0x04
  u8 direction;             // 0x08
  u8 pad9[0xe - 9];
  u16 flags; // 0x0e
  f32 dist;  // 0x10
  f32 width; // 0x14
};

#define GTN_PATHINFO(p) ((GTNPATHINFO_s *)((u8 *)(p) + 0x84))

// STUB: LEGOBATMAN 0x006baf30
// close: only the epilogue sharing differs (orig keeps the first_time
// return 0 separate and merges param_count==0 with the final one).
i32 Action_GoToNode(AISYS_s *sys, AISCRIPTPROCESS_s *processor,
                    AIPACKET_s *packet, char **params, i32 param_count,
                    i32 first_time, f32 elapsed) {
  nuvec_s difference;
  if (packet == NULL || packet->pd0 == NULL || packet->path_set == NULL ||
      packet->path_node == NULL)
    return 1;
  if (first_time != 0) {
    if (param_count == 0)
      return 0;
    for (i32 index = 1; index < param_count; ++index) {
      if (AIActionParseSpeedFn == NULL ||
          AIActionParseSpeedFn(params[index], &packet->goal_speed_mode) == 0)
        packet->movement_param =
            AIParamToFloatEx(packet, processor, params[index]);
    }
    processor->action_data_3 =
        AIPathFindNode(sys, (struct AIPATH_s *)packet->path_set, params[0]);
    GTNPATHNODE_s *node = (GTNPATHNODE_s *)processor->action_data_3;
    if (node == NULL || node->connection_count == 0)
      return 1;
    {
      GTNPATH_s *path = (GTNPATH_s *)packet->path_set;
      i32 node_index = node - path->nodes;
      GTN_PATHINFO(processor)->path = path;
      GTN_PATHINFO(processor)->connection = node->connections[0];
      if (GTN_PATHINFO(processor)->connection->node_indices[0] == node_index)
        GTN_PATHINFO(processor)->dist = 0.0f;
      else
        GTN_PATHINFO(processor)->dist = 1.0f;
      GTN_PATHINFO(processor)->flags |= 1;
      GTN_PATHINFO(processor)->width = 0.0f;
      GTN_PATHINFO(processor)->direction = 0;
      AIMoveInstruction(packet, &node->position, 0.0f,
                        (AIPATHINFO7_s *)GTN_PATHINFO(processor), 1,
                        packet->movement_param);
    }
    return 0;
  }
  GTNPATHNODE_s *node = (GTNPATHNODE_s *)processor->action_data_3;
  if (node == NULL)
    return 1;
  f32 distance_squared = NuVecXZDistSqr((nuvec_s *)((u8 *)packet + 0x14c),
                                        &node->position, &difference);
  AIMoveInstruction(packet, &node->position, 0.0f,
                    (AIPATHINFO7_s *)GTN_PATHINFO(processor), 1,
                    packet->movement_param);
  if (distance_squared < node->radius_squared)
    return 1;
  return 0;
}

// FUNCTION: LEGOBATMAN 0x006bae30
i32 Action_RetreatFromNearestOpponent(AISYS_s *sys,
                                      AISCRIPTPROCESS_s *processor,
                                      AIPACKET_s *packet, char **params,
                                      i32 param_count, i32 first_time,
                                      f32 elapsed) {
  if (packet == NULL)
    return 1;
  if (first_time != 0) {
    packet->movement_param = 1.0f;
    for (i32 i = 0; i < param_count; i++) {
      if (AIActionParseSpeedFn == NULL ||
          AIActionParseSpeedFn(params[i], &packet->goal_speed_mode) == 0)
        packet->movement_param = AIParamToFloatEx(packet, processor, params[i]);
    }
  }
  if (packet->pd4 != NULL) {
    u8 *target = (u8 *)packet->pd4->ai;
    AIMoveInstructionInline(
        packet, (nuvec_s *)(target + 0x174), *(f32 *)(target + 0x120),
        (AIPATHINFO7_s *)(target + 0x158), 2, packet->movement_param);
  }
  return 0;
}

// STUB: LEGOBATMAN 0x006bb100
// close: GoToNode's epilogue puzzle (orig keeps two "return 0" tails).
i32 Action_GoToNodeRandom(AISYS_s *sys, AISCRIPTPROCESS_s *processor,
                          AIPACKET_s *packet, char **params, i32 param_count,
                          i32 first_time, f32 elapsed) {
  nuvec_s difference;
  if (packet == NULL || packet->pd0 == NULL || packet->path_set == NULL ||
      packet->path_node == NULL)
    return 1;
  if (first_time != 0) {
    if (param_count != 0) {
      i32 selected = (i32)(NuRandFloat() * param_count);
      if (selected >= param_count)
        selected = param_count - 1;
      processor->action_data_3 = AIPathFindNode(
          sys, (struct AIPATH_s *)packet->path_set, params[selected]);
      GTNPATHNODE_s *node = (GTNPATHNODE_s *)processor->action_data_3;
      if (node == NULL || node->connection_count == 0)
        return 1;
      GTNPATH_s *path = (GTNPATH_s *)packet->path_set;
      i32 node_index = node - path->nodes;
      GTN_PATHINFO(processor)->path = path;
      GTN_PATHINFO(processor)->connection = node->connections[0];
      if (GTN_PATHINFO(processor)->connection->node_indices[0] == node_index)
        GTN_PATHINFO(processor)->dist = 0.0f;
      else
        GTN_PATHINFO(processor)->dist = 1.0f;
      GTN_PATHINFO(processor)->flags |= 1;
      GTN_PATHINFO(processor)->width = 0.0f;
      GTN_PATHINFO(processor)->direction = 0;
      AIMoveInstruction(packet, &node->position, 0.0f,
                        (AIPATHINFO7_s *)GTN_PATHINFO(processor), 1,
                        packet->movement_param);
      return 0;
    }
  } else {
    GTNPATHNODE_s *node = (GTNPATHNODE_s *)processor->action_data_3;
    if (node == NULL)
      return 1;
    f32 distance_squared = NuVecXZDistSqr((nuvec_s *)((u8 *)packet + 0x14c),
                                          &node->position, &difference);
    AIMoveInstruction(packet, &node->position, 0.0f,
                      (AIPATHINFO7_s *)GTN_PATHINFO(processor), 1,
                      packet->movement_param);
    if (distance_squared < node->radius_squared)
      return 1;
  }
  return 0;
}

f32 NuVecDistSqr(nuvec_s *a, nuvec_s *b, nuvec_s *d);
void NuVecRotateY(nuvec_s *v, nuvec_s *v0, i32 a);
void NuVecAdd(nuvec_s *out, nuvec_s *a, nuvec_s *b);
extern f32 ai_moveradius;

// Batman's path cursor is a dword longer, so the timer sits at +0xa0.
#define GTO_TIMER(p) (*(f32 *)((u8 *)(p) + 0xa0))

// AISYS_s +0x224 entries
struct GTOCREATURE_s {
  u8 pad0[0x20];
  nuvec_s pos;             // 0x20
  i32 y_rot;               // 0x2c
  AIPATHINFO7_s path_info; // 0x30
  u8 pad4c[0xa8 - 0x4c];
};

// STUB: LEGOBATMAN 0x006bb2a0
// close: logic lines up; orig keeps creature in ebp and pushes ebx before
// the first_time branch, ours puts creature in ebx (decl order, typed
// struct tried).
i32 Action_GoToOrigin(AISYS_s *sys, AISCRIPTPROCESS_s *processor,
                      AIPACKET_s *packet, char **params, i32 param_count,
                      i32 first_time, f32 elapsed) {
  GTOCREATURE_s *creature;
  nuvec_s *origin;
  f32 min_time = 0.0f;
  f32 max_time = 0.0f;
  nuvec_s difference;
  if (packet == NULL || packet->pd0 == NULL || packet->path_set == NULL ||
      packet->path_node == NULL || (packet->pd0->flags1f8 & 0x400) == 0 ||
      packet->origin_index == 0xff)
    return 1;
  creature = &(*(GTOCREATURE_s **)((u8 *)sys + 0x224))[packet->origin_index];
  origin =
      GetAICreatureOriginFn != NULL ? GetAICreatureOriginFn(sys, packet) : NULL;
  if (origin == NULL)
    origin = &creature->pos;

  if (first_time != 0) {
    packet->movement_param = 0.2f;
    if (param_count != 0) {
      for (i32 index = 0; index < param_count; ++index) {
        if (AIActionParseSpeedFn != NULL &&
            AIActionParseSpeedFn(params[index], &packet->goal_speed_mode) != 0)
          continue;
        char *value = NuStrIStr(params[index], "waittime");
        if (value != NULL)
          GTO_TIMER(processor) = AIParamToFloatEx(
              packet, processor, value + NuStrLen("waittime") + 1);
        else if ((value = NuStrIStr(params[index], "mintime")) != NULL)
          min_time = AIParamToFloatEx(packet, processor,
                                      value + NuStrLen("mintime") + 1);
        else if ((value = NuStrIStr(params[index], "maxtime")) != NULL)
          max_time = AIParamToFloatEx(packet, processor,
                                      value + NuStrLen("maxtime") + 1);
        else if (NuStrICmp(params[index], "xz_rangecheck") == 0)
          processor->action_data_2 = 1;
        else if ((value = NuStrIStr(params[index], "goalrange")) != NULL)
          packet->movement_param = AIParamToFloatEx(
              packet, processor, value + NuStrLen("goalrange") + 1);
        else
          packet->movement_param =
              AIParamToFloatEx(packet, processor, params[index]);
      }
    }
    if (GTO_TIMER(processor) == 0.0f) {
      if (max_time > min_time)
        GTO_TIMER(processor) = NuRandFloat() * (max_time - min_time) + min_time;
      else
        GTO_TIMER(processor) = 0.01f;
    }
    AIMoveInstruction(packet, origin, 0.0f, &creature->path_info, 1,
                      packet->movement_param);
    processor->action_pos.x = 0.0f;
    processor->action_pos.y = 0.0f;
    processor->action_pos.z = 1.0f;
    NuVecRotateY(&processor->action_pos, &processor->action_pos,
                 creature->y_rot);
    NuVecAdd(&processor->action_pos, &processor->action_pos, origin);
    return 0;
  }

  AIMoveInstruction(packet, origin, 0.0f, &creature->path_info, 1,
                    packet->movement_param);
  f32 distance_squared;
  if (processor->action_data_2 != 0)
    distance_squared =
        NuVecXZDistSqr((nuvec_s *)((u8 *)packet + 0x14c), origin, &difference);
  else
    distance_squared =
        NuVecDistSqr((nuvec_s *)((u8 *)packet + 0x14c), origin, &difference);
  f32 range = ai_moveradius + packet->movement_param;
  range += *(f32 *)((u8 *)packet->pd0 + 0x22c) * elapsed;
  if (distance_squared < range * range) {
    packet->look_target = &processor->action_pos;
    if (GTO_TIMER(processor) > 0.0f) {
      f32 remaining_time = GTO_TIMER(processor) - elapsed;
      GTO_TIMER(processor) = remaining_time;
      if (remaining_time < 0.0f)
        GTO_TIMER(processor) = 0.0f;
    } else
      return 1;
  }
  return 0;
}

// AIGROUP_s rows, 0x38 bytes each
struct AIROW_s {
  AIPATHINFO7_s path_info; // 0x00
  nuvec_s pos;             // 0x1c
  i32 y_rot;               // 0x28
  u8 pad2c[0x31 - 0x2c];
  u8 is_alive; // 0x31
  u8 pad32[2];
  u8 : 1;
  u8 is_turning : 1; // 0x34 bit 1
  u8 pad35[3];
};

struct AIGROUP_s {
  Unk_AIPacketObj *leader; // 0x00
  u8 pad4[2];
  u8 row_count; // 0x06
  u8 pad7;
  u8 count_across; // 0x08
  u8 pad9[0x50 - 9];
  u32 : 2;
  u32 is_reversed : 1;     // 0x50 bit 2
  u32 is_in_formation : 1; // 0x50 bit 3
  u32 is_row_turning : 1;  // 0x50 bit 4
  AIROW_s rows[4];         // 0x54
  u8 pad134[4];
  f32 x_spacing; // 0x138
};

typedef i32 (*AIROWMOVEFN)(AIGROUP_s *group, AIROW_s *row, AIROW_s *previous,
                           Unk_AIPacketObj *leader);
i32 RowMoveWander(AIGROUP_s *group, AIROW_s *row, AIROW_s *previous,
                  Unk_AIPacketObj *leader);
i32 RowMoveTowards(AIGROUP_s *group, AIROW_s *row, AIROW_s *previous,
                   Unk_AIPacketObj *leader);
void AIFormationFollow(AIPACKET_s *packet);

// Takes the group in esi: a static whose callers all live in this TU.
// FUNCTION: LEGOBATMAN 0x006ac0d0
static void FormationMove(AIGROUP_s *group, AIROWMOVEFN move) {
  AIROW_s *previous = NULL;
  i32 turning = 0;
  if (move != NULL && group->is_in_formation) {
    if (group->is_reversed) {
      for (i32 i = group->row_count - 1; i >= 0; --i) {
        AIROW_s *row = &group->rows[i];
        if (row->is_alive) {
          if (move(group, row, previous, group->leader) != 0)
            break;
          if (row->is_turning)
            turning = 1;
          previous = row;
        }
      }
    } else {
      for (i32 i = 0; i < group->row_count; ++i) {
        AIROW_s *row = &group->rows[i];
        if (row->is_alive) {
          if (move(group, row, previous, group->leader) != 0)
            break;
          if (row->is_turning)
            turning = 1;
          previous = row;
        }
      }
    }
  }
  group->is_row_turning = turning;
}

// The whole of AIMoveInstruction, formation case included; constant modes
// other than 1/4/5 fold the group test away.
static __forceinline void
AIMoveInstructionFull(AIPACKET_s *packet, nuvec_s *destination,
                      f32 stopping_distance, AIPATHINFO7_s *path_info, i32 mode,
                      f32 movement_parameter) {
  AIGROUP_s *group = packet->group;
  if (group != NULL && group->is_in_formation) {
    switch (mode) {
    case 1:
      if (group->leader == packet->pd0)
        FormationMove(group, RowMoveTowards);
      AIFormationFollow(packet);
      return;
    case 4:
      if (group->leader == packet->pd0)
        FormationMove(group, RowMoveWander);
      AIFormationFollow(packet);
      return;
    case 5:
      mode = 1;
      break;
    }
  }
  if (destination != NULL)
    *(nuvec_s *)((u8 *)packet + 0x1b8) = *destination;
  if (path_info != NULL)
    *(AIPATHINFO7_s *)((u8 *)packet + 0x1d0) = *path_info;
  *(f32 *)((u8 *)packet + 0x1c4) = stopping_distance;
  packet->movement_mode = mode;
  *(f32 *)((u8 *)packet + 0x1c8) = movement_parameter;
}

// FUNCTION: LEGOBATMAN 0x006b7110
extern "C" void AIMoveInstruction(AIPACKET_s *packet, nuvec_s *destination,
                                  f32 stopping_distance,
                                  AIPATHINFO7_s *path_info, i32 mode,
                                  f32 movement_parameter) {
  AIGROUP_s *group = packet->group;
  if (group != NULL && group->is_in_formation) {
    switch (mode) {
    case 1:
      if (group->leader == packet->pd0)
        FormationMove(group, RowMoveTowards);
      AIFormationFollow(packet);
      return;
    case 4:
      if (group->leader == packet->pd0)
        FormationMove(group, RowMoveWander);
      AIFormationFollow(packet);
      return;
    case 5:
      mode = 1;
      break;
    }
  }
  if (destination != NULL)
    *(nuvec_s *)((u8 *)packet + 0x1b8) = *destination;
  if (path_info != NULL)
    *(AIPATHINFO7_s *)((u8 *)packet + 0x1d0) = *path_info;
  *(f32 *)((u8 *)packet + 0x1c4) = stopping_distance;
  packet->movement_mode = mode;
  *(f32 *)((u8 *)packet + 0x1c8) = movement_parameter;
}
// STUB: LEGOBATMAN 0x006b71e0
// close: orig hoists one fldz above the column branches and reuses it (as
// FollowPath), and multiplies spacing by the int with fimul; ours fild/fmul.
void AIFormationFollow(AIPACKET_s *packet) {
  AIGROUP_s *group = packet->group;
  if (packet->group_row < group->row_count) {
    AIROW_s *row = &group->rows[packet->group_row];
    u8 column = packet->group_column;
    nuvec_s offset;
    if ((*((u8 *)packet + 0x1f3) & 1) != 0) {
      offset.x =
          (group->count_across & 1) != 0 ? 0.0f : -(0.5f * group->x_spacing);
    } else {
      offset.x = group->x_spacing * ((column + 1) / 2);
      if ((column & 1) != 0)
        offset.x = -offset.x;
    }
    if (group->is_reversed)
      offset.x = -offset.x;
    offset.y = 0.0f;
    offset.z = 0.0f;
    NuVecRotateY(&offset, &offset, row->y_rot);
    nuvec_s destination;
    NuVecAdd(&destination, &offset, &row->pos);
    packet->movement_event_flags |= 8;
    AIMoveInstructionFull(packet, &destination, 0.0f, &row->path_info, 5, 0.0f);
    nuvec_s *look = (nuvec_s *)((u8 *)packet + 0x78);
    look->x = 0.0f;
    look->y = 0.0f;
    look->z = 100.0f;
    NuVecRotateY(look, look, row->y_rot);
    NuVecAdd(look, look, &row->pos);
    packet->look_target = look;
  }
}

// STUB: LEGOBATMAN 0x006bbe00
// close: orig keeps the 0.0f (completion_time, stopping distance) live on
// the x87 stack from entry, ours re-materialises it with fldz (3 tries).
i32 Action_FollowPath(AISYS_s *sys, AISCRIPTPROCESS_s *processor,
                      AIPACKET_s *packet, char **params, i32 param_count,
                      i32 first_time, f32 elapsed) {
  f32 completion_time = 0.0f;
  f32 min_time = completion_time;
  f32 max_time = completion_time;
  if (packet == NULL || packet->pd0 == NULL || packet->pd0->obj == NULL)
    return 1;
  if (packet->path_set == NULL || packet->path_node == NULL)
    return 0;
  if (first_time != 0) {
    *(void **)((u8 *)packet + 0x18c) = NULL;
    for (i32 index = 0; index < param_count; ++index) {
      if (AIActionParseSpeedFn != NULL &&
          AIActionParseSpeedFn(params[index], &packet->goal_speed_mode) != 0)
        continue;
      char *value = NuStrIStr(params[index], "mintime");
      if (value != NULL) {
        min_time = AIParamToFloatEx(packet, processor, value + 8);
        continue;
      }
      value = NuStrIStr(params[index], "maxtime");
      if (value != NULL) {
        max_time = AIParamToFloatEx(packet, processor, value + 8);
        continue;
      }
      GTO_TIMER(processor) = AIParamToFloatEx(packet, processor, params[index]);
    }
    if (max_time > min_time)
      GTO_TIMER(processor) =
          NuRandFloat() * max_time + (1.0f - NuRandFloat()) * min_time;
  }
  AIMoveInstructionFull(packet, NULL, 0.0f, NULL, 4, packet->movement_param);
  if (!(GTO_TIMER(processor) > completion_time))
    return 0;
  GTO_TIMER(processor) -= elapsed;
  return completion_time >= GTO_TIMER(processor);
}

// FUNCTION: LEGOBATMAN 0x006bde60
void AIBugPitOwnerA::Release() {
  if (owned) {
    g_memPool->Free(data, size, 0);
    data = 0;
  }
}

// FUNCTION: LEGOBATMAN 0x006be050
void AIBugPitOwnerB::Release() {
  if (owned) {
    g_memPool->Free(data, size, 0);
    data = 0;
  }
}

// Script condition inits (saga aisys.cpp), reached through the condition
// keyword table.

void *AISysFindArea(AISYS_s *sys, char *name);
struct AILOCATOR_s *AIPathFindLocator(AISYS_s *aisys, char *name);
void *AIPathFindNode(AISYS_s *sys, void *path, char *name);

struct AIPathSysInit_s {
  u8 path_count; // 0x00
  u8 pad1[8 - 1];
  void *active_path; // 0x08
};

// FUNCTION: LEGOBATMAN 0x006b3ca0
void *Condition_LocatorRangeInit(AISYS_s *sys, char *arg, AISCRIPT_s *script) {
  return arg != NULL ? AIPathFindLocator(sys, arg) : NULL;
}

// FUNCTION: LEGOBATMAN 0x006b3cc0
void *Condition_CurrentLocatorIsInit(AISYS_s *sys, char *arg,
                                     AISCRIPT_s *script) {
  return arg != NULL ? AIPathFindLocator(sys, arg) : NULL;
}

// FUNCTION: LEGOBATMAN 0x006b3ce0
void *Condition_InTriggerAreaInit(AISYS_s *sys, char *arg, AISCRIPT_s *script) {
  return arg != NULL ? AISysFindArea(sys, arg) : NULL;
}

// FUNCTION: LEGOBATMAN 0x006b3d00
void *Condition_InLevelNodeInit(AISYS_s *sys, char *arg, AISCRIPT_s *script) {
  // 0x21c: the path system.
  AIPathSysInit_s *path_sys;
  if (sys != NULL &&
      (path_sys = *(AIPathSysInit_s **)((u8 *)sys + 0x21c)) != NULL &&
      path_sys->path_count != 0)
    return AIPathFindNode(sys, path_sys->active_path, arg);
  return NULL;
}

// FUNCTION: LEGOBATMAN 0x006b3d40
void *Condition_PlayerInLevelNodeInit(AISYS_s *sys, char *arg,
                                      AISCRIPT_s *script) {
  // 0x21c: the path system.
  AIPathSysInit_s *path_sys;
  if (sys != NULL &&
      (path_sys = *(AIPathSysInit_s **)((u8 *)sys + 0x21c)) != NULL &&
      path_sys->path_count != 0)
    return AIPathFindNode(sys, path_sys->active_path, arg);
  return NULL;
}

// FUNCTION: LEGOBATMAN 0x006b3d80
void *Condition_LevelNodeRangeInit(AISYS_s *sys, char *arg,
                                   AISCRIPT_s *script) {
  // 0x21c: the path system.
  AIPathSysInit_s *path_sys;
  if (sys != NULL &&
      (path_sys = *(AIPathSysInit_s **)((u8 *)sys + 0x21c)) != NULL &&
      path_sys->path_count != 0)
    return AIPathFindNode(sys, path_sys->active_path, arg);
  return NULL;
}

// FUNCTION: LEGOBATMAN 0x006b3dc0
void *Condition_PlayerInTriggerAreaInit(AISYS_s *sys, char *arg,
                                        AISCRIPT_s *script) {
  return arg != NULL ? AISysFindArea(sys, arg) : NULL;
}

// FUNCTION: LEGOBATMAN 0x006b3de0
void *Condition_BaddyInTriggerAreaInit(AISYS_s *sys, char *arg,
                                       AISCRIPT_s *script) {
  return arg != NULL ? AISysFindArea(sys, arg) : NULL;
}

// FUNCTION: LEGOBATMAN 0x006b3e00
void *Condition_GoodyInTriggerAreaInit(AISYS_s *sys, char *arg,
                                       AISCRIPT_s *script) {
  return arg != NULL ? AISysFindArea(sys, arg) : NULL;
}

// FUNCTION: LEGOBATMAN 0x006b3e20
void *Condition_OpponentInTriggerAreaInit(AISYS_s *sys, char *arg,
                                          AISCRIPT_s *script) {
  return arg != NULL ? AISysFindArea(sys, arg) : NULL;
}

// FUNCTION: LEGOBATMAN 0x006b3e40
void *Condition_OpponentToLocatorInit(AISYS_s *sys, char *arg,
                                      AISCRIPT_s *script) {
  return arg != NULL ? AIPathFindLocator(sys, arg) : NULL;
}

// FUNCTION: LEGOBATMAN 0x006b3e60
void *Condition_PlayerToLocatorInit(AISYS_s *sys, char *arg,
                                    AISCRIPT_s *script) {
  return arg != NULL ? AIPathFindLocator(sys, arg) : NULL;
}

struct AILOCATOR_s {
  char name[0x10];  // 0x00
  nuvec_s position; // 0x10
};

// STUB: LEGOBATMAN 0x006b3e80
// close: orig keeps a separate epilogue for the guard return 1 and holds the
// locator in ebx (epilogue puzzle); arg registers differ too.
i32 Action_FaceLocator(AISYS_s *sys, AISCRIPTPROCESS_s *processor,
                       AIPACKET_s *packet, char **params, i32 param_count,
                       i32 first_time, f32 elapsed) {
  if (packet != NULL && packet->pd0 != NULL && packet->pd0->obj != NULL &&
      packet->path_set != NULL && packet->path_node != NULL) {
    if (first_time != 0) {
      processor->action_data_3 = processor->locator_set;
      for (i32 index = 0; index < param_count; ++index) {
        if (AIActionParseSpeedFn != NULL &&
            AIActionParseSpeedFn(params[index], &packet->goal_speed_mode) !=
                0) {
          continue;
        }
        char *value = NuStrIStr(params[index], "name");
        if (value != NULL) {
          ++index;
          processor->action_data_3 =
              AIPathFindLocator(sys, value + NuStrLen("name") + 1);
        }
      }
    }
    AILOCATOR_s *locator = (AILOCATOR_s *)processor->action_data_3;
    if (locator != NULL) {
      packet->look_target = &locator->position;
      return 1;
    }
    return 0;
  }
  return 1;
}

// GLOBAL: LEGOBATMAN 0x00ad68e0
extern AISCRIPTPROCESS_s *g_unk00ad68e0;

static void AiSysSetStateDebugee(AISCRIPTPROCESS_s *process) {
  g_unk00ad68e0 = process;
}

// FUNCTION: LEGOBATMAN 0x006b48a0
i32 Action_NotifyStateChange(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                             AIPACKET_s *packet, char **args, int argc,
                             int flags, f32 time) {
  i32 enabled = 1;
  for (i32 i = 0; i < argc; i++) {
    if (NuStrICmp(args[i], "false") == 0)
      enabled = 0;
  }
  if (enabled)
    AiSysSetStateDebugee(process);
  else
    AiSysSetStateDebugee(NULL);
  return 1;
}

i32 ConditionsParseLineRunTime(AISYS_s *sys, AIPACKET_s *packet,
                               AISCRIPTPROCESS_s *process, char **args, i32 a);

// 0xb5 bits 0-1: if/else state of the process.
struct AIIfStateB_s {
  u8 state : 2;
};

#define IF_STATE(process) (((AIIfStateB_s *)((u8 *)(process) + 0xb5))->state)

// FUNCTION: LEGOBATMAN 0x006b4910
i32 Action_If(AISYS_s *sys, AISCRIPTPROCESS_s *process, AIPACKET_s *packet,
              char **args, int argc, int flags, f32 time) {
  if (ConditionsParseLineRunTime(sys, packet, process, args, 0) != 0)
    IF_STATE(process) = 0;
  else
    IF_STATE(process) = 1;
  return 1;
}

// FUNCTION: LEGOBATMAN 0x006b4960
i32 Action_OrIf(AISYS_s *sys, AISCRIPTPROCESS_s *process, AIPACKET_s *packet,
                char **args, int argc, int flags, f32 time) {
  if (IF_STATE(process) == 1) {
    if (ConditionsParseLineRunTime(sys, packet, process, args, 0) != 0) {
      IF_STATE(process) = 0;
      return 1;
    }
    IF_STATE(process) = 1;
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x006b49c0
i32 Action_AndIf(AISYS_s *sys, AISCRIPTPROCESS_s *process, AIPACKET_s *packet,
                 char **args, int argc, int flags, f32 time) {
  if (IF_STATE(process) == 0) {
    if (ConditionsParseLineRunTime(sys, packet, process, args, 0) != 0)
      IF_STATE(process) = 0;
    else
      IF_STATE(process) = 1;
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x006b4a20
i32 Action_ElseIf(AISYS_s *sys, AISCRIPTPROCESS_s *process, AIPACKET_s *packet,
                  char **args, int argc, int flags, f32 time) {
  if (IF_STATE(process) != 0 && IF_STATE(process) != 2)
    return Action_If(sys, process, packet, args, argc, flags, time);
  IF_STATE(process) = 2;
  return 1;
}
