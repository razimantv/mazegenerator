#ifndef LOOPERASEDRANDOMWALK_H
#define LOOPERASEDRANDOMWALK_H

#include "spanningtreealgorithm.h"
#include <vector>

class LoopErasedRandomWalk : public SpanningtreeAlgorithm {
 public:
  std::vector<std::pair<int, int>> SpanningTree(int, const Graph &);

 private:
  std::vector<int> visited;
  void LERW(int, int, const Graph &);
};

#endif /* end of include guard: LOOPERASEDRANDOMWALK_H */
