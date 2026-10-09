// gameapi/, file unknown (between rtleditor.cpp and listman_gen.cpp by link
// order).

struct Unk0062bf50 {
  unsigned char pad[0xb68];
  int field_b68;
};

// FUNCTION: LEGOBATMAN 0x0062bf50
bool IsUnk0062bf50Clear(Unk0062bf50 *p) { return p->field_b68 == 0; }

typedef struct MISSIONDATA_s MISSIONDATA;

typedef struct MISSIONSYS_s {
  int unk0;                      // 0x00
  MISSIONDATA *mission;          // 0x04
  unsigned char timer[0x1d - 8]; // 0x08 TIMER_s
  unsigned char active;          // 0x1d
} MISSIONSYS;

// GLOBAL: LEGOBATMAN 0x00acd7f8
extern MISSIONSYS *MissionSys;

struct TIMER_s;
void ResetTimer(TIMER_s *timer, float time);

// FUNCTION: LEGOBATMAN 0x0062db70
void Mission_Clear(MISSIONSYS *ms) {
  if (ms == 0) {
    ms = MissionSys;
    if (ms == 0) {
      return;
    }
  }
  ms->mission = 0;
  ms->active = 0;
  ResetTimer((TIMER_s *)ms->timer, 0.0f);
}

// FUNCTION: LEGOBATMAN 0x0062dba0
MISSIONDATA *Mission_Active(MISSIONSYS *ms) {
  if (ms == 0) {
    ms = MissionSys;
    if (ms == 0) {
      return 0;
    }
  }
  if (ms->active != 0 && ms->mission != 0) {
    return ms->mission;
  }
  return 0;
}
