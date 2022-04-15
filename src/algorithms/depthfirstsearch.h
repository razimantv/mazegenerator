#ifndef DEPTHFIRSTSEARCH_H
#define DEPTHFIRSTSEARCH_H

#include "spanningtreealgorithm.h"
#include <vector>

class DepthFirstSearch : public SpanningtreeAlgorithm {
 public:
  std::vector<std::pair<int, int>> SpanningTree(int, const Graph &);
  std::vector<int> Solve(int, const Graph&, int);

 private:
  std::vector<int> parent;
  void DFS(int, const Graph &);
};

#endif /* end of include guard: DEPTHFIRSTSEARCH_H */
