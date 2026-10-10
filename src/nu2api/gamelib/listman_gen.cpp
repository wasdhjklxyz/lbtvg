#include "../numath/nuinline_unk.h"
#include <stddef.h>

// Header statics: this TU's copies (bodies in nuinline_unk.h/nutrig_unk.h).
// FUNCTION: LEGOBATMAN 0x0067dce0
static void NuVec4Set(f32 *v, f32 x, f32 y, f32 z, f32 w);
// nu2api/gamelib/listman_gen.cpp: __FILE__ anchor at 0x0067dd00 (NuLstCreate).

// List node header is 16 bytes; user data follows it.
struct nulst_s;

struct nulstnode_s {
  nulst_s *owner;
  nulstnode_s *next;
  nulstnode_s *prev;
  unsigned short id;       // 0xc
  unsigned short used : 1; // 0xe
};

struct nulst_s {
  nulstnode_s *free;                 // 0x0
  nulstnode_s *free_tail;            // 0x4
  nulstnode_s *head;                 // 0x8
  nulstnode_s *tail;                 // 0xc
  unsigned short element_count;      // 0x10
  unsigned short element_size;       // 0x12
  unsigned short element_size_total; // 0x14
  short used_count;                  // 0x16
  int safe_thread;                   // 0x18
  unsigned char pad1c[4];
};

extern "C" void *NuMemAllocFn(int size, const char *file, int line);
extern "C" void NuMemFreeFn(void *p, const char *file, int line);

// GLOBAL: LEGOBATMAN 0x009d5824
extern int nu_current_thread_id;

// FUNCTION: LEGOBATMAN 0x0067dd00
nulst_s *NuLstCreate(int element_count, int element_size) {
  nulst_s *list;
  int element_size_total;
  int i;
  char *next;
  nulstnode_s *curr;

  element_size_total = element_size + sizeof(nulstnode_s);
  list = (nulst_s *)NuMemAllocFn(
      element_count * element_size_total + sizeof(nulst_s), __FILE__, 0x74);

  if (list != 0) {
    list->free = (nulstnode_s *)(list + 1);
    list->head = 0;
    list->tail = 0;

    list->element_count = element_count;
    list->element_size = element_size;
    list->element_size_total = element_size_total;
    list->used_count = 0;

    curr = list->free;
    next = (char *)curr + element_size_total;
    for (i = 1; i < element_count; i++) {
      curr->next = (nulstnode_s *)next;
      curr->id = i - 1;
      curr->owner = list;
      curr = (nulstnode_s *)next;
      next = next + element_size_total;
    }

    curr->next = 0;
    list->free_tail = curr;
    curr->id = i - 1;
    curr->owner = list;
    list->safe_thread = nu_current_thread_id;
  }

  return list;
}

// FUNCTION: LEGOBATMAN 0x0067dd90
nulst_s *NuLstCreateBuff(int count, int size, VARIPTR *buffer, VARIPTR end,
                         int alignment) {
  nulst_s *list = NULL;
  int i;
  unsigned int stride;
  unsigned int bytes;
  nulstnode_s *first_free;
  VARIPTR next;
  VARIPTR node;

  buffer->addr = (buffer->addr + alignment - 1) & ~(alignment - 1);
  stride = (size + sizeof(nulstnode_s) + alignment - 1) & -alignment;
  bytes = count * stride + sizeof(nulst_s);
  if (bytes < end.addr - buffer->addr) {
    list = (nulst_s *)buffer->void_ptr;
    buffer->addr += bytes;
    list->free = (nulstnode_s *)(list + 1);
    list->head = NULL;
    list->tail = NULL;
    list->element_count = count;
    list->element_size = size;
    list->element_size_total = stride;
    list->used_count = 0;
    first_free = list->free;
    node.void_ptr = first_free;
    next.char_ptr = node.char_ptr + stride;
    for (i = 1; i < count; ++i) {
      ((nulstnode_s *)node.void_ptr)->next = (nulstnode_s *)next.void_ptr;
      ((nulstnode_s *)node.void_ptr)->id = i - 1;
      ((nulstnode_s *)node.void_ptr)->owner = list;
      node.void_ptr = next.void_ptr;
      next.char_ptr = next.char_ptr + stride;
    }
    ((nulstnode_s *)node.void_ptr)->next = NULL;
    list->free_tail = (nulstnode_s *)node.void_ptr;
    ((nulstnode_s *)node.void_ptr)->id = i - 1;
    ((nulstnode_s *)node.void_ptr)->owner = list;
    list->safe_thread = nu_current_thread_id;
  }
  return list;
}

