// batman/flight_unk.cpp: the Flight level scripts (Mac: Flight_Init,
// Flight_Reset, Flight_Update, Flight_A/B/C_*), 0x510ff0..; file name
// unproven.

#include "../nu2api/nucore/common.h"
#include <stddef.h>
#include <string.h>

typedef struct FLIGHTWORLD_s WORLDINFO;

typedef struct GIZAIMESSAGE_s GIZAIMESSAGE;
typedef struct GIZAIMESSAGESYS_s GIZAIMESSAGESYS;

typedef struct FLIGHTOBJ_s {
  u8 pad0[0x80];
  f32 v80[3]; // 0x80
  u8 pad8c[0x1fc - 0x8c];
  u32 flags1fc; // 0x1fc
  u8 pad200[0x24d - 0x200];
  i8 b24d; // 0x24d
  u8 pad24e[0x15b0 - 0x24e];
  i16 type15b0; // 0x15b0
  u8 pad15b2[0x15c7 - 0x15b2];
  i8 current_hp; // 0x15c7
  u8 pad15c8[0x1600 - 0x15c8];
  i16 s1600; // 0x1600
} FLIGHTOBJ;

typedef struct FLIGHTEFFECTS_s {
  u8 pad0[0xa78];
  i32 smoke; // 0xa78
  u8 pada7c[0xa8c - 0xa7c];
  i32 fire; // 0xa8c
} FLIGHTEFFECTS;

typedef struct FLIGHTP138_s {
  u8 pad0[8];
  FLIGHTEFFECTS *effects; // 0x08
} FLIGHTP138;

typedef struct FLIGHTDATA_s {
  u8 pad0[0xc8];
  GIZAIMESSAGE *mad_turret1; // 0xc8
  GIZAIMESSAGE *mad_turret2; // 0xcc
  FLIGHTOBJ *boss_chopper;   // 0xd0
} FLIGHTDATA;

typedef struct FLIGHTWORLD_s {
  u8 pad0[0x12c];
  void *area; // 0x12c
  u8 pad130[0x138 - 0x130];
  FLIGHTP138 *p138; // 0x138
  u8 pad13c[0x2bf8 - 0x13c];
  void *ai_sys; // 0x2bf8
} FLIGHTWORLD;

// GLOBAL: LEGOBATMAN 0x009cee28
extern FLIGHTDATA FlightData;

void Flight_Reset(WORLDINFO *world);
void Flight_Update(WORLDINFO *world);

// FUNCTION: LEGOBATMAN 0x00510ff0
void Flight_Init(WORLDINFO *world) {
  memset(&FlightData, 0, sizeof(FlightData));
}

// FUNCTION: LEGOBATMAN 0x005115b0
void Flight_A_Init(WORLDINFO *world) {
  memset(&FlightData, 0, sizeof(FlightData));
}

// FUNCTION: LEGOBATMAN 0x005115d0
void Flight_A_Reset(WORLDINFO *world) { Flight_Reset(world); }

// FUNCTION: LEGOBATMAN 0x005115e0
void Flight_A_Update(WORLDINFO *world) { Flight_Update(world); }

// FUNCTION: LEGOBATMAN 0x005115f0
void Flight_B_Init(WORLDINFO *world) {
  memset(&FlightData, 0, sizeof(FlightData));
}

// FUNCTION: LEGOBATMAN 0x00511610
void Flight_B_Reset(WORLDINFO *world) { Flight_Reset(world); }

// FUNCTION: LEGOBATMAN 0x00511620
void Flight_B_Update(WORLDINFO *world) { Flight_Update(world); }

// FUNCTION: LEGOBATMAN 0x00511630
void Flight_C_Init(WORLDINFO *world) {
  memset(&FlightData, 0, sizeof(FlightData));
}

// GLOBAL: LEGOBATMAN 0x00ad210c
extern GIZAIMESSAGESYS *gizaimessagesys;
// GLOBAL: LEGOBATMAN 0x00961c54
extern i16 g_unk00961c54;

GIZAIMESSAGE *CheckGizAIMessage(GIZAIMESSAGESYS *sys, char const *name,
                                GIZAIMESSAGE *message);
FLIGHTOBJ *AIFindObjectUnk0044c930(void *ai_sys, char *name);

// FUNCTION: LEGOBATMAN 0x00511650
void Flight_C_Reset(WORLDINFO *world) {
  Flight_Reset(world);
  FlightData.mad_turret1 = CheckGizAIMessage(gizaimessagesys, "MadTurret1", 0);
  FlightData.mad_turret2 = CheckGizAIMessage(gizaimessagesys, "MadTurret2", 0);
  FlightData.boss_chopper =
      AIFindObjectUnk0044c930(world->ai_sys, "policeBossChopper");
  if (FlightData.boss_chopper != NULL)
    FlightData.boss_chopper->s1600 = g_unk00961c54;
}

// GLOBAL: LEGOBATMAN 0x00960894
extern FLIGHTWORLD *WORLD;
// GLOBAL: LEGOBATMAN 0x00a95fe0
extern f32 FRAMETIME;

extern "C" void AddVariableShotDebrisEffectTimed1(i32 effect, f32 *pos,
                                                  i32 count, f32 time, i32 a,
                                                  i32 b, i32 c);

// FUNCTION: LEGOBATMAN 0x005116d0
void Flight_C_Update(WORLDINFO *world) {
  Flight_Update(world);
  if (FlightData.boss_chopper != NULL &&
      (FlightData.boss_chopper->flags1fc & 0x1000) &&
      FlightData.boss_chopper->current_hp <= 4) {
    AddVariableShotDebrisEffectTimed1(WORLD->p138->effects->smoke,
                                      FlightData.boss_chopper->v80, 0x1e,
                                      FRAMETIME, 0, 0, 0);
    if (FlightData.boss_chopper->current_hp <= 2)
      AddVariableShotDebrisEffectTimed1(WORLD->p138->effects->fire,
                                        FlightData.boss_chopper->v80, 0x1e,
                                        FRAMETIME, 0, 0, 0);
  }
}

// GLOBAL: LEGOBATMAN 0x009ca8a8
extern void *g_unk009ca8a8; // the crane level's area

// FUNCTION: LEGOBATMAN 0x005117b0
i32 TurnOnCraneDirtyHack() { return WORLD->area == g_unk009ca8a8; }

// GLOBAL: LEGOBATMAN 0x00936538
extern i16 g_unk00936538; // character id the hack applies to

// FUNCTION: LEGOBATMAN 0x005117d0
i32 CraneDirtyHackCheck(FLIGHTOBJ *obj, FLIGHTOBJ *other) {
  if (obj != NULL && other != NULL && other->type15b0 == g_unk00936538)
    return 1;
  return obj->b24d;
}
