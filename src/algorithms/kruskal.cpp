#include "kruskal.h"
#include <algorithm>
#include <numeric>
#include <random>

std::vector<std::pair<int, int>> Kruskal::MinimumSpanningTree(
    int vertices, const Graph& adjacencylist) {
  std::vector<std::pair<int, int>> edges;
  for (int i = 0; i < vertices; ++i) {
    for (const auto& edge : adjacencylist[i]) {
      if (edge.first > i) edges.push_back({i, edge.first});
    }
  }
  shuffle(edges.begin(), edges.end(), generator);

  parent_ = std::vector<int>(vertices);
  std::iota(parent_.begin(), parent_.end(), 0);

  minimumspanningtree.clear();
  for (const auto& edge : edges) {
    int u = GetParent(edge.first), v = GetParent(edge.second);
    if (u == v) continue;
    parent_[u] = v;
    minimumspanningtree.push_back(edge);
  }
  return minimumspanningtree;
}

int Kruskal::GetParent(int u) {
  return (parent_[u] == u) ? u : (parent_[u] = GetParent(parent_[u]));
}

