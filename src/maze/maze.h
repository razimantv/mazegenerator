#ifndef MAZE_H
#define MAZE_H

#include "cellborder.h"
#include "spanningtreealgorithm.h"
#include <memory>
#include <vector>

class Maze {
 public:
  Maze(int = 0, int = 0, int = 1);
  void GenerateMaze(SpanningtreeAlgorithm*);
  void PrintMazeGnuplot(const std::string&) const;
  void PrintMazeSVG(const std::string&) const;
  void RemoveBorders(const std::vector<std::pair<int, int>>&);
  virtual void InitialiseGraph() = 0;

 protected:
  // Solving a maze is equivalent to finding a path in a graph
  int vertices_;
  std::vector<std::vector<std::pair<int, std::shared_ptr<CellBorder>>>>
      adjacencylist_;
  int startvertex_, endvertex_;

  virtual std::tuple<double, double, double, double> GetCoordinateBounds()
      const = 0;
};

#endif /* end of include guard: MAZE_H */
