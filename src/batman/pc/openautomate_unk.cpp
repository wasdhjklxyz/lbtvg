// batman/pc/, file unknown: NVIDIA's OpenAutomate client glue (oa*),
// linked between d3dCalls.cpp and nusound.cpp, in the Mac's order. Every
// entry point checks the init flag and forwards through the function table
// the OA runtime filled in. C++ (the Mac's static Cleanup is mangled), with
// the oa* API extern "C".

#include "../../nu2api/numath/nuinline_unk.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x00532960
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

typedef int oaBool;
typedef double oaFloat;
typedef char oaChar;
typedef int oaOptionDataType;
typedef int oaSignalType;
union oaValue;
struct oaCommand;

struct oaNamedOption {
  int pad0[8];
  int comparison_op; // 0x20, OA_COMP_OP_INVALID (-1) after init
  int pad24[5];
};

struct oaMessage {
  int w0;
  int w4;
};

// Table order as the Mac source lists the entry points; SendSignal last.
struct oaiFunctionTable {
  int table_size;                                        // 0x00
  int (*GetNextCommand)(struct oaCommand *command);      // 0x04
  const struct oaNamedOption *(*GetNextOption)();        // 0x08
  void (*AddOption)(const struct oaNamedOption *option); // 0x0c
  void (*AddOptionValue)(const oaChar *name, oaOptionDataType type,
                         const union oaValue *value); // 0x10
  void (*AddBenchmark)(const oaChar *name);           // 0x14
  void (*AddResultValue)(const oaChar *name, oaOptionDataType type,
                         const union oaValue *value); // 0x18
  void (*StartBenchmark)();                           // 0x1c
  void (*DisplayFrame)(oaFloat t);                    // 0x20
  void (*EndBenchmark)();                             // 0x24
  void (*AddFrameValue)(const oaChar *name, oaOptionDataType type,
                        const union oaValue *value); // 0x28
  oaBool (*SendSignal)(oaSignalType signal, struct oaMessage *message,
                       void *params); // 0x2c
};

// GLOBAL: LEGOBATMAN 0x009d6d68
extern int g_oaInitialized;
// GLOBAL: LEGOBATMAN 0x009d6d14
extern oaiFunctionTable g_oaFuncTable;

struct oaVersion {
  int major;
  int minor;
  int custom;
  int build;
};

// GLOBAL: LEGOBATMAN 0x009d6d44
extern oaVersion g_oaPluginVersion;
// GLOBAL: LEGOBATMAN 0x009d6d54
extern oaVersion g_oaVersion;
// GLOBAL: LEGOBATMAN 0x009d6d64
extern oaiFunctionTable *(*g_oaPluginInit)(const oaChar *init_str,
                                           oaVersion *plugin_version,
                                           oaVersion version);
// GLOBAL: LEGOBATMAN 0x009d6d6c
extern char *g_oaPluginPath;
// GLOBAL: LEGOBATMAN 0x009d6d70
extern char *g_oaUnk009d6d70;
// GLOBAL: LEGOBATMAN 0x009d6d74
extern HMODULE g_oaModule;

static void OaError(const char *msg) { fprintf(stderr, "ERROR: %s\n", msg); }

// FUNCTION: LEGOBATMAN 0x00533490
extern "C" const struct oaNamedOption *oaGetNextOption() {
  if (!g_oaInitialized) {
    OaError("OA not initialized.");
    return 0;
  }
  if (g_oaFuncTable.GetNextOption)
    return g_oaFuncTable.GetNextOption();
  return 0;
}

// FUNCTION: LEGOBATMAN 0x005334d0
extern "C" void oaInitOption(struct oaNamedOption *option) {
  memset(option, 0, sizeof(*option));
  option->comparison_op = -1;
}

// FUNCTION: LEGOBATMAN 0x005334f0
extern "C" void oaAddOption(const struct oaNamedOption *option) {
  if (!g_oaInitialized) {
    OaError("OA not initialized.");
    return;
  }
  if (g_oaFuncTable.AddOption)
    g_oaFuncTable.AddOption(option);
}

// FUNCTION: LEGOBATMAN 0x00533520
extern "C" void oaAddOptionValue(const oaChar *name, oaOptionDataType type,
                                 const union oaValue *value) {
  if (!g_oaInitialized) {
    OaError("OA not initialized.");
    return;
  }
  if (g_oaFuncTable.AddOptionValue)
    g_oaFuncTable.AddOptionValue(name, type, value);
}

