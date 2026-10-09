// nu2api/nu3d/nuanim_gen.cpp: __FILE__ anchor at 0x0072c750
// (NuAnimDataChunkDestroy).

typedef struct nuanimcurveset_s NUANIMCURVESET;

typedef struct nuanimdatachunk_s {
  int curve_set_count;         // 0x00
  int unk04;                   // 0x04
  NUANIMCURVESET **curve_sets; // 0x08
  void *curve_data;            // 0x0c
  void *shared_data;           // 0x10
} NUANIMDATACHUNK;

extern "C" void NuMemFreeFn(void *ptr, const char *file, int line);
void NuAnimCurveSetDestroy(NUANIMCURVESET *set, int destroy_curves);

// FUNCTION: LEGOBATMAN 0x0072c750
void NuAnimDataChunkDestroy(NUANIMDATACHUNK *chunk) {
  int destroy_curves = chunk->shared_data == 0;

  for (int i = 0; i < chunk->curve_set_count; i++) {
    if (chunk->curve_sets[i] != 0) {
      NuAnimCurveSetDestroy(chunk->curve_sets[i], destroy_curves);
    }
  }

  if (chunk->curve_data != 0) {
    NuMemFreeFn(chunk->curve_data, __FILE__, 0x551);
  }
  if (chunk->shared_data != 0) {
    NuMemFreeFn(chunk->shared_data, __FILE__, 0x553);
  }
  if (chunk->curve_sets != 0) {
    NuMemFreeFn(chunk->curve_sets, __FILE__, 0x556);
  }
  NuMemFreeFn(chunk, __FILE__, 0x558);
}
