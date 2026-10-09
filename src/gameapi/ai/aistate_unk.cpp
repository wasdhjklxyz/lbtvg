// gameapi/ai/aistate_unk.cpp: placed by tools/new.py; file name unproven.
// from saga gameapi/ai/aisys/aistate.cpp

#include "../../nu2api/nucore/common.h"
#include "../../nu2api/nucore/nulist.h"
#include "../../nu2api/nucore/nustring.h"
#include <stddef.h>

typedef struct AISCRIPTPARAMS_s {
  char *name;
  f32 default_val;
} AISCRIPTPARAMS;

typedef struct AISTATE_s {
  NULISTLNK list_node;
  NULISTHDR conditions;
  NULISTHDR actions;
  char *name;
  NULISTHDR ref_scripts;
} AISTATE;

typedef struct AISCRIPT_s {
  NULISTLNK list_node;
  char *name;         // 0x8
  char *derived_from; // ??
  u32 unkown;         // ?? NOTE: Could be before or after derived_from
  NULISTHDR states;   // 0x14
  AISCRIPTPARAMS params[4];
  AISTATE *base_state;
  u32 is_level_script : 1;
  u32 is_derived : 1;
  u32 is_derived_from_level_script : 1;
  NULISTHDR ref_scripts;
  NULISTHDR condition_macros;
  NULISTHDR action_macros;
} AISCRIPT;

// NOTE: related to LEGOBATMAN 0x006a62b0 it calls this and thats _AIStateFind
// in mac idk??
// FUNCTION: LEGOBATMAN 0x006a1950
AISTATE *AIStateFindUnk006a1950(char *name, AISCRIPT *script) {
  AISTATE *state;

  state = (AISTATE *)NuListGetHead(&script->states);

  if (name != NULL) {
    while (state != NULL) {
      if (NuStrICmp(name, state->name) == 0) {
        return state;
      }

      state = (AISTATE *)NuListGetNext(&script->states, &state->list_node);
    }
  }

  return NULL;
}
