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
