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
