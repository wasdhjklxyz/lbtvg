// batman/pcsettingsmanager_unk.cpp: PCSettingsManager (Mac symbols);
// between pcbatman.cpp and pcapi.cpp by link order, file name unproven.

#include "../nu2api/nucore/common.h"
#include "../nu2api/numath/nuinline_unk.h"

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x005240e0
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

typedef struct nufpar_s NUFPAR;

// 0x006dd150; the legacy keys point straight at it. Name by Mac order
// (NuFParGetInt, NuFParGetOptionalInt, NuFParGetIntRDP).
i32 NuFParGetIntRDP(NUFPAR *parser);

// Settings the xRead* keyword parsers fill. The base is inferred: FileVersion
// is read first and the rest follow from +0x40c.
struct PCSettingsManager {
  i32 file_version; // 0x0
  u8 pad4[0x40c - 0x4];
  i32 screen_width;        // 0x40c
  i32 screen_height;       // 0x410
  i32 window_width;        // 0x414
  i32 window_height;       // 0x418
  i32 window_left;         // 0x41c
  i32 window_top;          // 0x420
  i32 screen_refresh_rate; // 0x424
  bool vertical_sync;      // 0x428
  u8 pad429[0x42c - 0x429];
  i32 fsaa;                      // 0x42c
  i32 widescreen;                // 0x430
  i32 texture_quality;           // 0x434
  bool force_multithreaded_d3_d; // 0x438
  bool ignore_vendor_presets;    // 0x439
  u8 pad43a[0x440 - 0x43a];
  i32 sfxvolume;      // 0x440
  i32 music_volume;   // 0x444
  i32 master_volume;  // 0x448
  bool music_enabled; // 0x44c
  u8 pad44d[0x450 - 0x44d];
  i32 mouse_sensitivity; // 0x450
  bool invert_y;         // 0x454
  bool vibration;        // 0x455
  u8 pad456[0x457 - 0x456];
  bool bloom_enabled; // 0x457
  u8 pad458[0x45a - 0x458];
  bool dof_enabled; // 0x45a
  u8 pad45b[0x460 - 0x45b];
  i32 desired_shadow_method; // 0x460
  bool shadows_enabled;      // 0x464
  u8 pad465[0x46c - 0x465];
  i32 desired_dynamic_light_quality; // 0x46c
  u8 pad470[0x472 - 0x470];
  bool speed_blur_enabled; // 0x472
  u8 pad473[0x474 - 0x473];
  bool process_colour_enabled; // 0x474
  u8 pad475[0x476 - 0x475];
  bool edge_aaenabled; // 0x476
  u8 pad477[0x478 - 0x477];
  bool ssaoenabled; // 0x478
  u8 pad479[0x47a - 0x479];
  bool allow_vendor_extensions; // 0x47a
  bool use_hires;               // 0x47b
  bool use_hires_pending;       // 0x47c
  u8 pad47d[0x480 - 0x47d];
  i32 force_shader_model; // 0x480

  static void xReadFileVersion(NUFPAR *parser);
  static void xReadScreenWidth(NUFPAR *parser);
  static void xReadScreenHeight(NUFPAR *parser);
  static void xReadWindowWidth(NUFPAR *parser);
  static void xReadWindowHeight(NUFPAR *parser);
  static void xReadWindowLeft(NUFPAR *parser);
  static void xReadWindowTop(NUFPAR *parser);
  static void xReadScreenRefreshRate(NUFPAR *parser);
  static void xReadVerticalSync(NUFPAR *parser);
  static void xReadFSAA(NUFPAR *parser);
  static void xReadWidescreen(NUFPAR *parser);
  static void xReadTextureQuality(NUFPAR *parser);
  static void xReadForceMultithreadedD3D(NUFPAR *parser);
  static void xReadIgnoreVendorPresets(NUFPAR *parser);
  static void xReadSFXVolume(NUFPAR *parser);
  static void xReadMusicVolume(NUFPAR *parser);
  static void xReadMasterVolume(NUFPAR *parser);
  static void xReadMusicEnabled(NUFPAR *parser);
  static void xReadMouseSensitivity(NUFPAR *parser);
  static void xReadInvertY(NUFPAR *parser);
  static void xReadVibration(NUFPAR *parser);
  static void xReadBloomEnabled(NUFPAR *parser);
  static void xReadDofEnabled(NUFPAR *parser);
  static void xReadDesiredShadowMethod(NUFPAR *parser);
  static void xReadShadowsEnabled(NUFPAR *parser);
  static void xReadDesiredDynamicLightQuality(NUFPAR *parser);
  static void xReadSpeedBlurEnabled(NUFPAR *parser);
  static void xReadProcessColourEnabled(NUFPAR *parser);
  static void xReadEdgeAAEnabled(NUFPAR *parser);
  static void xReadSSAOEnabled(NUFPAR *parser);
  static void xReadAllowVendorExtensions(NUFPAR *parser);
  static void xReadUseHires(NUFPAR *parser);
  static void xReadUseHiresPending(NUFPAR *parser);
  static void xReadForceShaderModel(NUFPAR *parser);
};

