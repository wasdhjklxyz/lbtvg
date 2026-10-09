// nu2api/nucore/nuonline.cpp: 0x006d6620..0x006d6700, after nuvideo and
// before nulanguage. Names from the Mac order (bodies are saga's).

#include "common.h"

void NuOnlineInitPS(void);
int NuOnlineHasPlayerSignedInPS(int player);
int NuOnlineHasPlayerDownloadedPS(int player);
int NuOnlineAchievementAchievedPS(int achievement, void *callback);
int NuOnlineAchievementAchievedExPS(int player, int achievement,
                                    void *callback);

// FUNCTION: LEGOBATMAN 0x006d6620
void NuOnlineInit(void) { NuOnlineInitPS(); }

// FUNCTION: LEGOBATMAN 0x006d6630
void NuOnlineSignInPlayer(int player) {}

// FUNCTION: LEGOBATMAN 0x006d6640
int NuOnlineHasPlayerSignedIn(int player) {
  return NuOnlineHasPlayerSignedInPS(player);
}

// FUNCTION: LEGOBATMAN 0x006d6650
int NuOnlineHasPlayerSignedInEx(int player) { return 0; }

// FUNCTION: LEGOBATMAN 0x006d6660
int NuOnlineHasPlayerDownloaded(int player) {
  return NuOnlineHasPlayerDownloadedPS(player);
}

// FUNCTION: LEGOBATMAN 0x006d6670
int NuOnlineAchievementAchieved(int achievement, void *callback) {
  return NuOnlineAchievementAchievedPS(achievement, callback);
}

// FUNCTION: LEGOBATMAN 0x006d6680
int NuOnlineAchievementAchievedEx(int player, int achievement, void *callback) {
  return NuOnlineAchievementAchievedExPS(player, achievement, callback);
}

// FUNCTION: LEGOBATMAN 0x006d6690
void NuOnlineSetPresenceMode(int mode) {}

// FUNCTION: LEGOBATMAN 0x006d66a0
void NuOnlineSetPresenceModeEx(int player, int mode) {}

// FUNCTION: LEGOBATMAN 0x006d66b0
void NuOnlineSetDefaultPresenceMode(int mode) {}

// FUNCTION: LEGOBATMAN 0x006d66c0
void NuOnlineSetContext(int context, int value) {}

// FUNCTION: LEGOBATMAN 0x006d66d0
void NuOnlineSetContextEx(int player, int context, int value) {}

// FUNCTION: LEGOBATMAN 0x006d66e0
void NuOnlineSetDefaultContext(int context, int value) {}

// FUNCTION: LEGOBATMAN 0x006d66f0
void NuOnlineSetProperty(int property, void *value) {}

// FUNCTION: LEGOBATMAN 0x006d6700
void NuOnlineSetPropertyEx(int player, int property, void *value) {}
