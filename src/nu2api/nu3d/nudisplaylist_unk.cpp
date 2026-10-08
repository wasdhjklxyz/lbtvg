// nu2api/nu3d/nudisplaylist_unk.cpp: between nuanim_gen.cpp (0x0072c750) and
// job_occlusion.cpp (0x0073bbc0); NuSpecialDrawAt sits next to the display
// list renderer it calls.

#include "nuspecial.h"

int NuDisplayListRndrSpecial(nuhspecial_s *sp, numtx_s *mtx, int a, int b,
                             int c);

// FUNCTION: LEGOBATMAN 0x0073aa10
int NuSpecialDrawAt(nuhspecial_s *sp, numtx_s *mtx) {
  if (sp && sp->scene && sp->display_special)
    return NuDisplayListRndrSpecial(sp, mtx, 0, 0, 0);
  return 0;
}
