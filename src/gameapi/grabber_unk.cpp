// gameapi/grabber_unk.cpp: Grabber_* (saga gizmos_grabber.cpp); file name
// unproven.

#include "../batman/worldinfo_unk.h"
#include "../nu2api/nucore/common.h"

struct GRABBERSYS_s {
  u8 pad000[0x692];
  u8 active; // 0x692
};

// FUNCTION: LEGOBATMAN 0x00601280
i32 Grabber_IsActiveGrabber(GameObject_s *object) {
  GRABBERSYS_s *sys = WorldInfo_CurrentlyActive()->grabber_sys;
  if (sys == 0)
    return -1;
  if ((object->p54->p24->flags148 & 0x8000000) == 0)
    return -1;
  return sys->active != 0;
}
