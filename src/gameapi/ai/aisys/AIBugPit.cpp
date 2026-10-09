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
