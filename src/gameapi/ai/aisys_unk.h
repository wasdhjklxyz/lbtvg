#pragma once
// AI script system types; only fields with evidence.

#include "../../nu2api/nu3d/nuspecial.h"
#include "../../nu2api/nucore/nulist.h"
#include "../gameobject_unk.h"

struct AISYS_s;
struct AISCRIPT_s;
struct AIPATH_s;
struct AIPATHCNX_s;
struct APIOBJECT_s;
struct AISCRIPTPROCESS_s;
struct AIAREA_s;
struct AILOCATOR_s;
struct AILOCATORSET_s;
struct AILOCALMESSAGE_s;

// APIOBJECT_s in ref/saga/src/legoapi/items/base/apiobject.h: a GameObject_s*
// first, position at +0x5c.
struct Unk_AIPacketObj {
  GameObject_s *obj; // 0x00
  u8 pad0[0x5c - 4];
  nuvec_s pos5c; // 0x5c
};

struct AIPACKET_s {
  u8 pad0[0xd0];
  Unk_AIPacketObj *pd0; // 0xd0
  Unk_AIPacketObj *pd4; // 0xd4
  u8 pad1[0xe4 - 0xd8];
  Unk_AIPacketObj *pe4; // 0xe4
};

// Per-character route cursor. This type is shared by the AI runtime packet,
// creature spawn records, and formation rows.
typedef struct AIPATHINFO_s {
  AIPATH_s *path;
  AIPATHCNX_s *connection;
  u8 direction;
  u8 path_index;

  u16 game_flags;
  u16 next_check;

  union {
    struct {
      u8 on_path : 1;
      u8 was_on_path : 1;
      u8 route_checked : 1;
      u8 narrow_path : 1;
    };
    u8 flags;
  };
  u8 padding_0x0f;

  f32 dist;
  f32 width;
} AIPATHINFO;

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

typedef struct AISCRIPTPROCESSSTACK_s {
  f32 complex_params[4];
  u8 is_first_time_state;
  u8 force_complex_eval;
} AISCRIPTPROCESSSTACK;

typedef struct AIREFSCRIPT_s {
  NULISTLNK list_node;
  char *name;
  struct AISCRIPT_s *script;
  char *return_state_name;
  struct AISTATE_s *return_state;
  u32 check_global_scripts : 1;
  u32 check_level_scripts : 1;
  NULISTHDR conditions;
} AIREFSCRIPT;

typedef struct AISCRIPTPROCESS_s {
  AISCRIPT *base_script;
  AISCRIPT *script;

  AISTATE *state;
  NULISTLNK *action_node;
  AISTATE *next_state;
  f32 params[4];
  f32 script_timer;

  AISCRIPTPROCESSSTACK param_stack[2];

  u32 is_first_time_action : 1;
  u32 is_disabled : 1;
  u32 unknown_flag_4 : 1;

  AIREFSCRIPT *active_refs[4];
  i32 active_ref_count;

  union {
    u8 action_data_1;
    u8 hold_special_button;
  };
  u8 action_data_2;
  u16 action_data_6;
  union {
    void *action_data_3;
    APIOBJECT_s *override_control_object;
    i32 speeder_ahead_latched;
  };
  union {
    f32 action_data_4;
    f32 follow_direction_fire_range;
    GameObject_s *action_object;
  };
  union {
    f32 action_data_5;
    f32 follow_direction_fire_interval;
  };

  NUVEC action_pos;

  AIPATHINFO path_info;

  f32 action_timer;

  AIAREA_s *unknown_a0;
  union {
    AILOCATOR_s *unknown_a4;
    AILOCATOR_s *locator;
  };
  union {
    AILOCATORSET_s *unknown_a8;
    AILOCATORSET_s *locator_set;
  };
  NUGSPLINE *unknown_ac;

  // Types uncertain.
  union {
    u8 unknown_b0;
    u8 creature_set;
  };
  u16 unknown_b2;

  u32 unkown_b4;
  u8 interrupt_priority;
  u8 interrupt_id;

  u16 action_data_7;

  f32 interrupt_timer;
  AISTATE *interrupt_state;
  AISTATE *return_to_state;

  AILOCALMESSAGE_s *local_messages;
} AISCRIPTPROCESS;

typedef Unk_AIPacketObj *AIGETNAMEDAPIOBJECT(AISYS_s *sys, char *name);

// GLOBAL: LEGOBATMAN 0x00ad695c
extern AIGETNAMEDAPIOBJECT *GetNamedAPIObjectFn;

f32 AIParamToFloat(AISCRIPTPROCESS_s *process, char *str);
f32 AIParamToFloatEx(AIPACKET_s *packet, AISCRIPTPROCESS_s *process, char *str);
i32 AIScriptSetBaseScriptStateByName(AISCRIPTPROCESS_s *process, char *name);
void AIScriptProcess(AISYS_s *sys, GameObject_s *obj, AISCRIPTPROCESS_s *packet,
                     AISCRIPTPROCESS_s *process, f32 elapsed);
