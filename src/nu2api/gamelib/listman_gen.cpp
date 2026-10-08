// nu2api/gamelib/listman_gen.cpp: __FILE__ anchor at 0x0067dd00 (NuLstCreate).

// List node header is 16 bytes; user data follows it.
struct nulstnode_s {
  int f0;
  nulstnode_s *next;
  unsigned char pad[0x10 - 8];
};

struct nulst_s {
  int f0;
  int f4;
  nulstnode_s *head;
};

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
