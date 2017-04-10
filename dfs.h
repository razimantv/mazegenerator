/* dfs.h
 * Generate the maze using depth-first search: declarations
 */

#ifndef DFS_H
#define DFS_H

#include <vector>

// DFS algorithm to generate a maze with a unique solution
void DFS_algorithm(int u, int v, int H, int W,
                   std::vector<std::vector<char>> &seen,
                   std::vector<std::vector<char>> &maze);

// Create a maze of given size
int DFS_createmaze(int H, int W, std::vector<std::vector<char>> &maze);
#endif
