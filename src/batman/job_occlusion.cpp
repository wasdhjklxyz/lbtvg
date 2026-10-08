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
