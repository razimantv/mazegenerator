#ifndef PRIM_H
#define PRIM_H

#include <vector>
#include "minimumspanningtreealgorithm.h"

class Prim: public MinimumSpanningtreeAlgorithm {
 public:
  std::vector<std::pair<int, int>> MinimumSpanningTree(int, const Graph &);

 private:
  void PrimAlgorithm(int, const Graph &);
};

#endif /* end of include guard: PRIM_H */
