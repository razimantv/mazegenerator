/* dfs.cpp
 * Generate the maze using depth-first search: implementations
 */

# include <algorithm>
# include <iostream>
# include <vector>
# include "mazebase.h"
# include "dfs.h"

void DFS_algorithm(int u,int v,int H,int W,std::vector<std::vector<char>>& seen,
    std::vector<std::vector<char>>& maze)
{
  seen[u][v]=1;

  //order to look for neighbours : randomised
  int order[]={0,1,2,3};
  std::random_shuffle(order,order+4);

  for(int i=0;i<4;i++)
  {
    int unext=u+neigh[order[i]][0],vnext=v+neigh[order[i]][1];

    //Check that found vertex is valid and unvisited
    if(unext<0 or unext>=H or vnext<0 or vnext>=W or seen[unext][vnext])
      continue;

    //We move from (u,v) to (unext,vnext) : Remove the barrier between them
    removeedge(u,v,unext,vnext,maze); 

    //Recurse
    DFS_algorithm(unext,vnext,H,W,seen,maze);
  }
}

//Create a maze of given size
int DFS_createmaze(int H,int W,std::vector<std::vector<char>>& maze)
{
  int err;

  err=initmaze(H,W,maze);
  if(err)
  {
    std::cerr<<"Maze creation using DFS failed : ";
    std::cerr<<"Error initializing maze vector\n";
    return 1;
  }

  //Seen array initialised to 0
  std::vector<std::vector<char>> seen(H,std::vector<char>(W,0));
  
  //Randomise the pseudorandom generator with the current time
  srand(unsigned(time(0)));

  //Run the recursice DFS algorithm to make the maze
  DFS_algorithm(rand()%H,rand()%W,H,W,seen,maze);
  return 0;
}
