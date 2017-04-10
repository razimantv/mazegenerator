/* subdivide.cpp
 * Generate the maze using recursive subdivision: implementations
 */

#include "subdivide.h"
#include "mazebase.h"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>

// Recursive subdivision algorithm to generate a maze with a unique solution
void subdivision_algorithm(int h1, int h2, int w1, int w2, int H, int W,
                           std::vector<std::vector<char>> &maze) {
  // If the section to subdivide is a row or column, remove all edges in it
  if (h2 - h1 == 1) {
    for (int i = w1; i < w2 - 1; i++) {
      removeedge(h1, i, h1, i + 1, maze);
    }
    return;
  } else if (w2 - w1 == 1) {
    for (int i = h1; i < h2 - 1; i++) {
      removeedge(i, w1, i + 1, w1, maze);
    }
    return;
  }

  // Choose positions to divide
  // To avoid really thin divisions, choose division from centre half
  int h1p = (h1 * 3 + h2) / 4, h2p = h1 + h2 - h1p;
  int hdiv = rand() % (h2p - h1p - 1) + h1p + 1;
  int w1p = (w1 * 3 + w2) / 4, w2p = w1 + w2 - w1p;
  int wdiv = rand() % (w2p - w1p - 1) + w1p + 1;

  // Recurse
  subdivision_algorithm(h1, hdiv, w1, wdiv, H, W, maze);
  subdivision_algorithm(h1, hdiv, wdiv, w2, H, W, maze);
  subdivision_algorithm(hdiv, h2, w1, wdiv, H, W, maze);
  subdivision_algorithm(hdiv, h2, wdiv, w2, H, W, maze);

  // Now choose points of division on the four walls
  std::vector<std::pair<int, int>> edges;
  int r;
  // Top vertical line
  r = rand() % (hdiv - h1) + h1;
  edges.push_back(std::make_pair(r * W + wdiv - 1, r * W + wdiv));
  // Bottom vertical line
  r = rand() % (h2 - hdiv) + hdiv;
  edges.push_back(std::make_pair(r * W + wdiv - 1, r * W + wdiv));
  // Left horizontal line
  r = rand() % (wdiv - w1) + w1;
  edges.push_back(std::make_pair(hdiv * W + r - W, hdiv * W + r));
  // Right horizontal line
  r = rand() % (w2 - wdiv) + wdiv;
  edges.push_back(std::make_pair(hdiv * W + r - W, hdiv * W + r));

  // Now discard one of the four
  swap(edges[3], edges[rand() % 4]);

  // Remove the three non-discarded edges
  for (int i = 0; i < 3; i++) {
    int u1 = edges[i].first / W, v1 = edges[i].first % W;
    int u2 = edges[i].second / W, v2 = edges[i].second % W;
    removeedge(u1, v1, u2, v2, maze);
  }
}

// Create a maze of given size
int subdivision_createmaze(int H, int W, std::vector<std::vector<char>> &maze) {
  int err;

  err = initmaze(H, W, maze);
  if (err) {
    std::cerr << "Maze creation using recursive subdivision failed : ";
    std::cerr << "Error initializing maze vector\n";
    return 1;
  }

  // Randomise the pseudorandom generator with the current time
  srand(unsigned(time(0)));

  // Run the recursice subdivision algorithm to make the maze
  subdivision_algorithm(0, H, 0, W, H, W, maze);
  return 0;
}
