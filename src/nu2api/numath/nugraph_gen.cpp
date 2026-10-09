// nu2api/numath/nugraph_gen.cpp (leaked __FILE__, anchor 0x0068e660).

#include "../nucore/common.h"
#include <stddef.h>

extern "C" void NuMemFreeFn(void *ptr, const char *file, int line);

struct nuvec_s;

// GLOBAL: LEGOBATMAN 0x00ad3bd4
static nuvec_s *curve;
// GLOBAL: LEGOBATMAN 0x00ad3bd8
static nuvec_s *control;

// from saga legoapi/render/core/nugraph.cpp
// FUNCTION: LEGOBATMAN 0x0068e660
extern "C" void nugraphFreeTempCurveData(void) {
  if (curve != NULL)
    NuMemFreeFn(curve, __FILE__, 0x1f7);
  curve = NULL;
  if (control != NULL)
    NuMemFreeFn(control, __FILE__, 0x1f9);
  control = NULL;
}
