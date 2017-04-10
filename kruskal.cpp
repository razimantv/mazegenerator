/* kruskal.cpp
 * Generate the maze based on Kruskal's algorithm : implementations
 */

#include "kruskal.h"
#include "mazebase.h"
#include <algorithm>
#include <iostream>

// Initialise the parent for all vertices
void disjointset_initparent(int N, std::vector<int> &parent) {
  // Initially, every vertex is the parent of itself
  for (int i = 0; i < N; i++) {
    parent[i] = i;
  }
}

// Find the parent of a given vertex
int disjointset_getroot(int u, std::vector<int> &parent) {
  // Path compression:
  // If the parent of a vertex is not itself,
  //  set the parent to be the root recursively
  if (parent[u] != u)
    parent[u] = disjointset_getroot(parent[u], parent);

  // Now the parent is guaranteed to be the root : Return it
  return parent[u];
}

// Check whether two vertices are in the same component
bool disjointset_checksamecomponent(int u, int v, std::vector<int> &parent) {
  return disjointset_getroot(u, parent) == disjointset_getroot(v, parent);
}

// Merge two vertices
void disjointset_merge(int u, int v, std::vector<int> &parent) {
  // Set the parent of root of one vertex to be the parent of the root of the
  // other
  parent[disjointset_getroot(u, parent)] = disjointset_getroot(v, parent);
}

int Kruskal_initialize(int H, int W, std::vector<int> &parent,
                       std::vector<std::pair<int, int>> &Kruskalarray) {
  // Initialise the disjoint set datastructure
  parent.resize(H * W);
  disjointset_initparent(H * W, parent);

  // Add all the horizontal edges to the edge array
  for (int h = 0; h < H; h++)
    for (int w = 0; w < W - 1; w++)
      Kruskalarray.push_back(std::make_pair(h * W + w, h * W + w + 1));

  // Add all the vertical edges to the edge array
  for (int h = 0; h < H - 1; h++)
    for (int w = 0; w < W; w++)
      Kruskalarray.push_back(std::make_pair(h * W + w, h * W + w + W));

  return 0;
}

// Kruskal's algorithm to generate a maze with a unique solution
int Kruskal_algorithm(int H, int W, std::vector<int> &parent,
                      std::vector<std::pair<int, int>> &Kruskalarray,
                      std::vector<std::vector<char>> &maze) {
  // Randomise the pseudorandom generator with the current time
  // Required since edge weights for Kruskal have been randomised
  srand(unsigned(time(0)));

  // Here be randomness
  random_shuffle(Kruskalarray.begin(), Kruskalarray.end());

  // Standard Kruskal's algorithm implementation
  // Remove as many edges as we can without introducing cycles
  for (int i = 0; i < (int)Kruskalarray.size(); i++) {
    int u = Kruskalarray[i].first, v = Kruskalarray[i].second;

    // If the two vertices are in the same component, do not add an edge
    if (disjointset_checksamecomponent(u, v, parent))
      continue;

    // Vertices are in different components. Remove the barrier between them
    int hu = u / W, wu = u % W;
    int hv = v / W, wv = v % W;
    removeedge(hu, wu, hv, wv, maze);

    // Merge the components
    disjointset_merge(u, v, parent);
  }

  return 0;
}

int Kruskal_createmaze(int H, int W, std::vector<std::vector<char>> &maze) {
  int err;

  err = initmaze(H, W, maze);
  if (err) {
    std::cerr << "Maze creation using Kruskal failed : ";
    std::cerr << "Error initializing maze vector\n";
    return 1;
  }

  // Initialize the various arrays
  std::vector<int> parent;
  std::vector<std::pair<int, int>> Kruskalarray;
  err = Kruskal_initialize(H, W, parent, Kruskalarray);

  // Create the maze using Kruskal's algorithm
  Kruskal_algorithm(H, W, parent, Kruskalarray, maze);
  return 0;
}

