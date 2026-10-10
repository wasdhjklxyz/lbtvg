// gameapi/edtools, file unknown: EdRef (saga edtoolsall.cpp), between
// edstring.cpp and edspecial.cpp.

#include "../../nu2api/nucore/common.h"
#include "edref_unk.h"

void *NuMemCpy(void *dst, const void *src, u32 size);

// saga: EdType (0x10 bytes here).
struct EdType {
  char *name; // 0x0
  i32 size;   // 0x4
  u8 pad8[0x10 - 8];
};

// theRegistry's type table.
// GLOBAL: LEGOBATMAN 0x009a9308
extern EdType *g_unk009a9308;
// GLOBAL: LEGOBATMAN 0x009a9328
extern i32 g_unk009a9328;
// GLOBAL: LEGOBATMAN 0x02a125fc
extern i32 g_unk02a125fc; // a type id accepted for g_unk02a12600 (saga: VuVec)
// GLOBAL: LEGOBATMAN 0x02a12600
extern i32 g_unk02a12600; // saga: EdType_NuVec

i32 Unk007958e0(char *a, ...); // empty in the release build

static inline EdType *GetType(i32 id) {
  if (id >= 0 && id < g_unk009a9328)
    return &g_unk009a9308[id];
  return 0;
}

// FUNCTION: LEGOBATMAN 0x0079d510
void EdRef::CheckType(i32 requested_type) {
  if (requested_type == type_id ||
      (type_id == g_unk02a12600 && requested_type == g_unk02a125fc))
    return;
  EdType *requested = GetType(requested_type);
  EdType *own = GetType(type_id);
  Unk007958e0(requested->name, own->name);
}

// FUNCTION: LEGOBATMAN 0x0079d580
i32 EdRef::GetTypeSize(i32 requested_type, i32 data_size) {
  CheckType(requested_type);
  i32 type_size = size;
  if (type_size <= 0)
    type_size = GetType(type_id)->size;
  if (data_size > 0 && type_size > data_size)
    type_size = Unk007958e0(GetType(type_id)->name, type_size);
  return type_size;
}

// FUNCTION: LEGOBATMAN 0x0079d600
void EdRef::GetMemberData(void *object, i32 requested_type, void *data,
                          i32 data_size) {
  void *member = (u8 *)object + member_offset;
  i32 type_size = GetTypeSize(requested_type, data_size);
  if (member != 0) {
    if (attributes & 0x40000000)
      member = *(void **)member;
    NuMemCpy(data, member, type_size);
  }
}

// FUNCTION: LEGOBATMAN 0x0079d640
void EdRef::SetMemberData(void *object, i32 requested_type, void *data,
                          i32 data_size, i16 *changed) {
  void *member = (u8 *)object + member_offset;
  i32 type_size = GetTypeSize(requested_type, data_size);
  if (member != 0) {
    if (attributes & 0x40000000)
      member = *(void **)member;
    NuMemCpy(member, data, type_size);
  }
}
