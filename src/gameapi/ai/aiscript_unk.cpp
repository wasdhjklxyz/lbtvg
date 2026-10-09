// gameapi/ai/aiscript_unk.cpp: placed by tools/new.py; file name unproven.
// from saga gameapi/ai/aisys/aiscript.cpp

#include "../../nu2api/nucore/common.h"
#include "../../nu2api/nucore/nustring.h"
#include <stddef.h>

#include "aisys_unk.h"

struct AISYS_s {
  u8 pad0[0x228];
  NULISTHDR scripts; // 0x228
};

// GLOBAL: LEGOBATMAN 0x00ad435c
extern NULISTHDR global_aiscripts;

// FUNCTION: LEGOBATMAN 0x006a19a0
AISCRIPT *AIScriptFind(AISYS_s *sys, char *name, i32 can_use_default,
                       i32 check_level_scripts, i32 check_global_scripts) {
  AISCRIPT *script;

  if (name != NULL) {
    if (check_level_scripts && sys != NULL) {
      script = (AISCRIPT *)NuListGetHead(&sys->scripts);

      while (script != NULL) {
        if (NuStrICmp(name, script->name) == 0) {
          return script;
        }

        script = (AISCRIPT *)NuListGetNext(&sys->scripts, &script->list_node);
      }
    }

    if (check_global_scripts) {
      script = (AISCRIPT *)NuListGetHead(&global_aiscripts);

      while (script != NULL) {
        if (NuStrICmp(name, script->name) == 0) {
          return script;
        }

        script =
            (AISCRIPT *)NuListGetNext(&global_aiscripts, &script->list_node);
      }
    }
  }

  if (can_use_default) {
    script = (AISCRIPT *)NuListGetHead(&global_aiscripts);

    while (script != NULL) {
      if (NuStrICmp("default", script->name) == 0) {
        return script;
      }

      script = (AISCRIPT *)NuListGetNext(&global_aiscripts, &script->list_node);
    }
  }

  return NULL;
}

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

typedef i32 nurdpgetvarfn(char *expr, f32 *float_out, i32 *int_out);

// GLOBAL: LEGOBATMAN 0x00ad4540
extern AISCRIPTPROCESS *AiEvalExpressionProcessor;
// GLOBAL: LEGOBATMAN 0x00ad453c
extern AIPACKET_s *AiEvalExpressionPacket;

i32 AiEvalExpressionNameLoopup(char *expr, f32 *float_out, i32 *int_out);
f32 NuRDPFVar(char *input, nurdpgetvarfn *get_var_fn);

// FUNCTION: LEGOBATMAN 0x006a34d0
f32 AIParamToFloatEx(AIPACKET_s *packet, AISCRIPTPROCESS *processor,
                     char *param) {
  char *cursor;
  f32 result;
  i32 is_number;

  is_number = 1;
  cursor = param;

  do {
    if (*cursor == '\0') {
      break;
    }

    // The original accepts signed character values from -45 through '9',
    // except '/'; this is its numeric fast path, not a digit-only test.
    if (*cursor < -45 || *cursor > '9' || *cursor == '/') {
      is_number = 0;
      break;
    }

    cursor++;
  } while (true);

  if (is_number) {
    return NuAToF(param);
  }

  AiEvalExpressionProcessor = processor;
  AiEvalExpressionPacket = packet;

  result = NuRDPFVar(param, AiEvalExpressionNameLoopup);

  AiEvalExpressionProcessor = 0;

  return result;
}
