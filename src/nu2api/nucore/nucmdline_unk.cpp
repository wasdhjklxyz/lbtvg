// nu2api/nucore/nucmdline_unk.cpp: between gcutscn.cpp (0x006ceef0) and
// nupad_gen.cpp (0x006d5700). The two globals are adjacent; saga keeps nuapi
// state in one struct (ref/saga/src/nu2api/nucore/nuapi.h), so model it so.

#include <stdarg.h>
#include <stdio.h>
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

// --- NuFrameBegin (Mac body: NuPortalInit, NuWaterReset, NuMouseRead,
// NuKeyboardRead, NuOcclusionManagerBeginFrame, NuRndrFrameLights::Reset).
void NuPortalInit(void);
void NuTexUnk006e6ba0(void); // NuWaterReset
void NuMouseRead(void);
void NuKeyboardRead(void);
void NuOcclusionManagerBeginFrame(void);
class NuShadowLightManagerUnk {
public:
  void ResetUnk0070a470();
};
// GLOBAL: LEGOBATMAN 0x029e6380
extern NuShadowLightManagerUnk nuspecial_shadow_manager;
// nuapi frame counter
// GLOBAL: LEGOBATMAN 0x00adf668
int g_nuapiFrameCount;

// FUNCTION: LEGOBATMAN 0x006d52f0
void NuFrameBegin(void) {
  g_nuapiFrameCount++;
  NuPortalInit();
  NuTexUnk006e6ba0();
  NuMouseRead();
  NuKeyboardRead();
  NuOcclusionManagerBeginFrame();
  nuspecial_shadow_manager.ResetUnk0070a470();
}

// FUNCTION: LEGOBATMAN 0x006d5420
void NuFrameSetMinDelay(int delay) { nuapi_min_delay = delay; }

// --- PrintStartupString: one line of the startup log on screen ------------
void Unk007101d0(int mode);
void Unk00710390(int mode);
void Unk00710900(void *font);
void Unk007109b0(void *font, int colour);
void Unk00710930(void *font, float sx, float sy);
void Unk00737a50(void *font, int x, int y, int size, char *text);
void Unk00710210(void);
void Unk007103c0(void);
// GLOBAL: LEGOBATMAN 0x029f3f30
extern void *g_unk029f3f30;
// GLOBAL: LEGOBATMAN 0x00afa238
int g_nuStartupStringY;

// FUNCTION: LEGOBATMAN 0x006d5430
void PrintStartupString(int colour, char *fmt, ...) {
  char buf[256];
  va_list args;

  va_start(args, fmt);
  vsprintf(buf, fmt, args);
  Unk007101d0(2);
  Unk00710390(1);
  Unk00710900(g_unk029f3f30);
  Unk007109b0(g_unk029f3f30, colour);
  Unk00710930(g_unk029f3f30, 0.7f, 0.7f);
  Unk00737a50(g_unk029f3f30, 0x140, g_nuStartupStringY * 8, 0x10, buf);
  Unk00710210();
  Unk007103c0();
  g_nuStartupStringY += 0x14;
}

char *Unk00527ef0(void);

// Prints a pending message on the startup screen (name unknown).
// FUNCTION: LEGOBATMAN 0x006d5500
void NuUnk006d5500(void) {
  if (Unk00527ef0() != 0)
    PrintStartupString(-1, Unk00527ef0());
}

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
