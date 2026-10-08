// nu2api/nucore/nulist_unk.cpp: intrusive list accessors between gcutscn.cpp
// (0x006ceef0) and nupad_gen.cpp (0x006d5700).

struct nulistnode_s {
  nulistnode_s *next;
};

struct nulist_s {
  nulistnode_s *head;
  nulistnode_s *tail;
};

// FUNCTION: LEGOBATMAN 0x006d4230
nulistnode_s *NuListGetTail(nulist_s *list) { return list->tail; }

// FUNCTION: LEGOBATMAN 0x006d4240
nulistnode_s *NuListGetNext(nulist_s *list, nulistnode_s *node) {
  if (node)
    return node->next;
  return list->head;
}
