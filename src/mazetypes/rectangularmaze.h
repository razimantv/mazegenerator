#include "maze.h"

class RectangularMaze : public Maze {
 public:
  RectangularMaze(int, int);
  virtual void InitialiseGraph();

 private:
  int width_, height_;

  int VertexIndex(int, int);
  virtual std::tuple<double, double, double, double> GetCoordinateBounds()
      const;
};
