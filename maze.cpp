/*  solar.cpp: Generates a maze of arbitrary size using Kruskal's algorithm
 *
 *  Note: The program uses gnuplot to generate the graphical mesh
 *        Ensure that gnuplot has been installed and is part of the path before
 *        running the program.
 *
 *  Author: Raziman T V
 *
 *  License:
 *  You may use this document for whatever purpose you desire, as long as the 
 *  following conditions are satisfied:
 *  1) You do not use it in a way that suggests that I endorse the use.
 *  2) If you redistribute this code, I would be grateful if you credit me as 
 *     the author of the code. This is not a requirement.
 *  3) If this code is modified and redistributed with credit to me as the
 *     original author, the code should either contain a link to the source
 *     where the unaltered code can be found, or state explicitly that the code
 *     has been altered significantly since.
 */

# include <cstdio>
# include <cassert>
# include <cstdlib>
# include <cstring>
# include <vector>
# include <algorithm>
# include <ctime>

using namespace std;

//Height and width of the grid. Read from stdin
int H,W;

//Filenames of the gnuplot command file and the output jpg file
//The former is read from stdin. Appending ".jpg" gives the latter
char gnuplotfile[100],jpgfile[100];

//Used to store the system command strings before calling
char command[100];

//Store the maze in text format
//Used only for final drawing currently
char **board;

//Function to map (x,y) coordinate to integer in range [0,H*W)
int vertex(int h,int w)
{
  return h*W+w;
}

//Disjoint set data structure
//Details : http://en.wikipedia.org/wiki/Disjoint-set_data_structure
int *parent;

//Find the current component in the disjoint set
int getparent(int u)
{
  return (parent[u]==u)?u:parent[u]=getparent(parent[u]);
}

//Array to store all edges between cells in the board
//Kruskal is done based on the order in this array
vector<int> kruskalarray;

int main()
{
  //Randomise the pseudorandom generator with the current time
  //Required since edge weights for Kruskal have been randomised
  srand(unsigned(time(0)));

  //Read board dimensions and sanitize
  scanf("%d %d",&H,&W);
  assert(H>0);
  assert(W>0);

  //Read the filename for the gnuplot command file
  //and generate the jpg filename from it
  scanf("%s",gnuplotfile);
  assert(strlen(gnuplotfile)<50);
  strcpy(jpgfile,gnuplotfile);
  strcat(jpgfile,".jpg");

  //Allocate memory to the board
  board=new char*[2*H+1];
  assert(board!=NULL);
  for(int i=0;i<=2*H;i++)
  {
    board[i]=new char[2*W+2];
    assert(board[i]!=NULL);
  }

  for(int i=0;i<=2*H;i++)
  {
    board[i][2*W+1]=0;

    //The two bounding positions for the row
    board[i][0]=board[i][2*W]='|';
    for(int j=1;j<2*W;j++)
    {
      if((i%2)==1)
      {
        //We are in a row corresponding to an actual row in the maze
        if((j%2)==1)
        {
          //We are in an actual cell. So leave it free
          board[i][j]=' ';
        }
        else
        {
          //We are at a vertical barrier between two cells
          board[i][j]='|';
        }
      }
      else
      {
        //We are at a horizontal barrier between two cells
        board[i][j]='-';
      }
    }
  }

  //Remove the barrier from the bottom left and top right
  board[1][0]=board[2*H-1][2*W]=' ';

  //Initialise the disjoint set datastructure
  //Every vertex in its own component
  parent=new int[H*W];
  assert(parent!=NULL);
  for(int u=0;u<H*W;u++)
    parent[u]=u;

  //Add all the horizontal edges to the edge array
  for(int h=0;h<H;h++)
    for(int w=0;w<W-1;w++)
      kruskalarray.push_back(vertex(h,w)*H*W+vertex(h,w+1));

  //Add all the vertical edges to the edge array
  for(int h=0;h<H-1;h++)
    for(int w=0;w<W;w++)
      kruskalarray.push_back(vertex(h,w)*H*W+vertex(h+1,w));

  //Here be randomness
  random_shuffle(kruskalarray.begin(),kruskalarray.end());

  //Standard Kruskal's algorithm implementation
  //Remove as many edges as we can without introducing cycles
  for(int i=0;i<kruskalarray.size();i++)
  {
    int u=kruskalarray[i]/(H*W),v=kruskalarray[i]%(H*W);
    if(getparent(u)==getparent(v))
    {
      //The two vertices are in the same component. Do not add an edge
      continue;
    }

    //Vertices are in different components. Remove the barrier between them
    int hu=u/W,wu=u%W;
    int hv=v/W,wv=v%W;
    board[hu+hv+1][wu+wv+1]=' ';

    //Merge the components
    parent[getparent(u)]=getparent(v);
  }

  //Maze generation is done. Now to draw it

  //Open the gnuplot command file
  FILE *gfile;
  gfile=fopen(gnuplotfile,"w");
  assert(gfile!=NULL);

  //Set the plot range for gnuplot
  fprintf(gfile,"set xrange[0:%d]\n",W);
  fprintf(gfile,"set yrange[0:%d]\n",H);

  //If there is a vertical barrier between adjacent cells in a row, draw it
  for(int h=0;h<H;h++)
    for(int w=-1;w<W;w++)
      if(board[2*h+1][2*w+2]=='|')
        fprintf(gfile,"set arrow from %d,%d to %d,%d nohead lt -1 lw 5\n",
                w+1,h,w+1,h+1);

  //If there is a horizontal barrier between adjacent cells in a column, draw it
  for(int h=-1;h<H;h++)
    for(int w=0;w<W;w++)
      if(board[2*h+2][2*w+1]=='-')
        fprintf(gfile,"set arrow from %d,%d to %d,%d nohead lt -1 lw 5\n",
                w,h+1,w+1,h+1);

  //remove the graph-only artifacts : border and tics
  fprintf(gfile,"unset tics\n");
  fprintf(gfile,"unset border\n");

  //Set the image size based on maze size
  fprintf(gfile,"set term jpeg size %d,%d\n",30*W,30*H);
  fprintf(gfile,"set output \"%s\"\n",jpgfile);

  //Do the actual plotting
  //Gnuplot seems to be very particular that it won't draw any of this unless
  //you ask it to plot something. So plot the zero line
  fprintf(gfile,"plot 0 lc -1 notitle\n");
  fprintf(gfile,"set output\n");
  fclose(gfile);

  //The gnuplot command file has been created. Now just run gnuplot on it
  sprintf(command,"gnuplot %s",gnuplotfile);
  system(command);

  //Success (hopefully)
  return 0;
}
