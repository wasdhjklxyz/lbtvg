// adapted from saga nu2api/nucore/nulist.h
#pragma once

typedef struct nulistlnk_s {
  struct nulistlnk_s *next;
  struct nulistlnk_s *prev;
} NULISTLNK;

typedef struct nulisthdr_s {
  NULISTLNK *head;
  NULISTLNK *tail;
} NULISTHDR;

#ifdef __cplusplus
extern "C" {
#endif

NULISTLNK *NuListGetHead(NULISTHDR *list);
NULISTLNK *NuListGetTail(NULISTHDR *list);
NULISTLNK *NuListGetNext(NULISTHDR *list, NULISTLNK *node);

// WARN: Some function names do not match the mac 1.0.1 names i think
// void NuLinkedListAppend(NULISTHDR *list, NULISTLNK *node);
// int NuLinkedListCheck(NULISTHDR *list);
// void NuLinkedListInsert(NULISTHDR *list, NULISTLNK *node);
// void NuLinkedListInsertBefore(NULISTHDR *list, NULISTLNK *position,
//                               NULISTLNK *node);
// void NuLinkedListInsertAfter(NULISTHDR *list, NULISTLNK *position,
//                              NULISTLNK *node);
// void NuLinkedListRemove(NULISTHDR *list, NULISTLNK *node);
//
// NULISTLNK *NuLinkedListGetHead(NULISTHDR *list);
// NULISTLNK *NuLinkedListGetTail(NULISTHDR *list);
//
// NULISTLNK *NuLinkedListGetPrev(NULISTHDR *list, NULISTLNK *node);
// NULISTLNK *NuLinkedListGetNext(NULISTHDR *list, NULISTLNK *node);

#ifdef __cplusplus
}
#endif
