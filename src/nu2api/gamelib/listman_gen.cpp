// nu2api/gamelib/listman_gen.cpp: __FILE__ anchor at 0x0067dd00 (NuLstCreate).

// List node header is 16 bytes; user data follows it.
struct nulst_s;

struct nulstnode_s {
  nulst_s *owner;
  nulstnode_s *next;
  nulstnode_s *prev;
  unsigned short id; // 0xc
  unsigned short flags;
};

struct nulst_s {
  nulstnode_s *free;                 // 0x0
  nulstnode_s *free_tail;            // 0x4
  nulstnode_s *head;                 // 0x8
  nulstnode_s *tail;                 // 0xc
  unsigned short element_count;      // 0x10
  unsigned short element_size;       // 0x12
  unsigned short element_size_total; // 0x14
  unsigned short used_count;         // 0x16
  int safe_thread;                   // 0x18
  unsigned char pad1c[4];
};

extern "C" void *NuMemAllocFn(int size, const char *file, int line);

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
