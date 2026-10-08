// batman/job_occlusion.cpp (anchors 0x0073bbc0 and 0x0073c770).
// This TU is unoptimized in the original (frame pointers, spilled locals).
#pragma optimize("", off)

// FUNCTION: LEGOBATMAN 0x0073bd30
void JobOccVec4Set(float *v, float x, float y, float z, float w) {
  v[0] = x;
  v[1] = y;
  v[2] = z;
  v[3] = w;
}

// FUNCTION: LEGOBATMAN 0x0073bda0
void JobOccVec4Mul(float *d, const float *a, const float *b) {
  d[0] = a[0] * b[0];
  d[1] = a[1] * b[1];
  d[2] = a[2] * b[2];
  d[3] = a[3] * b[3];
}

// FUNCTION: LEGOBATMAN 0x0073bdf0
void JobOccVec4Copy(float *d, const float *s) {
  d[0] = s[0];
  d[1] = s[1];
  d[2] = s[2];
  d[3] = s[3];
}

// FUNCTION: LEGOBATMAN 0x0073be30
void JobOccVec3Add(float *d, const float *a, const float *b) {
  d[0] = a[0] + b[0];
  d[1] = a[1] + b[1];
  d[2] = a[2] + b[2];
}

// FUNCTION: LEGOBATMAN 0x0073bf40
void JobOccVec3Scale(float *d, const float *a, float s) {
  d[0] = a[0] * s;
  d[1] = a[1] * s;
  d[2] = a[2] * s;
}

// FUNCTION: LEGOBATMAN 0x0073bf70
void JobOccVec3Sub(float *d, const float *a, const float *b) {
  d[0] = a[0] - b[0];
  d[1] = a[1] - b[1];
  d[2] = a[2] - b[2];
}

// FUNCTION: LEGOBATMAN 0x0073c110
bool JobOccGetBit(unsigned int index, const unsigned char *bits) {
  unsigned int byteIndex = index >> 3;
  unsigned int bitIndex = index - (byteIndex << 3);
  bool result = (bits[byteIndex] >> bitIndex) & 1;
  return result;
}

// FUNCTION: LEGOBATMAN 0x0073c1d0
int JobOccGet2Bits(const unsigned char *bits, unsigned int index) {
  return (bits[index >> 2] >> ((index & 3) << 1)) & 3;
}

void Unk00684e50(float *, float *, float *);
void Unk00684ed0(float *, float *, float *);

// FUNCTION: LEGOBATMAN 0x0073bd60
void JobOccUnk0073bd60(float *a, float *b, float *c) { Unk00684ed0(a, b, c); }

// FUNCTION: LEGOBATMAN 0x0073bd80
void JobOccUnk0073bd80(float *a, float *b, float *c) { Unk00684e50(a, b, c); }

// Transform a 3-vector by a 4x4 matrix (16 floats, m[col*4+row]) into a
// 4-vector. FUNCTION: LEGOBATMAN 0x0073be70
void JobOccVec3TransformMtx(float *d, const float *v, const float *m) {
  d[0] = v[0] * m[0] + v[1] * m[4] + v[2] * m[8] + m[12];
  d[1] = v[0] * m[1] + v[1] * m[5] + v[2] * m[9] + m[13];
  d[2] = v[0] * m[2] + v[1] * m[6] + v[2] * m[10] + m[14];
  d[3] = v[0] * m[3] + v[1] * m[7] + v[2] * m[11] + m[15];
}

// `mask` must be block-scoped: /Od places inner-block locals below the
// function-scope ones.
// FUNCTION: LEGOBATMAN 0x0073c150
void JobOccSet2Bits(unsigned char *bits, unsigned int index,
                    unsigned char value) {
  unsigned int byteIndex = index >> 2;
  unsigned int shift = (index & 3) << 1;
  {
    unsigned char mask = 3 << shift;
    bits[byteIndex] |= mask;
    bits[byteIndex] ^= mask;
  }
  bits[byteIndex] |= value << shift;
}

// True when the box [min, max] (xy in clip space, z >= 0) is not fully inside.
// FUNCTION: LEGOBATMAN 0x0073c510
bool JobOccBoxOutsideClip(const float *max, const float *min) {
  bool result;
  if (!(min[0] < -1.0) && !(min[1] < -1.0) && !(min[2] < 0.0) &&
      !(max[0] > 1.0) && !(max[1] > 1.0))
    result = false;
  else
    result = true;
  return result;
}

// FUNCTION: LEGOBATMAN 0x0073c590
bool JobOccUnk0073c590(const float *a, const float *b, const float *c,
                       const float *d) {
  bool result;
  if (a[2] > d[2] && a[0] > c[0] && a[1] > c[0] && b[0] < d[0] && b[1] < d[1])
    result = true;
  else
    result = false;
  return result;
}

// 16-byte aligned 4-float vector; its methods live in batman/ (0x00492ab0
// empty ctor, 0x00492b70 Set, 0x00512030 xyz scale), reached through
// incremental-link thunks here.
struct __declspec(align(16)) JobOccVec {
  float v[4];
  JobOccVec();
  JobOccVec &Set(const float *src);
  JobOccVec &operator*=(float s);
};

