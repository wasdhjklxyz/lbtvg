// gameapi/edtools, file unknown (just before edspecial.cpp's anchor at
// 0x79e570): EdRefSpecialObject (saga edtoolsall.cpp).

#include "../../nu2api/nucore/common.h"
#include "edref_unk.h"

struct nuhspecial_s {
  void *scene;
  void *special;
  void *display_special;
};

// saga: SpecialObject; only the special is evidenced.
struct EDSPECIALOBJECT_s {
  u8 pad00[0x18];
  nuhspecial_s special; // 0x18
};

i32 NuSpecialGetCollision(nuhspecial_s *sp);
i32 NuSpecialGetVisibilityFn(nuhspecial_s *sp);
void NuSpecialSetCollision(nuhspecial_s *sp, i32 on);
void NuSpecialSetVisibility(nuhspecial_s *sp, i32 on);

struct EdRefSpecialObject : EdRef {
  void GetMemberData(void *object, i32 type, void *data, i32 size);
  void SetMemberData(void *object, i32 type, void *data, i32 size,
                     i16 *changed);
};

// FUNCTION: LEGOBATMAN 0x0079e2d0
void EdRefSpecialObject::GetMemberData(void *object, i32 type, void *data,
                                       i32 size) {
  EDSPECIALOBJECT_s *special_object = (EDSPECIALOBJECT_s *)object;
  CheckType(type);
  switch (member_offset) {
  case (i32)0x80000009:
    *(nuhspecial_s *)data = special_object->special;
    break;
  case (i32)0x8000000a:
    *(i32 *)data = NuSpecialGetVisibilityFn(&special_object->special);
    break;
  case (i32)0x8000000b:
    *(i32 *)data = NuSpecialGetCollision(&special_object->special);
    break;
  }
}

// FUNCTION: LEGOBATMAN 0x0079e350
void EdRefSpecialObject::SetMemberData(void *object, i32 type, void *data,
                                       i32 size, i16 *changed) {
  EDSPECIALOBJECT_s *special_object = (EDSPECIALOBJECT_s *)object;
  CheckType(type);
  switch (member_offset) {
  case (i32)0x80000009:
    special_object->special = *(nuhspecial_s *)data;
    break;
  case (i32)0x8000000a:
    NuSpecialSetVisibility(&special_object->special, *(i32 *)data);
    break;
  case (i32)0x8000000b:
    NuSpecialSetCollision(&special_object->special, *(i32 *)data);
    break;
  }
}
