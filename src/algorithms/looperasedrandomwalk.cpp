#include "looperasedrandomwalk.h"
#include <algorithm>

std::vector<std::pair<int, int>> LoopErasedRandomWalk::SpanningTree(
    int vertices, const Graph& adjacencylist) {
  spanningtree.clear();
  visited = std::vector<int>(vertices, 0);

  std::vector<int> nodes(vertices);
  std::iota(nodes.begin(), nodes.end(), 0);
  shuffle(nodes.begin(), nodes.end(), generator);
  visited[nodes[0]] = 1;
  for (int round = 1, i = 1; i < vertices; ++i) {
    if (visited[nodes[i]]) continue;
    ++round;
    LERW(nodes[i], round, adjacencylist);
  }

  return spanningtree;
}

void LoopErasedRandomWalk::LERW(int vertex, int round,
                                const Graph& adjacencylist) {
  std::vector<int> current;

  while (!visited[vertex]) {
    visited[vertex] = round;
    current.push_back(vertex);
    int nextvertex;
    do {
      nextvertex =
          adjacencylist[vertex]
                       [std::uniform_int_distribution<int>(
                            0, adjacencylist[vertex].size() - 1)(generator)]
                           .first;
    } while (nextvertex < 0);

    if (visited[nextvertex] == round) {
      // Erase the loop
      do {
        vertex = current.back();
        visited[vertex] = 0;
        current.pop_back();
      } while (vertex != nextvertex);
    }

    vertex = nextvertex;
  }
  current.push_back(vertex);
  for (unsigned int i = 0; i + 1 < current.size(); ++i) {
    spanningtree.push_back({current[i], current[i + 1]});
  }
}
