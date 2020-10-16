#include "usermaze.h"
#include <fstream>

UserMaze::UserMaze(std::string filename) : filename_(filename) {}

void UserMaze::InitialiseGraph() {
  Maze::InitialiseGraph();

  std::ifstream in(filename_);
  in >> vertices_;
  startvertex_ = 0;
  endvertex_ = vertices_ - 1;

  adjacencylist_.clear();
  adjacencylist_.resize(vertices_);

  xmin_ = std::numeric_limits<double>::max(), ymin_ = xmin_;
  xmax_ = std::numeric_limits<double>::min(), ymax_ = xmax_;
  int i, j;
  while (in >> i >> j) {
    std::string bordertype;
    in >> bordertype;
    if (bordertype == "Line") {
      double x1, x2, y1, y2;
      in >> x1 >> y1 >> x2 >> y2;
      xmax_ = std::max(xmax_, x2);
      ymax_ = std::max(ymax_, y2);
      xmin_ = std::min(xmin_, x1);
      ymin_ = std::min(ymin_, y1);
      adjacencylist_[i].push_back(
          {j, std::make_shared<LineBorder>(x1, y1, x2, y2)});
    } else if (bordertype == "Arc") {
      double cx, cy, r, theta1, theta2;
      in >> cx >> cy >> r >> theta1 >> theta2;
      xmax_ = std::max(xmax_, cx + r);
      ymax_ = std::max(ymax_, cy + r);
      xmin_ = std::min(xmin_, cx - r);
      ymin_ = std::min(ymin_, cy - r);
      adjacencylist_[i].push_back(
          {j, std::make_shared<ArcBorder>(cx, cy, r, theta1, theta2)});
    } else
      continue;
    if (j != -1)
      adjacencylist_[j].push_back({i, adjacencylist_[i].back().second});
  }
}

std::tuple<double, double, double, double> UserMaze::GetCoordinateBounds()
    const {
  return std::make_tuple(xmin_, ymin_, xmax_, ymax_);
}
