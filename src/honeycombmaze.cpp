#include "honeycombmaze.h"
#include <cmath>

const int HoneyCombMaze::neigh[6][2] = {{-1, 0}, {-1, 1}, {0, 1},
                                        {1, 0},  {1, -1}, {0, -1}};

HoneyCombMaze::HoneyCombMaze(int size)
    : Maze(3 * size * (size - 1) + 1, 0, 3 * size * (size - 1)), size_(size) {}

void HoneyCombMaze::InitialiseGraph() {
  Maze::InitialiseGraph();

  for (int u = -size_ + 1; u < size_; ++u) {
    auto vextent = VExtent(u);
    for (int v = vextent.first; v <= vextent.second; ++v) {
      int node = VertexIndex(u, v);
      for (int n = 0; n < 6; ++n) {
        int uu = u + neigh[n][0], vv = v + neigh[n][1];
        if (IsValidNode(uu, vv)) {
          int nnode = VertexIndex(uu, vv);
          if (nnode > node) continue;
          std::shared_ptr<LineBorder> ptr =
              std::make_shared<LineBorder>(GetEdge(u, v, n));
          adjacencylist_[node].push_back({nnode, ptr});
          adjacencylist_[nnode].push_back({node, ptr});
        } else {
          if ((node == startvertex_ and n == 0) or
              (node == endvertex_ and n == 3))
            continue;
          adjacencylist_[node].push_back(
              {-1, std::make_shared<LineBorder>(GetEdge(u, v, n))});
        }
      }
    }
  }
}

// u, v are diretions up and right-down
int HoneyCombMaze::VertexIndex(int u, int v) const {
  if (u <= 0)
    return ((3 * size_ + u) * (size_ + u - 1)) / 2 + v;
  else
    return (3 * size_ * (size_ - 1) + (4 * size_ - u - 1) * u) / 2 + v;
}

std::tuple<double, double, double, double> HoneyCombMaze::GetEdge(
    int u, int v, int edge) const {
  double dxu = sqrt(3) / 2, dyu = 1.5, dxv = sqrt(3), dyv = 0;
  double cx = dxu * u + dxv * v, cy = dyu * u + dyv * v;

  double theta1 = (edge - 2.5) * M_PI / 3, theta2 = theta1 + M_PI / 3;
  return std::make_tuple(cx + cos(theta1), cy + sin(theta1), cx + cos(theta2),
                         cy + sin(theta2));
}

std::tuple<double, double, double, double> HoneyCombMaze::GetCoordinateBounds()
    const {
  double xlim = sqrt(3) * (size_ - 0.5), ylim = 1.5 * size_ - 0.5;
  return std::make_tuple(-xlim, -ylim, xlim, ylim);
}

std::pair<int, int> HoneyCombMaze::VExtent(int u) {
  if (u < 0)
    return {-size_ - u + 1, size_ - 1};
  else
    return {-size_ + 1, size_ - 1 - u};
}

bool HoneyCombMaze::IsValidNode(int u, int v) {
  if (u <= -size_ or u >= size_) return false;
  auto vextent = VExtent(u);
  return v >= vextent.first and v <= vextent.second;
}
