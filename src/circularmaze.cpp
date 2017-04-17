#include "circularmaze.h"
#include <cmath>
#include <iostream>

CircularMaze::CircularMaze(int size) : size_(size) {
  ringnodecount_ = std::vector<int>(size_);
  ringnodeprefixsum_ = std::vector<int>(size_);
  ringnodecount_[0] = 1;
  ringnodeprefixsum_[0] = 0;

  for (int i = 1; i < size_; ++i) {
    ringnodecount_[i] = ringnodecount_[i - 1];
    if (2 * M_PI * i / ringnodecount_[i - 1] > 2) ringnodecount_[i] *= 2;
    ringnodeprefixsum_[i] = ringnodeprefixsum_[i - 1] + ringnodecount_[i - 1];
  }
  vertices_ = ringnodecount_.back() + ringnodeprefixsum_.back();
  startvertex_ = ringnodeprefixsum_.back();
  endvertex_ = startvertex_ + ringnodecount_.back() / 2;
}

void CircularMaze::InitialiseGraph() {
  Maze::InitialiseGraph();

  for (int i = 1; i < size_; ++i) {
    for (int j = 0; j < ringnodecount_[i]; ++j) {
      int node = ringnodeprefixsum_[i] + j, nnode;
      std::shared_ptr<CellBorder> ptr;

      nnode = ringnodeprefixsum_[i - 1] +
              (ringnodecount_[i - 1] * j) / ringnodecount_[i];
      ptr = std::make_shared<ArcBorder>(
          0, 0, i, j * 2 * M_PI / ringnodecount_[i] - M_PI / 2,
          (j + 1) * 2 * M_PI / ringnodecount_[i] - M_PI / 2);
      adjacencylist_[node].push_back({nnode, ptr});
      adjacencylist_[nnode].push_back({node, ptr});

      nnode = ringnodeprefixsum_[i] + ((j + 1) % ringnodecount_[i]);
      double theta = (j + 1) * 2 * M_PI / ringnodecount_[i] - M_PI / 2;
      ptr = std::make_shared<LineBorder>(i * cos(theta), i * sin(theta),
                                         (i + 1) * cos(theta),
                                         (i + 1) * sin(theta));
      adjacencylist_[node].push_back({nnode, ptr});
      adjacencylist_[nnode].push_back({node, ptr});

      if (i == size_ - 1 and node != startvertex_ and node != endvertex_) {
        ptr = std::make_shared<ArcBorder>(
            0, 0, size_, j * 2 * M_PI / ringnodecount_[i] - M_PI / 2,
            (j + 1) * 2 * M_PI / ringnodecount_[i] - M_PI / 2);
        adjacencylist_[node].push_back({-1, ptr});
      }
    }
  }
}

std::tuple<double, double, double, double> CircularMaze::GetCoordinateBounds()
    const {
  return std::make_tuple(-size_, -size_, size_, size_);
}
