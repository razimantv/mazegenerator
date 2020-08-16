#ifndef LOOPERASEDRANDOMWALK_H
#define LOOPERASEDRANDOMWALK_H

#include <vector>
#include "minimumspanningtreealgorithm.h"

class LoopErasedRandomWalk: public MinimumSpanningtreeAlgorithm {
 public:
  std::vector<std::pair<int, int>> MinimumSpanningTree(int, const Graph &);

 private:
  std::vector<int> visited;
  void LERW(int, int, const Graph &);
};

#endif /* end of include guard: LOOPERASEDRANDOMWALK_H */
