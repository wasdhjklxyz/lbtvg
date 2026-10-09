// batman/pc/, file unknown: NVIDIA's OpenAutomate client glue (oa*),
// linked between d3dCalls.cpp and nusound.cpp, in the Mac's order. Every
// entry point checks the init flag and forwards through the function table
// the OA runtime filled in. C++ (the Mac's static Cleanup is mangled), with
// the oa* API extern "C".

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

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

// GLOBAL: LEGOBATMAN 0x009d6d64
extern int g_oaUnk009d6d64;
// GLOBAL: LEGOBATMAN 0x009d6d6c
extern char *g_oaUnk009d6d6c;
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
  if (g_oaUnk009d6d6c) {
    free(g_oaUnk009d6d6c);
    g_oaUnk009d6d6c = NULL;
  }
  if (g_oaUnk009d6d70) {
    free(g_oaUnk009d6d70);
    g_oaUnk009d6d70 = NULL;
  }
  g_oaInitialized = 0;
  g_oaUnk009d6d64 = 0;
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
