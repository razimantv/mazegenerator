#include "maze.h"

class UserMaze : public Maze {
 public:
  UserMaze(std::string);
  virtual void InitialiseGraph();

 private:
  double xmin_, ymin_, xmax_, ymax_;
  std::string filename_;

  virtual std::tuple<double, double, double, double> GetCoordinateBounds()
      const;
};
