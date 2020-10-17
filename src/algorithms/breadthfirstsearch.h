#ifndef BREADTHFIRSTSEARCH_H
#define BREADTHFIRSTSEARCH_H

#include "spanningtreealgorithm.h"
#include <vector>

class BreadthFirstSearch : public SpanningtreeAlgorithm {
 public:
  std::vector<std::pair<int, int>> SpanningTree(int, const Graph&);

 private:
  std::vector<bool> visited;
  std::vector<int> currentlevel, nextlevel;
};

#endif /* end of include guard: BREADTHFIRSTSEARCH_H */
