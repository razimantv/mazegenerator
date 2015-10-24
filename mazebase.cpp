/* mazebase.cpp
 * Basic functions for dealing with meshes : implementations
 */

# include <cstdio>
# include <iostream>
# include "mazebase.h"

int initmaze(int H,int W,std::vector<std::vector<char>>& maze)
{
  //Check bounds
  if(H<=0 or H>1000 or W<=0 or W>1000)
  {
    std::cerr<<"Invalid size for maze : ";
    std::cerr<<"Mesh dimensions should be between 1 and 1000\n";
    return 1;
  }

  //Allocate
  maze=std::vector<std::vector<char>>(2*H+1,std::vector<char>(2*W+2,0));

  //"Draw" initial boundary
  for(int i=0;i<=2*H;i++)
  {
    maze[i][2*W+1]=0;

    //The two bounding positions for the row
    maze[i][0]=maze[i][2*W]='|';
    for(int j=1;j<2*W;j++)
    {
      if((i%2)==1)
      {
        //We are in a row corresponding to an actual row in the maze
        if((j%2)==1)
        {
          //We are in an actual cell. So leave it free
          maze[i][j]=' ';
        }
        else
        {
          //We are at a vertical barrier between two cells
          maze[i][j]='|';
        }
      }
      else
      {
        //We are at a horizontal barrier between two cells
        maze[i][j]='-';
      }
    }
  }

  //Remove the barrier from the bottom left and top right
  maze[1][0]=maze[2*H-1][2*W]=' ';

  return 0;
}

void removeedge(int h1,int w1,int h2,int w2,std::vector<std::vector<char>>& maze)
{
  maze[h1+h2+1][w1+w2+1]=' ';
}

void rawprint(std::vector<std::vector<char>>& maze)
{
  for(int i=0;i<(int)maze.size();i++)
  {
    for(int j=0;j<(int)maze[i].size()-1;j++)
    {
      printf("%c",maze[i][j]);
    }
    printf("\n");
  }
}

int plot(int H,int W,std::vector<std::vector<char>>& maze,const char* basename)
{
  //Generate names of gnuplot command file and png file from filename base
  char gnuplotfile[1000],pngfile[1000];
  sprintf(gnuplotfile,"%s.plt",basename);
  sprintf(pngfile,"%s.png",basename);

  //Open the gnuplot command file
  FILE *gfile;
  gfile=fopen(gnuplotfile,"w");
  if(gfile==NULL)
  {
    std::cerr<<"Maze plot error : Unable to open gnuplot command file\n";
    return 1;
  }

  //Set the plot range for gnuplot
  fprintf(gfile,"set xrange[0:%d]\n",W);
  fprintf(gfile,"set yrange[0:%d]\n",H);

  //If there is a vertical barrier between adjacent cells in a row, draw it
  for(int h=0;h<H;h++)
    for(int w=-1;w<W;w++)
      if(maze[2*h+1][2*w+2]=='|')
        fprintf(gfile,"set arrow from %d,%d to %d,%d nohead lt -1 lw 5\n",
                w+1,h,w+1,h+1);

  //If there is a horizontal barrier between adjacent cells in a column, draw it
  for(int h=-1;h<H;h++)
    for(int w=0;w<W;w++)
      if(maze[2*h+2][2*w+1]=='-')
        fprintf(gfile,"set arrow from %d,%d to %d,%d nohead lt -1 lw 5\n",
                w,h+1,w+1,h+1);

  //remove the graph-only artifacts : border and tics
  fprintf(gfile,"unset tics\n");
  fprintf(gfile,"unset border\n");

  //Set the image size based on maze size
  fprintf(gfile,"set term pngcairo size %d,%d\n",30*W,30*H);
  fprintf(gfile,"set output \"%s\"\n",pngfile);

  //Do the actual plotting
  //Gnuplot seems to be very particular that it won't draw any of this unless
  //you ask it to plot something. So plot the zero line
  fprintf(gfile,"plot 0 lc -1 notitle\n");
  fprintf(gfile,"set output\n");
  fclose(gfile);

  char command[1000];
  //The gnuplot command file has been created. Now just run gnuplot on it
  sprintf(command,"gnuplot %s",gnuplotfile);
  system(command);

  //Success (hopefully)
  return 0;
}