// GLOBAL: LEGOBATMAN 0x0094bc40
extern PCSettingsManager g_PCSettings;

class InputRemapClass {
public:
  i32 MakeCtrlStringEx(int pad, int ctrl, char *out, int a4, int a5, int a6,
                       int a7, char *brackets);
};

// GLOBAL: LEGOBATMAN 0x009cfaf4
extern InputRemapClass *g_inputRemap;
// GLOBAL: LEGOBATMAN 0x009d0300
extern char g_pcInputName[];

int NuPadUnk006d5df0(int i);

// FUNCTION: LEGOBATMAN 0x005235c0
extern "C" char *PcInput_GetInputName(int pad, int ctrl, int a3, int a4) {
  g_inputRemap->MakeCtrlStringEx(pad, ctrl, g_pcInputName, a3, a4, 1, 1, "()");
  return g_pcInputName;
}

// FUNCTION: LEGOBATMAN 0x00523600
extern "C" i32 PcInput_GetCtrlInputString(int pad, int ctrl, char *out) {
  InputRemapClass *remap = g_inputRemap;
  if (pad < 0)
    pad = 0;
  pad = NuPadUnk006d5df0(pad);
  if (ctrl >= 0)
    return remap->MakeCtrlStringEx(pad, ctrl, out, 0, 0, 1, 1, "()");
  *out = 0;
  return 0;
}

// FUNCTION: LEGOBATMAN 0x00524720
void PCSettingsManager::xReadFileVersion(NUFPAR *parser) {
  g_PCSettings.file_version = NuFParGetIntRDP(parser);
}

// FUNCTION: LEGOBATMAN 0x00524740
void PCSettingsManager::xReadScreenWidth(NUFPAR *parser) {
  g_PCSettings.screen_width = NuFParGetIntRDP(parser);
}

// FUNCTION: LEGOBATMAN 0x00524760
void PCSettingsManager::xReadScreenHeight(NUFPAR *parser) {
  g_PCSettings.screen_height = NuFParGetIntRDP(parser);
}

// FUNCTION: LEGOBATMAN 0x00524780
void PCSettingsManager::xReadWindowWidth(NUFPAR *parser) {
  g_PCSettings.window_width = NuFParGetIntRDP(parser);
}

// FUNCTION: LEGOBATMAN 0x005247a0
void PCSettingsManager::xReadWindowHeight(NUFPAR *parser) {
  g_PCSettings.window_height = NuFParGetIntRDP(parser);
}

// FUNCTION: LEGOBATMAN 0x005247c0
void PCSettingsManager::xReadWindowLeft(NUFPAR *parser) {
  g_PCSettings.window_left = NuFParGetIntRDP(parser);
}

// FUNCTION: LEGOBATMAN 0x005247e0
void PCSettingsManager::xReadWindowTop(NUFPAR *parser) {
  g_PCSettings.window_top = NuFParGetIntRDP(parser);
}

// FUNCTION: LEGOBATMAN 0x00524800
void PCSettingsManager::xReadScreenRefreshRate(NUFPAR *parser) {
  g_PCSettings.screen_refresh_rate = NuFParGetIntRDP(parser);
}

// FUNCTION: LEGOBATMAN 0x00524820
void PCSettingsManager::xReadVerticalSync(NUFPAR *parser) {
  g_PCSettings.vertical_sync = NuFParGetIntRDP(parser) > 0;
}

// FUNCTION: LEGOBATMAN 0x00524840
void PCSettingsManager::xReadFSAA(NUFPAR *parser) {
  g_PCSettings.fsaa = NuFParGetIntRDP(parser);
}

// FUNCTION: LEGOBATMAN 0x00524860
void PCSettingsManager::xReadWidescreen(NUFPAR *parser) {
  g_PCSettings.widescreen = NuFParGetIntRDP(parser);
}

// FUNCTION: LEGOBATMAN 0x00524880
void PCSettingsManager::xReadTextureQuality(NUFPAR *parser) {
  g_PCSettings.texture_quality = NuFParGetIntRDP(parser);
}

// FUNCTION: LEGOBATMAN 0x005248a0
void PCSettingsManager::xReadForceMultithreadedD3D(NUFPAR *parser) {
  g_PCSettings.force_multithreaded_d3_d = NuFParGetIntRDP(parser) > 0;
}

// FUNCTION: LEGOBATMAN 0x005248c0
void PCSettingsManager::xReadIgnoreVendorPresets(NUFPAR *parser) {
  g_PCSettings.ignore_vendor_presets = NuFParGetIntRDP(parser) > 0;
}

