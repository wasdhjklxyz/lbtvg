#pragma once
// Audio prototypes; the C ones follow ref/saga/src/legoapi/audio/sfx.h.

#include "../nu2api/numath/nuvec.h"

void PlaySfx(char *name, nuvec_s *pos);
void PlaySfxAndSetVolume(char *name, nuvec_s *pos, f32 volume);
i32 GetSfxId(char *name, nuvec_s *pos, i32 flags, i32 volume);
void GameAudio_PlaySfxById(i32 sfx_id, nuvec_s *position, i32 flags,
                           i32 volume);
void GameAudio_PlaySfx(i32 sfx, nuvec_s *position, i32 flags, i32 volume);
