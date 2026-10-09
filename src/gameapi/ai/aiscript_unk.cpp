// gameapi/ai/aiscript_unk.cpp: placed by tools/new.py; file name unproven.
// from saga gameapi/ai/aisys/aiscript.cpp

#include "../../nu2api/nucore/common.h"
#include "../../nu2api/nucore/nustring.h"
#include <stddef.h>
#include <string.h>

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

typedef struct AICONDITION_s {
  NULISTLNK list_node; // 0x00
  u32 pad8[3];
  char *arg; // 0x14
  u32 pad18[2];
  char *next_state_name; // 0x20
  u32 pad24[2];
} AICONDITION;

extern "C" void NuListAppendUnk006d40f0(NULISTHDR *list, NULISTLNK *node);
void NuMemCpy(unsigned char *dst, unsigned char *src, int n);

// A lone `ret` in the shipped build (debug logging compiled out), but called
// as an opaque function: callers reload values around it.
void AIScriptDebugUnk006a10a0(void);

static void *AIScriptBufferAlloc(VARIPTR *buf, VARIPTR *buf_end, u32 size) {
  void *ret = 0;
  if (buf != 0 && buf_end != 0 && buf->addr + size < buf_end->addr) {
    ret = (void *)((buf->addr + 15) & ~15);
    buf->addr = ((buf->addr + 15) & ~15) + size;
    memset(ret, 0, size);
  } else {
    AIScriptDebugUnk006a10a0();
  }
  return ret;
}

static char *AIScriptCopyString(char *str, VARIPTR *buf, VARIPTR *buf_end) {
  char *dst = 0;
  if (str != 0) {
    u32 len = NuStrLen(str);
    if (len != 0) {
      dst = (char *)AIScriptBufferAlloc(buf, buf_end, len + 1);
      NuStrCpy(dst, str);
    }
  }
  return dst;
}

// STUB: LEGOBATMAN 0x006a2730
// close: orig pushes ebx and loads buf before the NuListGetHead call; ours
// does it after the empty-list check. Rest lines up.
void AIScriptCopyConditions(NULISTHDR *src, NULISTHDR *dst, VARIPTR *buf,
                            VARIPTR *buf_end) {
  AICONDITION *src_cond;
  for (src_cond = (AICONDITION *)NuListGetHead(src); src_cond != 0;
       src_cond = (AICONDITION *)NuListGetNext(src, &src_cond->list_node)) {
    AICONDITION *dst_cond =
        (AICONDITION *)AIScriptBufferAlloc(buf, buf_end, sizeof(AICONDITION));
    if (dst_cond != 0) {
      memset(dst_cond, 0, sizeof(AICONDITION));
      NuMemCpy((unsigned char *)dst_cond, (unsigned char *)src_cond,
               sizeof(AICONDITION));
      memset(&dst_cond->list_node, 0, sizeof(NULISTLNK));
      dst_cond->arg = AIScriptCopyString(src_cond->arg, buf, buf_end);
      dst_cond->next_state_name =
          AIScriptCopyString(src_cond->next_state_name, buf, buf_end);
      NuListAppendUnk006d40f0(dst, &dst_cond->list_node);
    }
  }
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

// Generic AI script conditions (saga aisys.cpp), reached through the
// condition keyword table.

// FUNCTION: LEGOBATMAN 0x006a3540
f32 Condition_Timer(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                    AIPACKET_s *packet, char *str, void *argument) {
  return process->script_timer;
}

f32 NuRandFloat(void);

// FUNCTION: LEGOBATMAN 0x006a3550
f32 Condition_Random(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                     AIPACKET_s *packet, char *str, void *argument) {
  return NuRandFloat();
}

// FUNCTION: LEGOBATMAN 0x006a3560
void *Condition_AlwaysTrueInit(AISYS_s *sys, char *arg, AISCRIPT_s *script) {
  f32 value = 1.0f;
  if (arg != NULL && NuStrLen(arg) != 0)
    value = NuAToF(arg);
  return *(void **)&value;
}

// FUNCTION: LEGOBATMAN 0x006a35a0
f32 Condition_AlwaysTrue(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                         AIPACKET_s *packet, char *str, void *argument) {
  f32 value;
  *(void **)&value = argument;
  return value;
}

// FUNCTION: LEGOBATMAN 0x006a3690
f32 Condition_GotLocator(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                         AIPACKET_s *packet, char *str, void *argument) {
  // 0xa8: the process's current locator.
  if (*(void **)((u8 *)process + 0xa8) != NULL)
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x006a36b0
f32 Condition_GotLocatorSet(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                            AIPACKET_s *packet, char *str, void *argument) {
  // 0xac: the process's current locator set.
  if (*(void **)((u8 *)process + 0xac) != NULL)
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x006a36d0
f32 Condition_OnPath(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                     AIPACKET_s *packet, char *str, void *argument) {
  // 0x166 bit 0: on path.
  if (packet != NULL && (((u8 *)packet)[0x166] & 1))
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x006a36f0
f32 Condition_PlayerOnPath(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                           AIPACKET_s *packet, char *str, void *argument) {
  // 0x1698: player 1; its +4 is its AI packet.
  u8 *player_1 = *(u8 **)((u8 *)sys + 0x1698);
  if (player_1 != NULL && ((*(u8 **)(player_1 + 4))[0x166] & 1))
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x006a3710
f32 Condition_OpponentOnPath(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                             AIPACKET_s *packet, char *str, void *argument) {
  if (packet != NULL && packet->pe4 != NULL) {
    u8 *ai = (u8 *)packet->pe4->ai;
    if (ai != NULL && (ai[0x166] & 1))
      return 1.0f;
  }
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x006a3740
f32 Condition_TimeOffPath(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                          AIPACKET_s *packet, char *str, void *argument) {
  if (packet != NULL)
    return *(f32 *)((u8 *)packet + 0x210);
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x006a3760
f32 Condition_CurrentLocatorIs(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                               AIPACKET_s *packet, char *str, void *argument) {
  if (argument != NULL && *(void **)((u8 *)process + 0xa8) == argument)
    return 1.0f;
  return 0.0f;
}