// FUNCTION: LEGOBATMAN 0x005248e0
void PCSettingsManager::xReadSFXVolume(NUFPAR *parser) {
  g_PCSettings.sfxvolume = NuFParGetIntRDP(parser);
}

// FUNCTION: LEGOBATMAN 0x00524900
void PCSettingsManager::xReadMusicVolume(NUFPAR *parser) {
  g_PCSettings.music_volume = NuFParGetIntRDP(parser);
}

// FUNCTION: LEGOBATMAN 0x00524920
void PCSettingsManager::xReadMasterVolume(NUFPAR *parser) {
  g_PCSettings.master_volume = NuFParGetIntRDP(parser);
}

// FUNCTION: LEGOBATMAN 0x00524940
void PCSettingsManager::xReadMusicEnabled(NUFPAR *parser) {
  g_PCSettings.music_enabled = NuFParGetIntRDP(parser) > 0;
}

// FUNCTION: LEGOBATMAN 0x00524960
void PCSettingsManager::xReadMouseSensitivity(NUFPAR *parser) {
  g_PCSettings.mouse_sensitivity = NuFParGetIntRDP(parser);
}

// FUNCTION: LEGOBATMAN 0x00524980
void PCSettingsManager::xReadInvertY(NUFPAR *parser) {
  g_PCSettings.invert_y = NuFParGetIntRDP(parser) > 0;
}

// FUNCTION: LEGOBATMAN 0x005249a0
void PCSettingsManager::xReadVibration(NUFPAR *parser) {
  g_PCSettings.vibration = NuFParGetIntRDP(parser) > 0;
}

// FUNCTION: LEGOBATMAN 0x005249c0
void PCSettingsManager::xReadBloomEnabled(NUFPAR *parser) {
  g_PCSettings.bloom_enabled = NuFParGetIntRDP(parser) > 0;
}

// FUNCTION: LEGOBATMAN 0x005249e0
void PCSettingsManager::xReadDofEnabled(NUFPAR *parser) {
  g_PCSettings.dof_enabled = NuFParGetIntRDP(parser) > 0;
}

// FUNCTION: LEGOBATMAN 0x00524a00
void PCSettingsManager::xReadDesiredShadowMethod(NUFPAR *parser) {
  g_PCSettings.desired_shadow_method = NuFParGetIntRDP(parser);
}

// FUNCTION: LEGOBATMAN 0x00524a20
void PCSettingsManager::xReadShadowsEnabled(NUFPAR *parser) {
  g_PCSettings.shadows_enabled = NuFParGetIntRDP(parser) > 0;
}

// FUNCTION: LEGOBATMAN 0x00524a40
void PCSettingsManager::xReadDesiredDynamicLightQuality(NUFPAR *parser) {
  g_PCSettings.desired_dynamic_light_quality = NuFParGetIntRDP(parser);
}

// FUNCTION: LEGOBATMAN 0x00524a60
void PCSettingsManager::xReadSpeedBlurEnabled(NUFPAR *parser) {
  g_PCSettings.speed_blur_enabled = NuFParGetIntRDP(parser) > 0;
}

// FUNCTION: LEGOBATMAN 0x00524a80
void PCSettingsManager::xReadProcessColourEnabled(NUFPAR *parser) {
  g_PCSettings.process_colour_enabled = NuFParGetIntRDP(parser) > 0;
}

// FUNCTION: LEGOBATMAN 0x00524aa0
void PCSettingsManager::xReadEdgeAAEnabled(NUFPAR *parser) {
  g_PCSettings.edge_aaenabled = NuFParGetIntRDP(parser) > 0;
}

// FUNCTION: LEGOBATMAN 0x00524ac0
void PCSettingsManager::xReadSSAOEnabled(NUFPAR *parser) {
  g_PCSettings.ssaoenabled = NuFParGetIntRDP(parser) > 0;
}

// FUNCTION: LEGOBATMAN 0x00524ae0
void PCSettingsManager::xReadAllowVendorExtensions(NUFPAR *parser) {
  g_PCSettings.allow_vendor_extensions = NuFParGetIntRDP(parser) > 0;
}

// FUNCTION: LEGOBATMAN 0x00524b00
void PCSettingsManager::xReadUseHires(NUFPAR *parser) {
  g_PCSettings.use_hires = NuFParGetIntRDP(parser) > 0;
}

// FUNCTION: LEGOBATMAN 0x00524b20
void PCSettingsManager::xReadUseHiresPending(NUFPAR *parser) {
  g_PCSettings.use_hires_pending = NuFParGetIntRDP(parser) > 0;
}

// FUNCTION: LEGOBATMAN 0x00524b40
void PCSettingsManager::xReadForceShaderModel(NUFPAR *parser) {
  g_PCSettings.force_shader_model = NuFParGetIntRDP(parser);
}

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_pcsettingsmanager_unk(f32 *v, f32 a, i32 i) {
  NuVec4Set(v, a, a, a, a);
}
