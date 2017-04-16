#ifndef HEXAGONALMAZE_H
#define HEXAGONALMAZE_H

#include "maze.h"

class HexagonalMaze : public Maze {
 public:
  HexagonalMaze(int);
  virtual void InitialiseGraph();

 protected:
  int size_;

  virtual std::shared_ptr<CellBorder> GetEdge(int, int, int, int) const;
  int VertexIndex(int, int, int, int) const;
  std::tuple<double, double, double, double> GetCoordinateBounds() const;
};

#endif /* end of include guard: HEXAGONALMAZE_H */
