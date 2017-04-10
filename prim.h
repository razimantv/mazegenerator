/* prim.h
 * Generate the maze based on Prim's algorithm : declarations
 */

#ifndef PRIM_H
#define PRIM_H

#include <vector>

void Prim_pushneighbours(int u, int v, int H, int W,
                         std::vector<std::vector<char>> &seen,
                         std::vector<std::pair<int, int>> &edges);

// Prim's algorithm to generate a maze with a unique solution
void Prim_algorithm(int H, int W, std::vector<std::vector<char>> &maze);

// Create a maze of given size
int Prim_createmaze(int H, int W, std::vector<std::vector<char>> &maze);
#endif
