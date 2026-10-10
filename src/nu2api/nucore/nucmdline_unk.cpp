// nu2api/nucore/nucmdline_unk.cpp: between gcutscn.cpp (0x006ceef0) and
// nupad_gen.cpp (0x006d5700). The two globals are adjacent; saga keeps nuapi
// state in one struct (ref/saga/src/nu2api/nucore/nuapi.h), so model it so.

#include <string.h>

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

// --- NuAPIInit: nuapi (saga NUAPI) is 0xf4 bytes at 0x00adf648; its fields
// are separate globals in this tree.
// GLOBAL: LEGOBATMAN 0x00adf648
extern int g_nuPadUnk0adf648; // language
// GLOBAL: LEGOBATMAN 0x00adf64c
extern int g_nuPadUnk0adf64c;
// GLOBAL: LEGOBATMAN 0x00adf650
extern int g_nuapiVideoMode;
// GLOBAL: LEGOBATMAN 0x00adf654
extern int g_nuPadUnk0adf654; // video_swap_mode
// GLOBAL: LEGOBATMAN 0x00adf658
extern int g_nuPadUnk0adf658; // video_aspect
// GLOBAL: LEGOBATMAN 0x00adf664
extern float g_nuapiFps;
// GLOBAL: LEGOBATMAN 0x00adf698
int g_nuapiForcedFrameTime;
// GLOBAL: LEGOBATMAN 0x00adf6ac
int g_nuapiPadRecBufSize;
// GLOBAL: LEGOBATMAN 0x00adf6b8
extern int g_nuPadRecEndRecordButtons;
// GLOBAL: LEGOBATMAN 0x00adf6bc
extern int g_nuPadRecEndPlayButtons;
// GLOBAL: LEGOBATMAN 0x00adf6cc
int g_nuapiUnk0adf6cc;
// GLOBAL: LEGOBATMAN 0x00adf6ec
int g_nuapiUnk0adf6ec;
// GLOBAL: LEGOBATMAN 0x00adf6f0
int g_nuapiUnk0adf6f0;
// GLOBAL: LEGOBATMAN 0x00adf6fc
int g_nuapiUnk0adf6fc;
// GLOBAL: LEGOBATMAN 0x00adf700
float g_nuapiWind[4];
// GLOBAL: LEGOBATMAN 0x00adf728
int g_nuapiUnk0adf728[4];
// GLOBAL: LEGOBATMAN 0x00adf738
float g_nuapiUnk0adf738;
// GLOBAL: LEGOBATMAN 0x009d5844
extern int nuapi_forced_frame_time;
// GLOBAL: LEGOBATMAN 0x0099f294
extern int nuapi_padrec_buffersize;

// The whole struct, for the memset (its fields are declared above).
extern char nuapi[0xf4];

// STUB: LEGOBATMAN 0x006d5210
// Store order needs nuapi as one struct (separate globals get rescheduled).
void NuAPIInit(void) {
  memset(nuapi, 0, 0xf4);
  g_nuapiFps = 60.0f;
  g_nuapiWind[0] = 0.0f;
  g_nuapiWind[1] = 0.0f;
  g_nuapiVideoMode = 2;
  g_nuapiWind[2] = 0.0f;
  g_nuPadUnk0adf654 = 5;
  g_nuapiWind[3] = 0.0f;
  g_nuPadUnk0adf648 = 2;
  g_nuPadUnk0adf64c = 0;
  g_nuapiUnk0adf738 = 1.0f;
  g_nuPadUnk0adf658 = 0;
  g_nuapiDisableOSMenuFreeze = 0;
  g_nuapiForcedFrameTime = nuapi_forced_frame_time;
  g_nuapiMultiThreadRender = 1;
  g_nuapiUnk0adf6fc = 0;
  g_nuPadRecEndRecordButtons = 0;
  g_nuPadRecEndPlayButtons = 0;
  g_nuapiUnk0adf6cc = 0;
  g_nuapiCurrentGScene = 0;
  g_nuapiPadRecBufSize = nuapi_padrec_buffersize;
  g_nuapiUnk0adf6f0 = 4;
  g_nuapiUnk0adf6ec = 8;
  g_nuapiUnk0adf728[0] = 0;
  g_nuapiUnk0adf728[1] = 0;
  g_nuapiUnk0adf728[2] = 0;
  g_nuapiUnk0adf728[3] = 0;
}

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

// GLOBAL: LEGOBATMAN 0x00adf6b0
extern char *g_nuPadRecFilename;
// GLOBAL: LEGOBATMAN 0x00adf69c
extern int g_nuPadRecMode;
int NuStrICmp(const char *a, const char *b);
void Unk006d24b0(...);

// Scans the command line for PADRECORD/PADPLAY <file> (name unknown). The
// original reloads *argv after NuStrICmp, so it did not see that function's
// body: it is not in nupad_gen.cpp's TU.
// FUNCTION: LEGOBATMAN 0x006dd5d0
void NuPadUnk006dd5d0(void) {
  int argc = nucmdline.argc;
  char **argv;

  if (argc > 0) {
    argv = nucmdline.argv;
    do {
      if (*argv == 0)
        break;
      if (NuStrICmp(*argv, "PADRECORD") == 0) {
        argv++;
        if (--argc == 0)
          Unk006d24b0();
        g_nuPadRecFilename = *argv;
        g_nuPadRecMode = 1;
      } else if (NuStrICmp(*argv, "PADPLAY") == 0) {
        argv++;
        if (--argc == 0)
          Unk006d24b0();
        g_nuPadRecFilename = *argv;
        g_nuPadRecMode = 2;
      }
      argc--;
      argv++;
    } while (argc > 0);
    nucmdline.argv = argv;
    nucmdline.argc = argc;
  }
}