// FUNCTION: LEGOBATMAN 0x0067de40
void NuLstDestroy(nulst_s *list) { NuMemFreeFn(list, __FILE__, 0xf5); }

// FUNCTION: LEGOBATMAN 0x0067de60
nulstnode_s *NuLstAllocHead(nulst_s *list) {
  nulstnode_s *node;

  if (list->free != NULL) {
    node = list->free;
    list->free = list->free->next;
    if (list->free == NULL)
      list->free_tail = NULL;
    else
      list->free->prev = NULL;
    node->next = list->head;
    if (list->head != NULL)
      list->head->prev = node;
    else
      list->tail = node;
    node->prev = NULL;
    list->used_count++;
    list->head = node;
    node->used = 1;
    return node + 1;
  }
  return NULL;
}

// FUNCTION: LEGOBATMAN 0x0067dec0
nulstnode_s *NuLstAllocTail(nulst_s *list) {
  nulstnode_s *node;

  if (list->free != NULL) {
    node = list->free;
    list->free = list->free->next;
    if (list->free == NULL)
      list->free_tail = NULL;
    else
      list->free->prev = NULL;
    node->prev = list->tail;
    if (list->tail != NULL)
      list->tail->next = node;
    else
      list->head = node;
    node->next = NULL;
    list->tail = node;
    node->used = 1;
    list->used_count++;
    return node + 1;
  }
  return NULL;
}

// FUNCTION: LEGOBATMAN 0x0067df20
nulstnode_s *NuLstAllocFree(nulst_s *list) {
  nulstnode_s *node;

  if (list->free != NULL) {
    node = list->free;
    list->free = list->free->next;
    if (list->free == NULL)
      list->free_tail = NULL;
    else
      list->free->prev = NULL;
    node->used = 1;
    node->prev = NULL;
    list->used_count++;
    return node + 1;
  }
  return NULL;
}

// FUNCTION: LEGOBATMAN 0x0067df60
void NuLstAtachHead(nulst_s *list, nulstnode_s *node) {
  node->next = list->head;
  node->prev = NULL;
  if (list->head != NULL)
    list->head->prev = node;
  else
    list->tail = node;
  list->head = node;
}

// FUNCTION: LEGOBATMAN 0x0067df90
void NuLstAttachTail(nulst_s *list, nulstnode_s *node) {
  node->prev = list->tail;
  node->next = NULL;
  if (list->tail != NULL)
    list->tail->next = node;
  else
    list->head = node;
  list->tail = node;
}

// FUNCTION: LEGOBATMAN 0x0067dfc0
nulstnode_s *NuLstAllocBefore(nulstnode_s *anchor) {
  nulst_s *list;
  nulstnode_s *node;

  --anchor;
  list = anchor->owner;
  if (list->free != NULL) {
    node = list->free;
    list->free = list->free->next;
    if (list->free == NULL)
      list->free_tail = NULL;
    else
      list->free->prev = NULL;
    node->next = anchor;
    node->prev = anchor->prev;
    anchor->prev = node;
    if (node->prev != NULL)
      node->prev->next = node;
    else
      list->head = node;
    node->used = 1;
    list->used_count++;
    return node + 1;
  }
  return NULL;
}

