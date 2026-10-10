// nu2api/nucore/nucmdline_unk.cpp: between gcutscn.cpp (0x006ceef0) and
// nupad_gen.cpp (0x006d5700). The two globals are adjacent; saga keeps nuapi
// state in one struct (ref/saga/src/nu2api/nucore/nuapi.h), so model it so.

#include "nuapi_unk.h"
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
// GLOBAL: LEGOBATMAN 0x00b038c9
char UseCorrectDeadZoning;

// --- NuAPIInit: nuapi (saga NUAPI) is 0xf4 bytes at 0x00adf648; its fields
// are separate globals in this tree.
// GLOBAL: LEGOBATMAN 0x009d5844
extern int nuapi_forced_frame_time;
// GLOBAL: LEGOBATMAN 0x0099f294
extern int nuapi_padrec_buffersize;

// The whole struct, for the memset (its fields are declared above).

// STUB: LEGOBATMAN 0x006d5210
// Our cl sinks the swap_mode=5 / multi_thread_render=1 stores below the
// zero stores; the original keeps source order (any statement order).
void NuAPIInit(void) {
  memset(&nuapi, 0, sizeof(nuapi));
  nuapi.fps = 60.0f;
  nuapi.wind[0] = 0.0f;
  nuapi.wind[1] = 0.0f;
  nuapi.wind[2] = 0.0f;
  nuapi.wind[3] = 0.0f;
  nuapi.video_mode = 2;
  nuapi.video_swap_mode = 5;
  nuapi.language = 2;
  nuapi.unk04 = 0;
  nuapi.unkf0 = 1.0f;
  nuapi.video_aspect = 0;
  nuapi.disable_os_menu_freeze = 0;
  nuapi.unk50 = nuapi_forced_frame_time;
  nuapi.multi_thread_render = 1;
  nuapi.unkb4 = 0;
  nuapi.pad_record.end_record_buttons = 0;
  nuapi.pad_record.end_play_buttons = 0;
  nuapi.unk84 = 0;
  nuapi.current_gscene = 0;
  nuapi.pad_record.buf_size = nuapi_padrec_buffersize;
  nuapi.unka8 = 4;
  nuapi.unka4 = 8;
  memset(nuapi.unke0, 0, sizeof(nuapi.unke0));
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

// FUNCTION: LEGOBATMAN 0x006d52f0
void NuFrameBegin(void) {
  nuapi.nuframe_begin_cnt++;
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
void NuDisableOSMenuFreeze(void) { nuapi.disable_os_menu_freeze = 1; }

// FUNCTION: LEGOBATMAN 0x006d5570
void NuMultiThreadRender(int state) { nuapi.multi_thread_render = state; }

// FUNCTION: LEGOBATMAN 0x006d5580
void *NuAPIGetCurrentGScene(void) { return nuapi.current_gscene; }

// FUNCTION: LEGOBATMAN 0x006d5590
void NuAPISetCurrentGScene(void *gsc) { nuapi.current_gscene = gsc; }

// FUNCTION: LEGOBATMAN 0x006d55a0
void NuPadUseCorrectDeadZoning(int state) { UseCorrectDeadZoning = state; }

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
        nuapi.pad_record.filepath = *argv;
        nuapi.pad_record.mode = 1;
      } else if (NuStrICmp(*argv, "PADPLAY") == 0) {
        argv++;
        if (--argc == 0)
          Unk006d24b0();
        nuapi.pad_record.filepath = *argv;
        nuapi.pad_record.mode = 2;
      }
      argc--;
      argv++;
    } while (argc > 0);
    nucmdline.argv = argv;
    nucmdline.argc = argc;
  }
}
