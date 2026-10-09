// nu2api/nucore/nulist_unk.cpp: intrusive list accessors between gcutscn.cpp
// (0x006ceef0) and nupad_gen.cpp (0x006d5700).
// adapted from saga nu2api/nucore/nulist.cpp

#include "nulist.h"

#include <stddef.h>

extern "C" {

// FUNCTION: LEGOBATMAN 0x006d40d0
int NuListCheck(NULISTHDR *list) {
  int count = 0;
  for (NULISTLNK *node = list->head; node != NULL; node = node->next)
    count++;
  return count;
}

// FUNCTION: LEGOBATMAN 0x006d40f0
void NuListAppend(NULISTHDR *list, NULISTLNK *node) {
  node->next = NULL;
  node->prev = list->tail;
  if (list->tail != NULL)
    list->tail->next = node;
  list->tail = node;
  if (list->head == NULL)
    list->head = node;
}

// FUNCTION: LEGOBATMAN 0x006d4120
void NuListInsert(NULISTHDR *list, NULISTLNK *node) {
  node->next = list->head;
  node->prev = NULL;
  if (list->head != NULL)
    list->head->prev = node;
  if (list->tail == NULL)
    list->tail = node;
  list->head = node;
}

// FUNCTION: LEGOBATMAN 0x006d4150
void NuListInsertBefore(NULISTHDR *list, NULISTLNK *position, NULISTLNK *node) {
  if (position != NULL) {
    node->next = position;
    node->prev = position->prev;
    if (position->prev != NULL)
      position->prev->next = node;
    else
      list->head = node;
    position->prev = node;
  } else {
    NuListInsert(list, node);
  }
}

// FUNCTION: LEGOBATMAN 0x006d41a0
void NuListInsertAfter(NULISTHDR *list, NULISTLNK *position, NULISTLNK *node) {
  if (position != NULL) {
    node->prev = position;
    node->next = position->next;
    if (position->next != NULL)
      position->next->prev = node;
    else
      list->tail = node;
    position->next = node;
  } else {
    NuListAppend(list, node);
  }
}

// FUNCTION: LEGOBATMAN 0x006d41f0
void NuListRemove(NULISTHDR *list, NULISTLNK *node) {
  if (node->next != NULL)
    node->next->prev = node->prev;
  else
    list->tail = node->prev;
  if (node->prev != NULL)
    node->prev->next = node->next;
  else
    list->head = node->next;
}
}

// FUNCTION: LEGOBATMAN 0x006d4220
NULISTLNK *NuListGetHead(NULISTHDR *list) { return list->head; }

// FUNCTION: LEGOBATMAN 0x006d4230
NULISTLNK *NuListGetTail(NULISTHDR *list) { return list->tail; }

// FUNCTION: LEGOBATMAN 0x006d4240
NULISTLNK *NuListGetNext(NULISTHDR *list, NULISTLNK *node) {
  if (node)
    return node->next;
  return list->head;
}

// FUNCTION: LEGOBATMAN 0x006d4250
extern "C" NULISTLNK *NuListGetPrev(NULISTHDR *list, NULISTLNK *node) {
  if (node)
    return node->prev;
  return list->tail;
}

// WARN: Some functions do not match the mac 1.0.1 names to saga perfectly
// int NuLinkedListCheck(NULISTHDR *list) {
//   NULISTLNK *node;
//   int count;
//   NULISTLNK *previous = NULL;
//   count = 0;
//   node = list->head;
//   while (node != NULL) {
//     count++;
//     previous = node;
//     node = node->next;
//   }
//   return count;
// }
//
// void NuLinkedListAppend(NULISTHDR *list, NULISTLNK *node) {
//   node->next = NULL;
//   node->prev = list->tail;
//
//   if (list->tail != NULL) {
//     list->tail->next = node;
//   }
//
//   list->tail = node;
//
//   if (list->head == NULL) {
//     list->head = node;
//   }
// }
//
// void NuLinkedListInsert(NULISTHDR *list, NULISTLNK *node) {
//   node->next = list->head;
//   node->prev = NULL;
//
//   if (list->head != NULL) {
//     list->head->prev = node;
//   }
//
//   if (list->tail == NULL) {
//     list->tail = node;
//   }
//
//   list->head = node;
// }
//
// void NuLinkedListInsertBefore(NULISTHDR *list, NULISTLNK *position,
//                               NULISTLNK *node) {
//   if (position != NULL) {
//     node->next = position;
//     node->prev = position->prev;
//     if (position->prev != NULL) {
//       position->prev->next = node;
//     } else {
//       list->head = node;
//     }
//     position->prev = node;
//   } else {
//     NuLinkedListInsert(list, node);
//   }
// }
//
// void NuLinkedListInsertAfter(NULISTHDR *list, NULISTLNK *position,
//                              NULISTLNK *node) {
//   if (position != NULL) {
//     node->prev = position;
//     node->next = position->next;
//     if (position->next != NULL) {
//       position->next->prev = node;
//     } else {
//       list->tail = node;
//     }
//     position->next = node;
//   } else {
//     NuLinkedListAppend(list, node);
//   }
// }
//
// void NuLinkedListRemove(NULISTHDR *list, NULISTLNK *node) {
//   if (node->next != NULL) {
//     node->next->prev = node->prev;
//   } else {
//     list->tail = node->prev;
//   }
//
//   if (node->prev != NULL) {
//     node->prev->next = node->next;
//   } else {
//     list->head = node->next;
//   }
// }
//
// NULISTLNK *NuLinkedListGetHead(NULISTHDR *list) { return list->head; }
//
// NULISTLNK *NuLinkedListGetTail(NULISTHDR *list) { return list->tail; }
//
// NULISTLNK *NuLinkedListGetPrev(NULISTHDR *list, NULISTLNK *node) {
//   if (node != NULL) {
//     return node->prev;
//   } else {
//     return list->tail;
//   }
// }
//
// NULISTLNK *NuLinkedListGetNext(NULISTHDR *list, NULISTLNK *node) {
//   if (node != NULL) {
//     return node->next;
//   } else {
//     return list->head;
//   }
// }
