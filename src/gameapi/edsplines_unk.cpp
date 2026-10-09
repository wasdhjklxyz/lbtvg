// gameapi/edsplines_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nu2api/nucore/common.h"
#include <stddef.h>

// STUB: LEGOBATMAN 0x005b0510
// x87 + idiv spline-segment math; not attempted
#if 0
#include "../nu2api/nu3d/nuspecial.h"

struct SPLINEPOS_s {
    NUGSPLINE *spline; // 0x00
    i16 segment;       // 0x04
    i8 looping;        // 0x06
    union {
        u8 reached_end;
        u8 finished;
    }; // 0x07
    union {
        f32 segment_distance;
        f32 distance;
    }; // 0x08
    f32 segment_length; // 0x0c
    NUVEC position;     // 0x10
    union {
        f32 along;
        f32 normalized_position;
    }; // 0x1c
};

f32 NuVecDist(NUVEC *v0, NUVEC *v1, NUVEC *d);

void MoveSplinePosition(SPLINEPOS_s *, f32);

// from saga legoapi/render/fx/edsplines.cpp
void InitSplinePosition(SPLINEPOS_s *position, nugspline_s *spline, float distance, i32 looping) {
    if (position == NULL) {
        return;
    }

    SPLINEPOS_s *runtime = position;
    memset(runtime, 0, sizeof(*runtime));
    if (spline == NULL || spline->length < 2) {
        return;
    }

    runtime->spline = spline;
    runtime->looping = static_cast<u8>(looping);
    const NUVEC *first = reinterpret_cast<const NUVEC *>(reinterpret_cast<const u8 *>(spline->pts));
    const NUVEC *second = reinterpret_cast<const NUVEC *>(reinterpret_cast<const u8 *>(spline->pts) + spline->pt_size);
    runtime->segment_length = NuVecDist(const_cast<NUVEC *>(second), const_cast<NUVEC *>(first), NULL);
    if (distance > 0.0f) {
        MoveSplinePosition(position, distance);
    } else {
        runtime->position = *first;
        runtime->along = 0.0f;
    }
}
#endif
