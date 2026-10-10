// gameapi/edtools, file unknown: EdRef::GetMemberObject (saga
// edtoolsall.cpp), between edstring.cpp and edspecial.cpp.

#include "../../nu2api/nucore/common.h"
#include "edref_unk.h"

// FUNCTION: LEGOBATMAN 0x007994c0
void *EdRef::GetMemberObject(void *object) {
  void *member = (u8 *)object + member_offset;
  if (member != 0) {
    if (attributes & 0x40000000)
      return *(void **)member;
    return member;
  }
  return 0;
}
