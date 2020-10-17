#ifndef KRUSKAL_H
#define KRUSKAL_H

#include "spanningtreealgorithm.h"
#include <vector>

class Kruskal : public SpanningtreeAlgorithm {
 public:
  std::vector<std::pair<int, int>> SpanningTree(int, const Graph&);

 private:
  std::vector<int> parent_;

  int GetParent(int);
};

#endif /* end of include guard: KRUSKAL_H */
