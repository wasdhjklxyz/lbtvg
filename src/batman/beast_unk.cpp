// batman/, file unknown (between Move_CHARACTER and Move_BEAST).

#include "../gameapi/gameobject_unk.h"
#include "../gameapi/sfx_unk.h"

void PartStop_Flickerer(PART_s *part);

// FUNCTION: LEGOBATMAN 0x004ccf20
void PartStop_Poo(PART_s *part) {
  PartStop_Flickerer(part);
  PlaySfx("lego_PLOP", &part->pos);
}
