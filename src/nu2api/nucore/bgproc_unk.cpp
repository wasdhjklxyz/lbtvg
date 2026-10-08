// nu2api/nucore/bgproc_unk.cpp: background process list, between
// nufile_gen.cpp (0x006e0830) and nufile_pc.cpp (0x006e3430).

struct nulst_s;
struct nulstnode_s;
nulstnode_s *NuLstGetNext(nulst_s *list, nulstnode_s *node);

// GLOBAL: LEGOBATMAN 0x00b03c68
nulst_s *bgProcList;
// GLOBAL: LEGOBATMAN 0x00b058c0
nulstnode_s *bgProcActive;

// FUNCTION: LEGOBATMAN 0x006e2af0
nulstnode_s *bgGetProcActive(void) {
  if (bgProcActive)
    return bgProcActive;
  return NuLstGetNext(bgProcList, bgProcActive);
}
