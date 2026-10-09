#pragma once
// Prototypes after ref/saga/src/nu2api/nucore/nustring.h.

#include "common.h"

i32 NuStrICmp(const char *a, const char *b);
char *NuStrIStr(char *str, char *sub);
i32 NuStrLen(const char *str);
i32 NuStrCpy(char *dst, const char *src);
void NuStrCat(char *str, const char *ext);
i32 NuSPrintf(char *dest, const char *format, ...);
int NuAToI(const char *str);
f32 NuAToF(char *str);
