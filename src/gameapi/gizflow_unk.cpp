// gameapi/gizflow_unk.cpp: gizmo flow box script parsers (saga
// legoapi/gizmo/base/gizflow.cpp); file name unproven.

#include "../nu2api/nucore/common.h"
#include "../nu2api/nucore/nustring.h"
#include "../nu2api/numath/nuinline_unk.h"
#include <stddef.h>

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x00653460
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

typedef struct nufpar_s {
  u8 pad0[0x910];
  char *word_buf; // 0x910
} NUFPAR;

i32 NuFParGetWord(NUFPAR *parser);
i32 NuFParGetInt(NUFPAR *parser);
f32 NuFParGetFloat(NUFPAR *parser);
void NuStrNCpy(char *dst, const char *src, i32 n);

struct GIZMOSYS_s;
i32 GizmoGetTypeIDByName(GIZMOSYS_s *gizmo_sys, char *name);

struct GIZFLOW_s {
  GIZMOSYS_s *gizmo_sys;
};

struct FLOWBOX_s {
  u8 pad0[3];
  u8 type;       // 0x3
  u8 runtime_id; // 0x4
  u8 pad5[0xa - 0x5];
  u16 state_flags; // 0xa
  union {
    u8 *condition_data;              // 0xc
    struct FLOWBOXACTION_s *actions; // 0xc
  };
};

struct FLOWCONDITIONTYPE {
  i32 type;
  char *name;
  void (*load)(NUFPAR *parser);
};

// GLOBAL: LEGOBATMAN 0x00ad1a79
static i8 load_t_randomTime;
// GLOBAL: LEGOBATMAN 0x00ad1a7c
static i32 load_r_noutputs;
// GLOBAL: LEGOBATMAN 0x00ad1a80
static char load_gizmoname[32];
// GLOBAL: LEGOBATMAN 0x00ad1ab4
static u16 load_g_flags;
// GLOBAL: LEGOBATMAN 0x00ad1ab8
static i32 load_r_outputChance[8];
// GLOBAL: LEGOBATMAN 0x00ad1ad8
static char load_name[32];
// GLOBAL: LEGOBATMAN 0x00ad1af8
static i32 load_numgizmos;
// GLOBAL: LEGOBATMAN 0x00ad1afc
static i32 load_conditiontype;
// GLOBAL: LEGOBATMAN 0x00ad1b00
static f32 load_t_time;
// GLOBAL: LEGOBATMAN 0x00ad1bc8
static i32 load_gizmotype;
// GLOBAL: LEGOBATMAN 0x00ad1bd8
static GIZFLOW_s *load_gizflow;
// GLOBAL: LEGOBATMAN 0x00ad1be0
static FLOWBOX_s *load_flowbox;
// GLOBAL: LEGOBATMAN 0x00966e48
extern FLOWCONDITIONTYPE ConditionTypes[];

struct FLOWREMAP {
  i16 parents[16];       // 0x00
  i16 children[32];      // 0x20
  u8 parent_outputs[16]; // 0x60
};

// GLOBAL: LEGOBATMAN 0x00ad1ab0
static FLOWREMAP *remap;
// GLOBAL: LEGOBATMAN 0x00ad1b04
static i32 numRemaps;
// GLOBAL: LEGOBATMAN 0x00ad1be4
static i32 load_nparents;
// GLOBAL: LEGOBATMAN 0x00ad1be8
static i32 load_nchildren;

// FUNCTION: LEGOBATMAN 0x00654020
void xParent_Col(NUFPAR *parser) {
  if (load_nparents < 16) {
    remap[numRemaps].parents[load_nparents] = NuFParGetInt(parser);
    remap[numRemaps].parent_outputs[load_nparents++] = NuFParGetInt(parser);
  }
}

// FUNCTION: LEGOBATMAN 0x00654090
void xChild_Col(NUFPAR *parser) {
  if (load_nchildren < 32)
    remap[numRemaps].children[load_nchildren++] = NuFParGetInt(parser);
}

// GLOBAL: LEGOBATMAN 0x00ad1b88
static i32 load_parents[16];
// GLOBAL: LEGOBATMAN 0x00ad1aa0
static u8 load_parent_output_ix[16];
// GLOBAL: LEGOBATMAN 0x00ad1b08
static i32 load_children[32];

// FUNCTION: LEGOBATMAN 0x006540e0
static void remapParent(i32 id) {
  i32 slot = -1 - id;
  for (i32 i = 0; load_nparents < 16; ++i) {
    i16 parent = remap[slot].parents[i];
    if (parent == id)
      break;
    if (parent < 0) {
      remapParent(parent);
    } else {
      load_parents[load_nparents] = parent;
      load_parent_output_ix[load_nparents] = remap[slot].parent_outputs[i];
      ++load_nparents;
    }
  }
}

