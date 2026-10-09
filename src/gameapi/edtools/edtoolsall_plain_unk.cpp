// gameapi/edtools/edtoolsall_plain.cpp (saga), between oggreader and
// terrain.c: the edbits registration setters.

#include "../../nu2api/nucore/common.h"

struct nugscn_s;

// GLOBAL: LEGOBATMAN 0x00a28d00
void *unk_00a28d00;
// GLOBAL: LEGOBATMAN 0x00a28d04
struct nugscn_s *edbits_things_scene;
// GLOBAL: LEGOBATMAN 0x00a28d08
void *unk_00a28d08;

// FUNCTION: LEGOBATMAN 0x00561560
extern "C" void SetUnk00a28d00(void *value) { unk_00a28d00 = value; }

// FUNCTION: LEGOBATMAN 0x00561570
extern "C" void edbitsRegisterThingsScene(struct nugscn_s *scene) {
  edbits_things_scene = scene;
}

// FUNCTION: LEGOBATMAN 0x00561580
extern "C" void SetUnk00a28d08(void *value) { unk_00a28d08 = value; }
