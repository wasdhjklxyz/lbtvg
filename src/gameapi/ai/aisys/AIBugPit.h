// gameapi/ai/aisys/AIBugPit.h: grids used by the "bug pit" AI (strings
// "BugPitSplatterGrid" / "BugPitDensityGrid" at 0x006be180). The two grid
// classes are byte-identical except for cell size (48 vs 32); almost
// certainly one template instantiated twice, written out as two classes
// until the harness can address instantiations.

struct __declspec(align(16)) AIVec {
  float x;
  float y;
  float z;
  float w;

  AIVec() {}
  AIVec(float x_, float y_, float z_, float w_) : x(x_), y(y_), z(z_), w(w_) {}
};

// Allocator object at 0x00ad4388; only Free is known.
struct GMemPool {
  void Free(void *p, int size, int flags);
};

// GLOBAL: LEGOBATMAN 0x00ad4388
extern GMemPool *g_memPool;

struct AIBugPitCell48 {
  unsigned char pad[48];
};

struct AIBugPitCell32 {
  unsigned char pad[0x10];
  int cur;
  int next;
  unsigned char pad2[0x20 - 0x18];
};

struct AIBugPitGridBase {
  float originX;
  float originY;
  float originZ;
  float pad_c;
  float cellSize;
  float invCellSize;
  int cellCount;
  int width;
  int height;

  void CellOf(int index, int *ix, int *iz);
};

struct AIBugPitGrid48 : AIBugPitGridBase {
  AIBugPitCell48 *cells;
  unsigned char clamp;

  void WorldToCell(const AIVec *pos, int *ix, int *iz);
  int IndexOf(int ix, int iz);
  AIVec CellCenter(int ix, int iz);
  AIVec CellCenterOf(const AIBugPitCell48 *cell);
};

struct AIBugPitGrid32 : AIBugPitGridBase {
  AIBugPitCell32 *cells;
  unsigned char clamp;

  void WorldToCell(const AIVec *pos, int *ix, int *iz);
  int IndexOf(int ix, int iz);
  AIVec CellCenter(int ix, int iz);
  AIVec CellCenterOf(const AIBugPitCell32 *cell);
  void AdvanceCells();
};

// Owned buffer: pointer, byte size, owned flag. Two identical copies exist.
struct AIBugPitBufferA {
  void *data;
  int size;
  unsigned char owned;
  void Free();
  void Release();
};

struct AIBugPitBufferB {
  void *data;
  int size;
  unsigned char owned;
  void Free();
  void Release();
};

// Objects holding such a buffer at +0x2c. Two identical copies exist.
struct AIBugPitOwnerA {
  unsigned char pad[0x2c];
  void *data;
  int size;
  unsigned char owned;
  void Release();
};

struct AIBugPitOwnerB {
  unsigned char pad[0x2c];
  void *data;
  int size;
  unsigned char owned;
  void Release();
};
