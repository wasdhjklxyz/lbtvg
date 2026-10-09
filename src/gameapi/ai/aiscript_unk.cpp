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

struct APICHARSYS_s {
  u8 pad0[8];
  u16 model_id_capacity; // 0x08
};

// GLOBAL: LEGOBATMAN 0x00a94740
extern APICHARSYS_s *apicharsys;

struct OverrideAnimPacket_s {
  u8 pad0[0x126];
  i16 animation_override_from; // 0x126
  i16 animation_override_to;   // 0x128
};

struct OverrideAnimOwner_s {
  u8 pad0[0x54];
  void *character_data; // 0x54
};

i32 FindAnimIX(void *character_data, char *name);

// from saga gameapi/ai/aisys/aisys.cpp
// FUNCTION: LEGOBATMAN 0x006a5660
i32 Action_OverrideAnimation(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                             AIPACKET_s *packet, char **args, int argc,
                             int flags, f32 time) {
  i16 from = -1;
  i16 to = -1;
  if (packet == NULL || packet->pd0 == NULL || packet->pd0->obj == NULL ||
      flags == 0)
    return 1;

  for (i32 index = 0; index < argc; ++index) {
    if (NuStrICmp(args[index], "from=All") == 0) {
      from = (i16)apicharsys->model_id_capacity;
      continue;
    }
    char *value = NuStrIStr(args[index], "from=");
    if (value != NULL) {
      from = (i16)FindAnimIX(
          ((OverrideAnimOwner_s *)packet->pd0)->character_data, value + 5);
      continue;
    }
    value = NuStrIStr(args[index], "to=");
    if (value != NULL) {
      to = (i16)FindAnimIX(((OverrideAnimOwner_s *)packet->pd0)->character_data,
                           value + 3);
      continue;
    }
    if (process != NULL)
      *(f32 *)((u8 *)process + 0xa0) =
          AIParamToFloatEx(packet, process, args[0]);
  }

  if (to == -1)
    from = -1;
  ((OverrideAnimPacket_s *)packet)->animation_override_from = from;
  ((OverrideAnimPacket_s *)packet)->animation_override_to = to;
  return 1;
}
