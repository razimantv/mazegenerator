#ifndef MINIMUMSPANNINGTREEALGORITHM_H
#define MINIMUMSPANNINGTREEALGORITHM_H

#include <memory>
#include <random>
#include <vector>
#include "cellborder.h"

typedef std::vector<std::vector<std::pair<int, std::shared_ptr<CellBorder>>>>
    Graph;

class MinimumSpanningtreeAlgorithm {
 public:
  MinimumSpanningtreeAlgorithm();
  virtual std::vector<std::pair<int, int>> MinimumSpanningTree(
      int, const Graph&) = 0;

 protected:
  std::vector<std::pair<int, int>> minimumspanningtree;
  std::random_device randomdevice;
  std::mt19937 generator;
};

#endif /* end of include guard: MINIMUMSPANNINGTREEALGORITHM_H */
