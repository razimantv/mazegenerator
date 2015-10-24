/* mazebase.h
 * Basic functions for dealing with meshes : declarations
 */

# ifndef MAZEBASE_H
# define MAZEBASE_H

# include <string>
# include <vector>

int initmaze(int H,int W,std::vector<std::vector<char>>& maze);
void removeedge(int h1,int w1,int h2,int w2,std::vector<std::vector<char>>& maze);
void rawprint(std::vector<std::vector<char>>& maze);
int plot(int H,int W,std::vector<std::vector<char>>& maze,const char* basename);

# endif
