// gameapi/ai/aiscript_unk.cpp: placed by tools/new.py; file name unproven.
// from saga gameapi/ai/aisys/aiscript.cpp

#include "../../nu2api/nucore/common.h"
#include "../../nu2api/nucore/nustring.h"
#include <stddef.h>
#include <string.h>

#include "aisys_unk.h"

struct AICHARMODEL_s {
  i16 model_id;
};

// Per-creature defaults (0xa8 bytes), indexed by AIPACKET_s::origin_index.
struct AICREATURE_s {
  u8 pad0[0x20];
  nuvec_s pos; // 0x20
  u8 pad2c[0x94 - 0x2c];
  f32 view_distance;   // 0x94
  f32 hear_distance;   // 0x98
  f32 max_view_height; // 0x9c
  f32 min_view_height; // 0xa0
  u8 popa4[0xa8 - 0xa4];
};

struct AISYS_s {
  u8 pad0[0x224];
  AICREATURE_s *creatures; // 0x224
  NULISTHDR scripts;       // 0x228
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

// orig 0x006a1a90 takes buf in edi and inlines AIScriptBufferAlloc.
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

typedef struct nufpar_s {
  u8 pad0[0x910];
  char *word_buf; // 0x910
} NUFPAR;

i32 NuFParGetWord(NUFPAR *parser);
f32 NuFParGetFloat(NUFPAR *parser);
void NuStrNCpy(char *dst, const char *src, i32 n);
void AIDebugUnk006a10a0(...);

struct AISCRIPTCONST_s {
  char name[0x20]; // 0x00
  f32 default_val; // 0x20
};

// GLOBAL: LEGOBATMAN 0x00ad43c0
extern AISCRIPTCONST_s aiscript_const[];
// GLOBAL: LEGOBATMAN 0x00ad4520
extern i32 aiscript_const_curr;

i32 NuFParGetInt(NUFPAR *parser);

// GLOBAL: LEGOBATMAN 0x00ad44f0
extern VARIPTR *load_buff;
// GLOBAL: LEGOBATMAN 0x00ad44f4
extern VARIPTR *load_endbuff;
// GLOBAL: LEGOBATMAN 0x00ad44f8
extern AISCRIPT *load_aiscript;

// keyword "PARAM" in table 0x0099dce0
// STUB: LEGOBATMAN 0x006a2460
// close: only the AIScriptCopyString register convention differs (orig buf
// in edi, ours str); needs CopyString matched first.
void xParam(NUFPAR *parser) {
  if (load_aiscript == NULL)
    return;
  u32 param_idx = NuFParGetInt(parser);
  if (param_idx > 3)
    return;
  if (NuFParGetWord(parser) == 0)
    return;
  load_aiscript->params[param_idx].name =
      AIScriptCopyString(parser->word_buf, load_buff, load_endbuff);
  load_aiscript->params[param_idx].default_val = NuFParGetFloat(parser);
}

// keyword "CONST" in table 0x0099dce0
// FUNCTION: LEGOBATMAN 0x006a24d0
void xConst(NUFPAR *parser) {
  if (NuStrLen(parser->word_buf) >= 31)
    AIDebugUnk006a10a0(parser->word_buf);
  NuFParGetWord(parser);
  NuStrNCpy(aiscript_const[aiscript_const_curr].name, parser->word_buf, 0x20);
  aiscript_const[aiscript_const_curr].default_val = NuFParGetFloat(parser);
  aiscript_const_curr++;
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
AISTATE *AIStateFind(char *name, AISCRIPT *script);

// GLOBAL: LEGOBATMAN 0x00ad4524
extern i32 g_unk00ad4524;
// Lone `ret`s in the shipped build (debug reports compiled out).
void AIDebugUnk006a10a0(...);
void AIDebugUnk006a10b0(...);

// FUNCTION: LEGOBATMAN 0x006a4470
i32 Action_SetState(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                    AIPACKET_s *packet, char **args, int argc, int flags,
                    f32 time) {
  if (flags != 0 && argc != 0) {
    process->next_state = AIStateFind(args[0], process->script);
    process->unknown_flag_4 = 0;
    for (i32 i = 1; i < argc; i++) {
      if (NuStrICmp(args[i], "KeepBlockedMessages") == 0)
        process->unknown_flag_4 = 1;
    }
    if (process->next_state == NULL) {
      if (g_unk00ad4524 == 1)
        AIDebugUnk006a10b0(args[0], process->script->name,
                           process->state->name);
      else
        AIDebugUnk006a10a0(args[0], process->script->name,
                           process->state->name);
    }
  }
  return 0;
}

// GLOBAL: LEGOBATMAN 0x00ad6930
extern f32 (*GetViewRangeFn)(i32 model_id);
// GLOBAL: LEGOBATMAN 0x00ad6934
extern f32 (*GetHearDistanceFn)(i32 model_id);
// GLOBAL: LEGOBATMAN 0x00ad6938
extern f32 (*GetMaxViewHeightFn)(i32 model_id);
// GLOBAL: LEGOBATMAN 0x00ad693c
extern f32 (*GetMinViewHeightFn)(i32 model_id);

// FUNCTION: LEGOBATMAN 0x006a52b0
i32 Action_SetViewDistance(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                           AIPACKET_s *packet, char **args, int argc, int flags,
                           f32 time) {
  if (packet == NULL || packet->pd0 == NULL || flags == 0)
    return 1;
  Unk_AIPacketObj *object = packet->pd0;
  if (packet->origin_index != 0xff)
    object->viewdistance = sys->creatures[packet->origin_index].view_distance;
  else if (GetViewRangeFn != NULL)
    packet->pd0->viewdistance =
        GetViewRangeFn(object->character_model->model_id);
  else
    object->viewdistance = 1.0f;
  if (argc != 0 && NuStrICmp(args[0], "default") != 0)
    packet->pd0->viewdistance = AIParamToFloatEx(packet, process, args[0]);
  return 1;
}

// FUNCTION: LEGOBATMAN 0x006a5380
i32 Action_SetMinViewHeight(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                            AIPACKET_s *packet, char **args, int argc,
                            int flags, f32 time) {
  if (packet == NULL || packet->pd0 == NULL || flags == 0)
    return 1;
  Unk_AIPacketObj *object = packet->pd0;
  if (packet->origin_index != 0xff)
    object->minviewheight =
        sys->creatures[packet->origin_index].min_view_height;
  else if (GetMinViewHeightFn != NULL)
    packet->pd0->minviewheight =
        GetMinViewHeightFn(object->character_model->model_id);
  else
    object->minviewheight = 1.0f;
  if (argc != 0 && NuStrICmp(args[0], "default") != 0)
    packet->pd0->minviewheight = AIParamToFloatEx(packet, process, args[0]);
  return 1;
}

// FUNCTION: LEGOBATMAN 0x006a5450
i32 Action_SetMaxViewHeight(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                            AIPACKET_s *packet, char **args, int argc,
                            int flags, f32 time) {
  if (packet == NULL || packet->pd0 == NULL || flags == 0)
    return 1;
  Unk_AIPacketObj *object = packet->pd0;
  if (packet->origin_index != 0xff)
    object->maxviewheight =
        sys->creatures[packet->origin_index].max_view_height;
  else if (GetMaxViewHeightFn != NULL)
    packet->pd0->maxviewheight =
        GetMaxViewHeightFn(object->character_model->model_id);
  else
    object->maxviewheight = 1.0f;
  if (argc != 0 && NuStrICmp(args[0], "default") != 0)
    packet->pd0->maxviewheight = AIParamToFloatEx(packet, process, args[0]);
  return 1;
}

// FUNCTION: LEGOBATMAN 0x006a5520
i32 Action_SetHearDistance(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                           AIPACKET_s *packet, char **args, int argc, int flags,
                           f32 time) {
  if (packet == NULL || packet->pd0 == NULL || flags == 0)
    return 1;
  Unk_AIPacketObj *object = packet->pd0;
  if (packet->origin_index != 0xff)
    object->heardistance = sys->creatures[packet->origin_index].hear_distance;
  else if (GetHearDistanceFn != NULL)
    packet->pd0->heardistance =
        GetHearDistanceFn(object->character_model->model_id);
  else
    object->heardistance = 1.0f;
  if (argc != 0 && NuStrICmp(args[0], "default") != 0)
    packet->pd0->heardistance = AIParamToFloatEx(packet, process, args[0]);
  return 1;
}

// GLOBAL: LEGOBATMAN 0x00ad6960
extern nuvec_s *(*GetAICreatureOriginFn)(AISYS_s *sys, AIPACKET_s *packet);

// FUNCTION: LEGOBATMAN 0x006a57a0
i32 Action_ResetToOrigin(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                         AIPACKET_s *packet, char **args, int argc, int flags,
                         f32 time) {
  if (packet == NULL || sys == NULL)
    return 1;
  nuvec_s *(*fn)(AISYS_s *, AIPACKET_s *) = GetAICreatureOriginFn;
  nuvec_s *origin = fn != NULL ? fn(sys, packet) : NULL;
  if (origin != NULL) {
    packet->pd0->pos5c = *origin;
  } else if (packet->pd0 != NULL && (packet->pd0->flags1f8 & 0x400) &&
             packet->origin_index != 0xff) {
    packet->pd0->pos5c = sys->creatures[packet->origin_index].pos;
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x006a5b00
i32 Action_SetReturnToState(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                            AIPACKET_s *packet, char **args, int argc,
                            int flags, f32 time) {
  AISTATE *state = NULL;
  if (flags == 0 || process == NULL)
    return 1;
  if (process->state != NULL)
    state = process->state;
  for (i32 i = 0; i < argc; i++) {
    char *value = NuStrIStr(args[i], "state");
    if (value != NULL) {
      state = AIStateFind(value + 6, process->script);
      if (state == NULL) {
        if (g_unk00ad4524 == 0)
          AIDebugUnk006a10a0(args[0], process->script->name,
                             process->state->name);
        else
          AIDebugUnk006a10b0(args[0], process->script->name,
                             process->state->name);
      }
    }
  }
  process->return_to_state = state;
  return 1;
}

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

// FUNCTION: LEGOBATMAN 0x006a3ba0
f32 Condition_GotTriggerArea(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                             AIPACKET_s *packet, char *str, void *argument) {
  // 0xa4: the process's current trigger area.
  if (*(void **)((u8 *)process + 0xa4) != NULL)
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x006a3d30
f32 Condition_BaddyInTriggerArea(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                 AIPACKET_s *packet, char *str,
                                 void *argument) {
  u8 *area = (u8 *)argument;
  if (area == NULL)
    area = *(u8 **)((u8 *)process + 0xa4);
  // 0x2a: area runtime flags.
  if (area != NULL && (area[0x2a] & 4))
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x006a3d60
f32 Condition_GoodyInTriggerArea(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                 AIPACKET_s *packet, char *str,
                                 void *argument) {
  u8 *area = (u8 *)argument;
  if (area == NULL)
    area = *(u8 **)((u8 *)process + 0xa4);
  if (area != NULL && (area[0x2a] & 2))
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x006a3e00
f32 Condition_GotOpponent(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                          AIPACKET_s *packet, char *str, void *argument) {
  if (packet != NULL && packet->pe4 != NULL)
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x006a4390
f32 Condition_PathBlocked(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                          AIPACKET_s *packet, char *str, void *argument) {
  // 0x1f0 bit 22: path blocked.
  if (packet != NULL && (*(u32 *)((u8 *)packet + 0x1f0) & 0x400000))
    return 1.0f;
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x006a43b0
f32 Condition_InterruptID(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                          AIPACKET_s *packet, char *str, void *argument) {
  // 0xb9: interrupt id.
  if (process != NULL)
    return ((u8 *)process)[0xb9];
  return -1.0f;
}

// FUNCTION: LEGOBATMAN 0x006a43d0
void *Condition_IAmInit(AISYS_s *sys, char *arg, AISCRIPT_s *script) {
  return arg != NULL && GetNamedAPIObjectFn != NULL
             ? GetNamedAPIObjectFn(sys, arg)
             : NULL;
}

// FUNCTION: LEGOBATMAN 0x006a43f0
f32 Condition_IAm(AISYS_s *sys, AISCRIPTPROCESS_s *process, AIPACKET_s *packet,
                  char *str, void *argument) {
  if (packet != NULL && packet->pd0 != NULL) {
    if (packet->pd0 == argument)
      return 1.0f;
    Unk_GameObject54 *character = packet->pd0->character;
    if (character != NULL && character->file != NULL &&
        NuStrICmp(character->file, str) == 0)
      return 1.0f;
  }
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x006a4430
f32 Condition_StuckTime(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                        AIPACKET_s *packet, char *str, void *argument) {
  if (packet != NULL && packet->pd0 != NULL)
    return *(f32 *)((u8 *)packet->pd0 + 0x1d8);
  return 0.0f;
}

// FUNCTION: LEGOBATMAN 0x006a4450
f32 Condition_Param(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                    AIPACKET_s *packet, char *str, void *argument) {
  return AIParamToFloatEx(packet, process, str);
}

// FUNCTION: LEGOBATMAN 0x006a4d50
i32 Action_IgnoreWallSplines(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                             AIPACKET_s *packet, char **args, int argc,
                             int flags, f32 time) {
  if (packet == NULL || flags == 0)
    return 1;
  *(u32 *)((u8 *)packet + 0x1f0) |= 0x80;
  for (i32 i = 0; i < argc; i++) {
    if (NuStrICmp(args[i], "false") == 0)
      *(u32 *)((u8 *)packet + 0x1f0) &= ~0x80;
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x006a4e40
i32 Action_NoTerrain(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                     AIPACKET_s *packet, char **args, int argc, int flags,
                     f32 time) {
  if (packet == NULL || packet->pd0 == NULL || flags == 0)
    return 1;
  *(u32 *)((u8 *)packet->pd0 + 0x1fc) |= 0x20;
  for (i32 i = 0; i < argc; i++) {
    if (NuStrICmp(args[i], "false") == 0)
      *(u32 *)((u8 *)packet->pd0 + 0x1fc) &= ~0x20;
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x006a4ec0
i32 Action_NoLosCheck(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                      AIPACKET_s *packet, char **args, int argc, int flags,
                      f32 time) {
  if (packet == NULL || packet->pd0 == NULL || flags == 0)
    return 1;
  *(u32 *)((u8 *)packet->pd0 + 0x1fc) |= 0x400;
  for (i32 i = 0; i < argc; i++) {
    if (NuStrICmp(args[i], "false") == 0)
      *(u32 *)((u8 *)packet->pd0 + 0x1fc) &= ~0x400;
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x006a4f40
i32 Action_FlatTerrain(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                       AIPACKET_s *packet, char **args, int argc, int flags,
                       f32 time) {
  if (packet == NULL || packet->pd0 == NULL || flags == 0)
    return 1;
  *(u32 *)((u8 *)packet->pd0 + 0x1fc) |= 0x10;
  for (i32 i = 0; i < argc; i++) {
    if (NuStrICmp(args[i], "false") == 0)
      *(u32 *)((u8 *)packet->pd0 + 0x1fc) &= ~0x10;
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x006a4fc0
i32 Action_ShadowTerrain(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                         AIPACKET_s *packet, char **args, int argc, int flags,
                         f32 time) {
  if (packet == NULL || packet->pd0 == NULL || flags == 0)
    return 1;
  *(u32 *)((u8 *)packet->pd0 + 0x1fc) |= 0x8;
  for (i32 i = 0; i < argc; i++) {
    if (NuStrICmp(args[i], "false") == 0)
      *(u32 *)((u8 *)packet->pd0 + 0x1fc) &= ~0x8;
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x006a5040
i32 Action_DontUseShadowTerrain(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                                AIPACKET_s *packet, char **args, int argc,
                                int flags, f32 time) {
  if (packet == NULL || packet->pd0 == NULL || flags == 0)
    return 1;
  *(u32 *)((u8 *)packet->pd0 + 0x1fc) |= 0x40;
  for (i32 i = 0; i < argc; i++) {
    if (NuStrICmp(args[i], "false") == 0)
      *(u32 *)((u8 *)packet->pd0 + 0x1fc) &= ~0x40;
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x006a50c0
i32 Action_DontPush(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                    AIPACKET_s *packet, char **args, int argc, int flags,
                    f32 time) {
  if (packet == NULL || packet->pd0 == NULL || flags == 0)
    return 1;
  *(u32 *)((u8 *)packet->pd0 + 0x1fc) |= 0x2;
  for (i32 i = 0; i < argc; i++) {
    if (NuStrICmp(args[i], "false") == 0)
      *(u32 *)((u8 *)packet->pd0 + 0x1fc) &= ~0x2;
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x006a5140
i32 Action_CanSeeBehind(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                        AIPACKET_s *packet, char **args, int argc, int flags,
                        f32 time) {
  if (packet == NULL || packet->pd0 == NULL || flags == 0)
    return 1;
  *(u32 *)((u8 *)packet->pd0 + 0x1fc) |= 0x800;
  for (i32 i = 0; i < argc; i++) {
    if (NuStrICmp(args[i], "false") == 0)
      *(u32 *)((u8 *)packet->pd0 + 0x1fc) &= ~0x800;
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x006a51c0
i32 Action_RequiresLOS(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                       AIPACKET_s *packet, char **args, int argc, int flags,
                       f32 time) {
  if (packet == NULL || flags == 0)
    return 1;
  *(u32 *)((u8 *)packet + 0x1f0) |= 0x20000;
  for (i32 i = 0; i < argc; i++) {
    if (NuStrICmp(args[i], "false") == 0)
      *(u32 *)((u8 *)packet + 0x1f0) &= ~0x20000;
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x006a4dc0
i32 Action_CheckWallSplines(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                            AIPACKET_s *packet, char **args, int argc,
                            int flags, f32 time) {
  if (packet == NULL || flags == 0)
    return 1;
  *(u32 *)((u8 *)packet + 0x1f0) |= 0x100;
  for (i32 i = 0; i < argc; i++) {
    if (NuStrICmp(args[i], "false") == 0)
      *(u32 *)((u8 *)packet + 0x1f0) &= ~0x100;
  }
  if (*(u32 *)((u8 *)packet + 0x1f0) & 0x100)
    *(u32 *)((u8 *)packet + 0x1f0) |= 0x80;
  return 1;
}

// FUNCTION: LEGOBATMAN 0x006a5230
i32 Action_SetFullPathSearch(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                             AIPACKET_s *packet, char **args, int argc,
                             int flags, f32 time) {
  if (packet == NULL || packet->pd0 == NULL || flags == 0)
    return 1;
  *(u32 *)((u8 *)packet->pd0 + 0x1fc) &= ~0x80000;
  for (i32 i = 0; i < argc; i++) {
    if (NuStrICmp(args[i], "false") == 0)
      *(u32 *)((u8 *)packet->pd0 + 0x1fc) |= 0x80000;
  }
  return 1;
}

// 0x1fc bit 17 of the packet object.
struct AIObjFlags1fc_s {
  u32 pad : 17;
  u32 ignore_antinodes : 1;
};

// STUB: LEGOBATMAN 0x006a5820
// close: object and args swap edi/ebx (decl order, ternary tried).
i32 Action_SetIgnoreAntinodes(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                              AIPACKET_s *packet, char **args, int argc,
                              int flags, f32 time) {
  i32 ignore = 1;
  Unk_AIPacketObj *object = NULL;
  if (flags == 0)
    return ignore;
  if (packet != NULL && packet->pd0 != NULL)
    object = packet->pd0;
  for (i32 i = 0; i < argc; i++) {
    char *value = NuStrIStr(args[i], "character");
    if (value != NULL) {
      if (GetNamedAPIObjectFn != NULL)
        object = GetNamedAPIObjectFn(sys, value + 10);
    } else if (NuStrICmp("FALSE", args[0]) == 0) {
      ignore = 0;
    }
  }
  if (object != NULL)
    ((AIObjFlags1fc_s *)((u8 *)object + 0x1fc))->ignore_antinodes = ignore;
  return 1;
}

// FUNCTION: LEGOBATMAN 0x006a58d0
i32 Action_NoShadows(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                     AIPACKET_s *packet, char **args, int argc, int flags,
                     f32 time) {
  if (packet == NULL || flags == 0 || packet->pd0 == NULL)
    return 1;
  *(u32 *)((u8 *)packet->pd0 + 0x1f8) |= 0x2000;
  for (i32 i = 0; i < argc; i++) {
    if (NuStrICmp(args[i], "false") == 0)
      *(u32 *)((u8 *)packet->pd0 + 0x1f8) &= ~0x2000;
  }
  return 1;
}

// FUNCTION: LEGOBATMAN 0x006a5bb0
i32 Action_ReturnToState(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                         AIPACKET_s *packet, char **args, int argc, int flags,
                         f32 time) {
  // 0xc4: the state to return to.
  AISTATE **return_to_state = (AISTATE **)((u8 *)process + 0xc4);
  if (process != NULL && *return_to_state != NULL) {
    process->next_state = *return_to_state;
    *return_to_state = NULL;
    return 0;
  }
  return 1;
}

// 0xb5 bits 0-1: if/else state of the process.
struct AIIfState_s {
  u8 state : 2;
};

// FUNCTION: LEGOBATMAN 0x006a5be0
i32 Action_Else(AISYS_s *sys, AISCRIPTPROCESS_s *process, AIPACKET_s *packet,
                char **args, int argc, int flags, f32 time) {
  AIIfState_s *if_state = (AIIfState_s *)((u8 *)process + 0xb5);
  if (if_state->state != 0 && if_state->state != 2) {
    if_state->state = 0;
    return 1;
  }
  if_state->state = 2;
  return 1;
}

// FUNCTION: LEGOBATMAN 0x006a5c20
i32 Action_EndIf(AISYS_s *sys, AISCRIPTPROCESS_s *process, AIPACKET_s *packet,
                 char **args, int argc, int flags, f32 time) {
  ((AIIfState_s *)((u8 *)process + 0xb5))->state = 0;
  return 1;
}

// FUNCTION: LEGOBATMAN 0x006a55f0
i32 Action_SetMoveRadius(AISYS_s *sys, AISCRIPTPROCESS_s *process,
                         AIPACKET_s *packet, char **args, int argc, int flags,
                         f32 time) {
  // 0x120: mover height; owner +0xac: collision radius.
  if (packet == NULL || packet->pd0 == NULL || flags == 0)
    return 1;
  *(f32 *)((u8 *)packet + 0x120) = *(f32 *)((u8 *)packet->pd0 + 0xac) * 2.0f;
  if (argc != 0 && NuStrICmp(args[0], "default") != 0)
    *(f32 *)((u8 *)packet + 0x120) = AIParamToFloatEx(packet, process, args[0]);
  return 1;
}
