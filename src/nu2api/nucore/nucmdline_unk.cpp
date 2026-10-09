// nu2api/nucore/nucmdline_unk.cpp: between gcutscn.cpp (0x006ceef0) and
// nupad_gen.cpp (0x006d5700). The two globals are adjacent; saga keeps nuapi
// state in one struct (ref/saga/src/nu2api/nucore/nuapi.h), so model it so.

struct nucmdline_s {
  int argc;
  char **argv;
};

// GLOBAL: LEGOBATMAN 0x00b038e0
nucmdline_s nucmdline;

// Original loads both values before either store; neither plain globals nor
// this struct reproduce that ordering.
// STUB: LEGOBATMAN 0x006d5540
void NuCommandLine(int *argc, char ***argv) {
  nucmdline.argc = *argc;
  nucmdline.argv = *argv;
}

// Mac order after NuTimeScanlines: NuLanguageGet, NuFrameBegin,
// NuFrameSetMinDelay, PrintStartupString, NuPadRecordEndFrame,
// NuRegisterEndFrameCallBackFn, NuTerminateHardware, NuCommandLine,
// NuDisableOSMenuFreeze, NuMultiThreadRender, NuAPI{Get,Set}CurrentGScene,
// NuPadUseCorrectDeadZoning.

// GLOBAL: LEGOBATMAN 0x0099f298
extern int nuapi_min_delay;
// GLOBAL: LEGOBATMAN 0x00b038d0
void (*nuapi_endframe_callbackfn)(void);
// nuapi.disable_os_menu_freeze
// GLOBAL: LEGOBATMAN 0x00adf6dc
int g_nuapiDisableOSMenuFreeze;
// GLOBAL: LEGOBATMAN 0x00adf6f4
int g_nuapiMultiThreadRender;
// GLOBAL: LEGOBATMAN 0x00adf710
void *g_nuapiCurrentGScene;
// GLOBAL: LEGOBATMAN 0x00b038c9
char UseCorrectDeadZoning;

// FUNCTION: LEGOBATMAN 0x006d5420
void NuFrameSetMinDelay(int delay) { nuapi_min_delay = delay; }

// FUNCTION: LEGOBATMAN 0x006d5520
void NuRegisterEndFrameCallBackFn(void (*callback)(void)) {
  nuapi_endframe_callbackfn = callback;
}

// FUNCTION: LEGOBATMAN 0x006d5530
void NuTerminateHardware(void) {}

// FUNCTION: LEGOBATMAN 0x006d5560
void NuDisableOSMenuFreeze(void) { g_nuapiDisableOSMenuFreeze = 1; }

// FUNCTION: LEGOBATMAN 0x006d5570
void NuMultiThreadRender(int state) { g_nuapiMultiThreadRender = state; }

// FUNCTION: LEGOBATMAN 0x006d5580
void *NuAPIGetCurrentGScene(void) { return g_nuapiCurrentGScene; }

// FUNCTION: LEGOBATMAN 0x006d5590
void NuAPISetCurrentGScene(void *gsc) { g_nuapiCurrentGScene = gsc; }

// FUNCTION: LEGOBATMAN 0x006d55a0
void NuPadUseCorrectDeadZoning(int state) { UseCorrectDeadZoning = state; }