void JobOccAlignedVec4Mul(JobOccVec *vec, const float *m);

struct JobOccVecHolder {
  float v[4];
  JobOccVec Mul(const float *other);
};

// FUNCTION: LEGOBATMAN 0x0073c4b0
JobOccVec JobOccVecHolder::Mul(const float *other) {
  JobOccVec result;
  JobOccVec4Mul(result.v, v, other);
  return result;
}

// FUNCTION: LEGOBATMAN 0x0073c610
void JobOccProject(JobOccVec *vec, const float *m) {
  JobOccVec t;
  JobOccVec3TransformMtx(t.v, vec->v, m);
  vec->Set(t.v);
  *vec *= 1.0f / vec->v[3];
}

struct JobOccVecSet {
  float v[4];
  void Unk00529f70(float x, float y, float z, float w);
};

// GLOBAL: LEGOBATMAN 0x02a11970
int g_jobOccCubeInitialised;
// GLOBAL: LEGOBATMAN 0x02a11a10
JobOccVecSet g_jobOccCube[8];

// FUNCTION: LEGOBATMAN 0x0073bfb0
void JobOccInitCube() {
  if (g_jobOccCubeInitialised == 0) {
    g_jobOccCube[0].Unk00529f70(1.0f, 1.0f, 1.0f, 1.0f);
    g_jobOccCube[1].Unk00529f70(1.0f, 1.0f, -1.0f, 1.0f);
    g_jobOccCube[2].Unk00529f70(-1.0f, 1.0f, -1.0f, 1.0f);
    g_jobOccCube[3].Unk00529f70(-1.0f, 1.0f, 1.0f, 1.0f);
    g_jobOccCube[4].Unk00529f70(-1.0f, -1.0f, 1.0f, 1.0f);
    g_jobOccCube[5].Unk00529f70(-1.0f, -1.0f, -1.0f, 1.0f);
    g_jobOccCube[6].Unk00529f70(1.0f, -1.0f, -1.0f, 1.0f);
    g_jobOccCube[7].Unk00529f70(1.0f, -1.0f, 1.0f, 1.0f);
    g_jobOccCubeInitialised = 1;
  }
}

// Same 16-byte aligned layout, no constructor (no ctor call in the original).
struct __declspec(align(16)) JobOccVecB {
  float v[4];
  JobOccVecB &Unk004a35f0(JobOccVecB *out, const JobOccVecB *other);
};

// Counts edges of a 4-vertex polygon crossed by the horizontal ray from p;
// 0 as soon as two are hit.
// FUNCTION: LEGOBATMAN 0x0073c680
unsigned char JobOccPointInQuad(const float *p, JobOccVecB *quad) {
  unsigned int count = 0;
  unsigned int i;
  for (i = 0; i < 4; i++) {
    JobOccVecB *a = &quad[i];
    JobOccVecB *b = &quad[(i + 1) % 4];
    JobOccVecB d;
    b->Unk004a35f0(&d, a);
    float t = (p[1] - a->v[1]) / d.v[1];
    float x = t * d.v[0] + a->v[0];
    if (t > 0.0 && t < 1.0 && x > p[0])
      count++;
    if (count == 2)
      return 0;
  }
  return (unsigned char)count;
}

void Unk006e2090(void *ptr, const char *file, int line);
void *JobOccUnk0073c770(void **outBuffer, int p, int count);
unsigned char JobOccUnk0073c1f0(void *item, int count, void *buffer, void *p);

struct JobOccHeader {
  unsigned int unk0;
  unsigned char pad[4];
  unsigned int *countPtr;
};
struct JobOccList {
  unsigned char *items;
  unsigned int bits;
  int count;
};
struct JobOccJob {
  JobOccHeader *header;
  unsigned int *flags;
  JobOccList *list;
  int unk0c;
};

// STUB: LEGOBATMAN 0x0073bbc0
void JobOccRun(JobOccJob *job) {
  JobOccInitCube();
  JobOccHeader *header = job->header;
  unsigned int *flags = job->flags;
  JobOccList *list = job->list;
  unsigned int visible = header->unk0;
  unsigned int flagBits = *flags;
  int count = list->count;
  unsigned int total = *header->countPtr;
  void *buffer = 0;
  void *p = JobOccUnk0073c770(&buffer, job->unk0c, count);
  unsigned int n = 0;
  int bit = 0;
  unsigned int bits = list->bits;
  unsigned char *items = list->items;
  unsigned int i;
  for (i = 0; i < bits >> 5 && n < total; i++, n++) {
    while (!JobOccGetBit(bit, (const unsigned char *)flagBits))
      bit++;
    if (JobOccGet2Bits((const unsigned char *)visible, bit) == 1) {
      if (JobOccUnk0073c1f0(items + i * 0x20, count, buffer, p))
        JobOccSet2Bits((unsigned char *)visible, bit, 0);
    }
    bit++;
  }
  Unk006e2090(buffer, ".\\job_occlusion.cpp", 0x124);
}
