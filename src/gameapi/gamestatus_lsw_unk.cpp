// gameapi/gamestatus_lsw_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nu2api/nucore/common.h"
#include <stddef.h>

// STUB: LEGOBATMAN 0x0064ce30
// heavy x87 (two rumble channels kept on the FPU stack); not attempted
#if 0
struct STATUSPACKET_s {
    STATUSPACKET_LSW_s *lsw_packet;                               // 0x00
    i32 (*init_callback)(WORLDINFO_s *, STATUSPACKET_s *);        // 0x04
    i32 (*finish_callback)(WORLDINFO_s *, STATUSPACKET_s *, i32); // 0x08
    void (*reset_callback)(STATUSPACKET_s *);                     // 0x0c
    void (*draw_background_callback)(STATUSPACKET_s *);           // 0x10
    AREADATA_s *area;                                             // 0x14
    EPISODEDATA *episode;                                         // 0x18
    u32 *score;                                                   // 0x1c
    struct MISSIONDATA_s *mission;                                // 0x20
    f32 player0_rumble_amount;                                    // 0x24
    f32 player0_rumble_time;                                      // 0x28
    f32 player0_rumble_duration;                                  // 0x2c
    f32 player0_buzz_amount;                                      // 0x30
    u8 player0_rumble_priority;                                   // 0x34
    undefined field_0x35[0x38 - 0x35];
    f32 player1_rumble_amount;   // 0x38
    f32 player1_rumble_time;     // 0x3c
    f32 player1_rumble_duration; // 0x40
    f32 player1_buzz_amount;     // 0x44
    u8 player1_rumble_priority;  // 0x48
    u8 field_0x49[3];
    f32 area_time;          // 0x4c
    f32 collected_score;    // 0x50
    f32 true_hero_target;   // 0x54
    f32 true_hero_percent;  // 0x58
    f32 previous_best_time; // 0x5c
    f32 new_best_time;      // 0x60
    f32 elapsed_time;       // 0x64
    f32 field_0x68;
    u32 previous_best_score; // 0x6c
    u32 new_best_score;      // 0x70
    u32 original_score;      // 0x74
    u32 reward_score;        // 0x78
    u32 time_reward_score;   // 0x7c
    u32 final_reward_score;  // 0x80
    u32 coins_remaining[2];  // 0x84
    u32 coins_collected[2];  // 0x8c
    i32 new_minikits;        // 0x94
    u32 field_0x98;
    u16 player0_model; // 0x9c
    u16 player1_model; // 0x9e
    i16 area_id;       // 0xa0
    i8 episode_id;     // 0xa2
    u8 field_0xa3;
    union {
        u8 player_active[2];
        struct {
            u8 player0_active;
            u8 player1_active;
        };
    };
    u8 challenge_state;   // 0xa6
    u8 mission_state;     // 0xa7
    f32 superstory_time;  // 0xa8
    u32 superstory_score; // 0xac
    union {
        u8 field_0xb0;
        struct {
            u8 : 2;
            u8 true_hero_complete : 1;
            u8 next_area_unlocked : 1;
            u8 new_minikit_complete : 1;
            u8 bonus_winner : 1;
            u8 free_play : 1;
            u8 vehicle_area : 1;
        };
    };
    union {
        u8 mode_flags; // 0xb1
        struct {
            u8 bonus_mode : 1;
            u8 super_bonus_mode : 1;
            u8 super_story_mode : 1;
            u8 saved_and_exited : 1;
            u8 continue_story : 1;
            u8 : 3;
        };
    };
    u8 status_flags;          // 0xb2
    u8 minikit_count;         // 0xb3
    u8 minikit_max;           // 0xb4
    u8 stage_count;           // 0xb5
    i8 current_gold_brick;    // 0xb6
    u8 save_state;            // 0xb7
    u8 prompt_choice;         // 0xb8
    u8 newly_completed;       // 0xb9
    u8 previous_completion;   // 0xba
    u8 previous_gold_bricks;  // 0xbb
    u8 displayed_gold_bricks; // 0xbc
    u8 field_0xbd;
    i8 field_0xbe;
    u8 field_0xbf[7];
    i8 stage_types[0xee - 0xc6]; // 0xc6
    u8 gold_brick_enabled[40];   // 0xee
    i16 next_area;               // 0x116
    i8 chapter;                  // 0x118
    u8 field_0x119[3];
    struct STATUS_STAGE_s *previous_stage_2; // 0x11c
    struct STATUS_STAGE_s *previous_stage;   // 0x120
    struct STATUS_STAGE_s *stage;            // 0x124
    struct STATUS_STAGE_s *next_stage;       // 0x128
    undefined field_0x12c[0x14c - 0x12c];
};

typedef uint8_t u8;
typedef u8 undefined;

STATUSPACKET_s StatusPacket;

// from saga legoapi/menus/screens/gamestatus_lsw.cpp
void NewStatusRumbleBuzz(i32 player, float amount, float buzz, i32 priority) {
    if (!(amount > 0.0f)) {
        if (StatusPacket.player0_active != 0 && (player == 0 || player == -1)) {
            goto player0_buzz;
        }
        if (StatusPacket.player1_active != 0 && (player == 1 || player == -1)) {
            goto player1_buzz;
        }
        return;
    }

    if (StatusPacket.player0_active != 0 && (player == 0 || player == -1)) {
        if (StatusPacket.player0_rumble_time <= 0.0f || amount > StatusPacket.player0_rumble_time /
                                                                     StatusPacket.player0_rumble_duration *
                                                                     StatusPacket.player0_rumble_amount) {
            StatusPacket.player0_rumble_amount = amount;
            StatusPacket.player0_rumble_duration = amount;
            StatusPacket.player0_rumble_time = amount;
        }
    player0_buzz:
        if (buzz > StatusPacket.player0_buzz_amount) {
            StatusPacket.player0_buzz_amount = buzz;
        }
        if (priority > 0) {
            const i32 old_priority = StatusPacket.player0_rumble_priority;
            ++priority;
            if (priority > old_priority) {
                StatusPacket.player0_rumble_priority = static_cast<u8>(priority);
            }
        }
    }

    if (StatusPacket.player1_active != 0 && (player == 1 || player == -1)) {
        if (StatusPacket.player1_rumble_time <= 0.0f || amount > StatusPacket.player1_rumble_time /
                                                                     StatusPacket.player1_rumble_duration *
                                                                     StatusPacket.player1_rumble_amount) {
            StatusPacket.player1_rumble_amount = amount;
            StatusPacket.player1_rumble_duration = amount;
            StatusPacket.player1_rumble_time = amount;
        }
    player1_buzz:
        if (buzz > StatusPacket.player1_buzz_amount) {
            StatusPacket.player1_buzz_amount = buzz;
        }
        if (priority > 0) {
            const i32 old_priority = StatusPacket.player1_rumble_priority;
            ++priority;
            if (priority > old_priority) {
                StatusPacket.player1_rumble_priority = static_cast<u8>(priority);
            }
        }
    }
}
#endif
