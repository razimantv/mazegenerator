#ifndef CIRCULARHEXAGONMAZE_H
#define CIRCULARHEXAGONMAZE_H

#include "hexagonalmaze.h"

class CircularHexagonMaze : public HexagonalMaze {
 public:
  CircularHexagonMaze(int);

 protected:
  virtual std::shared_ptr<CellBorder> GetEdge(int, int, int, int) const;
  virtual std::tuple<double, double, double, double> GetCoordinateBounds()
      const;
};

#endif /* end of include guard: CIRCULARHEXAGONMAZE_H */
