#include "triangularmaze.h"

TriangularMaze::TriangularMaze(int rows) : rows_(rows) {
  // Calculate total vertices: row 0 has rows cells, row 1 has rows-1 cells, etc.
  // Total = rows + (rows-1) + ... + 1 = rows*(rows+1)/2
  vertices_ = rows * (rows + 1) / 2;
  adjacencylist_.resize(vertices_);
  
  // Start at top-left, end at bottom-right
  startvertex_ = 0;
  endvertex_ = vertices_ - 1;
}

int TriangularMaze::CellsInRow(int row) const {
  return rows_ - row;  // Top row has most cells, bottom row has 1 cell
}

int TriangularMaze::CellsBeforeRow(int row) const {
  // Sum of (rows) + (rows-1) + ... + (rows-row+1)
  int total = 0;
  for (int r = 0; r < row; r++) {
    total += CellsInRow(r);
  }
  return total;
}

int TriangularMaze::VertexIndex(int row, int col) const {
  return CellsBeforeRow(row) + col;
}

void TriangularMaze::InitialiseGraph() {
  Maze::InitialiseGraph();

  // Add boundary walls - exactly like rectangular maze but adjust for triangle
  
  // Top boundary
  for (int i = 0; i < CellsInRow(0); ++i) {
    adjacencylist_[VertexIndex(0, i)].push_back(
        {-1, std::make_shared<LineBorder>(i, 0, i + 1, 0)});
  }
  
  // Bottom boundary (only 1 cell in bottom row)
  adjacencylist_[VertexIndex(rows_ - 1, 0)].push_back(
      {-1, std::make_shared<LineBorder>(0, rows_, 1, rows_)});

  // Left and right boundaries
  for (int row = 0; row < rows_; ++row) {
    int cells_in_row = CellsInRow(row);
    
    // Left boundary (except for entry)
    if (row != 0) {
      adjacencylist_[VertexIndex(row, 0)].push_back(
          {-1, std::make_shared<LineBorder>(0, row, 0, row + 1)});
    }
    
    // Right boundary (except for exit)
    if (!(row == rows_ - 1 && cells_in_row == 1)) {
      adjacencylist_[VertexIndex(row, cells_in_row - 1)].push_back(
          {-1, std::make_shared<LineBorder>(cells_in_row, row, cells_in_row, row + 1)});
    }
  }
  
  // Add diagonal boundary walls on the right edge
  for (int row = 0; row < rows_ - 1; ++row) {
    int cells_in_row = CellsInRow(row);
    int cells_in_next_row = CellsInRow(row + 1);
    
    // Add diagonal wall from rightmost cell of current row to rightmost cell of next row
    if (!(row == rows_ - 2 && cells_in_next_row == 1)) { // except for exit
      adjacencylist_[VertexIndex(row, cells_in_row - 1)].push_back(
          {-1, std::make_shared<LineBorder>(cells_in_row, row + 1, cells_in_next_row, row + 1)});
    }
  }

  // Horizontal connections within each row
  for (int row = 0; row < rows_; ++row) {
    int cells_in_row = CellsInRow(row);
    for (int col = 0; col < cells_in_row - 1; ++col) {
      std::shared_ptr<LineBorder> ptr =
          std::make_shared<LineBorder>(col + 1, row, col + 1, row + 1);
      adjacencylist_[VertexIndex(row, col)].push_back({VertexIndex(row, col + 1), ptr});
      adjacencylist_[VertexIndex(row, col + 1)].push_back({VertexIndex(row, col), ptr});
    }
  }

  // Vertical connections between rows
  for (int row = 0; row < rows_ - 1; ++row) {
    int cells_in_next_row = CellsInRow(row + 1);
    
    for (int col = 0; col < cells_in_next_row; ++col) {
      std::shared_ptr<LineBorder> ptr =
          std::make_shared<LineBorder>(col, row + 1, col + 1, row + 1);
      adjacencylist_[VertexIndex(row, col)].push_back({VertexIndex(row + 1, col), ptr});
      adjacencylist_[VertexIndex(row + 1, col)].push_back({VertexIndex(row, col), ptr});
    }
  }
}

std::tuple<double, double, double, double> TriangularMaze::GetCoordinateBounds() const {
  return std::make_tuple(0, 0, rows_, rows_);
}