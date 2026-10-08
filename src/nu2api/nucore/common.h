#pragma once
// Basic typedefs after ref/saga/src/nu2api/nucore/common.h (VC8 has no
// stdint.h).

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed char i8;
typedef short i16;
typedef int i32;
typedef float f32;
typedef double f64;

typedef union variptr_u {
  void *void_ptr;
  char *char_ptr;
  i16 *i16_ptr;
  u8 *u8_ptr;
  u32 *u32_ptr;
  f32 *f32_ptr;
  struct numtx_s *mtx_ptr;
  unsigned int addr;
} VARIPTR;
