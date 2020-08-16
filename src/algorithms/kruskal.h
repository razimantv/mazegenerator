#ifndef KRUSKAL_H
#define KRUSKAL_H

#include <vector>
#include "minimumspanningtreealgorithm.h"

class Kruskal : public MinimumSpanningtreeAlgorithm {
 public:
  std::vector<std::pair<int, int>> MinimumSpanningTree(int, const Graph&);

 private:
  std::vector<int> parent_;

  int GetParent(int);
};

#endif /* end of include guard: KRUSKAL_H */
