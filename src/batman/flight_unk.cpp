// batman/flight_unk.cpp: the Flight level scripts (Mac: Flight_Init,
// Flight_Reset, Flight_Update, Flight_A/B/C_*), 0x510ff0..; file name
// unproven.

#include "../nu2api/nucore/common.h"
#include <string.h>

typedef struct WORLDINFO_s WORLDINFO;

typedef struct FLIGHTDATA_s {
  u8 pad[0xd4];
} FLIGHTDATA;

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
