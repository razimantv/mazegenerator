#ifndef PRIM_H
#define PRIM_H

#include "spanningtreealgorithm.h"
#include <vector>

class Prim : public SpanningtreeAlgorithm {
 public:
  std::vector<std::pair<int, int>> SpanningTree(int, const Graph &);

 private:
  void PrimAlgorithm(int, const Graph &);
};

#endif /* end of include guard: PRIM_H */
