// gameapi/, file unknown: BaseThing (RTTI .?AVBaseThing@@, vtable 0x86855c)
// and ThingManager (vtable 0x868590), 0x5912b0..0x591990. BaseThing slot
// names follow the Mac emission order (RemoveDependancies .. Effects), which
// the ThingManager loops confirm (ResetThings calls slot 5, ProcessThings
// slots 6..8, RenderThings 9, DisplayThings 10, EffectsThings 11).

#include "../nu2api/nucore/common.h"

struct ThingRemoveData;
struct ThingLevelData;
struct ThingResetData;
struct ThingProcessData;
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

  u32 u04;   // 0x04
  u32 flags; // 0x08, bit 0 no RemoveDependancies, 1 no EnterLevel, 2 no
             // ExitLevel
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

  BaseThing *things[0x40]; // 0x004
  i32 count;               // 0x104
  i32 i108;                // 0x108
  i32 pending;             // 0x10c
};

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

// FUNCTION: LEGOBATMAN 0x00591530
i32 ThingManager::RemoveDependanciesThings(ThingRemoveData *data) {
  i32 ok = 1;
  for (i32 i = 0; i < count; i++) {
    if (things[i] != 0 && (~things[i]->flags & 1))
      ok &= things[i]->RemoveDependancies(data);
  }
  return ok;
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
