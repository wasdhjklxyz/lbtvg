// gameapi/ai/aisys/AIBugPit.cpp: certain range 0x006b1220..0x006be490.

#include "AIBugPit.h"

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