// FUNCTION: LEGOBATMAN 0x00533550
extern "C" void oaAddBenchmark(const oaChar *name) {
  if (!g_oaInitialized) {
    OaError("OA not initialized.");
    return;
  }
  if (g_oaFuncTable.AddBenchmark)
    g_oaFuncTable.AddBenchmark(name);
}

// FUNCTION: LEGOBATMAN 0x00533580
extern "C" void oaAddResultValue(const oaChar *name, oaOptionDataType type,
                                 const union oaValue *value) {
  if (!g_oaInitialized) {
    OaError("OA not initialized.");
    return;
  }
  if (g_oaFuncTable.AddResultValue)
    g_oaFuncTable.AddResultValue(name, type, value);
}

// FUNCTION: LEGOBATMAN 0x005335b0
extern "C" oaBool oaSendSignal(oaSignalType signal, struct oaMessage *message,
                               void *params) {
  if (!g_oaInitialized) {
    OaError("OA not initialized.");
    return 0;
  }
  if (g_oaFuncTable.SendSignal)
    return g_oaFuncTable.SendSignal(signal, message, params);
  return 0;
}

// FUNCTION: LEGOBATMAN 0x005335f0
extern "C" void oaInitMessage(struct oaMessage *message) {
  memset(message, 0, sizeof(*message));
}

// FUNCTION: LEGOBATMAN 0x00533600
extern "C" void oaStartBenchmark() {
  if (!g_oaInitialized) {
    OaError("OA not initialized.");
    return;
  }
  if (g_oaFuncTable.StartBenchmark)
    g_oaFuncTable.StartBenchmark();
}

// FUNCTION: LEGOBATMAN 0x00533630
extern "C" void oaDisplayFrame(oaFloat t) {
  if (!g_oaInitialized) {
    OaError("OA not initialized.");
    return;
  }
  if (g_oaFuncTable.DisplayFrame)
    g_oaFuncTable.DisplayFrame(t);
}

// FUNCTION: LEGOBATMAN 0x00533670
extern "C" void oaEndBenchmark() {
  if (!g_oaInitialized) {
    OaError("OA not initialized.");
    return;
  }
  if (g_oaFuncTable.EndBenchmark)
    g_oaFuncTable.EndBenchmark();
}

// FUNCTION: LEGOBATMAN 0x005336a0
static void Cleanup() {
  if (g_oaPluginPath) {
    free(g_oaPluginPath);
    g_oaPluginPath = NULL;
  }
  if (g_oaUnk009d6d70) {
    free(g_oaUnk009d6d70);
    g_oaUnk009d6d70 = NULL;
  }
  g_oaInitialized = 0;
  g_oaPluginInit = 0;
  memset(&g_oaFuncTable, 0, sizeof(g_oaFuncTable));
  if (g_oaModule) {
    FreeLibrary(g_oaModule);
    g_oaModule = NULL;
  }
}

// FUNCTION: LEGOBATMAN 0x005339b0
extern "C" int oaGetNextCommand(struct oaCommand *command) {
  if (!g_oaInitialized) {
    OaError("OA not initialized.");
    return 0;
  }
  int type = g_oaFuncTable.GetNextCommand(command);
  if (type == 0)
    Cleanup();
  *(int *)command = type;
  return type;
}

// Replaces every find in str with repl, into a new malloc'd string. The
// search restarts repl's length past each hit, and strcpy_s gets the whole
// buffer size: both as shipped.
// STUB: LEGOBATMAN 0x00533710
// register convention: orig takes str in eax; ours compiles it cdecl
static char *ReplaceAllUnk00533710(const char *str, const char *find,
                                   const char *repl) {
  size_t str_len = strlen(str);
  size_t find_len = strlen(find);
  size_t repl_len = strlen(repl);
  int count = 0;
  const char *p;
  for (p = strstr(str, find); p != NULL; p = strstr(p + repl_len, find))
    count++;
  size_t size = (repl_len - find_len) * count + str_len + 1;
  char *buf = (char *)malloc(size);
  char *out = buf;
  for (p = strstr(str, find); p != NULL; p = strstr(str, find)) {
    size_t n = p - str;
    memcpy(out, str, n);
    out += n;
    str = p + find_len;
    memcpy(out, repl, repl_len);
    out += repl_len;
  }
  strcpy_s(out, size, str);
  return buf;
}

