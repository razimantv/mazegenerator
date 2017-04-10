/* kruskal.h
 * Generate the maze based on Kruskal's algorithm : declarations
 */

#ifndef KRUSKAL_H
#define KRUSKAL_H

#include <utility>
#include <vector>

// Disjoint set functions
// Initialise the parent for all vertices
void disjointset_initparent(int N, std::vector<int> &parent);
// Find the parent of a given vertex
int disjointset_getroot(int u, std::vector<int> &parent);
// Check whether two vertices are in the same component
bool disjointset_checksamecomponent(int u, int v, std::vector<int> &parent);
// Merge two vertices
void disjointset_merge(int u, int v, std::vector<int> &parent);

// Helper functions
// Initialize parent and edge vectors
int Kruskal_initialize(int H, int W, std::vector<int> &parent,
                       std::vector<std::pair<int, int>> &Kruskalarray);
// Kruskal's algorithm to generate a maze with a unique solution
int Kruskal_algorithm(int H, int W, std::vector<int> &parent,
                      std::vector<std::pair<int, int>> &Kruskalarray,
                      std::vector<std::vector<char>> &maze);

// Create a maze of given size
int Kruskal_createmaze(int H, int W, std::vector<std::vector<char>> &maze);
#endif
