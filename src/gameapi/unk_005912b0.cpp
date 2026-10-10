// gameapi/, file unknown: BaseThing (RTTI .?AVBaseThing@@, vtable 0x86855c)
// and ThingManager (vtable 0x868590), 0x5912b0..0x591990. BaseThing slot
// names follow the Mac emission order (RemoveDependancies .. Effects), which
// the ThingManager loops confirm (ResetThings calls slot 5, ProcessThings
// slots 6..8, RenderThings 9, DisplayThings 10, EffectsThings 11).

#include "../nu2api/nucore/common.h"

struct ThingRemoveData;
struct ThingLevelData;
struct ThingResetData;
struct ThingProcessData {
  u32 u00;
  i32 paused; // 0x04
};
struct ThingRenderData;

class BaseThing {
public:
  virtual ~BaseThing();                                       // 0
  virtual void Vfn1() = 0;                                    // 1
  virtual i32 RemoveDependancies(ThingRemoveData *data);      // 2
  virtual void EnterLevel(ThingLevelData *data);              // 3
  virtual void ExitLevel(ThingLevelData *data);               // 4
  virtual void Reset(ThingResetData *data);                   // 5
  virtual void Process(ThingProcessData *data);               // 6
  virtual void ProcessEvenWhenPaused(ThingProcessData *data); // 7
  virtual void ProcessOnlyWhenPaused(ThingProcessData *data); // 8
  virtual void Render(ThingRenderData *data);                 // 9
  virtual void Display(ThingRenderData *data);                // 10
  virtual void Effects(ThingRenderData *data);                // 11

  u32 u04; // 0x04
  // 0x08: bit 0 no RemoveDependancies, 1 no EnterLevel, 2 no ExitLevel,
  // 3 no Reset
  u32 flags; // 0x08
  i32 timed; // 0x0c, wrap the calls in timing bars
};

class ThingManager {
public:
  virtual ~ThingManager();
  virtual BaseThing *AddThing(BaseThing *thing);
  virtual BaseThing *AddThingAfterThis(BaseThing *thing);
  virtual void RemoveTemporaryThings();
  virtual i32 RemoveDependanciesThings(ThingRemoveData *data);
  virtual void ResetThings(ThingResetData *data);
  virtual void EnterLevelThings(ThingLevelData *data);
  virtual void ExitLevelThings(ThingLevelData *data);
  virtual void ProcessThings(ThingProcessData *data);
  virtual void RenderThings(ThingRenderData *data);
  virtual void DisplayThings(ThingRenderData *data);
  virtual void EffectsThings(ThingRenderData *data);

  BaseThing *things[0x40]; // 0x004
  i32 count;               // 0x104
  i32 permanent;           // 0x108, things below this survive
  i32 pending;             // 0x10c
  i32 timer;               // 0x110
};

void Unk00717840(i32 timer, i32 colour, const char *label);
void Unk00717880(i32 timer, i32 colour);

// FUNCTION: LEGOBATMAN 0x005912b0
i32 BaseThing::RemoveDependancies(ThingRemoveData *data) { return 1; }

// FUNCTION: LEGOBATMAN 0x005912c0
void BaseThing::EnterLevel(ThingLevelData *data) {}

// FUNCTION: LEGOBATMAN 0x005912d0
void BaseThing::ExitLevel(ThingLevelData *data) {}

// FUNCTION: LEGOBATMAN 0x005912e0
void BaseThing::Reset(ThingResetData *data) {}

// FUNCTION: LEGOBATMAN 0x005912f0
void BaseThing::Process(ThingProcessData *data) {}

// FUNCTION: LEGOBATMAN 0x00591300
void BaseThing::ProcessEvenWhenPaused(ThingProcessData *data) {}

// FUNCTION: LEGOBATMAN 0x00591310
void BaseThing::ProcessOnlyWhenPaused(ThingProcessData *data) {}

// FUNCTION: LEGOBATMAN 0x00591320
void BaseThing::Render(ThingRenderData *data) {}

// FUNCTION: LEGOBATMAN 0x00591330
void BaseThing::Display(ThingRenderData *data) {}

// FUNCTION: LEGOBATMAN 0x00591340
void BaseThing::Effects(ThingRenderData *data) {}

// FUNCTION: LEGOBATMAN 0x005913d0
BaseThing *ThingManager::AddThing(BaseThing *thing) {
  if (thing != 0 && count < 0x40) {
    things[count] = thing;
    count++;
  }
  count += pending;
  pending = 0;
  return thing;
}