// FUNCTION: LEGOBATMAN 0x00654160
static void remapChildren(i32 id) {
  i32 slot = -1 - id;
  for (i32 i = 0; load_nchildren < 32; ++i) {
    i16 child = remap[slot].children[i];
    if (child == id)
      break;
    if (child < 0)
      remapChildren(child);
    else
      load_children[load_nchildren++] = child;
  }
}

// FUNCTION: LEGOBATMAN 0x006543d0
void xParent(NUFPAR *parser) {
  if (load_nparents < 16) {
    i32 parent = NuFParGetInt(parser);
    if (parent < 0) {
      NuFParGetInt(parser);
      remapParent(parent);
    } else {
      load_parents[load_nparents] = parent;
      load_parent_output_ix[load_nparents] = NuFParGetInt(parser);
      ++load_nparents;
    }
  }
}

// FUNCTION: LEGOBATMAN 0x00654430
void xChild(NUFPAR *parser) {
  if (load_nchildren < 32) {
    i32 child = NuFParGetInt(parser);
    if (child < 0)
      remapChildren(child);
    else
      load_children[load_nchildren++] = child;
  }
}

// FUNCTION: LEGOBATMAN 0x006547d0
void xGizmoType(NUFPAR *parser) {
  NuFParGetWord(parser);
  if (parser->word_buf != 0)
    load_gizmotype =
        GizmoGetTypeIDByName(load_gizflow->gizmo_sys, parser->word_buf);
}

// FUNCTION: LEGOBATMAN 0x00654800
void xGizmoName(NUFPAR *parser) {
  NuFParGetWord(parser);
  if (parser->word_buf != 0)
    NuStrNCpy(load_gizmoname, parser->word_buf, 32);
}

// FUNCTION: LEGOBATMAN 0x00654830
void xStartInvis(NUFPAR *parser) { load_g_flags |= 1; }

// FUNCTION: LEGOBATMAN 0x00654840
void xEndDeact(NUFPAR *parser) { load_g_flags |= 8; }

// FUNCTION: LEGOBATMAN 0x00654850
void xEndInvis(NUFPAR *parser) { load_g_flags |= 2; }

// FUNCTION: LEGOBATMAN 0x00654860
void xReverse(NUFPAR *parser) { load_g_flags |= 4; }

// FUNCTION: LEGOBATMAN 0x00654870
void xReverseInvis(NUFPAR *parser) { load_g_flags |= 0x80; }

// FUNCTION: LEGOBATMAN 0x00654880
void xNotFreeplay(NUFPAR *parser) { load_g_flags |= 0x40; }

// FUNCTION: LEGOBATMAN 0x00654890
void xNotStoryMode(NUFPAR *parser) { load_g_flags |= 0x20; }

// FUNCTION: LEGOBATMAN 0x006548a0
void xOutputOnly(NUFPAR *parser) { load_g_flags = 0x10; }

// FUNCTION: LEGOBATMAN 0x006548b0
void xGizTimer(NUFPAR *parser) { load_t_time = NuFParGetFloat(parser); }

// FUNCTION: LEGOBATMAN 0x006548d0
void xGizRandomTime(NUFPAR *parser) { load_t_randomTime = 1; }

