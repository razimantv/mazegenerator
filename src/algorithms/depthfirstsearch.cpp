#include "depthfirstsearch.h"
#include <algorithm>

std::vector<std::pair<int, int>> DepthFirstSearch::SpanningTree(
    int vertices, const Graph& adjacencylist) {
  spanningtree.clear();
  visited = std::vector<bool>(vertices, 0);
  DFS(std::uniform_int_distribution<int>(0, vertices - 1)(generator),
      adjacencylist);
  return spanningtree;
}

void DepthFirstSearch::DFS(int vertex, const Graph& adjacencylist) {
  visited[vertex] = 1;
  std::vector<int> nodeorder(adjacencylist[vertex].size());
  std::iota(nodeorder.begin(), nodeorder.end(), 0);
  shuffle(nodeorder.begin(), nodeorder.end(), generator);

  for (auto index : nodeorder) {
    int nextvertex = adjacencylist[vertex][index].first;
    if (nextvertex < 0 or visited[nextvertex]) continue;
    spanningtree.push_back({vertex, nextvertex});
    DFS(nextvertex, adjacencylist);
  }
}