// FUNCTION: LEGOBATMAN 0x00533810
static int LoadPluginUnk00533810() {
  wchar_t path[0x800];
  if (!MultiByteToWideChar(CP_UTF8, 0, g_oaPluginPath,
                           strlen(g_oaPluginPath) + 1, path, 0x800)) {
    DWORD error = GetLastError();
    if (error == ERROR_INSUFFICIENT_BUFFER)
      fprintf(stderr, "OpenAutomate MultiByteToWideChar returned error: "
                      "ERROR_INSUFFICIENT_BUFFER\n");
    else if (error == ERROR_INVALID_FLAGS)
      fprintf(stderr, "OpenAutomate MultiByteToWideChar returned error: "
                      "ERROR_INVALID_FLAGS\n");
    else if (error == ERROR_INVALID_PARAMETER)
      fprintf(stderr, "OpenAutomate MultiByteToWideChar returned error: "
                      "ERROR_INVALID_PARAMETER\n");
    else if (error == ERROR_NO_UNICODE_TRANSLATION)
      fprintf(stderr, "OpenAutomate MultiByteToWideChar returned error: "
                      "ERROR_NO_UNICODE_TRANSLATION\n");
    if (strlen(g_oaPluginPath) == 0)
      fprintf(stderr, "OpenAutomate PluginPath was undefined\n");
    return 0;
  }
  g_oaModule = LoadLibraryW(path);
  if (!g_oaModule) {
    fprintf(stderr, "OpenAutomate Failed loading: '%s'\n", g_oaPluginPath);
    return 0;
  }
  g_oaPluginInit =
      (oaiFunctionTable * (*)(const oaChar *, oaVersion *, oaVersion))
          GetProcAddress(g_oaModule, "oaPluginInit");
  if (!g_oaPluginInit) {
    OaError("Plugin does not have the correct entry point.");
    return 0;
  }
  return 1;
}

// Splits "path;init" (with %20 decoded) into the two buffers. The decoded
// copy leaks.
// STUB: LEGOBATMAN 0x00533a00
// register convention: orig takes init_str in eax and path in ebx (!);
// ours compiles it cdecl
static int ParseInitStringUnk00533a00(const char *init_str, char *path,
                                      char *rest) {
  char *s = ReplaceAllUnk00533710(init_str, "%20", " ");
  size_t i;
  for (i = 0; s[i] != 0 && s[i] != ';'; i++)
    ;
  strncpy(path, s, i);
  path[i] = 0;
  if (s[i] == ';') {
    strcpy(rest, s + i + 1);
    return 1;
  }
  rest[0] = 0;
  return 1;
}

// STUB: LEGOBATMAN 0x00533a80
// register convention: orig keeps the path buffer in ebx for the
// ParseInitString call; the version copy-out schedules differently too
extern "C" oaBool oaInit(const oaChar *init_str, oaVersion *version) {
  if (g_oaInitialized) {
    OaError("oaInit() called more than once.");
    Cleanup();
    return 0;
  }
  size_t len = strlen(init_str) + 1;
  g_oaPluginPath = (char *)malloc(len);
  g_oaUnk009d6d70 = (char *)malloc(len);
  if (!ParseInitStringUnk00533a00(init_str, g_oaPluginPath, g_oaUnk009d6d70)) {
    OaError("Couldn't parse initialization string.");
    Cleanup();
    return 0;
  }
  g_oaPluginInit = NULL;
  memset(&g_oaFuncTable, 0, sizeof(g_oaFuncTable));
  if (!LoadPluginUnk00533810()) {
    OaError("Couldn't load the plugin.");
    Cleanup();
    return 0;
  }
  g_oaVersion.major = 0;
  g_oaVersion.minor = 5;
  g_oaVersion.custom = 0;
  g_oaVersion.build = 7;
  oaiFunctionTable *table =
      g_oaPluginInit(g_oaUnk009d6d70, &g_oaPluginVersion, g_oaVersion);
  if (g_oaPluginVersion.major != 0) {
    OaError("Plugin version mismatch.");
    Cleanup();
    return 0;
  }
  if (!table || !table->GetNextCommand) {
    OaError("Plugin is misconfigured.");
    Cleanup();
    return 0;
  }
  memcpy(&g_oaFuncTable, table,
         table->table_size > sizeof(g_oaFuncTable) ? sizeof(g_oaFuncTable)
                                                   : table->table_size);
  *version = g_oaVersion;
  g_oaInitialized = 1;
  return 1;
}

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_openautomate_unk(f32 *v, f32 a, i32 i) {
  NuVec4Set(v, a, a, a, a);
}
