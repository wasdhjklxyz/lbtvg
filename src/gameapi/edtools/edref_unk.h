// gameapi/edtools: EdRef (saga gameapi_edtools_types.h), shared by the
// EdRef* files.

#pragma once

#include "../../nu2api/nucore/common.h"

struct EdControl;

struct EdRef {
  virtual void *GetMemberObject(void *object);
  virtual void GetMemberData(void *object, i32 type, void *data, i32 size);
  virtual void SetMemberData(void *object, i32 type, void *data, i32 size,
                             i16 *changed);

  void CheckType(i32 requested_type);
  i32 GetTypeSize(i32 requested_type, i32 data_size);

  EdRef *next;           // 0x04
  EdRef *previous;       // 0x08
  i32 type_id;           // 0x0c
  char *name;            // 0x10
  i32 member_offset;     // 0x14
  i32 size;              // 0x18
  i32 attributes;        // 0x1c
  EdControl *control;    // 0x20
  i32 replication_group; // 0x24
};
