// batman/, file unknown (between Move_CHARACTER and Move_BEAST).

#include "../gameapi/gameobject_unk.h"
#include "../gameapi/sfx_unk.h"

void PartStop_Flickerer(PART_s *part);

// FUNCTION: LEGOBATMAN 0x004ccf20
void PartStop_Poo(PART_s *part) {
  PartStop_Flickerer(part);
  PlaySfx("lego_PLOP", &part->pos);
}

struct CHEAT_s {
  u32 pad0[4];
};

// GLOBAL: LEGOBATMAN 0x00936f10
extern CHEAT_s g_unk00936f10[];

i32 Cheat_IsOn(CHEAT_s *cheat);
i32 qrand(void);
void *AddGameMessage(char *text, nuvec_s *position, float scale,
                     nuvec_s *target_position, float target_scale,
                     unsigned char red, unsigned char green, unsigned char blue,
                     u32 flags, float duration);

// FUNCTION: LEGOBATMAN 0x004cfe50
void Cheat_SpecialHits(float damage, nuvec_s *position) {
  char *hits[14] = {"Pow!",   "Smack!", "Bing!",   "KaPow!", "Krakt!",
                    "Zowie!", "Clank!", "Crunch!", "Zap!",   "Slap!",
                    "Zoink!", "Splat!", "Biff!",   "Crack!"};
  if (damage != 0.0f && Cheat_IsOn(&g_unk00936f10[8]))
    AddGameMessage(hits[qrand() / (0xffff / 14 + 1)], position, 0.0f, 0, 2.0f,
                   0xff, 0xff, 0xff, 0x47, 0.2f);
}
