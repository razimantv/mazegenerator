#include "prim.h"
#include <algorithm>

std::vector<std::pair<int, int>> Prim::SpanningTree(
    int vertices, const Graph& adjacencylist) {
  spanningtree.clear();

  PrimAlgorithm(vertices, adjacencylist);
  return spanningtree;
}

void Prim::PrimAlgorithm(int vertices, const Graph& adjacencylist) {
  std::vector<bool> visited(vertices, false);
  std::vector<std::pair<int, int>> boundary;
  int vertex = std::uniform_int_distribution<int>(0, vertices - 1)(generator);

  for (int i = 1; i < vertices; ++i) {
    visited[vertex] = true;
    for (auto p : adjacencylist[vertex]) {
      if (p.first != -1 and !visited[p.first])
        boundary.push_back({vertex, p.first});
    }

    std::pair<int, int> nextedge = {-1, -1};
    do {
      int index =
          std::uniform_int_distribution<int>(0, boundary.size() - 1)(generator);
      std::swap(boundary[index], boundary.back());
      if (!visited[boundary.back().second]) nextedge = boundary.back();
      boundary.pop_back();
    } while (nextedge.first == -1);

    spanningtree.push_back(nextedge);
    vertex = nextedge.second;
  }
}
