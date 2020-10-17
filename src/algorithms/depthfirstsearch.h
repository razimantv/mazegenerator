#ifndef DEPTHFIRSTSEARCH_H
#define DEPTHFIRSTSEARCH_H

#include "spanningtreealgorithm.h"
#include <vector>

class DepthFirstSearch : public SpanningtreeAlgorithm {
 public:
  std::vector<std::pair<int, int>> SpanningTree(int, const Graph &);

 private:
  std::vector<bool> visited;
  void DFS(int, const Graph &);
};

#endif /* end of include guard: DEPTHFIRSTSEARCH_H */
