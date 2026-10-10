// gameapi/cutscene/unk_006d06f0.cpp: TU of unknown name after gcutscn.cpp
// (cutscene event dispatch by name; "Fn" names go to a global table).

#include "../../nu2api/nucore/common.h"
#include <stddef.h>
#include <stdio.h>

int NuStrICmp(const char *a, const char *b);
int NuStrNICmp(const char *a, const char *b, int n);

// Empty debug prints. The linker folds every empty function into 0x006c8d40
// (identical COMDAT folding), so one address hides several functions: an
// external one-argument hook (call kept) and this TU's own varargs print
// (known empty here, so the caller keeps edx live across the call).
void Unk006c8d40(char *name);
void CutsceneDebugPrintf(char *fmt, ...) {}
void Unk006d0330(void *data, i32 a, i32 b);

struct CutsceneFnEntry_s {
  char *name;          // 0x0
  void (*fn)(i32 arg); // 0x4
};

struct CutsceneNamedNode_s {
  CutsceneNamedNode_s *next; // 0x0
  char *name;                // 0x4
  void *data;                // 0x8
};

// GLOBAL: LEGOBATMAN 0x00ad75dc
extern CutsceneFnEntry_s *g_unk00ad75dc;

// FUNCTION: LEGOBATMAN 0x006d06f0
i32 Unk006d06f0(CutsceneNamedNode_s *list, char *name, i32 a, i32 b) {
  CutsceneFnEntry_s *entry = g_unk00ad75dc;
  if (NuStrNICmp(name, "Fn", 2) == 0) {
    for (; entry != NULL && entry->name != NULL; entry++) {
      if (NuStrICmp(name + 2, entry->name) == 0) {
        Unk006c8d40(name);
        if (entry->fn != NULL)
          entry->fn(0);
        return 0;
      }
    }
    return 0;
  }
  for (CutsceneNamedNode_s *node = list; node != NULL; node = node->next) {
    if (NuStrICmp(node->name, name) == 0) {
      Unk006c8d40(name);
      Unk006d0330(node->data, a, b);
      return 0;
    }
  }
  Unk006c8d40(name);
  return 0;
}

struct CutsceneEventOwner_s {
  u8 pad0[0x10];
  i32 i10;                   // 0x10
  CutsceneNamedNode_s *list; // 0x14
};

// FUNCTION: LEGOBATMAN 0x006d07b0
void Unk006d07b0(CutsceneEventOwner_s *owner, char **name, u8 *flags, i32 on) {
  char buf[32];
  if (on) {
    CutsceneDebugPrintf(*name, *name);
    flags[3] |= 1;
    Unk006d06f0(owner->list, *name, owner->i10, (i32)owner);
  } else {
    sprintf(buf, "reset_%s", *name);
    CutsceneDebugPrintf(*name, buf);
    flags[3] &= ~1;
    Unk006d06f0(owner->list, buf, owner->i10, (i32)owner);
  }
}
