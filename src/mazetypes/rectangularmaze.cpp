#include "rectangularmaze.h"

RectangularMaze::RectangularMaze(int width, int height)
    : Maze(width * height, 0, width * height - 1),
      width_(width),
      height_(height) {}

int RectangularMaze::VertexIndex(int row, int column) {
  return row * width_ + column;
}

void RectangularMaze::InitialiseGraph() {
  Maze::InitialiseGraph();

  // Lower and upper boundaries
  for (int i = 0; i < width_; ++i) {
    adjacencylist_[VertexIndex(0, i)].push_back(
        {-1, std::make_shared<LineBorder>(i, 0, i + 1, 0)});
    adjacencylist_[VertexIndex(height_ - 1, i)].push_back(
        {-1, std::make_shared<LineBorder>(i, height_, i + 1, height_)});
  }

  // Left and right boundaries, leaving space for entry and exit
  for (int i = 0; i < height_; ++i) {
    if (i != 0)
      adjacencylist_[VertexIndex(i, 0)].push_back(
          {-1, std::make_shared<LineBorder>(0, i, 0, i + 1)});
    if (i != height_ - 1)
      adjacencylist_[VertexIndex(i, 0)].push_back(
          {-1, std::make_shared<LineBorder>(width_, i, width_, i + 1)});
  }

  // Horizontally adjacent cells
  for (int i = 0; i < height_; ++i) {
    for (int j = 0; j < width_ - 1; ++j) {
      std::shared_ptr<LineBorder> ptr =
          std::make_shared<LineBorder>(j + 1, i, j + 1, i + 1);
      adjacencylist_[VertexIndex(i, j)].push_back({VertexIndex(i, j + 1), ptr});
      adjacencylist_[VertexIndex(i, j + 1)].push_back({VertexIndex(i, j), ptr});
    }
  }

  // Vertically adjacent cells
  for (int i = 0; i < height_ - 1; ++i) {
    for (int j = 0; j < width_; ++j) {
      std::shared_ptr<LineBorder> ptr =
          std::make_shared<LineBorder>(j, i + 1, j + 1, i + 1);
      adjacencylist_[VertexIndex(i, j)].push_back({VertexIndex(i + 1, j), ptr});
      adjacencylist_[VertexIndex(i + 1, j)].push_back({VertexIndex(i, j), ptr});
    }
  }
}

std::tuple<double, double, double, double>
RectangularMaze::GetCoordinateBounds() const {
  return std::make_tuple(0, 0, width_, height_);
}
