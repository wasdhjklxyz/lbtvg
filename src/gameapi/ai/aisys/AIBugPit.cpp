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
