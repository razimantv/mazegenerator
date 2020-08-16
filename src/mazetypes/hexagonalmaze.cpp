#include "hexagonalmaze.h"
#include <cmath>
#include <iostream>

HexagonalMaze::HexagonalMaze(int size) : Maze(6 * size * size), size_(size) {
  startvertex_ = VertexIndex(0, 1, size_ - 1, 0);
  endvertex_ = VertexIndex(3, 1, size_ - 1, 0);
}

void HexagonalMaze::InitialiseGraph() {
  Maze::InitialiseGraph();

  // Hexagon can be split into 6 triangular sectors
  // Each of which is subdivided into size_*size triangles

  for (int sector = 0; sector < 6; ++sector) {
    // Outer boundary, except entry and exit
    for (int i = 0; i < size_; ++i) {
      if ((i > 0) or (sector % 3 != 0)) {
        std::shared_ptr<CellBorder> ptr =
            this->GetEdge(sector, size_ - 1, i, 0);
        adjacencylist_[VertexIndex(sector, 0, size_ - 1, i)].push_back(
            {-1, ptr});
      }
    }

    // Border between the 6 major triangles
    for (int i = 0; i < size_; ++i) {
      std::shared_ptr<CellBorder> ptr = this->GetEdge(sector, i, i, 1);
      adjacencylist_[VertexIndex(sector, 0, i, i)].push_back(
          {VertexIndex((sector + 1) % 6, 0, i, 0), ptr});
      adjacencylist_[VertexIndex((sector + 1) % 6, 0, i, 0)].push_back(
          {VertexIndex(sector, 0, i, i), ptr});
    }

    // 0-type edge
    // Between up vertex (i,j) and down vertex (i,j)
    for (int i = 0; i < size_ - 1; ++i) {
      for (int j = 0; j <= i; ++j) {
        std::shared_ptr<CellBorder> ptr = this->GetEdge(sector, i, j, 0);
        adjacencylist_[VertexIndex(sector, 0, i, j)].push_back(
            {VertexIndex(sector, 1, i, j), ptr});
        adjacencylist_[VertexIndex(sector, 1, i, j)].push_back(
            {VertexIndex(sector, 0, i, j), ptr});
      }
    }

    // 1-type edge
    // Between up vertex (i,j) and down vertex (i-1,j)
    for (int i = 0; i < size_; ++i) {
      for (int j = 0; j < i; ++j) {
        std::shared_ptr<CellBorder> ptr = this->GetEdge(sector, i, j, 1);
        adjacencylist_[VertexIndex(sector, 0, i, j)].push_back(
            {VertexIndex(sector, 1, i - 1, j), ptr});
        adjacencylist_[VertexIndex(sector, 1, i - 1, j)].push_back(
            {VertexIndex(sector, 0, i, j), ptr});
      }
    }

    // 2-type edge
    // Between up vertex (i,j) and down vertex (i-1,j-1)
    for (int i = 0; i < size_; ++i) {
      for (int j = 1; j <= i; ++j) {
        std::shared_ptr<CellBorder> ptr = this->GetEdge(sector, i, j, 2);
        adjacencylist_[VertexIndex(sector, 0, i, j)].push_back(
            {VertexIndex(sector, 1, i - 1, j - 1), ptr});
        adjacencylist_[VertexIndex(sector, 1, i - 1, j - 1)].push_back(
            {VertexIndex(sector, 0, i, j), ptr});
      }
    }
  }
}

std::shared_ptr<CellBorder> HexagonalMaze::GetEdge(int sector, int row,
                                                   int column, int edge) const {
  // Coordinates of vertices of 0th sector
  double x1 = 0, y1 = 0, x2 = -size_ / 2.0, y2 = sqrt(3) * x2, x3 = -x2,
         y3 = y2;
  double dx12 = (x2 - x1) / size_, dy12 = (y2 - y1) / size_,
         dx23 = (x3 - x2) / size_, dy23 = (y3 - y2) / size_;

  double ex1, ey1, ex2, ey2;
  if (edge == 0) {
    // Edge 0 is the bottom edge, hence connecting
    // (row+1,column)-(row+1,column+1)
    ex1 = x1 + dx12 * (row + 1) + dx23 * column;
    ey1 = y1 + dy12 * (row + 1) + dy23 * column;
    ex2 = ex1 + dx23;
    ey2 = ey1 + dy23;
  } else if (edge == 1) {
    // (row,column)-(row+1,colum+1)
    ex1 = x1 + dx12 * row + dx23 * column;
    ey1 = y1 + dy12 * row + dy23 * column;
    ex2 = ex1 + dx12 + dx23;
    ey2 = ey1 + dy12 + dy23;
  } else {
    // (row,column)-(row+1,colum)
    ex1 = x1 + dx12 * row + dx23 * column;
    ey1 = y1 + dy12 * row + dy23 * column;
    ex2 = ex1 + dx12;
    ey2 = ey1 + dy12;
  }

  // Finally rotate to actual sector
  double theta = sector * M_PI / 3, sintheta = sin(theta),
         costheta = cos(theta);
  return std::make_shared<LineBorder>(
      ex1 * costheta - ey1 * sintheta, ex1 * sintheta + ey1 * costheta,
      ex2 * costheta - ey2 * sintheta, ex2 * sintheta + ey2 * costheta);
}

int HexagonalMaze::VertexIndex(int sector, int updown, int row,
                               int column) const {
  int vertexindex = sector * size_ * size_;
  if (updown == 1) vertexindex += (size_ * (size_ + 1)) / 2;
  vertexindex += (row * (row + 1)) / 2 + column;

  return vertexindex;
}

std::tuple<double, double, double, double> HexagonalMaze::GetCoordinateBounds()
    const {
  return std::make_tuple(-size_, -sqrt(3) / 2 * size_, size_,
                         sqrt(3) / 2 * size_);
}
