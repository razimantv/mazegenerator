#ifndef MAZETYPES_TRIANGULARMAZE_H_
#define MAZETYPES_TRIANGULARMAZE_H_

#include "../maze/maze.h"

class TriangularMaze : public Maze {
 private:
  int rows_;  // Number of rows in the triangle
  
  // Get vertex index for a cell at (row, col)
  int VertexIndex(int row, int col) const;
  
  // Get number of cells in a given row
  int CellsInRow(int row) const;
  
  // Get total number of cells up to (but not including) a given row
  int CellsBeforeRow(int row) const;

 public:
  TriangularMaze(int rows);
  void InitialiseGraph() override;
  std::tuple<double, double, double, double> GetCoordinateBounds() const override;
};

#endif  // MAZETYPES_TRIANGULARMAZE_H_