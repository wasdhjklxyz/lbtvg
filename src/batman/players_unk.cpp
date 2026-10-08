// batman/, file unknown: player activation (0x0043d430); saga keeps these in
// legoapi/characters/core/players.cpp.

#include "../gameapi/ai/aisys_unk.h"
#include "worldinfo_unk.h"

// GLOBAL: LEGOBATMAN 0x00a95fe0
extern f32 FRAMETIME;

void Player_ClearContext(GameObject_s *obj, i32 a);
void Player_ResetContexts(GameObject_s *obj);
void NewBuzz(nupad_s *pad, f32 duration, i32 mode);

// FUNCTION: LEGOBATMAN 0x0043d430
i32 DeactivatePlayer(GameObject_s *obj, f32 time, GameObject_s *by) {
  f32 saved;
  AISCRIPTPROCESS_s *process;
  if (obj->b9db != 0x17 || obj->f98c < time) {
    saved = obj->f11cc;
    Player_ClearContext(obj, 1);
    if (obj->b9db != 0x3e) {
      Player_ResetContexts(obj);
      obj->f11cc = saved;
      process = (AISCRIPTPROCESS_s *)obj->process290;
      obj->b9db = 0x17;
      if (AIScriptSetBaseScriptStateByName(process, "BeenDeactivated"))
        AIScriptProcess(g_unk00960894->aiSys2bf8, obj, process, process,
                        FRAMETIME);
      if (obj->p50->p0c->p204 && !(obj->p50->p08->p204->flags4 & 2))
        obj->s9d0 = 0x81;
      else if (obj->p50->p0c->p104)
        obj->s9d0 = 0x41;
      else
        obj->s9d0 = obj->s162c;
      obj->f98c = time;
      obj->f998 = 0.0f;
      obj->b9df = 0;
      obj->f988 = 0.0f;
      NewBuzz(obj->p112c->pad0, 0.1f, 0);
      return 1;
    }
  }
  return 0;
}
