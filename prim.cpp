/* prim.cpp
 * Generate the maze based on Prim's algorithm : implementations
 */

# include <algorithm>
# include <iostream>
# include <utility>
# include <vector>
# include "mazebase.h"
# include "prim.h"

void Prim_pushneighbours(int u, int v,int H,int W,
    std::vector<std::vector<char>>& seen,
    std::vector<std::pair<int,int>>& edges)
{
  //Loop over neighbours
  for(int i=0;i<4;i++)
  {
    int unext=u+neigh[i][0],vnext=v+neigh[i][1];

    //Check that found vertex is valid and unvisited
    if(unext<0 or unext>=H or vnext<0 or vnext>=W or seen[unext][vnext])
      continue;

    //Add the edge between you and the neighbour to the list
    edges.push_back(std::make_pair(u*W+v,unext*W+vnext));
  }
}

void Prim_algorithm(int H,int W,std::vector<std::vector<char>>& maze)
{
  std::vector<std::vector<char>> seen(H,std::vector<char>(W,0));
  std::vector<std::pair<int,int>> edges;

  //Randomise the pseudorandom generator with the current time
  srand(unsigned(time(0)));

  int u=rand()%H,v=rand()%W;
  seen[u][v]=1;
  Prim_pushneighbours(u,v,H,W,seen,edges);

  while(!edges.empty())
  {
    //Pick a random edge;
    int L=edges.size();
    int cur=rand()%L;
    if(cur!=L-1)swap(edges[L-1],edges[cur]);
    auto edge=edges[L-1];
    edges.pop_back();

    //Check whether the vertex has been visited through some other route
    u=edge.second/W,v=edge.second%W;
    if(seen[u][v])continue;

    //mark (u,v) as seen and remove the barrier to (uneigh,vneigh)
    seen[u][v]=1;
    int uneigh=edge.first/W,vneigh=edge.first%W;
    removeedge(u,v,uneigh,vneigh,maze);

    //Add the edges of (u,v) to the edge list
    Prim_pushneighbours(u,v,H,W,seen,edges);
  }
}

//Create a maze of given size
int Prim_createmaze(int H,int W,std::vector<std::vector<char>>& maze)
{
  int err;

  err=initmaze(H,W,maze);
  if(err)
  {
    std::cerr<<"Maze creation using Prim's algorithm failed : ";
    std::cerr<<"Error initializing maze vector\n";
    return 1;
  }

  //Run the algorithm to make the maze
  Prim_algorithm(H,W,maze);
  return 0;
}

