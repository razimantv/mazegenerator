/* subdivide.h
 * Generate the maze using recursive subdivision: declarations
 */

#ifndef SUBDIVIDE_H
#define SUBDIVIDE_H

#include <vector>

// Recursive subdivision algorithm to generate a maze with a unique solution
void subdivision_algorithm(int h1, int h2, int w1, int w2, int H, int W,
                           std::vector<std::vector<char>> &maze);

// Create a maze of given size
int subdivision_createmaze(int H, int W, std::vector<std::vector<char>> &maze);
#endif
