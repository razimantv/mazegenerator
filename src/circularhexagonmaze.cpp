#include "circularhexagonmaze.h"
#include <cmath>
#include <iostream>

CircularHexagonMaze::CircularHexagonMaze(int size) : HexagonalMaze(size) {}

std::shared_ptr<CellBorder> CircularHexagonMaze::GetEdge(int sector, int row,
                                                         int column,
                                                         int edge) const {
  if (edge == 0) {
    // Edge 0 is the bottom edge, hence connecting
    // (row+1,column)-(row+1,column+1) with an arc
    return std::make_shared<ArcBorder>(
        0, 0, row + 1, (sector - 2) * M_PI / 3 + column * M_PI / 3 / (row + 1),
        (sector - 2) * M_PI / 3 + (column + 1) * M_PI / 3 / (row + 1));
  }

  double ex1, ey1, ex2, ey2;
  if (edge == 1) {
    // (row,column)-(row+1,colum+1)
    double theta1 = (sector - 2) * M_PI / 3, theta2 = (sector - 2) * M_PI / 3;
    if (row > 0) theta1 += column * M_PI / 3 / row;
    theta2 += (column + 1) * M_PI / 3 / (row + 1);

    ex1 = row * cos(theta1);
    ey1 = row * sin(theta1);
    ex2 = (row + 1) * cos(theta2);
    ey2 = (row + 1) * sin(theta2);
  } else {
    // (row,column)-(row+1,colum)
    double theta1 = (sector - 2) * M_PI / 3, theta2 = (sector - 2) * M_PI / 3;
    if (row > 0) theta1 += column * M_PI / 3 / row;
    theta2 += column * M_PI / 3 / (row + 1);

    ex1 = row * cos(theta1);
    ey1 = row * sin(theta1);
    ex2 = (row + 1) * cos(theta2);
    ey2 = (row + 1) * sin(theta2);
  }
  return std::make_shared<LineBorder>(ex1, ey1, ex2, ey2);
}

std::tuple<double, double, double, double>
CircularHexagonMaze::GetCoordinateBounds() const {
  return std::make_tuple(-size_, -size_, size_, size_);
}
