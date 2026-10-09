// nu2api/nucore/pc/nuonline_pc.cpp: the PC layer under nuonline.cpp,
// 0x006e26a0..0x006e26f0. Names from the Mac order.

#include "../common.h"

// FUNCTION: LEGOBATMAN 0x006e26a0
int NuOnlineAchievementAchievedPS(int achievement, void *callback) { return 1; }

// FUNCTION: LEGOBATMAN 0x006e26b0
int NuOnlineAchievementAchievedExPS(int player, int achievement,
                                    void *callback) {
  return 1;
}

// FUNCTION: LEGOBATMAN 0x006e26c0
void NuOnlineInitPS(void) {}

// FUNCTION: LEGOBATMAN 0x006e26d0
int NuOnlineSignInPlayerPS(int player) { return 0; }

// FUNCTION: LEGOBATMAN 0x006e26e0
int NuOnlineHasPlayerSignedInPS(int player) { return 0; }

// FUNCTION: LEGOBATMAN 0x006e26f0
int NuOnlineHasPlayerDownloadedPS(int player) { return 0; }
