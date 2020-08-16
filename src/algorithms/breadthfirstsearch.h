#ifndef BREADTHFIRSTSEARCH_H
#define BREADTHFIRSTSEARCH_H

#include <vector>
#include "minimumspanningtreealgorithm.h"

class BreadthFirstSearch : public MinimumSpanningtreeAlgorithm {
 public:
  std::vector<std::pair<int, int>> MinimumSpanningTree(int, const Graph&);

 private:
  std::vector<bool> visited;
  std::vector<int> currentlevel, nextlevel;
};

#endif /* end of include guard: BREADTHFIRSTSEARCH_H */
