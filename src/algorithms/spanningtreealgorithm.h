#ifndef SPANNINGTREEALGORITHM_H
#define SPANNINGTREEALGORITHM_H

#include "cellborder.h"
#include <memory>
#include <random>
#include <vector>

typedef std::tuple<int, std::shared_ptr<CellBorder>> Edge;
typedef std::vector<std::vector<Edge>> Graph;

class SpanningtreeAlgorithm {
 public:
  SpanningtreeAlgorithm();
  virtual std::vector<std::pair<int, int>> SpanningTree(int, const Graph&) = 0;

 protected:
  std::vector<std::pair<int, int>> spanningtree;
  std::random_device randomdevice;
  std::mt19937 generator;
};

#endif /* end of include guard: SPANNINGTREEALGORITHM_H */
