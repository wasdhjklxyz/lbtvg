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
  int unk0;             // 0x00
  MISSIONDATA *mission; // 0x04
  unsigned char pad8[0x1d - 8];
  unsigned char active; // 0x1d
} MISSIONSYS;

// GLOBAL: LEGOBATMAN 0x00acd7f8
extern MISSIONSYS *MissionSys;

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
