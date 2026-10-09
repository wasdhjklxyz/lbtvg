// batman/shop_unk.cpp: placed by tools/new.py; file name unproven.

#include "../nu2api/nucore/common.h"
#include <stddef.h>

// STUB: LEGOBATMAN 0x004e8190
// long; u64 bit set via __allshl and many shop/menu fields; not attempted
#if 0
#include "../nu2api/nu3d/nuspecial.h"

struct shopitem_s {
    char name[0x40];
    char special_name[0x20];
    i16 item_id;
    u8 type;
    u8 unlocked;
    i32 price;
    nuhspecial_s special;
};

struct GAMESAVE_s Game;

void PlaySfx(char *name, nuvec_s *pos);

static f32 pickedbing;

NUVEC SubShelfPos[7] = {};

// from saga legoapi/menus/screens/shop.cpp
i32 BuyShopItem(shopitem_s *items, i32 index, i32 charge) {
    shopitem_s *item = &items[index];
    u32 *bits = NULL;
    if (item->type == 0)
        bits = Game.shop_hint_purchased_bits;
    else if (item->type == 1)
        bits = Game.shop_character_purchased_bits;
    else if (item->type == 2)
        bits = Game.extra_unlocked_bits;
    else if (item->type == 4)
        bits = &Game.shop_gold_brick_purchased_bits;
    else if (item->type == 5) {
        PlaySfx("MenuSelect", 0);
        return 1;
    }
    if (bits != NULL)
        bits[index / 32] |= static_cast<u32>(u64(1) << (index % 32));
    if (charge != 0)
        Game.coins -= item->price;
    pickedbing = 0.35f;
    item->unlocked = 1;
    if (charge != 0)
        PlaySfx("Shop_BuyCheat", &SubShelfPos[3]);
    return 1;
}
#endif
