// nu2api/nu3d/nuanim_gen.cpp: __FILE__ anchor at 0x0072c750
// (NuAnimDataChunkDestroy).

#include <string.h>

typedef struct nuanimcurveset_s NUANIMCURVESET;

typedef struct nuanimdatachunk_s {
  int curve_set_count;         // 0x00
  int unk04;                   // 0x04
  NUANIMCURVESET **curve_sets; // 0x08
  void *curve_data;            // 0x0c
  void *shared_data;           // 0x10
} NUANIMDATACHUNK;

extern "C" void NuMemFreeFn(void *ptr, const char *file, int line);
extern "C" void *NuMemAllocFn(int size, const char *file, int line);

typedef struct nuanimcurve_s {
  void *unk00; // 0x00
  void *data;  // 0x04
} NUANIMCURVE;

struct nuanimcurveset_s {
  void *unk00;          // 0x00
  void *curve_flags;    // 0x04
  NUANIMCURVE **curves; // 0x08
  signed char count;    // 0x0c
  unsigned char pad0d[3];
};

// FUNCTION: LEGOBATMAN 0x0070b290
NUANIMCURVESET *NuAnimCurveSetCreate(int curve_count) {
  NUANIMCURVESET *set = 0;
  if (curve_count != 0) {
    set =
        (NUANIMCURVESET *)NuMemAllocFn(sizeof(NUANIMCURVESET), __FILE__, 0x561);
    memset(set, 0, sizeof(NUANIMCURVESET));
    set->count = (signed char)curve_count;
    set->curves = (NUANIMCURVE **)NuMemAllocFn(curve_count * sizeof(void *),
                                               __FILE__, 0x568);
    memset(set->curves, 0, curve_count * sizeof(void *));
    set->curve_flags =
        NuMemAllocFn(curve_count * sizeof(void *), __FILE__, 0x56d);
    memset(set->curve_flags, 0, curve_count * sizeof(void *));
  }
  return set;
}

// FUNCTION: LEGOBATMAN 0x0070b310
void NuAnimCurveDestroy(NUANIMCURVE *curve) {
  if (curve->data != 0)
    NuMemFreeFn(curve->data, __FILE__, 0x577);
  NuMemFreeFn(curve, __FILE__, 0x578);
}

// FUNCTION: LEGOBATMAN 0x0070b350
void NuAnimCurveSetDestroy(NUANIMCURVESET *set, int destroy_curves) {
  if (set == 0)
    return;
  if (set->curves != 0) {
    if (destroy_curves != 0) {
      for (int i = 0; i < set->count; i++) {
        if (set->curves[i] != 0)
          NuAnimCurveDestroy(set->curves[i]);
      }
    }
    NuMemFreeFn(set->curves, __FILE__, 0x58d);
  }
  if (set->curve_flags != 0)
    NuMemFreeFn(set->curve_flags, __FILE__, 0x591);
  NuMemFreeFn(set, __FILE__, 0x593);
}

// GLOBAL: LEGOBATMAN 0x029f3efc
void *JointProcAnimFn;

// FUNCTION: LEGOBATMAN 0x0070b410
void SetProceduralAnimationFn(void *function) { JointProcAnimFn = function; }

typedef struct nuanimbuff_s {
  int joint_count;  // 0x00
  short max_joints; // 0x04
  short pad06;
  void *joints;               // 0x08
  unsigned char *joint_flags; // 0x0c
} NUANIMBUFF;

// GLOBAL: LEGOBATMAN 0x029f3ef8
extern NUANIMBUFF *globalbuffer;

void *NuScratchAlloc32(int size);
void NuScratchRelease(void);

// FUNCTION: LEGOBATMAN 0x0070b6c0
void NuAnimBuffCreateScratch(NUANIMBUFF *buffer) {
  if (buffer == 0)
    return;
  buffer->max_joints = globalbuffer->max_joints;
  buffer->joint_count = 0;
  buffer->joints = NuScratchAlloc32((buffer->max_joints * 3 + 3) * 0x10);
  buffer->joints = (void *)(((unsigned int)buffer->joints + 0xf) & ~0xf);
  buffer->joint_flags = (unsigned char *)NuScratchAlloc32(buffer->max_joints);
}

// FUNCTION: LEGOBATMAN 0x0070b710
void NuAnimBuffDestroyScratch(NUANIMBUFF *buffer) {
  if (buffer == 0)
    return;
  NuScratchRelease();
  NuScratchRelease();
  buffer->max_joints = 0;
  buffer->joints = 0;
  buffer->joint_flags = 0;
}

// GLOBAL: LEGOBATMAN 0x029f3e02
extern unsigned char ForceEulerToQuat;

// GLOBAL: LEGOBATMAN 0x029f3f00
extern int NumQuatPushes;

// GLOBAL: LEGOBATMAN 0x029ee7a0
extern unsigned char QuatPushes[4];

// FUNCTION: LEGOBATMAN 0x0070b860
unsigned char NuAnimSetUseQuatsFlag(unsigned char enabled) {
  unsigned char previous = ForceEulerToQuat;
  ForceEulerToQuat = enabled;
  return previous;
}

// FUNCTION: LEGOBATMAN 0x0070b870
unsigned char NuAnimGetUseQuatsFlag(void) { return ForceEulerToQuat; }

// FUNCTION: LEGOBATMAN 0x0070b880
unsigned char NuAnimPushSetUseQuatsFlag(unsigned char enabled) {
  unsigned char previous = ForceEulerToQuat;
  if (NumQuatPushes < 4) {
    QuatPushes[NumQuatPushes++] = ForceEulerToQuat;
    ForceEulerToQuat = enabled;
    return previous;
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x0070b8b0
unsigned char NuAnimPopUseQuatsFlag(void) {
  if (NumQuatPushes != 0) {
    ForceEulerToQuat = QuatPushes[--NumQuatPushes];
    return ForceEulerToQuat;
  }
  return 0;
}

typedef struct ani3_animheader_s {
  unsigned int magic;         // 0x00
  unsigned short node_count;  // 0x04
  unsigned short key_count;   // 0x06
  unsigned short key_stride;  // 0x08
  unsigned short frame_count; // 0x0a
  unsigned short curve_count; // 0x0c
  unsigned short first_frame; // 0x0e
} ANI3_ANIMHEADER;

// FUNCTION: LEGOBATMAN 0x0070b930
int NuAnimNumNodes(void *animation) {
  ANI3_ANIMHEADER *header = (ANI3_ANIMHEADER *)animation;
  if ((header->magic & 0xffffff00) == 0x414e4900)
    return header->node_count;
  return *(short *)((unsigned char *)animation + 4);
}

// FUNCTION: LEGOBATMAN 0x0070b950
extern "C" float NuAnimEndFrame(void *animation_data) {
  ANI3_ANIMHEADER *animation = (ANI3_ANIMHEADER *)animation_data;
  if ((animation->magic & 0xffffff00) == 0x414e4900)
    return (float)animation->first_frame + (float)animation->frame_count;
  return *(float *)animation_data;
}

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
