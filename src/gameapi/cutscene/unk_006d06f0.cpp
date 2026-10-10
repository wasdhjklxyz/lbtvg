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

// 0x34-byte timed events.
struct CutsceneEvent_s {
  char *name; // 0x00
  i32 type;   // 0x04, 0 = always, 1 = timed
  u8 pad8[0xc - 8];
  u8 flags; // 0x0c, 4 = resettable, 8 = not on the first update
  u8 pad0d[0x2e - 0xd];
  i16 item; // 0x2e, index into the owner's item table, -1 = none
  u8 pad30[0x34 - 0x30];
};

struct CutsceneEvents_s {
  u8 pad0[8];
  i32 count;               // 0x08
  CutsceneEvent_s *events; // 0x0c
};

// 0x50-byte items.
struct CutsceneItem_s {
  u8 pad0[0x44];
  u8 flags; // 0x44, bit 0 = active
  u8 pad45[0x48 - 0x45];
  void *data; // 0x48
  u8 pad4c[0x50 - 0x4c];
};

struct CutsceneItems_s {
  u8 pad0[0x20];
  CutsceneItem_s *items; // 0x20
};

struct CutsceneEventOwner_s {
  CutsceneEventOwner_s *next; // 0x00
  u8 pad4[8 - 4];
  CutsceneEvents_s *events;  // 0x08
  u8 (*states)[4];           // 0x0c, [2] 0xff = live, [3] bit 0 = fired
  i32 i10;                   // 0x10, CutsceneItems_s *
  CutsceneNamedNode_s *list; // 0x14
  u8 pad18[0x38 - 0x18];
  u8 flags; // 0x38, bit 0 = paused
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

i32 Unk006ce030(CutsceneEvent_s *event, void *item, i32 a, f32 t);
i32 Unk006ce380(CutsceneEvent_s *event, i32 a, f32 t);

// GLOBAL: LEGOBATMAN 0x00ad74d4
extern CutsceneEventOwner_s *g_unk00ad74d4;

// STUB: LEGOBATMAN 0x006d0850
// Close: the original copies the first argument into an extra stack slot
// (frame 0x34, reloaded into edx every event); ours reads the argument slot.
void Unk006d0850(i32 a, f32 t, i32 first) {
  for (CutsceneEventOwner_s *owner = g_unk00ad74d4; owner != NULL;
       owner = owner->next) {
    if (owner->flags & 1)
      continue;
    CutsceneEvents_s *events = owner->events;
    CutsceneItems_s *items = (CutsceneItems_s *)owner->i10;
    for (i32 i = 0; i < events->count; i++) {
      CutsceneEvent_s *event = &events->events[i];
      u8 *state = owner->states[i];
      i32 fire = 0;
      if (state[2] == 0xff && (!(state[3] & 1) || (event->flags & 4))) {
        switch (event->type) {
        case 0:
          fire = 1;
          break;
        case 1:
          if ((event->flags & 8) && first == 0 && !(state[3] & 1))
            break;
          if (event->item >= 0) {
            CutsceneItem_s *item = &items->items[event->item];
            if (!(item->flags & 1))
              break;
            if (item->data != NULL)
              fire = Unk006ce030(event, item->data, a, t);
            else
              fire = Unk006ce030(event, item, a, t);
          } else {
            fire = Unk006ce380(event, a, t);
          }
          break;
        }
      }
      if (fire) {
        if (!(state[3] & 1))
          Unk006d07b0(owner, &event->name, state, 1);
      } else if ((state[3] & 1) && (event->flags & 4)) {
        Unk006d07b0(owner, &event->name, state, 0);
      }
    }
  }
}