// FUNCTION: LEGOBATMAN 0x0067e020
nulstnode_s *NuLstAllocAfter(nulstnode_s *anchor) {
  nulst_s *list;
  nulstnode_s *node;

  --anchor;
  list = anchor->owner;
  if (list->free != NULL) {
    node = list->free;
    list->free = list->free->next;
    if (list->free == NULL)
      list->free_tail = NULL;
    else
      list->free->prev = NULL;
    node->prev = anchor;
    node->next = anchor->next;
    if (node->next != NULL)
      node->next->prev = node;
    else
      list->tail = node;
    node->used = 1;
    list->used_count++;
    return node + 1;
  }
  return NULL;
}

// FUNCTION: LEGOBATMAN 0x0067e080
void NuLstFree(nulstnode_s *node) {
  nulst_s *list;

  --node;
  list = node->owner;
  if (node->next != NULL)
    node->next->prev = node->prev;
  else
    list->tail = node->prev;
  if (node->prev != NULL)
    node->prev->next = node->next;
  else
    list->head = node->next;
  node->prev = list->free_tail;
  node->next = NULL;
  if (list->free_tail != NULL)
    list->free_tail->next = node;
  else
    list->free = list->free_tail = node;
  list->free_tail = node;
  list->used_count--;
  node->used = 0;
}

// FUNCTION: LEGOBATMAN 0x0067e0f0
nulstnode_s *NuLstGetByIdx(nulst_s *list, int index) {
  nulstnode_s *node = (nulstnode_s *)((unsigned char *)(list + 1) +
                                      (short)list->element_size_total * index);
  return node->used ? node + 1 : NULL;
}

// Body from ref/saga/src/nu2api/nucore/nulst.cpp (NuLstGetNext).
// FUNCTION: LEGOBATMAN 0x0067e110
nulstnode_s *NuLstGetNext(nulst_s *list, nulstnode_s *node) {
  if (node) {
    node--;
    if (node->next)
      return node->next + 1;
  } else if (list->head) {
    return list->head + 1;
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x0067e140
nulstnode_s *NuLstGetPrev(nulst_s *list, nulstnode_s *node) {
  if (node) {
    node--;
    if (node->prev)
      return node->prev + 1;
  } else if (list->tail) {
    return list->tail + 1;
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x0067e170
int NuLstMoveNext(nulst_s *list, nulstnode_s *node) {
  nulstnode_s *next;

  --node;
  if (node->next != NULL) {
    if (node->prev != NULL)
      node->prev->next = node->next;
    else
      list->head = node->next;
    node->next->prev = node->prev;
    if (node->next->next != NULL)
      node->next->next->prev = node;
    else
      list->tail = node;
    node->prev = node->next;
    next = node->next;
    node->next = node->next->next;
    next->next = node;
    return 1;
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x0067e1d0
int NuLstMovePrev(nulst_s *list, nulstnode_s *node) {
  nulstnode_s *prev;

  --node;
  if (node->prev != NULL) {
    if (node->next != NULL)
      node->next->prev = node->prev;
    else
      list->tail = node->prev;
    node->prev->next = node->next;
    if (node->prev->prev != NULL)
      node->prev->prev->next = node;
    else
      list->head = node;
    prev = node->prev;
    node->prev = node->prev->prev;
    node->next = prev;
    prev->prev = node;
    return 1;
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x0067e230
nulstnode_s *NuLstGetFree(nulst_s *list) { return list->free + 1; }

// FUNCTION: LEGOBATMAN 0x0067e240
nulstnode_s *NuLstAlloc(nulst_s *list) { return NuLstAllocHead(list); }

// Keeps the header-static copies above alive until their real callers are
// matched.
void Unk_InlineUser_listman_gen(f32 *v, f32 a, i32 i) {
  NuVec4Set(v, a, a, a, a);
}
