// gameapi/ai/aistate_unk.cpp: placed by tools/new.py; file name unproven.
// from saga gameapi/ai/aisys/aistate.cpp

#include "../../nu2api/nucore/common.h"
#include "../../nu2api/nucore/nustring.h"
#include "aisys_unk.h"
#include <stddef.h>

// FUNCTION: LEGOBATMAN 0x006a1950
AISTATE *AIStateFind(char *name, AISCRIPT *script) {
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

// FUNCTION: LEGOBATMAN 0x006a62b0
i32 AIScriptSetInterrupt(AISCRIPTPROCESS *processor, u8 priority, u8 id,
                         char *state_name, f32 time) {
  AISTATE *state;

  if (priority >= processor->interrupt_priority) {
    state = AIStateFind(state_name, processor->script);

    if (state != NULL) {
      processor->interrupt_priority = priority;
      processor->interrupt_id = id;
      processor->interrupt_state = state;
      processor->interrupt_timer = time;

      return 1;
    }
  }

  return 0;
}