// FUNCTION: LEGOBATMAN 0x00591410
BaseThing *ThingManager::AddThingAfterThis(BaseThing *thing) {
  if (thing != 0) {
    pending++;
    i32 index = count + pending;
    if (index < 0x40)
      things[index] = thing;
    return thing;
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x005914c0
void ThingManager::RemoveTemporaryThings() {
  for (i32 i = count - 1; i >= permanent; i--) {
    delete things[i];
    things[i] = 0;
  }
  count = permanent;
}

// FUNCTION: LEGOBATMAN 0x00591530
i32 ThingManager::RemoveDependanciesThings(ThingRemoveData *data) {
  i32 ok = 1;
  for (i32 i = 0; i < count; i++) {
    if (things[i] != 0 && (~things[i]->flags & 1))
      ok &= things[i]->RemoveDependancies(data);
  }
  return ok;
}

// FUNCTION: LEGOBATMAN 0x00591580
void ThingManager::ResetThings(ThingResetData *data) {
  for (i32 i = 0; i < count; i++) {
    if (things[i] != 0 && (~(things[i]->flags >> 3) & 1)) {
      if (things[i]->timed)
        Unk00717840(timer, 4, "Res");
      things[i]->Reset(data);
      if (things[i]->timed)
        Unk00717880(timer, 4);
    }
  }
}

// FUNCTION: LEGOBATMAN 0x00591600
void ThingManager::EnterLevelThings(ThingLevelData *data) {
  for (i32 i = 0; i < count; i++) {
    if (things[i] != 0 && (~(things[i]->flags >> 1) & 1))
      things[i]->EnterLevel(data);
  }
}

// FUNCTION: LEGOBATMAN 0x00591650
void ThingManager::ExitLevelThings(ThingLevelData *data) {
  for (i32 i = 0; i < count; i++) {
    if (things[i] != 0 && (~(things[i]->flags >> 2) & 1))
      things[i]->ExitLevel(data);
  }
}

// FUNCTION: LEGOBATMAN 0x005916a0
void ThingManager::ProcessThings(ThingProcessData *data) {
  i32 i;
  for (i = 0; i < count; i++) {
    if (things[i] != 0 && (~(things[i]->flags >> 5) & 1)) {
      if (things[i]->timed)
        Unk00717840(timer, 0, "PROC");
      things[i]->ProcessEvenWhenPaused(data);
      if (things[i]->timed)
        Unk00717880(timer, 0);
    }
  }
  if (data->paused) {
    for (i = 0; i < count; i++) {
      if (things[i] != 0 && (~(things[i]->flags >> 6) & 1)) {
        if (things[i]->timed)
          Unk00717840(timer, 0, "PROC");
        things[i]->ProcessOnlyWhenPaused(data);
        if (things[i]->timed)
          Unk00717880(timer, 0);
      }
    }
  } else {
    for (i = 0; i < count; i++) {
      if (things[i] != 0 && (~(things[i]->flags >> 4) & 1)) {
        if (things[i]->timed)
          Unk00717840(timer, 0, "PROC");
        things[i]->Process(data);
        if (things[i]->timed)
          Unk00717880(timer, 0);
      }
    }
  }
}

// FUNCTION: LEGOBATMAN 0x00591810
void ThingManager::RenderThings(ThingRenderData *data) {
  for (i32 i = 0; i < count; i++) {
    if (things[i] != 0 && (~(things[i]->flags >> 7) & 1)) {
      if (things[i]->timed)
        Unk00717840(timer, 1, "Rnd");
      things[i]->Render(data);
      if (things[i]->timed)
        Unk00717880(timer, 1);
    }
  }
}

// FUNCTION: LEGOBATMAN 0x00591890
void ThingManager::DisplayThings(ThingRenderData *data) {
  for (i32 i = 0; i < count; i++) {
    if (things[i] != 0 && (~(things[i]->flags >> 8) & 1)) {
      if (things[i]->timed)
        Unk00717840(timer, 3, "Dis");
      things[i]->Display(data);
      if (things[i]->timed)
        Unk00717880(timer, 3);
    }
  }
}

// FUNCTION: LEGOBATMAN 0x00591910
void ThingManager::EffectsThings(ThingRenderData *data) {
  for (i32 i = 0; i < count; i++) {
    if (things[i] != 0 && (~(things[i]->flags >> 9) & 1)) {
      if (things[i]->timed)
        Unk00717840(timer, 5, "Fx");
      things[i]->Effects(data);
      if (things[i]->timed)
        Unk00717880(timer, 5);
    }
  }
}
