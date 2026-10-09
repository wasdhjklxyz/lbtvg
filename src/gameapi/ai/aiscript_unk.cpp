// gameapi/ai/aiscript_unk.cpp: placed by tools/new.py; file name unproven.
// from saga gameapi/ai/aisys/aiscript.cpp

#include "../../nu2api/nucore/common.h"
#include "../../nu2api/nucore/nustring.h"
#include <stddef.h>

#include "aisys_unk.h"

// FUNCTION: LEGOBATMAN 0x006a3380
f32 AIParamToFloat(AISCRIPTPROCESS *processor, char *param) {
  char *cursor;
  u32 i;

  if (param != NULL) {

    if (processor != NULL) {
      cursor = NuStrIStr(param, "param");

      if (cursor != NULL) {
        i = NuAToI(cursor);

        if (i <= 3) {
          return processor->params[i];
        }
      } else if (processor->script != NULL) {
        for (int i = 0; i < 4; i++) {
          if (processor->script->params[i].name != NULL &&
              NuStrICmp(processor->script->params[i].name, param) == 0) {
            return processor->params[i];
          }
        }
      }
    }

    return NuAToF(param);
  }

  return 0.0f;
}
