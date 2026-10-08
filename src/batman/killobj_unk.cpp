// batman/, file unknown (KillGameObject / StunGameObject / ObjHitObj TU).

#include "../gameapi/gameobject_unk.h"

i32 KillGameObject(GameObject_s *obj, i32 a, i32 b);
void Arcade_PlayerKilled(i32 player, i32 flag);

// GLOBAL: LEGOBATMAN 0x00ad1178
extern i32 g_unk00ad1178;
// GLOBAL: LEGOBATMAN 0x00ab3960
extern GameObject_s *g_unk00ab3960[2];

// FUNCTION: LEGOBATMAN 0x0043d380
i32 KillPlayer(GameObject_s *obj, i32 a, i32 force, nuvec_s *pos) {
  if (obj->b257)
    return 0;
  if (force)
    obj->f1534 = 0.0f;
  else if (obj->f1534 > 0.0f)
    return 0;
  if (g_unk00ad1178) {
    i32 flag = 0;
    i32 other = obj->b24c == 0 ? 1 : (obj->b24c == 1 ? 0 : -1);
    if (g_unk00ab3960[other] && (g_unk00ab3960[other]->flags1f8 & 0x40000))
      flag = 1;
    if (other != -1)
      Arcade_PlayerKilled(other, flag);
  }
  return KillGameObject(obj, a, 0);
}
