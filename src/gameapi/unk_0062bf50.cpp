// gameapi/, file unknown (between rtleditor.cpp and listman_gen.cpp by link
// order).

#include "../nu2api/nucore/common.h"
#include "../nu2api/numath/nuinline_unk.h"
#include "../nu2api/numath/nutrig_unk.h"

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x0062be60
static f32 NuSinApprox(i32 angle);
// FUNCTION: LEGOBATMAN 0x0062bf00
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);

struct Unk0062bf50 {
  unsigned char pad[0xb68];
  int field_b68;
};

// FUNCTION: LEGOBATMAN 0x0062bf50
bool IsUnk0062bf50Clear(Unk0062bf50 *p) { return p->field_b68 == 0; }

typedef struct MISSIONDATA_s MISSIONDATA;

typedef struct MISSIONSYS_s {
  int unk0;                      // 0x00
  MISSIONDATA *mission;          // 0x04
  unsigned char timer[0x1d - 8]; // 0x08 TIMER_s
  unsigned char active;          // 0x1d
} MISSIONSYS;

// GLOBAL: LEGOBATMAN 0x00acd7f8
extern MISSIONSYS *MissionSys;

struct TIMER_s;
void ResetTimer(TIMER_s *timer, float time);

struct SHADOWMTL_s {
  u8 pad0[0x40];
  u32 filter_mode : 4; // 0x40
  u32 alpha_mode : 2;
  u32 attrib6 : 2;
  u32 attrib8 : 2;
  u32 attrib10 : 2;
  u32 attrib12 : 2;
  u32 z_mode : 2;
  u32 attrib16 : 2;
  u32 attrib18 : 1;
  u32 attrib19 : 1;
  u32 attrib20 : 3;
  u32 attrib23 : 9;
  u8 pad44[0x54 - 0x44];
  f32 f54; // 0x54
  f32 f58; // 0x58
  f32 f5c; // 0x5c
  u8 pad60[0x70 - 0x60];
  f32 f70; // 0x70
  u16 tid; // 0x74
  u16 w76; // 0x76
  u8 pad78[0x270 - 0x78];
  u32 flags270; // 0x270
};

// GLOBAL: LEGOBATMAN 0x00acd7f4
extern SHADOWMTL_s *CharShadowMtl;

SHADOWMTL_s *Unk00727dd0(i32 count);
u16 Unk006e6060(char *name, variptr_u *buffer, variptr_u buffer_end);
void NuMtlUpdate(SHADOWMTL_s *mtl);

// FUNCTION: LEGOBATMAN 0x0062d030
void CharShadows_InitMaterial(variptr_u *buffer, variptr_u buffer_end) {
  buffer->addr = (buffer->addr + 3) & ~3;
  CharShadowMtl = Unk00727dd0(1);
  CharShadowMtl->f54 = 1.0f;
  CharShadowMtl->f58 = 1.0f;
  CharShadowMtl->f5c = 1.0f;
  CharShadowMtl->attrib12 = 2;
  CharShadowMtl->z_mode = 1;
  CharShadowMtl->f70 = 0.999f;
  CharShadowMtl->filter_mode = 1;
  CharShadowMtl->attrib20 = 1;
  CharShadowMtl->w76 = 0xff;
  CharShadowMtl->attrib16 = 2;
  CharShadowMtl->tid = Unk006e6060("stuff\\gradient", buffer, buffer_end);
  CharShadowMtl->flags270 |= 0x100000;
  CharShadowMtl->flags270 |= 0x4000;
  NuMtlUpdate(CharShadowMtl);
}

// FUNCTION: LEGOBATMAN 0x0062db70
void Mission_Clear(MISSIONSYS *ms) {
  if (ms == 0) {
    ms = MissionSys;
    if (ms == 0) {
      return;
    }
  }
  ms->mission = 0;
  ms->active = 0;
  ResetTimer((TIMER_s *)ms->timer, 0.0f);
}

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

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_unk_0062bf50(f32 *v, f32 a, i32 i) {
  v[0] = NuSinApprox(i);
  NuVec4Set(v, a, a, a, a);
}