// FUNCTION: LEGOBATMAN 0x006548e0
void xRand_NumOutputs(NUFPAR *parser) {
  load_r_noutputs = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x00654900
void xRand_OutputChance(NUFPAR *parser) {
  i32 index = NuFParGetInt(parser);
  load_r_outputChance[index] = NuFParGetInt(parser);
}

// FUNCTION: LEGOBATMAN 0x00654930
void xName(NUFPAR *parser) {
  NuFParGetWord(parser);
  if (parser->word_buf != 0)
    NuStrNCpy(load_name, parser->word_buf, 32);
}

// FUNCTION: LEGOBATMAN 0x00654960
void xNumGizmos(NUFPAR *parser) { load_numgizmos = NuFParGetInt(parser); }

// FUNCTION: LEGOBATMAN 0x00654980
void xAIAssistID(NUFPAR *parser) {
  load_flowbox->runtime_id = NuFParGetInt(parser);
}

i32 NuFParGetLine(NUFPAR *parser);
i32 NuFParInterpretWord(NUFPAR *parser);
i32 NuFParPushCom(NUFPAR *parser, void *commands);
void NuFParPopCom(NUFPAR *parser);
void *GizmoBufferAlloc(VARIPTR *buf, VARIPTR *end, i32 size);

// GLOBAL: LEGOBATMAN 0x00ad1a78
static i8 load_conditionParam;
// GLOBAL: LEGOBATMAN 0x00ad1bd0
static VARIPTR *load_buff;
// GLOBAL: LEGOBATMAN 0x00ad1bd4
static VARIPTR *load_endbuff;
// GLOBAL: LEGOBATMAN 0x00966fa0
extern u8 cfgtab_Condition[];

// FUNCTION: LEGOBATMAN 0x006549f0
void xCondition(NUFPAR *parser) {
  if (load_flowbox == 0)
    return;
  load_flowbox->type = 1;
  load_conditiontype = -1;
  load_conditionParam = -1;
  NuStrCpy(load_gizmoname, "");
  NuFParPushCom(parser, cfgtab_Condition);
  while (NuFParGetLine(parser)) {
    NuFParGetWord(parser);
    if (NuStrICmp(parser->word_buf, "}") == 0)
      break;
    NuFParInterpretWord(parser);
  }
  NuFParPopCom(parser);
  if (load_conditiontype >= 0) {
    u8 *condition = (u8 *)GizmoBufferAlloc(load_buff, load_endbuff, 4);
    if (condition != 0) {
      condition[0] = load_conditiontype;
      condition[1] = load_conditionParam;
    }
    load_flowbox->condition_data = condition;
  }
}

// FUNCTION: LEGOBATMAN 0x00654ac0
void xConditionType(NUFPAR *parser) {
  i32 index = 0;
  NuFParGetWord(parser);
  for (FLOWCONDITIONTYPE *type = ConditionTypes; type->type != -1;
       type++, index++) {
    if (load_conditiontype != -1)
      break;
    if (NuStrICmp(type->name, parser->word_buf) == 0) {
      void (*load)(NUFPAR *) = type->load;
      load_conditiontype = index;
      if (load != 0)
        load(parser);
    }
  }
}

// FUNCTION: LEGOBATMAN 0x00654b30
void xMonitorInputs(NUFPAR *parser) { load_flowbox->state_flags |= 0x200; }

struct FLOWBOXACTION_s;
int NuStrCmp(const char *a, const char *b);

struct GIZACTIONDEFN_s {
  char *name; // 0x0
  u8 pad4[4];
  void (*load)(GIZFLOW_s *gizflow, FLOWBOX_s *flowbox, char **parameters,
               i32 parameter_count); // 0x8
};

struct FLOWBOXACTION_s {
  FLOWBOXACTION_s *next;       // 0x0
  char **parameters;           // 0x4
  i32 parameter_count;         // 0x8
  GIZACTIONDEFN_s *definition; // 0xc
};

// GLOBAL: LEGOBATMAN 0x00ad1bcc
static GIZACTIONDEFN_s *gizactiondefs;

// keyword "Action" in table 0x00966ec8
// STUB: LEGOBATMAN 0x00654bc0
// close: logic and layout line up; orig keeps previous in esi and the word
// pointer in ebx, ours keeps a zero in edi and previous on the stack.
void xAction(NUFPAR *parser) {
  FLOWBOXACTION_s *previous = NULL;
  char parameters[16][64];
  if (load_flowbox == NULL)
    return;
  load_flowbox->type = 2;
  while (NuFParGetLine(parser)) {
    NuFParGetWord(parser);
    if (NuStrICmp(parser->word_buf, "}") == 0)
      break;
    GIZACTIONDEFN_s *definition = gizactiondefs;
    char *word = parser->word_buf;
    if (definition == NULL)
      continue;
    for (; definition->name != NULL; definition++) {
      if (NuStrICmp(word, definition->name) != 0)
        continue;
      FLOWBOXACTION_s *action = (FLOWBOXACTION_s *)GizmoBufferAlloc(
          load_buff, load_endbuff, sizeof(FLOWBOXACTION_s));
      if (action == NULL)
        break;
      action->next = NULL;
      action->parameters = NULL;
      action->parameter_count = 0;
      action->definition = NULL;
      if (previous != NULL)
        previous->next = action;
      else
        load_flowbox->actions = action;
      action->definition = definition;
      i32 count = 0;
      while (NuFParGetWord(parser)) {
        if (NuStrCmp(parser->word_buf, "\\") == 0)
          NuFParGetLine(parser);
        else
          NuStrCpy(parameters[count++], parser->word_buf);
      }
      if (count != 0) {
        action->parameters = (char **)GizmoBufferAlloc(load_buff, load_endbuff,
                                                       count * sizeof(char *));
        if (action->parameters != NULL) {
          action->parameter_count = count;
          for (i32 i = 0; i < count; ++i) {
            char **destination = &action->parameters[i];
            VARIPTR *end = load_endbuff;
            VARIPTR *buffer = load_buff;
            char *str = parameters[i];
            char *parameter = NULL;
            if (str != NULL) {
              i32 length = NuStrLen(str);
              if (length != 0) {
                parameter = (char *)GizmoBufferAlloc(buffer, end, length + 1);
                NuStrCpy(parameter, str);
              }
            }
            *destination = parameter;
          }
        }
        if (definition->load != NULL)
          definition->load(load_gizflow, load_flowbox, action->parameters,
                           action->parameter_count);
      }
      previous = action;
      break;
    }
  }
}

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_gizflow_unk(f32 *v, f32 a, i32 i) {
  NuVec4Set(v, a, a, a, a);
}
